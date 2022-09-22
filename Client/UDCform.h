// ---------------------------------------------------------------------------

#ifndef UDCformH
#define UDCformH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "frame_modulo.h"
#include "UDCFrame.h"
#include "MyShape.h"

// ---------------------------------------------------------------------------
class TFormDatiUDC : public TForm
{
__published: // IDE-managed Components

    TGroupBox *GroupBox1;
    TEdit *eUDC;
    TLabel *Label13;
    TGroupBox *GroupBox3;
    TEdit *eIDArtUDCm;
    TLabel *Label21;
    TPanel *Panel4;
    TBitBtn *BitBtnClose;
    TfUDCSmall *fUDCSmall;
    TEdit *edArticolo;
    TLabel *Label1;

    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall BitBtnCloseClick(TObject *Sender);
    void __fastcall fUDCSmallsbSearchClick(TObject *Sender);

private: // User declarations
        public : // User declarations
    __fastcall TFormDatiUDC(TComponent* Owner);
    void VisualizzaDatiUDC();

    int IDUDC, pos_update;
};

// ---------------------------------------------------------------------------
extern PACKAGE TFormDatiUDC *FormDatiUDC;
// ---------------------------------------------------------------------------
#endif
