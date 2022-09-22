// ---------------------------------------------------------------------------

#ifndef PosPrelievoPLCH
#define PosPrelievoPLCH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include "clientdata.h"
#include "cspin.h"

// ---------------------------------------------------------------------------
class TFormPosPrelPLC : public TForm
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
    TGroupBox *gbPos;
    TLabel *Label25;
    TLabel *Label26;
    TEdit *eHDep1;
    TEdit *eHPrel1;
    TLabel *Label1;
    TLabel *lbZona;
    TLabel *Label2;
    TLabel *lbTipoPos;
    TGroupBox *gbPLCOut;
    TBitBtn *SetOutBtn;
    TCheckBox *cbAGVInIngombro;
    TCheckBox *cbRichiestaAbilitaPrel;
    TGroupBox *GroupBox6;
    TCheckBox *ckPresenzaUDC;
    TGroupBox *gbMatter;
    TLabel *Label11;
    TCheckBox *cbIn0;
    TCheckBox *cbIn1;
    TEdit *editIdPallet;
    TCheckBox *cbIn2;
    TCheckBox *cbIn3;

    void __fastcall FormActivate(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall BitBtn5Click(TObject *Sender);
    void __fastcall BitBtn4Click(TObject *Sender);
    void __fastcall BitBtn6Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall cbTipoUDCChange(TObject *Sender);
    void __fastcall eUDCChange(TObject *Sender);
    void __fastcall cbStatusChange(TObject *Sender);
    void __fastcall ckDisabilitaPosClick(TObject *Sender);
    void __fastcall cPrenotataPosClick(TObject *Sender);
    void __fastcall cbInMissioneClick(TObject *Sender);
    void __fastcall ckPresenzaUDCClick(TObject *Sender);


private: // User declarations
        public : // User declarations
    __fastcall TFormPosPrelPLC(TComponent* Owner);
    void VisualizzaDatiPosizione();
    void SetIdPlcDaPos();
    void SetInputOutputPlc();
    void AggiornaCampiPlc(DatiPlc &StructPosVisualizzata, DatiPrelievo &prelievo);
    void AssociaPlc();

    int pos, idplc, pos_plc, pos_struttura_plc, activate, input[2], output[2], plc_scarico_griglie, change_udc, cambiocheck, piano;
    DatiPlc StructPosVisualizzata;
    DatiPrelievo StructPosDati;
};

// ---------------------------------------------------------------------------
extern PACKAGE TFormPosPrelPLC *FormPosPrelPLC;
// ---------------------------------------------------------------------------
#endif
