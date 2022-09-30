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
#include "DBImpianto.h";

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma link "MyShape"
#pragma link "pos_UDC"
#pragma resource "*.dfm"
TfCorsia *fCorsia;

// ---------------------------------------------------------------------------
__fastcall TfCorsia::TfCorsia(TComponent* Owner)
    : TMDIChild(Owner) {
    // loadLanguage();
    // _TSL_FORM(this);
    change_dati_corsia = false;
    change_dati_pos = false;

}

// ---------------------------------------------------------------------------
void __fastcall TfCorsia::FormCreate(TObject *Sender) {
    // PER LANCIARE IL PRIMO GIRO SOLO UNA VOLTA, ALTRIMENTI PARTE DUE VOLTE
    cambiocheck = false;
    pianosel = 0;
    // MainForm->pos_udc
    VisualizzaPosizioneUDC();
}

// ---------------------------------------------------------------------------
void __fastcall TfCorsia::FormActivate(TObject * Sender) {
    Timer1Timer(this);
}

// ---------------------------------------------------------------------------
void TfCorsia::VisualizzaPosizioneUDC() {
    TfrPosUDC *frPosUDC;
    TMyShape *MyShape;

    AnsiString strsql, zona, str;
    // in base al numero di agv abilitati aggiorno le posizioni di attesa per ogni agv
    TRecordList TabPosizioni;
    TRecordList TabCorsie;
    TRecordList TabPiani;
    TIndexList TabUDC;

    int i, j, idudc, prenotata, esclusa, corsia_abilitata = 0, status, tipologia, corsia_da_destra, numpiani, pianioccupati;
    int lleft;

    corsia_da_destra = dmDB->ReturnCorsiaDaDestraDaPos(MainForm->pos_udc);
    if (corsia_da_destra)
        str = "Posizionicondatiudc WHERE Fila = " + IntToStr(MainForm->corsia_udc) + " ORDER BY Pos DESC"; // parto sempre da sx con la pos chiamata 1, poi nel crs può essere che sia l'ultima, dipende come deposito in corsia
    else
        str = "Posizionicondatiudc WHERE Fila = " + IntToStr(MainForm->corsia_udc) + " ORDER BY Pos"; // parto sempre da sx con la pos chiamata 1, poi nel crs può essere che sia l'ultima, dipende come deposito in corsia
    dmDB->CaricaTabella(str, TabPosizioni);
    dmDB->CaricaTabella("Corsie where Fila = " + IntToStr(MainForm->corsia_udc), TabCorsie);

    dmDB->FullTabellaK("Select * from UDC_view IDUDC", "IDUDC", TabUDC);
    i = 0;
    aggiornamento = true;

    // corsie
    if (i < TabCorsie.size()) {
        tipologia = TabCorsie[i]["TIPOLOGIA"].ToIntDef(0);
        lbNumCorsia->Caption = TabCorsie[i]["FILA"];
        Label8->Caption = TabCorsie[i]["NOMECORSIA"];
        Label11->Caption = TabCorsie[i]["ZONA"];
        ckAbilita->Checked = TabCorsie[i]["ABILITATA"].ToIntDef(0);
        corsia_abilitata = TabCorsie[i]["ABILITATA"].ToIntDef(0);
        ckPrenotata->Checked = TabCorsie[i]["CORSIAPRENOTATA"].ToIntDef(0);
        ckPiena->Checked = TabCorsie[i]["PIENA"].ToIntDef(0);
        ckVuota->Checked = TabCorsie[i]["VUOTA"].ToIntDef(0);
        ckUsoPrel->Checked = TabCorsie[i]["INUSOPRELIEVO"].ToIntDef(0);
        ckUsoDep->Checked = TabCorsie[i]["INUSODEPOSITO"].ToIntDef(0);
        cbTipo->Text = dmExtraFunction->RiempiTipologiaCorsia(cbTipo, dmExtraFunction->PadS(TabCorsie[i]["TIPOLOGIA"].ToIntDef(0), 2, "0")); // dmDB->ReturnCodiceTipoUDCDaId(tipologia);
        ckIgnoraOrdine->Checked = TabCorsie[i]["IGNORARIFORDINE"].ToIntDef(0);
        cbPriorita->Text = TabCorsie[i]["PRIORITA"];
        cbDepSelettivo->Checked = TabCorsie[i]["DEPOSITOSELETTIVO"].ToIntDef(0);
        edtAnnotazioni->Text = TabCorsie[i]["ANNOTAZIONI"];
    }

    Panel3->Width = 52 + (1590 - (frPosUDC1->Width * TabPosizioni.size())) / 2;
    lleft = Panel3->Left + Panel3->Width;
    // crea e gestisci i frame
    // Cancella i frame esistenti maggiori di quelli presenti
    // per fare una roba ottimizzare dovrei mettere in static il valore preceente
    for (i = TabPosizioni.size() + 1; i <= 15; i++) {
        frPosUDC = (TfrPosUDC*)(FindComponent("frPosUDC" + IntToStr(i)));
        if (frPosUDC != NULL)
            delete frPosUDC;
    }

    for (i = 1; i <= TabPosizioni.size(); i++) {
        frPosUDC = (TfrPosUDC*)(FindComponent("frPosUDC" + IntToStr(i)));

        if (frPosUDC == NULL) {
            frPosUDC = new TfrPosUDC(this);
            frPosUDC->Parent = this->Panel2;
            frPosUDC->Name = "frPosUDC" + IntToStr(i);
        }
        frPosUDC->Hint = "frPosUDC" + IntToStr(i);
        frPosUDC->Tag = i;
        frPosUDC->Left = lleft + (frPosUDC->Width * (i - 1));
        frPosUDC->Top = 0;
        frPosUDC->Enabled = true;
        frPosUDC->MyShape1->OnMouseUp = frScaffale1MyShape4MouseUp;
        frPosUDC->MyShape2->OnMouseUp = frScaffale1MyShape4MouseUp;
        frPosUDC->MyShape3->OnMouseUp = frScaffale1MyShape4MouseUp;

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

            dmDB->CaricaTabella("piani_view where pos = " + String(TabPosizioni[i]["POS"].ToIntDef(0)) + " order by piano", TabPiani);

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
                cbbTipoPosizione->Text = dmExtraFunction->RiempiTipologiaCorsia(cbbTipoPosizione, dmExtraFunction->PadS(TabPosizioni[i]["TIPOPOSIZIONE"].ToIntDef(0), 2, "0")); // dmDB->ReturnCodiceTipoUDCDaId(tipologia);

                eHDep->Text = TabPiani[pianosel]["HDEP"].ToIntDef(0);
                eHprel->Text = TabPiani[pianosel]["HPREL"].ToIntDef(0);
                ckDisabilitaPiano->Checked = TabPiani[pianosel]["DISABILITATA"].ToIntDef(0);
                ckPianoRiservato->Checked = TabPiani[pianosel]["PRENOTATA"].ToIntDef(0);
                numpiani = TabPosizioni[i]["NPIANI"].ToIntDef(0);
                pianioccupati = TabPosizioni[i]["NPIANIOCC"].ToIntDef(0);
                lbPiano->Caption = String(pianosel + 1);
                lbnomepiano->Caption = TabPiani[pianosel]["NOMEPOS"];
                edtRifOrdine->Text = TabPosizioni[i]["RIFORDINE"].Trim();

                ckDisabilitaPos->Checked = statodisabilita;
                cPrenotataPos->Checked = statoprenotata;
                if (idudc) {
                    eUDC->Text = IntToStr(idudc);
                    eIdArticolo->Text = TabUDC[idudc]["IDARTICOLO"];
                    // eTipologia->Text = dmDB->ReturnCodiceTipologiaDaId(TabUDC[idudc]["DESCRIZIONE"].ToIntDef(0)); // TipologiaArticoloStr[TabTotem[idtotem]["TIPOLOGIAARTICOLO"].ToIntDef(0)]  ;
                    eNome->Text = TabUDC[idudc]["NOME"]; // vedere cosa mettere
                    eTara->Text = TabUDC[idudc]["TARA"];
                    ePesoAttuale->Text = TabUDC[idudc]["PESOATTUALE"];
                    ePesoBase->Text = TabUDC[idudc]["PESOINIZIALE"];
                    eCaloKg->Text = TabUDC[idudc]["CALOPESOKG"];
                    eCaloPerc->Text = TabUDC[idudc]["CALOPESOPERC"];
                    // eProdotto->Text = TabUDC[idudc]["DESCART"];
                    // eStatus->Text = dmDBClient->ReturnDescStatoUDC(TabUDC[idudc]["STATO"].ToIntDef(0));
                    cbTipoUDC->Text = dmExtraFunction->RiempiTipoUDC(cbTipoUDC, dmExtraFunction->PadS(TabPiani[pianosel]["CODTIPOUDC"].ToIntDef(0), 2, "0"));
                    ////   cbbArtUDC->Text = dmExtraFunction->RiempiArticoliUDC(cbbArtUDC, TabPiani[pianosel]["IDARTUDC"]);
                    eIDArtUDC->Text = TabUDC[idudc]["IDARTUDC"].ToIntDef(0);
                    eCodeArtUDC->Text = TabUDC[idudc]["CARTUDC"];
                    edCodUDC->Text = dmDB->RitornaCodiceUDCdaUDC(idudc);
                    cbImpilabile->Checked = TabUDC[idudc]["PIANIPERUDC"].ToIntDef(0);
                }
                else {
                    eUDC->Text = "0";
                    status = 0;
                    eIdArticolo->Text = "0";
                    // eTipologia->Text = dmDB->ReturnCodiceTipoUDCDaId(0); // TipologiaArticoloStr[TabTotem[idtotem]["TIPOLOGIAARTICOLO"].ToIntDef(0)]  ;
                    eNome->Text = ""; // vedere cosa mettere
                    eTara->Text = "0";
                    ePesoAttuale->Text = "0";
                    ePesoBase->Text = "0";
                    eCaloKg->Text = "0";
                    eCaloPerc->Text = "0";
                    // eProdotto->Text = TabUDC[idudc]["DESCART"];
                    // eProdotto->Text = "";
                    cbTipoUDC->Text = dmExtraFunction->RiempiTipoUDC(cbTipoUDC, "00");
                    ////    cbbArtUDC->Text = dmExtraFunction->RiempiArticoliUDC(cbbArtUDC, "0");
                    eIDArtUDC->Text = "";
                    eCodeArtUDC->Text = "";
                    edCodUDC->Text = "";
                    cbImpilabile->Checked = 0;
                    // cbbTipoPosizione->Text = "00";

                }

                // frPosUDC->Shape1->Top = frPosUDC->MyShape1->Top - 10;
                frPosUDC->Shape1->BringToFront();
                frPosUDC->Shape1->Left = frPosUDC->MyShape1->Left - 4;
                frPosUDC->Shape1->Height = frPosUDC->MyShape1->Height;
                frPosUDC->Shape1->Top = 270 - ((pianosel + 1) * frPosUDC->Shape1->Height);
                frPosUDC->Shape1->Visible = true;
            }
            else
                frPosUDC->Shape1->Visible = false;
            frPosUDC->Visible = true;

        }
    }

    aggiornamento = false;
    change_dati_pos = false;
}

