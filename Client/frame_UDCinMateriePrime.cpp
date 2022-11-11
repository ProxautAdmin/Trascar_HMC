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
    filtro = filtro + " and piano=1  ";
    filtro = filtro + " and ISNULL(pos_selezionata,0)=0 and ISNULL(pos_disabilitata,0)=0 and ISNULL(pos_prenotata,0)=0 ";
    filtro = filtro + " and ISNULL(selezionata,0)=0 and ISNULL(disabilitata,0)=0 and ISNULL(prenotata,0)=0  ";
    filtro = filtro + " order by Pos";
    try {
        ADOQuery1->Close();
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add(filtro);
        ADOQuery1->Open();
        ADOQuery1->First();
    }
    catch (...) {
        dmDB->LogMsg("Errore in refresh griglia PosizioniMateriePrime ");
    }

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
