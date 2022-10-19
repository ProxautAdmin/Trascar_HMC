// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PosPrelievoPLC.h"
#include "db.h"
#include "dbclient.h"
#include "SocketComunicazioneClient.h"
#include "DataExchange.h"
#include "dmFunzioniComuniClientServer.h"
#include "extrafunction.h"

#include "Main.h"
#include "DBImpianto.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TFormPosPrelPLC *FormPosPrelPLC;

void TFormPosPrelPLC::VisualizzaDatiPosizione() {

    TRecordList TabPosizioni;
    int i;
    /// ATTENZIONE CHE CAVOLO E?
    dmDB->CaricaTabella("piani_view where Pos = " + IntToStr(pos)+ " and piano=" + IntToStr(piano) + " order by pos, piano", TabPosizioni);

    for (i = 0; i < TabPosizioni.size(); i++) {
        if (i == 0) {
            eHPrel1->Text = TabPosizioni[i]["HPREL"];
            eHDep1->Text = TabPosizioni[i]["HDEP"];
            Label22->Caption = TabPosizioni[i]["POS"];
            // Label22->Caption = TabPosizioni[i]["NOMEPOS"];
            // Label24->Caption = TabPosizioni[i]["CUSTOMERPOS"];
            Label24->Caption = TabPosizioni[i]["NOMEPOS"];
            ckDisabilitaPos->Checked = TabPosizioni[i]["DISABILITATA"].ToIntDef(0) | TabPosizioni[i]["POS_DISABILITA"].ToIntDef(0);
            cPrenotataPos->Checked = TabPosizioni[i]["POS_PRENOTATA"].ToIntDef(0);
            // ckPresenzaUDC->Checked = TabPosizioni[i]["IDUDC"].ToIntDef(0);
            // lbZona->Caption = TabPosizioni[i]["ZONA"];
            lbZona->Caption = TabPosizioni[i]["NOMEREALEPIANO"];
            lbTipoPos->Caption = TabPosizioni[i]["TIPOPOSIZIONE"];

        }

    }
}

void TFormPosPrelPLC::SetIdPlcDaPos() {

    //
}

void TFormPosPrelPLC::SetInputOutputPlc() {
    ///? ok la  struttura degli if ma cosa mettere?
    /*
     if (idplc == 1) {
     input = ClientData.PlcVemacArelleIngresso.input_segnali;
     output = ClientData.PlcVemacArelleIngresso.uscitedascrivere;
     }
     else if (idplc == 2) {
     input = ClientData.PlcVemacArelleUscita.input_segnali;
     output = ClientData.PlcVemacArelleUscita.uscitedascrivere;
     }
     else if (idplc == 3) {
     input = ClientData.PlcDepositoArellePiene[pos_plc].input_segnali;
     output = ClientData.PlcDepositoArellePiene[pos_plc].uscitedascrivere;
     }
     else if (idplc == 4) {
     input = ClientData.PlcPrelievoArelleVuote[pos_plc].input_segnali;
     output = ClientData.PlcPrelievoArelleVuote[pos_plc].uscitedascrivere;
     }
     */

}

// ---------------------------------------------------------------------------
__fastcall TFormPosPrelPLC::TFormPosPrelPLC(TComponent * Owner) : TForm(Owner) {

}

// ---------------------------------------------------------------------------
void __fastcall TFormPosPrelPLC::FormActivate(TObject * Sender) {
    //
    change_udc = false;
    activate = true;
    piano=1;

    SpeedButton1->Down = !ClientData.ParametriFunzionali.Simula;
    SetIdPlcDaPos();
    // AssociaPlc();
    VisualizzaDatiPosizione();
    // input
    plc_scarico_griglie = 0;
    cambiocheck = 0;
    TCheckBox *OggettoCheck;
    AnsiString TempString;
    AnsiString Funzione = "PLCSIGNAL";

    AssociaPlc();
    if (ClientData.ParametriFunzionali.Simula) {
        AggiornaCampiPlc(StructPosVisualizzata, StructPosDati);
    }

    Timer1Timer(this);
}
// ---------------------------------------------------------------------------

void __fastcall TFormPosPrelPLC::Timer1Timer(TObject * Sender) {
    TCheckBox *Check;
    AnsiString stringa_hex;
    int i, indice_edit;
    TEdit *frLoad;
    // aggiorno
    Timer1->Enabled = false;
    gbPos->Enabled = dmDB->pwdlevel;

    if ((activate) || (SpeedButton1->Down) || ((!dmDB->pwdlevel) && (!ClientData.ParametriFunzionali.Simula))) {
        // AssociaPlc();
        AggiornaCampiPlc(StructPosVisualizzata, StructPosDati);
    }
    BitBtn5->Enabled = dmDB->pwdlevel;
    activate = false;
    Timer1->Enabled = true;
}

// ---------------------------------------------------------------------------
void TFormPosPrelPLC::AssociaPlc() {
    int i, j;

    for (i = 1; i <= IDX_PLCPRELIEVO; i++) {
        for (j = 1; j <= NUM_PRELIEVOPLC; j++) {
            for (int p = 1; p <= NUMPIANI_PREL; p++) {
                if (ClientData.Plc[i].Prelievo[j][p].pos == pos) {
                    piano = p;
                    StructPosVisualizzata = ClientData.Plc[i];
                    StructPosDati = ClientData.Plc[i].Prelievo[j][p];
                    break;
                }
            }
        }
    }
}

