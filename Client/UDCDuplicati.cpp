//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UDCDuplicati.h"
#include "insarticles.h"
#include "anagrafica_articoli.h"
#include "db.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormUDCDuplicati *FormUDCDuplicati;
//---------------------------------------------------------------------------
__fastcall TFormUDCDuplicati::TFormUDCDuplicati(TComponent* Owner)
    : TForm(Owner)
{
	insert_produzione = 0 ;
}
//---------------------------------------------------------------------------

void __fastcall TFormUDCDuplicati::BitBtn4Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormUDCDuplicati::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    ADOQuery1->Close();


}
//---------------------------------------------------------------------------

void __fastcall TFormUDCDuplicati::FormActivate(TObject *Sender)
{
AnsiString filtro ;
bool filter ;
    //

	filtro = "select Pos, (Ltrim(RTrim(NomePos))) as NomePos, fila, zona, nomecorsia, idudc from piani_view where idudc in (select idudc from piani group by idudc having count(idudc) > 1) and idudc > 0  ";
	filtro = filtro + " order by idudc desc";
	ADOQuery1->Close();
	ADOQuery1->SQL->Clear() ;
	ADOQuery1->SQL->Add(filtro);
	ADOQuery1->Open();
	ADOQuery1->First();

}
//---------------------------------------------------------------------------




