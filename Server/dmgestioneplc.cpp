// ---------------------------------------------------------------------------

#pragma hdrstop

#include "dmgestioneplc.h"
#include "main.h"
#include "db.h"
#include "PLCphoenixThread.h"
#include "dmFunzioniComuniClientServer.h"
#include "socket.h"
#include "datamodulecomandiagv.h"
#include "dmgestionedatabase.h"
#include "ExtraFunction.h"
#include "DBImpianto.h"
#include "PLCThread.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TDMPlc *DMPlc;
extern CLIENTSOCKET *cs;

// ---------------------------------------------------------------------------

__fastcall TDMPlc::TDMPlc(TComponent* Owner) : TDataModule(Owner) {
	AnsiString strsql;
	TADOQuery *ADOQuery;
	init = true;
	int PosPrel[] = {601};
	int PosPrel_FuoriIngombro[] = {651};
	int PosDep[] = {602, 603};
	int PosDep_FuoriIngombro[] = {163, 653};
	// inizializzo struttura
	for (int i = 1; i <= N_PLC; i++) {
		// num. plc
		ClientData.Plc[i].id_thread_plc = i - 1;
		// zona
		ClientData.Plc[i].Zona = i; // uso futuro?
		// postazioni
		ClientData.Plc[i].SegnaliGenerali[1].dbInput = 41;
		ClientData.Plc[i].SegnaliGenerali[1].dbOutput = 40;

		ClientData.Plc[i].SegnaliGenerali[1].byteOutput_accesso = 0;
		ClientData.Plc[i].SegnaliGenerali[1].bitOutput_conferma_accesso = 0;

		ClientData.Plc[i].SegnaliGenerali[1].byteInput_accesso = 0;
		ClientData.Plc[i].SegnaliGenerali[1].bitInput_conferma_accesso = 0;
		ClientData.Plc[i].SegnaliGenerali[1].bitInput_accesso_terminato = 0;

		for (int j = 1; j <= NUM_PRELIEVOPLC; j++) {
			for (int p = 1; p <= NUMPIANI_PREL; p++) {
				// PRELIEVO
				ClientData.Plc[i].Prelievo[j][p].idplc = IDX_PLCPRELIEVO;
				ClientData.Plc[i].Prelievo[j][p].dbInput = 45;
				ClientData.Plc[i].Prelievo[j][p].dbOutput = 44;
				//
				ClientData.Plc[i].Prelievo[j][p].pos = PosPrel[j - 1];
				ClientData.Plc[i].Prelievo[j][p].pos_fuori_ingombro = PosPrel_FuoriIngombro[j - 1];
				ClientData.Plc[i].Prelievo[j][p].numpiani = 1;
				// in
				// ClientData.Plc[i].Prelievo[1].byteInput = 10;
				// ClientData.Plc[i].Prelievo[2].byteInput = 50;
				//
				ClientData.Plc[i].Prelievo[j][p].bit_Ready = 0;
				ClientData.Plc[i].Prelievo[j][p].bit_InAllarme = 1;
				ClientData.Plc[i].Prelievo[j][p].bit_ProntaAlPrelievo = 4;
				ClientData.Plc[i].Prelievo[j][p].bit_OkPrelievo = 2;
				// out
				// ClientData.Plc[i].Prelievo[1].byteOutput = 10;
				// ClientData.Plc[i].Prelievo[2].byteOutput = 50;
				ClientData.Plc[i].Prelievo[j][p].bit_RichiestaAbilitaPrel = 0;
				ClientData.Plc[i].Prelievo[j][p].RichiestaAbilitaPrel = 0;
				ClientData.Plc[i].Prelievo[j][p].bit_AGVInIngombro = 1;
				ClientData.Plc[i].Prelievo[j][p].AGVInIngombro = 0;
			}
		}

		for (int j = 1; j <= NUM_DEPOSITOPLC; j++) {
			for (int p = 1; p <= NUMPIANI_DEP; p++) {
				// DEPOSITO
				ClientData.Plc[i].Deposito[j][p].idplc = IDX_PLCDEPOSITO;
				ClientData.Plc[i].Deposito[j][p].dbInput = 41 + (2 - ((j - 1) * 2));
				ClientData.Plc[i].Deposito[j][p].dbOutput = 40 + (2 - ((j - 1) * 2));
				//
				ClientData.Plc[i].Deposito[j][p].pos = PosDep[j - 1];
				ClientData.Plc[i].Deposito[j][p].pos_fuori_ingombro = PosDep_FuoriIngombro[j - 1];
				// in
				ClientData.Plc[i].Deposito[j][p].byteInput = 0;
				ClientData.Plc[i].Deposito[j][p].bit_Ready = 0;
				ClientData.Plc[i].Deposito[j][p].bit_InAllarme = 1;
				ClientData.Plc[i].Deposito[j][p].bit_ProntaAlDeposito = 4;
				ClientData.Plc[i].Deposito[j][p].bit_OkDeposito = 2;
				// out
				// ClientData.Plc[i].Deposito[j][p].offsetOUT =0;
				// ClientData.Plc[i].Deposito[j][p].byteOutput = 0;
				ClientData.Plc[i].Deposito[j][p].bit_RichiestaAbilitaDep = 0;
				ClientData.Plc[i].Deposito[j][p].RichiestaAbilitaDep = 0;
				ClientData.Plc[i].Deposito[j][p].bit_AGVInIngombro = 1;
				ClientData.Plc[i].Deposito[j][p].AGVInIngombro = 0;
			}
		}
	}

	LeggiStrutturaDatiSegnali();
}

