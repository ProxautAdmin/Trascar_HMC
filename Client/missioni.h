//---------------------------------------------------------------------------

#ifndef missioniH
#define missioniH
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
class TfMissioni : public TMDIChild
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TLabel *Label3;
	TLabel *Label6;
	TDateTimePicker *DaData;
	TCheckBox *FilterOnDateFrom;
	TCheckBox *FilterOnDateTo;
	TDateTimePicker *AData;
	TDateTimePicker *AOra;
	TDateTimePicker *DaOra;
	TCheckBox *PrelCheck;
	TCSpinEdit *Prel;
	TCheckBox *DepCheck;
	TCSpinEdit *Dep;
	TCSpinEdit *CSpinEdit1;
	TCSpinEdit *CSpinEdit2;
	TCheckBox *CheckBox2;
	TCheckBox *CheckBox4;
	TCSpinEdit *CSpinEdit3;
	TCheckBox *CheckBox1;
    TComboBox *cbEsito;
	TDBGrid *DBGrid1;
	TDataSource *DataSource1;
	TADOQuery *Query1;
	TLabel *Label2;
	TLabel *Tot;
	TBitBtn *BitBtn1;
	TCheckBox *CheckBox3;
	TEdit *edCodUDC;
	TCheckBox *CheckBox5;
	TEdit *Edit1;
	TCheckBox *CheckBox6;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label16;
	TLabel *Label9;
	TLabel *Label12;
	TADOQuery *ADOQuery1;
    TBitBtn *BitBtn2;
	TEdit *edDropCust;
	TEdit *edPickCust;
    TCheckBox *cbMsnAttive;
    TEdit *edCM;
    TCheckBox *CheckBox7;
    TCSpinEdit *NMis;
    TCheckBox *NMisCheck;
    TBitBtn *btEsporta;
    TBitBtn *btCancellaRiga;
    TADOQuery *QueryDel;
    TCheckBox *cbMissReali;
    TBitBtn *btMssioniPlus;
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall NMisCheckClick(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall btEsportaClick(TObject *Sender);
    void __fastcall btCancellaRigaClick(TObject *Sender);
    void __fastcall btMssioniPlusClick(TObject *Sender);
private:	// User declarations
    AnsiString StringaExcel;
public:		// User declarations
	__fastcall TfMissioni(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TfMissioni *fMissioni;
//---------------------------------------------------------------------------
#endif
