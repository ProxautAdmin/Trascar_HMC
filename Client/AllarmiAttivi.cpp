//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AllarmiAttivi.h"
#include "insarticles.h"
#include "anagrafica_articoli.h"
#include "db.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAllarmiAttivi *FormAllarmiAttivi;
//---------------------------------------------------------------------------
__fastcall TFormAllarmiAttivi::TFormAllarmiAttivi(TComponent* Owner)
    : TForm(Owner)
{
	insert_produzione = 0 ;
}
//---------------------------------------------------------------------------

void __fastcall TFormAllarmiAttivi::BitBtn4Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormAllarmiAttivi::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    ADOQuery1->Close();


}
//---------------------------------------------------------------------------

void __fastcall TFormAllarmiAttivi::FormActivate(TObject *Sender)
{
AnsiString filtro ;
bool filter ;
    //

	filtro = "Select * from ActiveAlarm ";
	filtro = filtro + " order by AlarmNumber";
	ADOQuery1->Close();
	ADOQuery1->SQL->Clear() ;
	ADOQuery1->SQL->Add(filtro);
	ADOQuery1->Open();
	ADOQuery1->First();

}
//---------------------------------------------------------------------------




