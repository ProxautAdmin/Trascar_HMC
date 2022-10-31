// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frame_UDCinMateriePrime.h"
#include "dmFunzioniComuniClientServer.h"
#include "main.h"
#include "db.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrameUDCinMateriePrime *FrameUDCinMateriePrime;

// ---------------------------------------------------------------------------
__fastcall TFrameUDCinMateriePrime::TFrameUDCinMateriePrime(TComponent* Owner) : TFrame(Owner) {

}

// ---------------------------------------------------------------------------
void TFrameUDCinMateriePrime::RiempiCampi() {
    AnsiString filtro = ""; ;
    filtro = "Select * from PosizioniMateriePrime ";
    filtro = filtro + " where idudc>0  ";
    filtro = filtro + " order by Pos";
    ADOQuery1->Close();
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add(filtro);
    ADOQuery1->Open();
    ADOQuery1->First();

}

void __fastcall TFrameUDCinMateriePrime::btScegliClick(TObject *Sender) {
    MainForm->trova_HMC_Anagrafica = ADOQuery1->FieldByName("componente")->AsString;
}
// ---------------------------------------------------------------------------

void __fastcall TFrameUDCinMateriePrime::FrameEnter(TObject *Sender)
{
    RiempiCampi();
}
// ---------------------------------------------------------------------------
