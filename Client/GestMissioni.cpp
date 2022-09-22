// ---------------------------------------------------------------------------

#pragma hdrstop

#include "GestMissioni.h"
#include "dmFunzioniComuniClientServer.h"
#include "clientdata.h"
#include "ExtraFunction.h"
#include "percorsiprototipo.h"
#include "DBClient.h"
#include "main.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TdmGestMissioni *dmGestMissioni;
extern int indice_inizio_campi[NCAMPI];
extern TCentroMissione CentroMissione;
extern DATILGV tab;

// ---------------------------------------------------------------------------
__fastcall TdmGestMissioni::TdmGestMissioni(TComponent* Owner)
    : TDataModule(Owner)
{
}

void TdmGestMissioni::InsertUDC1Click(TMenuItem *Item, int agvdaspostare) {
    // NON GESTITO
   /* AnsiString str, code, s;
    int corsia_dep, posdep;

    bool esegui = true;
    if (!ClientData.ParametriFunzionali.AbilitaMissioni) {
        if (Application->MessageBox(L"Mission disabled. Do you want to create mission ? ", L"Confirm !", MB_YESNO) == IDNO)
            esegui = false;
    }
    if (esegui) {
        if (Item != NULL) {
            corsia_dep = dmDB->ReturnFilaDaPos(Item->Tag);
            posdep = dmDB->ControllaDepositoOk(Item->Tag, 0);
            if (!posdep)
                dmDB->ControllaDepositoOk(Item->Tag, 1);
            if ((posdep) || (Application->MessageBox(L"Unreachable Drop Position, Do you want to create the missione the same ? ", L"Confirm !!!", MB_YESNO) == IDYES)) {
                // inviare missione al server
                if (!posdep)
                    posdep = Item->Tag;
                // controllare
                CentroMissione.Agv = agvdaspostare;
                CentroMissione.posprel = ClientData.DatiAgv[agvdaspostare].pos;
                CentroMissione.posdep = posdep;
                CentroMissione.h_prel = dmDB->RitornaAltezzedaPosizione(CentroMissione.posprel, 1, "HPREL");
                CentroMissione.h_dep = dmDB->RitornaAltezzedaPosizione(CentroMissione.posdep, 1, "HDEP");

                CentroMissione.TipoMissione = 2;
                CentroMissione.Priorita = 2;
                CentroMissione.IDUDC = 0;
                CentroMissione.CorsiaDeposito = "";
                CentroMissione.CodTipoMovimento = 1;
                dmDB->GeneraCentroMissione(CentroMissione);

                // SocketDataModule->InviaMissioneClient(ClientData.DatiAgv[agvdaspostare].pos, posdep, 4, agvdaspostare);

            }
        }
        agvdaspostare = 0;
    }   */
}

void TdmGestMissioni::DepositaUDC1Click(TMenuItem *Item) {
    // DEPOSITA UDC DA SPOSTAMENTO
    AnsiString str, code, s;
    int corsia_dep, posdep;
    bool esegui = true;
    // TCentroMissione CentroMissione;
    if (!ClientData.ParametriFunzionali.AbilitaMissioni) {
        if (Application->MessageBox(L"Missioni disabilitate, vuoi creare ugualmente il Centro Missione? ", L"Conferma !", MB_YESNO) == IDNO)
            esegui = false;
    }
    if (esegui) {
        if (Item != NULL) {
            corsia_dep = dmDB->ReturnFilaDaPos(Item->Tag);
            posdep = dmDB->ControllaDepositoOk(Item->Tag, 0, 1);
            if (posdep <= 0)
                posdep = dmDB->ControllaDepositoOk(Item->Tag, 1, 1);
            if ((posdep > 0) || ((posdep <= 0) && (Item->Tag))) {
                // if (Application->MessageBox(L"Position not reachable or reserved. Do you want to create mission? ", L"Confirm ", MB_YESNO) == IDYES) {
                // inviare missione al server
                if (posdep <= -1)
                    posdep = Item->Tag;
                CentroMissione.Agv = 0;
                CentroMissione.IDUDC = 0; // lo valorizzo nella generacentromissione
                CentroMissione.posdep = posdep;
                CentroMissione.Priorita = 2;
                CentroMissione.CorsiaDeposito = dmDB->ReturnNomeCorsiaDaPos(Item->Tag);
                CentroMissione.TipoMissione = 2;
                CentroMissione.CodTipoMissione = 0;
                CentroMissione.CodTipoMovimento = 1;
                dmDB->GeneraCentroMissione(CentroMissione);
            }
        }
        // dmDB->SvuotaStruturaCentroMissioni(0);
    }
}

