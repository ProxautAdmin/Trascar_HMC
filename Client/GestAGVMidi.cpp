// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GestAGVMidi.h"
#include "DB.h"
#include "ZonaA1Frame.h"
#include "ZonaA2Frame.h"
#include "ZonaGFrame.h"
#include "ZonaHFrame.h"
#include "ZonaIFrame.h"
#include "ZonaDFrame.h"
#include "ZonaCFrame.h"
#include "ZonaBFrame.h"
#include "ZonaEFrame.h"
#include "ZonaFFrame.h"
#include "main.h"
#include "DBClient.h"
#include "ExtraFunction.h"
#include "DBImpianto.h"
#include "SocketComunicazioneClient.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma link "cspin"
#pragma link "ZonaA1Frame"
#pragma link "ZonaHFrame"
#pragma resource "*.dfm"
TfGestAGVMidi *fGestAGVMidi;
TFontStyles AStyle(1);
TSpeedButton *TSBut;
TfrZonaA1 *frA1;
TfrZonaA2 *frA2;
TfrZonaG *frG;
TfrZonaH *frH;
TfrZonaI *frI;
TfrZonaD *frD;
TfrZonaC *frC;
TfrZonaB *frB;
TfrZonaE *frE;
TfrZonaF *frF;

// ---------------------------------------------------------------------------
__fastcall TfGestAGVMidi::TfGestAGVMidi(TComponent* Owner)
    : TMDIChild(Owner)
{

}
// ---------------------------------------------------------------------------

