//---------------------------------------------------------------------------

#ifndef articlesH
#define articlesH
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
class TFormArticles : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TDBGrid *DBGrid1;
    TADOQuery *ADOQuery1;
    TDataSource *DataSource1;
    TPanel *Panel5;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TBitBtn *BitBtn3;
    TBitBtn *BitBtn4;
    TADOQuery *QueryDel;
    TGroupBox *GroupBox1;
    TEdit *Edit1;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *Edit3;
    TLabel *Label6;
    TLabel *Tot;
    TCheckBox *CheckBox1;
    TShape *Shape1;
    TColorDialog *ColorDialog1;
	TEdit *Edit2;
	TLabel *Label4;
	TComboBox *cbTipoCarne;
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
    void __fastcall Shape1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormArticles(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormArticles *FormArticles;
//---------------------------------------------------------------------------
#endif
