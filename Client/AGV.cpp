// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AGV.h"
#include "Main.h"
#include "DBClient.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CHILDWIN"
#pragma link "AGVFrameL"
#pragma resource "*.dfm"
TfAGV *fAGV;

// ---------------------------------------------------------------------------
__fastcall TfAGV::TfAGV(TComponent* Owner)
	: TMDIChild(Owner)
{
	first = true;
	// TimerUpdTimer(NULL);
}
// ---------------------------------------------------------------------------

void TfAGV::ColoraShape(int stato, TShape *t, TColor ColorON, TColor ColorOFF)
{
	if (stato) {
		t->Brush->Color = ColorON;
	}
	else {
		t->Brush->Color = ColorOFF;
	}
}
// ---------------------------------------------------------------------------

AnsiString NavDesc(char n) {
	switch (n) {
	case 'A':
		return "'A' - Assente";
	case 'I':
		return "'I' - Idle";
	case 'R':
		return "'R' - Running";
	case 'S':
		return "'S' - Stopped";
	case 'W':
		return "'W' - Waiting";
	case 'B':
		return "'B' - Blocked";
	case 'D':
		return "'D' - Done";
	case 'E':
		return "'E' - Error";
	}
	return "?";
}
// ---------------------------------------------------------------------------

AnsiString AutoDesc(char n) {
	switch (n) {
	case 'A':
		return "'A' - Automatico";
	case 'S':
		return "'S' - Semiautomatico";
	case 'M':
		return "'M' - Manuale";
	}
	return "?";
}
// ---------------------------------------------------------------------------

AnsiString CaricoDesc(char n) {
	switch (n) {
	case 'E':
		return "'E' - Scarico";
	case 'R':
		return "'R' - Carico 1";
	case 'L':
		return "'L' - Carico 2";
	case 'F':
		return "'F' - Carico completo";
	}
	return "?";
}
// ---------------------------------------------------------------------------

