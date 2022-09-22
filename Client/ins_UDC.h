// ---------------------------------------------------------------------------

#ifndef ins_UDCH
#define ins_UDCH
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
class TFormInsUDC : public TForm
{
__published: // IDE-managed Components
    TPanel *pnTitolo;
    TPanel *Panel2;
    TPanel *pnUDC;
    TBitBtn *btConferma;
    TBitBtn *btEsci;
    TADOQuery *Query;
    TEdit *edIDUDC;
    TLabel *Label5;
    TLabel *Label3;
    TLabel *Label1;
    TComboBox *cbTipoUDC;
    TComboBox *cbStatus;
    TEdit *edCodUDC;
    TLabel *Label4;
    TLabel *Label6;
    TEdit *edDescUDC;
    TPanel *pnArticolo;
    TPanel *pnContenitore;
    TEdit *edCodArt;
    TLabel *Label2;
    TLabel *Label7;
    TEdit *edLotto;
    TLabel *Label8;
    TEdit *edPeso;
    TCheckBox *cbRiservato;
    TCheckBox *cbParziale;
    TEdit *edIntImp;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *lbDescArt;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *lbDescContenitore;
    TComboBox *cbBase;

    void __fastcall btEsciClick(TObject *Sender);
    void __fastcall btConfermaClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormActivate(TObject *Sender);

private: // User declarations
        public : // User declarations
    __fastcall TFormInsUDC(TComponent* Owner);

    bool insert;
    int idtabella;
};

// ---------------------------------------------------------------------------
extern PACKAGE TFormInsUDC *FormInsUDC;
// ---------------------------------------------------------------------------
#endif
