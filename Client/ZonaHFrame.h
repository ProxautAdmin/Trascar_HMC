// ---------------------------------------------------------------------------

#ifndef ZonaHFrameH
#define ZonaHFrameH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "MyShape.h"
#include "clientdata.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Buttons.hpp>

// ---------------------------------------------------------------------------
class TfrZonaH : public TFrame
{
__published: // IDE-managed Components

    TPanel *Panel1;
    TPanel *pnPosH1;
    TPanel *pnPosH2;
    TPanel *pnPosH3;
    TLabel *lbSelH;
    TPanel *pnPosH4;
    TImageList *ImageListMenu;
    TPopupMenu *PopupMenu;
    TPanel *Panel2;
    TLabel *Label1;
    TSpeedButton *sbCerca;
    TEdit *tDescArticolo;
    TEdit *edIDArt;
    TEdit *edCodArt;
    TLabel *lIdUDC;

    void __fastcall pnPosH1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Carica1Click(TObject * Sender);
    void __fastcall Carica2Click(TObject * Sender);
    void __fastcall SvuotaPosizioneClick(TObject * Sender);
    void __fastcall sbCercaClick(TObject *Sender);

private: // User declarations
        public : // User declarations
    __fastcall TfrZonaH(TComponent* Owner);
    void AggiornaDati();
            bool AbilitaConferma;
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrZonaH *frZonaH;
// ---------------------------------------------------------------------------
#endif
