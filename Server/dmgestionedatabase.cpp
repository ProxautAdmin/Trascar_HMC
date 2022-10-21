// ---------------------------------------------------------------------------

#pragma hdrstop

#include "dmgestionedatabase.h"
#include "dmFunzioniComuniClientServer.h"
#include "ExtraFunction.h"
#include "clientdata.h"
#include "main.h"
#include "percorsiprototipo.h"
#include "socket.h"
#include "DBImpianto.h"
#include "datamodulecomandiagv.h"
#include "PLCphoenixThread.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"

TdmDBServer *dmDBServer;

extern int indice_inizio_campi[NCAMPI];
extern DATILGV tab;
extern CLIENTSOCKET *cs;

// ---------------------------------------------------------------------------
__fastcall TdmDBServer::TdmDBServer(TComponent* Owner) : TDataModule(Owner) {
    minuto_prec = 0;
    letto_data_peso_campione = 0;
}
// ---------------------------------------------------------------------------

void __fastcall TdmDBServer::DataModuleCreate(TObject * Sender) {
    TimerMissioniDB->Enabled = true;

}

// ---------------------------------------------------------------------------

void __fastcall TdmDBServer::TimerMissioniDBTimer(TObject * Sender) {
    //
    if ((MainForm->count_inizio < 21) || (MainForm->close_program))
        return;
    TimerMissioniDB->Enabled = false;

    dmDBImpianto->AnnullaMissione(); // fallo prima se è stato 0 lo setto a 10 e se è stato
    if (ClientData.ParametriFunzionali.AbilitaMissioni) {
        if (dmDB->ContaMissioniAttive(0) < ClientData.ParametriFunzionali.NumMissioniLimite) {
            // genera missioni da cm
            RicercaNuovaMissionedaGenerare(0);
        }
    }

    // Missioni automatiche
    GeneraCMDaGaA1(0);
    GeneraCMDaHaB(0);
    // GeneraCMDaIaA2(0);
    // giorno /notte
    if (ClientData.ParametriFunzionali.Giorno == 0) {
        GeneraCMDaPrelievo(0); // missione da prelievo C
    }
    else {
        // GeneraCMDaGaA1(0); // ga G a A1
    }

    TimerMissioniDB->Enabled = true;

}
// ---------------------------------------------------------------------------

