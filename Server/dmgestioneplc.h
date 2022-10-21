// ---------------------------------------------------------------------------

#ifndef dmgestioneplcH
#define dmgestioneplcH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ExtCtrls.hpp>
// ---------------------------------------------------------------------------
#include "clientdata.h"

class TDMPlc : public TDataModule
{
__published: // IDE-managed Components

    TTimer *TimerEventi;
    TTimer *TimerPlc;
    TTimer *TimerWatchDogPlc;
    TTimer *TimerCarScar;

    void __fastcall TimerEventiTimer(TObject *Sender);
    void __fastcall DataModuleCreate(TObject *Sender);
    void __fastcall TimerPlcTimer(TObject *Sender);
    void __fastcall TimerCarScarTimer(TObject *Sender);
    void __fastcall TimerWatchDogPlcTimer(TObject *Sender);

private: // User declarations
        public : // User declarations
    __fastcall TDMPlc(TComponent* Owner);
    void ModificaByteStrutturaPlc(int input, int posplc, int byte, int value, int idx=0);

   void ControlloIngressoPrelievi(DatiPlc &Plc, DatiPrelievo &Prelievo)  ;
   void ControlloIngressoDepositi(DatiPlc &Plc, DatiDeposito &Deposito) ;

    void CompilaStrutturaPrelievo(DatiPlc & Plc, DatiPrelievo & Prelievo);
    void CompilaStrutturaDeposito(DatiPlc & Plc, DatiDeposito & Deposito);

    int AgvInIngombroPosizione(DatiPrelievo &Prelievo, DatiDeposito &Deposito);


    void ControlloStatoDepositi(DatiPlc & Plc, DatiDeposito & Deposito)  ;
    void ControlloStatoPrelievi(DatiPlc & Plc, DatiPrelievo & Prelievo) ;


    int ScriviIdPalletPosizioneDiDeposito(int piano, int posizione, int agv);
    int InserisciIdPalletInAgv(int piano, int posizione,  int agv);


	int lettodatiporte, primogiroplc;
    bool sync_lamp, init;


    void LeggiStrutturaDatiSegnali();

};

// ---------------------------------------------------------------------------
extern PACKAGE TDMPlc *DMPlc;
// ---------------------------------------------------------------------------
#endif
