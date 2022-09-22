// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ins_Utenti.h"
#include "db.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFrmInsUtenti *FrmInsUtenti;

// ---------------------------------------------------------------------------
__fastcall TFrmInsUtenti::TFrmInsUtenti(TComponent* Owner)
    : TForm(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TFrmInsUtenti::BitBtn2Click(TObject *Sender)
{
    Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFrmInsUtenti::BitBtn1Click(TObject *Sender)
{
    AnsiString stringa, ora1, ora2, ora3, timemax, timemin;
    int ok, idart_presente;

    if (insert) {
        stringa = "Insert Into login (userid,password,idlogin,sistema)";
        stringa = stringa + " VALUES ('" + edUser->Text.Trim() + "','" + edPassword->Text.Trim() + "'," + cbLivello->Text.Trim() + "," + (cbSistema->Checked ? 1 : 0) + ")";
    }
    else {

        stringa = "Update login set ";
        stringa = stringa + " userid = '" + edUser->Text.Trim() + "' ";
        stringa = stringa + " ,password = '" + edPassword->Text.Trim() + "' ";
        stringa = stringa + " ,idlogin = " + cbLivello->Text.Trim() + " ";
        stringa = stringa + " ,sistema = " + (cbSistema->Checked ? 1 : 0) + " ";
        stringa = stringa + " where id = " + edId->Text.Trim();
    }
    try {
        Query->Close();
        Query->SQL->Clear();
        Query->SQL->Append(stringa);
        Query->ExecSQL();
        dmDB->LogMsg(stringa);
    }
    catch (...) {
        dmDB->LogMsg("Eccezione insert/update Utenti");
    }
    Query->Close();
    Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFrmInsUtenti::FormClose(TObject *Sender,
    TCloseAction &Action)
{
    insert = false;
    // FormTipologia->FormActivate(this);
}

// ---------------------------------------------------------------------------
void __fastcall TFrmInsUtenti::FormActivate(TObject *Sender)
{

    // cbFunzione->Enabled = false;
    // edId->Enabled = false;
    if (insert)
        FrmInsUtenti->Caption = "Inserisci utenti";
    else
        FrmInsUtenti->Caption = "Modifica utenti";

}
// ---------------------------------------------------------------------------