int TdmDBServer::RicercaNuovaMissionedaGenerare(int stato, int cod_tipo_mov) {
    TMissione m;
    AnsiString strsql, strresult, CorsiaDeposito, stringa_hex;
    int tipo_mis, priorita, agv, tipologia, PrelieviRimasti;
    bool trovato, posplc;
    int IDPlc, idx, idx_plc;
    int dbposprel, dbposdep, dbpianoprel, dbpianodep;
    AnsiString ZonaPrel, ZonaDep;
    TADOQuery *ADOQuery;
    TUDC UDCMissione;
    int i, ok_genera = 0, idudc, idudc2, res = 0, piano_dep = 0;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql = "SELECT * from centromissioni where Stato=0 ";
        strsql = strsql + " order by priorita, Generata ";
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        // FA CAGARE E NON FUNZIONA CON PIU' AGV  MA PER ORA VA BENE COSI'
        if (ADOQuery->RecordCount) {
            // per limitare la generazione delle missioni un ciclo alla volta, controllo che ok_genera=0, in questo modo scorre i record finche'
            // o sono finiti o ne ha trovato uno buono, altrimenti potrebbe fermarsi a generare anche se potrebbe proseguire
            while ((!ADOQuery->Eof) && (ok_genera == 0)) {
                ok_genera = 0;
                m.idgestionale = 0;
                m.idudc2 = 0;
                m.Lotto = 0;
                m.nodopassaggioprel = 0;
                m.nodopassaggiodep = 0;
                m.Agv = 1;
                m.tipoudc = 0;
                //
                // m.altezza_udc = ADOQuery->FieldByName("Height")->AsInteger;
                // m.tipoudc = ADOQuery->FieldByName("TipoUDC")->AsInteger;
                m.priorita = ADOQuery->FieldByName("Priorita")->AsInteger;
                m.idudc = ADOQuery->FieldByName("IdUdc")->AsInteger;

                m.idcentromissioni = ADOQuery->FieldByName("id")->AsInteger;
                m.Agv = ADOQuery->FieldByName("AGV")->AsInteger;
                m.tipologia_mis = ADOQuery->FieldByName("TipoMissione")->AsInteger;
                m.tipo_mis = m.tipologia_mis;
                m.tipologia_mov = m.tipo_mis;
                // prelievo
                m.piano_prel = 0;
                m.h_prel = 0;
                m.posprel = 0;
                idx = 1;
                trovato = false;
                posplc = false;
                dbposprel = ADOQuery->FieldByName("posprel")->AsInteger;
                dbpianoprel = ADOQuery->FieldByName("pianoprel")->AsInteger;
                dbposdep = ADOQuery->FieldByName("posdep")->AsInteger;
                dbpianodep = ADOQuery->FieldByName("pianodep")->AsInteger;
                ZonaPrel = ADOQuery->FieldByName("ZonaPrelievo")->AsString.Trim();
                ZonaDep = ADOQuery->FieldByName("ZonaDeposito")->AsString.Trim();

                // vedo se devo calcolare ora prel/dep o vanno gia' bene
                if (dbposprel == 0) {
                    if (ZonaPrel == "G") {
                        dmDBImpianto->PrelievoVuoti(7, dbposprel, dbpianoprel);
                        // m.corsia_dep = dmDB->FilaPosizione(m.posdep);

                    }
                    else if (ZonaPrel == "I") {
                        dbposprel = dmDBImpianto->CercaPrelievo("I", TIPOLOGIA_MATERIEPRIME);
                        dbpianoprel = 1;
                    }
                }

                if (dbposdep == 0) {
                    if (ZonaDep == "D") {
                        dmDBImpianto->TornaPosDepLibera("D", dbposdep, dbpianodep);
                    }
                    else if (ZonaDep == "J") {
                        dmDBImpianto->TornaPosDepLibera("J", dbposdep, dbpianodep, TIPOLOGIA_MATERIEPRIME);
                    }

                }

                // controllo che le missioni alle postazioni prelievo siano praticabili
                while ((idx <= NUM_PRELIEVOPLC) && (trovato == false)) {
                    idx_plc = IDX_PLCPRELIEVO; // metti che ci siano altri indici cosi' lo tratto come voglio
                    if (ClientData.Plc[idx_plc].Prelievo[idx][1].pos == dbposprel) {
                        if ((ClientData.Plc[idx_plc].Prelievo[idx][1].Ready) && (!ClientData.Plc[idx_plc].Prelievo[idx][1].InAllarme) && (ClientData.Plc[idx_plc].Prelievo[idx][1].ProntaAlPrelievo)) {
                            m.posprel = dbposprel;
                            m.piano_prel = dbpianoprel;
                            m.h_prel = dmDB->RitornaAltezzedaPosizione(m.posprel, m.piano_prel, "HPREL");
                            m.corsia_prel = dmDB->FilaPosizione(m.posprel);
                            trovato = true;
                        }
                        posplc = true;
                    }
                    idx++;
                }

                // se non sono posizioni plc E non sono praticabili
                if ((m.posprel == 0) && (posplc == false)) {
                    m.posprel = dbposprel;
                    m.piano_prel = dbpianoprel;
                    if (ZonaPrel == "G")
                        m.h_prel = dmExtraFunction->Constrain(ClientData.ParametriFunzionali.altezza_pallet * (m.piano_prel + 1), 70, 1500);
                    else
                        m.h_prel = dmDB->RitornaAltezzedaPosizione(m.posprel, m.piano_prel, "HPREL");

                    m.corsia_prel = dmDB->FilaPosizione(m.posprel);
                }

                if ((m.posprel > 0) && (m.piano_prel > 0) && (m.h_prel > 0)) {
                    if (dmDB->ControllaPosizioneOK(m.posprel, m.piano_prel) > 0) {
                        // deposito
                        m.posdep = 0;
                        m.piano_dep = 0;
                        m.h_dep = 0;
                        idx = 1;
                        trovato = false;
                        posplc = false;
                        // controllo che le missioni alle postazioni deposito siano praticabili
                        while ((idx <= NUM_DEPOSITOPLC) && (trovato == false)) {
                            idx_plc = IDX_PLCDEPOSITO; // metti che ci siano altri indici cosi' lo tratto come voglio
                            if (ClientData.Plc[idx_plc].Deposito[idx][1].pos == dbposdep) {
                                if ((ClientData.Plc[idx_plc].Deposito[idx][1].Ready) && (!ClientData.Plc[idx_plc].Deposito[idx][1].InAllarme) && (ClientData.Plc[idx_plc].Deposito[idx][1].ProntaAlDeposito)) {
                                    m.posdep = dbposdep;
                                    m.piano_dep = dbpianodep;
                                    m.h_dep = dmDB->RitornaAltezzedaPosizione(m.posdep, m.piano_dep, "HDEP");
                                    m.corsia_dep = dmDB->FilaPosizione(m.posdep);
                                    trovato = true;
                                }
                                posplc = true;
                            }
                            idx++;
                        }

                        // se non sono posizioni plc E non sono praticabili OPPURE e' una missione manuale
                        if ((m.posdep == 0) && (posplc == false)) {
                            m.posdep = dbposdep;
                            m.piano_dep = dbpianodep; ;
                            m.h_dep = max(dmDB->RitornaAltezzedaPosizione(m.posdep, m.piano_dep, "HDEP"), ALTEZZA_PRELIEVO);
                            m.corsia_dep = dmDB->FilaPosizione(m.posdep);
                        }

                        if ((m.posdep > 0) && (m.piano_dep > 0) && (m.h_dep > 0)) {
                            if (dmDB->ControllaPosizioneOK(m.posdep, m.piano_dep) == 1) {
                                res = GeneraMissione(m);
                                if (res > 0) {
                                    dmDBServer->AggiornaStatoCentroMissioni(m.idcentromissioni, 1);
                                    ok_genera = 1;
                                }
                            }
                        }
                    }
                }

                ADOQuery->Next();
            }
            ADOQuery->Close();
        }
    }
    catch (...) {}
    delete ADOQuery;
    return res;
}

int TdmDBServer::GeneraCMDaPrelievo(int val) {
    TCentroMissione cm;
    AnsiString strsql;
    int tipo_mis, agv;
    int id_articolo, IDPlc;
    TUDC UDCMissione;
    int i, ok_genera = 0, idudc, idudc2, res = 0, piano_dep = 0;
    int idx_plc;

    try {
        for (int idx = 1; idx <= NUM_PRELIEVOPLC; idx++) {
            idx_plc = IDX_PLCPRELIEVO; // metti che ci siano altri indici cosi' lo tratto come voglio
            if ((ClientData.Plc[idx_plc].Prelievo[idx][1].Ready) && (!ClientData.Plc[idx_plc].Prelievo[idx][1].InAllarme) && (ClientData.Plc[idx_plc].Prelievo[idx][1].ProntaAlPrelievo)) {
                if (dmDB->PresenzaCentroMissione(ClientData.Plc[idx_plc].Prelievo[idx][1].pos, 0) == 0) {
                    if (dmDB->PosPresenteMissioneAttiva(ClientData.Plc[idx_plc].Prelievo[idx][1].pos) == 0) {
                        // per limitare la generazione delle missioni un ciclo alla volta, controllo che ok_genera=0, in questo modo scorre i record finche'
                        // o sono finiti o ne ha trovato uno buono, altrimenti potrebbe fermarsi a generare anche se potrebbe proseguire
                        if (ok_genera == 0) {
                            cm.TipoMissione = 0;
                            cm.CodTipoMovimento = 0;
                            cm.CodTipoMissione = 1;
                            cm.Agv = 1;
                            cm.IDUDC = 1; // ???
                            cm.TipoUDC = 0; // ??
                            cm.stato = 0;
                            cm.Priorita = dmDB->priorita_missioni[1];

                            cm.posprel = ClientData.Plc[idx_plc].Prelievo[idx][1].pos;
                            cm.pianoprel = 1;
                            cm.h_prel = dmDB->RitornaAltezzedaPosizione(cm.posprel, cm.pianoprel, "HPREL");
                            // cm.corsia_prel = dmDB->FilaPosizione(m.posprel);

                            if ((cm.posprel > 0) && (cm.pianoprel > 0) && (cm.h_prel > 0) && (cm.TipoMissione != 2)) {
                                // rimando deposito a missione
                                cm.ZonaDeposito = "D";
                                cm.posdep = 0;
                                cm.pianodep = 0;
                                cm.h_dep = 0;

                                res = dmDB->GeneraCentroMissione(cm);
                                if (res > 0) {
                                    // dmDBServer->AggiornaStatoCentroMissioni(m.idcentromissioni, 1);
                                    ok_genera = 1;
                                }

                            }

                        }
                    }
                }
            }
        }
    }
    catch (...) {}
    return res;
}

