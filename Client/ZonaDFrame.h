// ---------------------------------------------------------------------------

#ifndef ZonaDFrameH
#define ZonaDFrameH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "MyShape.h"
#include "clientdata.h"
#include <Vcl.ExtCtrls.hpp>

// ---------------------------------------------------------------------------
class TfrZonaD : public TFrame
{
__published: // IDE-managed Components

    TPanel *pnD;
    TButton *btSvuotaZonaD;
    void __fastcall btSvuotaZonaDClick(TObject *Sender);

private: // User declarations
        public : // User declarations
    __fastcall TfrZonaD(TComponent* Owner);
    void AggiornaDati();
    bool AbilitaConferma ;
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrZonaD *frZonaD;
// ---------------------------------------------------------------------------
#endif
