// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GestUser.h"
#include "ins_Utenti.h"
#include "db.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmGestUser *FrmGestUser;

// ---------------------------------------------------------------------------
__fastcall TFrmGestUser::TFrmGestUser(TComponent* Owner)
    : TForm(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TFrmGestUser::FormActivate(TObject *Sender)
{
    // Edit3->Text = "";

    btIns->Enabled = true;
    btMod->Enabled = true;
    edFunctionChange();
}
// ---------------------------------------------------------------------------

void TFrmGestUser::edFunctionChange()
{
    Word Year, Month, Day;
    AnsiString filtro, str;

    filtro = "SELECT ID, UserId, password, '***' as fakePassword, IdLogin, CASE WHEN Sistema >= 1 THEN 'X' ELSE '' END AS sistema ";
    filtro = filtro + " from Login ";
    filtro = filtro + " where (1=1) ";

    try {
        if (edUtente->Text != "") {
            filtro = filtro + " and funzione LIKE '%" + edUtente->Text + "%'";
        }
    }
    catch (...) {}
    filtro = filtro + " order by userID, id";
    ADOQuery1->Close();
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add(filtro);
    ADOQuery1->Open();
    ADOQuery1->First();
    Tot->Caption = IntToStr(ADOQuery1->RecordCount);

}

// ---------------------------------------------------------------------------
void __fastcall TFrmGestUser::btModClick(TObject *Sender)
{
    FrmInsUtenti->insert = false;
    FrmInsUtenti->edId->Text = ADOQuery1->FieldByName("ID")->AsInteger;
    FrmInsUtenti->edUser->Text = ADOQuery1->FieldByName("UserID")->AsString;
    FrmInsUtenti->edPassword->Text = ADOQuery1->FieldByName("Password")->AsString;
    FrmInsUtenti->cbLivello->Text = ADOQuery1->FieldByName("IdLogin")->AsInteger;
    FrmInsUtenti->cbSistema->Checked = (ADOQuery1->FieldByName("sistema")->AsString == "X" ? true : false);
    FrmInsUtenti->ShowModal();
    edFunctionChange();

}

// ---------------------------------------------------------------------------
void __fastcall TFrmGestUser::btInsClick(TObject *Sender) {

    FrmInsUtenti->insert = true;
    FrmInsUtenti->edId->Text = "";
    FrmInsUtenti->edUser->Text = "";
    FrmInsUtenti->edPassword->Text = 0;
    FrmInsUtenti->cbLivello->Text = 0;
    FrmInsUtenti->cbSistema->Checked = false;
    FrmInsUtenti->ShowModal();
    edFunctionChange();

}

// ---------------------------------------------------------------------------
void __fastcall TFrmGestUser::ADOQuery1AfterScroll(TDataSet *DataSet) {
    // per correggere il bug della grid che seleziona una riga sola e dopo non rimette la scrollbar
    DBGrid1->Width = DBGrid1->Width + 1;
    DBGrid1->Width = DBGrid1->Width - 1;
}

// ---------------------------------------------------------------------------
void __fastcall TFrmGestUser::btEsciClick(TObject *Sender) {
    Close();
}
// ---------------------------------------------------------------------------

void __fastcall TFrmGestUser::btDelClick(TObject *Sender)
{
    AnsiString strsql;
    if (ADOQuery1->FieldByName("sistema")->AsString != "X") {
        if ((ADOQuery1->RecNo) && (Application->MessageBox(L"Sei sicuro di voler cancellare questo utente?", L"Conferma", MB_YESNO) == IDYES)) {
            QueryDel->Close();
            QueryDel->SQL->Clear();
            strsql = "delete from Login where id = " + ADOQuery1->FieldByName("ID")->AsString;
            QueryDel->SQL->Append(strsql);
            QueryDel->ExecSQL();
            QueryDel->Close();
            dmDB->LogMsg(strsql);
            edFunctionChange();
        }
    }
    else{
        ShowMessage("Utente di sistema non eliminabile");
    }
}
// ---------------------------------------------------------------------------
