// ---------------------------------------------------------------------------

#pragma hdrstop

#include "dmgestioneporte.h"
#include "main.h"
// #include "dmphoenix.h"
#include "percorsiprototipo.h"
#include "socket.h"
#include "db.h"
#include "datamodulecomandiagv.h"
#include "PLCphoenixThread.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TDMGestione_Porte *DMGestione_Porte;
extern CLIENTSOCKET *cs;
extern DATILGV tab;

// ---------------------------------------------------------------------------
__fastcall TDMGestione_Porte::TDMGestione_Porte(TComponent* Owner)
	: TDataModule(Owner)
{
	sync_lamp = false;
	init = true;
	for (int i = 0; i < 10; i++)
		Postazioni_Libere_Antincendio[i] = 0;
	Postazioni_Libere_Antincendio[0] = 0;
	Postazioni_Libere_Antincendio[1] = 0;
	Postazioni_Libere_Antincendio[2] = 0;

}

// ---------------------------------------------------------------------------
void TDMGestione_Porte::LeggiStrutturaDatiPorte() {
	// aggiorno struttura porte
	AnsiString strsql, strlog = "";
	TADOQuery *ADOQuery;
	int res = 0, id = 0;
	try {

		if (!dmDB->ADOConnection1->Connected)
			return;
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = dmDB->ADOConnection1;
		strsql = "Select * from Porte ";
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Text = strsql;
		ADOQuery->Open();
		ADOQuery->First();
		while (!ADOQuery->Eof) {
			id = ADOQuery->FieldByName("IdPorta")->AsInteger;
			ClientData.DatiPorte[id].id = id;
			ClientData.DatiPorte[id].tipomodulo = ADOQuery->FieldByName("TipoModulo")->AsInteger;
			ClientData.DatiPorte[id].id_thread_plc = ADOQuery->FieldByName("ID_PLC")->AsInteger;
			ClientData.DatiPorte[id].Stato = ADOQuery->FieldByName("Stato")->AsInteger;
			ClientData.DatiPorte[id].DBInput = ADOQuery->FieldByName("DBInput")->AsInteger;
			ClientData.DatiPorte[id].DBOutput = ADOQuery->FieldByName("DBOutput")->AsInteger;
			ClientData.DatiPorte[id].DW_Addr_PortaAperta = ADOQuery->FieldByName("DW_Addr_PortaAperta")->AsInteger;
			ClientData.DatiPorte[id].DW_Addr_PortaChiusa = ADOQuery->FieldByName("DW_Addr_PortaChiusa")->AsInteger;
			ClientData.DatiPorte[id].DW_Addr_PortaTFuocoAperta = ADOQuery->FieldByName("DW_Addr_PortaTFuocoAperta")->AsInteger;
			ClientData.DatiPorte[id].Bit_PortaAperta = ADOQuery->FieldByName("Bit_PortaAperta")->AsInteger;
			ClientData.DatiPorte[id].Bit_PortaChiusa = ADOQuery->FieldByName("Bit_PortaChiusa")->AsInteger;
			ClientData.DatiPorte[id].Bit_PortaTFuocoAperta = ADOQuery->FieldByName("Bit_PortaTFuocoAperta")->AsInteger;
			ClientData.DatiPorte[id].DW_Addr_ApriPorta = ADOQuery->FieldByName("DW_Addr_ApriPorta")->AsInteger;
			ClientData.DatiPorte[id].DW_Addr_ChiudiPorta = ADOQuery->FieldByName("DW_Addr_ChiudiPorta")->AsInteger;
			ClientData.DatiPorte[id].Bit_ApriPorta = ADOQuery->FieldByName("Bit_ApriPorta")->AsInteger;
			ClientData.DatiPorte[id].Bit_ChiudiPorta = ADOQuery->FieldByName("Bit_ChiudiPorta")->AsInteger;
			ClientData.DatiPorte[id].DW_Addr_OkAccessoAgv = ADOQuery->FieldByName("DW_Addr_OkAccessoAgv")->AsInteger;
			ClientData.DatiPorte[id].Bit_OkAccessoAgv = ADOQuery->FieldByName("Bit_OkAccessoAgv")->AsInteger;
			ClientData.DatiPorte[id].Bit_Memoria_Apertura_Operatore = ADOQuery->FieldByName("Bit_Memoria_Apertura_Operatore")->AsInteger;
			ClientData.DatiPorte[id].tipoporta = ADOQuery->FieldByName("tipoporta")->AsInteger;

			ADOQuery->Next();
		}
		ADOQuery->Close();
		lettodatiporte = true;
	}
	catch (...) {
	}
	delete ADOQuery;
}

void TDMGestione_Porte::LeggiStrutturaDatiSemafori() {
	// aggiorno struttura porte
	AnsiString strsql, strlog = "";
	TADOQuery *ADOQuery;
	int res = 0, id = 0;
	try {

		if (!dmDB->ADOConnection1->Connected)
			return;
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = dmDB->ADOConnection1;
		strsql = "Select * from semafori ";
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Text = strsql;
		ADOQuery->Open();
		ADOQuery->First();
		while (!ADOQuery->Eof) {
			id = ADOQuery->FieldByName("Id")->AsInteger;
			ClientData.DatiSemafori[id].id = id;
			ClientData.DatiSemafori[id].tipomodulo = ADOQuery->FieldByName("TipoModulo")->AsInteger;
			ClientData.DatiSemafori[id].id_thread_plc = ADOQuery->FieldByName("ID_PLC")->AsInteger;
			ClientData.DatiSemafori[id].Stato = ADOQuery->FieldByName("Stato")->AsInteger;
			ClientData.DatiSemafori[id].DBOutput = ADOQuery->FieldByName("DBOutput")->AsInteger;
			ClientData.DatiSemafori[id].DW_Addr_LuceVerde = ADOQuery->FieldByName("DW_Addr_LuceVerde")->AsInteger;
			ClientData.DatiSemafori[id].DW_Addr_LuceRossa = ADOQuery->FieldByName("DW_Addr_LuceRossa")->AsInteger;
			ClientData.DatiSemafori[id].Bit_LuceVerde = ADOQuery->FieldByName("Bit_LuceVerde")->AsInteger;
			ClientData.DatiSemafori[id].Bit_LuceRossa = ADOQuery->FieldByName("Bit_LuceRossa")->AsInteger;
			ClientData.DatiSemafori[id].posx = ADOQuery->FieldByName("PosX")->AsInteger;
			ClientData.DatiSemafori[id].posy = ADOQuery->FieldByName("PosY")->AsInteger;
			ClientData.DatiSemafori[id].height = ADOQuery->FieldByName("height")->AsInteger;
			ClientData.DatiSemafori[id].width = ADOQuery->FieldByName("width")->AsInteger;
			// ClientData.DatiSemafori[id].DW_Addr_OkAccessoAgv = ADOQuery->FieldByName("DW_Addr_OkAccessoAgv")->AsInteger;
			// ClientData.DatiSemafori[id].Bit_OkAccessoAgv = ADOQuery->FieldByName("Bit_OkAccessoAgv")->AsInteger;
			ADOQuery->Next();
		}
		ADOQuery->Close();
	}
	catch (...) {
	}
	delete ADOQuery;
}

