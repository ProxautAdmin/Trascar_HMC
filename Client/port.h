//---------------------------------------------------------------------------

#ifndef portH
#define portH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormPortStatus : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TLabel *Label1;
	TLabel *Label3;
    TLabel *Label4;
    TTimer *Timer1;
    TBitBtn *btOk;
    TBitBtn *btEsci;
    TSpeedButton *SpeedButton1;
    TLabel *Label2;
    TEdit *Edit2;
    TLabel *Label10;
	TLabel *Label5;
	TEdit *Edit1;
	TLabel *Label6;
	TEdit *Edit3;
	TGroupBox *GroupBox1;
	TCheckBox *CheckBox1;
	TCheckBox *CheckBox2;
	TLabel *Label7;
	TEdit *Edit4;
	TGroupBox *GroupBox2;
	TCheckBox *ckInput1;
	TRadioGroup *RadioGroup2;
    TBitBtn *btSetOut;
	TCheckBox *ckInput2;
    TCheckBox *ckInput3;
    TCheckBox *CheckBox3;
    TCheckBox *CheckBox4;
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall btOkClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormDeactivate(TObject *Sender);
    void __fastcall btEsciClick(TObject *Sender);
    void __fastcall RadioGroup2Click(TObject *Sender);
	void __fastcall ckInput1Click(TObject *Sender);
	void __fastcall btSetOutClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormPortStatus(TComponent* Owner);
    int idport,count_forzatura_attiva ;
	bool activate,forzatura_attiva ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPortStatus *FormPortStatus;
//---------------------------------------------------------------------------
#endif
