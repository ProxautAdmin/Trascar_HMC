// ---------------------------------------------------------------------------

#ifndef ZonaFFrameH
#define ZonaFFrameH
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

// ---------------------------------------------------------------------------
class TfrZonaF : public TFrame
{
__published: // IDE-managed Components

    TPanel *Panel1;
    TPanel *pnPosF1;
    TPanel *pnPosF2;
    TLabel *lbSelF;
    TImageList *ImageListMenu;
    TPopupMenu *PopupMenu;

    void __fastcall pnPosF1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);


private: // User declarations
        public : // User declarations
    __fastcall TfrZonaF(TComponent* Owner);
    void AggiornaDati();
            bool AbilitaConferma;
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrZonaF *frZonaF;
// ---------------------------------------------------------------------------
#endif
