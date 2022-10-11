//---------------------------------------------------------------------------

#ifndef frame_ArticoliSovrappostiH
#define frame_ArticoliSovrappostiH
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
class TFrameSovrapposti : public TFrame
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TDBGrid *DBGrid1;
    TADOQuery *ADOQuery1;
    TDataSource *DataSource1;
    void __fastcall btScegliClick(TObject *Sender);
    void __fastcall FrameEnter(TObject *Sender);

private:	// User declarations
    int ccont;
public:		// User declarations
	__fastcall TFrameSovrapposti(TComponent* Owner);
	void RiempiCampi() ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameSovrapposti *FrameSovrapposti;
//---------------------------------------------------------------------------
#endif
