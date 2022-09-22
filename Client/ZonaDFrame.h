// ---------------------------------------------------------------------------

#ifndef ZonaDFrameH
#define ZonaDFrameH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "MyShape.h"
#include "clientdata.h"
#include <Vcl.ExtCtrls.hpp>

// ---------------------------------------------------------------------------
class TfrZonaD : public TFrame
{
__published: // IDE-managed Components

    TPanel *pnD;
    TPanel *pnPosD1;
    TPanel *pnPosD2;
    TPanel *pnPosD3;
    TPanel *pnPosD4;
    TPanel *pnPosD5;
    TPanel *pnPosD6;
    TPanel *pnPosD7;
    TPanel *pnPosD8;
    TLabel *lbSelD;

    void __fastcall pnPosDMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

private: // User declarations
        public : // User declarations
    __fastcall TfrZonaD(TComponent* Owner);
    void AggiornaDati();
    bool AbilitaConferma ;
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrZonaD *frZonaD;
// ---------------------------------------------------------------------------
#endif
