// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PosDepositoPLC.h"
#include "db.h"
#include "dbclient.h"
#include "SocketComunicazioneClient.h"
#include "DataExchange.h"
#include "dmFunzioniComuniClientServer.h"
#include "extrafunction.h"
#include "udclist.h"
#include "Main.h"
#include "DBImpianto.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPosDepPLC *FormPosDepPLC;

__fastcall TFormPosDepPLC::TFormPosDepPLC(TComponent* Owner) : TForm(Owner) {

}

// ---------------------------------------------------------------------------

void __fastcall TFormPosDepPLC::FormActivate(TObject *Sender) {
    //
    change_udc = false;
    activate = true;
    piano=1;
    SpeedButton1->Down = !ClientData.ParametriFunzionali.Simula;
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

void TFormPosDepPLC::AssociaPlc() {
    int i, j;

    for (i = 1; i <= IDX_PLCDEPOSITO; i++) {
        for (j = 1; j <= NUM_DEPOSITOPLC; j++) {
            for (int p = 1; p <= NUMPIANI_PREL; p++) {
                if (ClientData.Plc[i].Deposito[j][p].pos == pos) {
                 piano = p;
                    StructPosVisualizzata = ClientData.Plc[i];
                    StructPosDati = ClientData.Plc[i].Deposito[j][p];
                    break;
                }
            }
        }
    }
    BitBtn5->Enabled = dmDB->pwdlevel;
}

void TFormPosDepPLC::VisualizzaDatiPosizione() {

    TRecordList TabPosizioni;
    int tipo = 0;
    TIndexList TabUDC;
    int i;
    dmDB->CaricaTabella("piani_view where Pos = " + IntToStr(pos) + " and piano=" + IntToStr(piano) + " order by pos, piano", TabPosizioni);

    for (i = 0; i < TabPosizioni.size(); i++) { // inutile a 1 piano ma uso futuro
        if (i == 0) {
            Label22->Caption = TabPosizioni[i]["POS"];
            // Label24->Caption = TabPosizioni[i]["CUSTOMERPOS"];
            Label24->Caption = TabPosizioni[i]["NOMEPOS"];
            ckDisabilitaPos->Checked = TabPosizioni[i]["DISABILITATA"].ToIntDef(0) | TabPosizioni[i]["POS_DISABILITA"].ToIntDef(0);
            cPrenotataPos->Checked = TabPosizioni[i]["POS_PRENOTATA"].ToIntDef(0);
            lbZona->Caption = TabPosizioni[i]["ZONA"];
            lbTipoPos->Caption = TabPosizioni[i]["TIPOPOSIZIONE"];
            // ckPresenzaUDC->Checked = TabPosizioni[i]["IDUDC"].ToIntDef(0);
            eHPrel1->Text = TabPosizioni[i]["HPREL"];
            eHDep1->Text = TabPosizioni[i]["HDEP"];
        }
    }
}

void __fastcall TFormPosDepPLC::Timer1Timer(TObject *Sender) {
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

    activate = false;
    Timer1->Enabled = true;
}

// ---------------------------------------------------------------------------

void TFormPosDepPLC::AggiornaCampiPlc(DatiPlc &StructPosVisualizzata, DatiDeposito &deposito) {
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

    // input
    cbIn0->Checked = deposito.ProntaAlDeposito;
    cbIn1->Checked = deposito.OkDeposito;
    cbIn2->Checked = deposito.Ready;
    cbIn3->Checked = deposito.InAllarme;

    // output
    cbRichiestaAbilitaDep->Checked = deposito.RichiestaAbilitaDep;
    cbAGVInIngombro->Checked = deposito.AGVInIngombro;
    editIdPallet->Text = deposito.PalletID;

}

void __fastcall TFormPosDepPLC::FormClose(TObject *Sender, TCloseAction &Action) {
    Timer1->Enabled = false;
}
// ---------------------------------------------------------------------------

void __fastcall TFormPosDepPLC::BitBtn5Click(TObject *Sender) {
    TUDC UdcMod;
    // dmDB->UpdatePos(pos, cPrenotataPos->Checked, ckDisabilitaPos->Checked, fUDCSmall->eIDArtUDC->Text.ToIntDef(0), eHPrel->Text.ToIntDef(0), eHDep->Text.ToIntDef(0));
    if ((change_udc) && (dmDB->pwdlevel))
    {
        dmDB->UpdatePosNoStato(pos, 0, eHPrel1->Text.ToIntDef(5), eHDep1->Text.ToIntDef(65), 1);
        // dmDB->ArticoloPrelevatoDepositato(pos, edCodUDC->Text.ToIntDef(0), piano, );  // da mettere a posto
        // UpdateAltezzaPiano(int pos, int piano, int corsia, int hprel, int hdep)
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

void __fastcall TFormPosDepPLC::BitBtn4Click(TObject *Sender) {
    Close();
}
// ---------------------------------------------------------------------------

void __fastcall TFormPosDepPLC::BitBtn6Click(TObject *Sender) {
    int i, j;
    TCheckBox *cb;
    if (ClientData.ParametriFunzionali.Simula) {
        input[0] = 0;
        input[1] = 0;
        input[0] = dmExtraFunction->bitWrite(input[0], StructPosDati.bit_Ready, cbIn2->Checked);
        input[0] = dmExtraFunction->bitWrite(input[0], StructPosDati.bit_InAllarme, cbIn3->Checked);
        input[0] = dmExtraFunction->bitWrite(input[0], StructPosDati.bit_ProntaAlDeposito, cbIn0->Checked);
        input[0] = dmExtraFunction->bitWrite(input[0], StructPosDati.bit_OkDeposito, cbIn1->Checked);

        SocketDataModule->InviaSimulazioneInputPlc(pos, 0, input[0], piano);

    }
}
// ---------------------------------------------------------------------------

void __fastcall TFormPosDepPLC::BitBtn1Click(TObject *Sender) {
    // forzo output
    int i;
    TCheckBox *cb;
    if (dmDB->pwdlevel) {
        output[0] = 0;
        output[1] = 0;
        // byte 0
        if (cbAGVInIngombro->Checked)
            output[0] |= dmDB->bit[StructPosDati.bit_AGVInIngombro];
        if (cbRichiestaAbilitaDep->Checked)
            output[0] |= dmDB->bit[StructPosDati.bit_RichiestaAbilitaDep];

        SocketDataModule->InviaUscitePlc(pos, 0, output[0], piano);
        // SocketDataModule->InviaUscitePlc(pos, StructPosDati.byteOutput, output[1], 1);
    }

}

// ---------------------------------------------------------------------------

void __fastcall TFormPosDepPLC::ckDisabilitaPosClick(TObject *Sender) {
    if (!activate)
        cambiocheck = true;
}
// ---------------------------------------------------------------------------

void __fastcall TFormPosDepPLC::cPrenotataPosClick(TObject *Sender) {
    if (!activate)
        cambiocheck = true;

}

// ---------------------------------------------------------------------------
void __fastcall TFormPosDepPLC::cbInMissioneClick(TObject *Sender) {
    if (!activate)
        cambiocheck = true;
}
// ---------------------------------------------------------------------------

void __fastcall TFormPosDepPLC::ckPresenzaUDCClick(TObject *Sender) {
    if (!activate)
        change_udc = true;

}

// ---------------------------------------------------------------------------
void __fastcall TFormPosDepPLC::ckInput1Click(TObject *Sender) {
    if (!activate)
        change_udc = true;
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