void __fastcall TfAGV::TimerUpdTimer(TObject *Sender)
{
	short i, n;
	int h_pallet;
	char statoagv[1];
	AnsiString s, statoagvstr, filename, lotto, codice;
	TRecordList TabAGV;
	dmDB->CaricaTabella("StatoAGV ORDER BY ID", TabAGV);
	TfrAgvL *frAgvL;

	TimerUpd->Enabled = false;
	active = true;
	for (i = 1; i <= NAGV; i++) {

		frAgvL = (TfrAgvL*)(FindComponent("frAgvL" + IntToStr(i)));
		frAgvL->idagv = i;
		frAgvL->aggiornamento = true;
		frAgvL->lePosizione->Text = ClientData.DatiAgv[i].pos;
		frAgvL->leLivBatt->Text = ClientData.DatiAgv[i].LivelloBatteria;
		frAgvL->leDestinazione->Text = ClientData.DatiAgv[i].dest;
		frAgvL->leMissione->Text = ClientData.DatiAgv[i].idmis;
		frAgvL->cbAuto->Checked = (!ClientData.DatiAgv[i].manuale && !ClientData.DatiAgv[i].semiautomatico);
		frAgvL->cbMan->Checked = ClientData.DatiAgv[i].manuale;
		frAgvL->cbAlarm->Checked = ClientData.DatiAgv[i].alarm;
		frAgvL->cbSemiAuto->Checked = ClientData.DatiAgv[i].semiautomatico;
		frAgvL->cbLoaded->Checked = ClientData.DatiAgv[i].load;
		frAgvL->cbCarica->Checked = ClientData.DatiAgv[i].incarica;
		frAgvL->cbBattBassa->Checked = ClientData.DatiAgv[i].bitbattbassa;
		frAgvL->cbSospeso->Checked = ClientData.DatiAgv[i].sospeso;
		// frAgvL->leDest->Text = MainForm->DatiAgv[i].dep;
		// frAgvL->leIDUDC->Text = ClientData.DatiAgv[i].DatiUDC.IDUDC;codice_pallet
		frAgvL->leIDUDC->Text = ClientData.DatiAgv[i].DatiUDC.IDUDC;
		frAgvL->cbForcheDX->Checked = ClientData.DatiAgv[i].forchedx;
		frAgvL->cbForcheSX->Checked = ClientData.DatiAgv[i].forchesx;
		frAgvL->leLatoForchePrel->Text = ClientData.DatiAgv[i].latoforcheprel;
		frAgvL->leLatoForcheDep->Text = ClientData.DatiAgv[i].latoforchedep;
		frAgvL->leNodoPassaggioPrel->Text = ClientData.DatiAgv[i].nodopassaggioprel;
		frAgvL->leNodoPassaggioDep->Text = ClientData.DatiAgv[i].nodopassaggiodep;

      //  frAgvL->ckbDaEtichettare->Checked = ClientData.DatiAgv[i].da_etichettare;

        frAgvL->ckbStandbyAccesso->Checked = ClientData.DatiAgv[i].standby_operatore;

        frAgvL->leTipoUDC->Text = ClientData.DatiAgv[i].tipo_pallet;
        frAgvL->EditAltezzaPallet->Text = ClientData.DatiAgv[i].altezza_pallet;
		/*if ((ClientData.DatiAgv[i].forche_in_trazione == 0) && (ClientData.DatiAgv[i].forchesx == 0))
		{
			frAgvL->ckbForcheInTrazione->Checked = true;
		}
		else
		{
			frAgvL->ckbForcheInTrazione->Checked = false;
		}*/
        frAgvL->ckbForcheInTrazione->Checked = ClientData.DatiAgv[i].forche_in_trazione;
		/* frAgvL->lePiano->Text = MainForm->DatiAgv[i].pianodep;
		 frAgvL->leCorsia->Text = MainForm->DatiAgv[i].corsiadep;
		 frAgvL->leHCorsia->Text = MainForm->DatiAgv[i].h_corsia; */

		if ((!first) && (((ClientData.DatiAgv[i].load) && (!frAgvL->cbLoaded->Checked)) || ((ClientData.DatiAgv[i].load) && (!frAgvL->cbLoaded->Checked))))
			first = true; // ricarico dati pallet

		if (first) {
			if (ClientData.DatiAgv[i].DatiUDC.IDUDC) {
				// frAgvL->leTipoUDC->Text = ClientData.DatiAgv[i].DatiUDC.TipoUdc;
			}
		}
		frAgvL->cbInMissione->Checked = ClientData.DatiAgv[i].idmis;
		/* switch(MainForm->DatiAgv[i].statomis) {
		 case 1 :
		 frAgvL->leStatoMis->Text = "GENERATA" ;
		 break ;
		 case 2 :
		 frAgvL->leStatoMis->Text = "INIZIATA" ;
		 break ;
		 case 3 :
		 frAgvL->leStatoMis->Text = "PRELEVATO" ;
		 break ;
		 default :
		 frAgvL->leStatoMis->Text = "NO MISS" ;
		 break ;
		 } */
		if (ClientData.DatiAgv[i].load)
			filename = "agv_load.png";
		else
			filename = "agv_unload.png";

		if (((MainForm->imgfile_agv[i] != filename) || (first)) && (FileExists(filename))) {
			frAgvL->Image1->Picture->LoadFromFile(filename);
			MainForm->imgfile_agv[i] = filename;
		}
		/* if (TabAGV[i]["ABILITATO"].ToIntDef(0)) {
		 frAgvL->Panel1->Color = clBtnFace ;
		 } else {
		 frAgvL->Panel1->Color = clRed ;
		 } */
		frAgvL->aggiornamento = false;
		frAgvL->pInMis->Enabled = dmDB->pwdlevel;
	}
	active = false;
	first = false;
	TimerUpd->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TfAGV::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Shift.Contains(ssCtrl) && (Key == VK_DELETE))
		Key = 0; // ignore
}
// ---------------------------------------------------------------------------

void __fastcall TfAGV::FormActivate(TObject *Sender)
{
	int i, ttop, alt;
	TfrAgvL *frLoad;
	TimerUpd->Enabled = false;
	// crea e gestisci i frame
	ttop = 0;
	alt = ((float) frAgvL1->Height * 1.05f);
	ScrollBox1->VertScrollBar->Range = alt * NAGV;

	for (i = 1; i <= NAGV; i++) {
		frLoad = (TfrAgvL*)(FindComponent("frAgvL" + IntToStr(i)));
		if (frLoad == NULL) {
			frLoad = new TfrAgvL(this);
			frLoad->Parent = this->Panel1;
			frLoad->Name = "frAgvL" + IntToStr(i);
		}
		// operazioni comuni per tutti
		frLoad->Hint = "frAgvL" + IntToStr(i);
		frLoad->Tag = i;
		frLoad->Left = 0;
		frLoad->Top = ttop + alt * (i - 1);
		frLoad->pAGV->Caption = "AGV " + String(i) + " tipo: " + String(ClientData.DatiAgv[i].tipoAGV);
		// frLoad->RiempiCampi(0);

	}
	TimerUpd->Enabled = true;
	first = true;
}
// ---------------------------------------------------------------------------


