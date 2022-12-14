// ---------------------------------------------------------------------------

#ifndef ZonaA1FrameH
#define ZonaA1FrameH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "MyShape.h"
#include "clientdata.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include "frame_ArticoliInLavorazione.h"
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.Grids.hpp>

// ---------------------------------------------------------------------------
class TfrZonaA1 : public TFrame
{
__published: // IDE-managed Components

    TPanel *pnA;
    TPanel *pnPosA1;
    TPanel *pnPosA2;
    TPanel *pnPosA3;
    TPanel *pnPosA4;
    TPanel *pnPosA5;
    TPanel *pnPosA6;
    TPanel *pnPosA7;
    TPanel *pInfoUDC;
    TLabeledEdit *leDescArticolo;
    TLabeledEdit *leCodArt;
    TLabeledEdit *leIdUDC;
    TLabel *Label2;
    TCheckBox *ckImpilabile;
    TADOQuery *ADOQuery1;
    TDataSource *DataSource1;
    TFrameArticoliInLavorazione *FrameArticoliInLavorazione;
    TADOQuery *ADOQuery2;
    TDataSource *DataSource2;
    TMemo *Memo1;

    void __fastcall pnPosAMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall pnPosADblClick(TObject *Sender);

private: // User declarations

        public : // User declarations
    __fastcall TfrZonaA1(TComponent* Owner);
	void AggiornaDati() ;
	int CercaConCodart(int riga);
    void AggiornaGriglia() ;

    bool AbilitaConferma;

};

// ---------------------------------------------------------------------------
extern PACKAGE TfrZonaA1 *frZonaA1;
// ---------------------------------------------------------------------------
#endif
