// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ZonaA1Frame.h"
#include "DB.h"
#include "DBImpianto.h"
#include "main.h"
#include "anagrafica_articoli.h"
#include "ExtraFunction.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MyShape"
#pragma link "frame_ArticoliInLavorazione"
#pragma resource "*.dfm"
TfrZonaA1 *frZonaA1;
AnsiString Zona;

// ---------------------------------------------------------------------------
__fastcall TfrZonaA1::TfrZonaA1(TComponent* Owner) : TFrame(Owner) {
    Zona = "A";
    AbilitaConferma = 1;
    FrameArticoliInLavorazione->FrameEnter(this);

}

// ---------------------------------------------------------------------------

void TfrZonaA1::AggiornaGriglia() {
    FrameArticoliInLavorazione->FrameEnter(this);
}

void TfrZonaA1::AggiornaDati() {
    int numeroelementi = 7;
    int idx = 0, trovato = 0; ;
    int pos;
    AnsiString str;
    TPanel *Pan;
    TRecordList TabPosizioni;
    str = "select *, (SELECT COUNT(*) AS Expr1 FROM dbo.Piani AS Piani_1 WHERE (pos = dbo.Piani_View.Pos) AND (IDUDC <> 0)) as npianiocc from piani_view ";
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
                    else if (TabPosizioni[idx]["NPIANIOCC"].ToIntDef(0) == 1) {
                        Pan->Color = clYellow;
                    }
                    else if (TabPosizioni[idx]["NPIANIOCC"].ToIntDef(0) == 2) {
                        Pan->Color = clWebOrange;
                    }
                    else {
                        Pan->Color = clWhite;
                    }
                    // caption
                    // caption
                    if (TabPosizioni[idx]["IDUDC"].ToIntDef(0) == 0) {
                        Pan->Caption = "Pos." + IntToStr(j) + " (" + IntToStr(Pan->Tag) + ")";
                    }
                    else {
                        Pan->Caption = "Pos." + IntToStr(j) + " - " + IntToStr(TabPosizioni[idx]["IDUDC"].ToIntDef(0));
                    }
                    Pan->Hint = TabPosizioni[idx]["IDUDC"];

                }
                if (!trovato)
                    idx++;
            }
        }
    }
}

void __fastcall TfrZonaA1::pnPosAMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    TPanel *Pan;
    int pianodeposito;
    TUDC UDC;
    Pan = (TPanel*) Sender;
    if (Pan != NULL) {
        UDC.IDUDC = Pan->Hint.ToIntDef(0);
        leIdUDC->Text = UDC.IDUDC;
        if (Button == mbLeft) {
            if (Pan->Color == clLime) {
                dmDBImpianto->AggiornaSelezionePosizioni(Zona, Pan->Tag, 0);
            }
            else if (Pan->Color == clYellow) {
                // UDC.IDUDC = CercaConCodart(Pan->Hint.Trim());
                // escludo anche i vuoti
                if (UDC.IDUDC > 1) {
                    // dmDB->ArticoloPrelevatoDepositato(Pan->Tag, UDC.IDUDC, 1, dmDB->FilaPosizione(Pan->Tag));
                    dmDBImpianto->AggiornaSelezionePosizioni(Zona, Pan->Tag, 1);
                }

            }
        }

        if (Button == mbRight) {
            if (Pan->Color == clYellow) {
                // dmDB->ArticoloPrelevatoDepositato(Pan->Tag, 1, 1, dmDB->FilaPosizione(Pan->Tag));
                UDC.IDUDC = CercaConCodart(Pan->Tag - 100);
                if (UDC.IDUDC > 0) {
                    dmDB->ArticoloPrelevatoDepositato(Pan->Tag, UDC.IDUDC, 1, dmDB->FilaPosizione(Pan->Tag));
                }
            }
            leIdUDC->Text = UDC.IDUDC;
        }

        // gestione udc/articoli

        if (UDC.IDUDC > 0) {
            // udc esiste
            dmDB->LeggiStrutturaUdc(UDC);
            leIdUDC->Text = UDC.IDUDC;
            leCodArt->Text = UDC.Articolo.CodArt;
            leDescArticolo->Text = UDC.Articolo.Descrizione;
            ckImpilabile->Checked = UDC.IndiceImpilabilita; // da sistemare
        }
        else {
            leIdUDC->Text = "";
            leCodArt->Text = "";
            leDescArticolo->Text = "";
            ckImpilabile->Checked = false; // da sistemare
        }

    }
}

// ---------------------------------------------------------------------------

void __fastcall TfrZonaA1::pnPosADblClick(TObject *Sender) {
    TPanel *Pan;
    int pianodeposito;
    TUDC UDC;
    Pan = (TPanel*) Sender;
    if (Pan != NULL) {
        UDC.IDUDC = 0;
        leIdUDC->Text = UDC.IDUDC;
        if (Pan->Color == clYellow) {
            if (Application->MessageBox(L"Svuotare la posizione significa creare una missione di deposito pallet. Vuoi procedere?", L"Conferma", MB_YESNO) == IDYES)
                dmDB->ArticoloPrelevatoDepositato(Pan->Tag, 0, 1, dmDB->FilaPosizione(Pan->Tag));
        }
        else if (Pan->Color == clWhite) {
            dmDB->ArticoloPrelevatoDepositato(Pan->Tag, 1, 1, dmDB->FilaPosizione(Pan->Tag));
        }

        // gestione udc/articoli

        leIdUDC->Text = "0";
        leCodArt->Text = "";
        leDescArticolo->Text = "";
        ckImpilabile->Checked = false; // da sistemare

    }
}
// ---------------------------------------------------------------------------

int TfrZonaA1::CercaConCodart(int riga) {
    int res = 0;
    AnsiString Articolo;
    TUDC UDC;
    Articolo = dmDBImpianto->TornaCodartConRigaDaHMC_ORDINI_IN_LAVORAZIONE(riga, UDC);
    if (Articolo != "") {
        UDC.IDUDC = dmDB->IDUDCdaCodart(Articolo);
        if (UDC.IDUDC == 0) {
            UDC.Articolo.IDArticolo = 0; // MainForm->trova_idarticolo;
            dmExtraFunction->StringToChar("", UDC.Lotto);
            // dmExtraFunction->StringToChar(Articolo, UDC.Articolo.CodArt);
            // dmExtraFunction->StringToChar(FrameArticoliInLavorazione->ADOQuery1->FieldByName("Descrizione Articolo")->AsString, UDC.Articolo.Descrizione);
            UDC.CodTipoUDC = 0;
            // UDC.IndiceImpilabilita = 1; //FrameArticoliInLavorazione->ADOQuery1->FieldByName("sovrapposto")->AsInteger;
            UDC.Parziale = 0;
            UDC.Riservato = 0;
            UDC.CodStato = 0;
            UDC.IDUDC = dmDB->InsertUpdateUDC(UDC);
        }
        res = UDC.IDUDC;
    }
    return res;
}
