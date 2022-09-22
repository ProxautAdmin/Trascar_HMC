//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "datamodulemodbustcp.h"
#include "db.h"
#include "main.h"
#include "datamodulecomandiagv.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "IdModBusClient"
#pragma link "ModbusTypes"
#pragma resource "*.dfm"
TDataModuleModBus *DataModuleModBus;
//---------------------------------------------------------------------------
    /* Protocollo Modbus */
    /*
    buffer_response[0] = 0 ;  // id transazione byte hight (sempre a 0)
    buffer_response[1] = 0 ;  // id transazione byte low (sempre a 0)
    buffer_response[2] = 0 ;  // id protocollo byte hight (sempre a 0)
    buffer_response[3] = 0 ;  // id protocollo byte low (sempre a 0)
    buffer_response[4] = 0 ;  //numero di byte inviati byte hight
    buffer_response[5] = 0 ;  //numero di byte inviati byte low ( numero a byte, se invio un Holding Register a 16 bit invierò 2 byte )
    buffer_response[6] = 0 ;  // indirizzo dello slave
    buffer_response[7] = 0 ;  // Funzione modbus : 01 Read Coil (0xxxx), 03 Read Holding Registers (4xxxx), 04 Read Input Registers (3xxxx), 05 Force Single Coil (0xxxx), 06 Preset Single Register (4xxxx) , 15 Force multiple coils (0xxxx), 16 Force multiple register (4xxxx)
    buffer_response[8] = 0 ;  //inizio dati primo byte hight
    buffer_response[9] = 0 ;  //inizio dati primo byte low
    buffer_response[10] = 0 ;  //secondo dato bite hight
    buffer_response[11] = 0 ;  //secondo dato bite low
    */
__fastcall TDataModuleModBus::TDataModuleModBus(TComponent* Owner)
    : TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDataModuleModBus::DataModuleCreate(TObject *Sender)
{
/*	IdModBusClient1->Host = "10.100.192.69";
	IdModBusClient1->Port = 503;
	try {
		IdModBusClient1->Connect();
	}catch(...){}
	num = 0 ;
*/
}
//---------------------------------------------------------------------------
int  TDataModuleModBus::ReturnNBitFromInteger(int value)
{
//ritorno il n° bit dall'intero
	int i,resvalue = -1 ;
	for (i = 0 ; i<= 7 ; i++) {
		if (MainForm->bit[i] & value ) {
			resvalue = i ;
			break ;

        }
    }
	return resvalue ;
}





void __fastcall TDataModuleModBus::TimerTxTimer(TObject *Sender)
{
	TimerTx->Enabled = false ;
	InviaTelegramma();
}
//---------------------------------------------------------------------------
void  TDataModuleModBus::InviaTelegramma()
{
/*
	int i,nbyte,id_richiedente,tipo_richiesta,indirizzo_registro_iniziale,n_registri_da_leggere,indice_dati,nbytesend ,id_registro,valore,byteletti;
	int nbit,nadd,value;
	div_t x ;
	if (ClientSocket1->Socket->Connected) {
			//lettura da parte dello slave(pannellino) del valore di alcuni registri (Holding Register)
		indirizzo_registro_iniziale = 1 ;
		n_registri_da_leggere = 1;
		buffer_response[0] = 0 ;  // id transazione byte hight (sempre a 0)
		buffer_response[1] = 0 ;  // id transazione byte low (sempre a 0)
		buffer_response[2] = 0 ;  // id protocollo byte hight (sempre a 0)
		buffer_response[3] = 0 ;  // id protocollo byte low (sempre a 0)
		buffer_response[4] = 0 ;  //numero di byte del telegramma inviati byte hight
		nbytesend = (n_registri_da_leggere * 2 )+3 ;
		//sprintf(command,"%2X",nbytesend) ; //converto in hex
		buffer_response[5] = nbytesend;  //numero di byte inviati byte low ( numero a byte, se invio un Holding Register a 16 bit invierò 2 byte )
		buffer_response[6] = 1 ;  // indirizzo dello slave
		buffer_response[7] = 3 ;  // Funzione modbus : 01 Read Coil (0xxxx), 03 Read Holding Registers (4xxxx), 04 Read Input Registers (3xxxx), 05 Force Single Coil (0xxxx), 06 Preset Single Register (4xxxx) , 15 Force multiple coils (0xxxx), 16 Force multiple register (4xxxx)

		buffer_response[8] = (n_registri_da_leggere * 2) ;  // numero di byte dei registri letti inviati
		indice_dati = 11 ;
		buffer_response[9] = 0 ;
		buffer_response[10] = 0 ;
		ClientSocket1->Socket->SendBuf(buffer_response,indice_dati ) ;
	}
*/
	unsigned int word ,i;
	WORD word2 ,test[10];
	int ok ;
	if (IdModBusClient1->Connected()) {
		ok = IdModBusClient1->ReadHoldingRegisters(1,10,test,1);
	//	word2 =  random(10) ;
		if (num < 255)
			num++ ;
		else
			num = 0 ;
		test[2] = num ;
		ok = IdModBusClient1->WriteRegisters(1, test, 1);
/*		MainForm->ListBox1->Items->Clear();
		for (i = 0 ;i<10;i++)
			MainForm->ListBox1->Items->Append(test[i]);   */
	}
	TimerTx->Enabled = true ;

}

void __fastcall TDataModuleModBus::IdModBusClient1Connected(TObject *Sender)
{
	int connesso = 1 ;
	TimerTx->Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TDataModuleModBus::IdModBusClient1ResponseError(const BYTE FunctionCode,
          const BYTE ErrorCode, const TModBusResponseBuffer &ResponseBuffer)

{
	int error= 1 ;
}
//---------------------------------------------------------------------------

void __fastcall TDataModuleModBus::IdModBusClient1ResponseMismatch(const BYTE RequestFunctionCode,
          const BYTE ResponseFunctionCode, const TModBusResponseBuffer &ResponseBuffer)

{
	int mismatch= 1 ;

}
//---------------------------------------------------------------------------

void __fastcall TDataModuleModBus::IdModBusClient1Status(TObject *ASender, const TIdStatus AStatus,
          const UnicodeString AStatusText)
{
int status = 1 ;
}
//---------------------------------------------------------------------------





