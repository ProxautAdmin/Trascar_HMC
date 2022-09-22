//---------------------------------------------------------------------------

#ifndef ZonaA1FrameH
#define ZonaA1FrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "MyShape.h"
#include "clientdata.h"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrZonaA1 : public TFrame
{
__published:	// IDE-managed Components
    TPanel *pnA;
    TPanel *pnPosA1;
    TPanel *pnPosA2;
    TPanel *pnPosA3;
    TLabel *lbSelA;
    TPanel *pnPosA4;
    TPanel *pnPosA5;
    TPanel *pnPosA6;
    TPanel *pnPosA7;
    TListBox *ListBox1;
    TLabel *Label1;
    void __fastcall pnPosAMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations

public:		// User declarations
    __fastcall TfrZonaA1(TComponent* Owner);
    void AggiornaDati();


};
//---------------------------------------------------------------------------
extern PACKAGE TfrZonaA1 *frZonaA1;
//---------------------------------------------------------------------------
#endif
