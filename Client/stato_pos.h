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

    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel6;
    TPanel *Panel7;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TPanel *Panel8;
    TBitBtn *BitBtnChange;
    TBitBtn *BitBtn2;
    TPanel *Panel5;
    TLabel *Label5;
    TLabel *lbNumCorsia;
    TLabel *Label7;
    TLabel *Label8;
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
    TGroupBox *GroupBox4;
    TLabel *Label13;
    TLabel *Label14;
    TEdit *eUDC;
    TLabeledEdit *leNumUDC;
    TPanel *Panel4;
    TPanel *Panel3;
    TfrPosUDC *frPosUDC1;
    TLabel *Label19;
    TGroupBox *GroupBox5;
    TEdit *ePesoAttuale;
    TLabel *Label18;
    TEdit *ePesoBase;
    TLabel *Label35;
    TGroupBox *GroupBox6;
    TGroupBox *GroupBox7;
    TComboBox *cbTipoUDC;
    TLabel *Label23;
    TEdit *eTara;
    TEdit *eCaloKg;
    TEdit *eCaloPerc;
    TLabel *Label22;
    TLabel *Label24;
    TSpeedButton *SpeedButton2;
    TGroupBox *GroupBox8;
    TLabel *Label25;
    TLabel *Label26;
    TEdit *eHDep;
    TEdit *eHprel;
	TSpeedButton *SpeedButton3;
    TEdit *eIdArticolo;
    TLabel *Label16;
    TLabel *Label21;
    TEdit *eIDArtUDC;
    TEdit *eNome;
    TLabel *Label15;
    TEdit *eCodeArtUDC;
    TLabel *Label20;
    TEdit *eUltimoUDC;
    TLabel *Label27;
    TLabel *Label28;
    TSpeedButton *sbArticolo;
    TLabel *Label17;
    TLabel *lbPiano;
    TEdit *edCodUDC;
	TLabel *lbnomepiano;
	TLabel *Label30;
	TGroupBox *GroupBox9;
	TLabeledEdit *lePesoTotale;
	TLabeledEdit *lePesoTotaleNoTara;
	TCheckBox *ckDisabilitaPiano;
	TCheckBox *ckPianoRiservato;
	TBitBtn *btDisabilitaPiani;
	TCheckBox *ckIgnoraOrdine;
	TCheckBox *cbDepSelettivo;
	TEdit *edtAnnotazioni;
	TLabel *lbl2;
	TLabel *lbl3;
	TComboBox *cbbArtUDC;
	TGroupBox *GroupBox10;
	TEdit *edtRifOrdine;
	TComboBox *cbbTipoPosizione;
	TLabel *lbl1;
	TLabel *Label29;
    TCheckBox *cbImpilabile;

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
    void __fastcall SpeedButton2Click(TObject *Sender);
    void __fastcall ckDisabilitaPosClick(TObject *Sender);
	void __fastcall SpeedButton3Click(TObject *Sender);
    void __fastcall cPrenotataPosClick(TObject *Sender);
    void __fastcall sbArticoloClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);



private: // User declarations
        public : // User declarations
    __fastcall TfCorsia(TComponent* Owner);
    void VisualizzaPosizioneUDC();
    void CalcolaPesoTotale();


    int  statodisabilita, statoprenotata, pianosel;
    bool change_dati_corsia, change_dati_pos, aggiornamento, change_dati_udc, cambiocheck;
};

// ---------------------------------------------------------------------------
extern PACKAGE TfCorsia *fCorsia;
// ---------------------------------------------------------------------------
#endif
