// ---------------------------------------------------------------------------

#ifndef dmgestionedatabaseH
#define dmgestionedatabaseH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "db.h"
#include <Vcl.ExtCtrls.hpp>

class TdmDBServer : public TDataModule
{
__published: // IDE-managed Components

	TTimer *TimerMissioniDB;
	TTimer *TimerAggiornamentiMinuti;
	TTimer *TimerStanza;

	void __fastcall TimerMissioniDBTimer(TObject *Sender);
	void __fastcall DataModuleCreate(TObject *Sender);
	void __fastcall TimerAggiornamentiMinutiTimer(TObject *Sender);
	void __fastcall TimerStanzaTimer(TObject *Sender);

private: // User declarations
		public : // User declarations
	__fastcall TdmDBServer(TComponent* Owner);
	void NuovoRecordConsumoBatteria(int idagv);
	int AggiornaStatoCentroMissioni(int id, int stato);
	int UpdateStatoPorta(int porta, int stato);
	int RicercaNuovaMissionedaGenerare(int stato = 0, int cod_tipo_mov = 0);
	int AggiornaMissione(int id, int stato, int agv, int peso = 0);
	void ComponiMessaggioAllarme(int num_messaggio, int id_messaggio, AnsiString params1 = "", AnsiString params2 = "");
	void AbortMission(int idmiss);
	void RicercaMissioneIntermediaInCentroMissioni(int idartudc);
	int CreaIdMissione();
	int GeneraMissione(TMissione m);
	void AggiornaStrutturaPLC();
	int AggiornaStatoOrdineTabelleAutoStart();
	int RitornaTipologiaMissioneDaMissioni(int id);

	void RitornaCaricoScarico(int tipomis, int pos_prel, int pos_dep, int pianoprel, int pianodep, int &tabcarico, int &tabscarico);

	int RitornaIndicePostazione(int pos);
	int SegnalaCaricoSenzaCodice();
	int RitornaPosDaCM(int pos);
	int RitornaPosDaMiss(int pos, AnsiString tipopos);
	void MissionePrelievoManuale(int buttontag);
	void RitornaNodoPassaggio(int agv, int posprel, int posdep, int latoforcheprel, int latoforchedep, int tipomiss, int &pospassanteprel, int &pospassantedep);
	int CercaZona(int xx, int yy);
	int GeneraCMDaFaE(int val);

	int GeneraCMDaPrelievo(int val);
	int GeneraCMDaGaA1(int val);
	int GeneraCMDaHaB(int val);
	int GeneraCMDaIaA2(int val);


	int minuto_prec, letto_data_peso_campione;
	AnsiString DataOraPesoCampione;
	TRecordList TabToProxaut;
};

// ---------------------------------------------------------------------------
extern PACKAGE TdmDBServer *dmDBServer;
// ---------------------------------------------------------------------------
#endif