int TdmDBServer::GeneraCMDaGaA1(int val) {
    TCentroMissione cm;
    AnsiString strsql;
    int tipo_mis, agv;
    int npianiocc, hprel;
    TUDC UDCMissione;
    int i, ok_genera = 0, idudc, idudc2, res = 0, piano_dep = 0;

    try {
        if (ok_genera == 0) {
            cm.TipoMissione = 0;
            cm.CodTipoMovimento = 0;
            cm.CodTipoMissione = 1;
            cm.Agv = 1;
            cm.IDUDC = 1; // ???
            cm.TipoUDC = 0; // ??
            cm.stato = 0;
            cm.Priorita = dmDB->priorita_missioni[2];

            cm.posdep = dmDBImpianto->TornaPosVuoteZona(1);
            cm.pianodep = 1;
            cm.h_dep = dmDB->RitornaAltezzedaPosizione(cm.posdep, cm.pianodep, "HDEP");
            if (cm.posdep > 0) {
                if (dmDB->PresenzaCentroMissione(cm.posdep, 0) == 0) {
                    if (dmDB->PosPresenteMissioneAttiva(cm.posdep) == 0) {
                        cm.ZonaPrelievo = "G";
                        cm.posprel = 0;
                        cm.pianoprel = 0;
                        cm.h_prel = 0;

                        // per ora lo faccio cosi' anche se si era detto di andare 2 piani sopra
                        // cm.h_prel = dmDB->RitornaAltezzedaPosizione(cm.posprel, cm.pianoprel, "HPREL");
                        res = dmDB->GeneraCentroMissione(cm);
                        if (res > 0) {
                            // dmDBServer->AggiornaStatoCentroMissioni(m.idcentromissioni, 1);
                            ok_genera = 1;
                        }
                    }
                }
            }
        }
    }
    catch (...) {}
    return res;
}

int TdmDBServer::GeneraCMDaHaB(int val) {
    TCentroMissione cm;
    AnsiString strsql;
    int tipo_mis, agv;
    int id_articolo, IDPlc;
    TUDC UDCMissione;
    int i, ok_genera = 0, idudc, idudc2, res = 0, piano_dep = 0;
    int idx_plc, idx;

    try {
        idx = 1; // fisso
        idx_plc = IDX_PLCDEPOSITO; // metti che ci siano altri indici cosi' lo tratto come voglio
        if ((ClientData.Plc[idx_plc].Deposito[idx][1].Ready) && (!ClientData.Plc[idx_plc].Deposito[idx][1].InAllarme) && (ClientData.Plc[idx_plc].Deposito[idx][1].ProntaAlDeposito)) {
            if (dmDB->PresenzaCentroMissione(ClientData.Plc[idx_plc].Deposito[idx][1].pos, 0) == 0) {
                if (dmDB->PosPresenteMissioneAttiva(ClientData.Plc[idx_plc].Deposito[idx][1].pos) == 0) {
                    // per limitare la generazione delle missioni un ciclo alla volta, controllo che ok_genera=0, in questo modo scorre i record finche'
                    // o sono finiti o ne ha trovato uno buono, altrimenti potrebbe fermarsi a generare anche se potrebbe proseguire
                    if (ok_genera == 0) {
                        cm.TipoMissione = 0;
                        cm.CodTipoMovimento = 0;
                        cm.CodTipoMissione = 1;
                        cm.Agv = 1;
                        cm.IDUDC = 1; // ???
                        cm.TipoUDC = 0; // ??
                        cm.stato = 0;
                        cm.Priorita = dmDB->priorita_missioni[3];

                        cm.posdep = ClientData.Plc[idx_plc].Deposito[idx][1].pos;
                        cm.pianodep = 1;
                        cm.h_dep = dmDB->RitornaAltezzedaPosizione(cm.posdep, cm.pianodep, "HDEP");
                        // cm.corsia_prel = dmDB->FilaPosizione(m.posprel);

                        if ((cm.posdep > 0) && (cm.pianodep > 0) && (cm.h_dep > 0)) {
                            cm.posprel = dmDBImpianto->CercaPrelievoH();
                            if (cm.posprel > 0) {
                                cm.pianoprel = 1;
                                cm.h_prel = dmDB->RitornaAltezzedaPosizione(cm.posprel, cm.pianoprel, "HPREL");
                                if (dmDB->PresenzaCentroMissione(cm.posprel, 0) == 0) {
                                    if (dmDB->PosPresenteMissioneAttiva(cm.posprel) == 0) {
                                        res = dmDB->GeneraCentroMissione(cm);
                                    }
                                }
                                if (res > 0) {
                                    // dmDBServer->AggiornaStatoCentroMissioni(m.idcentromissioni, 1);
                                    ok_genera = 1;
                                }
                            }
                        }
                    }
                }
            }

        }

    }
    catch (...) {}
    return res;
}

