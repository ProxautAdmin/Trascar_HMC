//---------------------------------------------------------------------------

#ifndef threadmodbusclientH
#define threadmodbusclientH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "IdModBusClient.hpp"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include "ModbusTypes.hpp"
#include <vector>

#define N_PLC           2

#define MAX_REGISTER 	10

#define	WRITEREGISTER  	1
#define	WRITEREGISTERS  2
//---------------------------------------------------------------------------
struct MODBUS{
	WORD Register[MAX_REGISTER] ;
	WORD RegisterOld[MAX_REGISTER] ;//se serve fare eventi
	int EventRegister[MAX_REGISTER] ;//se serve fare eventi
	int num_registri;
	int indice_inizio_registro;
} ;
struct ModbusWriteCom {
	int indice_inizio_registro;
	int num_registri;
	int cmd;
	WORD RegisterToWrite[MAX_REGISTER] ;
};
class ModBusClientThread : public TThread
{
private:
protected:
	void __fastcall Execute();
public:
	__fastcall ModBusClientThread(bool CreateSuspended,int IdModbus);

	void ConnettiPLCModbus() ;
	void DisconnettiPLCModbus() ;
	void ScriviRegistro(int indice_inizio_registro , WORD RegistriDaScrivere,bool simulazione = false) ;
	void ScriviRegistri(int indice_inizio_registro , int num_registri , WORD * RegistriDaScrivere,bool simulazione = false) ;
	void StopThread() ;

	std::vector<ModbusWriteCom> AsyncCom;
	TIdModBusClient *IdModBusClient;
	AnsiString PLCAddress;
	int PLCPort,tentativi;
	int ok_read_write,id ;
	bool Stopped,devo_scrivere,connected ,init;
	MODBUS Register_Plc ;

};
extern ModBusClientThread *PLCThreadModbus[N_PLC];
//---------------------------------------------------------------------------
#endif
