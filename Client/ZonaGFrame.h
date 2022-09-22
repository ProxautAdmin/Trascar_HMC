//---------------------------------------------------------------------------

#ifndef ZonaGFrameH
#define ZonaGFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "MyShape.h"
#include "clientdata.h"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrZonaG : public TFrame
{
__published:	// IDE-managed Components
    TPanel *pnG;
    TPanel *pnPosG1;
    TPanel *pnPosG2;
    TPanel *pnPosG3;
    TPanel *pnPosG4;
    TPanel *pnPosG5;
    TPanel *pnPosG6;
    TPanel *pnPosG7;
    TPanel *pnPosG8;
    TPanel *pnPosG9;
    TPanel *pnPosG10;
    TPanel *pnPosG11;
    TPanel *pnPosG12;
    TPanel *pnPosG13;
    TPanel *pnExtra;
    TLabel *Label1;
    TComboBox *cbNumero;
    void __fastcall pnPosGMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations
public:		// User declarations
    __fastcall TfrZonaG(TComponent* Owner);
    void AggiornaDati() ;
            bool AbilitaConferma;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrZonaG *frZonaG;
//---------------------------------------------------------------------------
#endif
