// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "anagrafica_articoli.h"
// #include "ins_articoli.h"
#include "DataExchange.h"
#include "ExtraFunction.h"
// #include "DBClient.h"
#include "db.h"
#include "main.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAnagraficaArticoli *FormAnagraficaArticoli;

// ---------------------------------------------------------------------------
__fastcall TFormAnagraficaArticoli::TFormAnagraficaArticoli(TComponent* Owner)
    : TForm(Owner)
{
    insert_produzione = 0;
}
// ---------------------------------------------------------------------------

void __fastcall TFormAnagraficaArticoli::FormActivate(TObject *Sender)
{
    AnsiString filtro;
    bool filter = false;

    //
    Panel1->Caption = "Articoli";
    //

    ProductName = "Nessuno";
    if (insert_produzione) {
        BitBtn1->Caption = "Seleziona IdArticolo";
    }
    else {
        BitBtn1->Caption = "Inserisci Nuovo ";
    }
    BitBtn1->Enabled = !insert_produzione;
    BitBtn2->Enabled = !insert_produzione;
    BitBtn3->Enabled = !insert_produzione;
    btInsInPos->Enabled = insert_produzione;

    Filtra();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAnagraficaArticoli::DBGrid1DblClick(TObject *Sender)
{
    // modifico la cella selezionata
    BitBtn2Click(this);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAnagraficaArticoli::BitBtn4Click(TObject *Sender)
{
    Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAnagraficaArticoli::FormClose(TObject *Sender,
    TCloseAction &Action)
{
    /*
     ADOQuery1->Close();
     Edit1->Text = "";
     Edit2->Text = "";
     Edit3->Text = "";
     CheckBox1->Checked = false;
     Shape1->Brush->Color = clWhite;
     dmDB->aggiorna_tab_tipologia_locale = 1;
     insert_produzione = 0;
     */
}

// ---------------------------------------------------------------------------
void __fastcall TFormAnagraficaArticoli::BitBtn3Click(TObject *Sender)
{
    AnsiString strsql;
    if ((ADOQuery1->RecNo) && (Application->MessageBox(L"Are you sure to delete this Item?", L"Confirm!!!", MB_YESNO) == IDYES)) {
        QueryDel->Close();
        QueryDel->SQL->Clear();
        strsql = "delete from Articoli where IdArticolo = " + ADOQuery1->FieldByName("IdArticolo")->AsString;
        QueryDel->SQL->Append(strsql);
        QueryDel->ExecSQL();
        QueryDel->Close();
        dmDB->LogMsg(strsql);
        FormActivate(Sender);
    }

}

// ---------------------------------------------------------------------------
void __fastcall TFormAnagraficaArticoli::FormDeactivate(TObject *Sender)
{
    /*
     if (!FormInsertArticles->Active) {
     artsel = 0;
     Close();
     }
     */
    Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAnagraficaArticoli::DBGrid1DrawColumnCell(TObject *Sender,
    const TRect &Rect, int DataCol, TColumn *Column,
    TGridDrawState State)
{
    int color;
    AnsiString code;
    if ((Column->FieldName == "Colore") &&
        (Column->Field->AsInteger > 0)) {
        code = IntToStr(Column->Field->AsInteger);
        color = Column->Field->AsInteger;
        DBGrid1->Canvas->Brush->Color = color;
        DBGrid1->Canvas->Font->Color = color;
        DBGrid1->Canvas->TextRect(Rect, Rect.left + 2, Rect.top + 2, code);
    }

}

// ---------------------------------------------------------------------------

void __fastcall TFormAnagraficaArticoli::CheckBox1Click(TObject *Sender)
{
    Filtra();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAnagraficaArticoli::Edit1Change(TObject *Sender)
{
    FormActivate(this);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAnagraficaArticoli::BitBtn1Click(TObject *Sender)
{
    if (insert_produzione) {
        if ((ADOQuery1->RecNo) && (ADOQuery1->FieldByName("IdArticolo")->AsInteger > 0)) {
            ProductName = ADOQuery1->FieldByName("Nome")->AsString;
            // CodiceProdotto = ADOQuery1->FieldByName("Codice")->AsString;
            IdArticolo = ADOQuery1->FieldByName("IdArticolo")->AsInteger;
            Close();
        }
    }
    else {
        /*
         FormInsertArticles->insert = true;
         FormInsertArticles->EdIdArticles->Text = IntToStr(dmDB->CreaIdArticoloAnagrafica());
         FormInsertArticles->EdName->Text = "";
         FormInsertArticles->EdDesc->Text = "";
         FormInsertArticles->edCodice->Text = "";
         FormInsertArticles->EdDesc->Text = "";
         FormInsertArticles->EdFase->Text = "0";
         FormInsertArticles->Show();
         */
    }
}
// ---------------------------------------------------------------------------

void __fastcall TFormAnagraficaArticoli::BitBtn2Click(TObject *Sender)
{
    if ((ADOQuery1->RecNo) && (ADOQuery1->FieldByName("IdArticolo")->AsInteger > 0)) {
        /*
         FormInsertArticles->insert = false;
         FormInsertArticles->EdIdArticles->Text = ADOQuery1->FieldByName("IdArticolo")->AsString.Trim();
         FormInsertArticles->EdName->Text = ADOQuery1->FieldByName("Nome")->AsString.Trim();
         // FormInsertArticles->edCodice->Text = ADOQuery1->FieldByName("Codice")->AsString;
         FormInsertArticles->EdDesc->Text = ADOQuery1->FieldByName("Descrizione")->AsString.Trim();
         FormInsertArticles->EdFase->Text = ADOQuery1->FieldByName("IDFase")->AsString.Trim();
         FormInsertArticles->Color->Brush->Color = ADOQuery1->FieldByName("Colore")->AsInteger;
         FormInsertArticles->Show();
         */
    }

}

// ---------------------------------------------------------------------------

void __fastcall TFormAnagraficaArticoli::ADOQuery1AfterScroll(TDataSet *DataSet)
{
    // per correggere il bug della grid che seleziona una riga sola e dopo non rimette la scrollbar
    DBGrid1->Width = DBGrid1->Width + 1;
    DBGrid1->Width = DBGrid1->Width - 1;
}
// ---------------------------------------------------------------------------

void __fastcall TFormAnagraficaArticoli::btInsInPosClick(TObject *Sender)
{
    int artUDCold;
    AnsiString messaggio;
    MainForm->trova_idarticolo =0;
    if (insert_produzione) {
        if ((ADOQuery1->RecNo) && (ADOQuery1->FieldByName("IdArticolo")->AsInteger > 0)) {
            MainForm->trova_idarticolo = ADOQuery1->FieldByName("IdArticolo")->AsInteger;
        }
    }
    Close();

}
// ---------------------------------------------------------------------------

void TFormAnagraficaArticoli::Filtra() {
    AnsiString filtro = ""; ;
    // serve questo flag ?
    filtro = "Select * from Articoli where (1=1) ";

    if (edDescrizione->Text != "") {
        filtro = filtro + " and Descrizione LIKE '%" + edDescrizione->Text + "%'";

    }
    if ((edEdIDArt->Text != "") && (edEdIDArt->Text.ToIntDef(0) > 0)) {
        filtro = filtro + " AND IDArticolo =" + edEdIDArt->Text;
    }

    try {
        if (edCodArt->Text != "") {
            filtro = filtro + " AND codart LIKE'%" + edCodArt->Text + "%'";
        }
    }
    catch (...) {}

    /* if (!filter)
     filtro = "Select * from Articles  "; */
    filtro = filtro + " order by IdArticolo";
    ADOQuery1->Close();
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add(filtro);
    ADOQuery1->Open();
    ADOQuery1->First();
    Tot->Caption = IntToStr(ADOQuery1->RecordCount);

}

void __fastcall TFormAnagraficaArticoli::edEdIDArtExit(TObject *Sender)
{
    Filtra();
}
// ---------------------------------------------------------------------------
