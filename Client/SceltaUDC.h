//---------------------------------------------------------------------------

#ifndef SceltaUDCH
#define SceltaUDCH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormSceltaUDC : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel4;
    TBitBtn *BitBtn4;
    TBitBtn *BitBtn5;
    TBitBtn *btAll;
    TBitBtn *btNone;
    TGroupBox *GroupBox4;
    TCheckBox *cb1;
    TGroupBox *GroupBox1;
    TLabel *lbZona;
    TLabel *lbRoom;
    TLabel *lbCustPos;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall BitBtn5Click(TObject *Sender);
    void __fastcall BitBtn4Click(TObject *Sender);
    void __fastcall btAllClick(TObject *Sender);
    void __fastcall btNoneClick(TObject *Sender);
    void __fastcall FormDeactivate(TObject *Sender);

private:	// User declarations
public:		// User declarations
    __fastcall TFormSceltaUDC(TComponent* Owner);
    int pos    ;
    AnsiString zonaparam,strselected;

};
//---------------------------------------------------------------------------
extern PACKAGE TFormSceltaUDC *FormSceltaUDC;
//---------------------------------------------------------------------------
#endif
