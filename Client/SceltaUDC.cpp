// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SceltaUDC.h"
#include "db.h"
#include "DBClient.h"
#include "SocketComunicazioneClient.h"
#include "ExtraFunction.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSceltaUDC *FormSceltaUDC;
extern TCentroMissione CentroMissione;

// ---------------------------------------------------------------------------
__fastcall TFormSceltaUDC::TFormSceltaUDC(TComponent* Owner)
    : TForm(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TFormSceltaUDC::FormActivate(TObject *Sender) {
    TCheckBox *OggettoCB;
    AnsiString zona, custpos, strsql;
    int stanza, corsie, alveoli, starttop, segno, numalveolo;
    // la form accetta in ingresso sia la pos che la zona
    // se ho la pos
    if (pos > 0)
        custpos = dmDBClient->RitornaCustPosinPosizione(pos);
    else
        custpos = zonaparam;
    zona = custpos.SubString(1, 3);
    stanza = custpos.SubString(4, 3).ToIntDef(0);
    strsql = "SELECT * from strutturazone where zona = '" + zona + "' and ( dastanza <=" + String(stanza) + " and astanza >=" + String(stanza) + ")";

    dmDB->QueryUDC->Close();
    dmDB->QueryUDC->SQL->Clear();
    dmDB->QueryUDC->SQL->Append(strsql);
    dmDB->QueryUDC->Open();
    if (dmDB->QueryUDC->RecordCount) {
        corsie = dmDB->QueryUDC->FieldByName("numfile")->AsInteger;
        alveoli = dmDB->QueryUDC->FieldByName("alveoli")->AsInteger;
        // larghezza frame
        this->Width = ((40 * corsie) + 100 > 240 ? (40 * corsie) + 100 : 240);
        // altezza frame
        this->Height = 340 + (20 * alveoli);
        // POSIZIONE OGGETTI NELLA FORM
        // GroupBox4->Top = 100;
        // GroupBox1->Top = 35;
        // questi non servono se oggetti ancorati all frame
        // GroupBox1->Left=GroupBox4->Left;
        // GroupBox1->Width=GroupBox1->Width;
        // GroupBox4->Width = this->Width - 40;
        // GroupBox4->Height = this->Height - 260;
        BitBtn5->Left = 40 + (this->Width / 3) - BitBtn5->Width;
        BitBtn4->Left = 60 + 2 * (this->Width / 3) - BitBtn5->Width;
        btAll->Left = BitBtn5->Left + 10;
        btNone->Left = BitBtn4->Left + 10;
        lbCustPos->Caption = custpos;
        lbZona->Caption = zona;
        lbRoom->Caption = stanza;
        // inizia a disegnare dall'alto  ?
        if (dmDB->QueryUDC->FieldByName("daAlto")->AsInteger) {
            segno = 1;
            starttop = 20;
        }
        else {
            segno = -1;
            starttop = GroupBox4->Height - 60;
        }

        for (int riga = 0; riga < alveoli; riga++) {
            for (int colonna = 1; colonna <= corsie; colonna++) {
                // numalveolo = (corsie * riga) + colonna; // se disposizione righe orizzontali
                numalveolo = ((colonna - 1) * alveoli) + riga + 1;
                OggettoCB = (TCheckBox*) FindComponent("cb" + String(numalveolo));
                if (OggettoCB == NULL) {
                    OggettoCB = new TCheckBox(this);
                    OggettoCB->Parent = this->GroupBox4;
                    OggettoCB->Name = "cb" + String(numalveolo);
                }
                OggettoCB->Caption = "";
                OggettoCB->Top = starttop + (segno * (20 * riga));
                OggettoCB->Left = (40 * colonna);
                OggettoCB->Caption = String(numalveolo);
                OggettoCB->Tag = numalveolo;
                OggettoCB->Hint = String(numalveolo);
                // setta check
                if (Pos("(" + String(numalveolo) + ")", strselected) > 0)
                    OggettoCB->Checked = true;
            }
        }
        //

    }
    else {
        strselected = "---";
        Close();
    }
}
// ---------------------------------------------------------------------------

void __fastcall TFormSceltaUDC::BitBtn5Click(TObject * Sender) {

    // scrivo lista frames selezionati
    int numalveolo, corsie, alveoli;
    strselected = "";
    TCheckBox *OggettoCB;
    corsie = dmDB->QueryUDC->FieldByName("numfile")->AsInteger;
    alveoli = dmDB->QueryUDC->FieldByName("alveoli")->AsInteger;

    for (int riga = 0; riga < alveoli; riga++) {
        for (int colonna = 1; colonna <= corsie; colonna++) {
            numalveolo = (corsie * riga) + colonna; // numero cb/alveolo
            OggettoCB = (TCheckBox*) FindComponent("cb" + String(numalveolo));
            if (OggettoCB != NULL) {
                if (OggettoCB->Checked) {
                    strselected += "(" + OggettoCB->Name.SubString(3, 2) + ")";
                }
            }
        }
    }

    /*
     FormWeightFrames->FrameSelected[idweight] = strselected;
     FormWeightFrames->AggiornaNumFrames(idweight);
     FormWeightFrames->selframe = idweight;
     FormWeightFrames->BitBtn1->Enabled = true;
     */
    Close();
}
// ---------------------------------------------------------------------------

void __fastcall TFormSceltaUDC::BitBtn4Click(TObject * Sender)
{
    CentroMissione.FilaInCorsiaDeposito = 0; // in questo modo al'uscita so se ho confermato un valore oppure no.
    Close();
}
// ---------------------------------------------------------------------------

void __fastcall TFormSceltaUDC::btAllClick(TObject * Sender) {
    int numalveolo, alveoli, corsie;
    TCheckBox *OggettoCB;
    corsie = dmDB->QueryUDC->FieldByName("numfile")->AsInteger;
    alveoli = dmDB->QueryUDC->FieldByName("alveoli")->AsInteger;

    for (int riga = 0; riga < alveoli; riga++) {
        for (int colonna = 1; colonna <= corsie; colonna++) {
            numalveolo = (corsie * riga) + colonna; // numero cb/alveolo
            OggettoCB = (TCheckBox*) FindComponent("cb" + String(numalveolo));
            if (OggettoCB != NULL) {
                OggettoCB->Checked = true;
            }
        }
    }
}
// ---------------------------------------------------------------------------

void __fastcall TFormSceltaUDC::btNoneClick(TObject * Sender) {
    int numalveolo, alveoli, corsie;
    TCheckBox *OggettoCB;
    corsie = dmDB->QueryUDC->FieldByName("numfile")->AsInteger;
    alveoli = dmDB->QueryUDC->FieldByName("alveoli")->AsInteger;

    for (int riga = 0; riga < alveoli; riga++) {
        for (int colonna = 1; colonna <= corsie; colonna++) {
            numalveolo = (corsie * riga) + colonna; // numero cb/alveolo
            OggettoCB = (TCheckBox*) FindComponent("cb" + String(numalveolo));
            if (OggettoCB != NULL) {
                OggettoCB->Checked = false;
            }
        }
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormSceltaUDC::FormDeactivate(TObject *Sender) {
    int numalveolo, alveoli, corsie;
    TCheckBox *OggettoCB;
    corsie = dmDB->QueryUDC->FieldByName("numfile")->AsInteger;
    alveoli = dmDB->QueryUDC->FieldByName("alveoli")->AsInteger;

    for (int riga = 0; riga < alveoli; riga++) {
        for (int colonna = 1; colonna <= corsie; colonna++) {
            numalveolo = (corsie * riga) + colonna; // numero cb/alveolo
            OggettoCB = (TCheckBox*) FindComponent("cb" + String(numalveolo));
            if (OggettoCB != NULL) {
                delete OggettoCB;
            }
        }
    }
    dmDB->QueryUDC->Close();
    dmDB->QueryUDC->SQL->Clear();
}
// ---------------------------------------------------------------------------
