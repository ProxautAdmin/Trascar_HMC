//---------------------------------------------------------------------------

#ifndef frame_ArticoliInLavorazioneH
#define frame_ArticoliInLavorazioneH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "clientdata.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.Grids.hpp>

//---------------------------------------------------------------------------
class TFrameArticoliInLavorazione : public TFrame
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TDBGrid *DBGrid1;
    TADOQuery *ADOQuery1;
    TDataSource *DataSource1;
    void __fastcall btScegliClick(TObject *Sender);
    void __fastcall FrameEnter(TObject *Sender);
    void __fastcall DBGrid1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

private:	// User declarations
    int ccont;
public:		// User declarations
	__fastcall TFrameArticoliInLavorazione(TComponent* Owner);
	void RiempiCampi() ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameArticoliInLavorazione *FrameArticoliInLavorazione;
//---------------------------------------------------------------------------
#endif
