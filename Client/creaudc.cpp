//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "creaudc.h"
#include "db.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormCreaUdc *FormCreaUdc;
//---------------------------------------------------------------------------
__fastcall TFormCreaUdc::TFormCreaUdc(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormCreaUdc::BitBtnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

