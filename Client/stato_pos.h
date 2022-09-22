// ---------------------------------------------------------------------------

#ifndef stato_posH
#define stato_posH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "ChildWin.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "MyShape.h"
#include <Vcl.Graphics.hpp>
#include "pos_UDC.h"

// ---------------------------------------------------------------------------
class TfCorsia : public TMDIChild
{
__published: // IDE-managed Components

    TPanel *MainPanel;
    TPanel *PanelScaffale;
    TPanel *PanelPosizione;
    TPanel *Panel7;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TPanel *Panel8;
    TBitBtn *BitBtnChange;
    TBitBtn *BitBtn2;
    TPanel *PanelFila;
    TPanel *Panel9;
    TPanel *Panel10;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn3;
    TGroupBox *GroupBox1;
    TCheckBox *ckUsoPrel;
    TCheckBox *ckUsoDep;
    TCheckBox *ckAbilita;
    TCheckBox *ckPrenotata;
    TCheckBox *ckPiena;
    TCheckBox *ckVuota;
    TGroupBox *GroupBox2;
    TLabel *Label9;
    TComboBox *cbTipo;
    TLabel *Label10;
    TComboBox *cbPriorita;
    TLabel *Label11;
    TLabel *Label12;
    TBitBtn *BitBtn4;
    TBitBtn *BitBtn5;
    TBitBtn *BitBtn6;
    TTimer *Timer1;
    TGroupBox *GroupBox3;
    TCheckBox *ckDisabilitaPos;
    TCheckBox *cPrenotataPos;
    TLabeledEdit *leNumUDC;
    TGroupBox *GroupBox8;
    TLabel *Label25;
    TLabel *Label26;
    TEdit *eHDep;
    TEdit *eHprel;
    TLabel *Label17;
    TLabel *lbPiano;
    TLabel *lbnomepiano;
    TLabel *Label30;
    TComboBox *cbbTipoPosizione;
    TLabel *lbl1;
    TCheckBox *ckDisabilitaPiano;
    TCheckBox *ckPianoRiservato;
    TBitBtn *btDisabilitaPiani;
    TEdit *edtRifOrdine;
    TEdit *edtAnnotazioni;
    TLabel *lbl2;
    TCheckBox *ckIgnoraOrdine;
    TCheckBox *cbDepSelettivo;
    TPanel *PanelData;
    TBitBtn *BitBtn7;
    TEdit *eUDC;
    TLabel *Label13;
    TLabel *Label14;
    TEdit *Edit1;
    TLabel *Label15;
    TEdit *Edit2;
    TEdit *Edit3;
    TLabel *Label16;
    TEdit *editTabella;
    TLabel *Label18;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label5;
    TLabel *Label6;
    TBitBtn *btnModificaAltezza;

    void __fastcall frScaffale1MyShape4MouseUp(TObject *Sender, TMouseButton Button,
        TShiftState Shift, int X, int Y);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall BitBtn5Click(TObject *Sender);
    void __fastcall BitBtn3Click(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall BitBtn4Click(TObject *Sender);
    void __fastcall BitBtnChangeClick(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall BitBtn6Click(TObject *Sender);
    void __fastcall cbTipoChange(TObject *Sender);
    void __fastcall ckAbilitaClick(TObject *Sender);
    void __fastcall ckDisabilitaPosClick(TObject *Sender);
    void __fastcall SpeedButton3Click(TObject *Sender);
    void __fastcall cPrenotataPosClick(TObject *Sender);
    void __fastcall sbArticoloClick(TObject *Sender);
    void __fastcall btDisabilitaPianiClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall BitBtn7Click(TObject *Sender);
    void __fastcall btnModificaAltezzaClick(TObject *Sender);

private: // User declarations

    TRecordList TabPosizioni;
    TRecordList TabCorsie;
    TRecordList TabPiani;
    TIndexList TabUDC;

public: // User declarations

    __fastcall TfCorsia(TComponent* Owner);
    void VisualizzaPosizioneUDC();
    void CaricaDB();

    int statodisabilita, statoprenotata, pianosel, pos;
    bool change_dati_corsia, change_dati_pos, aggiornamento, change_dati_udc, cambiocheck;

    int count_inizio;
    int TopPiano;
};

// ---------------------------------------------------------------------------
extern PACKAGE TfCorsia *fCorsia;
// ---------------------------------------------------------------------------
#endif