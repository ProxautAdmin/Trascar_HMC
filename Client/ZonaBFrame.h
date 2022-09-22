//---------------------------------------------------------------------------

#ifndef ZonaBFrameH
#define ZonaBFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "MyShape.h"
#include "clientdata.h"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrZonaB : public TFrame
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *pnPosB1;
    TLabel *lbSelH;
    void __fastcall pnPosBMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations
public:		// User declarations
    __fastcall TfrZonaB(TComponent* Owner);
    void AggiornaDati() ;
    bool AbilitaConferma;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrZonaB *frZonaB;
//---------------------------------------------------------------------------
#endif
