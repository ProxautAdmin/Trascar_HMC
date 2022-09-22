//---------------------------------------------------------------------------

#include <vcl.h>
#include <IniFiles.hpp>
#include <System.WideStrUtils.hpp>
#pragma hdrstop

#include "threadmodbusclient.h"
#include "db.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall ModBusClientThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
ModBusClientThread *PLCThreadModbus[N_PLC];

__fastcall ModBusClientThread::ModBusClientThread(bool CreateSuspended,int IdModbus)
	: TThread(CreateSuspended)
{
	TIniFile *ini;
    int i ;
//	NameThreadForDebugging("PLCThread", ThreadID);
	Stopped = false;
	ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
	PLCAddress = ini->ReadString("PLCMODBUSCLIENT"+IntToStr(IdModbus), "PLCAddress", "127.0.0.1");
	PLCPort = StrToInt(ini->ReadString("PLCMODBUSCLIENT"+IntToStr(IdModbus), "PLCPort", "502"));
	delete ini;
	connected = false ;
	AsyncCom.clear();
	memset(Register_Plc.Register, 0, sizeof(MAX_REGISTER));
	memset(Register_Plc.RegisterOld, 0, sizeof(MAX_REGISTER));
	memset(Register_Plc.EventRegister, 0, sizeof(MAX_REGISTER));
   /*	for (i  =0 ;i<MAX_REGISTER ;i++) {
		Register_Plc.Register[i] = 0 ;
		Register_Plc.RegisterOld[i] = 0 ;
		Register_Plc.EventRegister[i] = 0 ;
	}  */
	Register_Plc.indice_inizio_registro = 1 ;
	Register_Plc.num_registri = MAX_REGISTER ;
	id = IdModbus ;
	init = true ;
}
//---------------------------------------------------------------------------
void __fastcall ModBusClientThread::Execute()
{
	//---- Place thread code here ----
	int i ;
	Sleep(5000);
	while (!Stopped) {
		try {
			if (init) {
				try {
					ConnettiPLCModbus();
				}catch(...){}
				init = false ;
			}else {
				try {
					connected = IdModBusClient->Connected();

				} catch (...) {
					connected = false ;
				}
				if (!connected) {
					try {
						ConnettiPLCModbus();
					}catch(...){}
				}
				else  {
					// Lettura dati da PLC
					ok_read_write = false ;
					if (!AsyncCom.empty()) {
						switch (AsyncCom[0].cmd) {
							try {

								case WRITEREGISTER:
									ok_read_write = IdModBusClient->WriteRegister(AsyncCom[0].indice_inizio_registro,AsyncCom[0].RegisterToWrite[AsyncCom[0].indice_inizio_registro]);
									break;
								case WRITEREGISTERS:
									//const int RegisterData_High è l'indice più alto, quindi il numero di registri da scrivere - 1 partendo dall'indice_inizio_registro
									ok_read_write = IdModBusClient->WriteRegisters(AsyncCom[0].indice_inizio_registro,AsyncCom[0].RegisterToWrite,AsyncCom[0].num_registri - 1);
									break;
							} catch (...) {
								dmDB->LogMsg("Eccezione modulo modbus plc id "+IntToStr(id)+" dopo scrittura");
							}
						}
						if (!ok_read_write) {
							tentativi++;
							if (tentativi > 10) {
								AsyncCom.erase(AsyncCom.begin());
								DisconnettiPLCModbus();
								dmDB->LogMsg("Disconnesso modulo modbus plc id "+IntToStr(id)+" dopo 10 tentavivi di scrittura");
							}else
								dmDB->LogMsg("Scrittura non riuscita su modulo modbus plc id "+IntToStr(id));

						}else {
							AsyncCom.erase(AsyncCom.begin());
							tentativi = 0 ;
						}
						Sleep(20);
					}//else {//leggo
					//leggo ad ogni giro
					ok_read_write = false ;
					try {

						ok_read_write = IdModBusClient->ReadHoldingRegisters(Register_Plc.indice_inizio_registro,Register_Plc.num_registri,Register_Plc.Register,Register_Plc.indice_inizio_registro + Register_Plc.num_registri - 1);
						if (ok_read_write) {
							for (i  =0 ;i<MAX_REGISTER ;i++) {
								if (Register_Plc.Register[i] != Register_Plc.RegisterOld[i])
									Register_Plc.EventRegister[i] = 1 ; //salvo un evento al cambio di stato di un registro
								Register_Plc.RegisterOld[i] = Register_Plc.Register[i] ;
							}
						}else {
							DisconnettiPLCModbus();
							dmDB->LogMsg("Lettura non riuscita su modulo modbus plc id "+IntToStr(id));
						}
					} catch (...) {
						dmDB->LogMsg("Eccezione modulo modbus plc id "+IntToStr(id)+" dopo lettura");
					}

					//}
				}
			Sleep(50);
			}
		} catch (...) {
			dmDB->LogMsg("Tread modbus id "+IntToStr(id)+" in eccezione");
			DisconnettiPLCModbus();
			Sleep(1000);
		}
	}
	if (connected) {
		DisconnettiPLCModbus();
		Suspend();
	}
}
//---------------------------------------------------------------------------
void ModBusClientThread::ConnettiPLCModbus() {
	IdModBusClient = new TIdModBusClient (NULL);
	IdModBusClient->Host = PLCAddress ;
	IdModBusClient->Port = PLCPort;
	try {
		IdModBusClient->Connect();
	}catch(...){}
	tentativi = 0;
}

