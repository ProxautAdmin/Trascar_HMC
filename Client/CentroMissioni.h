//---------------------------------------------------------------------------

#ifndef CentroMissioniH
#define CentroMissioniH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "ChildWin.h"
#include "cspin.h"
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TfCentroMissioni : public TMDIChild
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TLabel *Label3;
	TLabel *Label5;
	TLabel *Label7;
	TLabel *Label4;
	TLabel *Label8;
	TCheckBox *FilterOnDateTo;
	TDateTimePicker *AData;
	TDateTimePicker *AOra;
	TCheckBox *PrelCheck;
	TCSpinEdit *Prel;
	TCheckBox *DepCheck;
	TCSpinEdit *Dep;
	TCheckBox *NMisCheck;
	TCSpinEdit *NMis;
	TCSpinEdit *CSpinEdit1;
	TCSpinEdit *CSpinEdit2;
	TCheckBox *CheckBox2;
	TCheckBox *CheckBox1;
	TDBGrid *DBGrid1;
	TDataSource *DataSource1;
	TADOQuery *Query1;
	TLabel *Label2;
	TLabel *Tot;
	TBitBtn *BitBtn1;
	TCheckBox *CheckBox3;
	TEdit *Destinazione1;
	TCheckBox *CheckBox5;
	TEdit *Edit1;
	TADOQuery *ADOQuery1;
    TComboBox *cbEsito;
    TCheckBox *CheckBox7;
    TBitBtn *BitBtn2;
    TBitBtn *btDelRiga;
    TADOQuery *QueryDel;
	TEdit *edCodUDC;
    TEdit *edPrelievo;
    TCheckBox *cbPrelievo;
    TBitBtn *btEsporta;
    TBitBtn *btGestCMiss;
    TBitBtn *btBarcode;
    TBitBtn *BitBtn3;
    TDateTimePicker *DaOra;
    TDateTimePicker *DaData;
    TCheckBox *FilterOnDateFrom;
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall NMisCheckClick(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall btDelRigaClick(TObject *Sender);
    void __fastcall btEsportaClick(TObject *Sender);
    void __fastcall BitBtn3Click(TObject *Sender);
private:	// User declarations
        AnsiString StringaExcel;
public:		// User declarations
	__fastcall TfCentroMissioni(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TfCentroMissioni *fCentroMissioni;
//---------------------------------------------------------------------------
#endif