// ---------------------------------------------------------------------------

void TDMPlc::LeggiStrutturaDatiSegnali() {
	// sarebbe da mettere in plc porte ma facciamo un'eccezione
	/*
	 AnsiString strsql, strlog = "";
	 TADOQuery *ADOQuery;
	 int res = 0, id = 1, id_plc, zonaidx = 1;
	 try {
	 if (!dmDB->ADOConnection1->Connected)
	 return;
	 ADOQuery = new TADOQuery(NULL);
	 ADOQuery->Connection = dmDB->ADOConnection1;
	 strsql = "Select * from pulsanti order by id";
	 ADOQuery->SQL->Clear();
	 ADOQuery->SQL->Text = strsql;
	 ADOQuery->Open();
	 ADOQuery->First();
	 while ((!ADOQuery->Eof) && (id <= NUM_POSTAZIONI)) {
	 ClientData.Zona[zonaidx].id_thread_plc =ADOQuery->FieldByName("id_plc")->AsInteger;
	 ClientData.Zona[zonaidx].Postazione[id].DatiPulsante.byteInput = ADOQuery->FieldByName("DW_Addr_In")->AsInteger;
	 ClientData.Zona[zonaidx].Postazione[id].DatiPulsante.byteOutput = ADOQuery->FieldByName("DW_Addr_Out")->AsInteger;
	 ClientData.Zona[zonaidx].Postazione[id].DatiPulsante.bitInput = ADOQuery->FieldByName("Bit_In")->AsInteger;
	 ClientData.Zona[zonaidx].Postazione[id].DatiPulsante.bitOutput = ADOQuery->FieldByName("Bit_Out")->AsInteger;
	 ClientData.Zona[zonaidx].Postazione[id].pos  =ADOQuery->FieldByName("pos")->AsInteger
	 ClientData.Zona[zonaidx].Postazione[id_plc].StatoPulsante= ADOQuery->FieldByName("stato")->AsInteger;
	 id++;
	 ADOQuery->Next();
	 }
	 ADOQuery->Close();
	 lettodatiporte = true;
	 }
	 catch (...) {
	 }
	 delete ADOQuery;
	 */
}

