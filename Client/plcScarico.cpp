// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "plcScarico.h"
#include "db.h"
#include "dbclient.h"
#include "SocketComunicazioneClient.h"
#include "DataExchange.h"
#include "dmFunzioniComuniClientServer.h"
#include "extrafunction.h"
#include "udclist.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "UDCFrame"
#pragma resource "*.dfm"
TFormPlcScarico *FormPlcScarico;

void TFormPlcScarico::VisualizzaDatiPosizione() {

	TRecordList TabPosizioni;
	TIndexList TabUDC;
	int i;
	dmDB->CaricaTabella("piani_view where Pos = " + IntToStr(pos), TabPosizioni);
	dmDBClient->FullTabellaK("Select *, ARTICOLI.DESCRIZIONE as [descart], ArticoliUDC.Codice as [CARTUDC] from UDC INNER JOIN TabTipoUDC ON UDC.CodTipoUDC = TabTipoUDC.IDTipoUDC LEFT OUTER JOIN ArticoliUDC ON UDC.IDArtUDC = ArticoliUDC.IDArtUDC LEFT OUTER JOIN Articoli ON ArticoliUDC.IDArticolo = Articoli.IDArticolo ORDER BY UDC.IDUDC", "IDUDC", TabUDC);

	for (i = 0; i < TabPosizioni.size(); i++) {
		Label22->Caption = TabPosizioni[i]["POS"];
		Label24->Caption = TabPosizioni[i]["CUSTOMERPOS"];
		eHPrel->Text = TabPosizioni[i]["HPREL"];
		eHDep->Text = TabPosizioni[i]["HDEP"];
		ckDisabilitaPos->Checked = TabPosizioni[i]["DISABILITATA"].ToIntDef(0) | TabPosizioni[i]["POS_DISABILITA"].ToIntDef(0);;
		cPrenotataPos->Checked = TabPosizioni[i]["PRENOTATA"].ToIntDef(0);
		eUDC->Text = TabPosizioni[i]["IDUDC"].ToIntDef(0);
	}

}

void TFormPlcScarico::SetIdPlcDaPos() {

	//
}

