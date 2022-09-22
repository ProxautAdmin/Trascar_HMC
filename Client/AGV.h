//---------------------------------------------------------------------------

#ifndef AGVH
#define AGVH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "CHILDWIN.h"
#include "DB.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.Grids.hpp>
#include <Data.Win.ADODB.hpp>
#include <Vcl.Dialogs.hpp>
#include "AGVFrameL.h"
#include <Vcl.Menus.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------
class TfAGV : public TMDIChild
{
__published:	// IDE-managed Components
	TTimer *TimerUpd;
	TScrollBox *ScrollBox1;
	TPanel *Panel4;
	TPanel *Panel1;
    TfrAgvL *frAgvL1;
	void __fastcall TimerUpdTimer(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormActivate(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TfAGV(TComponent* Owner);
	void ColoraShape(int stato, TShape *t, TColor ColorON = clLime, TColor ColorOFF = clGray);
	TIndexList ListaMacchine;
	AnsiString PPFolder;
	bool active,first ;
};
//---------------------------------------------------------------------------
extern PACKAGE TfAGV *fAGV;
//---------------------------------------------------------------------------
#endif
