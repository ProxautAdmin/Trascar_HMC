//---------------------------------------------------------------------------

#ifndef creaudcH
#define creaudcH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormCreaUdc : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TBitBtn *BitBtnClose;
	TLabel *Label1;
	TComboBox *cbCodice;
	TBitBtn *BitBtnChange;
	TLabel *Label2;
	TEdit *eLotto;
	TEdit *eHPallet;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *eUdc;
	void __fastcall BitBtnCloseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormCreaUdc(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCreaUdc *FormCreaUdc;
//---------------------------------------------------------------------------
#endif
