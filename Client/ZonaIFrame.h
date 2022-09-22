// ---------------------------------------------------------------------------

#ifndef ZonaIFrameH
#define ZonaIFrameH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "MyShape.h"
#include "clientdata.h"
#include <Vcl.ExtCtrls.hpp>

// ---------------------------------------------------------------------------
class TfrZonaI : public TFrame
{
__published: // IDE-managed Components

    TPanel *pnI;
    TPanel *pnPosI1;
    TPanel *pnPosI2;
    TPanel *pnPosI3;
    TPanel *pnPosI4;
    TPanel *pnPosI5;
    TPanel *pnPosI6;
    TPanel *pnPosI7;
    TPanel *pnPosI8;
    TPanel *pnPosI9;
    TPanel *pnPosI10;
    TLabel *lbSelI;
    void __fastcall pnPosIMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

private: // User declarations
        public : // User declarations
    __fastcall TfrZonaI(TComponent* Owner);
    void AggiornaDati();
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrZonaI *frZonaI;
// ---------------------------------------------------------------------------
#endif
