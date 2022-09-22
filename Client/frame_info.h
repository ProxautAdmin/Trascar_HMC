//---------------------------------------------------------------------------

#ifndef frame_infoH
#define frame_infoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "clientdata.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>

//---------------------------------------------------------------------------
class TFrameInfo : public TFrame
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
    TTimer *TimerInfo;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *edDouble;
    TEdit *edMisC;
    TLabel *lbTimeToProx;
    TLabel *lbTimeDouble;
    TLabel *lbTimeToMission;
    TEdit *edToProx;
    TGroupBox *gbZoom;
    TButton *btZona1;
    TButton *btZona2;
    TGroupBox *GroupBox2;
    TButton *Button3;
    TLabel *lbCont;
    TLabel *Label5;
    TButton *Button1;
    TLabel *Label4;
    TEdit *Edit1;
    void __fastcall TimerInfoTimer(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
    void __fastcall btZona1Click(TObject *Sender);
    void __fastcall btZona2Click(TObject *Sender);

private:	// User declarations
    int ccont;
public:		// User declarations
	__fastcall TFrameInfo(TComponent* Owner);
	void RiempiCampi(AnsiString struttura_hex) ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameInfo *FrameInfo;
//---------------------------------------------------------------------------
#endif
