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
    void __fastcall TimerWatchDogPlcTimer(TObject *Sender);
    void __fastcall TimerCarScarTimer(TObject *Sender);

private: // User declarations
        public : // User declarations
    __fastcall TDMPlc(TComponent* Owner);
    void ModificaByteStrutturaPlc(int input, int posplc, int byte, int value, int idx=0);
    void CaricaStatoPosizioneInStruttura();

 

    void CompilaStrutturaPrelievo(DatiPlc & Plc, DatiPrelievo & Prelievo);
    void CompilaStrutturaDeposito(DatiPlc & Plc, DatiDeposito & Deposito);
    void CompilaStrutturaSegnaliGenerali(DatiPlc & Plc, DatiSegnaliGenerali & SegnaliGenerali);

    int AgvInIngombroPosizione(DatiPrelievo &Prelievo, DatiDeposito &Deposito);

    void ResettaRichiestaIngressoBaiaPrelievo(DatiPrelievo &Prelievo);
    void ResettaRichiestaIngressoBaiaDeposito(DatiDeposito &Deposito);

    int AgvInIngombroPosizionePrelievo(DatiPrelievo &Prelievo);
    int AgvInIngombroPosizioneDeposito(DatiDeposito &Deposito);

    void GestioneIngombroAgvPrelievo(DatiPrelievo &Prelievo);
    void GestioneIngombroAgvDeposito(DatiDeposito &Deposito);

    void GestioneRichiestaIngressoBaiaPrelievo(DatiPrelievo &Prelievo);
    void GestioneRichiestaIngressoBaiaDeposito(DatiDeposito &Deposito);


    int ScriviIdPalletPosizioneDiDeposito(int piano, int posizione, int agv);
    int InserisciIdPalletInAgv(int piano, int posizione,  int agv);

	void GestioneStandbyEmergenza(DatiSegnaliGenerali & SegnaliGenerali);

	int lettodatiporte, primogiroplc;
    bool sync_lamp, init;

    int AgvInIngombroPosizioneDepositoNuovo(DatiDeposito & Deposito);
    int AgvInIngombroPosizionePrelievoNuovo(DatiPrelievo & Prelievo);

    void LeggiStrutturaDatiSegnali();

};

// ---------------------------------------------------------------------------
extern PACKAGE TDMPlc *DMPlc;
// ---------------------------------------------------------------------------
#endif
