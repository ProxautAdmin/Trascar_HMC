// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ZonaDFrame.h"
#include "DB.h"
#include "DBImpianto.h"
#include "main.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MyShape"
#pragma resource "*.dfm"
TfrZonaD *frZonaD;
AnsiString Zona;

// ---------------------------------------------------------------------------
__fastcall TfrZonaD::TfrZonaD(TComponent* Owner) : TFrame(Owner) {
    Zona = "D";
    AbilitaConferma = 0;
}

void TfrZonaD::AggiornaDati() {

}

void __fastcall TfrZonaD::btSvuotaZonaDClick(TObject *Sender) {
    int idx_plc = IDX_PLCPRELIEVO;
    int idx = 1;

    if (ClientData.ParametriFunzionali.Giorno != 0) {
        // if (dmDB->PresenzaCentroMissione(ClientData.Plc[idx_plc].Prelievo[idx][1].pos, 0) == 0) {       // non funziona a livello logico
        if (1 == 1) {
            if (dmDB->PosPresenteMissioneAttiva(ClientData.Plc[idx_plc].Prelievo[idx][1].pos) == 0) {
                if (Application->MessageBox(L"Sei sicuro di voler svuotare tutte le posizioni in zona D?", L"Conferma", MB_YESNO) == IDYES) {
                    dmDB->QueryPos->Close();
                    dmDB->QueryPos->SQL->Clear();
                    dmDB->QueryPos->SQL->Append("update piani_view SET idudc = 0 where idzona=4");
                    dmDB->QueryPos->ExecSQL();
                    dmDB->QueryPos->Close();
                    dmDB->LogMsg("Svuotata zona D");
                }
            }
            else
                ShowMessage("Operazione non permessa con missioni presenti");
        }
        else
            ShowMessage("Operazione non permessa con missioni presenti in Centro Missioni");
    }
    else
        ShowMessage("Operazione non permessa con l'impianto in stato NOTTE");

}
// ---------------------------------------------------------------------------