int TdmDBServer::GeneraCMDaIaA2(int val) {
    TCentroMissione cm;
    AnsiString strsql;
    int tipo_mis, agv;
    int npianiocc, hprel;
    TUDC UDCMissione;
    int i, ok_genera = 0, idudc, idudc2, res = 0, piano_dep = 0;

    try {
        if (ok_genera == 0) {
            cm.TipoMissione = 0;
            cm.CodTipoMovimento = 0;
            cm.CodTipoMissione = 0;
            cm.Agv = 1;
            cm.IDUDC = 1; // ???
            cm.TipoUDC = 0; // ??
            cm.stato = 0;
            cm.Priorita = dmDB->priorita_missioni[4];

            cm.ZonaPrelievo = "I";
            cm.posprel = dmDBImpianto->CercaPrelievo(cm.ZonaPrelievo, TIPOLOGIA_MATERIEPRIME);
            if (cm.posprel > 0) {
                cm.posprel = 0;
                cm.pianoprel = 0;
                cm.h_prel = 0;

                cm.ZonaDeposito = "J";
                dmDBImpianto->TornaPosDepLibera(cm.ZonaDeposito, cm.posdep, cm.pianodep, TIPOLOGIA_MATERIEPRIME);
                if (cm.posdep > 0) {
                    cm.h_dep = 0;
                    res = dmDB->GeneraCentroMissione(cm);
                    if (res > 0) {
                        // dmDBServer->AggiornaStatoCentroMissioni(m.idcentromissioni, 1);
                        ok_genera = 1;
                    }
                }
            }
        }
    }
    catch (...) {}
    return res;
}

int TdmDBServer::RitornaPosDaCM(int pos) {
    AnsiString strsql, articolo;
    TADOQuery *ADOQuery;
    int res = 0;

    if (!dmDB->ADOConnection1->Connected)
        return 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql = "SELECT count(*) as ccont FROM CentroMissioni where posdep= " + IntToStr(pos) + " and stato = 0";
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("ccont")->AsInteger;
        }
        ADOQuery->Close();
    }
    catch (...) {}
    delete ADOQuery;
    return res;
}

int TdmDBServer::AggiornaStatoCentroMissioni(int id, int stato) {
    AnsiString strsql, time;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        if (stato == 0)
            strsql = "UPDATE CentroMissioni SET stato = stato + 1 ";
        else
            strsql = "UPDATE CentroMissioni SET stato = " + IntToStr(stato);
        if (stato >= 1)
            strsql = strsql + " , Fine = GetDate()";
        strsql = strsql + " where id = " + IntToStr(id);
        ADOQuery->SQL->Text = strsql;
        res = ADOQuery->ExecSQL();
        dmDB->LogMsg(strsql + " , result : " + IntToStr(res));
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDBServer::CreaIdMissione() {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;
    TLocateOptions Opts;
    Opts.Clear();
    int idmiss = 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql.printf("SELECT * FROM Missioni order by Generata");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        idmiss = ADOQuery->FieldByName("Id")->AsInteger;
        do {
            if (idmiss == 1000000) {
                idmiss = 1;
            }
            else
                idmiss++;
        }
        while (ADOQuery->Locate("Id", idmiss, Opts));
        ADOQuery->Close();
    }
    catch (...) {}
    delete ADOQuery;
    return idmiss;
}