void TFormPosPrelPLC::AggiornaCampiPlc(DatiPlc & StructPosVisualizzata, DatiPrelievo &prelievo) {
    int i;
    TCheckBox *cb;
    AnsiString stringa_hex;

    if (StructPosVisualizzata.PlcConnesso)
        Shape4->Brush->Color = clLime;
    else
        Shape4->Brush->Color = clRed;
    lWatchDogPlc->Caption = IntToStr(StructPosVisualizzata.watchdog_plc);
    lWatchDogPc->Caption = IntToStr(StructPosVisualizzata.watchdog_pc);
    lCycle->Caption = IntToStr(StructPosVisualizzata.CycleTime) + " ms";
    lErrors->Caption = IntToStr(StructPosVisualizzata.Error);
    //
    ckPresenzaUDC->Checked = 0; // fare
    // input
    cbIn0->Checked = prelievo.ProntaAlPrelievo;
    cbIn1->Checked = prelievo.OkPrelievo;
    cbIn2->Checked = prelievo.Ready;
    cbIn3->Checked = prelievo.InAllarme;

    // output
    cbRichiestaAbilitaPrel->Checked = prelievo.RichiestaAbilitaPrel;
    cbAGVInIngombro->Checked = prelievo.AGVInIngombro;
    editIdPallet->Text = prelievo.PalletID;
}

void __fastcall TFormPosPrelPLC::FormClose(TObject * Sender, TCloseAction & Action) {
    Timer1->Enabled = false;
}
// ---------------------------------------------------------------------------

void __fastcall TFormPosPrelPLC::BitBtn5Click(TObject * Sender) {
    TUDC UdcMod;
    if ((change_udc) && (dmDB->pwdlevel)) {
        dmDB->UpdatePosNoStato(pos, 0, eHPrel1->Text.ToIntDef(5), eHDep1->Text.ToIntDef(65), 1);
        // dmDB->ArticoloPrelevatoDepositato(pos, edCodUDC->Text.ToIntDef(0), piano);     // !!! DA METTERE A POSTO
    }
    if ((dmDB->pwdlevel) && (cambiocheck)) {
         dmDB->UpdateSoloStato(pos, cPrenotataPos->Checked, ckDisabilitaPos->Checked);
    }
    if ((dmDB->pwdlevel) && ((cambiocheck) || (change_udc))) {
        SocketDataModule->InviaAggiornamentoPLC(); // aggiorna posizioni PLC con stati e udc
    }
    BitBtn6Click(this);
    Close();
    dmDB->aggiorna_tab_posizioni_locale = 1;
    MainForm->AggiornaMappa();
}
// ---------------------------------------------------------------------------

void __fastcall TFormPosPrelPLC::BitBtn4Click(TObject * Sender) {
    Close();
}
// ---------------------------------------------------------------------------

void __fastcall TFormPosPrelPLC::BitBtn6Click(TObject * Sender) {
    int i, j;
    TCheckBox *cb;

    if (ClientData.ParametriFunzionali.Simula) {
        input[0] = 0;
        input[1] = 0;
        input[0] = dmExtraFunction->bitWrite(input[0], StructPosDati.bit_Ready, cbIn2->Checked);
        input[0] = dmExtraFunction->bitWrite(input[0], StructPosDati.bit_InAllarme, cbIn3->Checked);
        input[0] = dmExtraFunction->bitWrite(input[0], StructPosDati.bit_ProntaAlPrelievo, cbIn0->Checked);
        input[0] = dmExtraFunction->bitWrite(input[0], StructPosDati.bit_OkPrelievo, cbIn1->Checked);

        // SocketDataModule->InviaSimulazioneInputPlc(pos, StructPosDati.byteInput, edCodArtUDC->Text.ToIntDef(0));
        SocketDataModule->InviaSimulazioneInputPlc(pos, 0, input[0]);

    }

}
// ---------------------------------------------------------------------------

void __fastcall TFormPosPrelPLC::BitBtn1Click(TObject * Sender) {
    // forzo output
    int i;

    TCheckBox *cb;
    if (dmDB->pwdlevel) {
        output[0] = 0;
        output[1] = 0;
        if (cbRichiestaAbilitaPrel->Checked)
            output[0] |= dmDB->bit[StructPosDati.bit_RichiestaAbilitaPrel];
        if (cbAGVInIngombro->Checked)
            output[0] |= dmDB->bit[StructPosDati.bit_AGVInIngombro];

        SocketDataModule->InviaUscitePlc(pos, 0, output[0], 0);

    }
}

// ---------------------------------------------------------------------------

void __fastcall TFormPosPrelPLC::cbTipoUDCChange(TObject * Sender) {
    if (!activate)
        change_udc = true;
}
// ---------------------------------------------------------------------------

void __fastcall TFormPosPrelPLC::eUDCChange(TObject * Sender) {
    if (!activate)
        change_udc = true;

}
// ---------------------------------------------------------------------------

void __fastcall TFormPosPrelPLC::cbStatusChange(TObject * Sender) {
    if (!activate)
        change_udc = true;

}
// ---------------------------------------------------------------------------

void __fastcall TFormPosPrelPLC::ckDisabilitaPosClick(TObject * Sender) {
    if (!activate)
        cambiocheck = true;
}
// ---------------------------------------------------------------------------

void __fastcall TFormPosPrelPLC::cPrenotataPosClick(TObject * Sender) {
    if (!activate)
        cambiocheck = true;

}

// ---------------------------------------------------------------------------
void __fastcall TFormPosPrelPLC::cbInMissioneClick(TObject * Sender) {
    if (!activate)
        cambiocheck = true;
}
// ---------------------------------------------------------------------------

void __fastcall TFormPosPrelPLC::ckPresenzaUDCClick(TObject * Sender) {
    if (!activate)
        change_udc = true;

}

// ---------------------------------------------------------------------------
