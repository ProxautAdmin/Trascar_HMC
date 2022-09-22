// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "missioni.h"
#include "db.h"
#include "main.h"
#include "ExtraFunction.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma link "cspin"
#pragma resource "*.dfm"
TfMissioni *fMissioni;

// ---------------------------------------------------------------------------
__fastcall TfMissioni::TfMissioni(TComponent* Owner)
    : TMDIChild(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TfMissioni::BitBtn1Click(TObject *Sender)
{
    if (Application->MessageBox(L"Are you sure you want to empty the Historic ?", L"Confirm!!!", MB_YESNO) == IDYES) {
        Query1->Close();
        Query1->SQL->Clear();
        Query1->SQL->Append("delete from Missioni ");
        Query1->ExecSQL();
        Query1->Close();
        dmDB->LogMsg("Delete from missioni");
        FormActivate(Sender);
    }

}

// ---------------------------------------------------------------------------
void __fastcall TfMissioni::FormActivate(TObject *Sender)
{
    IndexListIterator ind;
    int i;
    DaData->Date = Date();
    AData->Date = Date();
    FilterOnDateFrom->Checked = false;
    FilterOnDateTo->Checked = false;
    CheckBox1->Checked = false;
    CheckBox2->Checked = false;
    CheckBox3->Checked = false;
    CheckBox4->Checked = false;
    CheckBox5->Checked = false;
    CheckBox6->Checked = false;
    cbMsnAttive->Checked = true;

    edCodUDC->Text = "";
    edPickCust->Text = "";
    edDropCust->Text = "";

    if (dmDB->ZonaString == "Z") {
        BitBtn1->Visible = false;
        BitBtn2->Visible = false;
        btCancellaRiga->Visible = false;
    }
    else {
        BitBtn1->Enabled = false; // dmDB->pwdlevel;
        btCancellaRiga->Visible = dmDB->pwdlevel;
    }

    cbEsito->Text = dmExtraFunction->RiempiEsitoMissione(cbEsito, "INIZIATA");

    if (MainForm->recnum > 0) {
        NMis->Text = MainForm->recnum;
        cbMsnAttive->Checked = false;
        NMisCheck->Checked = true;
        MainForm->recnum = 0;
    }

    // nascondi campi se non amministratore
    // for (i = 14; i <= 33; i++) {
    // DBGrid1->Columns->Items[i]->Visible = dmDB->pwdlevel;
    // }

}
// ---------------------------------------------------------------------------

void __fastcall TfMissioni::NMisCheckClick(TObject *Sender)
{
    Word Year, Month, Day;
    AnsiString str = "", filter = "", filtrotempo, filtronpallet, filtronpalletvuoti;
    int secondi_totali, secondi_missione, filtropesototale;
    float result, result_mis;
    filter = " where (1=1) ";

    Query1->Close();
    Query1->SQL->Clear();

    if (cbMsnAttive->Checked)
        filter = filter + " and (esito <>'COMPLETATA' and esito <>'ABORTITA' and esito <>'CANCELLATA') ";

    if (cbMissReali->Checked)
        filter = filter + " and (tipomissione = 0 or  tipomissione =2) ";

    if (NMisCheck->Checked) {
        filter = filter + " and Id =" + NMis->Value;
    }

    if (PrelCheck->Checked) {
        filter = filter + " and PosPrel >=" + Prel->Value;
        filter = filter + " AND PosPrel <=" + Dep->Value;
    }

    if (DepCheck->Checked) {
        filter = filter + " and PosDep >=" + CSpinEdit1->Value;
        filter = filter + " AND PosDep <=" + CSpinEdit2->Value;
    }

    if (FilterOnDateFrom->Checked) {
        DecodeDate(DaData->Date, Year, Month, Day);
        str = DateToStr(DaData->Date) + " " + TimeToStr(DaOra->Time);
        filter = filter + " and generata >= '" + FormatDateTime(MainForm->format_data_ora_datatbase, StrToDateTime(str)) + "'";
    }

    if ((CheckBox2->Checked) && (edCodUDC->Text != "")) {
        filter = filter + " and CodUDC LIKE '%" + edCodUDC->Text + "%'";
    }

    if ((CheckBox1->Checked) && (cbEsito->Items->IndexOf(cbEsito->Text) >= 0)) {
        filter = filter + " and Esito = '" + cbEsito->Text + "'";
    }
    if ((CheckBox3->Checked) && (edPickCust->Text != "")) {
        filter = filter + " and NomePosPrel LIKE '%" + edPickCust->Text + "%'";
    }
    if ((CheckBox6->Checked) && (edDropCust->Text != "")) {
        filter = filter + " and NomePosDep LIKE '%" + edDropCust->Text + "%'";
    }

    if (CheckBox4->Checked) {
        filter = filter + " and Agv =" + CSpinEdit3->Value;
    }
    if ((CheckBox5->Checked) && (Edit1->Text.ToIntDef(0) > 0)) {
        filter = filter + " and IdUDC =" + Edit1->Text;
    }

    if ((CheckBox7->Checked) && (edCM->Text.ToIntDef(0) > 0)) {
        filter = filter + " and IdCentromissioni =" + edCM->Text;
    }

    if (FilterOnDateTo->Checked) {
        DecodeDate(AData->Date, Year, Month, Day);
        // str = IntToStr(Day)+"."+IntToStr(Month)+"."+IntToStr(Year); //il separatore per le query sulle tabelle di BDE è il . e non la barra /
        str = DateToStr(AData->Date) + " " + TimeToStr(AOra->Time);
        filter = filter + " and generata <= '" + FormatDateTime(MainForm->format_data_ora_datatbase, StrToDateTime(str)) + "'";
    }

    filtrotempo = "SELECT COUNT(*) AS N, SUM(CAST(Fine - Iniziata AS float) * 24 * 60 * 60) AS T FROM Missioni_view" + filter + " AND NOT(Fine IS NULL) AND NOT(Iniziata IS NULL) and Esito ='COMPLETATA'";
    // filtronpallet = "SELECT COUNT(*) AS N, SUM(Peso) AS PesoTotale FROM Missioni_view " + filter + " AND ( TipoMissione = 0 or TipoMissione = 2 ) AND NOT(Fine IS NULL) AND NOT(Iniziata IS NULL) and Esito ='COMPLETATA' AND (IdArtUdc > 0 )";
    // filtronpalletvuoti = "SELECT COUNT(*) AS N FROM Missioni_view " + filter + "  AND NOT(Fine IS NULL) AND NOT(Iniziata IS NULL) and Esito ='COMPLETATA' AND (IdUDC= 0  )";
    filter = "Select *, trim(risposta_agvx) as trimrisposta_agvx from Missioni_view " + filter;

    filter = filter + " order by generata";
    Query1->SQL->Append(filter);
    Query1->Open();
    Tot->Caption = IntToStr(Query1->RecordCount);
    Query1->Last();
    Label12->Caption = "0";
    Label14->Caption = "0";
    Label16->Caption = "0";
    StringaExcel = filter;

    try {
        ADOQuery1->Close();
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Append(filtrotempo);
        ADOQuery1->Open();
        if (ADOQuery1->FieldByName("T")->AsString != "") {
            result = ADOQuery1->FieldByName("T")->AsFloat;
            result_mis = result / Query1->RecordCount;
            secondi_totali = ADOQuery1->FieldByName("T")->AsInteger;
            secondi_missione = secondi_totali / Query1->RecordCount;
            // str.sprintf("%.2f",result) ;
            // lbTotConcluse->Caption = IntToStr(Query1->RecordCount);
            str.sprintf("%.2f", result_mis);
            Label14->Caption = str + " sec.";
            Label16->Caption = dmDB->RitornaOrarioDaSecondi(secondi_missione);
            Label12->Caption = dmDB->RitornaOrarioDaSecondi(result);
        }
        ADOQuery1->Close();
    }
    catch (...) {
        dmDB->LogMsg("Eccezione su calcolo tempistiche missioni");
    }

}

// ---------------------------------------------------------------------------
void __fastcall TfMissioni::BitBtn2Click(TObject *Sender)
{
    int res;
    if (Application->MessageBox(L"Do you want to historicise table?", L"Please confirm", MB_YESNO) == IDYES) {
        res = dmExtraFunction->StoricizzaTabelle("missioni", "History_missioni", "generata", "MONTH", 3);
        if (res > 0)
            Application->MessageBox(L"Historicised", L"Ok", MB_OK);
        else
            Application->MessageBox(L"Historicizing problems", L"Read log", MB_OK);
        FormActivate(Sender);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TfMissioni::btEsportaClick(TObject *Sender) {
    if (Tot->Caption.ToIntDef(0) > 0)
        dmExtraFunction->EsportaExcel(StringaExcel, "Missioni");
}

// ---------------------------------------------------------------------------
void __fastcall TfMissioni::btCancellaRigaClick(TObject *Sender)
{
    AnsiString sqlstring;
    AnsiString tempo;
    tempo = Query1->FieldByName("tempo")->AsString;
    if (tempo == "") {
        if (Application->MessageBox(L"OPERAZIONE PERICOLOSA! Vuoi eliminare il record?", L"Confermare", MB_YESNO) == IDYES) {
            try {
                dmDB->SettaPianoSelezionato(Query1->FieldByName("posprel")->AsInteger, Query1->FieldByName("pianoprel")->AsInteger, 0);
                dmDB->PrenotaPos(Query1->FieldByName("posprel")->AsInteger, 0, dmDB->FilaPosizione(Query1->FieldByName("posprel")->AsInteger));
                dmDB->SettaPianoSelezionato(Query1->FieldByName("posdep")->AsInteger, Query1->FieldByName("pianodep")->AsInteger, 0);
                dmDB->PrenotaPos(Query1->FieldByName("posdep")->AsInteger, 0, dmDB->FilaPosizione(Query1->FieldByName("posprel")->AsInteger));

                QueryDel->Close();
                QueryDel->SQL->Clear();
                sqlstring = "delete from missioni where ID=" + Query1->FieldByName("ID")->AsString;
                QueryDel->SQL->Append(sqlstring);
                QueryDel->ExecSQL();
                QueryDel->Close();
                dmDB->LogMsg("Eliminazione manuale record missioni " + Query1->FieldByName("ID")->AsString);
            }
            catch (...) {
                dmDB->LogMsg("Problema eliminazione manuale record missioni " + Query1->FieldByName("ID")->AsString);
            }
            NMisCheckClick(Sender);
        }
    }
    else {
        ShowMessage("Missione gia' conclusa non eliminabile.");
        NMisCheckClick(Sender);
    }
}
// ---------------------------------------------------------------------------

void __fastcall TfMissioni::btMssioniPlusClick(TObject *Sender)
{
    if ((Query1->Active) && (Query1->RecNo)) {
//        MainForm->CentraleUDCClick(this);

    }
}
// ---------------------------------------------------------------------------