void TDMGestione_Porte::LeggiStrutturaDatiPulsanti() {
	// sarebbe da mettere in plc porte ma facciamo un'eccezione
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
			id_plc = ADOQuery->FieldByName("id_plc")->AsInteger;
			ClientData.Zona[zonaidx].Postazione[id_plc].DatiPulsante.byteInput = ADOQuery->FieldByName("DW_Addr_In")->AsInteger;
			ClientData.Zona[zonaidx].Postazione[id_plc].DatiPulsante.byteOutput = ADOQuery->FieldByName("DW_Addr_Out")->AsInteger;
			ClientData.Zona[zonaidx].Postazione[id_plc].DatiPulsante.bitInput = ADOQuery->FieldByName("Bit_In")->AsInteger;
			ClientData.Zona[zonaidx].Postazione[id_plc].DatiPulsante.bitOutput = ADOQuery->FieldByName("Bit_Out")->AsInteger;
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
}
 /*
void TDMGestione_Porte::LeggiStrutturaDatiSensori() {
	// aggiorno struttura porte
	AnsiString strsql, strlog = "";
	TADOQuery *ADOQuery;
	int res = 0, id = 0;
	try {
		if (!dmDB->ADOConnection1->Connected)
			return;
		ADOQuery = new TADOQuery(NULL);
		ADOQuery->Connection = dmDB->ADOConnection1;
		strsql = "Select * from Sensori order by IdSensore";
		ADOQuery->SQL->Clear();
		ADOQuery->SQL->Text = strsql;
		ADOQuery->Open();
		ADOQuery->First();
		while (!ADOQuery->Eof) {
			id = ADOQuery->FieldByName("IdSensore")->AsInteger;
			ClientData.Sensori[id].id = id;
			ClientData.Sensori[id].tipomodulo = ADOQuery->FieldByName("TipoModulo")->AsInteger;
			ClientData.Sensori[id].id_thread_plc = ADOQuery->FieldByName("ID_PLC")->AsInteger;
			ClientData.Sensori[id].Stato = ADOQuery->FieldByName("Stato")->AsInteger;
			ClientData.Sensori[id].DBInput = ADOQuery->FieldByName("DBInput")->AsInteger;
			ClientData.Sensori[id].DBOutput = ADOQuery->FieldByName("DBOutput")->AsInteger;
			ClientData.Sensori[id].DW_Addr_Lettura = ADOQuery->FieldByName("DW_Addr_Lettura")->AsInteger;
			ClientData.Sensori[id].Bit_Lettura = ADOQuery->FieldByName("Bit_Lettura")->AsInteger;
			ClientData.Sensori[id].DW_Addr_Scrittura = ADOQuery->FieldByName("DW_Addr_Scrittura")->AsInteger;
			ClientData.Sensori[id].Bit_Scrittura = ADOQuery->FieldByName("Bit_Scrittura")->AsInteger;
			ADOQuery->Next();
		}
		ADOQuery->Close();
		lettodatiporte = true;
	}
	catch (...) {
	}
	delete ADOQuery;
}
  */
void __fastcall TDMGestione_Porte::TimerEventiTimer(TObject *Sender)
{
	int i, j, agv, k, anomalia_generatori;
	Evento ev;

	// gestisco le missioni degli agv e tutti gli eventi

	if ((MainForm->count_inizio <= 15) || (MainForm->close_program))
		return;
	TimerEventi->Enabled = false;
	agv = 1;
	if (dmDB->lettostatoagv) {
		if (!lettodatiporte) {
			LeggiStrutturaDatiPorte();
			LeggiStrutturaDatiSemafori();
		 //	LeggiStrutturaDatiSensori();
			LeggiStrutturaDatiPulsanti();
			TimerPlcPorte->Enabled = true;
		}

		if (MainForm->count_inizio > 20) {
			ControllaPorteDaAprire();
			// GestioneRotAlarm() ;
			ControllaPorteAperte();
			ControllaResetSegnaleApriChiudiPorta(3); // vedere se serve ,tolgo solo il segnale chiusura
			// GestioneSemafori();           SOSTUTUITA COL MODBUS

		}
	}
	TimerEventi->Enabled = true;
}
// ---------------------------------------------------------------------------

void TDMGestione_Porte::LogStatoPorta(int porta) {
	AnsiString strlog;
	strlog = "Stato Porta " + IntToStr(porta)
		+ " , Porta Aperta : " + IntToStr(ClientData.DatiPorte[porta].PortaAperta)
		+ " , Porta Chiusa : " + IntToStr(ClientData.DatiPorte[porta].PortaChiusa)
		+ " , Apri Porta : " + IntToStr(ClientData.DatiPorte[porta].ApriPorta)
		+ " , Chiudi Porta : " + IntToStr(ClientData.DatiPorte[porta].ChiudiPorta);
	dmDB->LogMsg(strlog);
}

