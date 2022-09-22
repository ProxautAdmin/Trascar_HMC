//---------------------------------------------------------------------------

#ifndef insarticlesH
#define insarticlesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CSPIN.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <Mask.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormInsertArticles : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TLabel *Label1;
    TEdit *EdIdArticles;
    TLabel *Label2;
    TEdit *EdName;
    TLabel *Label3;
    TEdit *EdDesc;
    TShape *Color;
    TLabel *Label9;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TADOQuery *Query;
    TColorDialog *ColorDialog1;
	TRadioGroup *TipoCarne;
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall ColorMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
public:		// User declarations
    __fastcall TFormInsertArticles(TComponent* Owner);
    bool insert ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormInsertArticles *FormInsertArticles;
//---------------------------------------------------------------------------
#endif