int TdmDBServer::GeneraMissione(TMissione m)
{
    int res = 0, idmiss, tabcarico = 0, tabscarico = 0, evita_agv = 0;
    int passa_pre_il_nodo_prel = 0, id_pos_prel_ribalatatori = -1, prenota = 0, pos_prel_ribaltatore, pos_dep_ribaltatore;
    int idartudc = 0, tipoPosPrel = 0, tipoPosDep = 0;
    int nodopassaggiodest = 0;
    int latoprel = 0, latodep = 0;
    TADOQuery *ADOQuery;
    TUDC UDCMissione;
    AnsiString strsql, nomeprel, nomedep, nomecorsia, codicearticolo;
    Evento ev;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;

        idmiss = CreaIdMissione();
        dmDB->AggiornaTabPostazioni();

        UDCMissione.IDUDC = m.idudc;
        // dmDB->LeggiStrutturaUdc(UDCMissione);
        nomeprel = dmDB->NomePosizione(m.posprel);
        nomedep = dmDB->NomePosizione(m.posdep);

        // check agv
        m.Agv = dmExtraFunction->Constrain(m.Agv, 0, NAGV);

        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;

        if ((idmiss > 0) && (tab.mappa_nodi.find(m.posprel) != tab.mappa_nodi.end()) && (tab.mappa_nodi.find(m.posdep) != tab.mappa_nodi.end())) {
            strsql.printf
                ("INSERT INTO Missioni ( ID, POSPREL ,POSDEP, ESITO ,GENERATA," "TIPOMISSIONE,PRIORITA,AGV,HPREL,HDEP," "POSPRELCLIENTE,POSDEPCLIENTE,IdUDC,CodTipoMovimento,EvitaAgv,"
                "CodiceArticolo,IdArtUDC,IdCentroMissioni,PianoPrel,PianoDep, idudc2, tabellaprelievo, tabelladeposito, latoforcheprel, latoforchedep, nodopassaggioprel,nodopassaggiodep, IDWMS )" " VALUES (%d,%d,%d,'%s',%s,"
                "%d,%d,%d,%d,%d," "'%s','%s',%d,%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)"
                , idmiss
                , m.posprel
                , m.posdep
                , "CREATE"
                , "GetDate()"
                , m.tipo_mis
                , m.priorita
                , m.Agv
                , m.h_prel
                , m.h_dep
                , nomeprel.c_str()
                , nomedep.c_str()
                , m.idudc
                , 0
                , evita_agv
                , codicearticolo.c_str()
                , m.tipoudc // aggiunto tipo udc //IdArtUDC
                , m.idcentromissioni
                , m.piano_prel
                , m.piano_dep
                , m.idudc2
                , m.altezza_udc // era lotto ora è altezza udc //AltezzaPallet
                , tabcarico
                , tabscarico
                , m.latoforcheprel
                , m.latoforchedep
                , m.nodopassaggioprel
                , m.nodopassaggiodep
                , m.idgestionale
                );
            ADOQuery->SQL->Text = strsql;
            res = ADOQuery->ExecSQL();

            sprintf(ev, "%c,%d,%d,%d,%d,%d,%d,%d,0,0,%d,%d,%d,%d,%d,%d,%d,0,0,%d"
                , EV_NUOVAMISSIONE
                , idmiss
                , m.posprel
                , m.h_prel
                , m.posdep
                , m.h_dep
                , m.priorita
                , m.tipo_mis
                , tabcarico
                , tabscarico
                , m.Agv
                , m.nodopassaggioprel
                , m.nodopassaggiodep
                , m.latoforcheprel
                , m.latoforchedep
                , evita_agv);
            cs->SendEv(ev);
            if (res == 1) {
                if (m.Agv) {
                    ClientData.DatiAgv[m.Agv].idmis = idmiss;
                    ClientData.DatiAgv[m.Agv].generata = 1;
                    if (m.tipo_mis < 2)
                        ClientData.DatiAgv[m.Agv].dest = m.posprel;
                    else
                        ClientData.DatiAgv[m.Agv].dest = m.posdep;
                    ClientData.DatiAgv[m.Agv].prel = m.posprel;
                    ClientData.DatiAgv[m.Agv].dep = m.posdep;
                    ClientData.DatiAgv[m.Agv].tipomis = m.tipo_mis;
                    // ClientData.DatiAgv[agv].DatiUDC.IDUDC = m.idudc;  // !!!! forse da mettere
                    // ClientData.DatiAgv[agv].DatiUDC[2].IDUDC = idUDC2;

                    dmDB->AggiornaStatoAgv(m.Agv);
                }
                if (m.tipo_mis <= 1) {
                    // pre il prelievo della vasca vuota dai ribaltatori imposto prenotata con il bit[1], devo tenere conto di una missione sotot pero
                    prenota = 1;
                    dmDB->PrenotaPos(m.posprel, prenota, m.corsia_prel);
                    dmDB->SettaPosSelezionata(m.posprel, 0, 0);

                }
                if ((m.tipo_mis == 0) || (m.tipo_mis == 2)) {
                    prenota = 1;
                    dmDB->PrenotaPos(m.posdep, prenota, m.corsia_dep);
                    dmDB->SettaPosSelezionata(m.posdep, 0, 0);
                }
                dmDB->AggiornaTabPostazioni();
                dmDB->aggiorna_tab_posizioni_locale = 1;
                // AggiornaStatoAgv(0);
                ClientData.ridisegna_mappa = 1;
                AggiornaStrutturaPLC();
                // invia variabile
                // dmComandiAgv->InviaTabellaDeposito(1, tabscarico);     Lotto
            }
            dmDB->LogMsg(strsql);
        }
        else {
            strsql = "Id Missione non trovato o pos_prel " + IntToStr(m.posprel) + " o pos_dep " + IntToStr(m.posdep) + " non presenti nel crs";
            dmDB->LogMsg(strsql);
        }
    }
    catch (...) {}
    delete ADOQuery;
    return res;
}

void TdmDBServer::RitornaNodoPassaggio(int agv, int posprel, int posdep, int latoforcheprel, int latoforchedep, int tipomiss, int &pospassanteprel, int &pospassantedep) {
    // Passaggio obbligato per nodo: controllo
    pospassanteprel = 0;
    pospassantedep = 0;

    if ((pospassanteprel < 0) || (pospassanteprel > 9999)) {
        pospassanteprel = 0;
    }
    if ((pospassantedep < 0) || (pospassantedep > 9999)) {
        pospassantedep = 0;
    }

}

int TdmDBServer::RitornaTipologiaMissioneDaMissioni(int id) {
    AnsiString strsql, articolo, pos;
    TADOQuery *ADOQuery;
    int res = 0;

    if (!dmDB->ADOConnection1->Connected)
        return 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql = "SELECT CodTipoMissione FROM Missioni where Id = " + IntToStr(id);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("CodTipoMissione")->AsInteger;
        }
        ADOQuery->Close();
    }
    catch (...) {}
    delete ADOQuery;
    return res;
}

