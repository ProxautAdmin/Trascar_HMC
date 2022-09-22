//---------------------------------------------------------------------------

#ifndef GestUserH
#define GestUserH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
//---------------------------------------------------------------------------
class TFrmGestUser : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TBitBtn *btEsci;
    TBitBtn *btDel;
    TBitBtn *btIns;
    TBitBtn *btMod;
    TPanel *Panel3;
    TPanel *Panel4;
    TDBGrid *DBGrid1;
    TPanel *Panel5;
    TGroupBox *GroupBox1;
    TLabel *Label3;
    TLabel *Label6;
    TLabel *Tot;
    TEdit *edUtente;
    TADOQuery *ADOQuery1;
    TADOQuery *QueryDel;
    TDataSource *DataSource1;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall btModClick(TObject *Sender);
    void __fastcall btInsClick(TObject *Sender);
    void __fastcall ADOQuery1AfterScroll(TDataSet *DataSet);
    void __fastcall btEsciClick(TObject *Sender);
    void __fastcall btDelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFrmGestUser(TComponent* Owner);

    void TFrmGestUser::edFunctionChange();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmGestUser *FrmGestUser;
//---------------------------------------------------------------------------
#endif
