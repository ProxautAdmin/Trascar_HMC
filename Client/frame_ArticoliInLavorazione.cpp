// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frame_ArticoliInLavorazione.h"
#include "dmFunzioniComuniClientServer.h"
#include "main.h"
#include "db.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrameArticoliInLavorazione *FrameArticoliInLavorazione;

// ---------------------------------------------------------------------------
__fastcall TFrameArticoliInLavorazione::TFrameArticoliInLavorazione(TComponent* Owner) : TFrame(Owner) {

}

// ---------------------------------------------------------------------------
void TFrameArticoliInLavorazione::RiempiCampi() {
    AnsiString filtro = ""; ;

    filtro = "Select * from HMC_ORDINI_IN_LAVORAZIONE_copia ";
    filtro = filtro + " order by posizione";
    try {
        ADOQuery1->Close();
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add(filtro);
        ADOQuery1->Open();
        ADOQuery1->First();
    }
    catch (...) {
        dmDB->LogMsg("Errore in refresh griglia HMC_ORDINI_IN_LAVORAZIONE_copia ");
    }
}

void __fastcall TFrameArticoliInLavorazione::btScegliClick(TObject *Sender) {
    MainForm->trova_HMC_Anagrafica = ADOQuery1->FieldByName("articolo")->AsString;
}
// ---------------------------------------------------------------------------

void __fastcall TFrameArticoliInLavorazione::FrameEnter(TObject *Sender)
{
    RiempiCampi();
}

// ---------------------------------------------------------------------------
void __fastcall TFrameArticoliInLavorazione::DBGrid1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    // questa cosa del timer fa cagare ma diversamente da errore

    // TfGestAGVMidi *fGestAGVMidi;
    // if (fGestAGVMidi != NULL) {
    MainForm->codiceriga = ADOQuery1->FieldByName("posizione")->AsString.ToIntDef(0);
    // }
}
// ---------------------------------------------------------------------------
