// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UDCform.h"
#include "main.h"
#include "DBClient.h"
#include "DB.h"
#include "ExtraFunction.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "frame_modulo"
#pragma link "UDCFrame"
#pragma link "MyShape"
#pragma resource "*.dfm"
TFormDatiUDC *FormDatiUDC;

// ---------------------------------------------------------------------------
__fastcall TFormDatiUDC::TFormDatiUDC(TComponent* Owner)
    : TForm(Owner)
{
}
// ---------------------------------------------------------------------------

void TFormDatiUDC::VisualizzaDatiUDC() {
    AnsiString strsql, zona, str;

    TIndexList TabUDC;
    TMyShape *Pal;
    int i, status, tipologia;
    int lleft;

    dmDBClient->FullTabellaK("Select * from UDCList WHERE idUDC=" + String(IDUDC), "IDUDC", TabUDC);
    i = 0;

    if (IDUDC) {
        eUDC->Text = IntToStr(IDUDC);
        fUDCSmall->eIDArtUDC->Text = TabUDC[IDUDC]["IDARTUDC"].ToIntDef(0);
        edArticolo->Text == TabUDC[IDUDC]["IDARTICOLO"].ToIntDef(0);
        // fUDCSmall->eCodice->Text = TabUDC[IDUDC]["CODARTUDC"];
        fUDCSmall->RiempiCampi(fUDCSmall->eIDArtUDC->Text.ToIntDef(0), dmDB->ReturnStringaHexDatiDaIdUdc(IDUDC), true);

    }
    else {
        if (eIDArtUDCm->Text.ToIntDef(0) != 0) {
            fUDCSmall->eIDArtUDC->Text = eIDArtUDCm->Text;
            edArticolo->Text == TabUDC[IDUDC]["IDARTICOLO"].ToIntDef(0);
        }
        else {
            // eUDC->Text = dmDB->CreaIdArtUdc();
            eUDC->Text = "0";
            edArticolo->Text = "0";
        }
        status = 0;
        // eStatus->Text = "";
        // eTipologia->Text = "";
        // fUDCSmall->eCodice->Text= "";
    }

}

void __fastcall TFormDatiUDC::FormClose(TObject *Sender, TCloseAction &Action) {
    MainForm->aggiorna_stato_pos = 1;

}
// ---------------------------------------------------------------------------

void __fastcall TFormDatiUDC::FormActivate(TObject *Sender)
{
    // BitBtn1->Enabled = pos_update;
    fUDCSmall->BtnWriteUdcPlc->Visible = false;
    fUDCSmall->BtnWritePlc->Visible = false;

    VisualizzaDatiUDC();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDatiUDC::BitBtnCloseClick(TObject *Sender)
{
    Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDatiUDC::fUDCSmallsbSearchClick(TObject *Sender)
{
    fUDCSmall->sbSearchClick(this);

}
// ---------------------------------------------------------------------------
