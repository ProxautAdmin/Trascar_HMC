// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "stato_pos.h"
#include "db.h"
#include "main.h"
#include "DBClient.h"
#include "UDClist.h"
// #include "UDCform.h"
// #include "anagrafica_articoli.h"
#include "ExtraFunction.h"
#include "GestMissioni.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma link "MyShape"
#pragma link "pos_UDC"
#pragma resource "*.dfm"
TfCorsia *fCorsia;
extern TCentroMissione CentroMissione;

// ---------------------------------------------------------------------------
__fastcall TfCorsia::TfCorsia(TComponent* Owner)
	: TMDIChild(Owner)
{
	change_dati_corsia = false;
	change_dati_pos = false;

}

// ---------------------------------------------------------------------------
void __fastcall TfCorsia::FormCreate(TObject *Sender) {
	// PER LANCIARE IL PRIMO GIRO SOLO UNA VOLTA, ALTRIMENTI PARTE DUE VOLTE
	cambiocheck = false;
	pianosel = 0;
	TopPiano = 0;
	CaricaDB();
	VisualizzaPosizioneUDC();
}

// ---------------------------------------------------------------------------
void __fastcall TfCorsia::FormActivate(TObject * Sender) {
	// change_dati_udc = false;
	Timer1Timer(this);
}

// ---------------------------------------------------------------------------
void TfCorsia::CaricaDB() {
	AnsiString str;
	int corsia_da_destra, posda, posa;
	dmDB->CaricaTabella("Corsie where Fila = " + IntToStr(MainForm->corsia_udc), TabCorsie);
	corsia_da_destra = TabCorsie[0]["CORSIAINIZIODX"].ToIntDef(0); // dmDB->ReturnCorsiaDaDestraDaPos(MainForm->pos_udc);
	posda = MainForm->pos_udc; // (MainForm->pos_udc - (MainForm->pos_udc % 100)) + (((((MainForm->pos_udc - 1) % 100) / 3) * 3) + 1); // trovo la prima posizione di 3
	posa = MainForm->pos_udc; // posda + 3;
	/* if (!corsia_da_destra)
	 str = "Posizionicondatiudc WHERE pos >= " + IntToStr(posda) + " and pos<" + IntToStr(posa) + " ORDER BY Pos"; // parto sempre da sx con la pos chiamata 1, poi nel crs può essere che sia l'ultima, dipende come deposito in corsia
	 else
	 str = "Posizionicondatiudc WHERE pos >= " + IntToStr(posda) + " and pos<" + IntToStr(posa) + " ORDER BY Pos DESC"; // parto sempre da sx con la pos chiamata 1, poi nel crs può essere che sia l'ultima, dipende come deposito in corsia
	 */
	str = "Posizionicondatiudc WHERE Fila = " + IntToStr(MainForm->corsia_udc) + " AND pos = " + IntToStr(posda) + " ORDER BY Pos ";
	// parto sempre da sx con la pos chiamata 1, poi nel crs può essere che sia l'ultima, dipende come deposito in corsia
	dmDB->CaricaTabella(str, TabPosizioni);

}

