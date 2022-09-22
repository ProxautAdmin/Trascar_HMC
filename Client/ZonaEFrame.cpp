// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ZonaEFrame.h"
#include "DB.h"
#include "DBImpianto.h"
#include "main.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MyShape"
#pragma resource "*.dfm"
TfrZonaE *frZonaE;
AnsiString Zona;

// ---------------------------------------------------------------------------
__fastcall TfrZonaE::TfrZonaE(TComponent* Owner) : TFrame(Owner) {
    Zona = "E";
}

// ---------------------------------------------------------------------------

void TfrZonaE::AggiornaDati() {
    int numeroelementi = 1;
    int idx = 0, trovato = 0; ;
    int pos;
    AnsiString str;
    TPanel *Pan;
    TRecordList TabPosizioni;
    str = "piani_view WHERE Zona = '" + Zona + "' ORDER BY Pos ";
    dmDB->CaricaTabella(str, TabPosizioni);
    for (int j = 1; j <= numeroelementi; j++) {
        Pan = (TPanel*) FindComponent("pnPos" + Zona + IntToStr(j));
        if (Pan != Null) {
            idx = 0;
            trovato = 0;

            while ((idx < TabPosizioni.size()) && (trovato == 0)) {
                pos = TabPosizioni[idx]["POS"].ToIntDef(0);
                if (Pan->Tag == pos) {
                    trovato = 1;
                    if (TabPosizioni[idx]["PRENOTATA"].ToIntDef(0) || TabPosizioni[idx]["POS_PRENOTATA"].ToIntDef(0)) {
                        Pan->Color = clBlue;
                    }
                    else if (TabPosizioni[idx]["DISABILITATA"].ToIntDef(0) || TabPosizioni[idx]["POS_DISABILITA"].ToIntDef(0)) {
                        Pan->Color = clRed;
                    }
                    else if (TabPosizioni[idx]["SELEZIONATA"].ToIntDef(0)) {
                        Pan->Color = clLime;
                    }
                    else if (TabPosizioni[idx]["IDUDC"].ToIntDef(0) > 0) {
                        Pan->Color = clYellow;
                    }
                    else {
                        Pan->Color = clWhite;
                    }
                    // caption
                    if (TabPosizioni[idx]["IDUDC"].ToIntDef(0) == 0) {
                        Pan->Caption = "Pos." + IntToStr(j) + " (" + IntToStr(Pan->Tag) + ")";
                    }
                    else {
                        Pan->Caption = "Pos." + IntToStr(j) + " - " + IntToStr(TabPosizioni[idx]["IDUDC"].ToIntDef(0));
                    }
                }
                if (!trovato)
                    idx++;
            }
        }
    }
}

// ---------------------------------------------------------------------------

void __fastcall TfrZonaE::pnPosEMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
      TPanel *Pan;
    Pan = (TPanel*) Sender;
    if (Pan != NULL) {
        if (Button == mbLeft) {
            if (Pan->Color == clLime) {
                dmDBImpianto->AggiornaSelezionePosizioni(Zona, Pan->Tag, 0);

            }
            else {
                // dmDBImpianto->AggiornaSelezionePosizioni(Zona, 0, 0);
                if (Pan->Color == clYellow) {
                    dmDBImpianto->AggiornaSelezionePosizioni(Zona, Pan->Tag, 1);
                }

            }
        }
        if (Button == mbRight) {
            if (Pan->Color == clWhite) {
                dmDB->ArticoloPrelevatoDepositato(Pan->Tag, 1, 1, dmDB->FilaPosizione(Pan->Tag));
            }
            else if (Pan->Color == clYellow) {
                dmDB->ArticoloPrelevatoDepositato(Pan->Tag, 0, 1, dmDB->FilaPosizione(Pan->Tag));
            }

        }
    }
}
//---------------------------------------------------------------------------

