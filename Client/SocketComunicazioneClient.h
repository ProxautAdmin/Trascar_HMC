// ---------------------------------------------------------------------------

#ifndef SocketComunicazioneClientH
#define SocketComunicazioneClientH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
#include <ExtCtrls.hpp>

// ---------------------------------------------------------------------------
class TSocketDataModule : public TDataModule
{
__published: // IDE-managed Components

    TClientSocket *ClientSocket;
    TTimer *TimerConnected;
    TTimer *TimerTimeout;

    void __fastcall DataModuleDestroy(TObject *Sender);
    void __fastcall TimerConnectedTimer(TObject *Sender);
    void __fastcall ClientSocketError(TObject *Sender,
        TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
        int &ErrorCode);
    void __fastcall DataModuleCreate(TObject *Sender);
    void __fastcall TimerTimeoutTimer(TObject *Sender);
    void __fastcall ClientSocketRead(TObject *Sender,
        TCustomWinSocket *Socket);
    void __fastcall ClientSocketDisconnect(TObject *Sender, TCustomWinSocket *Socket);

private: // User declarations
        public : // User declarations
    __fastcall TSocketDataModule(TComponent* Owner);
    void Connetti();
    void Disconnetti();
    bool SeiConnesso();
    void Invia(AnsiString telegramma);
    void InviaMissioneClient(int pick, int drop, int tm, int agv);
    void InviaStandByClient(int agv, int stand_by);
    void ScriviPhoenixClient(int modulo, int addr, int bit, int value);
    void SimulaInputPhoenix(int modulo, int addr, int bit, int value);
    void ScriviBytePlc(int plc, int db, int dw, int value);
    void ScriviWordPlc(int plc, int db, int dw, int value);
    void ScriviDWordPlc(int plc, int db, int dw, int value);
    void ScriviBufferPlc(int plc, int db, int dw, int numbyte, AnsiString buffer);
    void InviaSimulazioneInputPlc(int pos_plc,int byte, int value, int idx=0);
	void InviaUscitePlc(int pos_plc,int byte, int value, int idx=0) ;
	void InviaRichiestaSimulazione(int simula);
    void InviaStatoPorta(int porta, int stato);
    void InviaRichiestaStart(int start);
    void InviaAggiornamentoTabella(int idtabella);
    void InviaAbortAll(int resetprenotazioni);
    void ScriviStringaHexPlc(int plc, int db, int dw, int numbyte, AnsiString stringa_hex);
    void InviaAggiornamentoParametri(int idparametro);
    void InviaAggiornamentoProduzione();
    void InviaIdUdcAgv(int idudc, int agv);
    void EscludiAgvDaSupervisione(int idagv);
    void InviaStatoPortaFumo(int porta, int stato)    ;
    void InviaSimulazioneInputPorta(int IDPorta,int byte, int value)  ;
	void InviaUscitePorta(int idporta,int byte, int value) ;
	void InviaSimulazioneInputIdUdc(int pos_plc, int value) ;
	void InviaUscitePortaFumo(int idporta,int byte, int value) ;
    void InviaAggiornamentoCB(int idparametro);
    void InviaScritturaModBus(int IDPLPC,int reg, int value) ;
	void SimulaRegistroModBus(int IDPLPC,int reg, int value) ;
	void MissionePrelievoManuale(int buttontag);
    void InviaAggiornamentoPLC() ;
    void ResettaPLC() ;
    void InviaLeggiStatoAGV() ;
    void InviaRichiestaGiorno(int start) ;

    bool connected;
    int readbytes, prima_connessione;
};

// ---------------------------------------------------------------------------
extern PACKAGE TSocketDataModule *SocketDataModule;
// ---------------------------------------------------------------------------
#endif
