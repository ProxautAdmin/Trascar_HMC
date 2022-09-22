//---------------------------------------------------------------------------

#ifndef PorteFumoH
#define PorteFumoH
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
class TFormStatusPorteFumo : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TLabel *Label1;
	TLabel *Label3;
    TLabel *Label4;
    TRadioGroup *RadioGroup2;
    TTimer *Timer1;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn3;
    TSpeedButton *SpeedButton1;
    TLabel *Label2;
    TEdit *Edit2;
    TLabel *Label10;
	TLabel *Label7;
	TEdit *Edit4;
    TGroupBox *GroupBox2;
    TLabel *Label5;
    TEdit *eInput1;
    TLabel *Label6;
    TEdit *eInput2;
    TLabel *Label8;
    TEdit *eInput3;
    TEdit *eInput4;
    TLabel *Label9;
    TCheckBox *cbInput1;
    TCheckBox *cbInput2;
    TCheckBox *cbInput3;
    TCheckBox *cbInput4;
	TRadioGroup *RadioGroup1;
	TLabel *Label11;
	TEdit *Edit1;
	TEdit *Edit3;
	TLabel *Label12;
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormDeactivate(TObject *Sender);
    void __fastcall RadioGroup1Click(TObject *Sender);
    void __fastcall BitBtn3Click(TObject *Sender);
    void __fastcall RadioGroup2Click(TObject *Sender);
	void __fastcall CheckBox1Click(TObject *Sender);
	void __fastcall CheckBox2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormStatusPorteFumo(TComponent* Owner);
    int idport,count_forzatura_attiva ;
	bool activate,forzatura_attiva ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormStatusPorteFumo *FormStatusPorteFumo;
//---------------------------------------------------------------------------
#endif
