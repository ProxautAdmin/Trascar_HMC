// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ins_UDC.h"
#include "db.h"
#include "ExtraFunction.h"
// #include "anagrafica_articoliUDC.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFormInsUDC *FormInsUDC;

// ---------------------------------------------------------------------------
__fastcall TFormInsUDC::TFormInsUDC(TComponent* Owner)
    : TForm(Owner) {
}

// ---------------------------------------------------------------------------

void __fastcall TFormInsUDC::FormActivate(TObject *Sender) {
    TADOQuery *ADOQuery;
    AnsiString stringa;

    edIDUDC->Enabled = false;
    if (insert) {
        edCodUDC->Text = "";
        edDescUDC->Text = "";
        cbTipoUDC->Text = dmExtraFunction->RiempiTipoUDC(cbTipoUDC, "00");
        cbStatus->Text = dmExtraFunction->RiempiTipoUDC(cbStatus, "00"); ;
        edLotto->Text = "";
        edPeso->Text = "0";
        cbRiservato->Checked = false;
        cbParziale->Checked = false;
        edIntImp->Text = "0";
        edCodArt->Text = "";
        lbDescArt->Caption = "";
        cbBase->Text = dmExtraFunction->RiempiTipoUDC(cbBase, "00");
        lbDescContenitore->Caption = "";
    }
    else {

        try {
            ADOQuery = new TADOQuery(NULL);
            ADOQuery->Connection = dmDB->ADOConnection1;
            stringa = "Select * from udc_view where IdUDC = " + IntToStr(edIDUDC->Text.ToIntDef(0)) + " and IDUDC>0";
            ADOQuery->Close();
            ADOQuery->SQL->Clear();
            ADOQuery->SQL->Append(stringa);
            ADOQuery->Open();
            if (ADOQuery->RecordCount > 0) {
                edCodUDC->Text = ADOQuery->FieldByName("CodUDC")->AsString;
                edDescUDC->Text = ADOQuery->FieldByName("DescUDC")->AsString;
                cbTipoUDC->Text = dmExtraFunction->RiempiTipoUDC(cbTipoUDC, dmExtraFunction->PadS(ADOQuery->FieldByName("Codtipoudc")->AsString, 2, "0"));
                cbStatus->Text = dmExtraFunction->RiempiTipoStatus(cbStatus, dmExtraFunction->PadS(ADOQuery->FieldByName("stato")->AsString, 2, "0"));
                edLotto->Text = ADOQuery->FieldByName("lotto")->AsString;
                edPeso->Text = ADOQuery->FieldByName("PesoAttuale")->AsString;
                cbRiservato->Checked = ADOQuery->FieldByName("Riservato")->AsInteger;
                cbParziale->Checked = ADOQuery->FieldByName("Parziale")->AsInteger;
                edIntImp->Text = ADOQuery->FieldByName("IndiceImpilabilita")->AsString;
                edCodArt->Text = ADOQuery->FieldByName("codart")->AsString;
                lbDescArt->Caption = ADOQuery->FieldByName("descart")->AsString;
                cbBase->Text = dmExtraFunction->RiempiTipoUDC(cbBase, dmExtraFunction->PadS(ADOQuery->FieldByName("idcontenitore")->AsString, 2, "0"));
                lbDescContenitore->Caption = ADOQuery->FieldByName("DescContenitore")->AsString;
            }
            ADOQuery->Close();
            // MainForm->LogMsg(stringa);
        }
        catch (...) {

        }
        delete ADOQuery;

    }
}

// ---------------------------------------------------------------------------

void __fastcall TFormInsUDC::btEsciClick(TObject *Sender)
{
    Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsUDC::btConfermaClick(TObject *Sender)
{
    AnsiString stringa;
    int ok;

    if (insert) {
        /*
         stringa = "Insert Into UDC(IDUDC, CodUDC, DescUDC, Codtipoudc,stato, lotto, PesoAttuale, Riservato, Parziale, IndiceImpilabilita, idcontenitore)";
         stringa = stringa + " VALUES(" +
         edIDUDC->Text + ", " +
         " '" + edCodUDC->Text.Trim() + "', " +
         " '" + edDescUDC->Text.Trim() + "', " +
         " '" + StrToInt(cbTipoUDC->Text.SubString(1, 2)) + ", " +
         cbStatus->Text.SubString(1, 2) + ", " +
         " '" + edLotto->Text.Trim() + "', " +
         IntToStr(edPeso->Text.ToIntDef(0)) + ", " +
         (cbRiservato->Checked ? 1 : 0) + ", " +
         (cbParziale->Checked ? 1 : 0) + ", " +
         edIntImp->Text.ToIntDef(0) + ", " +
         " '" + StrToInt(cbBase->Text.SubString(1, 2)) + ", " +
         Trim(edCodUDC->Text.UpperCase()) + "')";
         */
        stringa.printf("INSERT INTO UDC (IDUDC, CodUDC, Descrizione, Codtipoudc, stato, lotto, PesoAttuale, Riservato, Parziale, IndiceImpilabilita, idtipocontenitore )"
            " VALUES (%d,'%s','%s',%d,%d,'%s',%d,%d,%d, %d, %d)"
            , edIDUDC->Text.ToIntDef(0)
            , edCodUDC->Text.UpperCase().c_str()
            , edDescUDC->Text.c_str()
            , StrToInt(cbTipoUDC->Text.SubString(1, 2))
            , StrToInt(cbStatus->Text.SubString(1, 2))
            , edLotto->Text.c_str()
            , edPeso->Text.ToIntDef(0)
            , (cbRiservato->Checked ? 1 : 0)
            , (cbParziale->Checked ? 1 : 0)
            , edIntImp->Text.ToIntDef(0)
            , StrToInt(cbBase->Text.SubString(1, 2))
            );
    }
    else {
        stringa.printf("Update UDC set CodUDC='%s', Descrizione='%s', Codtipoudc=%d, stato=%d, lotto='%s', PesoAttuale=%d, Riservato=%d, Parziale=%d, IndiceImpilabilita=%d, idtipocontenitore=%d where IDUDC =%d"
            , edCodUDC->Text.UpperCase().c_str()
            , edDescUDC->Text.c_str()
            , StrToInt(cbTipoUDC->Text.SubString(1, 2))
            , StrToInt(cbStatus->Text.SubString(1, 2))
            , edLotto->Text
            , edPeso->Text.ToIntDef(0)
            , (cbRiservato->Checked ? 1 : 0)
            , (cbParziale->Checked ? 1 : 0)
            , edIntImp->Text.ToIntDef(0)
            , StrToInt(cbBase->Text.SubString(1, 2))
            , edIDUDC->Text.ToIntDef(0)
            );
    }

    try {
        Query->Close();
        Query->SQL->Clear();
        Query->SQL->Append(stringa);
        Query->ExecSQL();
        Query->Close();
        dmDB->LogMsg(stringa);
        Close();
    }
    catch (...) {
        dmDB->LogMsg("Eccezione insert/update UDC");
        Query->Close();
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormInsUDC::FormClose(TObject * Sender,
    TCloseAction & Action)
{
    insert = false;
    // FormAnagraficaArticoliUDC->FormActivate(this);

}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
