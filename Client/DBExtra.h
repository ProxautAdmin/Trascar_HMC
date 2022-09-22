//---------------------------------------------------------------------------

#ifndef DBExtraH
#define DBExtraH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "ChildWin.h"
#include "cspin.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <vector>
//---------------------------------------------------------------------------
class TfDBExtra : public TMDIChild
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TLabel *Label1;
	TLabel *Label3;
	TEdit *edID;
    TEdit *edDaSME;
	TDBGrid *DBGrid1;
	TDataSource *DataSource1;
	TADOQuery *Query1;
	TLabel *Tot;
	TLabel *Label2;
    TEdit *edPos;
    TEdit *edPriorita;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TEdit *edCodUDC;
    TEdit *edCodOperatore;
    TLabel *Label9;
    TBitBtn *BitBtnDel;
    TBitBtn *BitBtnIns;
    TBitBtn *BitBtnVar;
    TADOQuery *QueryDel;
	TCheckBox *cbMsnAttive;
    TDateTimePicker *DaOra;
    TDateTimePicker *AOra;
    TDateTimePicker *AData;
    TCheckBox *FilterOnDateTo;
    TCheckBox *FilterOnDateFrom;
    TDateTimePicker *DaData;
    TBitBtn *btRefresh;
	void __fastcall CheckBox1Click(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall DBGrid1TitleClick(TColumn *Column);
	void __fastcall DBGrid1DblClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
    void __fastcall BitBtnDelClick(TObject *Sender);
    void __fastcall BitBtnInsClick(TObject *Sender);
    void __fastcall BitBtnVarClick(TObject *Sender);
    void __fastcall btRefreshClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfDBExtra(TComponent* Owner);
    struct order {
		AnsiString NomeCampo ;
        bool descendig ;
	};
	std::vector<order> Ordinamento ;
};
//---------------------------------------------------------------------------
extern PACKAGE TfDBExtra *fDBExtra;
//---------------------------------------------------------------------------
#endif
