//---------------------------------------------------------------------------

#ifndef ZonaCFrameH
#define ZonaCFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "MyShape.h"
#include "clientdata.h"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrZonaC : public TFrame
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *pnPosC1;
    TLabel *lbSelH;
    void __fastcall pnPosCMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations
public:		// User declarations
    __fastcall TfrZonaC(TComponent* Owner);
    void AggiornaDati() ;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrZonaC *frZonaC;
//---------------------------------------------------------------------------
#endif
