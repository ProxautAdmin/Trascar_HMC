// ---------------------------------------------------------------------------

#ifndef plcScaricoH
#define plcScaricoH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include "UDCFrame.h"
#include <Vcl.ComCtrls.hpp>
#include "clientdata.h"

// ---------------------------------------------------------------------------
class TFormPlcScarico : public TForm
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
    TGroupBox *GroupBox4;
    TGroupBox *GroupBox8;
    TLabel *Label25;
    TLabel *Label26;
    TEdit *eHDep;
    TEdit *eHPrel;
    TGroupBox *GroupBox9;
    TCheckBox *ckOutput1;
    TGroupBox *GroupBox6;
    TCheckBox *ckInput1;
    TCheckBox *ckInput2;
    TCheckBox *ckInput3;
    TEdit *eUDC;
    TLabel *Label1;
    TBitBtn *BitBtn6;
    TBitBtn *BitBtn1;

    void __fastcall FormActivate(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall BitBtn5Click(TObject *Sender);
    void __fastcall BitBtn4Click(TObject *Sender);
    void __fastcall BitBtn6Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall SpeedButton2Click(TObject *Sender);
    void __fastcall cbTipoUDCChange(TObject *Sender);
    void __fastcall eUDCChange(TObject *Sender);
    void __fastcall cbStatusChange(TObject *Sender);
    void __fastcall ckDisabilitaPosClick(TObject *Sender);
    void __fastcall cPrenotataPosClick(TObject *Sender);
    void __fastcall SpeedButton3Click(TObject *Sender);

private: // User declarations
        public : // User declarations
    __fastcall TFormPlcScarico(TComponent* Owner);
    void VisualizzaDatiPosizione();
    void SetIdPlcDaPos();
    void SetInputOutputPlc();
    void AggiornaCampiPlc(DatiPlcTGW &StructPlc);
    void AssociaPlc(DatiPlcTGW &StructPlc);

    int pos, idplc, pos_plc, pos_struttura_plc, activate, input, output, plc_scarico_griglie, change_udc, cambiocheck;
    DatiPlcTGW StructPlcVisualizzata;
    int posizionecarico;
};

// ---------------------------------------------------------------------------
extern PACKAGE TFormPlcScarico *FormPlcScarico;
// ---------------------------------------------------------------------------
#endif
