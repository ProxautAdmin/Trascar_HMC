// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "config.h"
#include "main.h"
#include "DataExchange.h"
#include "SocketComunicazioneClient.h"
#include "ExtraFunction.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma link "selectAgvframe"
#pragma link "configCBFrame"
#pragma resource "*.dfm"
TfrConfig *frConfig;
extern char N_carrelli;

// ---------------------------------------------------------------------------
__fastcall TfrConfig::TfrConfig(TComponent* Owner)
    : TMDIChild(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TfrConfig::FormActivate(TObject *Sender) {
    int i, val, valcb;
    numframe = 8;
    AnsiString Funzione = "CONFIG";
    /*
     PARAMETRI:
     01-09 missions settings
     10-19 priorita'
     30-39 posizioni caricabatteria
     50-79 Parametri funzionali vari (peso ecc.)
     80-99 Settaggi interni (traccia, info ecc)
     */

    TIndexList TabParametri;
    TfrSelectAgv *frSelAgv;
    TComboBox *Combo;
    TCheckBox *Check;
    TfrCB *frCB;

    dmDB->CaricaTabellaK("Parametri order by IdParametro", "IdParametro", TabParametri);

    // crea posizioni combo cb
    for (i = 1; i <= NAGV; i++) {
        frCB = (TfrCB*) FindComponent("frCB" + IntToStr(i));

        if (frCB == NULL) {
            frCB = new TfrCB(this);
            frCB->Parent = this->pBatteria;
            frCB->Name = "frCB" + IntToStr(i);
        }
        // operazioni comuni per tutti
        frCB->GroupBox1->Caption = "Agv " + String(i) + "                       Pos BC";
        frCB->Hint = "frCB" + IntToStr(i);
        frCB->Tag = i;
        frCB->Left = 10 + ((float) frCB->Width * 1.05f) * ((i - 1) % 4);
        frCB->Top = 10 + ((float) frCB->Height * 1.10f) * ((i - 1) / 4);
        frCB->RiempiCampi(i);
    }

    // FRAME
    for (i = 1; i <= numframe; i++) {
        frSelAgv = (TfrSelectAgv*) FindComponent("frSelectAgv" + IntToStr(i));
        if (frSelAgv != NULL) {
            frSelAgv->RiempiCampi(i - 1);
        }
    }

    // caricabatterie (da 31 a 39)
    num_params = dmDBClient->RitornaMaxIDParametro(); // numero massimo ID parametri
    // 50-79 Parametri funzionali vari (peso ecc.)
    lePesoCamp->Text = TabParametri[50]["INTVALUE"].ToIntDef(0);
    lePesoCamp->EditLabel->Caption = DataForm->Traduci(Funzione, 1);
    lePesoFreq->Text = TabParametri[51]["INTVALUE"].ToIntDef(0);
    lePesoFreq->EditLabel->Caption = DataForm->Traduci(Funzione, 2);
    cbPrioritaPeso->Text = TabParametri[52]["INTVALUE"].ToIntDef(0);
    leTolleranza->Text = TabParametri[53]["INTVALUE"].ToIntDef(0);
    leTolleranza->EditLabel->Caption = DataForm->Traduci(Funzione, 3);
    cMissioniPeso->Checked = TabParametri[54]["INTVALUE"].ToIntDef(0);
    leNumMissioni->Text = TabParametri[56]["INTVALUE"].ToIntDef(0);
    // sospensione lavoro 60 - 70
    cbDaGiorno1->Text = dmExtraFunction->RiempiGiorniSettimana(cbDaGiorno1, TabParametri[60]["INTVALUE"].ToIntDef(0));
    cbAGiorno1->Text = dmExtraFunction->RiempiGiorniSettimana(cbAGiorno1, TabParametri[61]["INTVALUE"].ToIntDef(0));
    DaOra1->Time = StrToTime(TabParametri[70]["STRINGVALUE"]);
    AOra1->Time = StrToTime(TabParametri[71]["STRINGVALUE"]);

    // parametri gestione programma    da 80
    cAGVMov->Checked = TabParametri[80]["INTVALUE"].ToIntDef(0);
    cInfoBox->Checked = TabParametri[81]["INTVALUE"].ToIntDef(0);
    ckAbilitaCheckGiornoNotte->Checked = TabParametri[83]["INTVALUE"].ToIntDef(0);

    // parametro pallet basso BONFANTI
    editAltezzaPalletBasso->Text = TabParametri[90]["INTVALUE"].ToIntDef(0);

    for (i = 1; i <= num_params; i++) {
        Val_Init_Params[i] = TabParametri[i]["INTVALUE"].ToIntDef(0);
        Str_Init_Params[i] = TabParametri[i]["STRINGVALUE"];
    }

    // abilitazioni
    pAttivita->Visible = true;

}

// ---------------------------------------------------------------------------
void __fastcall TfrConfig::BitBtnChangeClick(TObject *Sender) {
    int i, val, valp = 0, invia_aggiornamento = 0;
    TfrSelectAgv *frSelAgv;
    TComboBox *Combo;
    TCheckBox *Check;

    if (dmDB->pwdlevel) {
        for (i = 1; i <= numframe; i++) {
            frSelAgv = (TfrSelectAgv*) FindComponent("frSelectAgv" + IntToStr(i));
            if (frSelAgv != NULL) {
                frSelAgv->SalvaCampi(i - 1);
            }
        }

        // crea posizioni combo cb

        for (i = 1; i <= NAGV; i++) {
            frCB = (TfrCB*) FindComponent("frCB" + IntToStr(i));
            if (frCB != NULL) {
                valp = invia_aggiornamento + frCB->SalvaCampi(i, valp);
            }
        }
        if (Val_Init_Params[30] != valp) {
            dmDB->AggiornaParametri(30, valp);
            invia_aggiornamento = 1;
        }

        if (Val_Init_Params[50] != lePesoCamp->Text) {
            dmDB->AggiornaParametri(50, lePesoCamp->Text.ToIntDef(0));
            invia_aggiornamento = 1; // devo inviare un messaggio al server per ricaricare i parametri
        }
        if (Val_Init_Params[51] != lePesoFreq->Text) {
            dmDB->AggiornaParametri(51, lePesoFreq->Text.ToIntDef(0));
            invia_aggiornamento = 1; // devo inviare un messaggio al server per ricaricare i parametri
        }
        if (Val_Init_Params[52] != cbPrioritaPeso->Text.SubString(1, 1).ToIntDef(0)) {
            dmDB->AggiornaParametri(52, cbPrioritaPeso->Text.SubString(1, 1).ToIntDef(0));
            invia_aggiornamento = 1; // devo inviare un messaggio al server per ricaricare i parametri
        }
        if (Val_Init_Params[53] != leTolleranza->Text) {
            dmDB->AggiornaParametri(53, leTolleranza->Text.ToIntDef(0));
            invia_aggiornamento = 1; // devo inviare un messaggio al server per ricaricare i parametri
        }

        if (Val_Init_Params[54] != cMissioniPeso->Checked) {
            dmDB->AggiornaParametri(54, cMissioniPeso->Checked);
            invia_aggiornamento = 1; // devo inviare un messaggio al server per ricaricare i parametri
        }

        if (Val_Init_Params[56] != leNumMissioni->Text.ToIntDef(0)) {
            dmDB->AggiornaParametri(56, leNumMissioni->Text.ToIntDef(0));
            invia_aggiornamento = 1; // devo inviare un messaggio al server per ricaricare i parametri
        }

        // giorni
        if (Val_Init_Params[60] != StrToInt(cbDaGiorno1->Text.SubString(1, 1))) {
            dmDB->AggiornaParametri(60, StrToInt(cbDaGiorno1->Text.SubString(1, 1)));
            invia_aggiornamento = 1; // devo inviare un messaggio al server per ricaricare i parametri
        }
        if (Val_Init_Params[61] != StrToInt(cbAGiorno1->Text.SubString(1, 1))) {
            dmDB->AggiornaParametri(61, StrToInt(cbAGiorno1->Text.SubString(1, 1)));
            invia_aggiornamento = 1; // devo inviare un messaggio al server per ricaricare i parametri
        }
        // ora
        if (Str_Init_Params[70] != TimeToStr(DaOra1->Time)) {
            dmDB->AggiornaParametri(70, 0, TimeToStr(DaOra1->Time));
            invia_aggiornamento = 1; // devo inviare un messaggio al server per ricaricare i parametri
        }
        if (Str_Init_Params[71] != TimeToStr(AOra1->Time)) {
            dmDB->AggiornaParametri(71, 0, TimeToStr(AOra1->Time));
            invia_aggiornamento = 1; // devo inviare un messaggio al server per ricaricare i parametri
        }
        if (Val_Init_Params[80] != cAGVMov->Checked) {
            dmDB->AggiornaParametri(80, cAGVMov->Checked);
            invia_aggiornamento = 1; // devo inviare un messaggio al server per ricaricare i parametri
        }
        if (Val_Init_Params[83] != ckAbilitaCheckGiornoNotte->Checked) {
            dmDB->AggiornaParametri(83, ckAbilitaCheckGiornoNotte->Checked);
            invia_aggiornamento = 1; // devo inviare un messaggio al server per ricaricare i parametri
        }

        // parametro pallet basso BONFANTI  / altezza pallet HMC
        if (Val_Init_Params[90] != editAltezzaPalletBasso->Text) {
            dmDB->AggiornaParametri(90, editAltezzaPalletBasso->Text.ToIntDef(0));
            invia_aggiornamento = 1; // devo inviare un messaggio al server per ricaricare i parametri
        }

        if (invia_aggiornamento)
            SocketDataModule->InviaAggiornamentoParametri(0);
        SocketDataModule->InviaAggiornamentoCB(0); // riabilitare??????
    }
    // dopo scrittura, refresh dei parametri
    dmDB->LeggiParametri();

    Close();
}
// ---------------------------------------------------------------------------

void __fastcall TfrConfig::Timer1Timer(TObject *Sender) {
    int i;
    TfrSelectAgv *frSelAgv;

    pExtra->Visible = true;
    pPesi->Visible = false;

    BitBtnChange->Enabled = dmDB->pwdlevel;

    // btnApplicaModifiche->Enabled = dmDB->pwdlevel;

    BitBtnPos->Enabled = false; //(dmDB->pwdlevel >= 8);
    btStoricizza->Enabled = (dmDB->pwdlevel > 5);
    // btService->Enabled = (dmDB->pwdlevel==9);
    for (i = 1; i <= numframe; i++) {
        frSelAgv = (TfrSelectAgv*) FindComponent("frSelectAgv" + IntToStr(i));
        if (frSelAgv != NULL) {
            frSelAgv->Enabled = dmDB->pwdlevel;
        }
    }
}

// ---------------------------------------------------------------------------

void __fastcall TfrConfig::BitBtnPosClick(TObject *Sender)
{
    // Application->MessageBox(L"Function disabled", L"Function disabled", MB_OK);
    dmDBClient->InserisciCorsiePostazioniDatabase();
    // dmDBClient->InserisciPLCDatabase();

}
// ---------------------------------------------------------------------------

void __fastcall TfrConfig::btStoricizzaClick(TObject *Sender) {

    if (Application->MessageBox(L"Storicizza Log, Allarmi, Batteria, Missioni, CentroMissioni?", L"Conferma", MB_YESNO) == IDYES) {
        if (dmExtraFunction->StoricizzaTutto("MONTH", 3) == 5)
            Application->MessageBox(L"Storicizzazione effettuata", L"Ok", MB_OK);
        else
            Application->MessageBox(L"problemi Storicizzazione", L"Read log", MB_OK);
        FormActivate(Sender);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TfrConfig::btServiceClick(TObject *Sender) {
    if ((dmDB->pwdlevel == 9) && (!ClientData.ParametriFunzionali.AbilitaMissioni)) {
        if (Application->MessageBox(L"Sei sicuro di voler resettare tute le missioni?", L"Conferma", MB_YESNO) == IDYES) {
            // devo inviare al server lo svuotamento
            // sprintf(ev, "SVUOTA,%d",0);
            // cs->SendEv(ev);
            if (Application->MessageBox(L"Vuoi resettare tutte le prenotazioni?", L"Conferma", MB_YESNO) == IDYES) {
                try {
                    dmDB->QueryPos->Close();
                    dmDB->QueryPos->SQL->Clear();
                    dmDB->QueryPos->SQL->Append("UPDATE Corsie SET CorsiaPrenotata = 0");
                    dmDB->QueryPos->ExecSQL();
                    dmDB->QueryPos->Close();

                    dmDB->QueryPos->SQL->Clear();
                    dmDB->QueryPos->SQL->Append("UPDATE Posizioni SET Prenotata = 0");
                    dmDB->QueryPos->ExecSQL();
                    dmDB->QueryPos->Close();
                    dmDB->LogMsg("Reset prenotazioni ");
                }
                catch (...) {
                    dmDB->LogMsg("Eccezione Reset Prenotazioni");
                }
            }
            SocketDataModule->InviaAbortAll(1);
            // MainForm->aggiornamappa = 1 ;
        }
    }
    else
        ShowMessage("Operazione permessa solo a utenti di Livello 9 e missioni disabilitate");

}

// ---------------------------------------------------------------------------
void __fastcall TfrConfig::btnForzaPortaClick(TObject *Sender)
{
    // forzo output
    int i;
    TCheckBox *cb;
    if (dmDB->pwdlevel) {
        output[0] = 0;
        // byte 0
        if (ckbForzaPorta->Checked)
            ClientData.Plc[1].SegnaliGenerali[1].uscitedascrivere[0] |= dmDB->bit[ClientData.Plc[1].SegnaliGenerali[1].bitOutput_conferma_accesso];
        else
            ClientData.Plc[1].SegnaliGenerali[1].uscitedascrivere[0] &= dmDB->bitAnd[ClientData.Plc[1].SegnaliGenerali[1].bitOutput_conferma_accesso];
        // SocketDataModule->InviaUscitePlc(pos, StructPosDati.byteOutput, output[1], 1);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TfrConfig::btResetPosClick(TObject *Sender)
{
    if (Application->MessageBox(L"Sei sicuro di voler resettare le posizioni? ", L"Conferma ", MB_YESNO) == IDYES) {
        // dmDBImpianto->UpdateVariazioni(0, "");
        SocketDataModule->ResettaPLC();
        SocketDataModule->InviaAggiornamentoPLC();
    }
}
// ---------------------------------------------------------------------------

void __fastcall TfrConfig::btResetAGVClick(TObject *Sender)
{
    if ((dmDB->pwdlevel) && (!ClientData.ParametriFunzionali.AbilitaMissioni)) {
        if (Application->MessageBox(L"Sei sicuro di voler resettare tute le missioni?", L"Conferma", MB_YESNO) == IDYES) {
            // devo inviare al server lo svuotamento
            // sprintf(ev, "SVUOTA,%d",0);
            // cs->SendEv(ev);
            if (Application->MessageBox(L"Vuoi resettare tutte le prenotazioni?", L"Conferma", MB_YESNO) == IDYES) {
                try {
                    dmDB->QueryPos->Close();
                    dmDB->QueryPos->SQL->Clear();
                    dmDB->QueryPos->SQL->Append("UPDATE Corsie SET CorsiaPrenotata = 0");
                    dmDB->QueryPos->ExecSQL();
                    dmDB->QueryPos->Close();

                    dmDB->QueryPos->SQL->Clear();
                    dmDB->QueryPos->SQL->Append("UPDATE Posizioni SET Prenotata = 0");
                    dmDB->QueryPos->ExecSQL();
                    dmDB->QueryPos->Close();
                    dmDB->LogMsg("Reset prenotazioni ");

                    dmDB->QueryPos->SQL->Clear();
                    dmDB->QueryPos->SQL->Append("UPDATE statoagv SET idmis = 0, generata=0");
                    dmDB->QueryPos->ExecSQL();
                    dmDB->QueryPos->Close();
                    SocketDataModule->InviaLeggiStatoAGV();
                    dmDB->LogMsg("Reset statoAGV ");
                }
                catch (...) {
                    dmDB->LogMsg("Eccezione Reset Prenotazioni");
                }
            }
            SocketDataModule->InviaAbortAll(1);
            // MainForm->aggiornamappa = 1 ;
        }
    }
    else
        ShowMessage("Operazione permessa solo a utenti amministratori e missioni disabilitate");
}
// ---------------------------------------------------------------------------