void __fastcall TfCorsia::frScaffale1MyShape4MouseUp(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    TMyShape *Pal;

    Pal = (TMyShape*) Sender;
    if (Pal != NULL) {
        pianosel = StrToInt(Pal->Name.SubString(Pal->Name.Length(), 1)) - 1;
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
    GroupBox4->Enabled = dmDB->pwdlevel;
    GroupBox8->Enabled = dmDB->pwdlevel;
    BitBtn3->Enabled = !ckVuota->Checked;
    BitBtn2->Enabled = (eUDC->Text.ToIntDef(0) > 0);
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
            ////      dmDBImpianto->CambiaZonaPosizione(lbNumCorsia->Caption.ToIntDef(0), tipologia);
            res = res + dmDB->UpdateCorsia(MainForm->corsia_udc, ckPrenotata->Checked, ckAbilita->Checked,
                ckPiena->Checked, ckVuota->Checked, ckUsoPrel->Checked, ckUsoDep->Checked,
                cbPriorita->Text.ToIntDef(1), tipologia, ckIgnoraOrdine->Checked, cbDepSelettivo->Checked, edtAnnotazioni->Text);
            if (res) {
                // dmDB->LogMsg(stringa);
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

void __fastcall TfCorsia::BitBtnChangeClick(TObject * Sender)
{
    int idartudc, status = 0;
    /*
     idartudc = eIDArtUDC->Text.ToIntDef(0);

     // udcd settato pieno e artudc non definito
     if ((idartudc == 0) && (status == 3)) {
     FormDatiUDC->pos_update = false;
     FormDatiUDC->IDUDC = 0;
     FormDatiUDC->ShowModal();
     eIDArtUDC->Text = FormDatiUDC->fUDCSmall->eIDArtUDC->Text;
     eIdArticolo->Text = FormDatiUDC->edArticolo->Text;
     BitBtn6Click(this);
     }
     // udcd settato pieno e artudc  definito
     else if ((idartudc != 0) && (status == 3)) {
     FormDatiUDC->pos_update = true;
     FormDatiUDC->IDUDC = eUDC->Text.ToIntDef(0);
     FormDatiUDC->eIDArtUDCm->Text = idartudc;
     FormDatiUDC->ShowModal();
     eIDArtUDC->Text = FormDatiUDC->fUDCSmall->eIDArtUDC->Text;
     eIdArticolo->Text = FormDatiUDC->edArticolo->Text;
     BitBtn6Click(this);

     }
     // artudc settato ma stato vuoto
     else if ((idartudc != 0) && (status < 3)) {
     Application->MessageBox(L"With article UDC, UDC can't be empty", L"Please correct", MB_OK);
     }
     else
     Application->MessageBox(L"Fill article UDC and set UDC status as FULL to confirm", L"Please correct", MB_OK);
     */
}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::BitBtn2Click(TObject * Sender)
{
    if (Application->MessageBox(L"Are you sure you want to empty the Position ?", L"Confirm!!!", MB_YESNO) == IDYES) {
        dmDB->SvuotaPiano(MainForm->pos_udc, pianosel + 1);
        VisualizzaPosizioneUDC();
        // MainForm->aggiornamappa = 1 ;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::BitBtn6Click(TObject * Sender) {
    AnsiString stringa;
    int res, artudc;
    TUDC UdcMod;
    try {
        if (dmDB->ADOConnection1->Connected) {
            //
            // AGGIIORNARE LA POSIZIONE NON SOLO L'UDC
            //
            UdcMod.IDUDC = eUDC->Text.ToIntDef(0);
            ////    dmDB->LeggiStrutturaUdc(UdcMod);
            if ((UdcMod.IDUDC) && (dmDB->pwdlevel)) {
                ////        UdcMod.stato = 0;
                UdcMod.CodTipoUDC = StrToInt(cbTipoUDC->Text.SubString(1, 2));

                UdcMod.IdArtUDC = 0; // eIDArtUDC->Text.ToIntDef(0);
                UdcMod.tara = 0; // eTara->Text.ToIntDef(0);
                UdcMod.pesoattuale = 0; // ePesoAttuale->Text.ToIntDef(0);
                artudc = (eUDC->Text.ToIntDef(0) == 1 ? 0 : cbbArtUDC->Text.ToIntDef(0));
                ////    dmDB->InsertUpdateUDC(UdcMod);
                ////        res = dmDB->UpdatePiano(MainForm->pos_udc, lbPiano->Caption.ToIntDef(0), eUDC->Text.ToIntDef(0), eHprel->Text.ToIntDef(0), eHDep->Text.ToIntDef(0), artudc);
            }
            if (cambiocheck) {
                if (Application->MessageBox(L"Sei sicuro di voler cambiare lo stato UDC ?", L"Conferma!!!", MB_YESNO) == IDYES) {
                    dmDB->UpdateSoloStato(MainForm->pos_udc, cPrenotataPos->Checked, ckDisabilitaPos->Checked);
                    // if (ckDisabilitaPos->Checked)
                    // dmGestMissioni->GestisciMissioniPeso(MainForm->pos_udc);
                    cambiocheck = false;
                }

            }
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

void TfCorsia::CalcolaPesoTotale() {
    AnsiString stringa;
    stringa = "SELECT  COUNT(*) AS N, SUM(UDC.PesoAttuale ) AS T, SUM(UDC.Tara ) AS UDCTelai from UDC inner join Posizioni on UDC.IdUDC = Posizioni.IdUDC  where Posizioni.Fila = " + IntToStr(MainForm->corsia_udc) + " and Posizioni.IdUDC > 0 ";
    try {
        dmDB->QueryPeso->Close();
        dmDB->QueryPeso->SQL->Clear();
        dmDB->QueryPeso->SQL->Append(stringa);
        dmDB->QueryPeso->Open();
        leNumUDC->Text = "0";
        lePesoTotale->Text = "0";
        lePesoTotaleNoTara->Text = "0";
        if (dmDB->QueryPeso->FieldByName("T")->AsString != "") {
            leNumUDC->Text = dmDB->QueryPeso->FieldByName("N")->AsInteger;
            lePesoTotale->Text = dmDB->QueryPeso->FieldByName("T")->AsInteger;
            lePesoTotaleNoTara->Text = dmDB->QueryPeso->FieldByName("T")->AsInteger - dmDB->QueryPeso->FieldByName("Tara")->AsInteger;
        }
        dmDB->QueryPeso->Close();
    }
    catch (...) {
        dmDB->LogMsg("Eccezione su calcolo pesi corsie");
    }
}

void __fastcall TfCorsia::ckAbilitaClick(TObject * Sender)
{
    if (!aggiornamento)
        change_dati_corsia = true;

}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::BitBtn1Click(TObject * Sender)
{

    MainForm->corsia_udc = dmDBClient->ReturnNextFilaDaPos(lbNumCorsia->Caption.ToIntDef(0));
    MainForm->pos_udc = dmDBClient->ReturnPrimaPosDaFila(MainForm->corsia_udc);
    VisualizzaPosizioneUDC();

}
// ---------------------------------------------------------------------------

void __fastcall TfCorsia::BitBtn5Click(TObject * Sender) {
    MainForm->corsia_udc = dmDBClient->ReturnPrevFilaDaPos(lbNumCorsia->Caption.ToIntDef(0));
    MainForm->pos_udc = dmDBClient->ReturnPrimaPosDaFila(MainForm->corsia_udc);
    VisualizzaPosizioneUDC();

}

// ---------------------------------------------------------------------------
void __fastcall TfCorsia::SpeedButton2Click(TObject * Sender)
{
    FormUDCList->blocca_insert = false;
    FormUDCList->pos = MainForm->pos_udc;
    FormUDCList->ShowModal();
    VisualizzaPosizioneUDC();
    change_dati_pos = true;

}

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
    if (Application->MessageBox(L"Sei sicuro di voler cancellare questo elemento?", L"Conferma!!!", MB_YESNO) == IDYES) {
        UdcMod.IDUDC = eUDC->Text.ToIntDef(0);
        ////   dmDB->LeggiStrutturaUdc(UdcMod);
        if (UdcMod.IDUDC) {
            UdcMod.IdArtUDC = 0;
            ////       dmDB->InsertUpdateUDC(UdcMod);
            VisualizzaPosizioneUDC();
        }
        else
            ShowMessage("IdUdc non trovato");

    }
}

// ---------------------------------------------------------------------------
