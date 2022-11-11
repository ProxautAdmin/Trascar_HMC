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
    TTabSheet *Tab;
    int numpos, ttop, lleft, calcpos, contafile, ordinamento, poscalcolata;
    int numdestperfila = 7;
    AnsiString Zona;

    // crea e gestisci i frame
    posselezionata = 0;
    destselezionata = 0;
    impila = 0;
    ttop = (MainForm->ScrollBox1->Height - 260 - 280 - PanelDest->Top - PanelTitle->Height) / 2; // -altezza frame-altezza panel4 fa un po' cagare
    ttop = 10;
    pos = MainForm->pos_udc;
    // PanelPrel->Height = gbMain->Height / 2;
    PanelPrel->Top = 0;
    PanelPrel->Left = 0;
    dmExtraFunction->ComboScelte(cbPriorita, 9, 3, 1);

    // PREL
    // A1
    if (dmDBImpianto->CheckZonaUtente("A") > 0) {
        frA1 = (TfrZonaA1*)(FindComponent("frZonaA1Prel"));
        if (frA1 == NULL) {
            frA1 = new TfrZonaA1(this);
            frA1->Parent = this->tsZonaA1Prel;
            frA1->Name = "frZonaA1Prel";
        }
    }
    else {
        Tab = (TTabSheet*)(FindComponent("tsZonaA1Prel"));
        if (Tab != NULL)
            Tab->TabVisible = false;
    }
    // A2
    if (dmDBImpianto->CheckZonaUtente("J") > 0) {
        frA2 = (TfrZonaA2*)(FindComponent("frZonaA2Prel"));
        if (frA2 == NULL) {
            frA2 = new TfrZonaA2(this);
            frA2->Parent = this->tsZonaA2Prel;
            frA2->Name = "frZonaA2Prel";
        }
    }
    else {
        Tab = (TTabSheet*)(FindComponent("tsZonaA2Prel"));
        if (Tab != NULL)
            Tab->TabVisible = false;
    }
    // G
    Zona = "G";
    if (dmDBImpianto->CheckZonaUtente(Zona) > 0) {
        frG = (TfrZonaG*)(FindComponent("frZonaGPrel"));
        if (frG == NULL) {
            frG = new TfrZonaG(this);
            frG->Parent = this->tsZonaGPrel;
            frG->Name = "frZonaGPrel";
        }
    }
    else {
        Tab = (TTabSheet*)(FindComponent("tsZona" + Zona + "Prel"));
        if (Tab != NULL)
            Tab->TabVisible = false;
    }
    // H
    Zona = "H";
    if (dmDBImpianto->CheckZonaUtente(Zona) > 0) {
        frH = (TfrZonaH*)(FindComponent("frZonaHPrel"));
        if (frH == NULL) {
            frH = new TfrZonaH(this);
            frH->Parent = this->tsZonaHPrel;
            frH->Name = "frZonaHPrel";
        }
    }
    else {
        Tab = (TTabSheet*)(FindComponent("tsZona" + Zona + "Prel"));
        if (Tab != NULL)
            Tab->TabVisible = false;
    }
    // I
    Zona = "I";
    if (dmDBImpianto->CheckZonaUtente(Zona) > 0) {
        frI = (TfrZonaI*)(FindComponent("frZonaIPrel"));
        if (frI == NULL) {
            frI = new TfrZonaI(this);
            frI->Parent = this->tsZonaIPrel;
            frI->Name = "frZonaIPrel";
        }
    }
    else {
        Tab = (TTabSheet*)(FindComponent("tsZona" + Zona + "Prel"));
        if (Tab != NULL)
            Tab->TabVisible = false;
    }
    // D
    Zona = "D";
    if (dmDBImpianto->CheckZonaUtente(Zona) > 0) {
        frD = (TfrZonaD*)(FindComponent("frZonaDPrel"));
        if (frD == NULL) {
            frD = new TfrZonaD(this);
            frD->Parent = this->tsZonaDPrel;
            frD->Name = "frZonaDPrel";
        }
    }
    else {
        Tab = (TTabSheet*)(FindComponent("tsZona" + Zona + "Prel"));
        if (Tab != NULL)
            Tab->TabVisible = false;
    }
    // C
    Zona = "C";
    if (dmDBImpianto->CheckZonaUtente(Zona) > 0) {
        frC = (TfrZonaC*)(FindComponent("frZonaCPrel"));
        if (frC == NULL) {
            frC = new TfrZonaC(this);
            frC->Parent = this->tsZonaCPrel;
            frC->Name = "frZonaCPrel";
            frC->Enabled = false;
        }
    }
    else {
        Tab = (TTabSheet*)(FindComponent("tsZona" + Zona + "Prel"));
        if (Tab != NULL)
            Tab->TabVisible = false;
    }
    // B
    Zona = "B";
    if (dmDBImpianto->CheckZonaUtente(Zona) > 0) {
        frB = (TfrZonaB*)(FindComponent("frZonaBPrel"));
        if (frB == NULL) {
            frB = new TfrZonaB(this);
            frB->Parent = this->tsZonaBPrel;
            frB->Name = "frZonaBPrel";
            frB->Enabled = false;
        }
    }
    else {
        Tab = (TTabSheet*)(FindComponent("tsZona" + Zona + "Prel"));
        if (Tab != NULL)
            Tab->TabVisible = false;
    }
    // E
    Zona = "E";
    if (dmDBImpianto->CheckZonaUtente(Zona) > 0) {
        frE = (TfrZonaE*)(FindComponent("frZonaEPrel"));
        if (frE == NULL) {
            frE = new TfrZonaE(this);
            frE->Parent = this->tsZonaEPrel;
            frE->Name = "frZonaEPrel";
            frE->Enabled = false;
        }
    }
    else {
        Tab = (TTabSheet*)(FindComponent("tsZona" + Zona + "Prel"));
        if (Tab != NULL)
            Tab->TabVisible = false;
    }
    // F
    Zona = "F";
    if (dmDBImpianto->CheckZonaUtente(Zona) > 0) {
        frF = (TfrZonaF*)(FindComponent("frZonaFPrel"));
        if (frF == NULL) {
            frF = new TfrZonaF(this);
            frF->Parent = this->tsZonaFPrel;
            frF->Name = "frZonaFPrel";
        }
    }
    else {
        Tab = (TTabSheet*)(FindComponent("tsZona" + Zona + "Prel"));
        if (Tab != NULL)
            Tab->TabVisible = false;
    }

    // DEST
    // A1
    if (1 == 1) { // in qualche modo filtrare
        frA1 = (TfrZonaA1*)(FindComponent("frZonaA1Dest"));
        if (frA1 == NULL) {
            frA1 = new TfrZonaA1(this);
            frA1->Parent = this->tsZonaA1Dest;
            frA1->Name = "frZonaA1Dest";
        }
    }
    // A2
    if (1 == 1) { // in qualche modo filtrare
        frA2 = (TfrZonaA2*)(FindComponent("frZonaA2Dest"));
        if (frA2 == NULL) {
            frA2 = new TfrZonaA2(this);
            frA2->Parent = this->tsZonaA2Dest;
            frA2->Name = "frZonaA2Dest";
        }
    }
    // G
    if (1 == 1) { // in qualche modo filtrare
        frG = (TfrZonaG*)(FindComponent("frZonaGDest"));
        if (frG == NULL) {
            frG = new TfrZonaG(this);
            frG->Parent = this->tsZonaGDest;
            frG->Name = "frZonaGDest";
        }
    }
    // H
    if (1 == 1) { // in qualche modo filtrare
        frH = (TfrZonaH*)(FindComponent("frZonaHDest"));
        if (frH == NULL) {
            frH = new TfrZonaH(this);
            frH->Parent = this->tsZonaHDest;
            frH->Name = "frZonaHDest";
        }
    }
    // I
    if (1 == 1) { // in qualche modo filtrare
        frI = (TfrZonaI*)(FindComponent("frZonaIDest"));
        if (frI == NULL) {
            frI = new TfrZonaI(this);
            frI->Parent = this->tsZonaIDest;
            frI->Name = "frZonaIDest";
        }
    }
    // D
    if (1 == 1) { // in qualche modo filtrare
        frD = (TfrZonaD*)(FindComponent("frZonaDDest"));
        if (frD == NULL) {
            frD = new TfrZonaD(this);
            frD->Parent = this->tsZonaDDest;
            frD->Name = "frZonaDDest";
        }
    }

    // C
    if (1 == 1) { // in qualche modo filtrare
        frC = (TfrZonaC*)(FindComponent("frZonaCDest"));
        if (frC == NULL) {
            frC = new TfrZonaC(this);
            frC->Parent = this->tsZonaCDest;
            frC->Name = "frZonaCDest";
        }
        frC->Enabled = false;
    }

    // B
    if (1 == 1) { // in qualche modo filtrare
        frB = (TfrZonaB*)(FindComponent("frZonaBDest"));
        if (frB == NULL) {
            frB = new TfrZonaB(this);
            frB->Parent = this->tsZonaBDest;
            frB->Name = "frZonaBDest";
        }
        frB->Enabled = false;
    }
    // E
    if (1 == 1) { // in qualche modo filtrare
        frE = (TfrZonaE*)(FindComponent("frZonaEDest"));
        if (frE == NULL) {
            frE = new TfrZonaE(this);
            frE->Parent = this->tsZonaEDest;
            frE->Name = "frZonaEDest";
        }
        frE->Enabled = false;
    }
    // F
    if (1 == 1) { // in qualche modo filtrare
        frF = (TfrZonaF*)(FindComponent("frZonaFDest"));
        if (frF == NULL) {
            frF = new TfrZonaF(this);
            frF->Parent = this->tsZonaFDest;
            frF->Name = "frZonaFDest";
        }
    }

    // AggiornaDatiLocali();
    zonascelta = "";
    cbLinea->Text = "01";

    pcPrel->TabIndex = 0;
    TimerRef->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TfGestAGVMidi::TimerRefTimer(TObject * Sender) {
    // static AnsiString zonascelta = "";
    AnsiString ts;
    TimerRef->Enabled = false;

    // NB ottimizzare, non c'e' bisogno di un tipo diverso TfrZonaxx
    // prel
    if (pcPrel->ActivePage->Hint == "A") {
        TabVisibili(0, 0, 0, 1, 0, 0, 0, 1, 0);
        frA1 = (TfrZonaA1*)(FindComponent("frZonaA1Prel"));
        if (frA1 != NULL) {
            frA1->AggiornaDati();
            btConferma->Enabled = frA1->AbilitaConferma;
        }
    }
    else if (pcPrel->ActivePage->Hint == "J") {
        TabVisibili(0, 0, 1, 0, 0, 0, 0, 0, 0);
        frA2 = (TfrZonaA2*)(FindComponent("frZonaA2Prel"));
        if (frA2 != NULL) {
            frA2->AggiornaDati();
            btConferma->Enabled = frA1->AbilitaConferma;
        }
    }
    else if (pcPrel->ActivePage->Hint == "G") {
        TabVisibili(1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        frG = (TfrZonaG*)(FindComponent("frZonaGPrel"));
        if (frG != NULL) {
            frG->AggiornaDati();
            btConferma->Enabled = frG->AbilitaConferma;
        }
    }
    else if (pcPrel->ActivePage->Hint == "H") {
        TabVisibili(0, 0, 0, 0, 0, 0, 0, 1, 0, 0);
        frH = (TfrZonaH*)(FindComponent("frZonaHPrel"));
        if (frH != NULL) {
            frH->AggiornaDati();
            btConferma->Enabled = frH->AbilitaConferma;
        }
    }
    else if (pcPrel->ActivePage->Hint == "I") {
        TabVisibili(0, 1, 0, 0, 0, 0, 0, 0, 0, 0);
        frI = (TfrZonaI*)(FindComponent("frZonaIPrel"));
        if (frI != NULL) {
            frI->AggiornaDati();
            btConferma->Enabled = frI->AbilitaConferma;
        }
    }
    else if (pcPrel->ActivePage->Hint == "D") {
        TabVisibili(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        frD = (TfrZonaD*)(FindComponent("frZonaDPrel"));
        if (frD != NULL) {
            frD->AggiornaDati();
            btConferma->Enabled = frD->AbilitaConferma;
        }
    }
    else if (pcPrel->ActivePage->Hint == "C") {
        TabVisibili(0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
        frC = (TfrZonaC*)(FindComponent("frZonaCPrel"));
        if (frC != NULL) {
            frC->AggiornaDati();
            btConferma->Enabled = frC->AbilitaConferma;
        }
    }
    else if (pcPrel->ActivePage->Hint == "B") {
        TabVisibili(0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
        frB = (TfrZonaB*)(FindComponent("frZonaBPrel"));
        if (frB != NULL) {
            frB->AggiornaDati();
            btConferma->Enabled = frB->AbilitaConferma;
        }
    }
    else if (pcPrel->ActivePage->Hint == "E") {
        TabVisibili(0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
        frE = (TfrZonaE*)(FindComponent("frZonaEPrel"));
        if (frE != NULL) {
            frE->AggiornaDati();
            btConferma->Enabled = frE->AbilitaConferma;
        }
    }
    else if (pcPrel->ActivePage->Hint == "F") {
        TabVisibili(0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
        frF = (TfrZonaF*)(FindComponent("frZonaFPrel"));
        if (frF != NULL) {
            frF->AggiornaDati();
            btConferma->Enabled = frF->AbilitaConferma;
        }
    }

    // dest
    if (pcDest->ActivePage != NULL) {
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
    }

    // refresh griglie o dati
    if (pcPrel->ActivePage->Hint == "A") {
        pLinea->Visible = true;
        btRefresh->Enabled = true;
        if (zonascelta != "A") {
            pcDest->TabIndex = 0;
        }
        zonascelta = "A";
    }
    else if (pcPrel->ActivePage->Hint == "J") {
        pLinea->Visible = false;
        btRefresh->Enabled = false;
        if (zonascelta != "J") {
            if (frA2 != NULL) {
                frA2->AggiornaGriglia();
            }
        }
        zonascelta = "J";
    }
    else {
        pLinea->Visible = false;
        btRefresh->Enabled = false;
        zonascelta = "";
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

void TfGestAGVMidi::AggiornaDatiLocali() {
    AnsiString OldCod = "", NewCod = "";
    int idudc = 0;
    // fa davvero cagare ma che pure basta quindi va bene cosi'
    OldCod = dmDBImpianto->TornaArticoloDaHMC_ORDINI_IN_LAVORAZIONEcopia(cbLinea->Text);
    dmDBImpianto->ClonaHMC_ORDINI_IN_LAVORAZIONE(cbLinea->Text);
    NewCod = dmDBImpianto->TornaArticoloDaHMC_ORDINI_IN_LAVORAZIONEcopia(cbLinea->Text);
    if (NewCod != OldCod) {
        idudc = dmDB->RitornaUDCdaCodUDC(OldCod);
        dmDBImpianto->GeneraCMDaHaBFineProduzione(idudc);
    }
    frA1 = (TfrZonaA1*)(FindComponent("frZonaA1Prel"));
    if (frA1 != NULL) {
        frA1->AggiornaGriglia();
    }
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
    int premissionegiacreata = 0;
    int posprel = 0, pianoprel = 0, posdep = 0, pianodep = 0, tipoposizione, tipoposizionedep, idudc;

    // prel
    ZonaPrel = pcPrel->Pages[pcPrel->TabIndex]->Hint;
    if (ZonaPrel == "F") {
        SettaPianoSelezionatoPerZona(1, "F");
    }
    else
        TornaPosPrelSelezionata(ZonaPrel, posprel, pianoprel, tipoposizione, idudc);

    // dep
    ZonaDep = pcDest->ActivePage->Hint;
    if ((ZonaPrel == "A")) { // && (ZonaDep == "H"))
        if ((dmDBImpianto->TornaIndiceImpilabilitadaIDUDC(idudc) == 1) && (ZonaDep != "B"))
            dmDBImpianto->TornaPosDepLiberaH("H", idudc, posdep, pianodep);
        else if ((dmDBImpianto->TornaIndiceImpilabilitadaIDUDC(idudc) == 1) && (ZonaDep == "B")) {
            if (Application->MessageBox(L"Vuoi portare un pallet impilabile all'avvolgitore?", L"Conferma", MB_YESNO) == IDYES)
                dmDBImpianto->TornaPosDepLibera("B", posdep, pianodep);
        }
        else
            dmDBImpianto->TornaPosDepLibera("B", posdep, pianodep);
    }
    else if (ZonaPrel == "J") {
        if (tipoposizione == TIPOLOGIA_SCARTO)
            dmDBImpianto->TornaPosDepLibera(ZonaDep, posdep, pianodep, TIPOLOGIA_SCARTO);
        else {
            idudc = dmDB->RitornaUDCdaPosPiano(MainForm->pos_udc, 1);
            if (idudc == 0) {
                // se e' vuoto porto la selezione di J
                ZonaPrel = "I";
                posprel = dmDBImpianto->UDCPresenteInMagazzinoPerTipo(MainForm->trova_udc, TIPOLOGIA_MATERIEPRIME);
                if (posprel == 0)
                    ShowMessage("Materia prima non trovata, missione non creata");
                pianoprel = 1;
                ZonaDep = "J";
                posdep = MainForm->pos_udc;
                pianodep = 1;
            }
            else {
                // altrimenti prima porta via
                ZonaPrel = "J";
                TornaPosPrelSelezionata(ZonaPrel, posprel, pianoprel, tipoposizione, idudc);
                ZonaDep = "G";
                dmDBImpianto->TornaPosDepLibera("G", posdep, pianodep, TIPOLOGIA_PALLET);

            }

        }
    }
    else if ((ZonaPrel == "I") && (ZonaDep == "J") && (tipoposizione == TIPOLOGIA_MATERIEPRIME))
        dmDBImpianto->TornaPosDepLibera(ZonaDep, posdep, pianodep, TIPOLOGIA_MATERIEPRIME);
    else
        dmDBImpianto->TornaPosDepLibera(ZonaDep, posdep, pianodep);

    // genera cmissione
    if ((posprel > 0) && (posdep > 0) && (pianoprel > 0) && (pianodep > 0)) {
        // se deposito g azzero
        if ((ZonaDep == "G") && (ZonaPrel == "J")) {
            posdep = 0;
            pianodep = 0;
            CentroMis.ZonaDeposito = ZonaDep;
        }
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
        if ((ZonaPrel == "J") && (ZonaDep == "G"))
            CentroMis.IDUDC = 1;
        else
            CentroMis.IDUDC = dmDB->RitornaUDCdaPosPiano(CentroMis.posprel, CentroMis.pianoprel);
        CentroMis.CorsiaDeposito = " ";
        CentroMis.stato = 0;
        CentroMis.FilaInCorsiaDeposito = 0;
        CentroMis.TipoUDC = 0;
        CentroMis.DestinazioneModuli = 0;
        if (dmDB->PresenzaCentroMissionePerPosizioni(CentroMis.posprel, CentroMis.posdep, 0) == 0) {
            dmDB->GeneraCentroMissione(CentroMis);
            // tolgo prenotazioni
            dmDBImpianto->AggiornaSelezionePosizioni(ZonaPrel, 0, 0);
            dmDBImpianto->AggiornaSelezionePosizioni(ZonaDep, 0, 0);
        }
        else {
            premissionegiacreata = 1;
        }
    }

    if ((ZonaPrel == "J") && (ZonaDep == "G") && (tipoposizione != TIPOLOGIA_SCARTO)) {
        CentroMis.ZonaDeposito = ZonaPrel;
        CentroMis.ZonaPrelievo = "I";
        CentroMis.posprel = dmDBImpianto->UDCPresenteInMagazzinoPerTipo(MainForm->trova_udc, TIPOLOGIA_MATERIEPRIME);
        // dmDB->UDCPresenteInMagazzino(MainForm->trova_udc);
        CentroMis.pianoprel = 1;
        CentroMis.posdep = MainForm->pos_udc;
        CentroMis.pianodep = 1;
        CentroMis.IDUDC = 1; // porto sempre un vuoto MainForm->trova_udc;
        CentroMis.h_prel = dmDB->RitornaAltezzedaPosizione(CentroMis.posprel, CentroMis.pianoprel, "HPREL");
        CentroMis.h_dep = dmDB->RitornaAltezzedaPosizione(CentroMis.posdep, CentroMis.pianodep, "HDEP");
        CentroMis.TipoMissione = 0;
        CentroMis.CodTipoMovimento = 0;
        CentroMis.CodTipoMissione = 0;
        CentroMis.Priorita = cbPriorita->Text.ToIntDef(1) - 1;
        CentroMis.Agv = 1;

        CentroMis.CorsiaDeposito = " ";
        CentroMis.stato = 0;
        CentroMis.FilaInCorsiaDeposito = 0;
        CentroMis.TipoUDC = 0;
        CentroMis.DestinazioneModuli = 0;
        if (dmDB->PresenzaCentroMissionePerPosizioni(CentroMis.posprel, CentroMis.posdep, 0) == 0) {
            dmDB->GeneraCentroMissione(CentroMis);
            // tolgo prenotazioni
            dmDBImpianto->AggiornaSelezionePosizioni(ZonaPrel, 0, 0);
            dmDBImpianto->AggiornaSelezionePosizioni(ZonaDep, 0, 0);
        }
        else {
            premissionegiacreata = 1;
        }
    }
    if (premissionegiacreata > 0)
        ShowMessage("PreMissione gia' creata, operazione annullata");

    if ((ZonaPrel == "J") || (ZonaDep == "J"))
        zonascelta = "";

    MainForm->pos_udc = 0;
    MainForm->trova_udc = 0;
    cbPriorita->Text = "3";
    // Close();

}

// ---------------------------------------------------------------------------

void TfGestAGVMidi::TornaPosPrelSelezionata(AnsiString Zona, int &pos, int &piano, int &tipoposizione, int &idudc) {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;
    pos = 0;
    piano = 0;
    tipoposizione = 0;
    idudc = 0;

    AnsiString codudc = " ";
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql.printf("Select idudc, pos, piano, tipoposizione from piani_view where selezionata > 0 and zona = '%s' order by pos, piano", Zona);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->First();
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

void TfGestAGVMidi::SettaPianoSelezionatoPerZona(int setta, AnsiString zona) {
    // setta il campo selezionata sulla posizione data. Se viene passata la zone setta su tutta la zona
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "UPDATE piani_view SET Selezionata = " + IntToStr(setta);
        stringa += " where idudc>0 and zona = '" + zona + "'";
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Settata Zona " + zona + ", valore : " + IntToStr(setta));
        delete ADOQuery;
    }
    catch (...) {
    }
}

void TfGestAGVMidi::SettaPianoSelezionatoPerPos(int setta, int pos) {
    // setta il campo selezionata sulla posizione data. Se viene passata la zone setta su tutta la zona
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "UPDATE piani_view SET Selezionata = " + IntToStr(setta);
        stringa += " where pos = '" + IntToStr(pos) + "'";
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Selezionata Pos " + IntToStr(pos) + ", valore : " + IntToStr(setta));
        delete ADOQuery;
    }
    catch (...) {
    }
}

void __fastcall TfGestAGVMidi::FormDestroy(TObject * Sender)
{
    int numpos;
    /*
     TFGestAGVFrame *frLoad;

     numpos = dmDB->RitornaNumPosDaPos(pos);

     for (int i = 1; i <= numpos; i++) {
     frLoad = (TFGestAGVFrame*)(FindComponent(" FGestAGVFrame " + IntToStr(i)));
     if (frLoad != NULL) {
     delete frLoad;
     }
     }
     */
}

// ---------------------------------------------------------------------------
void __fastcall TfGestAGVMidi::btRefreshClick(TObject * Sender)
{
    AggiornaDatiLocali();

}

// ---------------------------------------------------------------------------
void __fastcall TfGestAGVMidi::TimerRigaTimer(TObject * Sender) {
    // questa cosa del timer fa cagare ma diversamente da errore
    TimerRiga->Enabled = false;

    if (MainForm->codiceriga > 0) {
        cbLinea->Text = dmExtraFunction->PadS(IntToStr(MainForm->codiceriga), 2, "0");
        MainForm->codiceriga = 0;
    }
    TimerRiga->Enabled = true;

}
// ---------------------------------------------------------------------------
