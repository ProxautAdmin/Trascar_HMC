// ---------------------------------------------------------------------------

#ifndef anagrafica_articoliH
#define anagrafica_articoliH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <DBGrids.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>

// ---------------------------------------------------------------------------
class TFormAnagraficaArticoli : public TForm
{
__published: // IDE-managed Components

    TPanel *Panel1;
    TPanel *Panel3;
    TPanel *Panel4;
    TDBGrid *DBGrid1;
    TADOQuery *ADOQuery1;
    TDataSource *DataSource1;
    TPanel *Panel5;
    TADOQuery *QueryDel;
    TGroupBox *GroupBox1;
    TLabel *lb04;
    TLabel *Label6;
    TLabel *Tot;
    TEdit *edCodArt;
    TPanel *Panel2;
    TPanel *Panel6;
    TBitBtn *BitBtn8;
    TBitBtn *BitBtn3;
    TBitBtn *BitBtn2;
    TBitBtn *BitBtn1;
    TEdit *edFase;
    TLabel *lb03;
    TEdit *edDescrizione;
    TLabel *lb02;
    TEdit *edEdIDArt;
    TLabel *lb01;
    TBitBtn *btInsInPos;

    void __fastcall DBGrid1DblClick(TObject *Sender);
    void __fastcall BitBtn4Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall BitBtn3Click(TObject *Sender);
    void __fastcall FormDeactivate(TObject *Sender);
    void __fastcall DBGrid1DrawColumnCell(TObject *Sender,
        const TRect &Rect, int DataCol, TColumn *Column,
        TGridDrawState State);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall CheckBox1Click(TObject *Sender);
    void __fastcall Edit1Change(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall ADOQuery1AfterScroll(TDataSet *DataSet);
    void __fastcall btInsInPosClick(TObject *Sender);
    void __fastcall edEdIDArtExit(TObject *Sender);

private: // User declarations
        public : // User declarations
    __fastcall TFormAnagraficaArticoli(TComponent* Owner);

    void Filtra();

    int insert_produzione, IdArticolo, artsel, idartudc;
    AnsiString ProductName;
};

// ---------------------------------------------------------------------------
extern PACKAGE TFormAnagraficaArticoli *FormAnagraficaArticoli;
// ---------------------------------------------------------------------------
#endif
