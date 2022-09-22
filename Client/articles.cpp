//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "articles.h"
#include "insarticles.h"
#include "db.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormArticles *FormArticles;
//---------------------------------------------------------------------------
__fastcall TFormArticles::TFormArticles(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormArticles::DBGrid1DblClick(TObject *Sender)
{
    // modifico la cella selezionata
    BitBtn2Click(this) ;    
}
//---------------------------------------------------------------------------
void __fastcall TFormArticles::BitBtn4Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormArticles::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    ADOQuery1->Close();
    Edit1->Text = "";
    Edit2->Text = "";
    Edit3->Text = "";
    cbTipoCarne->Text = "";
	CheckBox1->Checked = false ;
    Shape1->Brush->Color = clWhite ;
	dmDB->aggiorna_tab_tipologia_locale = 1 ;
}
//---------------------------------------------------------------------------
void __fastcall TFormArticles::BitBtn3Click(TObject *Sender)
{
AnsiString strsql ;
	if ((ADOQuery1->RecNo)&&(Application->MessageBox(L"Are you sure to delete this Item?", L"Confirm!!!", MB_YESNO) == IDYES)) {
        QueryDel->Close();
		QueryDel->SQL->Clear();
		strsql = "delete from TipologiaArticoli where IdTipologia = "+ADOQuery1->FieldByName("IdTipologia")->AsString ;
		QueryDel->SQL->Append( strsql);
        QueryDel->ExecSQL();
        QueryDel->Close();
		dmDB->LogMsg(strsql);
		FormActivate(Sender);
	}

}
//---------------------------------------------------------------------------
void __fastcall TFormArticles::FormDeactivate(TObject *Sender)
{
	if (!FormInsertArticles->Active)
		Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormArticles::DBGrid1DrawColumnCell(TObject *Sender,
      const TRect &Rect, int DataCol, TColumn *Column,
      TGridDrawState State)
{
int color ;
AnsiString code ;
	if ((Column->FieldName == "Colore") &&
		(Column->Field->AsInteger > 0)) {
        code = IntToStr(Column->Field->AsInteger);
		color = Column->Field->AsInteger;
        DBGrid1->Canvas->Brush->Color = color;
        DBGrid1->Canvas->Font->Color = color;
		DBGrid1->Canvas->TextRect(Rect, Rect.left + 2, Rect.top + 2, code);
	}

}
//---------------------------------------------------------------------------
void __fastcall TFormArticles::FormActivate(TObject *Sender)
{
AnsiString filtro ;
bool filter ;
    //
/*
	Panel1->Caption = MainForm->NomeArticolo +" List" ;
    FormArticles->Caption = MainForm->NomeArticolo +" List" ;
    Label1->Caption = MainForm->NomeArticolo +" Name :" ;
    BitBtn1->Caption = "Insert "+MainForm->NomeArticolo ;
    BitBtn2->Caption = "Modify "+MainForm->NomeArticolo ;
    BitBtn3->Caption = "Erase "+MainForm->NomeArticolo ;
    DBGrid1->Columns->Items[0]->Title->Caption = MainForm->NomeArticolo +" Id" ;
    DBGrid1->Columns->Items[1]->Title->Caption = MainForm->NomeArticolo +" N°" ;
*/

    filter = true ;
	filtro = "Select * from TipologiaArticoli where IdTipologia > 0 ";
	if (Edit1->Text != "") {
		if (filter)
			filtro += " AND ";
		filtro += "CodiceTipologia LIKE '%" + Edit1->Text+"%'";
		filter = true;
	}
	if (Edit2->Text != "") {
		if (filter)
			filtro += " AND ";
		filtro += "Descrizione LIKE '%" + Edit2->Text+"%'";
		filter = true;
	}
    try {
		if ((Edit3->Text != "")&&(Edit3->Text.ToIntDef(0) > 0)) {
            if (filter)
                filtro += " AND ";
			filtro += "IdTipologia =" + Edit3->Text;
            filter = true;
        }
    }catch(...){}
    try {
		if ((cbTipoCarne->Text != "")&&(cbTipoCarne->Items->IndexOf(cbTipoCarne->Text) > 0)) {
            if (filter)
                filtro += " AND ";
			filtro += "TipoCarne =" + IntToStr(cbTipoCarne->Items->IndexOf(cbTipoCarne->Text));
            filter = true;
        }
    }catch(...){}
	try {
		if (CheckBox1->Checked) {
			if (filter)
				filtro += " AND ";
			filtro += "Colore =" + IntToStr(Shape1->Brush->Color);
			filter = true;
		}
	}catch(...){}
/*    if (!filter)
		filtro = "Select * from Articles  ";  */
	filtro = filtro + " order by IdTipologia";
	ADOQuery1->Close();
	ADOQuery1->SQL->Clear() ;
	ADOQuery1->SQL->Add(filtro);
	ADOQuery1->Open();
	ADOQuery1->First();
	Tot->Caption = IntToStr(ADOQuery1->RecordCount);
}
//---------------------------------------------------------------------------
void __fastcall TFormArticles::CheckBox1Click(TObject *Sender)
{
    FormActivate(this);
        
}
//---------------------------------------------------------------------------
void __fastcall TFormArticles::Edit1Change(TObject *Sender)
{
    FormActivate(this);
}
//---------------------------------------------------------------------------
void __fastcall TFormArticles::Shape1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
int color ;
    ColorDialog1->Execute();
    color = ColorDialog1->Color ;
    Shape1->Brush->Color = color ;
}
//---------------------------------------------------------------------------
void __fastcall TFormArticles::BitBtn1Click(TObject *Sender)
{
    FormInsertArticles->insert = true ;
	FormInsertArticles->EdIdArticles->Text = IntToStr(dmDB->CreaIdTipologia());
	FormInsertArticles->EdName->Text = "";
    FormInsertArticles->EdDesc->Text = "";
	FormInsertArticles->Color->Brush->Color = clWhite;
	FormInsertArticles->TipoCarne->ItemIndex = 0;
	FormInsertArticles->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormArticles::BitBtn2Click(TObject *Sender)
{
	if ((ADOQuery1->RecNo)&&(ADOQuery1->FieldByName("IdTipologia")->AsInteger > 0)) {
        FormInsertArticles->insert = false ;
		FormInsertArticles->EdIdArticles->Text = ADOQuery1->FieldByName("IdTipologia")->AsString;
		FormInsertArticles->EdName->Text = ADOQuery1->FieldByName("CodiceTipologia")->AsString;
		FormInsertArticles->EdDesc->Text = ADOQuery1->FieldByName("Descrizione")->AsString;
		FormInsertArticles->Color->Brush->Color = ADOQuery1->FieldByName("Colore")->AsInteger ;
		FormInsertArticles->TipoCarne->ItemIndex = ADOQuery1->FieldByName("TipoCarne")->AsInteger ;
		FormInsertArticles->Show();
    }

}
//---------------------------------------------------------------------------

