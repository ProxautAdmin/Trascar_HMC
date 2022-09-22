// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AGVFrameL.h"
#include "SocketComunicazioneClient.h"
#include "main.h"
#include "db.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrAgvL *frAgvL;

// ---------------------------------------------------------------------------
__fastcall TfrAgvL::TfrAgvL(TComponent* Owner)
    : TFrame(Owner)
{

}
// ---------------------------------------------------------------------------

void __fastcall TfrAgvL::ForzaIdCoilsuAgv1Click(TObject *Sender)
{
    //
    AnsiString code;
    if ((ClientData.DatiAgv[idagv].load)||(ClientData.DatiAgv[idagv].DatiUDC.IDUDC)) {
        code = InputBox(L"Insert Id UDC", L"Id UDC", L"0");
        if (code != "0") {
            SocketDataModule->InviaIdUdcAgv(code.ToIntDef(0),idagv) ;
        }
    }

}
// ---------------------------------------------------------------------------



