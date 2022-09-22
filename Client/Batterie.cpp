// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Batterie.h"
#include "db.h"
#include "main.h"
#include "ExtraFunction.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma link "cspin"
#pragma resource "*.dfm"
TfBatterie *fBatterie;

// ---------------------------------------------------------------------------
__fastcall TfBatterie::TfBatterie(TComponent* Owner)
    : TMDIChild(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TfBatterie::BitBtn1Click(TObject *Sender)
{
    if (Application->MessageBox(L"Are you sure you want to empty the Historic ?", L"Confirm!!!", MB_YESNO) == IDYES) {
        Query1->Close();
        Query1->SQL->Clear();
        Query1->SQL->Append("delete from Batteria ");
        Query1->ExecSQL();
        Query1->Close();
        FormActivate(Sender);
    }

}

// ---------------------------------------------------------------------------
void __fastcall TfBatterie::FormActivate(TObject *Sender)
{
    IndexListIterator ind;
    int i;

    AData->Date = Date();
    FilterOnDateTo->Checked = false;
    cbDestination->Checked = false;
    cbBatteryState->Checked = false;
    CheckBox3->Checked = false;
    CheckBox4->Checked = false;
    CheckBox5->Checked = false;

    BitBtn1->Enabled = dmDB->pwdlevel;
    edBatteryState->Text = "";
    CSpinEdit3->MaxValue = NAGV;

    NMisCheckClick(this);

}
// ---------------------------------------------------------------------------

void __fastcall TfBatterie::NMisCheckClick(TObject *Sender)
{
    Word Year, Month, Day;
    AnsiString str = "", filter = " where ", filtrotempo, filtronpallet, filtronpalletvuoti;
    bool filtro = false;
    int secondi_totali, secondi_missione, filtropesototale;
    float result, result_mis;
    Query1->Close();
    Query1->SQL->Clear();
    if (NMisCheck->Checked) {
        filter = filter + "Idmissione =" + NMis->Value;
        filtro = true;
    }
    if (PrelCheck->Checked) {
        if (filtro)
            filter = filter + " AND ";
        filter = filter + "Pos >=" + Prel->Value;
        filter = filter + " AND Pos <=" + Dep->Value;
        filtro = true;
    }
    if (DepCheck->Checked) {
        if (filtro)
            filter = filter + " AND ";
        filter = filter + "PosPrecedente >=" + cLastPosfrom->Value;
        filter = filter + " AND PosPrecedente <=" + cLastPosTo->Value;
        filtro = true;
    }
    if ((cbBatteryState->Checked) && (edBatteryState->Text != "")) {
        if (filtro)
            filter = filter + " AND ";
        filter = filter + "statobatteria =" + edBatteryState->Text.ToIntDef(0);
        filtro = true;
    }
    if ((cbDestination->Checked) && (edDestination->Text != "")) {
        if (filtro)
            filter = filter + " AND ";
        filter = filter + "Destination =" + edDestination->Text.ToIntDef(0);
        filtro = true;
    }
    if (CheckBox4->Checked) {
        if (filtro)
            filter = filter + " AND ";
        filter = filter + "Agv =" + CSpinEdit3->Value;
        filtro = true;
    }
    if ((CheckBox5->Checked) && (Edit1->Text.ToIntDef(0) > 0)) {
        if (filtro)
            filter = filter + " AND ";
        filter = filter + "id =" + Edit1->Text.ToIntDef(0);
        filtro = true;
    }

    if (CheckBox3->Checked) {
        if (filtro)
            filter = filter + " AND ";
        filter = filter + "statoAgv =" + edAGVState->Text.ToIntDef(0);
        filtro = true;
    }
    if (FilterOnDateTo->Checked) {
        DecodeDate(AData->Date, Year, Month, Day);
        // str = IntToStr(Day)+"."+IntToStr(Month)+"."+IntToStr(Year); //il separatore per le query sulle tabelle di BDE è il . e non la barra /
        str = DateToStr(AData->Date) + " " + TimeToStr(AOra->Time);
        if (filtro)
            filter = filter + " AND ";
        // filter =filter + "generata <= '" + str +" "+ TimeToStr(AOra->Time) + "'";
        filter = filter + "dataora <= '" + FormatDateTime(dmDB->format_data_ora_database, StrToDateTime(str)) + "'";
        filtro = true;
    }

    if (!filtro) {
        filter = "Select * from Batteria";
        filtronpalletvuoti = "SELECT COUNT(*) AS N FROM Batteria ";
    }
    else {
        filtronpalletvuoti = "SELECT COUNT(*) AS N FROM batteria ";
        filter = "Select * from batteria" + filter;
    }
    filter = filter + " order by id";
    Query1->SQL->Append(filter);
    Query1->Open();
    Tot->Caption = IntToStr(Query1->RecordCount);
    Query1->Last();
    Label12->Caption = "0";
    Label15->Caption = "0";
    /*
     QueryVis->Close();
     QueryVis->SQL->Clear();
     QueryVis->SQL->Append("Select * from Tempreport order by Int1, int2");
     QueryVis->Open();
     QueryVis->First();
     */

}

// ---------------------------------------------------------------------------
void __fastcall TfBatterie::BitBtn2Click(TObject *Sender) {
    int res;
    if (Application->MessageBox(L"Do you want to historicise table?", L"Please confirm", MB_YESNO) == IDYES) {
        res = dmExtraFunction->StoricizzaTabelle("batteria", "History_Batteria", "dataora", "MONTH", 3);
        if (res > 0)
            Application->MessageBox(L"Historicised", L"Ok", MB_OK);
        else
            Application->MessageBox(L"Historicizing problems", L"Read log", MB_OK);
        FormActivate(Sender);
    }
}
// ---------------------------------------------------------------------------

void TfBatterie::Report() {
    TADOQuery *ADOQuery;
    AnsiString strbatteria, strreport;
    int res, reccount, reccur = 1; ;
    // ATTENZIONE! GESTIONE UTENTE
    // cancello temporaneo
    strreport = "delete tempreport";
    QueryRep->Close();
    QueryRep->SQL->Clear();
    QueryRep->SQL->Text = strreport;
    res = QueryRep->ExecSQL();
    // seleziono batteria
    strbatteria = "SELECT Pos, PosPrecedente, AGV, "
        "SUM (consumoattuale -consumonodoprec) as TotaleConsumoNodo,"
        "count(pos) as Cont, "
        "AVG (consumoattuale -consumonodoprec) as MediaConsumoNodo, "
        "min(consumoattuale -consumonodoprec) as ConsumoMin, "
        "max(consumoattuale -consumonodoprec) as ConsumoMax "
        "FROM batteria "
        "GROUP BY agv, pos, posprecedente "
        "order by pos, posprecedente, agv";
    QueryBat->Close();
    QueryBat->SQL->Clear();
    QueryBat->SQL->Text = strbatteria;
    QueryBat->Open();
    QueryBat->First();
    reccount = QueryBat->RecordCount;
    while (!QueryBat->Eof) {
        strreport = "INSERT INTO Tempreport (Int1 ,Int2, Int3, Dec1, Int4, Dec2, Dec3, Dec4)"
            " VALUES (" +
            QueryBat->FieldByName("pos")->AsString + ", " +
            QueryBat->FieldByName("posprecedente")->AsString + ", " +
            QueryBat->FieldByName("AGV")->AsString + ", " +
            "REPLACE('" + QueryBat->FieldByName("TotaleConsumoNodo")->AsString + "', ',' , '.')," +
            QueryBat->FieldByName("Cont")->AsString + ", " +
            "REPLACE('" + QueryBat->FieldByName("MediaConsumoNodo")->AsString + "', ',' , '.')," +
            "REPLACE('" + QueryBat->FieldByName("ConsumoMin")->AsString + "', ',' , '.')," +
            "REPLACE('" + QueryBat->FieldByName("ConsumoMax")->AsString + "', ',' , '.')" +
            ")";

        /*
         strreport = "Select Int1 from Tempreport where Int1= " + QueryBat->FieldByName("pos")->AsString + " and Int2= " + QueryBat->FieldByName("posprecedente")->AsString;
         QueryRep->Close();
         QueryRep->SQL->Clear();
         QueryRep->SQL->Append(strreport);
         QueryRep->Open();

         if (QueryRep->RecordCount == 0) {
         strreport = "INSERT INTO Tempreport (Int1 ,Int2,Dec1, Int3)"
         " VALUES (" +
         QueryBat->FieldByName("pos")->AsString + "," +
         QueryBat->FieldByName("posprecedente")->AsString + "," +
         String(printf("%.5,f", QueryBat->FieldByName("consumoattuale")->AsFloat - QueryBat->FieldByName("consumonodoprec")->AsFloat)) +
         ",1" + ")";

         }
         else {
         strreport = " UPDATE Tempreport SET "
         " Int3 = Int3 + 1, Dec1 = dec1 + " + String(printf("%.5,f", QueryBat->FieldByName("consumoattuale")->AsFloat - QueryBat->FieldByName("consumonodoprec")->AsFloat)) +
         " where Int1 = " + QueryBat->FieldByName("pos")->AsString + " and Int2 = " + QueryBat->FieldByName("posprecedente")->AsString;

         }
         */
        QueryRep->Close();
        QueryRep->SQL->Clear();
        QueryRep->SQL->Text = strreport;
        res = QueryRep->ExecSQL();
        QueryBat->Next();
        lbTot->Caption = String(reccur) + " / " + String(reccount);
        reccur++;
    }

    QueryVis->Close();
    QueryVis->SQL->Clear();
    strreport = "Select Int1 as NodoDa, Int2 as NodoA, Int3 as AGV, Dec1 as TotaleConsumoNodo, Int4 as Cont, Dec2 as MediaConsumoNodo, Dec3 as ConsumoMin, Dec4 as ConsumoMax from Tempreport order by Int1, int2, AGV";
    QueryVis->SQL->Append(strreport);
    QueryVis->Open();
    QueryVis->First();

    // esporta in excel
    dmExtraFunction->EsportaExcel(strreport, "StatNodi");

}

void __fastcall TfBatterie::btStatClick(TObject * Sender) {
    Report();
}

// ---------------------------------------------------------------------------
