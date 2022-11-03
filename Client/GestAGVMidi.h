// ---------------------------------------------------------------------------

#ifndef GestAGVMidiH
#define GestAGVMidiH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "ChildWin.h"
#include "cspin.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
// #include "ZonaA1Frame.h"
// #include "ZonaHFrame.h"
#include <vector>

// ---------------------------------------------------------------------------
class TfGestAGVMidi : public TMDIChild
{
__published: // IDE-managed Components

    TPanel *Panel2;
    TTimer *TimerRef;
    TADOQuery *Query;
    TBitBtn *btConferma;
    TGroupBox *gbMain;
    TPanel *PanelPrel;
    TPanel *PanelDest;
    TBitBtn *btRefresh;
    TDataSource *DataSource1;
    TADOQuery *Query1;
    TPageControl *pcPrel;
    TTabSheet *tsZonaA1Prel;
    TPanel *pnOptionPrel;
    TPanel *pnOptioDep;
    TPageControl *pcDest;
    TTabSheet *tsZonaA1Dest;
    TTabSheet *tsZonaGPrel;
    TTabSheet *tsZonaGDest;
    TTabSheet *tsZonaHPrel;
    TTabSheet *tsZonaHDest;
    TTabSheet *tsZonaIPrel;
    TTabSheet *tsZonaIDest;
    TTabSheet *tsZonaDPrel;
    TTabSheet *tsZonaDDest;
    TTabSheet *tsZonaA2Prel;
    TTabSheet *tsZonaA2Dest;
    TTabSheet *tsZonaCPrel;
    TTabSheet *tsZonaCDest;
    TTabSheet *tsZonaBDest;
    TTabSheet *tsZonaBPrel;
    TTabSheet *tsZonaEPrel;
    TTabSheet *tsZonaEDest;
    TTabSheet *tsZonaFPrel;
    TTabSheet *tsZonaFDest;
    TLabel *Label1;
    TComboBox *cbPriorita;
    TPanel *pLinea;
    TLabel *Label2;
    TComboBox *cbLinea;

    void __fastcall TimerRefTimer(TObject *Sender);
    void __fastcall BitBtnCloseClick(TObject *Sender);
    void __fastcall btConfermaClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall btRefreshClick(TObject *Sender);

private: // User declarations
        public : // User declarations
    __fastcall TfGestAGVMidi(TComponent* Owner);

    struct order {
        AnsiString NomeCampo;
        bool descendig;
    };

    std::vector<order>Ordinamento;
    void AggiornaDatiLocali();
    void TabVisibili(bool tab0 = 0, bool tab1 = 0, bool tab2 = 0, bool tab3 = 0, bool tab4 = 0, bool tab5 = 0, bool tab6 = 0, bool tab7 = 0, bool tab8 = 0, bool tab9 = 0);
    void TornaPosPrelSelezionata(AnsiString Zona, int &pos, int &piano, int &tipoposizione, int &idudc);
    void SettaPianoSelezionatoPerZona(int setta, AnsiString zona);
    void SettaPianoSelezionatoPerPos(int setta, int pos);

    int pos, zona, posselezionata, destselezionata, impila;
};

// ---------------------------------------------------------------------------
extern PACKAGE TfGestAGVMidi *fGestAGVMidi;
// ---------------------------------------------------------------------------
#endif
