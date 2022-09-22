//---------------------------------------------------------------------------

#ifndef plcsignal_lavatriceH
#define plcsignal_lavatriceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include "UDCFrame.h"
//---------------------------------------------------------------------------
class TFormPlcLavatrice : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TGroupBox *GroupBox2;
	TCheckBox *ckInput1;
	TCheckBox *ckInput2;
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
    TBitBtn *BitBtn6;
	TGroupBox *GroupBox3;
	TCheckBox *ckDisabilitaPos;
	TCheckBox *cPrenotataPos;
	TGroupBox *GroupBox4;
	TCheckBox *ckOutput1;
	TCheckBox *ckOutput2;
	TCheckBox *ckOutput3;
	TBitBtn *BitBtn1;
    TCheckBox *ckInput3;
    TCheckBox *ckInput4;
    TCheckBox *ckOutput4;
    TGroupBox *GroupBox1;
    TEdit *eHDep;
    TLabel *Label25;
    TEdit *eHprel;
    TLabel *Label26;
    TGroupBox *GroupBox5;
    TLabel *Label3;
	TLabel *Label13;
	TEdit *eUDC;
	TSpeedButton *SpeedButton2;
	TComboBox *ComboBox1;
	TSpeedButton *SpeedButton3;
    TPanel *Panel3;
    TLabel *Label1;
    TLabel *lCycle;
    TLabel *Label6;
    TLabel *lErrors;
    TPanel *Panel8;
    TPanel *Panel15;
    TShape *Shape4;
    TGroupBox *GroupBox6;
    TLabel *Label9;
    TLabel *lWatchDogPlc;
    TLabel *Label12;
    TLabel *lWatchDogPc;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall BitBtn5Click(TObject *Sender);
    void __fastcall BitBtn4Click(TObject *Sender);
    void __fastcall BitBtn6Click(TObject *Sender);
	void __fastcall SpeedButton2Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall SpeedButton3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormPlcLavatrice(TComponent* Owner);
	void VisualizzaDatiPosizione() ;
	void SetIdPlcDaPos() ;
	void SetInputOutputPlc() ;
	int pos,idplc,pos_plc,activate,input,output ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPlcLavatrice *FormPlcLavatrice;
//---------------------------------------------------------------------------
#endif