void TDMGestione_Porte::ControllaPorteDaAprire() {
	int i, res = 0, agv, baia, porta;
	Evento ev;
	for (agv = 1; agv <= NAGV; agv++) { // la porta tagliafuoco non la controllo
		if ((ClientData.DatiAgv[agv].richiestaconsenso) && (!ClientData.DatiAgv[agv].consensodato)) {
			try {
				porta = dmDB->ReturnIdPortaDaStringaConsensoPorta(MainForm->strcons[agv]);
				if (porta) {
					res = AgvInPosAperturaPorta(porta, agv);
					if (((!ClientData.DatiPorte[porta].ApriPorta) && (porta < 3) && (ClientData.DatiPorte[porta].Stato == 1) &&
						((!ClientData.DatiPorte[porta].memoria_apertura_operatore) || (ClientData.DatiPorte[porta].PortaAperta))) ||
						((ClientData.DatiPorte[porta].ApriPorta) && (porta > 4) && (ClientData.DatiPorte[porta].Stato == 1) &&
						((!ClientData.DatiPorte[porta].memoria_apertura_operatore) || (!ClientData.DatiPorte[porta].PortaAperta)))) {
						// porta in automatico
						// con la memoria di apertura non la richiudo ma faccio passare l'agv
						if (res) {
							if (porta < 3) {
								if (SbarreChiuse(3)) {
									if ((!ClientData.DatiPorte[porta].ApriPorta) && (ClientData.DatiPorte[porta].Stato == 1) &&
										((!ClientData.DatiPorte[porta].memoria_apertura_operatore) || (ClientData.DatiPorte[porta].PortaAperta))) {
										// porta in automatico
										// con la memoria di apertura non la richiudo ma faccio passare l'agv
										if (res) {
										// apro la porta
										ClientData.DatiPorte[porta].RichiestaAttiva++;
										ClientData.DatiAgv[agv].primo_ciclo_consenso = 0;
										dmDB->LogMsg("Apro Porta " + IntToStr(porta) + " per passaggio agv " + IntToStr(agv));
										ApriChiudiPorta(porta, APRIPORTA);
										}
									}
								}
							}
							if ((porta == 5) || (porta == 7)) {
								if ((ClientData.DatiPorte[porta].ApriPorta) && (ClientData.DatiPorte[porta].Stato == 1) &&
									(ClientData.DatiPorte[porta].PortaChiusa)) {
									// porta in automatico
									// con la memoria di apertura non la richiudo ma faccio passare l'agv
									if (res) {
										// apro la porta
										ClientData.DatiPorte[porta].RichiestaAttiva++;
										ClientData.DatiAgv[agv].primo_ciclo_consenso = 0;
										dmDB->LogMsg("Chiudo Porta " + IntToStr(porta) + " per passaggio agv " + IntToStr(agv));
										ApriChiudiPorta(porta, CHIUDIPORTA);
									}
								}

							}
						}
					}
					else if ((porta < 5) && (((res) || (ClientData.DatiPorte[porta].Stato >= 2)) && (!ClientData.DatiPorte[porta].ChiudiPorta) &&
						((ClientData.DatiPorte[porta].Stato == 3) || ((ClientData.DatiPorte[porta].PortaAperta) &&
						((ClientData.DatiPorte[porta].ApriPorta) || (ClientData.DatiPorte[porta].Stato == 2))) ||
						((ClientData.DatiPorte[porta].PortaTFuoco) && ((ClientData.DatiPorte[porta].Stato == 2)))))) {
						// porta esclusa oppure aperta in manuale o con richiesta
						// porta esclusa oppure aperta,invio consenso
						if (SbarreChiuse(3)) {
							// if (((porta >= 5) && (SbarreChiuse(5))) || (SbarreChiuse(3))) {
							if (ClientData.DatiAgv[agv].primo_ciclo_consenso) // gestione inversione agv su porta già aperta
									ClientData.DatiPorte[porta].RichiestaAttiva++;
							ClientData.DatiAgv[agv].consensodato = ClientData.DatiAgv[agv].nodob;
							ClientData.DatiAgv[agv].richiestaconsenso = 0;
							dmDB->LogMsg("Consenso attraversamento porta/sbarra " + IntToStr(porta) + " inviato ad Agv " + IntToStr(agv) + " in pos " + IntToStr(ClientData.DatiAgv[agv].pos) + " ,dest " +
								IntToStr(ClientData.DatiAgv[agv].dest));
							LogStatoPorta(porta);
							sprintf(ev, "O,R,%d,%d,%d,%d", agv, ClientData.DatiAgv[agv].nodoa, ClientData.DatiAgv[agv].nodob, 1);
							cs->SendEv(ev);
							dmDB->AggiornaStatoAgv(agv);
							if (ClientData.DatiAgv[agv].allarme_consenso_porta) {
								MainForm->MessageBar1->DelMsgNo(ClientData.DatiAgv[agv].num_allarme_consenso_porta);
								MainForm->MessageBar1->DelMsgNo(401); // sbarre
								ClientData.DatiAgv[agv].allarme_consenso_porta = 0;
								dmDB->AggiornaAllarmiAttivi();
							}
						}
					}
					else if ((porta >= 5) && (((res) || (ClientData.DatiPorte[porta].Stato >= 2)) && (!ClientData.DatiPorte[porta].ChiudiPorta) &&
						((ClientData.DatiPorte[porta].Stato == 3) || ((ClientData.DatiPorte[porta].PortaChiusa) &&
						((!ClientData.DatiPorte[porta].ApriPorta) || (ClientData.DatiPorte[porta].Stato == 2)))))) {
						// porta esclusa oppure aperta in manuale o con richiesta
						// porta esclusa oppure aperta,invio consenso
						// if (((porta >= 5) && (SbarreChiuse(5))) || (SbarreChiuse(3))) {
						if (ClientData.DatiAgv[agv].primo_ciclo_consenso) // gestione inversione agv su porta già aperta
								ClientData.DatiPorte[porta].RichiestaAttiva++;
						ClientData.DatiAgv[agv].consensodato = ClientData.DatiAgv[agv].nodob;
						ClientData.DatiAgv[agv].richiestaconsenso = 0;
						dmDB->LogMsg("Consenso attraversamento porta/sbarra " + IntToStr(porta) + " inviato ad Agv " + IntToStr(agv) + " in pos " + IntToStr(ClientData.DatiAgv[agv].pos) + " ,dest " + IntToStr(ClientData.DatiAgv[agv].dest));
						LogStatoPorta(porta);
						sprintf(ev, "O,R,%d,%d,%d,%d", agv, ClientData.DatiAgv[agv].nodoa, ClientData.DatiAgv[agv].nodob, 1);
						cs->SendEv(ev);
						dmDB->AggiornaStatoAgv(agv);
						if (ClientData.DatiAgv[agv].allarme_consenso_porta) {
							MainForm->MessageBar1->DelMsgNo(ClientData.DatiAgv[agv].num_allarme_consenso_porta);
							MainForm->MessageBar1->DelMsgNo(401); // sbarre
							ClientData.DatiAgv[agv].allarme_consenso_porta = 0;
							dmDB->AggiornaAllarmiAttivi();
						}
					}
					else if ((!ClientData.DatiAgv[agv].allarme_consenso_porta) && (ClientData.DatiAgv[agv].pos == ClientData.DatiAgv[agv].nodoa)) {
						if ((porta >= 3) && (porta <= 4))
							MainForm->MessageBar1->Messages->Strings[ClientData.DatiAgv[agv].num_allarme_consenso_porta] = "Agv in attesa sbarre chiuse per attraversamento";
						else if (!ClientData.DatiPorte[porta].ApriPorta)
							MainForm->MessageBar1->Messages->Strings[ClientData.DatiAgv[agv].num_allarme_consenso_porta] = "Agv in attesa chiusura sbarre per aprire la porta ";
						else
							MainForm->MessageBar1->Messages->Strings[ClientData.DatiAgv[agv].num_allarme_consenso_porta] = "Agv in Attesa apertura porta " + IntToStr(porta);
						MainForm->MessageBar1->AddMsgNo(ClientData.DatiAgv[agv].num_allarme_consenso_porta);
						ClientData.DatiAgv[agv].allarme_consenso_porta = 1;
						dmDB->AggiornaAllarmiAttivi();
					}
					// else if (!SbarreChiuse(porta)) {
					// MainForm->MessageBar1->AddMsgNo(301);
					// dmDB->AggiornaAllarmiAttivi();
					// }
					ClientData.DatiAgv[agv].primo_ciclo_consenso = 0;
				}
			}
			catch (...) {
				dmDB->LogMsg("Eccezione controllo str apertura porte con stringa ricevuta : " + MainForm->strcons[agv]);
			}
		}
	}
}