void TdmGestMissioni::MisManSpost1Click(TMenuItem *Item) {

  /*  AnsiString str, code;
    int corsia_dep, posdep;
    bool esegui = true;
    // TCentroMissione CentroMissione;
    if (!ClientData.ParametriFunzionali.AbilitaMissioni) {
        if (Application->MessageBox(L"Missioni disabilitate, vuoi creare ugualmente il Centro Missione? ", L"Confirm !", MB_YESNO) == IDNO)
            esegui = false;
    }
    if (esegui) {
        if (Item != NULL) {
            if ((CentroMissione.posprel == 0)) {
                CentroMissione.posprel = Item->Tag;
                CentroMissione.CodTipoMovimento = 3; // spostamento
                CentroMissione.IDUDC = dmDBClient->TornaUDCinPosizione(CentroMissione.posprel);
            }
            else {
                CentroMissione.posdep = Item->Tag;
            }
            if (CentroMissione.IDUDC == 0) {
                // annullo la selezione di partrenza
                CentroMissione.posprel = 0;
            }

            if ((CentroMissione.posprel > 0) && (CentroMissione.posdep > 0)) {
                CentroMissione.Agv = 0;
                CentroMissione.CorsiaDeposito = dmDB->ReturnNomeCorsiaDaPos(Item->Tag);
                CentroMissione.CodTipoMissione = 0; // MISSIONE MANUALE
                CentroMissione.TipoMissione = 0; // car-scar
                CentroMissione.Priorita = 2;
                CentroMissione.pianoprel = dmDB->PrimoPianoOk(CentroMissione.posprel) - 1;
                CentroMissione.pianodep = dmDB->PrimoPianoOk(CentroMissione.posdep);
                // setto prel primopianook a -1 dando per buono   che sara' sempre >1 per via di un udc presente
                CentroMissione.h_prel = dmDB->RitornaAltezzedaPosizione(CentroMissione.posprel, CentroMissione.pianoprel, "HPREL");
                CentroMissione.h_dep = dmDB->RitornaAltezzedaPosizione(CentroMissione.posdep, CentroMissione.pianodep, "HDEP");
                if ((CentroMissione.h_prel > 10) && (CentroMissione.h_dep > 10)) { // se e' minore c'e' un problema tipo posizione piena
                    dmDB->GeneraCentroMissione(CentroMissione);
                    CentroMissione.posprel = 0; // AZZERO
                    CentroMissione.posdep = 0; // AZZERO
                    dmDB->LogMsg("Lancio missione manuale MisManSpost1Click");
                }
                else
                    dmDB->LogMsg("Errore di pos piena per missione manuale MisManSpost1Click");
            }
        }
    }    */
}

void TdmGestMissioni::PesaUDC1Click(TMenuItem *Item) {

  /*  if (Item != NULL) {
        // dmDB->SvuotaStruturaCentroMissioni(0);
        // FormSceltaModuli->ShowModal();
        if (CentroMissione.DestinazioneModuli == 0) {
            // sono uscito annullando la scelta
            ShowMessage("Scelta annullata per prelievo in " + IntToStr(Item->Tag));
        }
        else {
            CentroMissione.posprel = Item->Tag;
            CentroMissione.CodTipoMovimento = 3;
            ShowMessage("Seleziona destinazione per UDC prelevato in " + IntToStr(Item->Tag) + "(tasto dx del mouse)");
        }
    }        */
}