void ModBusClientThread::DisconnettiPLCModbus() {
	connected = false;
	try {
		if (connected) {
			IdModBusClient->Disconnect();
		}
	}catch(...){}
  // delete IdModBusClient ;
	tentativi = 0;
}
void ModBusClientThread::ScriviRegistro(int indice_inizio_registro , WORD RegistriDaScrivere,bool simulazione) {
	ModbusWriteCom c;

	memset(c.RegisterToWrite, 0, sizeof(MAX_REGISTER));
	c.indice_inizio_registro = indice_inizio_registro;
	c.num_registri = 1;
	c.cmd = WRITEREGISTER;
	if ((simulazione)&&(!connected)) { //se sono collegato scrivo sul plc anche in simulazione
		// scrivo in memoria i valori già quando richiamo la funzione, non eseguo la scrittura
		if (indice_inizio_registro > 0 )
			Register_Plc.Register[indice_inizio_registro-1] = RegistriDaScrivere ;  //in scrittura passo il registro + 1 , in simulazione cambio quello che poi rileggo
	}else {
		c.RegisterToWrite[indice_inizio_registro] = RegistriDaScrivere ;
		AsyncCom.push_back(c);
	}
}
void ModBusClientThread::ScriviRegistri(int indice_inizio_registro , int num_registri , WORD * RegistriDaScrivere,bool simulazione) {
	ModbusWriteCom c;

	memset(c.RegisterToWrite, 0, sizeof(MAX_REGISTER));
	c.indice_inizio_registro = indice_inizio_registro;
	c.num_registri = num_registri;
	c.cmd = WRITEREGISTERS;
//	c.RegisterToWrite = Register_Plc.Register ;
	if ((simulazione)&&(!connected)) {
		// scrivo in memoria i valori già quando richiamo la funzione, non eseguo la scrittura
		if (indice_inizio_registro > 0 )
			memcpy(Register_Plc.Register + indice_inizio_registro - 1 , RegistriDaScrivere, num_registri);
	}else {
		memcpy(c.RegisterToWrite , RegistriDaScrivere, MAX_REGISTER);
		AsyncCom.push_back(c);
	}
}
void ModBusClientThread::StopThread() {
	Stopped = true;
}

