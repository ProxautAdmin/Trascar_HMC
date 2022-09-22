//---------------------------------------------------------------------------

#ifndef ZonaA2FrameH
#define ZonaA2FrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "MyShape.h"
#include "clientdata.h"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrZonaA2 : public TFrame
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
    TPanel *pnPosA8;
    TPanel *pnPosA9;
    void __fastcall pnPosAMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations

public:		// User declarations
    __fastcall TfrZonaA2(TComponent* Owner);
    void AggiornaDati();
            bool AbilitaConferma;


};
//---------------------------------------------------------------------------
extern PACKAGE TfrZonaA2 *frZonaA2;
//---------------------------------------------------------------------------
#endif
