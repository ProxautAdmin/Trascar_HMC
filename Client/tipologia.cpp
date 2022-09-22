// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "tipologia.h"
#include "db.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormTipologia *FormTipologia;

// ---------------------------------------------------------------------------
__fastcall TFormTipologia::TFormTipologia(TComponent* Owner)
    : TForm(Owner)
{
    insert_produzione = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormTipologia::BitBtn4Click(TObject *Sender)
{
    Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormTipologia::FormClose(TObject *Sender,
    TCloseAction &Action)
{
    ADOQuery1->Close();
    cbTipoCarne->Text = "";
	Shape1->Brush->Color = clWhite;
	dmDB->aggiorna_tab_tipologia_locale = 1 ;
}

// ---------------------------------------------------------------------------
void __fastcall TFormTipologia::BitBtn3Click(TObject *Sender)
{
    AnsiString strsql;
    if ((ADOQuery1->RecNo) && (Application->MessageBox(L"Are you sure to delete this Item?", L"Confirm!!!", MB_YESNO) == IDYES)) {
        QueryDel->Close();
        QueryDel->SQL->Clear();
        strsql = "delete from TabTipologiaCorsia where codtipo = " + ADOQuery1->FieldByName("codtipo")->AsString;
        QueryDel->SQL->Append(strsql);
        QueryDel->ExecSQL();
        QueryDel->Close();
        dmDB->LogMsg(strsql);
        FormActivate(Sender);
    }

}

// ---------------------------------------------------------------------------
void __fastcall TFormTipologia::FormDeactivate(TObject *Sender) {
    // Close();

}

// ---------------------------------------------------------------------------
void __fastcall TFormTipologia::DBGrid1DrawColumnCell(TObject *Sender,
    const TRect &Rect, int DataCol, TColumn *Column,
    TGridDrawState State)
{
    int color;
	AnsiString code;
	if (Column->FieldName == "Colore")  {
		code = IntToStr(Column->Field->AsInteger);
        color = Column->Field->AsInteger;
        DBGrid1->Canvas->Brush->Color = color;
        DBGrid1->Canvas->Font->Color = color;
        DBGrid1->Canvas->TextRect(Rect, Rect.left + 2, Rect.top + 2, code);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormTipologia::FormActivate(TObject *Sender) {
    AnsiString filtro;

    filtro = "Select * from TabTipologiaCorsia order by codtipo";

    ADOQuery1->Close();
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add(filtro);
    ADOQuery1->Open();
    ADOQuery1->First();
    Tot->Caption = IntToStr(ADOQuery1->RecordCount);

    BitBtn1->Visible = false;
    BitBtn2->Visible = false;
    BitBtn3->Visible = false;

}

// ---------------------------------------------------------------------------
void __fastcall TFormTipologia::CheckBox1Click(TObject *Sender) {
    FormActivate(this);

}

// ---------------------------------------------------------------------------
void __fastcall TFormTipologia::Edit1Change(TObject *Sender)
{
    FormActivate(this);
}

// ---------------------------------------------------------------------------
void __fastcall TFormTipologia::Shape1MouseUp(TObject *Sender,
    TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int color;
    ColorDialog1->Execute();
    color = ColorDialog1->Color;
    Shape1->Brush->Color = color;

}

// ---------------------------------------------------------------------------
void __fastcall TFormTipologia::BitBtn1Click(TObject *Sender)
{
    /*
     if (insert_produzione) {
     if ((ADOQuery1->RecNo) && (ADOQuery1->FieldByName("IdTipologia")->AsInteger > 0)) {
     TipoProdotto = ADOQuery1->FieldByName("CodiceTipologia")->AsString;
     Close();
     }
     }
     else {
     FormInsertArticles->insert = true;
     FormInsertArticles->EdIdArticles->Text = IntToStr(dmDB->CreaIdTipologia());
     FormInsertArticles->EdName->Text = "";
     FormInsertArticles->EdDesc->Text = "";
     FormInsertArticles->Color->Brush->Color = clWhite;
     FormInsertArticles->TipoCarne->ItemIndex = 0;
     FormInsertArticles->Show();
     }
     */
}
// ---------------------------------------------------------------------------

void __fastcall TFormTipologia::BitBtn2Click(TObject *Sender) {
    /*
     if ((ADOQuery1->RecNo) && (ADOQuery1->FieldByName("IdTipologia")->AsInteger > 0)) {
     FormInsertArticles->insert = false;
     FormInsertArticles->EdIdArticles->Text = ADOQuery1->FieldByName("IdTipologia")->AsString;
     FormInsertArticles->EdName->Text = ADOQuery1->FieldByName("CodiceTipologia")->AsString;
     FormInsertArticles->EdDesc->Text = ADOQuery1->FieldByName("Descrizione")->AsString;
     FormInsertArticles->Color->Brush->Color = ADOQuery1->FieldByName("Colore")->AsInteger;
     FormInsertArticles->TipoCarne->ItemIndex = ADOQuery1->FieldByName("TipoCarne")->AsInteger;
     FormInsertArticles->Show();
     }
     */
}

// ---------------------------------------------------------------------------

void __fastcall TFormTipologia::DBGrid1DblClick(TObject *Sender)
{
    AnsiString stringa;
    stringa = "Update TabTipologiaCorsia set colore=" +String(Shape1->Brush->Color) ;
    stringa = stringa + " where codtipo = " + ADOQuery1->FieldByName("codtipo")->AsString;

    try {
        ADOQuery1->Close();
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Append(stringa);
        ADOQuery1->ExecSQL();
        ADOQuery1->Close();
        dmDB->LogMsg("Update colore tabTipocorsia");
        // dmDB->AggiornaTabAnagrafica();
              FormActivate(this);
	}
        catch (...) {
		dmDB->LogMsg("Eccezzione su Update colore TipoCorsia");
    }
    }
    // ---------------------------------------------------------------------------

void __fastcall TFormTipologia::DBGrid1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
             if (Button == mbRight) {
       Shape1->Brush->Color = ADOQuery1->FieldByName("Colore")->AsInteger;
    // modifico la cella selezionata
    BitBtn2Click(this);
    }
}
//---------------------------------------------------------------------------

