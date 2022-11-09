// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ZonaIFrame.h"
#include "DB.h"
#include "DBImpianto.h"
#include "main.h"
#include "ExtraFunction.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MyShape"
#pragma link "frame_ArticoliMP"
#pragma resource "*.dfm"
TfrZonaI *frZonaI;
AnsiString Zona;

// ---------------------------------------------------------------------------
__fastcall TfrZonaI::TfrZonaI(TComponent* Owner) : TFrame(Owner) {
    Zona = "I";
    AbilitaConferma = 1;
    FrameMatPrime->FrameEnter(this);
}

void TfrZonaI::AggiornaGriglia() {
    FrameMatPrime->FrameEnter(this);
}

void TfrZonaI::AggiornaDati() {
    int numeroelementi = 12;
    int idx = 0, trovato = 0; ;
    int pos;
    int numpostazioni = 10;
    AnsiString str;
    TPanel *Pan;
    TRecordList TabPosizioni;
    str = "select *, (SELECT COUNT(*) AS Expr1 FROM dbo.Piani_view AS Piani_1 WHERE (pos = dbo.Piani_View.Pos) AND (IDUDC <> 0)) as npianiocc from piani_view ";
    str += " WHERE Zona = '" + Zona + "' and piano= 1 ORDER BY Pos ";
    dmDB->FullTabella(str, TabPosizioni);
    for (int j = 1; j <= numeroelementi; j++) {
        Pan = (TPanel*) FindComponent("pnPos" + Zona + IntToStr(j));
        if (Pan != Null) {
            idx = 0;
            trovato = 0;

            while ((idx < TabPosizioni.size()) && (trovato == 0)) {
                pos = TabPosizioni[idx]["POS"].ToIntDef(0);
                if (Pan->Tag == pos) {
                    trovato = 1;
                    if (TabPosizioni[idx]["TIPOPOSIZIONE"].ToIntDef(0) == TIPOLOGIA_SCARTO) {
                        Pan->Color = clBtnFace;
                        Pan->Enabled = false;
                    }
                    else if (TabPosizioni[idx]["PRENOTATA"].ToIntDef(0) || TabPosizioni[idx]["POS_PRENOTATA"].ToIntDef(0)) {
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
                    if (TabPosizioni[idx]["TIPOPOSIZIONE"].ToIntDef(0) == TIPOLOGIA_SCARTO) {
                        Pan->Caption = "SCARTO";
                    }
                    else if (TabPosizioni[idx]["IDUDC"].ToIntDef(0) == 0) {
                        Pan->Caption = "Pos." + IntToStr((numeroelementi - j) + 1) + " (" + IntToStr(Pan->Tag) + ")";
                    }
                    else {
                        Pan->Caption = Trim( IntToStr((numeroelementi - j) + 1) + "-" + TabPosizioni[idx]["DESCUDC"]);
                    }
                    Pan->Hint = TabPosizioni[idx]["IDUDC"].ToIntDef(0);
                }
                if (!trovato)
                    idx++;
            }
        }
    }
}

void __fastcall TfrZonaI::pnPosIMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    TUDC UDC;
    TPanel *Pan;
    Pan = (TPanel*) Sender;
    if (Pan != NULL) {
        UDC.IDUDC = Pan->Hint.ToIntDef(0);
        leIdUDC->Text = UDC.IDUDC;
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
                // dmDB->ArticoloPrelevatoDepositato(Pan->Tag, 1, 1, dmDB->FilaPosizione(Pan->Tag));
                UDC.IDUDC = CercaConCodart(FrameMatPrime->ADOQuery1->FieldByName("componente")->AsString);
                if (UDC.IDUDC > 0) {
                    dmDB->ArticoloPrelevatoDepositato(Pan->Tag, UDC.IDUDC, 1, dmDB->FilaPosizione(Pan->Tag));
                }
            }
            else if (Pan->Color == clYellow) {
                dmDB->ArticoloPrelevatoDepositato(Pan->Tag, 0, 1, dmDB->FilaPosizione(Pan->Tag));
            }
            leIdUDC->Text = UDC.IDUDC;
        }

        // gestione udc/articoli
        if (UDC.IDUDC > 0) {
            // udc esiste
            dmDB->LeggiStrutturaUdc(UDC);
            // leIDArt->Text = UDC.Articolo.IDArticolo;
            leCodArt->Text = UDC.Articolo.CodArt;
            leDescArticolo->Text = UDC.Articolo.Descrizione;
        }
    }
}
// ---------------------------------------------------------------------------

int TfrZonaI::CercaConCodart(AnsiString CodArt) {
    int res = 0;
    TUDC UDC;
    UDC.IDUDC = dmDB->IDUDCdaCodart(FrameMatPrime->ADOQuery1->FieldByName("componente")->AsString);
    if (UDC.IDUDC == 0) {
        UDC.Articolo.IDArticolo = 0; // MainForm->trova_idarticolo;
        dmExtraFunction->StringToChar("", UDC.Lotto);
        dmExtraFunction->StringToChar(CodArt, UDC.Articolo.CodArt);
        dmExtraFunction->StringToChar(FrameMatPrime->ADOQuery1->FieldByName("desccrizione breve")->AsString, UDC.Articolo.Descrizione);
        UDC.CodTipoUDC = 0;
        UDC.IndiceImpilabilita = 0;
        UDC.Parziale = 0;
        UDC.Riservato = 0;
        UDC.CodStato = 0;
        UDC.IDUDC = dmDB->InsertUpdateUDC(UDC);
    }
    res = UDC.IDUDC;
    return res;
}
