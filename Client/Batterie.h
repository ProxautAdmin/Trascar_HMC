// ---------------------------------------------------------------------------

#ifndef BatterieH
#define BatterieH
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
class TfBatterie : public TMDIChild
{
__published: // IDE-managed Components

    TPanel *Panel2;
    TLabel *Label3;
    TLabel *Label5;
    TLabel *Label6;
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
    TCSpinEdit *cLastPosfrom;
    TCSpinEdit *cLastPosTo;
    TCheckBox *cbBatteryState;
    TCheckBox *CheckBox4;
    TCSpinEdit *CSpinEdit3;
    TDBGrid *DBGrid1;
    TDataSource *DataSource1;
    TADOQuery *Query1;
    TLabel *Label2;
    TLabel *Tot;
    TBitBtn *BitBtn1;
    TCheckBox *CheckBox3;
    TEdit *edBatteryState;
    TCheckBox *CheckBox5;
    TEdit *Edit1;
    TCheckBox *cbDestination;
    TLabel *Label11;
    TLabel *Label9;
    TLabel *Label12;
    TLabel *Label10;
    TLabel *Label15;
    TBitBtn *BitBtn2;
    TEdit *edAGVState;
    TEdit *edDestination;
    TDataSource *DataSourceRep;
    TADOQuery *QueryRep;
    TADOQuery *QueryBat;
    TBitBtn *btStat;
    TADOQuery *QueryVis;
    TLabel *lbTot;
    TDBGrid *DBGrid2;
    TLabel *Label1;

    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall NMisCheckClick(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall btStatClick(TObject *Sender);

private: // User declarations
        public : // User declarations
    __fastcall TfBatterie(TComponent* Owner);
   void Report();
};

// ---------------------------------------------------------------------------
extern PACKAGE TfBatterie *fBatterie;
// ---------------------------------------------------------------------------
#endif
