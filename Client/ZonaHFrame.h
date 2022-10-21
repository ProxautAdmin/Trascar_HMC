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
#include "frame_ArticoliSovrapposti.h"
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.Grids.hpp>

// ---------------------------------------------------------------------------
class TfrZonaH : public TFrame
{
__published: // IDE-managed Components

    TPanel *Panel1;
    TPanel *pnPosH1;
    TPanel *pnPosH2;
    TPanel *pnPosH3;
    TPanel *pnPosH4;
    TImageList *ImageListMenu;
    TPopupMenu *PopupMenu;
    TFrameSovrapposti *FrameSovrapposti;
    TADOQuery *ADOQuery1;
    TDataSource *DataSource1;
    TPanel *pInfoUDC;
    TLabel *Label2;
    TLabeledEdit *leDescArticolo;
    TLabeledEdit *leIdUDC;
    TLabeledEdit *leCodArt;
    TCheckBox *ckImpilabile;

    void __fastcall pnPosH1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Carica1Click(TObject * Sender);
    void __fastcall Carica2Click(TObject * Sender);
    void __fastcall SvuotaPosizioneClick(TObject * Sender);

private: // User declarations
        public : // User declarations
    __fastcall TfrZonaH(TComponent* Owner);
    void AggiornaDati();
    int CercaConCodart(AnsiString CodArt) ;
    int FillDescrizione(int idudc);

            bool AbilitaConferma;
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrZonaH *frZonaH;
// ---------------------------------------------------------------------------
#endif