void TdmGestMissioni::MoveMisMan1Click(TMenuItem *Item) {
    // missione manuale con numero posizione da input manuale
  /*  int agv;
    AnsiString posman;
    int posint, posdep, pianoprel, pianodep, hprel, hdep;
    bool esegui = true;

    if (!ClientData.ParametriFunzionali.AbilitaMissioni) {
        if (Application->MessageBox(L"Missioni disabilitate, vuoi creare ugualmente il Centro Missione", L"Conferma !", MB_YESNO) == IDNO)
            esegui = false;
    }
    if (esegui) {
        if (Item != NULL) {
            posman = InputBox(L"Imposta il nodo di destinazione", L"Movimento", L"0");
            posman = posman.Trim();
            posint = posman.ToIntDef(0);
            if (tab.mappa_nodi.find(posint) != tab.mappa_nodi.end()) {
                posdep = dmDB->ControllaDepositoOk(posint, 0, 1);
                if (posdep <= 0)
                    posdep = dmDB->ControllaDepositoOk(posint, 1, 1);
                if ((!ClientData.DatiAgv[Item->Tag].load) || (posdep > 0)
                    || (Application->MessageBox(L"Posizione irraggiungibile o impegnata. Vuoi creare ugualmente il Centro Missione ? ", L"Confirm !!!", MB_YESNO) == IDYES)) {
                    if (posdep <= 0) { // se non è una posizione di prel/dep
                        posdep = posint;
                    }
                    CentroMissione.Agv = Item->Tag;
                    CentroMissione.posprel = ClientData.DatiAgv[Item->Tag].pos;
                    CentroMissione.posdep = posdep;
                    pianoprel = dmDB->PrimoPianoOk(CentroMissione.posprel) - 1;
                    pianodep = dmDB->PrimoPianoOk(CentroMissione.posdep);
                    CentroMissione.pianoprel = (pianoprel <= 0 ? 1 : pianoprel);
                    CentroMissione.pianodep = (pianodep <= 0 ? 1 : pianodep);
                    hprel = dmDB->RitornaAltezzedaPosizione(CentroMissione.posprel, CentroMissione.pianoprel, "HPREL");
                    hdep = dmDB->RitornaAltezzedaPosizione(CentroMissione.posdep, CentroMissione.pianodep, "HDEP");
                    CentroMissione.h_prel = (hprel <= 0 ? dmDB->quota_dep_default : hprel);
                    CentroMissione.h_dep = (hdep <= 0 ? dmDB->quota_dep_default : hdep);
                    CentroMissione.Priorita = 1;
                    CentroMissione.IDUDC = ClientData.DatiAgv[Item->Tag].DatiUDC.IDUDC;
                    CentroMissione.TipoMissione = 4; // spostamento
                    CentroMissione.CodTipoMissione = 4; // MISSIONE movimento
                    CentroMissione.CodTipoMovimento = 4; // spostamento
                    CentroMissione.CorsiaDeposito = dmDB->ReturnNomeCorsiaDaPos(Item->Tag);
                    dmDB->GeneraCentroMissione(CentroMissione);
                }
            }
            else
                if (posint > 0) {
                Application->MessageBox(L"Posizione o nodo non trovato", L"Missione non creata", MB_OK);
            }
        }
    }  */
}

