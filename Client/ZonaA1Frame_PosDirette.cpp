// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ZonaA1Frame_PosDirette.h"
#include "DB.h"
#include "DBImpianto.h"
#include "main.h"
#include "anagrafica_articoli.h"
#include "ExtraFunction.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MyShape"
#pragma resource "*.dfm"
TfrZonaA1PosDirette *frZonaA1PosDirette;
AnsiString Zona;

// ---------------------------------------------------------------------------
__fastcall TfrZonaA1PosDirette::TfrZonaA1PosDirette(TComponent* Owner) : TFrame(Owner) {
    Zona = "A";
    AbilitaConferma = 1;

}

// ---------------------------------------------------------------------------

void TfrZonaA1PosDirette::AggiornaDati() {
    int numeroelementi = 7;
    int idx = 0, trovato = 0; ;
    int pos;
    AnsiString str;
    TPanel *Pan;
    TRecordList TabPosizioni;
    str = "select POS,PRENOTATA,POS_PRENOTATA, DISABILITATA, POS_DISABILITA, SELEZIONATA, ARTICOLO from ZonaA1 ";
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
                    if (TabPosizioni[idx]["PRENOTATA"].ToIntDef(0) || TabPosizioni[idx]["POS_PRENOTATA"].ToIntDef(0)) {
                        Pan->Color = clBlue;
                    }
                    else if (TabPosizioni[idx]["DISABILITATA"].ToIntDef(0) || TabPosizioni[idx]["POS_DISABILITA"].ToIntDef(0)) {
                        Pan->Color = clRed;
                    }
                    else if (TabPosizioni[idx]["SELEZIONATA"].ToIntDef(0)) {
                        Pan->Color = clLime;
                    }
                    else if (TabPosizioni[idx]["ARTICOLO"] != "") {
                        Pan->Color = clYellow;
                    }
                    else {
                        Pan->Color = clWhite;
                    }
                    // caption
                    if (TabPosizioni[idx]["ARTICOLO"] == "") {
                        Pan->Caption = "Pos." + IntToStr(j) + " (" + IntToStr(Pan->Tag) + ")";

                    }
                    else {
                        Pan->Caption = TabPosizioni[idx]["ARTICOLO"];

                    }

                    Pan->Hint = TabPosizioni[idx]["ARTICOLO"];
                }
                if (!trovato)
                    idx++;
            }
        }
    }
}

void __fastcall TfrZonaA1PosDirette::pnPosAMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    TPanel *Pan;
    int pianodeposito;
    TUDC UDC;
    Pan = (TPanel*) Sender;
    if (Pan != NULL) {
        // UDC.IDUDC = Pan->Hint.ToIntDef(0);
        // leIdUDC->Text = UDC.IDUDC;
        // gestione tasto mause
        if (Button == mbLeft) {
            if (Pan->Color == clLime) {
                dmDBImpianto->AggiornaSelezionePosizioni(Zona, Pan->Tag, 0);
            }
            else if (Pan->Color == clYellow) {
                UDC.IDUDC = CercaConCodart(Pan->Hint.Trim());
                if (UDC.IDUDC > 0) {
                    dmDB->ArticoloPrelevatoDepositato(Pan->Tag, UDC.IDUDC, 1, dmDB->FilaPosizione(Pan->Tag));
                }
                dmDBImpianto->AggiornaSelezionePosizioni(Zona, Pan->Tag, 1);
            }
        }

        if (Button == mbRight) {
            if (Pan->Color == clWhite) {
                UDC.IDUDC = 0;
            }
            else if ((Pan->Color == clYellow) || (Pan->Color == clLime)) {
                UDC.IDUDC = CercaConCodart(Pan->Hint.Trim());
            }

        }

        // gestione udc/articoli

        if (UDC.IDUDC > 0) {
            // udc esiste
            dmDB->LeggiStrutturaUdc(UDC);
            leIdUDC->Text = UDC.IDUDC;
            leCodArt->Text = UDC.Articolo.CodArt;
            leDescArticolo->Text = UDC.Articolo.Descrizione;
            ckImpilabile->Checked = UDC.IndiceImpilabilita;     //da sistemare
        }
        else {
            leIdUDC->Text = "";
            leCodArt->Text = "";
            leDescArticolo->Text = "";
            ckImpilabile->Checked = false;       //da sistemare
        }

    }
}

// ---------------------------------------------------------------------------
void __fastcall TfrZonaA1PosDirette::sbCercaClick(TObject *Sender) {
    TUDC UDC;
    FormAnagraficaArticoli->insert_produzione = true;
    FormAnagraficaArticoli->edEdIDArt->Text = tDescArticolo->Text;
    FormAnagraficaArticoli->ShowModal();
    if (MainForm->trova_idarticolo != edIDArt->Text.ToIntDef(0)) {
        edIDArt->Text = MainForm->trova_idarticolo;
        UDC.IDUDC = dmDB->IDUDCdaIDArticolo(edIDArt->Text.Trim().ToIntDef(0));
        if (UDC.IDUDC == 0) {
            UDC.Articolo.IDArticolo = MainForm->trova_idarticolo;
            dmExtraFunction->StringToChar("", UDC.Lotto);
            UDC.CodTipoUDC = 0;
            UDC.IndiceImpilabilita = 0;
            UDC.Parziale = 0;
            UDC.Riservato = 0;
            UDC.IDUDC = dmDB->InsertUpdateUDC(UDC);
        }
        // zona cerca
        dmDB->LeggiStrutturaUdc(UDC);
        lIdUDC->Caption = UDC.IDUDC;
        edIDArt->Text = UDC.Articolo.IDArticolo;
        edCodArt->Text = UDC.Articolo.CodArt;
        tDescArticolo->Text = UDC.Articolo.Descrizione;
        ckImpilabile->Checked = UDC.IndiceImpilabilita;

    }
}

// ---------------------------------------------------------------------------

int TfrZonaA1PosDirette::CercaConCodart(AnsiString CodArt) {
    // sarebbe da fare una funzione unica con zona I. Ora non ho tempo
    int res = 0;
    TUDC UDC;
    UDC.IDUDC = dmDB->IDUDCdaCodart(CodArt);
    if (UDC.IDUDC == 0) {
        UDC.Articolo.IDArticolo = 0; // MainForm->trova_idarticolo;
        dmExtraFunction->StringToChar("", UDC.Lotto);
        dmExtraFunction->StringToChar(CodArt, UDC.Articolo.CodArt);
        dmExtraFunction->StringToChar(dmDBImpianto->TornaDescrizionedaZonaA1(CodArt, UDC.IndiceImpilabilita), UDC.Articolo.Descrizione);
        UDC.CodTipoUDC = 0;
        // UDC.IndiceImpilabilita = 0;  vedi funzione
        UDC.Parziale = 0;
        UDC.Riservato = 0;
        UDC.CodStato = 0;
        UDC.IDUDC = dmDB->InsertUpdateUDC(UDC);
    }
    res = UDC.IDUDC;
    return res;
}