void __fastcall TDMPlc::DataModuleCreate(TObject * Sender) {

	TimerEventi->Enabled = true;
	TimerPlc->Enabled = true;
	TimerWatchDogPlc->Enabled = true;
	TimerCarScar->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TDMPlc::TimerEventiTimer(TObject *Sender) {
	int i, j, agv, k, anomalia_generatori;

	if ((MainForm->count_inizio < 19) || (MainForm->close_program))
		return;

	TimerEventi->Enabled = false;
	if ((PLCThread[0]->connected) || (ClientData.ParametriFunzionali.Simula)) {
		if (dmDB->lettostatoagv) {
			for (i = 1; i <= N_PLC; i++) {
				for (j = 1; j <= NUM_PRELIEVOPLC; j++) {
					for (int p = 1; p <= NUMPIANI_PREL; p++) {
						if (!ClientData.Plc[i].forzature_attive) {
							ControlloIngressoPrelievi(ClientData.Plc[i], ClientData.Plc[i].Prelievo[j][p]);

						}
					}
				}
				for (j = 1; j <= NUM_DEPOSITOPLC; j++) {
					for (int p = 1; p <= NUMPIANI_DEP; p++) {
						if (!ClientData.Plc[i].forzature_attive) {
							ControlloIngressoDepositi(ClientData.Plc[i], ClientData.Plc[i].Deposito[j][p]);

						}
					}
				}
			}
		}
	}
	TimerEventi->Enabled = true;
}

// ---------------------------------------------------------------------------

void __fastcall TDMPlc::TimerCarScarTimer(TObject *Sender) {
	int i, j;

	if ((MainForm->count_inizio < 19) || (MainForm->close_program))
		return;

	TimerCarScar->Enabled = false;
	if ((PLCThread[0]->connected) || (ClientData.ParametriFunzionali.Simula)) {
		if (dmDB->lettostatoagv) {
			for (i = 1; i <= N_PLC; i++) {
				if (ClientData.Plc[i].PlcConnesso) {
					for (j = 1; j <= NUM_PRELIEVOPLC; j++) {
						for (int p = 1; p <= NUMPIANI_PREL; p++) {
							if (ClientData.Plc[i].Prelievo[j][p].idplc == i) {
								if (!ClientData.Plc[i].forzature_attive) {
									ControlloStatoPrelievi(ClientData.Plc[i], ClientData.Plc[i].Prelievo[j][p]);

								}
							}
						}
					}
					for (j = 1; j <= NUM_DEPOSITOPLC; j++) {
						for (int p = 1; p <= NUMPIANI_DEP; p++) {
							if (ClientData.Plc[i].Deposito[j][p].idplc == i) {
								if (!ClientData.Plc[i].forzature_attive) {
									ControlloStatoDepositi(ClientData.Plc[i], ClientData.Plc[i].Deposito[j][p]);
								}
							}
						}
					}
				}
			}
		}
	}
	TimerCarScar->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TDMPlc::TimerPlcTimer(TObject * Sender) {
	// imposto datyi in memoria da Plc
	int i, j, indice, valdascrivere, bitout;
	static int lampeggio = 0;

	if ((MainForm->count_inizio < 10) || (!dmDB->lettostatoagv) || (MainForm->close_program))
		return;
	TimerPlc->Enabled = false;

	for (i = 1; i <= N_PLC; i++) {
		for (j = 1; j <= NUM_PRELIEVOPLC; j++) {
			for (int p = 1; p <= NUMPIANI_PREL; p++) {
				CompilaStrutturaPrelievo(ClientData.Plc[i], ClientData.Plc[i].Prelievo[j][p]);
			}
		}
		for (j = 1; j <= NUM_DEPOSITOPLC; j++) {
			for (int p = 1; p <= NUMPIANI_DEP; p++) {
				CompilaStrutturaDeposito(ClientData.Plc[i], ClientData.Plc[i].Deposito[j][p]);
			}
		}

	}

	// in
	lampeggio = ++lampeggio % 2;
	TimerPlc->Enabled = true;
}

// ---------------------------------------------------------------------------

void TDMPlc::CompilaStrutturaPrelievo(DatiPlc & Plc, DatiPrelievo & Prelievo) {
	AnsiString stringa = "";
	int i, j, indice_byte;
	int bytedipartenza = 0;
	// compilo i dati della struttura plc passata
	Plc.PlcConnesso = PLCThread[Plc.id_thread_plc]->connected;
	Plc.Error = PLCThread[Plc.id_thread_plc]->PLCError;
	Plc.CycleTime = PLCThread[Plc.id_thread_plc]->tempo_ciclo_int;
	Plc.watchdog_plc = PLCThread[Plc.id_thread_plc]->watchdog_plc;
	Plc.watchdog_pc = PLCThread[Plc.id_thread_plc]->watchdog_pc;

	if ((PLCThread[Plc.id_thread_plc]->connected) || (ClientData.ParametriFunzionali.Simula)) {
		if ((PLCThread[Plc.id_thread_plc]->connected) && (!ClientData.ParametriFunzionali.Simula)) {
			// raw data
			Prelievo.StatoInput[bytedipartenza] = PLCThread[Plc.id_thread_plc]->DB[Prelievo.dbInput][Prelievo.byteInput];
			// Prelievo.StatoInput[1] = PLCThread[Plc.id_thread_plc]->DB[Prelievo.dbInput][Prelievo.byteInput + 1];
			// Prelievo.StatoInput[2] = PLCThread[Plc.id_thread_plc]->DB[Prelievo.dbInput][Prelievo.byteInput + 2];
			// Prelievo.StatoInput[3] = PLCThread[Plc.id_thread_plc]->DB[Prelievo.dbInput][Prelievo.byteInput + 3];
			// for (i = 4; i < 6; i++) {
			// Prelievo.Dati[i] = PLCThread[Plc.id_thread_plc]->DB[Prelievo.dbOutput][Prelievo.byteOutput ];
			// }
		}
		// in
		// per visualizzazione a video DA SOSTITUIRE con Prelievo.byteInput[0]
		for (i = 0; i < 4; i++) {
			Prelievo.Dati[i] = Prelievo.StatoInput[i];
		}
		Prelievo.Ready = Prelievo.StatoInput[bytedipartenza] & dmDB->bit[Prelievo.bit_Ready];
		Prelievo.InAllarme = Prelievo.StatoInput[bytedipartenza] & dmDB->bit[Prelievo.bit_InAllarme];
		Prelievo.ProntaAlPrelievo = Prelievo.StatoInput[bytedipartenza] & dmDB->bit[Prelievo.bit_ProntaAlPrelievo];
		Prelievo.OkPrelievo = Prelievo.StatoInput[bytedipartenza] & dmDB->bit[Prelievo.bit_OkPrelievo];
		// Prelievo.PalletID = Prelievo.StatoInput[1];

		// out
		if ((PLCThread[Plc.id_thread_plc]->connected) && (!ClientData.ParametriFunzionali.Simula)) {
			// lettura missione da plc
			Prelievo.uscite[0] = PLCThread[Plc.id_thread_plc]->DB[Prelievo.dbOutput][Prelievo.byteOutput];
		}

		if (MainForm->count_inizio < 20) {
			Prelievo.uscitedascrivere[0] = Prelievo.uscite[0];
		}
		// dmExtraFunction->bitWrite(Prelievo.uscitedascrivere, Prelievo.bit_AGVRulliInMoto, ClientData.DatiAgv[1].RulliInMoto);
		Prelievo.RichiestaAbilitaPrel = Prelievo.uscitedascrivere[0] & dmDB->bit[Prelievo.bit_RichiestaAbilitaPrel];
		Prelievo.AGVInIngombro = Prelievo.uscitedascrivere[0] & dmDB->bit[Prelievo.bit_AGVInIngombro];

		if (MainForm->count_inizio > 20) {
			if (Prelievo.uscitedascrivere[0] != Prelievo.uscite[0]) {
				if (PLCThread[Plc.id_thread_plc]->connected) {
					PLCThread[Plc.id_thread_plc]->WriteByte(Prelievo.dbOutput, Prelievo.byteOutput, Prelievo.uscitedascrivere[0], ClientData.ParametriFunzionali.Simula);
				}
				else if (ClientData.ParametriFunzionali.Simula)
					Prelievo.uscite[0] = Prelievo.uscitedascrivere[0];
				dmDB->LogMsg("Modificato statouscite prelievo su Plc pos " + IntToStr(Prelievo.pos) + " , valore prec : " + IntToStr(Prelievo.uscite[0]) + " , nuovo : " + IntToStr(Prelievo.uscitedascrivere[0]));
			}

			if (Plc.forzature_attive) {
				if (Plc.count_forzature > 30) {
					Plc.count_forzature = 0;
					Plc.forzature_attive = 0;
				}
				else
					Plc.count_forzature++;
			}
			else
				Plc.count_forzature = 0;
		}
	}
}

void TDMPlc::CompilaStrutturaDeposito(DatiPlc & Plc, DatiDeposito & Deposito) {
	AnsiString stringa = "";
	int i, j, indice_byte;
	static int count = 0;
	int bytedipartenza = 0;

	Plc.PlcConnesso = PLCThread[Plc.id_thread_plc]->connected;
	Plc.Error = PLCThread[Plc.id_thread_plc]->PLCError;
	Plc.CycleTime = PLCThread[Plc.id_thread_plc]->tempo_ciclo_int;
	Plc.watchdog_plc = PLCThread[Plc.id_thread_plc]->watchdog_plc;
	Plc.watchdog_pc = PLCThread[Plc.id_thread_plc]->watchdog_pc;

	if ((PLCThread[Plc.id_thread_plc]->connected) || (ClientData.ParametriFunzionali.Simula)) {
		if ((PLCThread[Plc.id_thread_plc]->connected) && (!ClientData.ParametriFunzionali.Simula)) {
			// raw data
			Deposito.StatoInput[bytedipartenza] = PLCThread[Plc.id_thread_plc]->DB[Deposito.dbInput][Deposito.byteInput];
			// Deposito.StatoInput[bytedipartenza + 1] = PLCThread[Plc.id_thread_plc]->DB[Deposito.dbInput][Deposito.byteInput + 1];
			// Deposito.StatoInput[bytedipartenza + 2] = PLCThread[Plc.id_thread_plc]->DB[Deposito.dbInput][Deposito.byteInput + 2];
			// Deposito.StatoInput[bytedipartenza + 3] = PLCThread[Plc.id_thread_plc]->DB[Deposito.dbInput][Deposito.byteInput + 3];
			// for (i = 4; i < 6; i++) {
			// Deposito.Dati[i] = PLCThread[Plc.id_thread_plc]->DB[Deposito.dbInput][Deposito.byteOutput + (i - 4)];
			// }
		}

		// in
		// per visualizzazione a video DA SOSTITUIRE con Deposito.byteInput[0]
		for (i = 0; i < 4; i++) {
			Deposito.Dati[i] = Deposito.StatoInput[i];
		}

		Deposito.Ready = ((Deposito.StatoInput[bytedipartenza] & dmDB->bit[Deposito.bit_Ready]) == 0 ? 0 : 1);
		Deposito.InAllarme = ((Deposito.StatoInput[bytedipartenza] & dmDB->bit[Deposito.bit_InAllarme]) == 0 ? 0 : 1);
		Deposito.ProntaAlDeposito = ((Deposito.StatoInput[bytedipartenza] & dmDB->bit[Deposito.bit_ProntaAlDeposito]) == 0 ? 0 : 1);
		Deposito.OkDeposito = ((Deposito.StatoInput[bytedipartenza] & dmDB->bit[Deposito.bit_OkDeposito]) == 0 ? 0 : 1);

		// out
		if ((PLCThread[Plc.id_thread_plc]->connected) && (!ClientData.ParametriFunzionali.Simula)) {
			// lettura missione da plc
			Deposito.uscite[0] = PLCThread[Plc.id_thread_plc]->DB[Deposito.dbOutput][Deposito.byteOutput];
			// Deposito.uscite[0] = 0;
			// Deposito.uscite[1] = PLCThread[Plc.id_thread_plc]->DB[Deposito.dbOutput][Deposito.byteOutput + 4] * 256 + PLCThread[Plc.id_thread_plc]->DB[Deposito.dbOutput][Deposito.byteOutput + 4 + 1];
		}

		if (MainForm->count_inizio < 20) {
			Deposito.uscitedascrivere[0] = Deposito.uscite[0];
			Deposito.uscitedascrivere[1] = Deposito.uscite[1];
		}
		// dmExtraFunction->bitWrite(Deposito.uscitedascrivere, Deposito.bit_AGVRulliInMoto, ClientData.DatiAgv[1].RulliInMoto);
		Deposito.RichiestaAbilitaDep = Deposito.uscitedascrivere[0] & dmDB->bit[Deposito.bit_RichiestaAbilitaDep];
		Deposito.AGVInIngombro = Deposito.uscitedascrivere[0] & dmDB->bit[Deposito.bit_AGVInIngombro];
		// Deposito.PalletID = Deposito.uscitedascrivere[1];

		// gestione watchdog
		// gestisco uscite :
		if (MainForm->count_inizio > 20) {
			if (Deposito.uscitedascrivere[0] != Deposito.uscite[0]) {
				if (PLCThread[Plc.id_thread_plc]->connected) {
					PLCThread[Plc.id_thread_plc]->WriteByte(Deposito.dbOutput, Deposito.byteOutput, Deposito.uscitedascrivere[0], ClientData.ParametriFunzionali.Simula);
				}
				else if (ClientData.ParametriFunzionali.Simula)
					Deposito.uscite[0] = Deposito.uscitedascrivere[0];
				dmDB->LogMsg("Modificato statouscite deposito" + IntToStr(Deposito.pos) + " su Plc pos " + IntToStr(Deposito.pos) + " , valore prec : " + IntToStr(Deposito.uscite[0]) + " , nuovo : " +
					IntToStr(Deposito.uscitedascrivere[0]));
			}
			if (Plc.forzature_attive) {
				if (Plc.count_forzature > 30) {
					Plc.count_forzature = 0;
					Plc.forzature_attive = 0;
				}
				else
					Plc.count_forzature++;
			}
			else
				Plc.count_forzature = 0;

		}
	}
}

void TDMPlc::ControlloIngressoPrelievi(DatiPlc &Plc, DatiPrelievo &Prelievo) {
	int tempodaultimamissione = 10;
	Evento ev;
	int prelievirimasti, id;
	if (dmDB->lettostatoagv) {
		for (int agv = 1; agv <= NAGV; agv++) {
			// if ((ClientData.DatiAgv[agv].richiestaconsenso) && (!ClientData.DatiAgv[agv].consensodato) && (ClientData.DatiAgv[agv].pos == Prelievo.pos_fuori_ingombro) && (MainForm->strcons[agv] == "rulierac")) {
			// NB: si mette il controllo sul nodo solo se non controllo la stringa ma in questo caso
			if ((ClientData.DatiAgv[agv].richiestaconsenso) && (!ClientData.DatiAgv[agv].consensodato) && (MainForm->strcons[agv] == "rulierac")) {
				// if ((Prelievo.BaiaCaricoInAutomatico) && (!Prelievo.BaiaCaricoInEmergenza) && (Prelievo.OkIngresso)) {
				if ((!Prelievo.InAllarme) && (Prelievo.Ready)) { // && (Prelievo.OkPrelievo)) {
					ClientData.DatiAgv[agv].consensodato = ClientData.DatiAgv[agv].nodob;
					ClientData.DatiAgv[agv].richiestaconsenso = 0;
					dmDB->LogMsg("Consenso ingresso inviato ad Agv " + IntToStr(agv) + " in pos " + IntToStr(ClientData.DatiAgv[agv].pos) + " ,dest " + IntToStr(ClientData.DatiAgv[agv].dest));
					sprintf(ev, "O,R,%d,%d,%d,%d", agv, ClientData.DatiAgv[agv].nodoa, ClientData.DatiAgv[agv].nodob, 1);
					cs->SendEv(ev);
					dmDB->AggiornaStatoAgv(agv);
					if (ClientData.DatiAgv[agv].allarme_consenso) {
						MainForm->MessageBar1->DelMsgNo(ClientData.DatiAgv[agv].num_allarme_consenso);
						ClientData.DatiAgv[agv].allarme_consenso = 0;
						// dmDB->AggiornaAllarmiAttivi();
					}
				}
				else if ((!ClientData.DatiAgv[agv].allarme_consenso) && (ClientData.DatiAgv[agv].pos == ClientData.DatiAgv[agv].nodoa)) {
					MainForm->MessageBar1->Messages->Strings[ClientData.DatiAgv[agv].num_allarme_consenso] = "Agv in Attesa deposito";
					MainForm->MessageBar1->AddMsgNo(ClientData.DatiAgv[agv].num_allarme_consenso);
					ClientData.DatiAgv[agv].allarme_consenso = 1;
					// dmDB->AggiornaAllarmiAttivi();
				}

			}
		}
	}
}

void TDMPlc::ControlloIngressoDepositi(DatiPlc &Plc, DatiDeposito &Deposito) {
	int tempodaultimamissione = 10;
	AnsiString StringaConsenso;
	Evento ev;
	int prelievirimasti, id;
	if (dmDB->lettostatoagv) {
		for (int agv = 1; agv <= NAGV; agv++) {
			if (((ClientData.DatiAgv[agv].richiestaconsenso) && (!ClientData.DatiAgv[agv].consensodato)) && ((MainForm->strcons[agv] == "rulierab") || (MainForm->strcons[agv] == "rulierae"))) {
				// if ((Deposito.BaiaScaricoInAutomatico) && (!Deposito.BaiaScaricoInEmergenza) && (Deposito.OkIngresso)) {
				if ((!Deposito.InAllarme) && (Deposito.Ready)) { // && (Deposito.OkDeposito)) {
					ClientData.DatiAgv[agv].consensodato = ClientData.DatiAgv[agv].nodob;
					ClientData.DatiAgv[agv].richiestaconsenso = 0;
					dmDB->LogMsg("Consenso ingresso inviato ad Agv " + IntToStr(agv) + " in pos " + IntToStr(ClientData.DatiAgv[agv].pos) + " ,dest " + IntToStr(ClientData.DatiAgv[agv].dest));
					sprintf(ev, "O,R,%d,%d,%d,%d", agv, ClientData.DatiAgv[agv].nodoa, ClientData.DatiAgv[agv].nodob, 1);
					cs->SendEv(ev);
					dmDB->AggiornaStatoAgv(agv);
					if (ClientData.DatiAgv[agv].allarme_consenso) {
						MainForm->MessageBar1->DelMsgNo(ClientData.DatiAgv[agv].num_allarme_consenso);
						ClientData.DatiAgv[agv].allarme_consenso = 0;
						// dmDB->AggiornaAllarmiAttivi();
					}

				}
				else if ((!ClientData.DatiAgv[agv].allarme_consenso) && (ClientData.DatiAgv[agv].pos == ClientData.DatiAgv[agv].nodoa)) {
					MainForm->MessageBar1->Messages->Strings[ClientData.DatiAgv[agv].num_allarme_consenso] = "Agv in Attesa deposito";
					MainForm->MessageBar1->AddMsgNo(ClientData.DatiAgv[agv].num_allarme_consenso);
					ClientData.DatiAgv[agv].allarme_consenso = 1;
					// dmDB->AggiornaAllarmiAttivi();
				}

			}
		}
	}
}

void TDMPlc::ControlloStatoPrelievi(DatiPlc & Plc, DatiPrelievo & Prelievo) {
	for (int a = 1; a <= NAGV; a++) {
		if ((!ClientData.ParametriFunzionali.Simula) && (Plc.PlcConnesso)) {
			if ((ClientData.DatiAgv[a].pos == Prelievo.pos_fuori_ingombro) || (ClientData.DatiAgv[a].pos == Prelievo.pos)) {
				if ((!Prelievo.InAllarme) && (Prelievo.Ready)) {
					if (((ClientData.DatiAgv[a].pos == Prelievo.pos) || (ClientData.DatiAgv[a].pos == Prelievo.pos_fuori_ingombro)) && (ClientData.DatiAgv[a].dest == Prelievo.pos) && (!Prelievo.RichiestaAbilitaPrel)) {
						Prelievo.uscitedascrivere[0] = Prelievo.uscitedascrivere[0] | dmDB->bit[Prelievo.bit_RichiestaAbilitaPrel];
						dmDB->LogMsg("Set Richiesta Ingombro Prelievo su posizione " + IntToStr(Prelievo.pos));
					}
					else if (((ClientData.DatiAgv[a].pos == Prelievo.pos) || (ClientData.DatiAgv[a].pos == Prelievo.pos_fuori_ingombro)) && (!Prelievo.AGVInIngombro) && (ClientData.DatiAgv[a].dest == Prelievo.pos)) {
						// log reset rulli in moto
						Prelievo.uscitedascrivere[0] = Prelievo.uscitedascrivere[0] | dmDB->bit[Prelievo.bit_AGVInIngombro];
						dmDB->LogMsg("Set AGV In Ingombro Prelievo su posizione " + IntToStr(Prelievo.pos));
					}

				}

				if (((Prelievo.InAllarme) || (!Prelievo.Ready)) && (ClientData.DatiAgv[a].dest == Prelievo.pos) && ((ClientData.DatiAgv[a].dest == Prelievo.pos) || (ClientData.DatiAgv[a].pos == Prelievo.pos_fuori_ingombro))
					&& (!Prelievo.AGVInIngombro)) {
					if (!ClientData.DatiAgv[a].stand_by) {
						// fermo agv durante carico per allarme plc a terra
						dmComandiAgv->StandByAgv(a, 1);
						dmDB->LogMsg("Set AGV in standby su posizione " + IntToStr(Prelievo.pos));
					}
				}
				else if (((!Prelievo.InAllarme) && (Prelievo.Ready)) || ((ClientData.DatiAgv[a].dest != Prelievo.pos) && (ClientData.DatiAgv[a].pos != Prelievo.pos_fuori_ingombro))) {
					if (ClientData.DatiAgv[a].stand_by) {
						// fermo agv durante carico per allarme plc a terra
						dmComandiAgv->StandByAgv(a, 0);
						dmDB->LogMsg("Ripristino AGV da standby " + IntToStr(Prelievo.pos));
					}
				}

				if ((Prelievo.AGVInIngombro) && (ClientData.DatiAgv[a].dest != Prelievo.pos)) {
					// reset eventuali uscite rimaste alte con agv in altra posizione
					Prelievo.uscitedascrivere[0] = Prelievo.uscitedascrivere[0] & dmDB->bitAnd[Prelievo.bit_AGVInIngombro];
					dmDB->LogMsg("Reset InIngombroZonaCarico su posizione " + IntToStr(Prelievo.pos) + " in posizione " + IntToStr(ClientData.DatiAgv[a].pos));
				}
				if ((Prelievo.RichiestaAbilitaPrel) && (ClientData.DatiAgv[a].dest != Prelievo.pos) && (ClientData.DatiAgv[a].pos != Prelievo.pos_fuori_ingombro)) {
					// reset eventuali uscite rimaste alte con agv in altra posizione
					Prelievo.uscitedascrivere[0] = Prelievo.uscitedascrivere[0] & dmDB->bitAnd[Prelievo.bit_RichiestaAbilitaPrel];
					dmDB->LogMsg("Reset RichiestaAbilitaPrel su posizione " + IntToStr(Prelievo.pos) + " in posizione " + IntToStr(ClientData.DatiAgv[a].pos));
				}
			}
			// se sono fuori da tutto e ho ancora qualche bit alto
			else if ((ClientData.DatiAgv[a].pos != Prelievo.pos_fuori_ingombro) && (ClientData.DatiAgv[a].pos != Prelievo.pos)) {
				if (Prelievo.AGVInIngombro) {
					// reset eventuali uscite rimaste alte con agv in altra posizione
					Prelievo.uscitedascrivere[0] = Prelievo.uscitedascrivere[0] & dmDB->bitAnd[Prelievo.bit_AGVInIngombro];
					dmDB->LogMsg("Reset InIngombroZonaCarico su posizione " + IntToStr(Prelievo.pos) + " in posizione " + IntToStr(ClientData.DatiAgv[a].pos));
				}
				if (Prelievo.RichiestaAbilitaPrel) {
					// reset eventuali uscite rimaste alte con agv in altra posizione
					Prelievo.uscitedascrivere[0] = Prelievo.uscitedascrivere[0] & dmDB->bitAnd[Prelievo.bit_RichiestaAbilitaPrel];
					dmDB->LogMsg("Reset RichiestaIngombroZonaCarico su posizione " + IntToStr(Prelievo.pos) + " in posizione " + IntToStr(ClientData.DatiAgv[a].pos));
				}
				/*
				if (ClientData.DatiAgv[a].stand_by) {
					// fermo agv durante carico per allarme plc a terra
					dmComandiAgv->StandByAgv(a, 0);
					dmDB->LogMsg("Ripristino AGV da standby " + IntToStr(Prelievo.pos));
				}
				*/

			}
		}
	}

}

void TDMPlc::ControlloStatoDepositi(DatiPlc & Plc, DatiDeposito & Deposito) {
	for (int a = 1; a <= NAGV; a++) {
		if ((!ClientData.ParametriFunzionali.Simula) && (Plc.PlcConnesso)) {
			if ((ClientData.DatiAgv[a].pos == Deposito.pos_fuori_ingombro) || (ClientData.DatiAgv[a].pos == Deposito.pos)) {
				if ((!Deposito.InAllarme) && (Deposito.Ready)) {
					if (((ClientData.DatiAgv[a].pos == Deposito.pos) || (ClientData.DatiAgv[a].pos == Deposito.pos_fuori_ingombro)) && (ClientData.DatiAgv[a].dest == Deposito.pos) && (!Deposito.RichiestaAbilitaDep)) {
						Deposito.uscitedascrivere[0] = Deposito.uscitedascrivere[0] | dmDB->bit[Deposito.bit_RichiestaAbilitaDep];
						dmDB->LogMsg("Set RichiestaIngombroZonaScarico su posizione " + IntToStr(Deposito.pos));
					}
					else if ((ClientData.DatiAgv[a].pos == Deposito.pos) && (Deposito.OkDeposito) && (ClientData.DatiAgv[a].dest == Deposito.pos) && (!Deposito.AGVInIngombro)) {
						// rulli in moto
						Deposito.uscitedascrivere[0] = Deposito.uscitedascrivere[0] | dmDB->bit[Deposito.bit_AGVInIngombro];
						dmDB->LogMsg("Set IngombroZonaScarico su posizione " + IntToStr(Deposito.pos));
					}
				}
				if (((ClientData.DatiAgv[a].pos == Deposito.pos) || (ClientData.DatiAgv[a].pos == Deposito.pos_fuori_ingombro)) && (ClientData.DatiAgv[a].dest == Deposito.pos)
					&& ((Deposito.InAllarme) || (!Deposito.Ready))) {
					if (!ClientData.DatiAgv[a].stand_by) {
						// fermo agv durante carico per allarme plc a terra
						dmComandiAgv->StandByAgv(a, 1);
						dmDB->LogMsg("Set Standby agv su posizione " + IntToStr(Deposito.pos));
					}
				}
				else if ((!Deposito.InAllarme) || (ClientData.DatiAgv[a].dest != Deposito.pos)) {
					if (ClientData.DatiAgv[a].stand_by) {
						// riavvio agv durante carico per allarme plc a terra
						dmComandiAgv->StandByAgv(a, 0);
						dmDB->LogMsg("Reset Standby agv su posizione " + IntToStr(Deposito.pos));
					}
				}

				if ((ClientData.DatiAgv[a].dest != Deposito.pos) && (Deposito.AGVInIngombro)) {
					Deposito.uscitedascrivere[0] = Deposito.uscitedascrivere[0] & dmDB->bitAnd[Deposito.bit_AGVInIngombro];
					dmDB->LogMsg("Reset IngombroZonaScarico su posizione " + IntToStr(ClientData.DatiAgv[a].pos));
				}
				if ((ClientData.DatiAgv[a].dest != Deposito.pos) && (Deposito.RichiestaAbilitaDep)) {
					// reset eventuali uscite rimaste alte con agv in altra posizione
					Deposito.uscitedascrivere[0] = Deposito.uscitedascrivere[0] & dmDB->bitAnd[Deposito.bit_RichiestaAbilitaDep];
					dmDB->LogMsg("Reset RichiestaIngombroZonaScarico su posizione " + IntToStr(ClientData.DatiAgv[a].pos));
				}

			}
			else if ((ClientData.DatiAgv[a].pos != Deposito.pos_fuori_ingombro) && (ClientData.DatiAgv[a].pos != Deposito.pos)) {
				// non sono in zona se ho delle uscite attive
				if (Deposito.RichiestaAbilitaDep) {
					Deposito.uscitedascrivere[0] = Deposito.uscitedascrivere[0] & dmDB->bitAnd[Deposito.bit_RichiestaAbilitaDep];
					dmDB->LogMsg("Reset RichiestaIngombroZonaScarico su posizione " + IntToStr(ClientData.DatiAgv[a].pos));
				}
				if (Deposito.AGVInIngombro) {
					// reset eventuali uscite rimaste alte con agv in altra posizione
					Deposito.uscitedascrivere[0] = Deposito.uscitedascrivere[0] & dmDB->bitAnd[Deposito.bit_AGVInIngombro];
					dmDB->LogMsg("Reset IngombroZonaScarico su posizione " + IntToStr(ClientData.DatiAgv[a].pos));
				}
				/*
				if (ClientData.DatiAgv[a].stand_by) {
					// riavvio agv durante carico per allarme plc a terra
					dmComandiAgv->StandByAgv(a, 0);
					dmDB->LogMsg("Reset Standby agv su posizione " + IntToStr(Deposito.pos));
				}
				*/

			}

		}
	}
}

// ---------------------------------------------------------------------------

void TDMPlc::ModificaByteStrutturaPlc(int input, int posplc, int byte, int value, int idx) {
	int i, j;

	for (i = 1; i <= N_PLC; i++) {
		for (j = 1; j <= NUM_PRELIEVOPLC; j++) {
			for (int p = 1; p <= NUMPIANI_PREL; p++) {
				if (ClientData.Plc[i].Prelievo[j][p].pos == posplc) {
					if (input) {
						ClientData.Plc[i].Prelievo[j][p].StatoInput[byte] = value;
					}
					else {
						ClientData.Plc[i].Prelievo[j][p].uscitedascrivere[idx] = value;
						dmDB->LogMsg("Ricevuto forzature uscite plc su pos " + IntToStr(posplc) + " , byte " + IntToStr(byte) + " valore : " + IntToStr(value));
					}
					ClientData.Plc[i].forzature_attive = true;
					break;
				}
			}
		}

		for (j = 1; j <= NUM_DEPOSITOPLC; j++) {
			for (int p = 1; p <= NUMPIANI_DEP; p++) {
				if ((ClientData.Plc[i].Deposito[j][p].pos == posplc) && (p == idx)) {
					if (input) {
						ClientData.Plc[i].Deposito[j][p].StatoInput[byte] = value;
					}
					else {
						ClientData.Plc[i].Deposito[j][p].uscitedascrivere[byte] = value;
						dmDB->LogMsg("Ricevuto forzature uscite plc su pos " + IntToStr(posplc) + " , byte " + IntToStr(byte) + " valore : " + IntToStr(value));
					}
					ClientData.Plc[i].forzature_attive = true;
					break;
				}
			}
		}

	}
}

int TDMPlc::InserisciIdPalletInAgv(int piano, int posizione, int agv) {
	int idPlc = 0;
	idPlc = dmDBImpianto->RitornaIdPlcDaPosizione(posizione, 1, 1);
	ClientData.DatiAgv[agv].DatiUDC.IDUDC = ClientData.Plc[1].Prelievo[idPlc][piano].PalletID;
}

void __fastcall TDMPlc::TimerWatchDogPlcTimer(TObject * Sender) {
	// gestisco tutti i watchdog dei plc presenti, anche le porte sono qui
	int i;

	if ((MainForm->count_inizio < 15) || (MainForm->close_program)) {
		return;
	}

	TimerWatchDogPlc->Enabled = false;
	for (i = 0; i < N_PLC; i++) {
		if (PLCThread[i]->dbwatchdog_pc >= 0) {
			if ((PLCThread[i]->connected) && (!ClientData.ParametriFunzionali.Simula)) {
				PLCThread[i]->watchdog_plc = PLCThread[i]->DB[PLCThread[i]->dbwatchdog_plc][PLCThread[i]->wordwatchdog_plc] & dmDB->bit[PLCThread[0]->bit_watchdog_plc];
				PLCThread[i]->watchdog_pc = PLCThread[i]->DB[PLCThread[i]->dbwatchdog_pc][PLCThread[i]->wordwatchdog_pc] & dmDB->bit[PLCThread[0]->bit_watchdog_pc];

				if ((PLCThread[i]->watchdog_plc != PLCThread[i]->watchdog_pc) && (PLCThread[i]->connected)) {
					PLCThread[i]->WriteByte(PLCThread[i]->dbwatchdog_pc, PLCThread[i]->wordwatchdog_pc, PLCThread[i]->watchdog_plc, ClientData.ParametriFunzionali.Simula);
					if (PLCThread[i]->alarm_watchdog) {
						MainForm->MessageBar1->DelMsgNo(PLCThread[i]->num_alarm_watchdog);
						PLCThread[i]->alarm_watchdog = 0;
						dmDB->AggiornaAllarmiAttivi();
					}
					PLCThread[i]->count_alarm_watchdog = 0;
				}
				else {
					PLCThread[i]->count_alarm_watchdog++;
					if (PLCThread[i]->count_alarm_watchdog >= 20) {
						if (!PLCThread[i]->alarm_watchdog) {
							MainForm->MessageBar1->AddMsgNo(PLCThread[i]->num_alarm_watchdog);
							PLCThread[i]->alarm_watchdog = 1;
							dmDB->AggiornaAllarmiAttivi();
						}
						PLCThread[i]->count_alarm_watchdog = 0;
					}
				}
			}
		}
	}
	TimerWatchDogPlc->Enabled = true;
}
// ---------------------------------------------------------------------------
