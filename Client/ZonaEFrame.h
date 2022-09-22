//---------------------------------------------------------------------------

#ifndef ZonaEFrameH
#define ZonaEFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "MyShape.h"
#include "clientdata.h"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrZonaE : public TFrame
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *pnPosE1;
    TLabel *lbSelH;
    void __fastcall pnPosEMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations
public:		// User declarations
    __fastcall TfrZonaE(TComponent* Owner);
    void AggiornaDati() ;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrZonaE *frZonaE;
//---------------------------------------------------------------------------
#endif