int TdmDBServer::AggiornaMissione(int id, int stato, int agv, int peso) {
    AnsiString strsql, time;
    TADOQuery *ADOQuery;
    int res = 0, statomissione = 0;

    try {
        try {
            if (stato > 2)
                time = dmDB->ReturnTimeMission(id);
        }
        catch (...) {}

        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql = "UPDATE Missioni SET Esito = ";
        switch (stato) {
        case 1:
            strsql += "'INIZIATA' , Iniziata = GetDate(), Agv = " + IntToStr(agv) + " , BatteriaIniziale = " + IntToStr(ClientData.DatiAgv[agv].LivelloBatteria);
            statomissione = ID_MISSIONE_INIZIATA_DA_AGV;
            break;
        case 2:
            strsql += "'PRELEVATO' , Prelevato = GetDate()";
            statomissione = ID_MISSIONE_PRELEVATA_DA_AGV;
            break;
        case 3:
            strsql += "'COMPLETATA' , Fine = GetDate(), BatteriaFinale = " + IntToStr(ClientData.DatiAgv[agv].LivelloBatteria) + " , Tempo = '" + time + "'";
            statomissione = ID_MISSIONE_COMPLETATA;
            break;
        case 4:
            strsql += "'ABORTITA' , Fine = GetDate(), BatteriaFinale = " + IntToStr(ClientData.DatiAgv[agv].LivelloBatteria) + " , Tempo = '" + time + "'";
            if (ClientData.DatiAgv[agv].DatiUDC.IDUDC > 0)
                statomissione = ID_MISSIONE_ABORTITA_SENZA_PALLET;
            else
                statomissione = ID_MISSIONE_ABORTITA_CON_PALLET;
            break;
        case 5:
            strsql += "'CANCELLATA' , Fine = GetDate(), BatteriaFinale = " + IntToStr(ClientData.DatiAgv[agv].LivelloBatteria) + " , Tempo = '" + time + "'";
            if (ClientData.DatiAgv[agv].DatiUDC.IDUDC > 0)
                statomissione = ID_MISSIONE_ABORTITA_SENZA_PALLET;
            else
                statomissione = ID_MISSIONE_ABORTITA_CON_PALLET;
            break;
        }
        strsql += " where id = " + IntToStr(id);
        ADOQuery->SQL->Text = strsql;
        res = ADOQuery->ExecSQL();
        dmDB->LogMsg(strsql + " , result : " + IntToStr(res));
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

void TdmDBServer::AbortMission(int idmiss) {
    /* TADOQuery *ADOQuery;
     int prel = 0, posdep = 0, pianodep = 0, tipomis, corsiadep, nagv = 0, prenota, id_centro_missione = 0, piano_dep;
     int IDWMS = 0;
     int corsia_prel = 0, corsia_dep = 0;
     AnsiString NoPrel = "", stringa;
     try {
     if (!dmDB->ADOConnection1->Connected)
     return;
     ADOQuery = new TADOQuery(NULL);
     ADOQuery->Connection = dmDB->ADOConnection1;
     stringa = "Select * from Missioni_view where id = " + IntToStr(idmiss);
     ADOQuery->Close();
     ADOQuery->SQL->Clear();
     ADOQuery->SQL->Append(stringa);
     ADOQuery->Open();
     if (ADOQuery->RecordCount) {
     // posprel = ADOQuery->FieldByName("posprel")->AsInteger ;
     prel = ADOQuery->FieldByName("posprel")->AsInteger;
     NoPrel = ADOQuery->FieldByName("prelevato")->AsString;
     posdep = ADOQuery->FieldByName("posdep")->AsInteger;
     tipomis = ADOQuery->FieldByName("tipomissione")->AsInteger;
     nagv = ADOQuery->FieldByName("Agv")->AsInteger;
     piano_dep = ADOQuery->FieldByName("PianoDep")->AsInteger;
     id_centro_missione = ADOQuery->FieldByName("IdCentroMissioni")->AsInteger;

     corsia_prel = dmDB->QueryMissioniSocket->FieldByName("CorsiaPrel")->AsInteger;
     corsia_dep = dmDB->QueryMissioniSocket->FieldByName("CorsiaDep")->AsInteger;

     IDWMS = dmDB->QueryMissioniSocket->FieldByName("IDWMS")->AsInteger;
     // baia_dep = ADOQuery->FieldByName("PosDepCliente")->AsInteger;
     }
     dmDB->LogMsg("Missione n° " + IntToStr(idmiss) + " abortita da lista agvx");
     ADOQuery->Close();
     // guardare se ho un'anomalia sull'abort
     // guardo se devo uscire dall'ingombro
     dmDBServer->AggiornaMissione(idmiss, 4, nagv);
     dmDBImpianto->AggiornaStatoMissioniWMS(IDWMS, 6);

     if ((NoPrel == "") && (prel)) {
     prenota = 0;
     dmDB->PrenotaPos(prel, prenota, dmDB->FilaPosizione(prel));
     }
     if (id_centro_missione)
     dmDBServer->AggiornaStatoCentroMissioni(id_centro_missione, 5);
     prenota = 0;
     if (posdep)
     dmDB->PrenotaPos(posdep, prenota, dmDB->FilaPosizione(posdep));
     dmDB->AggiornaTabPostazioni();
     }
     catch (...) {}
     delete ADOQuery;
     return; */
}

void TdmDBServer::ComponiMessaggioAllarme(int num_messaggio, int id_messaggio, AnsiString params1, AnsiString params2)
{
    AnsiString str_messaggio;
    str_messaggio = dmDB->TabMessaggiAllarme[id_messaggio]["MESSAGGIO"] + " " + params1 + " " + params2;
    MainForm->MessageBar1->Messages->Strings[num_messaggio] = str_messaggio;
}

int TdmDBServer::RitornaCaricoScarico(int pos, int piano, int fila) { // mod corsia

    return dmDBImpianto->TornaTabella(pos, piano, fila); // mod corsia

}

// ---------------------------------------------------------------------------

int TdmDBServer::AggiornaStatoOrdineTabelleAutoStart() {
    AnsiString strsql, time;
    TADOQuery *ADOQuery;
    int res = 0, res2 = 0;
    /*
     try {
     if (!dmDB->ADOConnection1->Connected)
     return 0;

     ADOQuery = new TADOQuery(NULL);
     ADOQuery->Connection = dmDB->ADOConnection1;
     strsql = "UPDATE MovPesate SET StatoOrdine = " + IntToStr(STATO_ORDINE_IN_ESECUZIONE) + " where StatoOrdine <> " + IntToStr(STATO_ORDINE_IN_ESECUZIONE);
     strsql = strsql + " AND AutoStart > 0 AND DataOraDaIniziare >= GetDate()";
     ADOQuery->SQL->Text = strsql;
     res = ADOQuery->ExecSQL();
     ADOQuery->Close();
     ADOQuery->SQL->Clear();
     strsql = "UPDATE MovSpostamento SET StatoOrdine = " + IntToStr(STATO_ORDINE_IN_ESECUZIONE) + " where StatoOrdine <> " + IntToStr(STATO_ORDINE_IN_ESECUZIONE);
     strsql = strsql + " AND AutoStart > 0 AND DataOraDaIniziare >= GetDate()";
     ADOQuery->SQL->Append(strsql);
     res2 = ADOQuery->ExecSQL();
     // dmDB->LogMsg(strsql + " , result : " + IntToStr(res));
     }
     catch (...) {
     }
     delete ADOQuery;

     */ return res + res2;
}

void __fastcall TdmDBServer::TimerAggiornamentiMinutiTimer(TObject * Sender) {
    // aggiorno tabelle di autostart ogni minuto ogni minuto
    int indice, postazione;
    WORD Hour, Min, Sec, MSec;
    int fermati = 0;

    if ((MainForm->count_inizio < 20) || (MainForm->close_program) || (!dmDB->ADOConnection1->Connected) || (!dmDB->lettostatoagv))
        return;
    TimerAggiornamentiMinuti->Enabled = false;
    try {

        // per controlli sul minuto
        DecodeTime(Now(), Hour, Min, Sec, MSec);
        if (Min != minuto_prec) {
            // Cambio minuto
            AggiornaStatoOrdineTabelleAutoStart();
            minuto_prec = Min;
        }
    }
    catch (...) {

    }
    TimerAggiornamentiMinuti->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TdmDBServer::TimerStanzaTimer(TObject *Sender) {
    int inzona = 0;
    int idporta = 1;
    if ((MainForm->count_inizio < 20) || (MainForm->close_program) || (!dmDB->ADOConnection1->Connected) || (!dmDB->lettostatoagv))
        return;
    TimerStanza->Enabled = false;
    try {
        for (int i = 1; i <= NAGV; i++) {
            if (ClientData.DatiAgv[i].pos > 0) {
                inzona = CercaZona(tab.mappa_nodi[ClientData.DatiAgv[i].pos].posx, tab.mappa_nodi[ClientData.DatiAgv[i].pos].posy);
                if (inzona > 0) {
                    if (!ClientData.DatiPorte[idporta].AGVInZona)
                        PLCPhoenixThread[ClientData.DatiPorte[idporta].id_thread_plc]->ScriviPhoenix(ClientData.DatiPorte[idporta].Bit_AGVInZona, ClientData.DatiPorte[idporta].DW_Addr_AGVInZona, true);

                }
                else {
                    if (ClientData.DatiPorte[1].AGVInZona)
                        PLCPhoenixThread[ClientData.DatiPorte[idporta].id_thread_plc]->ScriviPhoenix(ClientData.DatiPorte[idporta].Bit_AGVInZona, ClientData.DatiPorte[idporta].DW_Addr_AGVInZona, false);

                }
            }
        }
    }
    catch (...) {

    }
    TimerStanza->Enabled = true;
}
// ---------------------------------------------------------------------------

int TdmDBServer::CercaZona(int xx, int yy) {
    AnsiString zona;
    int topx, topy, botx, boty;
    TADOQuery * ADOQuery;
    int res = 1;
    int found = 0;

    topx = 49000;
    botx = 55000;
    topy = 16500;
    boty = 8000;
    if (((xx >= topx) && (xx <= botx)) && ((yy >= boty) && (yy <= topy))) {
        found = 1;
    }

    return found;
}

void TdmDBServer::NuovoRecordConsumoBatteria(int idagv) {
    TADOQuery *ADOQuery;
    AnsiString stringa;
    int stato_agv = 0;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        if (ClientData.DatiAgv[idagv].manuale)
            stato_agv |= dmDB->bit[0];
        if (ClientData.DatiAgv[idagv].semiautomatico)
            stato_agv |= dmDB->bit[1];
        if (ClientData.DatiAgv[idagv].incarica)
            stato_agv |= dmDB->bit[2];
        if (ClientData.DatiAgv[idagv].load)
            stato_agv |= dmDB->bit[3];
        if (ClientData.DatiAgv[idagv].bitbattbassa)
            stato_agv |= dmDB->bit[4];
        stringa = "Insert Into Batteria (IdMissione,AGV,Pos,PosPrecedente,Destinazione,ConsumoAttuale,ConsumoNodoPrec,StatoBatteria,StatoAGV,DataOra)";
        stringa = stringa + " VALUES (" + IntToStr(ClientData.DatiAgv[idagv].idmis) + "," + IntToStr(idagv) + "," + IntToStr(ClientData.DatiAgv[idagv].pos) + "," + IntToStr(ClientData.DatiAgv[idagv].posprec) + "," +
            IntToStr(ClientData.DatiAgv[idagv].dest) + "," + MainForm->Cons_Bat[idagv] + "," // vedere se il dato viene corretto per il database, decimal 10,3
            + MainForm->Cons_BatOld[idagv] + "," // vedere se il dato viene corretto per il database, decimal 10,3
            + IntToStr(ClientData.DatiAgv[idagv].LivelloBatteria) + "," + IntToStr(stato_agv) + "," "GetDate()" // BEARBEITET_VON_EMATRIC
            ")";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->ExecSQL();
    }
    catch (...) {}
    delete ADOQuery;

}

void TdmDBServer::AggiornaStrutturaPLC() {
    int i, j;
    TIndexList TabPos;
    dmDB->CaricaTabellaK("piani_view order by pos", "pos", TabPos);
    for (i = 1;
        i <= N_PLC;
        i++) {
        for (j = 1; j <= NUM_PRELIEVOPLC; j++) {
            for (int p = 0; p < NUMPIANI_DEP; p++) {
                ClientData.Plc[i].Prelievo[j][p].generata = TabPos[j]["PRENOTATA"].ToIntDef(0) | TabPos[j]["POS_PRENOTATA"].ToIntDef(0);
                ClientData.Plc[i].Prelievo[j][p].esclusa = TabPos[j]["DISABILITATA"].ToIntDef(0) | TabPos[j]["POS_DISABILITA"].ToIntDef(0);
                ClientData.Plc[i].Prelievo[j][p].selezionata = TabPos[j]["SELEZIONATA"].ToIntDef(0);

            }
        }
        for (j = 1; j <= NUM_DEPOSITOPLC; j++) {
            for (int p = 0; p < NUMPIANI_DEP; p++) {
                ClientData.Plc[i].Deposito[j][p].generata = TabPos[j]["PRENOTATA"].ToIntDef(0) | TabPos[j]["POS_PRENOTATA"].ToIntDef(0);
                ClientData.Plc[i].Deposito[j][p].esclusa = TabPos[j]["DISABILITATA"].ToIntDef(0) | TabPos[j]["POS_DISABILITA"].ToIntDef(0);
                ClientData.Plc[i].Deposito[j][p].selezionata = TabPos[j]["SELEZIONATA"].ToIntDef(0);
            }
        }
    }
    /*
     dmDB->CaricaTabellaK("piani_view order by pos, piano", "pos", TabPos);
     for (i = 0; i < NUM_POSTAZIONI * 4; i++) {
     posplc = ClientData.Zona[(i / 4) + 1].Postazione[(i % 4) + 1].pos;
     ClientData.Zona[(i / 4) + 1].Postazione[(i % 4) + 1].generata = TabPos[posplc]["PRENOTATA"].ToIntDef(0) | TabPos[posplc]["POS_PRENOTATA"].ToIntDef(0);
     ClientData.Zona[(i / 4) + 1].Postazione[(i % 4) + 1].esclusa = TabPos[posplc]["DISABILITATA"].ToIntDef(0) | TabPos[posplc]["POS_DISABILITA"].ToIntDef(0);
     }
     */

}

int TdmDBServer::RitornaIndicePostazione(int pos) {
    int idx = 1, ret = 0;
    int indicezona = 1;
    bool trovato = false;
    while ((idx <= NUM_POSTAZIONI) && (!trovato)) {
        if (ClientData.Zona[indicezona].Postazione[idx].pos == pos) {
            trovato = true;
        }
        else {
            idx++;
        }
    }
    ret = idx;
    return ret;
}

int TdmDBServer::UpdateStatoPorta(int porta, int stato) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0, pren;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa.printf("UPDATE Porte SET " " Stato = %d where IdPorta = %d ", stato, porta);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg(stringa);
        // dmDB->LogMsg(" Update Pos "+IntToStr(pos)+", prenota : "+IntToStr(prenota)+", escludi : "+IntToStr(escludi)+", udc : "+CodiceUdc+", priorchiamata : "+IntToStr(priorita)+", secondi : "+IntToStr(secondi));
        delete ADOQuery;
        dmDB->aggiorna_tab_porte_locale = true;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {}
    return res;
}

int TdmDBServer::RitornaPosDaMiss(int pos, AnsiString tipopos) {
    AnsiString strsql, articolo;
    TADOQuery *ADOQuery;
    int res = 0;

    if (!dmDB->ADOConnection1->Connected)
        return 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql = "SELECT count(*) as ccont FROM Missioni where " + tipopos + "= " + IntToStr(pos) + " and (Tempo IS NULL)";
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("ccont")->AsInteger;
        }
        ADOQuery->Close();
    }
    catch (...) {}
    delete ADOQuery;
    return res;
}

