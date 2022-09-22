//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "login.h"
#include "db.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLogin *FormLogin;
//---------------------------------------------------------------------------
__fastcall TFormLogin::TFormLogin(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFormLogin::FormKeyPress(TObject *Sender, char &Key)
{
    if (Key == VK_RETURN)
        BitBtn1Click(this) ;
    else if (Key == VK_ESCAPE)
        BitBtn3Click(this) ;
}
//---------------------------------------------------------------------------
void __fastcall TFormLogin::BitBtn1Click(TObject *Sender)
{
	AnsiString stringa,user,pw ;
    user = Edit1->Text.UpperCase() ;
	pw = Edit2->Text.UpperCase() ;
	if (!dmDB->ControllaUtentePassword(user,pw))
		ShowMessage("Nome utente o Password errata");
	else
		dmDB->TimerPassword->Enabled = true ;
        dmDB->LogMsg("Utente in uso:" + user);
	Close();

}
//---------------------------------------------------------------------------
void __fastcall TFormLogin::BitBtn3Click(TObject *Sender)
{
	//
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormLogin::FormActivate(TObject *Sender)
{
    Edit1->Text = "";
    Edit2->Text = "";
	dmDB->TimerPassword->Enabled = false ;
    Edit1->SetFocus();
}
//---------------------------------------------------------------------------
