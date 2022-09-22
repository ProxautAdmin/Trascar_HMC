// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "plcsignal_lavatrice.h"
#include "db.h"
#include "SocketComunicazioneClient.h"
#include "DataExchange.h"
#include "UDClist.h"
#include "DBClient.h"
#include "ExtraFunction.h"


// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPlcLavatrice *FormPlcLavatrice;

void TFormPlcLavatrice::VisualizzaDatiPosizione() {

    TRecordList TabPosizioni;
    int i;
    dmDB->CaricaTabella("Posizioni where Pos = " + IntToStr(pos), TabPosizioni);

    for (i = 0; i < TabPosizioni.size(); i++) {
        Label22->Caption = TabPosizioni[i]["POS"];
        Label24->Caption = TabPosizioni[i]["CUSTOMERPOS"];
        eHDep->Text = TabPosizioni[i]["HDEP"]; // TabPosizioni[i]["HPREL"] ;
        eHprel->Text = TabPosizioni[i]["HPREL"];
        eUDC->Text = TabPosizioni[i]["IDUDC"]; // TabPosizioni[i]["HPREL"] ;
        // leForchePiano3_4->Text = TabPosizioni[i]["HDEP"] ;
        ckDisabilitaPos->Checked = TabPosizioni[i]["DISABILITA"].ToIntDef(0);
        cPrenotataPos->Checked = TabPosizioni[i]["PRENOTATA"].ToIntDef(0);
        ComboBox1->Text = dmExtraFunction->RiempiTipoUDC(ComboBox1, dmDBClient->ReturnCodStatoUDCdaIDUDC(TabPosizioni[i]["IDUDC"].ToIntDef(0)));
    }

}

void TFormPlcLavatrice::SetIdPlcDaPos() {

}

// ---------------------------------------------------------------------------
__fastcall TFormPlcLavatrice::TFormPlcLavatrice(TComponent* Owner)
    : TForm(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlcLavatrice::FormActivate(TObject *Sender)
{
    activate = true;
    TCheckBox *OggettoCheck;
    AnsiString TempString;
    AnsiString Funzione = "LAVA";
    // input
    for (int i = 1; i <= 4; i++) {
        OggettoCheck = (TCheckBox*) FindComponent("ckInput" + String(i));
        if (OggettoCheck != NULL) {
            TempString = DataForm->Traduci(Funzione, i);
            if (TempString != "") {
                OggettoCheck->Caption = TempString;
            }
        }
    }

    // output
    for (int i = 1; i <= 4; i++) {
        OggettoCheck = (TCheckBox*) FindComponent("ckOutput" + String(i));
        if (OggettoCheck != NULL) {
            TempString = DataForm->Traduci(Funzione, i + 4);
            if (TempString != "") {
                OggettoCheck->Caption = TempString;
            }
        }
    }
    SpeedButton1->Down = !ClientData.Simula;
    VisualizzaDatiPosizione();
    Timer1Timer(this);
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcLavatrice::Timer1Timer(TObject *Sender)
{
    TCheckBox *Check;
    int i, j;
    AnsiString stringa;
    // aggiorno
    Timer1->Enabled = false;
    GroupBox2->Enabled = ClientData.Simula;
    BitBtn6->Visible = ClientData.Simula;
    GroupBox1->Enabled = dmDB->pwdlevel;
    GroupBox3->Enabled = dmDB->pwdlevel;
    GroupBox5->Enabled = dmDB->pwdlevel;
    BitBtn1->Visible = (ClientData.Simula || dmDB->pwdlevel);
    GroupBox4->Enabled = dmDB->pwdlevel;

    if (ClientData.DatiLavatrice[1].PlcConnesso)
        Shape4->Brush->Color = clLime;
    else
        Shape4->Brush->Color = clRed;
    lWatchDogPlc->Caption = IntToStr(ClientData.DatiLavatrice[1].watchdog_plc);
    lWatchDogPc->Caption = IntToStr(ClientData.DatiLavatrice[1].watchdog_pc);
    lCycle->Caption = IntToStr(ClientData.DatiLavatrice[1].CycleTime) + " ms";
    lErrors->Caption = IntToStr(ClientData.DatiLavatrice[1].Error);

    Shape4->Brush->Color = clLime;

    if ((activate) || (SpeedButton1->Down) || ((!dmDB->pwdlevel) && (!ClientData.Simula))) {
		ckInput1->Checked = ClientData.DatiLavatrice[1].ChiamataDeposito;
		ckInput2->Checked = ClientData.DatiLavatrice[1].ChiamataPrelievo;
		ckInput3->Checked = ClientData.DatiLavatrice[1].OkIngressoAgv;

        // ckOutput1->Checked = ClientData.DatiLavatrice.PosFuoriIngombro[1];
        ckOutput1->Checked = ClientData.DatiLavatrice[1].AgvFuoriIngombro;

        ckOutput3->Checked = ClientData.DatiLavatrice[1].UDCPrelevato;
        ckOutput4->Checked = ClientData.DatiLavatrice[1].UDCDepositato;
        // ckOutput3->Checked = ClientData.DatiLavatrice.Bit_RichiestaIngresso;
    }
    activate = false;
    Timer1->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcLavatrice::FormClose(TObject *Sender, TCloseAction &Action)
{
    Timer1->Enabled = false;
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcLavatrice::BitBtn5Click(TObject *Sender)
{
    dmDB->UpdatePos(pos, cPrenotataPos->Checked, ckDisabilitaPos->Checked, eUDC->Text.ToIntDef(0), eHprel->Text.ToIntDef(ALTEZZA_PRELIEVO), eHDep->Text.ToIntDef(ALTEZZA_DEPOSITO));
    Close();
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcLavatrice::BitBtn4Click(TObject *Sender)
{
    Close();
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcLavatrice::BitBtn6Click(TObject *Sender)
{
    // forzo input in simulazione
    int i, val;
    TCheckBox *Check;
    if (ClientData.Simula) {
        val = 0;
        for (i = 0; i < 4; i++) {
            Check = (TCheckBox*) FindComponent("ckInput" + IntToStr(i + 1));
            if ((Check != NULL) && (Check->Checked))
                val |= dmDB->bit[i];
        }
		SocketDataModule->InviaSimulazioneInputPlc(ClientData.DatiLavatrice[1].Pos, 0, val);

    }

}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcLavatrice::SpeedButton2Click(TObject *Sender)
{

    FormUDCList->blocca_insert = false;
    FormUDCList->pos = ClientData.DatiLavatrice[1].Pos;
    FormUDCList->ShowModal();
    VisualizzaDatiPosizione();

}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcLavatrice::BitBtn1Click(TObject *Sender)
{
	int i, val;
	TCheckBox *Check;
	val = 0;
	for (i = 0; i < 4; i++) {
		Check = (TCheckBox*) FindComponent("ckOutput" + IntToStr(i + 1));
		if ((Check != NULL) && (Check->Checked))
			val |= dmDB->bit[i];
	}
	SocketDataModule->InviaUscitePlc(ClientData.DatiLavatrice[1].Pos, 0, val);

}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcLavatrice::SpeedButton3Click(TObject *Sender)
{
    // cancella idvasca da pos
    if (Application->MessageBox(L"Are you sure you want to empty the Position ?", L"Confirm!!!", MB_YESNO) == IDYES) {
        dmDB->SvuotaPosizione(pos);
        dmDB->LogMsg("Eliminata da operatore Udc su posizione lavatrice " + IntToStr(pos));
        VisualizzaDatiPosizione();
    }
}
// ---------------------------------------------------------------------------

