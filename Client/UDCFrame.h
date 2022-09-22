//---------------------------------------------------------------------------

#ifndef UDCFrameH
#define UDCFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include "frame_modulo.h"
//---------------------------------------------------------------------------
class TfUDCSmall : public TFrame
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox2;
	TLabel *Label7;
    TEdit *eIDArtUDC;
	TGroupBox *GroupBox1;
	TLabel *Label18;
	TEdit *Edit1;
	TLabel *Label20;
	TEdit *Edit2;
	TLabel *Label21;
	TEdit *Edit3;
	TLabel *Label22;
	TEdit *Edit4;
	TLabel *Label23;
	TEdit *Edit5;
	TLabel *Label24;
	TEdit *Edit6;
	TLabel *Label25;
	TEdit *Edit7;
	TLabel *Label26;
	TEdit *Edit8;
	TLabel *Label27;
	TEdit *Edit9;
	TLabel *Label28;
	TEdit *Edit10;
	TLabel *Label29;
	TEdit *Edit11;
	TLabel *Label30;
	TEdit *Edit12;
	TLabel *Label31;
	TEdit *Edit13;
	TLabel *Label32;
	TEdit *Edit14;
	TLabel *Label33;
	TEdit *Edit15;
	TLabel *Label36;
	TEdit *Edit16;
	TLabel *Label37;
	TEdit *Edit17;
	TLabel *Label38;
	TEdit *Edit18;
	TLabel *Label39;
	TEdit *Edit19;
	TLabel *Label40;
	TEdit *Edit20;
	TLabel *Label41;
	TEdit *Edit21;
	TLabel *Label42;
	TEdit *Edit22;
	TLabel *Label43;
	TEdit *Edit23;
	TLabel *Label44;
	TEdit *Edit24;
    TGroupBox *GroupBox3;
    TCheckBox *cbRiempi1;
    TCheckBox *cbRiempi2;
    TCheckBox *cbRiempi3;
    TCheckBox *cbRiempi4;
    TCheckBox *cbRiempi5;
    TCheckBox *cbRiempi6;
    TCheckBox *cbRiempi7;
    TCheckBox *cbRiempi8;
    TCheckBox *cbRiempi9;
    TCheckBox *cbRiempi10;
    TCheckBox *cbRiempi11;
    TCheckBox *cbRiempi12;
    TCheckBox *cbRiempi13;
    TCheckBox *cbRiempi14;
    TCheckBox *cbRiempi15;
    TCheckBox *cbRiempi16;
    TCheckBox *cbRiempi17;
    TCheckBox *cbRiempi18;
    TCheckBox *cbRiempi19;
    TCheckBox *cbRiempi20;
    TCheckBox *cbRiempi21;
    TCheckBox *cbRiempi22;
    TCheckBox *cbRiempi23;
    TCheckBox *cbRiempi24;
	TFrameModulo *FrameModulo1;
	TFrameModulo *FrameModulo2;
	TFrameModulo *FrameModulo3;
	TFrameModulo *FrameModulo4;
	TFrameModulo *FrameModulo5;
	TFrameModulo *FrameModulo6;
	TBitBtn *BitBtn1;
	TEdit *eLastUdc;
	TLabel *Label1;
	TEdit *eDest;
	TLabel *Label2;
	TBitBtn *BtnWritePlc;
	TSpeedButton *sbSearch;
	TBitBtn *BtnWriteUdcPlc;
    TBitBtn *BitBtnNew;
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BtnWriteUdcPlcClick(TObject *Sender);
	void __fastcall sbSearchClick(TObject *Sender);
    void __fastcall BitBtnNewClick(TObject *Sender);
  //	void __fastcall SpeedButton1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfUDCSmall(TComponent* Owner);
	void RiempiCampi(int idartudc,AnsiString struttura_hex,bool activate ) ;
	int idplc, db_strhex,dw_strhex ,numbyte,write_table,id_struttura ;
};
//---------------------------------------------------------------------------
extern PACKAGE TfUDCSmall *fUDCSmall;
//---------------------------------------------------------------------------
#endif