void TFormPlcScarico::SetInputOutputPlc() {
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
__fastcall TFormPlcScarico::TFormPlcScarico(TComponent* Owner)
	: TForm(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TFormPlcScarico::FormActivate(TObject *Sender)
{
	//
	change_udc = false;
	activate = true;
	SpeedButton1->Down = !ClientData.Simula;
	SetIdPlcDaPos();

	VisualizzaDatiPosizione();
	// input
	plc_scarico_griglie = 0;
	cambiocheck = 0;
	TCheckBox *OggettoCheck;
	AnsiString TempString;
	AnsiString Funzione = "PLCSIGNAL";

	// if ((pos/10)*10==pos){    // se e' postazione di scarico
	// }
	/*
	 for (int i = 1; i <= 3; i++) {
	 OggettoCheck = (TCheckBox*) FindComponent("ckInput" + String(i));
	 if (OggettoCheck != NULL) {
	 TempString = DataForm->Traduci(Funzione, i);
	 if (TempString != "") {
	 OggettoCheck->Caption = TempString;
	 }
	 }
	 }
	 // output

	 Funzione = "PLCSIGNAL";
	 for (int i = 1; i <= 1; i++) {
	 OggettoCheck = (TCheckBox*) FindComponent("ckOutput" + String(i));
	 if (OggettoCheck != NULL) {
	 TempString = DataForm->Traduci(Funzione, i + 8);
	 if (TempString != "") {
	 OggettoCheck->Caption = TempString;
	 }
	 }
	 OggettoCheck = (TCheckBox*) FindComponent("ckOutput_" + String(i));
	 if (OggettoCheck != NULL) {
	 TempString = DataForm->Traduci(Funzione, i + 16);
	 if (TempString != "") {
	 OggettoCheck->Caption = TempString;
	 }
	 }
	 }
	 */
	Timer1Timer(this);
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcScarico::Timer1Timer(TObject *Sender)
{
	TCheckBox *Check;
	AnsiString stringa_hex;
	int i, indice_edit;
	TEdit *frLoad;
	// aggiorno
	Timer1->Enabled = false;
	GroupBox6->Enabled = ClientData.Simula;
	BitBtn6->Visible = ClientData.Simula;
	BitBtn1->Visible = (ClientData.Simula || dmDB->pwdlevel);
	GroupBox3->Enabled = dmDB->pwdlevel;
	GroupBox4->Enabled = dmDB->pwdlevel;
	GroupBox8->Enabled = dmDB->pwdlevel;

	if ((activate) || (SpeedButton1->Down) || ((!dmDB->pwdlevel) && (!ClientData.Simula))) {
		AssociaPlc(StructPlcVisualizzata);
		AggiornaCampiPlc(StructPlcVisualizzata);
	}

	activate = false;
	Timer1->Enabled = true;
}

// ---------------------------------------------------------------------------
void TFormPlcScarico::AssociaPlc(DatiPlcTGW &StructPlc) {
	int i;

	for (i = 1; i < 5; i++) {

		if (ClientData.PlcTGW[i].DepositoTGW.pos == pos) {
			StructPlc = ClientData.PlcTGW[i];

			posizionecarico = 0;
			break;
		}
	}

}

void TFormPlcScarico::AggiornaCampiPlc(DatiPlcTGW &StructPlc) {
	int i;
	TCheckBox *cb;
	AnsiString stringa_hex;
	if (StructPlc.PlcConnesso)
		Shape4->Brush->Color = clLime;
	else
		Shape4->Brush->Color = clRed;
	lWatchDogPlc->Caption = IntToStr(StructPlc.watchdog_plc);
	lWatchDogPc->Caption = IntToStr(StructPlc.watchdog_pc);
	lCycle->Caption = IntToStr(StructPlc.CycleTime) + " ms";
	lErrors->Caption = IntToStr(StructPlc.Error);

	ckInput1->Checked = StructPlc.DepositoTGW.stazioneAgibile;
	ckInput2->Checked = StructPlc.DepositoTGW.postazioneLibera;
	ckInput3->Checked = StructPlc.DepositoTGW.consensoIngresso;

	ckOutput1->Checked = StructPlc.DepositoTGW.AGVFuoriIngombro;

		eUDC->Text = dmFunzioni->DaBufferAStringa(StructPlc.DepositoTGW.CodUDC, 20);

}

void __fastcall TFormPlcScarico::FormClose(TObject *Sender, TCloseAction &Action)
{
	Timer1->Enabled = false;
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcScarico::BitBtn5Click(TObject *Sender)
{
	TUDC UdcMod;
	// dmDB->UpdatePos(pos, cPrenotataPos->Checked, ckDisabilitaPos->Checked, fUDCSmall->eIDArtUDC->Text.ToIntDef(0), eHPrel->Text.ToIntDef(0), eHDep->Text.ToIntDef(0));
	if ((eUDC->Text.ToIntDef(0)) && (change_udc) && (dmDB->pwdlevel)) {
		UdcMod.IDUDC = eUDC->Text.ToIntDef(0);
		dmDB->LeggiStrutturaUdc(UdcMod);
		if ((UdcMod.IDUDC) && (dmDB->pwdlevel)) {
			UdcMod.CodTipoUDC = 2; // CHIEDERE!!!!
			dmDB->InsertUpdateUDC(UdcMod);
			dmDB->UpdatePosNoStato(pos, eUDC->Text.ToIntDef(0), eHPrel->Text.ToIntDef(5), eHDep->Text.ToIntDef(65),"");
		}
	}
	if ((dmDB->pwdlevel) && (cambiocheck))
		dmDB->UpdateSoloStato(pos, cPrenotataPos->Checked, ckDisabilitaPos->Checked);
	Close();
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcScarico::BitBtn4Click(TObject *Sender) {
	Close();
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcScarico::BitBtn6Click(TObject *Sender)
{
	// forzo input in simulazione
	int i;
	TCheckBox *cb;
	if (ClientData.Simula) {
		input = 0;
		for (i = 0; i < 3; i++) {
			cb = (TCheckBox*) FindComponent("ckInput" + IntToStr(i + 1));
			if ((cb != NULL) && (cb->Checked))
				input |= dmDB->bit[i];
		}
		SocketDataModule->InviaSimulazioneInputPlc(pos, 1, input);

	}

}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcScarico::BitBtn1Click(TObject *Sender)
{
		// forzo output
	int i;
	TCheckBox *cb;
	if (dmDB->pwdlevel) {
		output = 0;
		for (i = 0; i < 1; i++) {
			cb = (TCheckBox*) FindComponent("ckOutput" + IntToStr(i + 1));
			if ((cb != NULL) && (cb->Checked))
				output |= dmDB->bit[i];
		}
		SocketDataModule->InviaUscitePlc(pos, StructPlcVisualizzata.DepositoTGW.byteOutput, output);

	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormPlcScarico::SpeedButton2Click(TObject *Sender)
{
	FormUDCList->blocca_insert = false;
	FormUDCList->pos = pos;
	FormUDCList->ShowModal();
	FormActivate(this);
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcScarico::cbTipoUDCChange(TObject *Sender)
{
	if (!activate)
		change_udc = true;
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcScarico::eUDCChange(TObject *Sender)
{
	if (!activate)
		change_udc = true;

}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcScarico::cbStatusChange(TObject *Sender)
{
	if (!activate)
		change_udc = true;

}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcScarico::ckDisabilitaPosClick(TObject *Sender)
{
	if (!activate)
		cambiocheck = true;
}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcScarico::cPrenotataPosClick(TObject *Sender)
{
	if (!activate)
		cambiocheck = true;

}
// ---------------------------------------------------------------------------

void __fastcall TFormPlcScarico::SpeedButton3Click(TObject *Sender)
{
	TUDC UdcMod;
	if ((eUDC->Text.ToIntDef(0)) && (dmDB->pwdlevel)) {
		dmDB->LeggiStrutturaUdc(UdcMod);
		dmDB->LeggiStrutturaUdc(UdcMod);
		if (dmDB->pwdlevel) {
			if (Application->MessageBox(L"Are you sure to delete this Item?", L"Confirm!!!", MB_YESNO) == IDYES) {
				eUDC->Text = "0";
				UdcMod.stato = 0;
				UdcMod.CodTipoUDC = 0;
				dmDB->InsertUpdateUDC(UdcMod);
				dmDB->UpdatePosNoStato(pos, eUDC->Text.ToIntDef(0), eHPrel->Text.ToIntDef(5), eHDep->Text.ToIntDef(65),"");

			}
		}

		FormActivate(this);
	}

}
// ---------------------------------------------------------------------------

