//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "insarticles.h"
#include "db.h"
#include "tipologia.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFormInsertArticles *FormInsertArticles;
//---------------------------------------------------------------------------
__fastcall TFormInsertArticles::TFormInsertArticles(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertArticles::BitBtn2Click(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertArticles::BitBtn1Click(TObject *Sender)
{
AnsiString stringa ,ora1,ora2,ora3,timemax,timemin;
int ok ;
    //
		 if (insert) {
			stringa = "Insert Into TipologiaArticoli (IdTipologia,CodiceTipologia,Descrizione,Colore,TipoCarne)";
			stringa = stringa + " VALUES ("+EdIdArticles->Text+",'"+EdName->Text+"','"+EdDesc->Text+"',"+IntToStr(Color->Brush->Color)+","+IntToStr(TipoCarne->ItemIndex)+")";
		}else {
			stringa = "Update TipologiaArticoli set CodiceTipologia = '"+EdName->Text+"' ,Descrizione = '"+EdDesc->Text+"'";
			stringa = stringa + " ,Colore = "+IntToStr(Color->Brush->Color) ;
			stringa = stringa + " ,TipoCarne = "+IntToStr(TipoCarne->ItemIndex) ;
			stringa = stringa + " where IdTipologia = " +EdIdArticles->Text ;
		}
		try {
			Query->Close();
			Query->SQL->Clear();
			Query->SQL->Append(stringa) ;
			Query->ExecSQL();
			Query->Close();
			dmDB->LogMsg(stringa);
			dmDB->aggiorna_tab_tipologia_locale = 1 ;
			Close();
		}catch(...){
			dmDB->LogMsg("Eccezione insert/update tipologia");
			Query->Close();
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertArticles::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    insert = false ;
    FormTipologia->FormActivate(this);
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertArticles::FormActivate(TObject *Sender)
{
    //
/*    FormInsertArticles->Caption = "Insert/Modify "+MainForm->NomeArticolo;
    Panel1->Caption = "Insert/Modify "+MainForm->NomeArticolo;
    Label1->Caption = MainForm->NomeArticolo + " Id";
    Label2->Caption = MainForm->NomeArticolo + " N°";
    Label9->Caption = "Color "+MainForm->NomeTelaio + " Full";
	Recipe->Value = StrToInt(EdIdArticles->Text);            */
	if (EdIdArticles->Text.ToIntDef(0) < 3) {
		//tipologia totem vuoti gli cambio solo il colore
		EdName->Enabled = false ;
	} else
		EdName->Enabled = true ;
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertArticles::ColorMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
int color ;
    ColorDialog1->Execute();
    color = ColorDialog1->Color ;
    Color->Brush->Color = color ;

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

