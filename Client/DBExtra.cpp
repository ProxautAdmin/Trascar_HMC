// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DBExtra.h"
#include "db.h"
#include "main.h"
#include "MSG.h"
#include "ins_MssioniWMS.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma link "cspin"
#pragma resource "*.dfm"
TfDBExtra *fDBExtra;

// ---------------------------------------------------------------------------
__fastcall TfDBExtra::TfDBExtra(TComponent* Owner)
    : TMDIChild(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TfDBExtra::CheckBox1Click(TObject *Sender) {
    //
    Word Year, Month, Day;
    AnsiString str = "";
    Query1->Close();
    Query1->SQL->Clear();
    AnsiString filtro;
    filtro = "Select * from MissioniWMS_view where (1=1) ";

    if (edID->Text.Trim() != "") {
        filtro = filtro + " AND ID = " + edID->Text + "";
    }
    if (edDaSME->Text.Trim() != "") {
        filtro = filtro + " and Prelievo = '" + edDaSME->Text + "'";
    }
    if (edPos->Text.Trim() != "") {
        filtro = filtro + " AND Deposito =" + edPos->Text;
    }
    if (edPriorita->Text.Trim() != "") {
        filtro = filtro + " AND priorita = " + edPriorita->Text;
    }
    if (edCodUDC->Text.Trim() != "") {
        filtro = filtro + " AND IDUDC = '" + edCodUDC->Text + "'";
    }
    if (edCodOperatore->Text.Trim() != "") {
        filtro = filtro + " AND TipoUDC = " + edCodOperatore->Text + " ";
    }

    if (cbMsnAttive->Checked) {
        filtro = filtro + " and Stato <=4 ";
    }

    if (FilterOnDateFrom->Checked) {
        DecodeDate(DaData->Date, Year, Month, Day);
        str = IntToStr(Day) + "." + IntToStr(Month) + "." + IntToStr(Year); // il separatore per le query sulle tabelle di BDE è il . e non la barra /
        filtro = filtro + " and dataora >= '" + str + " " + TimeToStr(DaOra->Time) + "'";
    }
    if (FilterOnDateTo->Checked) {
        DecodeDate(AData->Date, Year, Month, Day);
        str = IntToStr(Day) + "." + IntToStr(Month) + "." + IntToStr(Year); // il separatore per le query sulle tabelle di BDE è il . e non la barra /
        filtro = filtro + "and dataora <= '" + str + " " + TimeToStr(AOra->Time) + "'";
    }

    filtro = filtro + " ORDER BY ID";

    Query1->SQL->Add(filtro);
    Query1->Open();
    Query1->Last();
    Tot->Caption = IntToStr(Query1->RecordCount);
    Ordinamento.clear();
    order Ordin;
    Ordin.NomeCampo = "dataora";
    Ordin.descendig = false;
    Ordinamento.push_back(Ordin);
    for (int i = 0; i < DBGrid1->Columns->Count; i++) {
        if (DBGrid1->Columns->Items[i]->FieldName == "dataora")
            DBGrid1->Columns->Items[i]->Title->Color = clMenu;
        else
            DBGrid1->Columns->Items[i]->Title->Color = clBtnFace;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TfDBExtra::FormActivate(TObject *Sender) {
    BitBtnDel->Enabled = false; // dmDB->pwdlevel;
    BitBtnVar->Enabled = dmDB->pwdlevel;
    BitBtnIns->Enabled = dmDB->pwdlevel;
    CheckBox1Click(this);
}

// ---------------------------------------------------------------------------
void __fastcall TfDBExtra::DBGrid1TitleClick(TColumn *Column) {
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

void __fastcall TfDBExtra::DBGrid1DblClick(TObject *Sender) {

    if ((Query1->Active) && (Query1->RecNo) && (Query1->FieldByName("ID")->AsInteger > 0)) {
        BitBtnVarClick(this);
    }

}
// ---------------------------------------------------------------------------

void __fastcall TfDBExtra::FormCreate(TObject *Sender)
{
    AnsiString oradel;
    /*
     DaData->Date = Date();
     AData->Date = Date();
     FilterOnPos->Checked = false;
     FilterOnDateFrom->Checked = false;
     FilterOnDateTo->Checked = false;
     CheckBox2->Checked = false;
     RadioGroup1->ItemIndex = 0 ;
     Allarme->Text = "";
     */

}
// ---------------------------------------------------------------------------

void __fastcall TfDBExtra::BitBtnDelClick(TObject *Sender)
{
    AnsiString sqlstring;
    if (Query1->FieldByName("id")->AsInteger > 0) {
        if (Application->MessageBox(L"Sei sicuro di voler cancellare il record selezionato ?", L"Conferma", MB_YESNO) == IDYES) {
            QueryDel->Close();
            QueryDel->SQL->Clear();
            sqlstring = "delete from MissioniSme where id=" + Query1->FieldByName("id")->AsString;
            QueryDel->SQL->Append(sqlstring);
            QueryDel->ExecSQL();
            QueryDel->Close();
            FormActivate(Sender);
        }
    }
}

// ---------------------------------------------------------------------------
void __fastcall TfDBExtra::BitBtnInsClick(TObject *Sender) {

    FIns_MissioniWMS->insert = true;
    FIns_MissioniWMS->edID->Text = 0;
    FIns_MissioniWMS->edDataOraCreazioneWMS->Text = "";
    FIns_MissioniWMS->edPosizionePrelievo->Text = "";
    FIns_MissioniWMS->edPosizioneDeposito->Text = "";
    FIns_MissioniWMS->edCorsiaDeposito->Text = "";
    FIns_MissioniWMS->edIDUDC->Text = "0";
    FIns_MissioniWMS->edPesoUDC->Text = "0";
    FIns_MissioniWMS->edTipoUDC->Text = "0";
    FIns_MissioniWMS->edTipoMissione->Text = "0";
    FIns_MissioniWMS->edPriorita->Text = "0";
    FIns_MissioniWMS->edIDAgv->Text = "0";
    FIns_MissioniWMS->edStatoMissioneWMS->Text = "0";
    FIns_MissioniWMS->edStatoMissioneProxaut->Text = "0";
    FIns_MissioniWMS->ShowModal();
    FormActivate(Sender);

}
// ---------------------------------------------------------------------------

void __fastcall TfDBExtra::BitBtnVarClick(TObject *Sender) {

    if (Query1->FieldByName("id")->AsInteger > 0) {
        FIns_MissioniWMS->insert = false;
        FIns_MissioniWMS->edID->Text = Query1->FieldByName("id")->AsString;
        FIns_MissioniWMS->edDataOraCreazioneWMS->Text = Query1->FieldByName("DataOraCreazioneWMS")->AsString;
        FIns_MissioniWMS->edPosizionePrelievo->Text = Query1->FieldByName("PosizionePrelievo")->AsString;
        FIns_MissioniWMS->edPosizioneDeposito->Text = Query1->FieldByName("PosizioneDeposito")->AsString;
        FIns_MissioniWMS->edCorsiaDeposito->Text = Query1->FieldByName("CorsiaDeposito")->AsString;
        FIns_MissioniWMS->edIDUDC->Text = Query1->FieldByName("IDUDC")->AsString;
        FIns_MissioniWMS->edTipoMissione->Text = Query1->FieldByName("TipoMissione")->AsString;
        FIns_MissioniWMS->edPriorita->Text = Query1->FieldByName("Priorita")->AsString;
        FIns_MissioniWMS->edIDAgv->Text = Query1->FieldByName("IDAgv")->AsString;
        FIns_MissioniWMS->edStatoMissioneWMS->Text = Query1->FieldByName("StatoMissioneWMS")->AsString;
        FIns_MissioniWMS->edStatoMissioneProxaut->Text = Query1->FieldByName("StatoMissioneProxaut")->AsString;
        FIns_MissioniWMS->ShowModal();
        FormActivate(Sender);
    }

}

// ---------------------------------------------------------------------------
void __fastcall TfDBExtra::btRefreshClick(TObject *Sender)
{
    FormActivate(Sender);
}
// ---------------------------------------------------------------------------
