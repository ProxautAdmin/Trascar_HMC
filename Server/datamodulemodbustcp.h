//---------------------------------------------------------------------------

#ifndef datamodulemodbustcpH
#define datamodulemodbustcpH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "IdModBusClient.hpp"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include "ModbusTypes.hpp"
//---------------------------------------------------------------------------
class TDataModuleModBus : public TDataModule
{
__published:	// IDE-managed Components
	TTimer *TimerTx;
	TIdModBusClient *IdModBusClient1;
    void __fastcall DataModuleCreate(TObject *Sender);
	void __fastcall TimerTxTimer(TObject *Sender);
	void __fastcall IdModBusClient1Connected(TObject *Sender);
	void __fastcall IdModBusClient1ResponseError(const BYTE FunctionCode, const BYTE ErrorCode,
          const TModBusResponseBuffer &ResponseBuffer);
	void __fastcall IdModBusClient1ResponseMismatch(const BYTE RequestFunctionCode,
          const BYTE ResponseFunctionCode, const TModBusResponseBuffer &ResponseBuffer);
	void __fastcall IdModBusClient1Status(TObject *ASender, const TIdStatus AStatus,
          const UnicodeString AStatusText);




private:	// User declarations
public:		// User declarations
    __fastcall TDataModuleModBus(TComponent* Owner);
	int  ReturnNBitFromInteger(int value) ;
	void  InviaTelegramma() ;
	unsigned char buffer[200];
	unsigned char buffer_response[200] ;
	int num ;
};
//---------------------------------------------------------------------------
extern PACKAGE TDataModuleModBus *DataModuleModBus;
//---------------------------------------------------------------------------
#endif
