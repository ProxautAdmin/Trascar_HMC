// ---------------------------------------------------------------------------

#ifndef logH
#define logH
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
class TfLog : public TMDIChild
{
__published: // IDE-managed Components

    TPanel *Panel2;
    TLabel *Label1;
    TLabel *Label3;
    TDateTimePicker *DaData;
    TCheckBox *FilterOnDateFrom;
    TCheckBox *FilterOnDateTo;
    TDateTimePicker *AData;
    TDateTimePicker *AOra;
    TDateTimePicker *DaOra;
    TEdit *Allarme;
    TEdit *Edit1;
    TDBGrid *DBGrid1;
    TDataSource *DataSource1;
    TADOQuery *Query1;
    TBitBtn *BitBtn1;
    TLabel *Tot;
    TLabel *Label2;
    TLabel *Label4;
    TEdit *Edit2;
    TLabel *Label5;
    TEdit *Edit3;
    TLabel *Label6;
    TEdit *Edit4;
    TBitBtn *BitBtn2;
    TBitBtn *btEsporta;

    void __fastcall CheckBox1Click(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall DBGrid1TitleClick(TColumn *Column);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall DBGrid1DblClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall btEsportaClick(TObject *Sender);

private: // User declarations

    AnsiString StringaExcel;

public: // User declarations

    __fastcall TfLog(TComponent* Owner);

    struct order {
        AnsiString NomeCampo;
        bool descendig;
    };

    std::vector<order>Ordinamento;
};

// ---------------------------------------------------------------------------
extern PACKAGE TfLog *fLog;
// ---------------------------------------------------------------------------
#endif
