// ---------------------------------------------------------------------------

#ifndef dmgestioneH
#define dmgestioneH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "clientdata.h"

#define ABBASSASEGNALIPORTA		0
#define APRIPORTA		1
#define APRIPORTAMETA	2
#define CHIUDIPORTA		3

// ---------------------------------------------------------------------------
class TDMGestioneEventi : public TDataModule
{
__published: // IDE-managed Components

    TTimer *TimerEventi;
    TTimer *TimerMissioni;
    TTimer *TimerPuliziaStorici;

    void __fastcall TimerEventiTimer(TObject *Sender);
    void __fastcall DataModuleCreate(TObject *Sender);
    void __fastcall TimerMissioniTimer(TObject *Sender);
    void __fastcall TimerPuliziaStoriciTimer(TObject *Sender);

private: // User declarations
        public : // User declarations
    __fastcall TDMGestioneEventi(TComponent* Owner);
    int CambiaDestinazioneAMissione(int posprel, int posdepvecchia, int posdepnuova);


    bool sync_lamp, init;
    TDateTime t;
};

// ---------------------------------------------------------------------------
extern PACKAGE TDMGestioneEventi *DMGestioneEventi;
// ---------------------------------------------------------------------------
#endif
