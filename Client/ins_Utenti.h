//---------------------------------------------------------------------------

#ifndef ins_UtentiH
#define ins_UtentiH
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
class TFrmInsUtenti : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TLabel *Label1;
    TEdit *edId;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *edPassword;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TADOQuery *Query;
    TEdit *edUser;
    TLabel *Label4;
    TComboBox *cbLivello;
    TCheckBox *cbSistema;
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormActivate(TObject *Sender);

private:	// User declarations
public:		// User declarations
    __fastcall TFrmInsUtenti(TComponent* Owner);
    bool insert ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmInsUtenti *FrmInsUtenti;
//---------------------------------------------------------------------------
#endif