void TdmGestMissioni::DropAgv1Click(TMenuItem *Item, int agvdaspostare) {
    //
   /* AnsiString str, code, s;
    int corsia_dep, posdep;
    bool esegui = true;

    if (!ClientData.ParametriFunzionali.AbilitaMissioni) {
        if (Application->MessageBox(L"Missioni disabilitate, vuoi creare ugualmente il Centro Missione", L"Conferma !", MB_YESNO) == IDNO)
            esegui = false;
    }
    if (esegui) {
        if ((Item != NULL) && (agvdaspostare) && (ClientData.DatiAgv[agvdaspostare].load)) {
            posdep = dmDB->ControllaDepositoOk(Item->Tag, 0, 1);
            if (!posdep)
                posdep = dmDB->ControllaDepositoOk(Item->Tag, 1, 1);
            if ((posdep) || (Application->MessageBox(L"Posizione irraggiungibile o impegnata, vuoi creare ugualmente il Centro Missione?", L"Conferma ", MB_YESNO) == IDYES)) {
                // controllare
                CentroMissione.Agv = agvdaspostare;
                CentroMissione.posprel = ClientData.DatiAgv[agvdaspostare].pos;
                CentroMissione.posdep = posdep;
                CentroMissione.pianoprel = dmDB->PrimoPianoOk(CentroMissione.posprel) - 1;
                CentroMissione.pianodep = dmDB->PrimoPianoOk(CentroMissione.posdep);
                CentroMissione.h_prel = dmDB->RitornaAltezzedaPosizione(CentroMissione.posprel, CentroMissione.pianoprel, "HPREL");
                CentroMissione.h_dep = dmDB->RitornaAltezzedaPosizione(CentroMissione.posdep, CentroMissione.pianodep, "HDEP");
                CentroMissione.Priorita = 1;
                // CentroMissione.IDUDC = 0;
                CentroMissione.IDUDC = ClientData.DatiAgv[agvdaspostare].DatiUDC.IDUDC;
                CentroMissione.TipoMissione = 2; // deposito
                CentroMissione.CodTipoMissione = 0; // MISSIONE MANUALE
                CentroMissione.CodTipoMovimento = 2;
                CentroMissione.CorsiaDeposito = dmDB->ReturnNomeCorsiaDaPos(Item->Tag);
                dmDB->GeneraCentroMissione(CentroMissione);
            }
            else
                ShowMessage("Unreachable Drop Position");
        }
        agvdaspostare = 0;
    }       */
}

void TdmGestMissioni::MoveAgv1Click(TMenuItem *Item, int agvdaspostare) {
    // missione manuale    movimento e/o spostamento
 /*   AnsiString str, code, s;
    int corsia_dep, posdep, pianoprel, pianodep, hprel, hdep;

    bool esegui = true;

    if (!ClientData.ParametriFunzionali.AbilitaMissioni) {
        if (Application->MessageBox(L"Missioni disabilitate, vuoi creare ugualmente il Centro Missione?", L"Conferma !", MB_YESNO) == IDNO)
            esegui = false;
    }
    if (esegui) {
        if ((Item != NULL) && (agvdaspostare)) {
            corsia_dep = dmDB->ReturnFilaDaPos(Item->Tag);
            posdep = dmDB->ControllaDepositoOk(Item->Tag, 0, 1);
            if (!posdep)
                dmDB->ControllaDepositoOk(Item->Tag, 1, 1);
            if ((posdep) || (Application->MessageBox(L"Posizione irraggiungibile o impegnata, vuoi creare ugualmente il Centro Missione", L"Conferma ", MB_YESNO) == IDYES)) {
                // inviare missione al server
                if (!posdep)
                    posdep = Item->Tag;
                // controllare
                CentroMissione.Agv = agvdaspostare;
                CentroMissione.posprel = ClientData.DatiAgv[agvdaspostare].pos;
                CentroMissione.posdep = posdep;

                CentroMissione.Priorita = 1;
                CentroMissione.IDUDC = ClientData.DatiAgv[agvdaspostare].DatiUDC.IDUDC;
                CentroMissione.TipoUDC = 0;
                CentroMissione.CorsiaDeposito = dmDB->ReturnNomeCorsiaDaPos(Item->Tag);
                CentroMissione.CodTipoMissione = 4; // MISSIONE movimento
                CentroMissione.TipoMissione = 4;
                CentroMissione.CodTipoMovimento = 4;
                pianoprel = dmDB->PrimoPianoOk(CentroMissione.posprel) - 1;
                pianodep = dmDB->PrimoPianoOk(CentroMissione.posdep);
                CentroMissione.pianoprel = (pianoprel <= 0 ? 1 : pianoprel);
                CentroMissione.pianodep = (pianodep <= 0 ? 1 : pianodep);
                hprel = dmDB->RitornaAltezzedaPosizione(CentroMissione.posprel, CentroMissione.pianoprel, "HPREL");
                hdep = dmDB->RitornaAltezzedaPosizione(CentroMissione.posdep, CentroMissione.pianodep, "HDEP");
                CentroMissione.h_prel = (hprel <= 0 ? dmDB->quota_dep_default : hprel);
                CentroMissione.h_dep = (hdep <= 0 ? dmDB->quota_dep_default : hdep);
                dmDB->GeneraCentroMissione(CentroMissione);
            }
        }
        agvdaspostare = 0;
    }   */
}