int TDMGestione_Porte::AggiungiRimuoviRichiestaConsensoPorta(AnsiString strconsenso, int aggiungi) {

	int j, res = 0;
	AnsiString str_porta;

	j = dmDB->ReturnIdPortaDaStringaConsensoPorta(strconsenso);
	if ((j > 0) && (j <= NUM_PORTE)) { // la porta tagliafuoco non la controllo
		if ((!aggiungi) && (ClientData.DatiPorte[j].RichiestaAttiva > 0))
			ClientData.DatiPorte[j].RichiestaAttiva--;
		else
			ClientData.DatiPorte[j].RichiestaAttiva = aggiungi;
		res = 1;
		dmDB->LogMsg("AggiungiRimuoviRichiestaConsensoPorta str " + strconsenso + " valore attuale : " + IntToStr(ClientData.DatiPorte[j].RichiestaAttiva));
	}
	return res;
}

int TDMGestione_Porte::ControllaPorteAperte() {

	int i, res = 0, agv_in_pos_apertura = 0, consenso_dato_agv_su_porta = 0;
	// for (i = 1; i <= NUM_PORTE; i++) {
	for (i = 1; i <= NUM_PORTE; i++) {
		if (ClientData.DatiPorte[i].tipoporta == 0) { // porte 0=porte. 1=sbarre
			if (((ClientData.DatiPorte[i].PlcConnesso) || (ClientData.ParametriFunzionali.Simula)) && (!ClientData.DatiPorte[i].forzature_attive)) {
				if ((((ClientData.DatiPorte[i].PortaAperta) || (ClientData.DatiPorte[i].ApriPorta) || (ClientData.DatiPorte[i].PortaTFuoco)) && (i < 5))
					|| ((((ClientData.DatiPorte[i].PortaChiusa) || (!ClientData.DatiPorte[i].ApriPorta)) && (i == 5))
					|| (((ClientData.DatiPorte[i].PortaChiusa) || (!ClientData.DatiPorte[i].ApriPorta)) && (i == 7))) // nuova porta
					) {
					res = AgvInPosPorta(i);
					agv_in_pos_apertura = AgvInPosAperturaPorta(i);
					consenso_dato_agv_su_porta = ConsensoInviatoAdAgvInPosAperturaPorta(i);
					if (i < 5) {
						if (dmDB->StatoPorta(i) != 1) {
							// porta non in automatico
							if ((ClientData.DatiPorte[i].ApriPorta) || (ClientData.DatiPorte[i].ChiudiPorta)) {
								// PLCPhoenixThread[0]->ScriviPhoenix(ClientData.DatiPhoenix[0].bit_apri_porta[i],ClientData.DatiPhoenix[0].addr_apri_porta[i],false) ;
								ApriChiudiPorta(i, ABBASSASEGNALIPORTA);
								dmDB->LogMsg("Abbasso segnale porta " + IntToStr(i) + " perchè porta non in automatico");
							}
							if (ClientData.DatiPorte[i].ChiudiPorta) {
								// PLCPhoenixThread[0]->ScriviPhoenix(ClientData.DatiPhoenix[0].bit_chiudi_porta[i],ClientData.DatiPhoenix[0].addr_chiudi_porta[i],false) ;
								// dmDB->LogMsg("Abbasso segnale chiusura porta "+IntToStr(i)+" perchè porta non in automatico");
							}
						}
						else if ((!res) && (!consenso_dato_agv_su_porta) && (!ClientData.DatiPorte[i].count_comando) &&
							((!agv_in_pos_apertura) || (!ClientData.DatiPorte[i].RichiestaAttiva))) {
							// chiudo la porta
							if (!ClientData.DatiPorte[i].memoria_apertura_operatore) {
								ClientData.DatiPorte[i].RichiestaAttiva = 0;
								if (ClientData.DatiPorte[i].ApriPorta) // || (!ClientData.DatiPorte[i].ChiudiPorta))
										dmDB->LogMsg("Chiudo Porta " + IntToStr(i) + " perchè nessun agv in zona");
								// sostituire con   ApriChiudiPorta(i, ABBASSASEGNALIPORTA); ???
								ApriChiudiPorta(i, CHIUDIPORTA);
							}
							else {
								// tolgo il segnale di apertura se cè la memoria dell'apertura operatore
								if (ClientData.DatiPorte[i].ApriPorta)
									dmDB->LogMsg("Abbasso Segnali Porta " + IntToStr(i) + " perchè agv transitato e memoria operatore alta");
								ApriChiudiPorta(i, ABBASSASEGNALIPORTA);
							}
							// dmDB->AggiornaParametri(i);
						}
						else if ((res) && (consenso_dato_agv_su_porta) && (!ClientData.DatiAgv[consenso_dato_agv_su_porta].manuale)) {
							// chiudo la porta
							if (((!ClientData.DatiPorte[i].PortaAperta) && (ClientData.DatiPorte[i].ApriPorta)) ||
								(ClientData.DatiPorte[i].ChiudiPorta)) {
								if (!ClientData.DatiAgv[consenso_dato_agv_su_porta].stand_by) {
									dmComandiAgv->StandByAgv(consenso_dato_agv_su_porta, 1);
									dmDB->LogMsg("Blocco agv " + IntToStr(consenso_dato_agv_su_porta) + " perchè segnale porta aperta mancante su porta " + IntToStr(i));
									LogStatoPorta(i);
								}
							}
							else if ((ClientData.DatiAgv[consenso_dato_agv_su_porta].stand_by) && (!ClientData.DatiPorte[i].ChiudiPorta)) {
								if ((ClientData.DatiPorte[i].PortaAperta) && (ClientData.DatiPorte[i].ApriPorta)) {
									if (!ClientData.DatiAgv[consenso_dato_agv_su_porta].stand_by_antincendio) {
										dmComandiAgv->StandByAgv(consenso_dato_agv_su_porta, 0);
										dmDB->LogMsg("Riavvio agv " + IntToStr(consenso_dato_agv_su_porta) + " perchè segnale porta aperta ritornato su porta " + IntToStr(i));
										LogStatoPorta(i);
									}
								}
							}
						}
					}
					else {
						if (dmDB->StatoPorta(i) != 1) {
							// porta non in automatico
							if (!ClientData.DatiPorte[i].ApriPorta) {
								// PLCPhoenixThread[0]->ScriviPhoenix(ClientData.DatiPhoenix[0].bit_apri_porta[i],ClientData.DatiPhoenix[0].addr_apri_porta[i],false) ;
								ApriChiudiPorta(i, APRIPORTA);
								dmDB->LogMsg("Alzo segnale consenso apertura porta " + IntToStr(i) + " perchè porta non in automatico");
							}
							if (ClientData.DatiPorte[i].ChiudiPorta) {
								// PLCPhoenixThread[0]->ScriviPhoenix(ClientData.DatiPhoenix[0].bit_chiudi_porta[i],ClientData.DatiPhoenix[0].addr_chiudi_porta[i],false) ;
								// dmDB->LogMsg("Abbasso segnale chiusura porta "+IntToStr(i)+" perchè porta non in automatico");
							}
						}
						else if ((!res) && (!consenso_dato_agv_su_porta) && (!ClientData.DatiPorte[i].count_comando) &&
							((!agv_in_pos_apertura) || (!ClientData.DatiPorte[i].RichiestaAttiva))) {
							// chiudo la porta
							ClientData.DatiPorte[i].RichiestaAttiva = 0;
							if (!ClientData.DatiPorte[i].ApriPorta) // || (!ClientData.DatiPorte[i].ChiudiPorta))
									dmDB->LogMsg("Alzo segnale consenso apertura porta " + IntToStr(i) + " perchè agv non in zona ");
							// sostituire con   ApriChiudiPorta(i, ABBASSASEGNALIPORTA); ???
							ApriChiudiPorta(i, APRIPORTA);
						}
					}

				}
			}
		}
	}
	return res;
}

