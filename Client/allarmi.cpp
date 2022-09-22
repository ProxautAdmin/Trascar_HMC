// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "allarmi.h"
#include "db.h"
#include "main.h"
#include "MSG.h"
#include "ExtraFunction.h"
#include "missioni.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma link "cspin"
#pragma resource "*.dfm"
TfAllarmi *fAllarmi;

// ---------------------------------------------------------------------------
__fastcall TfAllarmi::TfAllarmi(TComponent* Owner)
    : TMDIChild(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TfAllarmi::CheckBox1Click(TObject *Sender)
{
    //
    Word Year, Month, Day;
    AnsiString str = "";
    Query1->Close();
    Query1->SQL->Clear();
    AnsiString filtro;
    filtro = "Select * from Allarmi where (1=1) ";

    if (Allarme->Text != "") {
        filtro = filtro + " and Nallarme = '" + Allarme->Text + "'";
    }
    if (FilterOnPos->Checked) {
        filtro = filtro + " and Posizione =" + Pos->Value;
    }
    if (CheckBox2->Checked) {
        filtro = filtro + " and Destinazione =" + CSpinEdit2->Value;
    }
    if (CheckBox4->Checked) {
        filtro = filtro + " and agv ='" + CSpinEdit1->Value + "'";
    }
    if (RadioGroup1->ItemIndex > 0) {
        filtro = filtro + " and ripristinato =" + IntToStr(RadioGroup1->ItemIndex - 1);
    }
    if (Edit1->Text != "") {
        filtro = filtro + " and Descrizione LIKE '%" + Edit1->Text + "%'";
    }
    if (FilterOnDateFrom->Checked) {
        DecodeDate(DaData->Date, Year, Month, Day);
        str = IntToStr(Day) + "." + IntToStr(Month) + "." + IntToStr(Year); // il separatore per le query sulle tabelle di BDE è il . e non la barra /
        filtro = filtro + " and dataora >= '" + str + " " + TimeToStr(DaOra->Time) + "'"; // filtro = filtro + "DataOra >= convert(datetime,'" + FormatDateTime(MainForm->format_data_ora_datatbase, StrToDateTime(str)) + "', 103)";
    }
    if (FilterOnDateTo->Checked) {
        DecodeDate(AData->Date, Year, Month, Day);
        str = IntToStr(Day) + "." + IntToStr(Month) + "." + IntToStr(Year); // il separatore per le query sulle tabelle di BDE è il . e non la barra /
        filtro = filtro + " and dataora <= '" + str + " " + TimeToStr(AOra->Time) + "'";
    }
    filtro = filtro + " order by DataOra";
    Query1->SQL->Add(filtro);
    Query1->Open();
    Query1->Last();
    StringaExcel = filtro;
    Tot->Caption = IntToStr(Query1->RecordCount);
    Ordinamento.clear();
    order Ordin;
    Ordin.NomeCampo = "DataOra";
    Ordin.descendig = false;
    Ordinamento.push_back(Ordin);
    for (int i = 0; i < DBGrid1->Columns->Count; i++) {
        if (DBGrid1->Columns->Items[i]->FieldName == "DataOra")
            DBGrid1->Columns->Items[i]->Title->Color = clMenu;
        else
            DBGrid1->Columns->Items[i]->Title->Color = clBtnFace;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TfAllarmi::FormActivate(TObject *Sender) {

    if (dmDB->ZonaString == "Z") {
        BitBtn1->Visible = false;
        btStorico->Visible = false;
    }
    else {
        BitBtn1->Visible = dmDB->pwdlevel;
    }
    CheckBox1Click(this);

}

// ---------------------------------------------------------------------------
void __fastcall TfAllarmi::DBGrid1TitleClick(TColumn *Column)
{
    order Ricerca;
    std::vector<order>::iterator i;
    Query1->Close();
    Query1->SQL->Strings[0] = Query1->SQL->Strings[0].SetLength(Query1->SQL->Strings[0].Pos("order by") - 1);
    Ricerca.NomeCampo = Column->FieldName;
    for (i = Ordinamento.begin(); i != Ordinamento.end(); i++) {
        if (i->NomeCampo == Ricerca.NomeCampo)
            break;
    }
    if (i != Ordinamento.end()) {
        Ricerca.descendig = !i->descendig;
        Ordinamento.erase(i);
        Ordinamento.insert(Ordinamento.begin(), Ricerca); // metto davanti così l'ordinamento principale è quello
        if (Ricerca.descendig)
            Column->Title->Color = clGrayText;
        else
            Column->Title->Color = clMenu;
    }
    else {
        Column->Title->Color = clMenu;
        Ricerca.descendig = false;
        Ordinamento.insert(Ordinamento.begin(), Ricerca); // metto davanti così l'ordinamento principale è quello
    }
    Query1->SQL->Strings[0] = Query1->SQL->Strings[0] + "order by ";
    for (i = Ordinamento.begin(); i != Ordinamento.end(); i++) {
        if (i != Ordinamento.begin())
            Query1->SQL->Strings[0] = Query1->SQL->Strings[0] + " , ";
        Query1->SQL->Strings[0] = Query1->SQL->Strings[0] + i->NomeCampo;
        if (i->descendig)
            Query1->SQL->Strings[0] = Query1->SQL->Strings[0] + " desc";
    }
    Query1->Open();

}

// ---------------------------------------------------------------------------
void __fastcall TfAllarmi::BitBtn1Click(TObject *Sender)
{
    if (Application->MessageBox(L"Sicuro di voler cancellare lo storico?", L"Conferma!!!", MB_YESNO) == IDYES) {
        Query1->Close();
        Query1->SQL->Clear();
        Query1->SQL->Append("delete from Allarmi ");
        Query1->ExecSQL();
        Query1->Close();
        FormActivate(Sender);
    }

}
// ---------------------------------------------------------------------------

void __fastcall TfAllarmi::DBGrid1DblClick(TObject *Sender)
{
    if ((Query1->Active) && (Query1->RecNo) && (Query1->FieldByName("Descrizione")->AsString != "")) {
        if (Query1->FieldByName("Missione")->AsInteger > 0) {
            MainForm->recnum = Query1->FieldByName("Missione")->AsInteger;
            MainForm->bMissioni->Click();
        }
        else {
            MSGForm->Label1->Caption = Query1->FieldByName("Descrizione")->AsString;
            MSGForm->Show();
        }
    }

}
// ---------------------------------------------------------------------------

void __fastcall TfAllarmi::FormCreate(TObject *Sender)
{
    AnsiString oradel;

    DaData->Date = Date();
    AData->Date = Date();
    FilterOnPos->Checked = false;
    FilterOnDateFrom->Checked = false;
    FilterOnDateTo->Checked = false;
    CheckBox2->Checked = false;
    RadioGroup1->ItemIndex = 0;
    Allarme->Text = "";

}
// ---------------------------------------------------------------------------

void __fastcall TfAllarmi::btEsportaClick(TObject *Sender)
{
    if (Tot->Caption.ToIntDef(0) > 0)
        dmExtraFunction->EsportaExcel(StringaExcel, "Allarmi");
}
// ---------------------------------------------------------------------------

void __fastcall TfAllarmi::btStoricoClick(TObject *Sender)
{
    int res;
    if (Application->MessageBox(L"Vuoi storicizzare?", L"Conferma", MB_YESNO) == IDYES) {
        res = dmExtraFunction->StoricizzaTabelle("Allarmi", "History_Allarmi", "dataora", "MONTH", 1);
        if (res > 0)
            Application->MessageBox(L"Storicizzazione Allarmi eseguita", L"Ok", MB_OK);
        else
            Application->MessageBox(L"Problema storicizzazione Allarmi", L"Leggi log", MB_OK);
        FormActivate(Sender);
    }
}
// ---------------------------------------------------------------------------
