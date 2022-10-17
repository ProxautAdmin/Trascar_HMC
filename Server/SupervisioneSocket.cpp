// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "SupervisioneSocket.h"
#include "percorsiprototipo.h"
#include "db.h"
#include "dmgestioneplc.h"
#include "datamodulecomandiagv.h"
#include "plcthread.h"
#include "dmgestionedatabase.h"
#include "ExtraFunction.h"
#include "DBImpianto.h"

extern CLIENTSOCKET *cs;
// extern CLIENTSOCKET *sock ;
extern DATILGV tab;

// ---------------------------------------------------------------------------
SUPERVISIONEMISS_STATESOCKET::SUPERVISIONEMISS_STATESOCKET() : MISS_STATESOCKET() {
}

void SUPERVISIONEMISS_STATESOCKET::Elabora(char *ev, TCustomWinSocket *Socket) {
    char s[100], str[100], answer[100], stmis, op, varvalue[50];
    Evento ev2;
    int n, pos, pianodep, idmiss, i, id, allarme, countvirgole, lista, destinazione, poscodice, cambiodest, posagv;
    AnsiString Msg, str2, strcons, posdepcliente;
    TObject *Sender;
    // Variant locvalues[2];
    AnsiString esito_mis, Codice, NoPrel, CodiceCliente, stringa, nomevar, valorevar, nomeposprel, nomeposdep;
    int pesata, dest = 0, word, val, tipomis, porta, posdep, prel, nagv, loaded, nodoa, nodob, corsia, livellobatt, bypass;
    int fila, prenota, id_centro_missione, IDWMS, tipologia_mov, tipologia_mis;
    int piano_prel = 1, piano_dep = 1, tipoplcprel = 0, tipoplcdep = 0, latoprel = 0, latodep = 0;

    int tipo_pallet = 0;
    int corsia_prel = 0, corsia_dep = 0;
    bool abilitata, update = false;
    int nodostop = 0;
    int altezza_pallet = 0;
    // if ((dmDB->ADOConnection1->Connected)&&(MainForm->lettostatoagv)&&(MainForm->lettoparametriplc)&&(MainForm->count_inizio > 3)) {
    if ((dmDB->ADOConnection1->Connected) && (dmDB->lettostatoagv)) {
        switch (ev[0]) {
        case EV_BIT:
            break;
        case EV_STATO_VAR:
            // Lettura barcode
            try {
                if (nomevar == "Var_Poll") {

                }
            }
            catch (...) {
                dmDB->LogMsg("Eccezione ricezione varvalue");
            }
            break;
        case EV_ATTESA:
            if (ev[2] == 'N') {
                // se sto per prelevare segnalo agv in ingombro
                sscanf(ev + 4, "%d,%d,%c", &n, &nodoa, &op);
                nagv = n;
                // vedere se il carico scarico dentro la vemac è da gestire con il consenso sul nodo
                ClientData.DatiAgv[nagv].nodoa = nodoa;
                ClientData.DatiAgv[nagv].consensonododato = 0;
                ClientData.DatiAgv[nagv].richiestaconsensonodo = 1;
                ClientData.DatiAgv[nagv].op_richiestaconsensonodo = op;
            }
            else {
                sscanf(ev + 4, "%d,%d,%d,%s,%d", &n, &nodoa, &nodob, str, posagv);
                nagv = n;
                if (!ClientData.DatiAgv[nagv].consensonododato) { // aspetto di avere resettato il consenso nodo perchè sul prelievo daalla vemac
                    // mi arriva dall'agvx prima la richiesta del ramo che l'agv che ha prelevato
                    ClientData.DatiAgv[nagv].nodoa = nodoa;
                    ClientData.DatiAgv[nagv].nodob = nodob;
                    ClientData.DatiAgv[nagv].richiestaconsenso = 1;
                    ClientData.DatiAgv[nagv].consensodato = 0;
                    strcons = str;
                    if (strcons.Pos(","))
                        strcons = strcons.SubString(1, strcons.Pos(",") - 1);
                    MainForm->strcons[nagv] = strcons;
                    if (strcons.Pos("porta"))
                        ClientData.DatiAgv[nagv].primo_ciclo_consenso = 1;
                }
            }
            break;
        case 'L':
            // cambia lo stato del loaded
            sscanf(ev + 2, "%d,%d", &nagv, &loaded);
            if (ClientData.DatiAgv[nagv].load != loaded) {
                dmDB->LogMsg("Ricevuto nuovo stato del carico a bordo dopo ripristino agv : Load Prec : " + IntToStr(ClientData.DatiAgv[nagv].load) + " , Load Nuovo : " + IntToStr(loaded));
                ClientData.DatiAgv[nagv].load = loaded;
                if (!loaded) {
                    ClientData.DatiAgv[nagv].DatiUDC.IDUDC = 0;
                    ClientData.DatiAgv[nagv].tipo_pallet = 0;
                    ClientData.DatiAgv[nagv].altezza_pallet = 0;
                    // dmDB->LeggiStrutturaUdc(ClientData.DatiAgv[nagv].DatiUDC);
                }
            }
            break;
        case '£':
            // richiesta consenso ramo completata
            sscanf(ev + 2, "%d,%d,%d,%s", &n, &nodoa, &nodob, str);
            nagv = n;
            // ClientData.DatiAgv[nagv].nodoa = nodoa ;   // CHIEDI A MATTEO PERCHE' E' COMMENTATO
            // ClientData.DatiAgv[nagv].nodob = nodob ;
            strcons = str;
            if (strcons.Pos(","))
                strcons = strcons.SubString(1, strcons.Pos(",") - 1);
            // RIMETTERE SE CI SONO DELLE PORTE DMGestione_Porte->AggiungiRimuoviRichiestaConsensoPorta(strcons, 0);
            if (ClientData.DatiAgv[nagv].consensodato == nodob) {
                ClientData.DatiAgv[nagv].consensodato = 0;
                dmDB->LogMsg("Resetto consenso dato ad agv " + IntToStr(nagv) + " dopo messaggio ricevuto da agvx e nodob = " + IntToStr(nodob));
            }
            MainForm->strcons[nagv] = "";
            break;
        case EV_STATOAGV:
            sscanf(ev + 2, "%d,%d,%d,%d,%d,%d,%8s,%s", &n, &pos, &dest, &idmiss, &livellobatt, &bypass, s, str);
            nagv = n;
            if ((ClientData.DatiAgv[nagv].pos != 0) && (pos == 0)) {
                // abortire tutte le missioni relative a quell'agv
                dmDB->LogMsg("Agv " + IntToStr(n) + " disabilitato in pos : " + IntToStr(ClientData.DatiAgv[nagv].pos));
            }
            else if ((pos != 0) && (ClientData.DatiAgv[nagv].pos == 0))
                dmDB->LogMsg("Agv " + IntToStr(n) + " riabilitato in pos : " + IntToStr(pos));
            if (ClientData.DatiAgv[nagv].pos != pos) {
                ClientData.DatiAgv[nagv].posprec = ClientData.DatiAgv[nagv].pos;
                ClientData.DatiAgv[nagv].pos = pos;
                update = true;
                // dmComandiAgv->AggiornaPrenotazioni();
            }
            ClientData.DatiAgv[nagv].idmis = idmiss;
            ClientData.DatiAgv[nagv].LivelloBatteria = livellobatt;
            ClientData.DatiAgv[nagv].stand_by = bypass & dmDB->bit[0];
            ClientData.DatiAgv[nagv].bumper_bypass = bypass & dmDB->bit[1];

            ClientData.DatiAgv[nagv].standby_operatore = bypass & dmDB->bit[4]; // !!! ATTENZIONE STANDBY OPERATORE

            ClientData.DatiAgv[nagv].registro5 = bypass;
            ClientData.DatiAgv[nagv].DestinazioneDaAGV = (bypass & 63) >> 2;
            if ((s[0] == '1') && (!ClientData.DatiAgv[nagv].bitbattbassa)) {
                ClientData.DatiAgv[nagv].bitbattbassa = true;
                dmDB->LogMsg("Ricevuto segnale da Agv di Batteria Bassa, capacita : " + IntToStr(livellobatt) + ", pos : " + IntToStr(pos));
            }
            else if (s[0] == '0') {
                if (ClientData.DatiAgv[nagv].bitbattbassa)
                    dmDB->LogMsg("Ricevuto reset segnale da Agv di Batteria Bassa, capacita : " + IntToStr(livellobatt) + ", pos : " + IntToStr(pos));
                ClientData.DatiAgv[nagv].bitbattbassa = false;
            }
            if (idmiss != 0) {
                stringa = "Select * from Missioni_view where id = " + IntToStr(idmiss);
                dmDB->QueryMissioniSocket->Close();
                dmDB->QueryMissioniSocket->SQL->Clear();
                dmDB->QueryMissioniSocket->SQL->Append(stringa);
                dmDB->QueryMissioniSocket->Open();
                if (dmDB->QueryMissioniSocket->RecordCount) {
                    ClientData.DatiAgv[nagv].tipomis = dmDB->QueryMissioniSocket->FieldByName("tipomissione")->AsInteger;
                    if ((ClientData.DatiAgv[nagv].tipomis < 2) && (!ClientData.DatiAgv[nagv].load))
                        ClientData.DatiAgv[nagv].dest = dmDB->QueryMissioniSocket->FieldByName("posprel")->AsInteger;
                    else
                        ClientData.DatiAgv[nagv].dest = dmDB->QueryMissioniSocket->FieldByName("posdep")->AsInteger;
                    ClientData.DatiAgv[nagv].prel = dmDB->QueryMissioniSocket->FieldByName("posprel")->AsInteger;
                    ClientData.DatiAgv[nagv].dep = dmDB->QueryMissioniSocket->FieldByName("posdep")->AsInteger;
                    ClientData.DatiAgv[nagv].latoforcheprel = dmDB->QueryMissioniSocket->FieldByName("latoforcheprel")->AsInteger;
                    ClientData.DatiAgv[nagv].latoforchedep = dmDB->QueryMissioniSocket->FieldByName("latoforchedep")->AsInteger;
                    ClientData.DatiAgv[nagv].nodopassaggioprel = dmDB->QueryMissioniSocket->FieldByName("nodopassaggioprel")->AsInteger;
                    ClientData.DatiAgv[nagv].nodopassaggiodep = dmDB->QueryMissioniSocket->FieldByName("nodopassaggiodep")->AsInteger;
                }
                dmDB->QueryMissioniSocket->Close();
            }
            else {
                ClientData.DatiAgv[nagv].dest = dest;
                ClientData.DatiAgv[nagv].prel = 0;
                ClientData.DatiAgv[nagv].dep = 0;
            }
            if (pos != 0) {
                if (s[7] == '1') {
                    if (!ClientData.DatiAgv[nagv].manuale) {
                        // taucs->SendEv(ev);
                        update = true;
                        dmDB->LogMsg("Agv numero " + IntToStr(n) + " in manuale in pos : " + IntToStr(pos));
                    }
                    // in manuale resetto i motori se sono in pos 1 o 2
                    ClientData.DatiAgv[nagv].manuale = true;
                    ClientData.DatiAgv[nagv].stand_by = 0;
                    // if (ClientData.DatiAgv[nagv].strcons != "")
                    // DMGestioneEventi->AggiungiRimuoviRichiestaConsensoPorta(ClientData.DatiAgv[nagv].strcons,0) ;
                    MainForm->strcons[nagv] = "";
                    ClientData.DatiAgv[nagv].richiestaconsenso = 0;
                    ClientData.DatiAgv[nagv].richiestaconsensonodo = 0;
                    ClientData.DatiAgv[nagv].consensonododato = 0;
                    // if ((ClientData.DatiAgv[nagv].consensodato) && (ClientData.DatiAgv[nagv].pos > 1000) && (ClientData.DatiAgv[nagv].pos <= 1005))
                    // ClientData.DatiAgv[nagv].consensodato = 0; // in cb resetto eventuali consensi
                    ClientData.DatiAgv[nagv].incarica = 0;
                    ClientData.DatiAgv[nagv].annullamissione = 0;
                    if (ClientData.DatiAgv[nagv].allarme_consenso) {
                        ClientData.DatiAgv[nagv].allarme_consenso = 0;
                        MainForm->MessageBar1->DelMsgNo(ClientData.DatiAgv[nagv].num_allarme_consenso); // alarm 502---506);
                        dmDB->AggiornaAllarmiAttivi();
                    }
                    if (ClientData.DatiAgv[nagv].allarme_carico) {
                        MainForm->MessageBar1->DelMsgNo(ClientData.DatiAgv[nagv].num_allarme_carico);
                        ClientData.DatiAgv[nagv].allarme_carico = 0;
                        dmDB->AggiornaAllarmiAttivi();
                    }
                    if (ClientData.DatiAgv[nagv].num_allarme_consenso_porta) {
                        ClientData.DatiAgv[nagv].num_allarme_consenso_porta = 0;
                        MainForm->MessageBar1->DelMsgNo(ClientData.DatiAgv[nagv].num_allarme_consenso_porta); // alarm 502---506);
                        dmDB->AggiornaAllarmiAttivi();
                    }
                    // se ho una richiesta alta e non sono nella posizione e neanche nel pos fuori ingombro lo resetto
                }
                else {
                    if (ClientData.DatiAgv[nagv].manuale) {
                        // taucs->SendEv(ev);
                        update = true;
                        dmDB->LogMsg("Agv numero " + IntToStr(n) + " in automatico in pos : " + IntToStr(pos) + " load : " + s[1] + " ; idmiss : " + IntToStr(idmiss));
                        // MainForm->InviaVelocita(ClientData.DatiAgv[nagv].vel_mis) ;
                    }
                    ClientData.DatiAgv[nagv].manuale = false;
                }
            }
            if (s[6] == '1')
                ClientData.DatiAgv[nagv].semiautomatico = 1;
            else
                ClientData.DatiAgv[nagv].semiautomatico = 0;

            if (s[2] == '0')
                ClientData.DatiAgv[nagv].incarica = false;
            else {
                ClientData.DatiAgv[nagv].incarica = true;
            }
            if (strcmp(ClientData.DatiAgv[nagv].allarme, str) != 0) {
                strcpy(ClientData.DatiAgv[nagv].allarme, str);
                dmDB->GestioneAllarmi(nagv);
                update = true;
            }
            if (s[1] == '1') {
                // ClientData.DatiAgv[nagv].load = true ;
                if (!ClientData.DatiAgv[nagv].load) {
                    ClientData.DatiAgv[nagv].load = true;
                    // dmDBImpianto->AggiornaStatoMissioniWMS(IDWMS, 3); // CHIEDI A MATTEO
                    dmDB->LogMsg("Ricevuto Agv con carico a bordo , Agv numero " + IntToStr(n) + " in pos : " + IntToStr(pos) + " ; idmiss : " + IntToStr(idmiss));
                    update = true;
                }
            }
            else if (ClientData.DatiAgv[nagv].load) {
                ClientData.DatiAgv[nagv].load = false;
                // dmDBImpianto->AggiornaStatoMissioniWMS(IDWMS, 4); // CHIEDI A MATTEO
                dmDB->LogMsg("Ricevuto Agv senza il carico a bordo , Agv numero " + IntToStr(n) + " in pos : " + IntToStr(pos) + " ; idmiss : " + IntToStr(idmiss));
                update = true;
            }
            if (update) {
                dmDB->AggiornaStatoAgv(nagv);
                ClientData.DatiAgv[nagv].disegna_agv = 1;
            }
            break;
        case EV_STATOMISSIONE:
            // sscanf(ev+2, "%d,%c,%d,%d,%d",&idmiss  , s , &pos, &id,&loaded);
            sscanf(ev + 2, "%d,%c,%d,%d,%d,%d", &idmiss, s, &pos, &id, &loaded, &pesata);
            nagv = id;
            if (ClientData.DatiAgv[nagv].pos != pos) {
                ClientData.DatiAgv[nagv].posprec = ClientData.DatiAgv[nagv].pos;
                ClientData.DatiAgv[nagv].pos = pos;
            }
            ClientData.DatiAgv[nagv].idmis = idmiss;
            ClientData.DatiAgv[nagv].load = loaded;
            stringa = "Select * from Missioni_view where id = " + IntToStr(idmiss);
            dmDB->QueryMissioniSocket->Close();
            dmDB->QueryMissioniSocket->SQL->Clear();
            dmDB->QueryMissioniSocket->SQL->Append(stringa);
            dmDB->QueryMissioniSocket->Open();
            esito_mis = "";
            if (dmDB->QueryMissioniSocket->RecordCount) {
                // tutto quello che mi serve dalla tab centromissioni lo leggo qui
                piano_prel = dmDB->QueryMissioniSocket->FieldByName("PianoPrel")->AsInteger;
                piano_dep = dmDB->QueryMissioniSocket->FieldByName("PianoDep")->AsInteger;
                id_centro_missione = dmDB->QueryMissioniSocket->FieldByName("IdCentroMissioni")->AsInteger;
                IDWMS = dmDB->QueryMissioniSocket->FieldByName("IDWMS")->AsInteger;
                esito_mis = dmDB->QueryMissioniSocket->FieldByName("Esito")->AsString;

            }
            switch (s[0]) {
            case 'I': // Missione Iniziata
                if (dmDB->QueryMissioniSocket->RecordCount) {
                    if (dmDB->QueryMissioniSocket->FieldByName("tipomissione")->AsInteger < 2)
                        ClientData.DatiAgv[nagv].dest = dmDB->QueryMissioniSocket->FieldByName("posprel")->AsInteger;
                    else
                        ClientData.DatiAgv[nagv].dest = dmDB->QueryMissioniSocket->FieldByName("posdep")->AsInteger;
                    ClientData.DatiAgv[nagv].tipomis = dmDB->QueryMissioniSocket->FieldByName("tipomissione")->AsInteger;
                    ClientData.DatiAgv[nagv].prel = dmDB->QueryMissioniSocket->FieldByName("posprel")->AsInteger;
                    ClientData.DatiAgv[nagv].dep = dmDB->QueryMissioniSocket->FieldByName("posdep")->AsInteger;
                    ClientData.DatiAgv[nagv].h_prel = dmDB->QueryMissioniSocket->FieldByName("hprel")->AsInteger;
                    ClientData.DatiAgv[nagv].h_dep = dmDB->QueryMissioniSocket->FieldByName("hdep")->AsInteger;
                    // ClientData.DatiAgv[nagv].da_etichettare = dmDB->QueryMissioniSocket->FieldByName("DaEtichettare")->AsInteger;

                    tipo_pallet = dmDB->QueryMissioniSocket->FieldByName("IdArtUDC")->AsInteger; // tipo pallet
                    //

                    prel = dmDB->QueryMissioniSocket->FieldByName("posprel")->AsInteger;

                    // corsia_prel = dmDB->QueryMissioniSocket->FieldByName("CorsiaPrel")->AsInteger;
                    /// MODIFICA CORSIA PREL
                    latoprel = dmDB->QueryMissioniSocket->FieldByName("latoprel")->AsInteger;
                    nodostop = dmDB->QueryMissioniSocket->FieldByName("NodoPassaggioPrel")->AsInteger;
                    // modifica solo deposito // 14_02_2022
                    latodep = dmDB->QueryMissioniSocket->FieldByName("latodep")->AsInteger;

                    dmComandiAgv->InvioGenVar(nagv, "tipo_pal", 'I', tipo_pallet); // modifica tipo pallet prelievo
                    dmDB->LogMsg("Inviato tipo pallet per prelievo a " + IntToStr(tipo_pallet));

                    if (ClientData.DatiAgv[nagv].tipomis == 0) { // prel-dep
                        nomeposprel = dmDB->QueryMissioniSocket->FieldByName("NomePosPrel")->AsString;
                        nomeposdep = dmDB->QueryMissioniSocket->FieldByName("nomeposdep")->AsString;
                        nodostop = dmDB->QueryMissioniSocket->FieldByName("NodoPassaggioPrel")->AsInteger;
                    }
                    else if (ClientData.DatiAgv[nagv].tipomis == 1) { // prel
                        nomeposprel = dmDB->QueryMissioniSocket->FieldByName("NomePosPrel")->AsString;
                        nomeposdep = "";
                        nodostop = dmDB->QueryMissioniSocket->FieldByName("NodoPassaggioPrel")->AsInteger;
                    }
                    else if (ClientData.DatiAgv[nagv].tipomis == 2) { // dep
                        nomeposprel = "";
                        nomeposdep = dmDB->QueryMissioniSocket->FieldByName("nomeposdep")->AsString;
                        nodostop = dmDB->QueryMissioniSocket->FieldByName("NodoPassaggioDep")->AsInteger; // passo dep all'inizio
                    }
                    else if (ClientData.DatiAgv[nagv].tipomis == 4) { // mov
                        nomeposprel = "";
                        nomeposdep = "";
                        nodostop = 0;
                    }
                    if ((ClientData.DatiAgv[nagv].da_etichettare) && (altezza_pallet <= dmDB->ParametriFunzionali.altezza_pallet_basso)) // !!!! DA METTERE A POSTO     altezza etichettatura
                    {
                        dmComandiAgv->InviaPalletBassoEtichettatura(1, nagv);
                    }
                    else
                    {
                        dmComandiAgv->InviaPalletBassoEtichettatura(0, nagv);
                    }
                    // modifica se missione manuale perdo id pallet // !!! ATTENZIONE
                    /* if (ClientData.DatiAgv[nagv].tipomis != 2)
                     {
                     if ((prel == 101) || (prel == 20)) // !!! DA METTERE A POSTO
                     {
                     ClientData.DatiAgv[nagv].codice_pallet = DMPlc->InserisciIdPalletInAgv(prel, nagv);
                     }
                     else
                     {
                     ClientData.DatiAgv[nagv].codice_pallet = dmDB->QueryMissioniSocket->FieldByName("idudc")->AsInteger;
                     }
                     } */
                    //
                    tipoplcprel = dmDB->QueryMissioniSocket->FieldByName("PosInputPLC_Prel")->AsInteger;
                    tipoplcdep = dmDB->QueryMissioniSocket->FieldByName("PosInputPLC_Dep")->AsInteger;
                }
                dmDB->QueryMissioniSocket->Close();
                dmDBServer->AggiornaMissione(idmiss, 1, nagv);
                // dmDBImpianto->AggiornaAgvMissioniWMS(IDWMS, nagv);
                // dmDBImpianto->AggiornaStatoMissioniWMS(IDWMS, 2);

                dmDB->LogMsg("Missione n° " + IntToStr(idmiss) + " iniziata da agv numero " + IntToStr(id));

                // CODICE BRICOMAN
                // invio Latoprel
                if (ClientData.DatiAgv[nagv].tipomis == 2)
                {
                    // dmComandiAgv->InvioGenVar(nagv, "latofork", 'I', latoprel);
                    dmComandiAgv->InvioGenVar(nagv, "latofork", 'I', latodep);
                    dmDB->LogMsg("Lato forche missione solo deposito AGV " + IntToStr(nagv) + " a " + IntToStr(latodep));
                }
                else
                {
                    dmComandiAgv->InvioGenVar(nagv, "latofork", 'I', latoprel);
                }
                // se dest e'una pos a terra lampeggio su posizione

                // 29-07-2021     invio pos
                // MODIFICA ?? 07/02/2022
                // dmComandiAgv->InvioGenVar(nagv, "nodostop", 'I', nodostop);
                ClientData.DatiAgv[nagv].fermaagv = 0;
                // dmDB->LogMsg("Inizio missione fermaagv: " + IntToStr(ClientData.DatiAgv[nagv].fermaagv) + " per Agv " + IntToStr(n));
                // FINE CODICE BRICOMAN
                // MainForm->DisegnaMappa();

                break;
            case 'P': // Prelievo effettuato
                prel = 0;
                corsia_prel = 0;
                if (dmDB->QueryMissioniSocket->RecordCount) {
                    tipomis = dmDB->QueryMissioniSocket->FieldByName("tipomissione")->AsInteger;
                    prel = dmDB->QueryMissioniSocket->FieldByName("posprel")->AsInteger;
                    corsia_prel = dmDB->FilaPosizione(prel);
                    /// MODIFICA CORSIA PREL
                    posdep = dmDB->QueryMissioniSocket->FieldByName("posdep")->AsInteger;
                    // bricoman: tolgo azzeramento, rimettere sugli altri impianti
                    ClientData.DatiAgv[nagv].DatiUDC.IDUDC = dmDB->QueryMissioniSocket->FieldByName("idudc")->AsInteger;
                    ClientData.DatiAgv[nagv].dest = dmDB->QueryMissioniSocket->FieldByName("posdep")->AsInteger;
                    id_centro_missione = dmDB->QueryMissioniSocket->FieldByName("IdCentroMissioni")->AsInteger;
                    IDWMS = dmDB->QueryMissioniSocket->FieldByName("IDWMS")->AsInteger;
                    tipologia_mov = dmDB->QueryMissioniSocket->FieldByName("CodTipoMovimento")->AsInteger;
                    tipologia_mis = dmDB->QueryMissioniSocket->FieldByName("CodTipoMissione")->AsInteger;
                    latodep = dmDB->QueryMissioniSocket->FieldByName("latodep")->AsInteger;
                    // tipoplcdep = dmDB->QueryMissioniSocket->FieldByName("PosInputPLC_Dep")->AsInteger;

                    ClientData.DatiAgv[nagv].DatiUDC.IDUDC = dmDB->QueryMissioniSocket->FieldByName("idudc")->AsInteger;
                    ClientData.DatiAgv[nagv].DatiUDC.IdArtUDC = dmDB->QueryMissioniSocket->FieldByName("idudc")->AsInteger;

                    ClientData.DatiAgv[nagv].tipo_pallet = dmDB->QueryMissioniSocket->FieldByName("IdArtUDC")->AsInteger; // è il tipo pallet

                }
                dmDB->QueryMissioniSocket->Close();
                dmDB->LogMsg("Prelievo missione " + IntToStr(idmiss) + " eseguito in posizione: " + IntToStr(pos) + " corsia " + corsia_prel + " da agv numero " + IntToStr(id));
                if (prel) {
                    // dmDBImpianto->AggiornaStatoMissioniWMS(IDWMS, 3);
                     dmDBServer->AggiornaMissione(idmiss, 2, nagv, pesata);
                    prenota = 0;

                    dmDB->PrenotaPos(prel, prenota, corsia_prel);
                    // S15-09-2022 SOLO HMC
                    if (corsia_prel == 100)
                        dmDB->ArticoloPrelevatoDepositato(prel, 0, 0, corsia_prel);
                    else
                        dmDB->ArticoloPrelevatoDepositato(prel, 0, piano_prel, corsia_prel);

                }
                else {
                    dmDB->LogMsg("Non Aggiorno dati perchè missione non trovata su prelievo , idmis: " + IntToStr(idmiss));
                }

                ClientData.DatiAgv[nagv].consensonododato = 0;
                ClientData.DatiAgv[nagv].consensodato = 0;
                ClientData.DatiAgv[nagv].richiestaconsensonodo = 0;
                break;
            case 'C': // Missione Completata
                posdep = 0;
                NoPrel = "";
                corsia_dep = 0;
                if (dmDB->QueryMissioniSocket->RecordCount) {
                    // posprel = dmDB->QueryMissioniSocket->FieldByName("posprel")->AsInteger ;
                    posdep = dmDB->QueryMissioniSocket->FieldByName("posdep")->AsInteger;
                    corsia_dep = dmDB->FilaPosizione(posdep);
                    tipomis = dmDB->QueryMissioniSocket->FieldByName("tipomissione")->AsInteger;
                    id_centro_missione = dmDB->QueryMissioniSocket->FieldByName("IdCentroMissioni")->AsInteger;
                    IDWMS = dmDB->QueryMissioniSocket->FieldByName("IDWMS")->AsInteger;
                    tipologia_mov = dmDB->QueryMissioniSocket->FieldByName("CodTipoMovimento")->AsInteger;
                    tipologia_mis = dmDB->QueryMissioniSocket->FieldByName("CodTipoMissione")->AsInteger;
                    tipoplcprel = dmDB->QueryMissioniSocket->FieldByName("PosInputPLC_Prel")->AsInteger;
                    tipoplcdep = dmDB->QueryMissioniSocket->FieldByName("PosInputPLC_Dep")->AsInteger;
                    if (((tipomis == 0) || (tipomis == 2)) && (!ClientData.DatiAgv[nagv].DatiUDC.IDUDC) && (dmDB->QueryMissioniSocket->FieldByName("idudc")->AsInteger)) {
                        ClientData.DatiAgv[nagv].DatiUDC.IDUDC = dmDB->QueryMissioniSocket->FieldByName("idudc")->AsInteger;

                    }
                }
                dmDB->LogMsg("Missione " + IntToStr(idmiss) + " completata, posizione: " + IntToStr(pos) + " da Agv n° " + IntToStr(id) + " peso ritornato " + IntToStr(pesata));
                dmDB->QueryMissioniSocket->Close();
                if (posdep > 0) {
                    dmDBServer->AggiornaMissione(idmiss, 3, nagv, pesata);
                    // dmDBImpianto->AggiornaStatoMissioniWMS(IDWMS, 4);

                    if ((tipomis == 0) || (tipomis == 2)) {
                        // deposito udc
                        // aggiorna Riferimento Ordine su Posizione deposito NB: va messo prima del riempimento della posizione senno' la trova piena e non aggiorna
                        dmDB->ArticoloPrelevatoDepositato(posdep, ClientData.DatiAgv[nagv].DatiUDC.IDUDC, piano_dep, corsia_dep);
                        dmDB->PrenotaPos(posdep, 0, corsia_dep);
                        // dmDB->SettaCorsiaInUso(dmDB->FilaPosizione(posdep), 0, 1); // setto alla creazione misisone in impostazione codice
                        // !!! DA METTERE A POSTO
                        /* if (posdep <= 20)
                         {
                         DMPlc->ScriviIdPalletPosizioneDiDeposito(piano_dep, posdep, nagv);
                         } */
                        // ClientData.DatiAgv[nagv].codice_pallet = 0; // mod pallet
                        if (ClientData.DatiAgv[nagv].DatiUDC.IdArtUDC) {
                            // dmDBServer->CompilaEdInviaRecordSap(ClientData.DatiAgv[nagv].DatiUDC,id_centro_missione,posdep," 10 ") ;//invio missione completata a sap
                            // EVITABILI????
                            dmDB->AggiornaUltimoIdUdcPerArticoloUdc(ClientData.DatiAgv[nagv].DatiUDC.IdArtUDC, ClientData.DatiAgv[nagv].DatiUDC.IDUDC); // al deposito imposto ultimo udc all'articolo
                        }
                    }
                    else if (tipomis == 4) {
                        dmDB->PrenotaPos(posdep, 0, corsia_dep);
                        // dmDB->SettaCorsiaInUso(dmDB->FilaPosizione(posdep), 0, 1);
                    }

                }
                else
                    dmDB->LogMsg("Non Aggiorno dati perchè missione non trovata, idmis: " + IntToStr(idmiss));
                ClientData.DatiAgv[nagv].idmis = 0;
                ClientData.DatiAgv[nagv].prel = 0;
                ClientData.DatiAgv[nagv].dep = 0;
                ClientData.DatiAgv[nagv].h_prel = 0;
                ClientData.DatiAgv[nagv].h_dep = 0;
                ClientData.DatiAgv[nagv].dep = 0;
                ClientData.DatiAgv[nagv].generata = 0;
                ClientData.DatiAgv[nagv].tipomis = -1;
                ClientData.DatiAgv[nagv].consensodato = 0;
                ClientData.DatiAgv[nagv].annullamissione = 0;
                ClientData.DatiAgv[nagv].richiestaconsenso = 0;
                ClientData.DatiAgv[nagv].richiestaconsensonodo = 0;
                ClientData.DatiAgv[nagv].consensonododato = 0;
                ClientData.DatiAgv[nagv].latoforcheprel = 0;
                ClientData.DatiAgv[nagv].latoforchedep = 0;
                ClientData.DatiAgv[nagv].nodopassaggioprel = 0;
                ClientData.DatiAgv[nagv].nodopassaggiodep = 0;
                ClientData.DatiAgv[nagv].fermaagv = 0;
                ClientData.DatiAgv[nagv].da_etichettare = 0;

                // dmDB->LogMsg("Fine missione fermaagv: " + IntToStr(ClientData.DatiAgv[nagv].fermaagv) + " per Agv " + IntToStr(nagv));

                // dmExtraFunction->StringToChar(DateTimeToStr(Now()), ClientData.DatiAgv[nagv].DataOraUltimaMissione);
                if (!loaded) {
                    ClientData.DatiAgv[nagv].DatiUDC.IDUDC = 0;
                    ClientData.DatiAgv[nagv].tipo_pallet = 0;
                    ClientData.DatiAgv[nagv].altezza_pallet = 0;
                    // dmDB->LeggiStrutturaUdc(ClientData.DatiAgv[nagv].DatiUDC);
                }

                break;
            case 'A': // Missione Abortita
                // MainForm->AbortMission(idmiss);
                corsia_prel = 0;
                corsia_dep = 0;
                if (dmDB->QueryMissioniSocket->RecordCount) {
                    // posprel = dmDB->QueryMissioniSocket->FieldByName(" posprel ")->AsInteger ;
                    prel = dmDB->QueryMissioniSocket->FieldByName("posprel")->AsInteger;
                    corsia_prel = dmDB->FilaPosizione(prel);
                    NoPrel = dmDB->QueryMissioniSocket->FieldByName("prelevato")->AsString;
                    posdep = dmDB->QueryMissioniSocket->FieldByName("posdep")->AsInteger;
                    corsia_dep = dmDB->FilaPosizione(posdep);
                    // corsiadep = dmDB->QueryMissioniSocket->FieldByName(" CorsiaDep ")->AsInteger ;
                    tipomis = dmDB->QueryMissioniSocket->FieldByName("tipomissione")->AsInteger;
                    id_centro_missione = dmDB->QueryMissioniSocket->FieldByName("IdCentroMissioni")->AsInteger;
                    IDWMS = dmDB->QueryMissioniSocket->FieldByName("IDWMS")->AsInteger;
                    tipoplcprel = dmDB->QueryMissioniSocket->FieldByName("PosInputPLC_Prel")->AsInteger;
                    tipoplcdep = dmDB->QueryMissioniSocket->FieldByName("PosInputPLC_Dep")->AsInteger;
                }
                dmDB->LogMsg("Missione n° " + IntToStr(idmiss) + " abortita su agv n° " + IntToStr(id) + " carico: " + IntToStr(loaded));
                dmDB->QueryMissioniSocket->Close();
                // guardare se ho un'anomalia sull'abort
                // guardo se devo uscire dall'ingombro
                if (!ClientData.DatiAgv[nagv].load) {
                    dmDBServer->AggiornaMissione(idmiss, 5, nagv);
                    // dmDBImpianto->AggiornaStatoMissioniWMS(IDWMS, 6);
                }
                else {
                    dmDBServer->AggiornaMissione(idmiss, 4, nagv);
                    // dmDBImpianto->AggiornaStatoMissioniWMS(IDWMS, 5);
                }
                if (NoPrel == "") {
                    // devo azzerare la chiamata
                    prenota = 0;
                    dmDB->PrenotaPos(prel, prenota, corsia_prel);
                }

                prenota = 0;
                dmDB->PrenotaPos(posdep, prenota, corsia_dep);
                ClientData.DatiAgv[nagv].consensodato = 0;
                ClientData.DatiAgv[nagv].generata = 0;
                ClientData.DatiAgv[nagv].idmis = 0;
                ClientData.DatiAgv[nagv].prel = 0;
                ClientData.DatiAgv[nagv].dep = 0;
                ClientData.DatiAgv[nagv].h_prel = 0;
                ClientData.DatiAgv[nagv].h_dep = 0;
                ClientData.DatiAgv[nagv].tipomis = -1;
                ClientData.DatiAgv[nagv].annullamissione = 0;
                ClientData.DatiAgv[nagv].richiestaconsenso = 0;
                ClientData.DatiAgv[nagv].richiestaconsensonodo = 0;
                ClientData.DatiAgv[nagv].consensonododato = 0;
                ClientData.DatiAgv[nagv].latoforcheprel = 0;
                ClientData.DatiAgv[nagv].latoforchedep = 0;
                ClientData.DatiAgv[nagv].nodopassaggioprel = 0;
                ClientData.DatiAgv[nagv].nodopassaggiodep = 0;
                ClientData.DatiAgv[nagv].fermaagv = 0;

                // MODIFICA PIANO
                ClientData.DatiAgv[nagv].piano_prelievo = 0;
                ClientData.DatiAgv[nagv].piano_deposito = 0;
                ClientData.DatiAgv[nagv].da_etichettare = 0;
                //

                // dmDB->LogMsg("Abort missione fermaagv: " + IntToStr(ClientData.DatiAgv[nagv].fermaagv) + " per Agv " + IntToStr(nagv));
                // dmExtraFunction->StringToChar(DateTimeToStr(Now()), ClientData.DatiAgv[nagv].DataOraUltimaMissione);
                if (!loaded) {
                    ClientData.DatiAgv[nagv].DatiUDC.IDUDC = 0;
                    ClientData.DatiAgv[nagv].tipo_pallet = 0;
                    ClientData.DatiAgv[nagv].altezza_pallet = 0;
                    // dmDB->LeggiStrutturaUdc(ClientData.DatiAgv[nagv].DatiUDC);
                    // ClientData.DatiAgv[nagv].codice_pallet = 0; // mod pallet
                }
            }
            dmDB->aggiorna_tab_posizioni_locale = 1;
            dmDB->AggiornaStatoAgv(nagv);
            ClientData.ridisegna_mappa = 1;
            ClientData.DatiAgv[nagv].disegna_agv = 1;
            // dmComandiAgv->AggiornaPrenotazioni();
        }
    }
    if ((dmDB->ADOConnection1->Connected) && (dmDB->lettostatoagv) && (MainForm->count_inizio > 5)) {
        sprintf(answer, "OK,%s", ev);
        SendAnswer(answer, Socket);
        // MainForm->AggiornaStatoAgv() ;
    }
}

#pragma package(smart_init)
