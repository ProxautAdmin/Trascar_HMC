//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "anagrafica_fornitori.h"
#include "ins_anagrafica_articoli.h"
#include "db.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAnagraficaFornitori *FormAnagraficaFornitori;
//---------------------------------------------------------------------------
__fastcall TFormAnagraficaFornitori::TFormAnagraficaFornitori(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormAnagraficaFornitori::DBGrid1DblClick(TObject *Sender)
{
    // modifico la cella selezionata
    BitBtn2Click(this) ;    
}
//---------------------------------------------------------------------------
void __fastcall TFormAnagraficaFornitori::BitBtn4Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormAnagraficaFornitori::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    ADOQuery1->Close();
    Edit1->Text = "";
    Edit2->Text = "";
    Edit3->Text = "";
    cbTipoCarne->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TFormAnagraficaFornitori::BitBtn3Click(TObject *Sender)
{
AnsiString strsql ;
	if ((ADOQuery1->RecNo)&&(Application->MessageBox(L"Are you sure to delete this Item?", L"Confirm!!!", MB_YESNO) == IDYES)) {
        QueryDel->Close();
		QueryDel->SQL->Clear();
		strsql = "delete from AnagraficaFornitori where IdFornitore = "+ADOQuery1->FieldByName("IdFornitore")->AsString ;
		QueryDel->SQL->Append( strsql);
        QueryDel->ExecSQL();
        QueryDel->Close();
		dmDB->LogMsg(strsql);
		FormActivate(Sender);
	}

}
//---------------------------------------------------------------------------
void __fastcall TFormAnagraficaFornitori::FormDeactivate(TObject *Sender)
{
	if (!FormInsAnagraficaArticoli->Active)
		Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormAnagraficaFornitori::FormActivate(TObject *Sender)
{
AnsiString filtro ;
bool filter ;
    //

    filter = true ;
	filtro = "Select * from AnagraficaFornitori where IdFornitore > 0 ";
	if (Edit1->Text != "") {
		if (filter)
			filtro  = filtro + " AND ";
		filtro  = filtro + "CodiceLIKE '%" + Edit1->Text+"%'";
		filter = true;
	}
	if (Edit2->Text != "") {
		if (filter)
			filtro  = filtro + " AND ";
		filtro  = filtro + "Descrizione LIKE '%" + Edit2->Text+"%'";
		filter = true;
	}
	try {
		if ((Edit3->Text != "")&&(Edit3->Text.ToIntDef(0) > 0)) {
            if (filter)
                filtro  = filtro + " AND ";
			filtro  = filtro + "IdFornitore =" + Edit3->Text;
            filter = true;
        }
    }catch(...){}
	try {
		if ((cbTipoCarne->Text != "")&&(cbTipoCarne->Items->IndexOf(cbTipoCarne->Text) > 0)) {
			if (filter)
				filtro  = filtro + " AND ";
			filtro  = filtro + "Tipologia =" + IntToStr(cbTipoCarne->Items->IndexOf(cbTipoCarne->Text));
			filter = true;
		}
	}catch(...){}
/*    if (!filter)
		filtro = "Select * from Articles  ";  */
	filtro = filtro + " order by IdFornitore";
	ADOQuery1->Close();
	ADOQuery1->SQL->Clear() ;
	ADOQuery1->SQL->Add(filtro);
	ADOQuery1->Open();
	ADOQuery1->First();
	Tot->Caption = IntToStr(ADOQuery1->RecordCount);
}
//---------------------------------------------------------------------------
void __fastcall TFormAnagraficaFornitori::Edit1Change(TObject *Sender)
{
    FormActivate(this);
}
//---------------------------------------------------------------------------
void __fastcall TFormAnagraficaFornitori::BitBtn1Click(TObject *Sender)
{
	FormInsAnagraficaArticoli->idtabella = 2 ;
	FormInsAnagraficaArticoli->insert = true ;
	FormInsAnagraficaArticoli->EdIdArticles->Text = IntToStr(dmDB->CreaIdFornitore());
	FormInsAnagraficaArticoli->EdName->Text = "";
	FormInsAnagraficaArticoli->EdDesc->Text = "";
	FormInsAnagraficaArticoli->TipoCarne->ItemIndex = 0;
	FormInsAnagraficaArticoli->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormAnagraficaFornitori::BitBtn2Click(TObject *Sender)
{
	if ((ADOQuery1->RecNo)&&(ADOQuery1->FieldByName("IdFornitore")->AsInteger > 0)) {
		FormInsAnagraficaArticoli->idtabella = 2 ;
		FormInsAnagraficaArticoli->insert = false ;
		FormInsAnagraficaArticoli->EdIdArticles->Text = ADOQuery1->FieldByName("IdFornitore")->AsString;
		FormInsAnagraficaArticoli->EdName->Text = ADOQuery1->FieldByName("Codice")->AsString;
		FormInsAnagraficaArticoli->EdDesc->Text = ADOQuery1->FieldByName("Descrizione")->AsString;
		FormInsAnagraficaArticoli->TipoCarne->ItemIndex = ADOQuery1->FieldByName("TipoCarne")->AsInteger ;
		FormInsAnagraficaArticoli->Show();
    }

}
//---------------------------------------------------------------------------