void TfCorsia::VisualizzaPosizioneUDC() {
	TfrPosUDC *frPosUDC;
	TMyShape *MyShape;
	AnsiString strsql, zona;
	int i, j, idudc, prenotata, esclusa, corsia_abilitata = 0, status, tipologia, numpiani, pianioccupati;
	int posda, posa;
	int lleft;

	i = 0;
	aggiornamento = true;

	// corsie
	if (i < TabCorsie.size()) {
		tipologia = TabCorsie[i]["TIPOLOGIA"].ToIntDef(0);
		Label6->Caption = TabCorsie[i]["FILA"];
		Label8->Caption = TabCorsie[i]["NOMECORSIA"];
		Label11->Caption = TabCorsie[i]["ZONA"];
		ckAbilita->Checked = TabCorsie[i]["ABILITATA"].ToIntDef(0);
		corsia_abilitata = TabCorsie[i]["ABILITATA"].ToIntDef(0);
		ckPrenotata->Checked = TabCorsie[i]["CORSIAPRENOTATA"].ToIntDef(0);
		ckPiena->Checked = TabCorsie[i]["PIENA"].ToIntDef(0);
		ckVuota->Checked = TabCorsie[i]["VUOTA"].ToIntDef(0);
		ckUsoPrel->Checked = TabCorsie[i]["INUSOPRELIEVO"].ToIntDef(0);
		ckUsoDep->Checked = TabCorsie[i]["INUSODEPOSITO"].ToIntDef(0);
		cbTipo->Text = dmExtraFunction->RiempiTipologiaCorsia(cbTipo, TabCorsie[i]["TIPOLOGIA"]); // dmDB->ReturnCodiceTipoUDCDaId(tipologia);
		ckIgnoraOrdine->Checked = TabCorsie[i]["IGNORARIFORDINE"].ToIntDef(0);
		cbPriorita->Text = TabCorsie[i]["PRIORITA"];
		cbDepSelettivo->Checked = TabCorsie[i]["DEPOSITOSELETTIVO"].ToIntDef(0);
		edtAnnotazioni->Text = TabCorsie[i]["ANNOTAZIONI"];
	}

	lleft = 5; // PanelScaffale->Left + PanelSx->Width;
	// crea e gestisci i frame
	// Cancella i frame esistenti maggiori di quelli presenti
	// per fare una roba ottimizzare dovrei mettere in static il valore preceente
	for (i = TabPosizioni.size() + 1; i <= 15; i++) {
		frPosUDC = (TfrPosUDC*)(FindComponent("frPosUDC" + IntToStr(i)));
		if (frPosUDC != NULL)
			delete frPosUDC;
	}

	for (i = 1; i <= TabPosizioni.size(); i++)
	{
		frPosUDC = (TfrPosUDC*)(FindComponent("frPosUDC" + IntToStr(i)));

		if (frPosUDC == NULL)
		{
			frPosUDC = new TfrPosUDC(this);
			frPosUDC->Parent = this->PanelScaffale;
			frPosUDC->Name = "frPosUDC" + IntToStr(i);
		}
		frPosUDC->Hint = "frPosUDC" + IntToStr(i);
		frPosUDC->Tag = i;
		frPosUDC->Left = lleft + (frPosUDC->Width * (i - 1) * 1.02f);
		frPosUDC->Top = (PanelScaffale->Height - frPosUDC->Height) / 2;

		frPosUDC->Enabled = true;
		frPosUDC->MyShape1->OnMouseUp = frScaffale1MyShape4MouseUp;
		frPosUDC->MyShape2->OnMouseUp = frScaffale1MyShape4MouseUp;
		frPosUDC->MyShape3->OnMouseUp = frScaffale1MyShape4MouseUp;
		frPosUDC->MyShape4->OnMouseUp = frScaffale1MyShape4MouseUp;
		/* frPosUDC->MyShape5->OnMouseUp = frScaffale1MyShape4MouseUp;
		 frPosUDC->MyShape6->OnMouseUp = frScaffale1MyShape4MouseUp;
		 frPosUDC->MyShape7->OnMouseUp = frScaffale1MyShape4MouseUp; */

		// operazioni comuni per tutti
		frPosUDC->Panel2->Caption = String(i);
		// dmDB->CaricaTabella("piani where pos = " + TabPosizioni[i]["POS"] + " order by piano", TabPiani);

	}

	for (i = 0; i <= TabPosizioni.size(); i++) {
		frPosUDC = (TfrPosUDC*)(FindComponent("frPosUDC" + IntToStr(i + 1)));
		if (frPosUDC != NULL) {
			frPosUDC->Panel2->Caption = TabPosizioni[i]["CUSTOMERPOS"];
			prenotata = TabPosizioni[i]["PRENOTATA"].ToIntDef(0);
			if ((!corsia_abilitata) || (TabPosizioni[i]["DISABILITA"].ToIntDef(0)))
				esclusa = 1;
			else
				esclusa = 0;

			dmDB->CaricaTabella("piani_view where Fila = " + IntToStr(MainForm->corsia_udc) + " AND pos = " + String(TabPosizioni[i]["POS"].ToIntDef(0)) + " order by piano", TabPiani);

			frPosUDC->RiempiCampi(TabPiani, prenotata, esclusa);
			idudc = TabPiani[pianosel]["IDUDC"].ToIntDef(0);
			// non funziona. Se si capisce come si puo' abilitare onmouseup su piani 4 e 5
			MyShape = (TMyShape*)(FindComponent("frPosUDC->MyShape" + IntToStr(pianosel + 1)));
			if (MyShape != NULL) {
				// MyShape->OnMouseUp = frScaffale1MyShape4MouseUp;
			}

			if (MainForm->pos_udc == TabPosizioni[i]["POS"].ToIntDef(0)) {
				Label2->Caption = TabPosizioni[i]["POS"];
				Label4->Caption = TabPosizioni[i]["CUSTOMERPOS"];
				statodisabilita = TabPosizioni[i]["DISABILITA"].ToIntDef(0);
				statoprenotata = TabPosizioni[i]["PRENOTATA"].ToIntDef(0);
				cbbTipoPosizione->Text = dmExtraFunction->RiempiTipologiaCorsia(cbbTipoPosizione, TabPosizioni[i]["TIPOPOSIZIONE"]); // dmDB->ReturnCodiceTipoUDCDaId(tipologia);

				eHDep->Text = TabPiani[pianosel]["HDEP"].ToIntDef(0);
				eHprel->Text = TabPiani[pianosel]["HPREL"].ToIntDef(0);
				ckDisabilitaPiano->Checked = TabPiani[pianosel]["DISABILITATA"].ToIntDef(0);
				ckPianoRiservato->Checked = TabPiani[pianosel]["PRENOTATA"].ToIntDef(0);
				numpiani = TabPosizioni[i]["NPIANI"].ToIntDef(0);
				pianioccupati = TabPosizioni[i]["NPIANIOCC"].ToIntDef(0);
				lbPiano->Caption = String(pianosel + 1);
				lbnomepiano->Caption = TabPiani[pianosel]["NOMEPOS"];
				edtRifOrdine->Text = TabPosizioni[i]["RIFORDINE"].Trim();
				Edit1->Text = TabPiani[pianosel]["TABCARICO"];
				Edit2->Text = TabPiani[pianosel]["TABSCARICO"];
				Edit3->Text = TabPosizioni[i]["LATO"];
                editTabella->Text=TabPiani[pianosel]["TABELLA"];

                lbPiano->Caption = TabPiani[pianosel]["PIANO"];

				ckDisabilitaPos->Checked = statodisabilita;
				cPrenotataPos->Checked = statoprenotata;
				if (idudc) {
					dmDB->FullTabellaK
						("Select IDUDC, CODUDC, ARTICOLI.DESCRIZIONE as [descart], CODTIPOUDC, ArticoliUDC.Codice as [CARTUDC] from UDC INNER JOIN TabTipoUDC ON UDC.CodTipoUDC = TabTipoUDC.IDTipoUDC LEFT OUTER JOIN ArticoliUDC ON UDC.IDArtUDC = ArticoliUDC.IDArtUDC LEFT OUTER JOIN Articoli ON ArticoliUDC.IDArticolo = Articoli.IDArticolo where idudc="
						+
						IntToStr(idudc) + " ORDER BY UDC.IDUDC", "IDUDC", TabUDC);
					eUDC->Text = IntToStr(idudc);
				}
				else {
					eUDC->Text = "0";
					status = 0;
					// cbbTipoPosizione->Text = "";

				}

				// frPosUDC->Shape1->Top = frPosUDC->MyShape1->Top - 10;
				frPosUDC->Shape1->BringToFront();
				frPosUDC->Shape1->Left = frPosUDC->MyShape1->Left - 10;
				frPosUDC->Shape1->Height = frPosUDC->MyShape1->Height + 10;
				frPosUDC->Shape1->Width = frPosUDC->MyShape1->Width + 20;
				frPosUDC->Shape1->Top = (TopPiano == 0 ? frPosUDC->MyShape1->Top : TopPiano) - 5;
				frPosUDC->Shape1->Visible = true;
			}
			else
				frPosUDC->Shape1->Visible = false;
			frPosUDC->Visible = true;

		}
	}

	BitBtnChange->Enabled = !idudc;

	aggiornamento = false;
	change_dati_pos = false;
}