void TdmDBServer::MissionePrelievoManuale(int buttontag) {
    TMissione m;
    AnsiString strsql, Nomepos = "";
    int tipo_mis, priorita, agv, tipologia, missione_manuale, PrelieviRimasti;
    TUDC UDCMissione;
    int i, ok_genera = 0, idudc, idudc2, res = 0, piano_dep = 0;

    if (buttontag > 0) {
        try {
            if (!dmDB->ADOConnection1->Connected)
                return;
            for (int i = 1; i <= NAGV; i++) {
                if ((ClientData.DatiAgv[i].idmis == 0) && (!ClientData.DatiAgv[i].load) && (!ClientData.DatiAgv[i].manuale && !ClientData.DatiAgv[i].semiautomatico)) {
                    ok_genera = 0;
                    m.idcentromissioni = 0;
                    m.tipologia_mov = 0;
                    m.tipologia_mis = 0; // PREL/ deposito
                    m.tipo_mis = 0;
                    m.nodopassaggioprel = 0;
                    m.nodopassaggiodep = 0;

                    m.posprel = buttontag;
                    m.piano_prel = 1;
                    // m.h_prel = dmDB->RitornaAltezzedaPosizione(m.posprel, m.piano_prel, "HPREL", m.corsia_prel); // !!! mettere a posto corsia prel
                    m.h_prel = dmDB->RitornaAltezzadaPosizioneNuovo(m.posprel, m.piano_prel, "HPREL", m.corsia_prel); // !!! mettere a posto corsia prel
                    // guardo se ho una pos storicizzata a terra
                    m.idudc = dmDB->RitornaUDCdaPosPiano(m.posprel, m.piano_prel);
                    ClientData.DatiAgv[i].DatiUDC.IDUDC = m.idudc;
                    dmDBImpianto->TornaPosPianoDaIDUDC(m.idudc, m.posdep, m.piano_dep);
                    // m.h_dep = dmDB->RitornaAltezzedaPosizione(m.posdep, m.piano_dep, "HDEP", m.corsia_dep); // !!! mettere a posto corsia dep
                    m.h_dep = dmDB->RitornaAltezzadaPosizioneNuovo(m.posdep, m.piano_dep, "HDEP", m.corsia_dep); // !!! mettere a posto corsia dep
                    m.priorita = 4;
                    m.Agv = i;
                    // ClientData.DatiAgv[i].DatiUDC.IDUDC = m.idudc;
                    m.idudc2 = 0;
                    m.Lotto = 0;

                    if ((m.posprel > 0) && (m.piano_prel > 0) && (m.h_prel > 0)) {
                        if ((m.posdep > 0) && (m.piano_dep > 0) && (m.h_dep > 0)) {
                            // if (((dmDB->ControllaPosizioneOK(m.posprel, m.piano_prel) == 1) && (dmDB->ControllaPosizioneOK(m.posdep, m.piano_dep) == 1)) || (missione_manuale == 1)) {
                            res = GeneraMissione(m);
                            if (res > 0) {
                                ok_genera = 1;
                                dmDB->LogMsg("Creata missione " + IntToStr(res) + " prelievo da INOUT manuale ");
                            }
                        }
                    }
                    else {
                        dmDB->LogMsg("Missione prelievo da INOUT manuale fallita per pos." + Nomepos);
                    }

                }
            }
        }
        catch (...) {}
    }
}

// ---------------------------------------------------------------------------