void __fastcall TfGestAGVMidi::FormActivate(TObject *Sender) {
    int numpos, ttop, lleft, calcpos, contafile, ordinamento, poscalcolata;
    int numdestperfila = 7;

    // crea e gestisci i frame
    posselezionata = 0;
    destselezionata = 0;
    ttop = (MainForm->ScrollBox1->Height - 260 - 280 - PanelDest->Top - PanelTitle->Height) / 2; // -altezza frame-altezza panel4 fa un po' cagare
    ttop = 10;
    pos = MainForm->pos_udc;
    PanelPrel->Height = gbMain->Height / 2;
    PanelPrel->Top = 0;
    PanelPrel->Left = 0;
    dmExtraFunction->ComboScelte(cbPriorita, 9, 1, 1);

    for (int i = 1; i <= 2; i++) {
        // A1
        if (i == 1)
            frA1 = (TfrZonaA1*)(FindComponent("frZonaA1Prel"));
        else
            frA1 = (TfrZonaA1*)(FindComponent("frZonaA1Dest"));
        if (frA1 == NULL) {
            frA1 = new TfrZonaA1(this);
            if (i == 1) {
                frA1->Parent = this->tsZonaA1Prel;
                frA1->Name = "frZonaA1Prel";
            }
            else {
                frA1->Parent = this->tsZonaA1Dest;
                frA1->Name = "frZonaA1Dest";
            }
        }
        // A2
        if (i == 1)
            frA2 = (TfrZonaA2*)(FindComponent("frZonaA2Prel"));
        else
            frA2 = (TfrZonaA2*)(FindComponent("frZonaA2Dest"));
        if (frA2 == NULL) {
            frA2 = new TfrZonaA2(this);
            if (i == 1) {
                frA2->Parent = this->tsZonaA2Prel;
                frA2->Name = "frZonaA2Prel";
            }
            else {
                frA2->Parent = this->tsZonaA2Dest;
                frA2->Name = "frZonaA2Dest";
            }
        }
        // G
        if (i == 1)
            frG = (TfrZonaG*)(FindComponent("frZonaGPrel"));
        else
            frG = (TfrZonaG*)(FindComponent("frZonaGDest"));
        if (frG == NULL) {
            frG = new TfrZonaG(this);
            if (i == 1) {
                frG->Parent = this->tsZonaGPrel;
                frG->Name = "frZonaGPrel";
            }
            else {
                frG->Parent = this->tsZonaGDest;
                frG->Name = "frZonaGDest";
            }
        }
        // H
        if (i == 1)
            frH = (TfrZonaH*)(FindComponent("frZonaHPrel"));
        else
            frH = (TfrZonaH*)(FindComponent("frZonaHDest"));
        if (frH == NULL) {
            frH = new TfrZonaH(this);
            if (i == 1) {
                frH->Parent = this->tsZonaHPrel;
                frH->Name = "frZonaHPrel";
            }
            else {
                frH->Parent = this->tsZonaHDest;
                frH->Name = "frZonaHDest";
            }
        }
        // I
        if (i == 1)
            frI = (TfrZonaI*)(FindComponent("frZonaIPrel"));
        else
            frI = (TfrZonaI*)(FindComponent("frZonaIDest"));
        if (frI == NULL) {
            frI = new TfrZonaI(this);
            if (i == 1) {
                frI->Parent = this->tsZonaIPrel;
                frI->Name = "frZonaIPrel";
            }
            else {
                frI->Parent = this->tsZonaIDest;
                frI->Name = "frZonaIDest";
            }
        }
        // D
        if (i == 1)
            frD = (TfrZonaD*)(FindComponent("frZonaDPrel"));
        else
            frD = (TfrZonaD*)(FindComponent("frZonaDDest"));
        if (frD == NULL) {
            frD = new TfrZonaD(this);
            if (i == 1) {
                frD->Parent = this->tsZonaDPrel;
                frD->Name = "frZonaDPrel";
            }
            else {
                frD->Parent = this->tsZonaDDest;
                frD->Name = "frZonaDDest";
            }
        }
        // C
        if (i == 1)
            frC = (TfrZonaC*)(FindComponent("frZonaCPrel"));
        else
            frC = (TfrZonaC*)(FindComponent("frZonaCDest"));
        if (frC == NULL) {
            frC = new TfrZonaC(this);
            if (i == 1) {
                frC->Parent = this->tsZonaCPrel;
                frC->Name = "frZonaCPrel";
            }
            else {
                frC->Parent = this->tsZonaCDest;
                frC->Name = "frZonaCDest";
            }
            frC->Enabled = false;
        }
        // B
        if (i == 1)
            frB = (TfrZonaB*)(FindComponent("frZonaBPrel"));
        else
            frB = (TfrZonaB*)(FindComponent("frZonaBDest"));
        if (frB == NULL) {
            frB = new TfrZonaB(this);
            if (i == 1) {
                frB->Parent = this->tsZonaBPrel;
                frB->Name = "frZonaBPrel";
            }
            else {
                frB->Parent = this->tsZonaBDest;
                frB->Name = "frZonaBDest";
            }
            frB->Enabled = false;
        }
        // E
        if (i == 1)
            frE = (TfrZonaE*)(FindComponent("frZonaEPrel"));
        else
            frE = (TfrZonaE*)(FindComponent("frZonaEDest"));
        if (frE == NULL) {
            frE = new TfrZonaE(this);
            if (i == 1) {
                frE->Parent = this->tsZonaEPrel;
                frE->Name = "frZonaEPrel";
            }
            else {
                frE->Parent = this->tsZonaEDest;
                frE->Name = "frZonaEDest";
            }
            frE->Enabled = false;
        }
        // F
        if (i == 1)
            frF = (TfrZonaF*)(FindComponent("frZonaFPrel"));
        else
            frF = (TfrZonaF*)(FindComponent("frZonaFDest"));
        if (frF == NULL) {
            frF = new TfrZonaF(this);
            if (i == 1) {
                frF->Parent = this->tsZonaFPrel;
                frF->Name = "frZonaFPrel";
            }
            else {
                frF->Parent = this->tsZonaFDest;
                frF->Name = "frZonaFDest";
            }
        }
    }

    AggiornaDati();

    pcPrel->TabIndex = 0;
    TimerRef->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TfGestAGVMidi::TimerRefTimer(TObject * Sender) {
    AnsiString ts;
    TimerRef->Enabled = false;

    // NB ottimizzare, non c'e' bisogno di un tipo diverso TfrZonaxx
    // prel
    if (pcPrel->TabIndex == 0) {
        TabVisibili(0, 0, 0, 1, 0, 0, 0, 0, 1);
        frA1 = (TfrZonaA1*)(FindComponent("frZonaA1Prel"));
        frA1->AggiornaDati();
        btConferma->Enabled = frA1->AbilitaConferma;
    }
    else if (pcPrel->TabIndex == 1) {
        TabVisibili(0, 0, 1, 0, 1, 0, 0, 0, 0);
        frA2 = (TfrZonaA2*)(FindComponent("frZonaA2Prel"));
        frA2->AggiornaDati();
        btConferma->Enabled = frA1->AbilitaConferma;
    }
    else if (pcPrel->TabIndex == 2) {
        TabVisibili(1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        frG = (TfrZonaG*)(FindComponent("frZonaGPrel"));
        frG->AggiornaDati();
        btConferma->Enabled = frG->AbilitaConferma;
    }
    else if (pcPrel->TabIndex == 3) {
        frH = (TfrZonaH*)(FindComponent("frZonaHPrel"));
        frH->AggiornaDati();
        btConferma->Enabled = frH->AbilitaConferma;
    }
    else if (pcPrel->TabIndex == 4) {
        TabVisibili(0, 1, 0, 0, 0, 0, 0, 0, 0, 0);
        frI = (TfrZonaI*)(FindComponent("frZonaIPrel"));
        frI->AggiornaDati();
        btConferma->Enabled = frI->AbilitaConferma;
    }
    else if (pcPrel->TabIndex == 5) {
        frD = (TfrZonaD*)(FindComponent("frZonaDPrel"));
        frD->AggiornaDati();
        btConferma->Enabled = frD->AbilitaConferma;
    }
    else if (pcPrel->TabIndex == 6) {
        TabVisibili(0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
        frC = (TfrZonaC*)(FindComponent("frZonaCPrel"));
        frC->AggiornaDati();
        btConferma->Enabled = frC->AbilitaConferma;
    }
    else if (pcPrel->TabIndex == 7) {
        TabVisibili(0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
        frB = (TfrZonaB*)(FindComponent("frZonaBPrel"));
        frB->AggiornaDati();
        btConferma->Enabled = frB->AbilitaConferma;
    }
    else if (pcPrel->TabIndex == 8) {
        TabVisibili(0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
        frE = (TfrZonaE*)(FindComponent("frZonaEPrel"));
        frE->AggiornaDati();
        btConferma->Enabled = frE->AbilitaConferma;
    }
    else if (pcPrel->TabIndex == 9) {
        TabVisibili(0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
        frF = (TfrZonaF*)(FindComponent("frZonaFPrel"));
        frF->AggiornaDati();
        btConferma->Enabled = frF->AbilitaConferma;
    }

    // dest
    if (pcDest->ActivePage->Hint == "A") {
        frA1 = (TfrZonaA1*)(FindComponent("frZonaA1Dest"));
        frA1->AggiornaDati();
    }
    else if (pcDest->ActivePage->Hint == "J") {
        frA2 = (TfrZonaA2*)(FindComponent("frZonaA2Dest"));
        frA2->AggiornaDati();
    }
    else if (pcDest->ActivePage->Hint == "G") {
        frG = (TfrZonaG*)(FindComponent("frZonaGDest"));
        frG->AggiornaDati();
    }
    else if (pcDest->ActivePage->Hint == "H") {
        frH = (TfrZonaH*)(FindComponent("frZonaHDest"));
        frH->AggiornaDati();
    }
    else if (pcDest->ActivePage->Hint == "I") {
        frI = (TfrZonaI*)(FindComponent("frZonaIDest"));
        frI->AggiornaDati();
    }
    else if (pcDest->ActivePage->Hint == "D") {
        frD = (TfrZonaD*)(FindComponent("frZonaDDest"));
        frD->AggiornaDati();
    }
    else if (pcDest->ActivePage->Hint == "C") {
        frC = (TfrZonaC*)(FindComponent("frZonaCDest"));
        frC->AggiornaDati();
    }
    else if (pcDest->ActivePage->Hint == "B") {
        frB = (TfrZonaB*)(FindComponent("frZonaBDest"));
        frB->AggiornaDati();
    }
    else if (pcDest->ActivePage->Hint == "E") {
        frE = (TfrZonaE*)(FindComponent("frZonaEDest"));
        frE->AggiornaDati();
    }
    else if (pcDest->ActivePage->Hint == "F") {
        frF = (TfrZonaF*)(FindComponent("frZonaFDest"));
        frF->AggiornaDati();
    }

    TimerRef->Enabled = true;
}
// ---------------------------------------------------------------------------

void TfGestAGVMidi::TabVisibili(bool tab0, bool tab1, bool tab2, bool tab3, bool tab4, bool tab5, bool tab6, bool tab7, bool tab8, bool tab9) {
    TTabSheet *Tab;

    Tab = (TTabSheet*)(FindComponent("tsZonaA1Dest"));
    if (Tab != NULL)
        Tab->TabVisible = tab0;

    Tab = (TTabSheet*)(FindComponent("tsZonaA2Dest"));
    if (Tab != NULL)
        Tab->TabVisible = tab1;

    Tab = (TTabSheet*)(FindComponent("tsZonaGDest"));
    if (Tab != NULL)
        Tab->TabVisible = tab2;

    Tab = (TTabSheet*)(FindComponent("tsZonaHDest"));
    if (Tab != NULL)
        Tab->TabVisible = tab3;

    Tab = (TTabSheet*)(FindComponent("tsZonaIDest"));
    if (Tab != NULL)
        Tab->TabVisible = tab4;

    Tab = (TTabSheet*)(FindComponent("tsZonaDDest"));
    if (Tab != NULL)
        Tab->TabVisible = tab5;

    Tab = (TTabSheet*)(FindComponent("tsZonaCDest"));
    if (Tab != NULL)
        Tab->TabVisible = tab6;

    Tab = (TTabSheet*)(FindComponent("tsZonaBDest"));
    if (Tab != NULL)
        Tab->TabVisible = tab7;

    Tab = (TTabSheet*)(FindComponent("tsZonaEDest"));
    if (Tab != NULL)
        Tab->TabVisible = tab8;

    Tab = (TTabSheet*)(FindComponent("tsZonaFDest"));
    if (Tab != NULL)
        Tab->TabVisible = tab9;

}

void TfGestAGVMidi::AggiornaDati() {

}

void __fastcall TfGestAGVMidi::BitBtnCloseClick(TObject * Sender)
{
    dmDB->SettaPosSelezionata(pos, 0, zona);
    // delete fGestAGVMidi;
    Close();

}
// ---------------------------------------------------------------------------

void __fastcall TfGestAGVMidi::btConfermaClick(TObject * Sender) {
    TCentroMissione CentroMis;
    TADOQuery *ADOQuery;
    AnsiString strsql, ZonaPrel, ZonaDep;
    bool chiudi = true;
    int posprel = 0, pianoprel = 0, posdep = 0, pianodep = 0, tipoposizione, idudc;

    // prel
    ZonaPrel = pcPrel->Pages[pcPrel->TabIndex]->Hint;
    TornaPosPrelSelezionata(ZonaPrel, posprel, pianoprel, tipoposizione, idudc);

    // dep
    ZonaDep = pcDest->ActivePage->Hint;
    if ((ZonaPrel == "A") && (ZonaDep == "H"))
        dmDBImpianto->TornaPosDepLiberaH(ZonaDep, idudc, posdep, pianodep);
    else if ((ZonaPrel == "J") && (ZonaDep == "I") && (tipoposizione == TIPOLOGIA_SCARTO))
        dmDBImpianto->TornaPosDepLibera(ZonaDep, posdep, pianodep, TIPOLOGIA_SCARTO);
    else if ((ZonaPrel == "J") && (ZonaDep == "G"))
        dmDBImpianto->TornaPosDepLibera(ZonaDep, posdep, pianodep, TIPOLOGIA_PALLET);
    else if ((ZonaPrel == "I") && (ZonaDep == "J"))
        dmDBImpianto->TornaPosDepLibera(ZonaDep, posdep, pianodep, TIPOLOGIA_MATERIEPRIME);
    else
        dmDBImpianto->TornaPosDepLibera(ZonaDep, posdep, pianodep);

    // genera cmissione
    if ((posprel > 0) && (posdep > 0) && (pianoprel > 0) && (pianodep > 0)) {
        CentroMis.posprel = posprel;
        CentroMis.posdep = posdep;
        CentroMis.pianoprel = pianoprel;
        CentroMis.pianodep = pianodep;
        CentroMis.h_prel = dmDB->RitornaAltezzedaPosizione(CentroMis.posprel, CentroMis.pianoprel, "HPREL");
        CentroMis.h_dep = dmDB->RitornaAltezzedaPosizione(CentroMis.posdep, CentroMis.pianodep, "HDEP");
        CentroMis.TipoMissione = 0;
        CentroMis.CodTipoMovimento = 0;
        CentroMis.CodTipoMissione = 0;
        CentroMis.Priorita = cbPriorita->Text.ToIntDef(1);
        CentroMis.Agv = 1;
        CentroMis.IDUDC = dmDB->RitornaUDCdaPosPiano(CentroMis.posprel, CentroMis.pianoprel);
        CentroMis.CorsiaDeposito = "";
        CentroMis.stato = 0;
        CentroMis.FilaInCorsiaDeposito = 0;
        CentroMis.TipoUDC = 0;
        CentroMis.DestinazioneModuli = 0;
        dmDB->GeneraCentroMissione(CentroMis);
        // tolgo prenotazioni
        dmDBImpianto->AggiornaSelezionePosizioni(ZonaPrel, 0, 0);
        dmDBImpianto->AggiornaSelezionePosizioni(ZonaDep, 0, 0);
    }
    Close();

}

// ---------------------------------------------------------------------------

void TfGestAGVMidi::TornaPosPrelSelezionata(AnsiString Zona, int &pos, int &piano, int &tipoposizione, int &idudc) {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;
    pos = 0;
    piano = 0;
    tipoposizione = 0;
    idudc = 0;

    AnsiString codudc = "";
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql.printf("Select idudc, pos, piano, tipoposizione from piani_view where selezionata>0 and zona='%s' ", Zona);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        if (ADOQuery->RecordCount) {
            idudc = ADOQuery->FieldByName("idudc")->AsInteger;
            pos = ADOQuery->FieldByName("pos")->AsInteger;
            piano = ADOQuery->FieldByName("piano")->AsInteger;
            tipoposizione = ADOQuery->FieldByName("tipoposizione")->AsInteger;
        }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return;
}

void __fastcall TfGestAGVMidi::FormDestroy(TObject * Sender)
{
    int numpos;
    /*
     TFGestAGVFrame *frLoad;

     numpos = dmDB->RitornaNumPosDaPos(pos);

     for (int i = 1; i <= numpos; i++) {
     frLoad = (TFGestAGVFrame*)(FindComponent("FGestAGVFrame" + IntToStr(i)));
     if (frLoad != NULL) {
     delete frLoad;
     }
     }
     */
}

// ---------------------------------------------------------------------------
