//---------------------------------------------------------------------------

#ifndef frame_moduloH
#define frame_moduloH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "clientdata.h"

//---------------------------------------------------------------------------
class TFrameModulo : public TFrame
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TLabel *Label4;
	TLabel *Label5;
	TEdit *editDato1;
	TEdit *editDato2;
	TLabel *Label1;
	TEdit *eC2;
	TEdit *eC1;
	TEdit *eC4;
	TEdit *eC3;
	TEdit *eC6;
	TEdit *eC5;
	TEdit *eC8;
	TEdit *eC7;
	TEdit *eC10;
	TEdit *eC9;
	TEdit *eC12;
	TEdit *eC11;
	TEdit *eC14;
	TEdit *eC13;
	TEdit *eC16;
	TEdit *eC15;
	TEdit *eC18;
	TEdit *eC17;
	TEdit *eC20;
	TEdit *eC19;
	TLabel *Label2;
	TEdit *eP2;
	TEdit *eP1;
	TEdit *eP4;
	TEdit *eP3;
	TEdit *eP6;
	TEdit *eP5;
	TEdit *eP8;
	TEdit *eP7;
	TEdit *eP10;
	TEdit *eP9;
	TEdit *eP12;
	TEdit *eP11;
	TEdit *eP14;
	TEdit *eP13;
	TEdit *eP16;
	TEdit *eP15;
	TEdit *eP18;
	TEdit *eP17;
	TEdit *eP20;
	TEdit *eP19;
	TEdit *editDato3;
	TLabel *Label3;
	void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFrameModulo(TComponent* Owner);
	void RiempiCampi(AnsiString struttura_hex) ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameModulo *FrameModulo;
//---------------------------------------------------------------------------
#endif