int TDMGestione_Porte::AgvInPostazioni_Libere_Antincendio(int agv) {
	int i, res = 0;
	AnsiString pos_porta_aperta;
	for (i = 0; i < 10; i++) {
		if (ClientData.DatiAgv[agv].pos == Postazioni_Libere_Antincendio[i]) {
			res = i;
			break;
		}
	}
	return res;
}

int TDMGestione_Porte::AgvInPosPorta(int porta) {
	int i, res = 0;
	AnsiString pos_porta_aperta;
	if ((porta > 0) && (porta <= NUM_PORTE)) {
		pos_porta_aperta = dmDB->PosizioniPortaAperta(porta);
		for (i = 1; i <= NAGV; i++) {
			if (pos_porta_aperta.Pos(";" + IntToStr(ClientData.DatiAgv[i].pos) + ";")) {
				res = i;
				break;
			}
		}
	}
	return res;
}

int TDMGestione_Porte::AgvInPosRichiestaAperturaPorta(int porta, int controlla_agv) {
	int i, res = 0;
	AnsiString pos_apertura;
	if ((porta > 0) && (porta <= NUM_PORTE)) {
		pos_apertura = dmDB->PosizioniRichiestaAperturaPorta(porta);
		for (i = 1; i <= NAGV; i++) {
			if (pos_apertura.Pos(";" + IntToStr(ClientData.DatiAgv[i].pos) + ";")) {
				res = i;
				break;
			}
		}
	}
	return res;
}

int TDMGestione_Porte::AgvInPosAperturaPorta(int porta, int controlla_agv) {
	int i, res = 0;
	AnsiString pos_porta_aperta, pos_apertura;
	if ((porta > 0) && (porta <= NUM_PORTE)) {
		pos_apertura = dmDB->PosizioniRichiestaAperturaPorta(porta);
		pos_porta_aperta = dmDB->PosizioniPortaAperta(porta);
		for (i = 1; i <= NAGV; i++) {
			if ((controlla_agv == 0) || (controlla_agv == i)) {
				if ((pos_apertura.Pos(";" + IntToStr(ClientData.DatiAgv[i].pos) + ";")) ||
					(pos_porta_aperta.Pos(";" + IntToStr(ClientData.DatiAgv[i].pos) + ";"))) {
					res = i;
					break;
				}
			}
		}
	}
	return res;
}

int TDMGestione_Porte::ConsensoInviatoAdAgvInPosAperturaPorta(int porta, int controlla_agv) {
	int i, res = 0;
	AnsiString pos_porta_aperta, pos_apertura;
	// ulteriore controll odi non avere un consenso dato su una posizone presente nella lista della porta che voglio chiudere
	if ((porta > 0) && (porta <= NUM_PORTE)) {
		pos_apertura = dmDB->PosizioniRichiestaAperturaPorta(porta);
		pos_porta_aperta = dmDB->PosizioniPortaAperta(porta);
		for (i = 1; i <= NAGV; i++) {
			if ((controlla_agv == 0) || (controlla_agv == i)) {
				if ((pos_apertura.Pos(";" + IntToStr(ClientData.DatiAgv[i].consensodato) + ";")) ||
					(pos_porta_aperta.Pos(";" + IntToStr(ClientData.DatiAgv[i].consensodato) + ";"))) {
					res = i;
					break;
				}
			}
		}
	}
	return res;
}