void TdmGestMissioni::GestisciMissioniPeso(int pos) {
  /*  TADOQuery *ADOQuery;
    int res = 0;
    AnsiString strsql;
    if (pos == dmDB->PosPesoCampione) {
        if (!dmDB->ADOConnection1->Connected)
            return;
        try {
            ADOQuery = new TADOQuery(NULL);
            ADOQuery->Connection = dmDB->ADOConnection1;
            // strsql = "UPDATE centromissioni SET stato=" + String(ESITO_MISSIONE_ANNULLATA) + " where tipomissione=" + String(ID_MISSIONE_PESO_CAMPIONE) + " and stato=1";
            ADOQuery->Close();
            ADOQuery->SQL->Clear();
            ADOQuery->SQL->Text = strsql;
            res = ADOQuery->ExecSQL();
            dmDB->LogMsg(strsql);
        }
        catch (...) {
            // ADOConnection1->Close();
            dmDB->LogMsg("Eccezione su disattivazione pesata :" + strsql);
        }
        delete ADOQuery;
    }  */
}

void TdmGestMissioni::MisManSpostPLCClick(TMenuItem *Item) {
 /*   AnsiString str, code;
    int corsia_dep, posdep;
    bool esegui = true;
    // TCentroMissione CentroMissione;
    if (!ClientData.ParametriFunzionali.AbilitaMissioni) {
        if (Application->MessageBox(L"Missioni disabilitate, vuoi creare ugualmente il Centro Missione? ", L"Confirm !", MB_YESNO) == IDNO)
            esegui = false;
    }
    if (esegui) {
        if (Item != NULL) {

            CentroMissione.CodTipoMovimento = 0; // spostamento
            CentroMissione.IDUDC = dmDBClient->TornaUDCinPosizione(CentroMissione.posprel);
            if ((CentroMissione.posprel > 0) && (CentroMissione.posdep > 0)) {
                CentroMissione.Agv = 0;
                CentroMissione.CorsiaDeposito = dmDB->ReturnNomeCorsiaDaPos(CentroMissione.posdep);
                CentroMissione.CodTipoMissione = 0; // MISSIONE MANUALE
                CentroMissione.TipoMissione = 0; // car-scar
                CentroMissione.Priorita = 2;
                CentroMissione.h_prel = dmDB->RitornaAltezzedaPosizione(CentroMissione.posprel, CentroMissione.pianoprel, "HPREL");
                CentroMissione.h_dep = dmDB->RitornaAltezzedaPosizione(CentroMissione.posdep, CentroMissione.pianodep, "HDEP");
                if ((CentroMissione.h_prel > 10) && (CentroMissione.h_dep > 10)) { // se e' minore c'e' un problema tipo posizione piena
                    dmDB->GeneraCentroMissione(CentroMissione);
                    CentroMissione.posprel = 0; // AZZERO
                    CentroMissione.posdep = 0; // AZZERO
                    CentroMissione.pianoprel = 0; // AZZERO
                    CentroMissione.pianodep = 0; // AZZERO
                    dmDB->LogMsg("Lancio missione manuale MisManSpost1Click");
                }
                else
                    dmDB->LogMsg("Errore di pos piena per missione manuale MisManSpost1Click");
            }
        }
    }     */
}
