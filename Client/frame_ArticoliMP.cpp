// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frame_ArticoliMP.h"
#include "dmFunzioniComuniClientServer.h"
#include "main.h"
#include "db.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrameMatPrime *FrameMatPrime;

// ---------------------------------------------------------------------------
__fastcall TFrameMatPrime::TFrameMatPrime(TComponent* Owner) : TFrame(Owner) {

}

// ---------------------------------------------------------------------------
void TFrameMatPrime::RiempiCampi() {
    AnsiString filtro = ""; ;
    filtro = "Select * from HMC_Anagrafica ";

    filtro = filtro + " order by componente";
    ADOQuery1->Close();
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add(filtro);
    ADOQuery1->Open();
    ADOQuery1->First();

}

void __fastcall TFrameMatPrime::btScegliClick(TObject *Sender) {
    MainForm->trova_HMC_Anagrafica = ADOQuery1->FieldByName("componente")->AsString;
}
// ---------------------------------------------------------------------------

void __fastcall TFrameMatPrime::FrameEnter(TObject *Sender)
{
    RiempiCampi();
}
// ---------------------------------------------------------------------------