int TDMGestione_Porte::ApriChiudiPorta(int porta, int apri) {
	int i, res = 0;

	if ((porta > 0) && (porta <= NUM_PORTE)) {
		// DataModulePhoenix->ScriviPhoenix(MainForm->PlcPhoenix[baia].bit_apri_chiudi_porta[porta],MainForm->PlcPhoenix[baia].add_apri_chiudi_porta[porta],true,baia) ;
		if (ClientData.DatiPorte[porta].tipomodulo == 2) {
			// modulo phoenix
			if (apri == 1) {
				// alzo aoertura e abbasso chiusura
				if (ClientData.DatiPorte[porta].Bit_ApriPorta >= 0)
					PLCPhoenixThread[ClientData.DatiPorte[porta].id_thread_plc]->ScriviPhoenix(ClientData.DatiPorte[porta].Bit_ApriPorta, ClientData.DatiPorte[porta].DW_Addr_ApriPorta, true);
			}
			else if (apri == 0) {
				// alzo chiusura e abbasso apertura
				if (ClientData.DatiPorte[porta].Bit_ApriPorta >= 0)
					PLCPhoenixThread[ClientData.DatiPorte[porta].id_thread_plc]->ScriviPhoenix(ClientData.DatiPorte[porta].Bit_ApriPorta, ClientData.DatiPorte[porta].DW_Addr_ApriPorta, false);
				if (ClientData.DatiPorte[porta].Bit_ChiudiPorta >= 0)
					PLCPhoenixThread[ClientData.DatiPorte[porta].id_thread_plc]->ScriviPhoenix(ClientData.DatiPorte[porta].Bit_ChiudiPorta, ClientData.DatiPorte[porta].DW_Addr_ChiudiPorta, false);

			}

			else if (apri == 3) {
				// alzo chiusura e abbasso apertura
				if (ClientData.DatiPorte[porta].Bit_ChiudiPorta >= 0)
					PLCPhoenixThread[ClientData.DatiPorte[porta].id_thread_plc]->ScriviPhoenix(ClientData.DatiPorte[porta].Bit_ChiudiPorta, ClientData.DatiPorte[porta].DW_Addr_ChiudiPorta, true);
				if (ClientData.DatiPorte[porta].Bit_ApriPorta >= 0)
					PLCPhoenixThread[ClientData.DatiPorte[porta].id_thread_plc]->ScriviPhoenix(ClientData.DatiPorte[porta].Bit_ApriPorta, ClientData.DatiPorte[porta].DW_Addr_ApriPorta, false);

			}

		}

	}
	return res;
}

void TDMGestione_Porte::ControllaResetSegnaleApriChiudiPorta(int tiporeset) {
	int i;
	int impulso = true;
	// vedere se serve resettare il comando
	// se si resetta il consenso va dato solo col segnale apertura
	// tiporeset parametrizzo se devo togliereil segnale apri porta con porta aperta, e chiudi porta con porta non aperta
	for (i = 1; i <= NUM_PORTE; i++) {
		// for (i = 1; i <= 2; i++) {
		if (((ClientData.DatiPorte[i].PlcConnesso) || (ClientData.ParametriFunzionali.Simula)) && (!ClientData.DatiPorte[i].forzature_attive)) {
			if ((ClientData.DatiPorte[i].PortaAperta) && (ClientData.DatiPorte[i].ApriPorta) && (tiporeset & dmDB->bit[0])) {
				// abbasso apri porta dopo segnale porta aperta
				// PLCPhoenixThread[0]->ScriviPhoenix(ClientData.DatiPorte[i].bit_apri_porta[i],ClientData.DatiPorte[i].addr_apri_porta[i],false) ;
				if (i > 2) {
					impulso = false;
				}
				if (impulso == true) {
					ApriChiudiPorta(i, ABBASSASEGNALIPORTA);
					dmDB->LogMsg("Abbasso segnale apertura porta " + IntToStr(i) + " con porta aperta e comando alto");
				}
			}
			else if ((!ClientData.DatiPorte[i].PortaAperta) && (ClientData.DatiPorte[i].ChiudiPorta) && (tiporeset & dmDB->bit[1]) && (dmDB->StatoPorta(i) == 1)) {
				// abbasso apri porta dopo segnale porta chiusa

				ApriChiudiPorta(i, ABBASSASEGNALIPORTA);
				dmDB->LogMsg("Abbasso segnale chiusura porta " + IntToStr(i) + " con porta chiusa ");
				// PLCPhoenixThread[0]->ScriviPhoenix(ClientData.DatiPorte[i].bit_chiudi_porta[i],ClientData.DatiPorte[i].addr_chiudi_porta[i],false) ;
				// dmDB->LogMsg("Abbasso segnale chiusura porta "+IntToStr(i)+" con porta non più aperta e comando alto");
			}
		}
	}
}

int TDMGestione_Porte::GestioneRotAlarm() {

	int i, j, res = 0, value;

	/* for (i=1 ; i<= MainForm->num_rot_alarm ; i++)  {
	 if ((!MainForm->DatiPlc[1].forzatura_attiva)&&((MainForm->DatiPlc[1].PlcConnesso)||(ClientData.Simula))) {
	 res = AgvInPosRotAlarm(i) ;
	 if (((!res)&&(MainForm->DatiPlc[1].rot_alarm[i]))||
	 ((res)&&(!MainForm->DatiPlc[1].rot_alarm[i]))){
	 //accendo - spengo luce rotalarm
	 SettaRotAlarm(i,res);
	 }
	 }
	 } */
	return res;
}

int TDMGestione_Porte::AgvInPosRotAlarm(int rotalarm) {
	int i, res = 0;

	/* for ( i = 1 ; i<= NAGV ; i++) {
	 //vedere se accendere solo se l'agv è in missione
	 if ((MainForm->DatiPlc[1].pos_rot_alarm[rotalarm].Pos (";"+IntToStr(ClientData.DatiAgv[i].pos)+";"))) {
	 res = i ;
	 break ;
	 }
	 } */
	return res;
}

int TDMGestione_Porte::SettaRotAlarm(int rotalarm, bool setta) {
	int i, res = 0;

	return res;
}

void __fastcall TDMGestione_Porte::DataModuleCreate(TObject * Sender)
{
	TimerEventi->Enabled = true;
	TimerPlcPorte->Enabled = false;
}
// ---------------------------------------------------------------------------

void __fastcall TDMGestione_Porte::TimerPlcPorteTimer(TObject * Sender)
{
	int agv, i, j;
	char code[24];
	// imposto dati in memoria da Plc
	if ((MainForm->count_inizio < 15) || (MainForm->close_program))
		return;
	TimerPlcPorte->Enabled = false;

	for (i = 1; i <= NUM_PORTE; i++) {
		if ((lettodatiporte) && (ClientData.DatiPorte[i].id_thread_plc >= 0))
			CompilaStrutturaPlcPorte(ClientData.DatiPorte[i]);
	}

	for (i = 1; i <= NUM_SEMAFORI; i++) {
		if ((lettodatiporte) && (ClientData.DatiSemafori[i].id_thread_plc >= 0))
			CompilaStrutturaPlcSemafori(ClientData.DatiSemafori[i]);

	}
    /*
	for (i = 1; i <= NUM_SENSORI; i++) {
		if ((lettodatiporte) && (ClientData.Sensori[i].id_thread_plc >= 0))
			CompilaStrutturaSensori(ClientData.Sensori[i]);
	}
     */
	if (MainForm->count_inizio > 20) {
		// if (ClientData.SegnaleAntincendio) {
		for (agv = 1; agv <= NAGV; agv++) {
			if ((!ClientData.DatiAgv[agv].stand_by_antincendio) && (!ClientData.DatiAgv[agv].manuale) && (ClientData.DatiAgv[agv].pos > 0)) {
				if (!AgvInPostazioni_Libere_Antincendio(agv)) {
					dmComandiAgv->StandByAgv(agv, 1);
					dmDB->LogMsg("Blocco agv " + IntToStr(agv) + " per segnale antincendio attivo ");
					ClientData.DatiAgv[agv].stand_by_antincendio = 1;
				}
			}
		}

	}

	// aggiungere gestione segnali lavatrice
	init = false;
	TimerPlcPorte->Enabled = true;

}

