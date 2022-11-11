// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frame_ArticoliSovrapposti.h"
#include "dmFunzioniComuniClientServer.h"
#include "main.h"
#include "db.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrameSovrapposti *FrameSovrapposti;

// ---------------------------------------------------------------------------
__fastcall TFrameSovrapposti::TFrameSovrapposti(TComponent* Owner) : TFrame(Owner) {

}

// ---------------------------------------------------------------------------
void TFrameSovrapposti::RiempiCampi() {
    AnsiString filtro = ""; ;
    filtro = "Select distinta, [Descrizione distinta] from HMC_SOVRAPPOSTI ";
    filtro = filtro + " order by distinta";
    try {
        ADOQuery1->Close();
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add(filtro);
        ADOQuery1->Open();
        ADOQuery1->First();
    }
    catch (...) {
        dmDB->LogMsg("Errore in refresh griglia HMC_SOVRAPPOSTI ");
    }
}

void __fastcall TFrameSovrapposti::btScegliClick(TObject *Sender) {
    MainForm->trova_HMC_Anagrafica = ADOQuery1->FieldByName("distinta")->AsString;
}
// ---------------------------------------------------------------------------

void __fastcall TFrameSovrapposti::FrameEnter(TObject *Sender)
{
    RiempiCampi();
}
// ---------------------------------------------------------------------------
