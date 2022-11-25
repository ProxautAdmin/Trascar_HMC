// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ZonaHFrame.h"
#include "DB.h"
#include "DBImpianto.h"
#include "main.h"
#include "ExtraFunction.h"
#include "frame_ArticoliSovrapposti.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MyShape"
#pragma link "frame_ArticoliSovrapposti"
#pragma resource "*.dfm"
TfrZonaH *frZonaH;
AnsiString Zona;

// ---------------------------------------------------------------------------
__fastcall TfrZonaH::TfrZonaH(TComponent* Owner) : TFrame(Owner) {
    Zona = "H";
    AbilitaConferma = 1;
    FrameSovrapposti->FrameEnter(this);
}

// ---------------------------------------------------------------------------

void TfrZonaH::AggiornaDati() {
    int numeroelementi = 4;
    int idx = 0, trovato = 0; ;
    int pos;
    AnsiString str;
    TPanel *Pan;
    TRecordList TabPosizioni;
    // sarebbe da farsi  con posizioni ma ok
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

// ---------------------------------------------------------------------------

void __fastcall TfrZonaH::pnPosH1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    TUDC UDC;
    TPanel *Pan;
    Pan = (TPanel*) Sender;

    if (Pan != NULL) {
        if (Button == mbLeft) {
            UDC.IDUDC = Pan->Hint.ToIntDef(0);
            leIdUDC->Text = UDC.IDUDC;
            if (Pan->Color == clLime) {
                dmDBImpianto->AggiornaSelezionePosizioni(Zona, Pan->Tag, 0);
                // UDC.IDUDC = 0;
            }
            else if ((Pan->Color == clWebOrange) || (Pan->Color == clYellow)) {
               // UDC.IDUDC = CercaConCodart(Pan->Hint.Trim());
                // if (UDC.IDUDC > 0) {
                // dmDB->ArticoloPrelevatoDepositato(Pan->Tag, UDC.IDUDC, 1, dmDB->FilaPosizione(Pan->Tag));
                // }
                dmDBImpianto->AggiornaSelezionePosizioni(Zona, Pan->Tag, 1);
            }
            else {
                FillDescrizione(UDC.IDUDC);
            }

        }
        if (Button == mbRight) {
            PopupMenu->Items->Clear();
            TMenuItem *NewItem1 = new TMenuItem(this); // create the separator
            NewItem1->Caption = "Carica 1 UDC in posizione";
            NewItem1->Tag = 1;
            NewItem1->Hint = Pan->Tag;
            // ImageListMenu->Draw(NewItem1->Bitmap->Canvas, X + Pan->Left, Y + Pan->Height, 1); // non va
            NewItem1->OnClick = Carica1Click;
            PopupMenu->Items->Add(NewItem1);
            //
            TMenuItem *NewItem2 = new TMenuItem(this); // create the separator
            NewItem2->Caption = "Carica 2 UDC in posizione";
            NewItem2->Tag = 2;
            NewItem2->Hint = Pan->Tag;
            NewItem2->OnClick = Carica2Click;
            PopupMenu->Items->Add(NewItem2);
            TMenuItem *NewItem3 = new TMenuItem(this); // create the separator
            NewItem3->Caption = "Svuota Posizione";
            NewItem3->Tag = 3;
            NewItem3->Hint = Pan->Tag;
            NewItem3->OnClick = SvuotaPosizioneClick;
            PopupMenu->Items->Add(NewItem3);

            PopupMenu->Popup(X + Pan->Left, Y + Pan->Height);
            /*
             if (Pan->Color == clWhite) {
             dmDB->ArticoloPrelevatoDepositato(Pan->Tag, 1, 1, dmDB->FilaPosizione(Pan->Tag));
             }
             else if (Pan->Color == clYellow) {
             dmDB->ArticoloPrelevatoDepositato(Pan->Tag, 0, 1, dmDB->FilaPosizione(Pan->Tag));
             }
             */

        }

        // gestione udc/articoli
        FillDescrizione(UDC.IDUDC);

    }
}
// ---------------------------------------------------------------------------

void __fastcall TfrZonaH::Carica1Click(TObject * Sender) {
    //
    TUDC UDC;
    AnsiString s;
    TMenuItem *Item = (TMenuItem*) Sender;
    if (Item != NULL) {
        UDC.IDUDC = CercaConCodart(FrameSovrapposti->ADOQuery1->FieldByName("distinta")->AsString);
        if (UDC.IDUDC > 0) {
            dmDBImpianto->AggiornaUDCPosizioni(Item->Hint.ToIntDef(0), UDC.IDUDC, 1);
        }
        FillDescrizione(UDC.IDUDC);
    }
}

void __fastcall TfrZonaH::Carica2Click(TObject * Sender) {
    //
    TUDC UDC;
    AnsiString s;
    TMenuItem *Item = (TMenuItem*) Sender;
    if (Item != NULL) {
        UDC.IDUDC = CercaConCodart(FrameSovrapposti->ADOQuery1->FieldByName("distinta")->AsString);
        if (UDC.IDUDC > 0) {
            dmDBImpianto->AggiornaUDCPosizioni(Item->Hint.ToIntDef(0), UDC.IDUDC, 2);
        }
        FillDescrizione(UDC.IDUDC);
    }
}

void __fastcall TfrZonaH::SvuotaPosizioneClick(TObject * Sender) {
    //
    AnsiString s;
    TMenuItem *Item = (TMenuItem*) Sender;
    if (Item != NULL) {
        if (Item->Hint.ToIntDef(0) > 0) {
            if (Application->MessageBox(L"Sei sicuro di voler svuotare una posizione piena?", L"Conferma", MB_YESNO) == IDYES) {
                dmDBImpianto->AggiornaUDCPosizioni(Item->Hint.ToIntDef(0), 0);
                FillDescrizione(0);
                dmDB->LogMsg("Svuotata pos. : " + IntToStr(Item->Hint.ToIntDef(0)));
            }
        }
    }
}

int TfrZonaH::CercaConCodart(AnsiString CodArt) {
    int res = 0;
    TUDC UDC;
    UDC.IDUDC = dmDB->IDUDCdaCodart(FrameSovrapposti->ADOQuery1->FieldByName("distinta")->AsString);
    if (UDC.IDUDC == 0) {
        UDC.Articolo.IDArticolo = 0; // MainForm->trova_idarticolo;
        dmExtraFunction->StringToChar("", UDC.Lotto);
        dmExtraFunction->StringToChar(CodArt, UDC.Articolo.CodArt);
        dmExtraFunction->StringToChar(FrameSovrapposti->ADOQuery1->FieldByName("Descrizione distinta")->AsString, UDC.Articolo.Descrizione);
        UDC.CodTipoUDC = 0;
        UDC.IndiceImpilabilita = 1;
        UDC.Parziale = 0;
        UDC.Riservato = 0;
        UDC.CodStato = 0;
        UDC.IDUDC = dmDB->InsertUpdateUDC(UDC);
    }
    res = UDC.IDUDC;
    return res;
}

// ---------------------------------------------------------------------------

int TfrZonaH::FillDescrizione(int idudc) {
    TUDC UDC;
    UDC.IDUDC = idudc;
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