// ---------------------------------------------------------------------------
void TDMGestione_Porte::CompilaStrutturaPlcPorte(TPorte & StructPlcPorte) {
	AnsiString stringa;
	int i, j, indice_byte;
	// compilo i dati della struttura plc passata

	StructPlcPorte.PlcConnesso = PLCPhoenixThread[StructPlcPorte.id_thread_plc]->connesso;
	if ((PLCPhoenixThread[StructPlcPorte.id_thread_plc]->connesso) || (ClientData.ParametriFunzionali.Simula)) {

		if ((StructPlcPorte.Bit_PortaAperta >= 0) && (StructPlcPorte.DW_Addr_PortaAperta >= 0)) {
			StructPlcPorte.PortaAperta = PLCPhoenixThread[StructPlcPorte.id_thread_plc]->IngressiPhoenix[StructPlcPorte.DW_Addr_PortaAperta] & dmDB->bit[StructPlcPorte.Bit_PortaAperta];
		}
		if ((StructPlcPorte.Bit_PortaChiusa >= 0) && (StructPlcPorte.DW_Addr_PortaChiusa >= 0)) {
			StructPlcPorte.PortaChiusa = PLCPhoenixThread[StructPlcPorte.id_thread_plc]->IngressiPhoenix[StructPlcPorte.DW_Addr_PortaChiusa] & dmDB->bit[StructPlcPorte.Bit_PortaChiusa];
		}
		if ((StructPlcPorte.PortaChiusa == 0) || (StructPlcPorte.Bit_PortaAperta == 0)) {
			// dmDB->LogMsg("Stato porta cambiato ");
		}
		// StructPlcPorte.PortaChiusa = StructPlcPorte.ByteInput & dmDB->bit[StructPlcPorte.Bit_PortaChiusa];
		if ((StructPlcPorte.Bit_PortaTFuocoAperta >= 0) && (StructPlcPorte.DW_Addr_PortaTFuocoAperta >= 0)) {
			StructPlcPorte.PortaTFuoco = PLCPhoenixThread[StructPlcPorte.id_thread_plc]->IngressiPhoenix[StructPlcPorte.DW_Addr_PortaTFuocoAperta] & dmDB->bit[StructPlcPorte.Bit_PortaTFuocoAperta];
		}

		// impostazioni variabili uscite
		if ((StructPlcPorte.Bit_ApriPorta >= 0) && (StructPlcPorte.DW_Addr_ApriPorta >= 0)) {
			StructPlcPorte.ApriPorta = PLCPhoenixThread[StructPlcPorte.id_thread_plc]->UscitePhoenix[StructPlcPorte.DW_Addr_ApriPorta] & dmDB->bit[StructPlcPorte.Bit_ApriPorta];
		}
		if ((StructPlcPorte.Bit_ChiudiPorta >= 0) && (StructPlcPorte.DW_Addr_ChiudiPorta >= 0)) {
			StructPlcPorte.ChiudiPorta = PLCPhoenixThread[StructPlcPorte.id_thread_plc]->UscitePhoenix[StructPlcPorte.DW_Addr_ChiudiPorta] & dmDB->bit[StructPlcPorte.Bit_ChiudiPorta];
		}
		StructPlcPorte.memoria_apertura_operatore = 0;
	}
	// gestisco uscite :
	if (MainForm->count_inizio > 20) {

		if (StructPlcPorte.forzature_attive) {
			if (StructPlcPorte.count_forzature > 30) {
				StructPlcPorte.count_forzature = 0;
				StructPlcPorte.forzature_attive = 0;
			}
			else
				StructPlcPorte.count_forzature++;
		}
		else
			StructPlcPorte.count_forzature = 0;
	}

}

void TDMGestione_Porte::CompilaStrutturaPlcSemafori(Semaforo & StructPlcSemaforo) {
	AnsiString stringa;
	int i, j, indice_byte;
	// compilo i dati della struttura plc passata

	StructPlcSemaforo.PlcConnesso = PLCPhoenixThread[StructPlcSemaforo.id_thread_plc]->connesso;
	if ((PLCPhoenixThread[StructPlcSemaforo.id_thread_plc]->connesso) || (ClientData.ParametriFunzionali.Simula)) {
		if ((StructPlcSemaforo.Bit_LuceVerde >= 0) && (StructPlcSemaforo.DW_Addr_LuceVerde >= 0)) {
			StructPlcSemaforo.LuceVerde = (PLCPhoenixThread[StructPlcSemaforo.id_thread_plc]->UscitePhoenix[StructPlcSemaforo.DW_Addr_LuceVerde] & dmDB->bit[StructPlcSemaforo.Bit_LuceVerde]);
			// impostazioni variabili uscite
			if ((StructPlcSemaforo.Bit_richiesta_ingresso >= 0) && (StructPlcSemaforo.DW_Addr_richiesta_ingresso >= 0))
				StructPlcSemaforo.richiestaIngresso = PLCPhoenixThread[StructPlcSemaforo.id_thread_plc]->IngressiPhoenix[StructPlcSemaforo.DW_Addr_richiesta_ingresso] & dmDB->bit[StructPlcSemaforo.Bit_richiesta_ingresso];
			// StructPlcSemaforo.forzature_attive = 1;
		}
		if ((StructPlcSemaforo.Bit_LuceRossa >= 0) && (StructPlcSemaforo.DW_Addr_LuceRossa >= 0)) {
			StructPlcSemaforo.LuceRossa = (PLCPhoenixThread[StructPlcSemaforo.id_thread_plc]->UscitePhoenix[StructPlcSemaforo.DW_Addr_LuceRossa] & dmDB->bit[StructPlcSemaforo.Bit_LuceRossa]);

		}
	}

}

