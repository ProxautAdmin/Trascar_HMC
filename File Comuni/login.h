//---------------------------------------------------------------------------

#ifndef loginH
#define loginH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <Buttons.hpp>
#include <DB.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormLogin : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TLabel *Label1;
    TEdit *Edit1;
    TLabel *Label2;
    TEdit *Edit2;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn3;
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall BitBtn3Click(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormLogin(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormLogin *FormLogin;
//---------------------------------------------------------------------------
#endif
