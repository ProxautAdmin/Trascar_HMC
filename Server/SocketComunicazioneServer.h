// ---------------------------------------------------------------------------

#ifndef SocketComunicazioneServerH
#define SocketComunicazioneServerH
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

    TServerSocket *ServerSocket1;
    TTimer *TimerSendClientData;

    void __fastcall ServerSocket1ClientRead(TObject *Sender, TCustomWinSocket *Socket);
    void __fastcall ServerSocket1ClientError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode);
    void __fastcall DataModuleCreate(TObject *Sender);
    void __fastcall TimerSendClientDataTimer(TObject *Sender);
    void __fastcall DataModuleDestroy(TObject *Sender);

private: // User declarations
    AnsiString Parser(AnsiString NomeCampo, AnsiString Telegramma, AnsiString Separatore);

public: // User declarations
    __fastcall TSocketDataModule(TComponent* Owner);
    void UpdateClientData();
};

// ---------------------------------------------------------------------------
extern PACKAGE TSocketDataModule *SocketDataModule;
// ---------------------------------------------------------------------------
#endif