void TDMGestione_Porte::CompilaStrutturaSensori(TSensori & StructSensori) {
	AnsiString stringa;
	int i, j, indice_byte;
	// compilo i dati della struttura plc passata

	StructSensori.PlcConnesso = PLCPhoenixThread[StructSensori.id_thread_plc]->connesso;
	if ((PLCPhoenixThread[StructSensori.id_thread_plc]->connesso) || (ClientData.ParametriFunzionali.Simula)) {

		if ((StructSensori.Bit_Lettura >= 0) && (StructSensori.DW_Addr_Lettura >= 0)) {
			StructSensori.PresenzaAllarme = PLCPhoenixThread[StructSensori.id_thread_plc]->IngressiPhoenix[StructSensori.DW_Addr_Lettura] & dmDB->bit[StructSensori.Bit_Lettura];
		}

	}
}

void TDMGestione_Porte::ModificaByteStrutturaPorte(int input, int idporta, int byte, int value)
{
	int i;
	if ((idporta) && (idporta < NUM_PORTE)) {
		if (input) {
			// ClientData.DatiPorte[idporta].ByteInput = value;
		}
		else {
			ClientData.DatiPorte[idporta].forzature_attive = true;
			if (value) {
				ApriChiudiPorta(idporta, APRIPORTA);
			}
			else
				ApriChiudiPorta(idporta, ABBASSASEGNALIPORTA);
		}
	}
}

int TDMGestione_Porte::GestioneSemafori() {

	int i, j, res = 0;
	// usato come rotalarm, con semafori veri rivedere tutto

	if (ClientData.DatiSemafori[1].PlcConnesso) {
		// in futuro mettere su + agv

		if ((ClientData.DatiAgv[1].alarm > 0) || (ClientData.DatiAgv[2].alarm > 0)) {
			if (ClientData.DatiSemafori[1].LuceRossa > 0) {
				SettaLuceSemaforo(1, 1);
			}
			else {
				SettaLuceSemaforo(1, 0);
			}
		}
		else {
			SettaLuceSemaforo(1, 1); // spengo il rosso

		}
	}

	return res;
}

int TDMGestione_Porte::SettaLuceSemaforo(int semaforo, int verde) {
	int i, res = 0;

	if ((semaforo > 0) && (semaforo <= 2)) {
		if (verde) {
			// MainForm->DatiPlc[1].statorot_alarm_dascrivere |= MainForm->bit[MainForm->DatiPlc[1].bitrot_alarm[rotalarm]] ;
			if ((ClientData.DatiSemafori[semaforo].Bit_LuceVerde >= 0) && (ClientData.DatiSemafori[semaforo].DW_Addr_LuceVerde >= 0)) {
				PLCPhoenixThread[ClientData.DatiSemafori[semaforo].id_thread_plc]->ScriviPhoenix(ClientData.DatiSemafori[semaforo].Bit_LuceVerde, ClientData.DatiSemafori[semaforo].DW_Addr_LuceVerde, true);
			}
			if ((ClientData.DatiSemafori[semaforo].Bit_LuceRossa >= 0) && (ClientData.DatiSemafori[semaforo].DW_Addr_LuceRossa >= 0)) {
				PLCPhoenixThread[ClientData.DatiSemafori[semaforo].id_thread_plc]->ScriviPhoenix(ClientData.DatiSemafori[semaforo].Bit_LuceRossa, ClientData.DatiSemafori[semaforo].DW_Addr_LuceRossa, false);
			}
			// dmDB->LogMsg("Setto Luce Verde Accesa e Rossa Spenta Semaforo " + IntToStr(semaforo));
		}
		else {
			if ((ClientData.DatiSemafori[semaforo].Bit_LuceVerde >= 0) && (ClientData.DatiSemafori[semaforo].DW_Addr_LuceVerde >= 0)) {
				PLCPhoenixThread[ClientData.DatiSemafori[semaforo].id_thread_plc]->ScriviPhoenix(ClientData.DatiSemafori[semaforo].Bit_LuceVerde, ClientData.DatiSemafori[semaforo].DW_Addr_LuceVerde, false);
			}
			if ((ClientData.DatiSemafori[semaforo].Bit_LuceRossa >= 0) && (ClientData.DatiSemafori[semaforo].DW_Addr_LuceRossa >= 0)) {
				PLCPhoenixThread[ClientData.DatiSemafori[semaforo].id_thread_plc]->ScriviPhoenix(ClientData.DatiSemafori[semaforo].Bit_LuceRossa, ClientData.DatiSemafori[semaforo].DW_Addr_LuceRossa, true);
			}
		}
	}
	return res;
}

void TDMGestione_Porte::ControllaSbarreDaChiudere() {
	int possbarra = 0;
	for (int agv = 1; agv <= NAGV; agv++) { // sarebbe da non mettere perche' il controllo agv e' sia' dentro la funzione agvinposporta. Da togliere quando tutto ok

		for (int i = 3; i <= NUM_PORTE; i++) {
			possbarra = AgvInPosPorta(i);
			// possbarra = AgvInPosAperturaPorta(i, agv);
			if (possbarra) {
				// ApriChiudiPorta(i, ABBASSASEGNALIPORTA);
				if (i < 5) {
					ApriChiudiPorta(i, APRIPORTA);
				}
				else {
					ApriChiudiPorta(i, CHIUDIPORTA);
				}
				// ApriChiudiPorta(i, CHIUDIPORTA);  NO
			}
			else {
				// controllare, forse non serve
				// ApriChiudiPorta(i, APRIPORTA);
			}
		}
	}
}

int TDMGestione_Porte::SbarreChiuse(int porta) {
	int ret = 0;
	if ((porta == 4) || (porta == 6) || (porta == 8)) {
		porta--;
	}
	if (porta == 3) {
		if (((ClientData.DatiPorte[porta].PortaAperta) && (ClientData.DatiPorte[porta + 1].PortaAperta) &&
			(!ClientData.DatiPorte[porta].ApriPorta) && (!ClientData.DatiPorte[porta + 1].ApriPorta)) ||
			((ClientData.DatiPorte[porta].Stato == 3) && (ClientData.DatiPorte[porta + 1].Stato == 3)) ||
			((ClientData.DatiPorte[porta].Stato == 0) && (ClientData.DatiPorte[porta + 1].Stato == 0))) {
			ret = 1;
		}
	}
	else if ((porta == 5) || (porta == 7)) {
		if (((ClientData.DatiPorte[porta].PortaChiusa) && (!ClientData.DatiPorte[porta + 1].PortaChiusa) &&
			(!ClientData.DatiPorte[porta].ApriPorta) && (!ClientData.DatiPorte[porta + 1].ApriPorta)) ||
			((ClientData.DatiPorte[porta].Stato == 3) && (ClientData.DatiPorte[porta + 1].Stato == 3)) ||
			((ClientData.DatiPorte[porta].Stato == 0) && (ClientData.DatiPorte[porta + 1].Stato == 0))) {
			ret = 1;
		}
	}

	return ret;
}
