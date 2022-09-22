// ---------------------------------------------------------------------------

#ifndef PosDepositoPLCH
#define PosDepositoPLCH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include "clientdata.h"

// ---------------------------------------------------------------------------
class TFormPosDepPLC : public TForm
{
__published: // IDE-managed Components

    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel7;
    TLabel *Label3;
    TLabel *lCycle;
    TLabel *Label6;
    TLabel *lErrors;
    TPanel *Panel8;
    TPanel *Panel15;
    TShape *Shape4;
    TGroupBox *GroupBox1;
    TLabel *Label9;
    TLabel *lWatchDogPlc;
    TLabel *Label12;
    TLabel *lWatchDogPc;
    TPanel *Panel4;
    TPanel *Panel5;
    TLabel *Label20;
    TLabel *Label22;
    TLabel *Label23;
    TLabel *Label24;
    TBitBtn *BitBtn4;
    TBitBtn *BitBtn5;
    TSpeedButton *SpeedButton1;
    TTimer *Timer1;
    TGroupBox *GroupBox3;
    TCheckBox *ckDisabilitaPos;
    TCheckBox *cPrenotataPos;
    TLabel *Label1;
    TLabel *lbZona;
    TLabel *Label4;
    TLabel *lbTipoPos;
	TGroupBox *gbPos;
	TLabel *Label7;
	TLabel *Label8;
	TEdit *eHDep1;
	TEdit *eHPrel1;
	TGroupBox *gbPLCOut;
    TCheckBox *cbRichiestaAbilitaDep;
    TCheckBox *cbAGVInIngombro;
    TBitBtn *SetOutBtn;
	TEdit *editIdPallet;
	TLabel *Label11;
    TGroupBox *gbMatter;
    TCheckBox *cbIn0;
    TCheckBox *cbIn1;
    TGroupBox *GroupBox6;
    TCheckBox *ckInput1;
    TCheckBox *cbIn2;
    TCheckBox *cbIn3;

    void __fastcall FormActivate(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall BitBtn5Click(TObject *Sender);
    void __fastcall BitBtn4Click(TObject *Sender);
    void __fastcall BitBtn6Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall ckDisabilitaPosClick(TObject *Sender);
    void __fastcall cPrenotataPosClick(TObject *Sender);
    void __fastcall cbInMissioneClick(TObject *Sender);
    void __fastcall ckPresenzaUDCClick(TObject *Sender);
    void __fastcall ckInput1Click(TObject *Sender);


private: // User declarations
        public : // User declarations
    __fastcall TFormPosDepPLC(TComponent* Owner);
    void VisualizzaDatiPosizione();
    void AggiornaCampiPlc(DatiPlc &StructPosVisualizzata, DatiDeposito &StructPosDati) ;
    void AssociaPlc();

    int pos, idplc, pos_plc, pos_struttura_plc, activate, input[2], output[2], plc_scarico_griglie, change_udc, cambiocheck, piano;
    DatiPlc StructPosVisualizzata;
    DatiDeposito StructPosDati;
};

// ---------------------------------------------------------------------------
extern PACKAGE TFormPosDepPLC *FormPosDepPLC;
// ---------------------------------------------------------------------------
#endif
