// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CentroMissioni.h"
#include "db.h"
#include "main.h"
#include "ExtraFunction.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma link "cspin"
#pragma resource "*.dfm"
TfCentroMissioni *fCentroMissioni;

// ---------------------------------------------------------------------------
__fastcall TfCentroMissioni::TfCentroMissioni(TComponent* Owner)
    : TMDIChild(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TfCentroMissioni::BitBtn1Click(TObject *Sender)
{
    if (Application->MessageBox(L"Sei sicuro di voler eleminare il CentroMissioni?", L"Confirm!!!", MB_YESNO) == IDYES) {
        Query1->Close();
        Query1->SQL->Clear();
        Query1->SQL->Append("delete from CentroMissioni ");
        Query1->ExecSQL();
        Query1->Close();
        dmDB->LogMsg("Delete from centromissioni");
        FormActivate(Sender);
    }

}

// ---------------------------------------------------------------------------
void __fastcall TfCentroMissioni::FormActivate(TObject *Sender)
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

    CheckBox5->Checked = false;

    CheckBox7->Checked = true;

    if (dmDB->ZonaString == "XXX") {
        btDelRiga->Visible = false; // dmDB->pwdlevel;
        btGestCMiss->Visible = false; // dmDB->pwdlevel;
        BitBtn2->Visible = false;
        BitBtn1->Visible = false;
        btBarcode->Visible = false;
    }
    else {
        btBarcode->Visible = true;
        BitBtn1->Visible = false;
    }

    Destinazione1->Text = "";

    // cbTipoMissione->Text=dmExtraFunction->RiempiTipoMissione(cbTipoMissione, "01");
    cbEsito->Text = dmExtraFunction->RiempiStatoMissione(cbEsito, "01");

    NMis->Visible = dmDB->pwdlevel;
    NMisCheck->Visible = dmDB->pwdlevel;

    NMisCheckClick(this);

}
// ---------------------------------------------------------------------------

void __fastcall TfCentroMissioni::NMisCheckClick(TObject *Sender)
{
    Word Year, Month, Day;
    AnsiString str = "", filter = " where (1=1) ", filtronpalletvuoti;
    int secondi_totali, secondi_missione, filtropesototale;
    float result, result_mis;

    Query1->Close();
    Query1->SQL->Clear();
    if (NMisCheck->Checked) {
        filter = filter + " and Id =" + NMis->Value;
    }
    if (CheckBox7->Checked) {
        filter = filter + " and stato <1";
        CheckBox1->Enabled = false;
        CheckBox7->Font->Color = clRed;
    }
    else {
        CheckBox7->Font->Color = clBlack;
        CheckBox1->Enabled = true;
    }

    if (PrelCheck->Checked) {
        filter = filter + " and PosPrel >=" + Prel->Value;
        filter = filter + " AND PosPrel <=" + Dep->Value;
    }
    if (DepCheck->Checked) {
        filter = filter + " and PosDep >=" + CSpinEdit1->Value;
        filter = filter + " AND PosDep <=" + CSpinEdit2->Value;
    }
    if ((cbPrelievo->Checked) && (edPrelievo->Text != "")) {
        filter = filter + " and nomeposprel LIKE'%" + edPrelievo->Text + "%'";
    }
    if ((CheckBox2->Checked) && (Destinazione1->Text != "")) {
        filter = filter + " and nomeposDep LIKE'%" + Destinazione1->Text + "%'";
    }

    if (CheckBox3->Checked) {
        filter = filter + " and CodUDC like '%" + edCodUDC->Text + "%'";
    }

    if ((CheckBox5->Checked) && (Edit1->Text.ToIntDef(0) > 0)) {
        filter = filter + " and IdUDC =" + Edit1->Text;
    }
    if (FilterOnDateFrom->Checked) {
        DecodeDate(DaData->Date, Year, Month, Day);
        str = DateToStr(DaData->Date) + " " + TimeToStr(DaOra->Time);
        filter = filter + " and generata >= '" + FormatDateTime(MainForm->format_data_ora_datatbase, StrToDateTime(str)) + "'";
    }

    if (FilterOnDateTo->Checked) {
        DecodeDate(AData->Date, Year, Month, Day);
        str = DateToStr(AData->Date) + " " + TimeToStr(AOra->Time);
        filter = filter + " and generata <= '" + FormatDateTime(dmDB->format_data_ora_database, StrToDateTime(str)) + "'";
    }
    filter = "Select * from CentroMissioni_view" + filter;

    filter = filter + " order by generata";
    Query1->SQL->Append(filter);
    Query1->Open();
    Tot->Caption = IntToStr(Query1->RecordCount);
    Query1->Last();
    StringaExcel = filter;

    try {
        ADOQuery1->Close();

    }
    catch (...) {
        dmDB->LogMsg("Eccezione su calcolo tempistiche missioni 2");
    }

}

// ---------------------------------------------------------------------------
void __fastcall TfCentroMissioni::BitBtn2Click(TObject *Sender)
{
    int res;
    if (Application->MessageBox(L"Vuoi storicizzare la tabella?", L"Please confirm", MB_YESNO) == IDYES) {
        res = dmExtraFunction->StoricizzaTabelle("CentroMissioni", "History_CentroMissioni", "fine", "MONTH", 3);
        if (res > 0)
            Application->MessageBox(L"Storicizzato", L"Ok", MB_OK);
        else
            Application->MessageBox(L"Problemi di storicizzazione", L"Read log", MB_OK);
        FormActivate(Sender);
    }
}
// ---------------------------------------------------------------------------

void __fastcall TfCentroMissioni::btDelRigaClick(TObject *Sender) {
    AnsiString sqlstring;
    int id, posprel, posdep;
    // cancello preontazioni
    id = Query1->FieldByName("ID")->AsInteger;
    posprel = Query1->FieldByName("posprel")->AsInteger;
    posdep = Query1->FieldByName("posdep")->AsInteger;
    // cancello missione
    try {
        QueryDel->Close();
        QueryDel->SQL->Clear();
        sqlstring = "delete from centromissioni where ID=" + Query1->FieldByName("ID")->AsString;
        QueryDel->SQL->Append(sqlstring);
        QueryDel->ExecSQL();
        dmDB->LogMsg("Eliminato CM ID  " + IntToStr(id) );
        QueryDel->Close();
        dmDB->SettaPosSelezionata(posprel, 0, 0);
        dmDB->SettaPosSelezionata(posdep, 0, 0);
        FormActivate(Sender);
    }
    catch (...) {
    }

}
// ---------------------------------------------------------------------------

void __fastcall TfCentroMissioni::btEsportaClick(TObject *Sender)
{
    if (Tot->Caption.ToIntDef(0) > 0)
        dmExtraFunction->EsportaExcel(StringaExcel, "CentroMissioni");
}

// ---------------------------------------------------------------------------

void __fastcall TfCentroMissioni::BitBtn3Click(TObject *Sender)
{
    NMisCheckClick(this);
}
// ---------------------------------------------------------------------------
