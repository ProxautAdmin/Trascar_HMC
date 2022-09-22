// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UDClist.h"
#include "db.h"
#include "DBClient.h"
#include "main.h"
#include "SocketComunicazioneClient.h"
// #include "crea_UDC.h"
#include "ins_UDC.h"
// #include "UDCform.h"
#include "ExtraFunction.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormUDCList *FormUDCList;

// ---------------------------------------------------------------------------
__fastcall TFormUDCList::TFormUDCList(TComponent* Owner)
    : TForm(Owner)
{
    blocca_insert = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormUDCList::BitBtn4Click(TObject *Sender)
{
    udcsel = 0;
    Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormUDCList::FormClose(TObject *Sender,
    TCloseAction &Action)
{
    ADOQuery1->Close();
    Edit1->Text = "";

    Edit3->Text = "";
    dmDB->aggiorna_tab_posizioni_locale = 1;
    blocca_insert = 0;

}

// ---------------------------------------------------------------------------
void __fastcall TFormUDCList::DBGrid1DrawColumnCell(TObject *Sender,
    const TRect &Rect, int DataCol, TColumn *Column,
    TGridDrawState State)
{
    int color;
    AnsiString code;
    if ((Column->FieldName == "Colore") &&
        (Column->Field->AsInteger > 0)) {
        code = IntToStr(Column->Field->AsInteger);
        color = Column->Field->AsInteger;
        DBGrid1->Canvas->Brush->Color = color;
        DBGrid1->Canvas->Font->Color = color;
        DBGrid1->Canvas->TextRect(Rect, Rect.left + 2, Rect.top + 2, code);
    }

}

// ---------------------------------------------------------------------------
void __fastcall TFormUDCList::FormActivate(TObject *Sender)
{
    Edit1->Text = "";

    Edit3->Text = "";
    // BitBtnEdit->Visible = false;
    // BitBtnAdd->Visible = false;

    Edit1Change(this);

}

// ---------------------------------------------------------------------------

void __fastcall TFormUDCList::Edit1Change(TObject *Sender)
{
    Word Year, Month, Day;
    AnsiString filtro, str;
    bool filter = false;
    //

    filtro = "select * ";
    filtro = filtro + " FROM UDC_view ";

    try {
        if (Edit1->Text != "") {
            if (filter)
                filtro = filtro + " AND ";
            else
                filtro = filtro + " where ";
            filtro = filtro + "codudc LIKE '%" + Edit1->Text + "%'";
            filter = true;
        }
    }
    catch (...) {}
    try {
        if ((Edit3->Text != "") && (Edit3->Text.ToIntDef(0) > 0)) {
            if (filter)
                filtro = filtro + " AND ";
            else
                filtro = filtro + " where ";
            filtro = filtro + "Idudc =" + Edit3->Text;
            filter = true;
        }
    }
    catch (...) {}

    /* if (!filter)
     filtro = "Select * from Articles  "; */
    filtro = filtro + " order by Idudc";
    ADOQuery1->Close();
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add(filtro);
    ADOQuery1->Open();
    ADOQuery1->Last();
    Tot->Caption = IntToStr(ADOQuery1->RecordCount);
    BitBtn1->Enabled = !blocca_insert;
    BitBtn2->Enabled = dmDB->pwdlevel;
}

// ---------------------------------------------------------------------------

void __fastcall TFormUDCList::BitBtn1Click(TObject *Sender) {
    /* int posold = 0, ret;
     AnsiString messaggio;
     udcsel = ADOQuery1->FieldByName("IdUDC")->AsInteger;
     if (Trim(ADOQuery1->FieldByName("CodUDC")->AsString) != "VUOTO") {
     posold = dmDBClient->CercaUDCinPosizione(udcsel);
     }
     if (posold > 0) {
     messaggio = "Are you sure you want to move UDC " + String(udcsel) + " from pos " + String(posold) + " to pos " + String(pos) + "?";
     if (Application->MessageBox(L"Are you sure you want to move UDC ", L"Confirm!!!", MB_YESNO) == IDYES) {
     dmDB->EliminaUDCdaPosizione(udcsel);
     dmDB->ArticoloPrelevatoDepositato(pos, udcsel, piano); // dmDB->InsertUDCInPosizione(udcsel, pos);
     }
     }
     else
     dmDB->ArticoloPrelevatoDepositato(pos, udcsel, piano); // dmDB->InsertUDCInPosizione(udcsel, pos);
     Close(); */

}
// ---------------------------------------------------------------------------

void __fastcall TFormUDCList::BitBtnEditClick(TObject *Sender)
{
    if ((ADOQuery1->RecNo) && (ADOQuery1->FieldByName("IDUDC")->AsInteger > 0)) {
        FormInsUDC->insert = false;
        FormInsUDC->edIDUDC->Text = ADOQuery1->FieldByName("IDUDC")->AsString;

        FormInsUDC->ShowModal();
        Edit1Change(this);
    }
}
// ---------------------------------------------------------------------------

void __fastcall TFormUDCList::BitBtnAddClick(TObject *Sender) {
    FormInsUDC->insert = true;
    FormInsUDC->edIDUDC->Text = dmDB->CreaIdUDC()  ;

    FormInsUDC->ShowModal();
    Edit1Change(this);
}
// ---------------------------------------------------------------------------

void __fastcall TFormUDCList::ADOQuery1AfterScroll(TDataSet *DataSet)
{
    // per correggere il bug della grid che seleziona una riga sola e dopo non rimette la scrollbar
    DBGrid1->Width = DBGrid1->Width + 1;
    DBGrid1->Width = DBGrid1->Width - 1;
}
// ---------------------------------------------------------------------------
