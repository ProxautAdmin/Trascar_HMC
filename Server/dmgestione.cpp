// ---------------------------------------------------------------------------

#pragma hdrstop

#include "dmgestione.h"
#include "main.h"
// #include "dmphoenix.h"
#include "percorsiprototipo.h"
#include "socket.h"
#include "db.h"
#include "dmgestionedatabase.h"
#include "datamodulecomandiagv.h"
#include "ExtraFunction.h"
#include "PLCThread.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TDMGestioneEventi *DMGestioneEventi;
extern CLIENTSOCKET *cs;
extern DATILGV tab;

// ---------------------------------------------------------------------------
__fastcall TDMGestioneEventi::TDMGestioneEventi(TComponent* Owner)
    : TDataModule(Owner)
{
    sync_lamp = false;
    init = true;
}

// ---------------------------------------------------------------------------
void __fastcall TDMGestioneEventi::TimerEventiTimer(TObject *Sender) {
    int i, j, agv, k, anomalia_generatori, aggiorna_allarmi_attivi;
    Evento ev;

    // gestisco le missioni degli agv e tutti gli eventi

    if ((MainForm->count_inizio <= 20) || (MainForm->close_program))
        return;
    TimerEventi->Enabled = false;
    // gestisco allarmi dei consensi che se escludono agv da pc non vanno via
    aggiorna_allarmi_attivi = 0;
    for (agv = 1; agv <= NAGV; agv++) {
        if ((ClientData.DatiAgv[agv].manuale) || (ClientData.DatiAgv[agv].pos == 0)) {
            if (ClientData.DatiAgv[agv].allarme_consenso) {
                ClientData.DatiAgv[agv].allarme_consenso = 0;
                MainForm->MessageBar1->DelMsgNo(ClientData.DatiAgv[agv].num_allarme_consenso); // alarm 502---506);
                aggiorna_allarmi_attivi = 1;
            }

            if (ClientData.DatiAgv[agv].allarme_consenso_porta) {
                ClientData.DatiAgv[agv].allarme_consenso_porta = 0;
                MainForm->MessageBar1->DelMsgNo(ClientData.DatiAgv[agv].num_allarme_consenso_porta); // alarm 502---506);
                aggiorna_allarmi_attivi = 1;
            }
        }
    }
    if (aggiorna_allarmi_attivi)
        dmDB->AggiornaAllarmiAttivi();

    TimerEventi->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TDMGestioneEventi::DataModuleCreate(TObject *Sender)
{
    TimerEventi->Enabled = true;
    TimerPuliziaStorici->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TDMGestioneEventi::TimerMissioniTimer(TObject *Sender) {
    int indice, agv;
    TRecordList TabListaMissioniPerAgv;
    int aggiorna_allarmi_attivi;
    AnsiString strsql;
    static int divisoretempo = 0;

    AnsiString TempC;
    // timer per generazione nuove missoni
    if ((MainForm->count_inizio < 20) || (MainForm->close_program)) {
        return;
    }
    TimerMissioni->Enabled = false;
    aggiorna_allarmi_attivi = 0;

    if (aggiorna_allarmi_attivi)
        dmDB->AggiornaAllarmiAttivi();

    // sospensione
    if (divisoretempo >= 9) { // controllo ogni 10 secondi

        divisoretempo = 0;
    }
    else {
        divisoretempo++;
    }


    TimerMissioni->Enabled = true;

}
// ---------------------------------------------------------------------------

int TDMGestioneEventi::CambiaDestinazioneAMissione(int posprel, int posdepvecchia, int posdepnuova) {
    /* int id_missione_da_cambiare ;
     int res = 0,tabcarico = 0,tabscarico = 0,h_prel=100,h_dep = 100 ,pos_dep_ribaltatore;
     int passa_pre_il_nodo_prel = 0,prenota = 0;
     AnsiString  nomedep,strsql ;
     Evento ev;
     TADOQuery *ADOQuery;
     if (!dmDB->ADOConnection1->Connected)
     return 0;
     id_missione_da_cambiare = dmDB->RicercaUltimaMissioneInCorsoDaPosPrelAPosDep(posprel,posdepvecchia);
     if (id_missione_da_cambiare) {
     h_prel = dmDB->TabPostazioniVasche[IntToStr(posprel)]["HPREL"].ToIntDef(dmDB->quota_prel_default) ;
     h_dep = dmDB->TabPostazioniVasche[IntToStr(posdepnuova)]["HDEP"].ToIntDef(dmDB->quota_dep_default) ;
     nomedep = dmDB->TabPostazioniVasche[IntToStr(posdepnuova)]["CUSTOMERPOS"] ;
     if ((id_missione_da_cambiare > 0 )&&(tab.mappa_nodi.find(posprel) != tab.mappa_nodi.end())&&(tab.mappa_nodi.find(posdepnuova) != tab.mappa_nodi.end())) {
     ADOQuery = new TADOQuery(NULL);
     ADOQuery->Connection = dmDB->ADOConnection1;
     strsql.printf("Update Missioni set POSDEP = %d ,HDEP = %d ,  POSDEPCLIENTE = '%s' where Id = %d"
     ,posdepnuova
     ,h_dep
     ,nomedep.c_str()
     ,id_missione_da_cambiare
     ) ;
     ADOQuery->SQL->Text = strsql;
     res = ADOQuery->ExecSQL();
     //sprintf(ev, "%c,%d,%d,%d,%d,%d,%d,%d,0,0,%d,%d,%d,%d,0,0,0,0,0", EV_NUOVAMISSIONE, id_missione_da_cambiare, posprel,h_prel, posdepnuova,h_dep, 0, 0,tabcarico,tabscarico,0,0);
     //devo passare idmissione = 0 all'agvx e come posprel la destinazione precedente
     sprintf(ev, "%c,%d,%d,%d,%d,%d,%d,%d,0,0,%d,%d,%d,%d,0,0,0,0,0", EV_NUOVAMISSIONE, 0, posdepvecchia,h_prel, posdepnuova,h_dep, 0, 0,tabcarico,tabscarico,0,0);
     cs->SendEv(ev);
     dmDB->LogMsg("Inviata missione per cambio destinazione da "+IntToStr(posdepvecchia)+" a "+IntToStr(posdepnuova)+" id missione : "+IntToStr(id_missione_da_cambiare) );
     if (res == 1) {
     dmDB->PrenotaPos(posprel , 1 , 0, 0) ; //azzero destinazione magazzino
     prenota = 0 ;
     pos_dep_ribaltatore = dmDB->PosizionePlcDepositoVascheRibaltatori(posdepnuova) ;
     if (pos_dep_ribaltatore >= 0) {
     prenota = ClientData.DatiPlcRibaltatori[pos_dep_ribaltatore].generata ;
     prenota = prenota | dmDB->bit[0] ;
     }else
     prenota = 1 ;
     dmDB->PrenotaPos(posdepvecchia , 0, dmDB->FilaPosizione(posdepvecchia)) ;
     if (!dmDB->ControllaCorsiaAncoraPiena(dmDB->FilaPosizione(posdepvecchia))) {
     //se la destinazione era all'inizio della corsia reimposto codice automatico in corsia
     dmDB->SvuotaCorsiaVasca(dmDB->FilaPosizione(posdepvecchia)); //svuoto corsia e imposto codice automatico
     }
     dmDB->PrenotaPos(posdepnuova , prenota, dmDB->FilaPosizione(posdepnuova)) ;
     dmDB->AggiornaTabPostazioni();
     dmDB->aggiorna_tab_posizioni_locale = 1 ;
     //dmDB->AggiornaStatoAgv(0);
     ClientData.ridisegna_mappa = 1 ;
     //dmComandiAgv->AggiornaPrenotazioni();
     }
     }

     }
     return res ; */
}

void __fastcall TDMGestioneEventi::TimerPuliziaStoriciTimer(TObject *Sender) {

    TDateTime adesso;
    WORD adessoY, adessoM, adessoD, Y, M, D;

    TimerPuliziaStorici->Enabled = false;
    try {
        adesso = Now();
        DecodeDate(adesso, adessoY, adessoM, adessoD);
        DecodeDate(t, Y, M, D);
        if ((adessoD != D) || (adessoM != M) || (adessoY != Y)) {
            // Cambio giorno
            dmExtraFunction->StoricizzaTutto("MONTH", 3);
            // PuliziaStorici();
            t = adesso;
        }
    }
    catch (...) {
        dmDB->ADOConnection1->Close();
    }
    TimerPuliziaStorici->Enabled = true;

}

// ---------------------------------------------------------------------------