void __fastcall TfCorsia::frScaffale1MyShape4MouseUp(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TMyShape *Pal;

	Pal = (TMyShape*) Sender;
	if (Pal != NULL) {
		pianosel = StrToInt(Pal->Name.SubString(Pal->Name.Length(), 1)) - 1;
		TopPiano = Pal->Top;
		if (Pal->Tag) {
			MainForm->pos_udc = Pal->Tag;
			VisualizzaPosizioneUDC();
		}
	}
}

// ---------------------------------------------------------------------------

void __fastcall TfCorsia::BitBtn3Click(TObject * Sender)
{
	if (Application->MessageBox(L"Are you sure you want to empty the Lane ?", L"Confirm!!!", MB_YESNO) == IDYES) {
		dmDB->SvuotaCorsia(MainForm->corsia_udc);
		VisualizzaPosizioneUDC();
		MainForm->aggiornamappa = 1;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::Timer1Timer(TObject * Sender)
{
	Timer1->Enabled = false;
	GroupBox1->Enabled = dmDB->pwdlevel;
	GroupBox3->Enabled = dmDB->pwdlevel;
	GroupBox8->Enabled = dmDB->pwdlevel;
	BitBtn3->Enabled = !ckVuota->Checked;
	BitBtn2->Enabled = (eUDC->Text.ToIntDef(0) > 0);

	if (count_inizio == 1)
		VisualizzaPosizioneUDC();
	if (count_inizio < 5)
		count_inizio++;

	if (MainForm->aggiorna_stato_pos) {
		VisualizzaPosizioneUDC();
		MainForm->aggiorna_stato_pos = 0;
	}
	BitBtn4->Enabled = change_dati_corsia;
	if ((dmDB->pwdlevel) && ((eUDC->Text.ToIntDef(0) > 0) || (change_dati_pos)))
		BitBtn6->Enabled = true;
	else
		BitBtn6->Enabled = false;
	Timer1->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::BitBtn4Click(TObject * Sender)
{
	AnsiString stringa, code;
	int res, tipologia;
	try {
		if ((dmDB->ADOConnection1->Connected) && (cbTipo->Items->IndexOf(cbTipo->Text) >= 0)) {
			tipologia = cbTipo->Text.SubString(1, 2).ToIntDef(0);

			res = dmDB->UpdateCorsia(MainForm->corsia_udc, ckPrenotata->Checked, ckAbilita->Checked,
				ckPiena->Checked, ckVuota->Checked, ckUsoPrel->Checked, ckUsoDep->Checked,
				cbPriorita->Text.ToIntDef(1), tipologia, ckIgnoraOrdine->Checked, cbDepSelettivo->Checked, edtAnnotazioni->Text);
			if (res) {
				// dmDB->LogMsg(stringa);
				CaricaDB();
				VisualizzaPosizioneUDC();
				// Close();
			}
			dmDB->aggiorna_tab_posizioni_locale = 1;

		}
		else
			ShowMessage("Lane Type not correct!!");
	}
	catch (...) {
	}

}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::BitBtn2Click(TObject * Sender)
{
	if (Application->MessageBox(L"Are you sure you want to empty the Position ?", L"Confirm!!!", MB_YESNO) == IDYES) {
		dmDB->SvuotaPiano(MainForm->pos_udc, pianosel + 1);
		CaricaDB();
		VisualizzaPosizioneUDC();
		// MainForm->aggiornamappa = 1 ;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::BitBtn6Click(TObject * Sender)
{
	AnsiString stringa;
	int res;
	TUDC UdcMod;
	try {
		if (dmDB->ADOConnection1->Connected) {
			//
			// AGGIIORNARE LA POSIZIONE NON SOLO L'UDC
			//
			//UdcMod.IDUDC = eUDC->Text.ToIntDef(0);
			if ((UdcMod.IDUDC) && (dmDB->pwdlevel)) {
				UdcMod.CodStato = 0;
				//UdcMod.CodTipoUDC = dmDB->RitornaTipoUDCdaUDC(UdcMod.IDUDC);

				UdcMod.IdArtUDC = 0; // eIDArtUDC->Text.ToIntDef(0);
				UdcMod.tara = 0; // eTara->Text.ToIntDef(0);
				UdcMod.pesoattuale = 0; // ePesoAttuale->Text.ToIntDef(0);
				//dmDB->InsertUpdateUDC(UdcMod);
				res = dmDB->UpdatePiano(MainForm->pos_udc, lbPiano->Caption.ToIntDef(0), eUDC->Text.ToIntDef(0), eHprel->Text.ToIntDef(0), eHDep->Text.ToIntDef(0));
			}
			if (cambiocheck) {
				if (Application->MessageBox(L"Are you sure you want change position status ?", L"Confirm!!!", MB_YESNO) == IDYES) {
					dmDB->UpdateSoloStato(MainForm->pos_udc, cPrenotataPos->Checked, ckDisabilitaPos->Checked);
					// if (ckDisabilitaPos->Checked)
					// dmGestMissioni->GestisciMissioniPeso(MainForm->pos_udc);
					cambiocheck = false;
				}

			}
			CaricaDB();
			VisualizzaPosizioneUDC();
			// }
			dmDB->aggiorna_tab_posizioni_locale = 1;
		}
	}
	catch (...) {
	}

}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::cbTipoChange(TObject * Sender) {
	if (!aggiornamento)
		change_dati_corsia = true;
	change_dati_pos = true;

}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::ckAbilitaClick(TObject * Sender)
{
	if (!aggiornamento)
		change_dati_corsia = true;

}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::BitBtn1Click(TObject * Sender)
{

	MainForm->corsia_udc = dmDBClient->ReturnNextFilaDaPos(Label6->Caption.ToIntDef(0));
	MainForm->pos_udc = dmDBClient->ReturnPrimaPosDaFila(MainForm->corsia_udc);
	CaricaDB();
	VisualizzaPosizioneUDC();

}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::BitBtn5Click(TObject * Sender) {
	MainForm->corsia_udc = dmDBClient->ReturnPrevFilaDaPos(Label6->Caption.ToIntDef(0));
	MainForm->pos_udc = dmDBClient->ReturnPrimaPosDaFila(MainForm->corsia_udc);
	CaricaDB();
	VisualizzaPosizioneUDC();

}

// ---------------------------------------------------------------------------
void __fastcall TfCorsia::sbArticoloClick(TObject * Sender)
{
	/*
	 FormAnagraficaArticoli->insert_produzione = true;
	 FormAnagraficaArticoli->IdArticolo = eIdArticolo->Text.ToIntDef(0);
	 FormAnagraficaArticoli->idartudc = eIDArtUDC->Text.ToIntDef(0);
	 FormAnagraficaArticoli->ShowModal();
	 dmDB->InsertArticoloPosizione(FormAnagraficaArticoli->IdArticolo, FormAnagraficaArticoli->idartudc);
	 VisualizzaPosizioneUDC();
	 change_dati_pos = true;
	 */
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
void __fastcall TfCorsia::ckDisabilitaPosClick(TObject * Sender)
{
	if (!aggiornamento) {
		change_dati_pos = true;
		cambiocheck = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TfCorsia::cPrenotataPosClick(TObject * Sender)
{
	if (!aggiornamento) {
		change_dati_pos = true;
		cambiocheck = true;
		ckPrenotata->Checked = cPrenotataPos->Checked;
	} // siamo sicuri? e se ci sono altre prenotazioni
}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::SpeedButton3Click(TObject * Sender)
{
	TUDC UdcMod;
	if (Application->MessageBox(L"Are you sure to delete this Item?", L"Confirm!!!", MB_YESNO) == IDYES) {
		UdcMod.IDUDC = eUDC->Text.ToIntDef(0);
		///dmDB->LeggiStrutturaUdc(UdcMod);
		if (UdcMod.IDUDC) {
			UdcMod.IdArtUDC = 0;
		   //	dmDB->InsertUpdateUDC(UdcMod);
		   ///	VisualizzaPosizioneUDC();
		}
		else
			ShowMessage("IdUdc not found");

	}
}

// ---------------------------------------------------------------------------

void __fastcall TfCorsia::btDisabilitaPianiClick(TObject * Sender)
{
	// FormSettaPiani->fila = StrToInt(Label6->Caption);
	// FormSettaPiani->ShowModal();
	// VisualizzaPosizioneUDC();
}

void __fastcall TfCorsia::BitBtnChangeClick(TObject * Sender)
{
	CentroMissione.posdep = MainForm->pos_udc;
	CentroMissione.pianodep = pianosel + 1;
	MainForm->piano_udc = CentroMissione.pianodep;
	/*
	 TMenuItem *NewItemx = new TMenuItem(this);
	 NewItemx->Tag = MainForm->pos_udc;
	 dmGestMissioni->MisManSpostPLCClick(NewItemx);
	 delete NewItemx;
	 */
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void __fastcall TfCorsia::BitBtn7Click(TObject *Sender)
{
	CentroMissione.posprel = MainForm->pos_udc;
	CentroMissione.pianoprel = pianosel + 1;
	MainForm->piano_udc = CentroMissione.pianoprel;
	/*
	 TMenuItem *NewItemx = new TMenuItem(this);
	 NewItemx->Tag = MainForm->pos_udc;
	 dmGestMissioni->MisManSpostPLCClick(NewItemx);
	 delete NewItemx;
	 */
}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::btnModificaAltezzaClick(TObject *Sender)
{
   	dmDB->UpdateAltezzaPiano(MainForm->pos_udc, pianosel + 1, MainForm->corsia_udc, eHprel->Text.ToIntDef(0), eHDep->Text.ToIntDef(0));
}
//---------------------------------------------------------------------------

