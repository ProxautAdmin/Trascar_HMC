// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <inifiles.hpp>
#include "SocketComunicazioneClient.h"
#include "DB.h"
#include "Main.h"
#include "ClientData.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSocketDataModule *SocketDataModule;

// ---------------------------------------------------------------------------
__fastcall TSocketDataModule::TSocketDataModule(TComponent* Owner)
    : TDataModule(Owner)
{
    int Port;
    AnsiString ServerAddress;
    TIniFile *Ini;

    Ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    Port = Ini->ReadInteger("Socket", "PortNumber", 11000);
    if (Port != 0)
        ClientSocket->Port = Port;
    else
        Ini->WriteInteger("Socket", "PortNumber", ClientSocket->Port);
    ServerAddress = Ini->ReadString("Socket", "ServerAddress", "");
    if (ServerAddress != "")
        ClientSocket->Address = ServerAddress;
    else
        Ini->WriteString("Socket", "ServerAddress", ClientSocket->Address);
    connected = false;
    prima_connessione = true;
    delete Ini;
}

// ---------------------------------------------------------------------------
void TSocketDataModule::Connetti() {
    try {
        ClientSocket->Open();
        connected = true;
        readbytes = 0;
        TimerTimeout->Enabled = true;
    }
    catch (...) {
        Disconnetti();
        connected = false;
    }
}
// ---------------------------------------------------------------------------

void TSocketDataModule::Disconnetti() {
    ClientSocket->Close();
    connected = false;
    TimerTimeout->Enabled = false;
}
// ---------------------------------------------------------------------------

bool TSocketDataModule::SeiConnesso() {
    return ClientSocket->Active;
}
// ---------------------------------------------------------------------------

void __fastcall TSocketDataModule::DataModuleDestroy(TObject *Sender)
{
    Disconnetti();
}
// ---------------------------------------------------------------------------

void __fastcall TSocketDataModule::TimerConnectedTimer(TObject *Sender)
{
    TimerConnected->Enabled = false;
    // connected = SeiConnesso();
    if (!connected) {
        Connetti();
    }
    if (!TimerTimeout->Enabled)
        TimerTimeout->Enabled = true;
    TimerConnected->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TSocketDataModule::ClientSocketError(TObject *Sender,
    TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    Disconnetti();
    ErrorCode = 0;
}
// ---------------------------------------------------------------------------

void __fastcall TSocketDataModule::DataModuleCreate(TObject *Sender)
{
    Connetti();
    TimerConnected->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TSocketDataModule::TimerTimeoutTimer(TObject *Sender)
{
    Disconnetti();
}
// ---------------------------------------------------------------------------

void __fastcall TSocketDataModule::ClientSocketRead(TObject *Sender,
    TCustomWinSocket *Socket)
{
    int nletti, ncopiati, nresto, startpos, scartati, agv, ridisegna;
    TClientData buf;
    char *p1, *p2, *start, *t;
    bool extradata;

    connected = true;
    TimerTimeout->Enabled = false;
    try {
        nletti = Socket->ReceiveBuf(&buf, sizeof(TClientData));
        start = (char *)(&buf);
        // nletto sono i caratteri ricevuti nell'ultima lettura (questa che sto elaborando)
        // readbytes sono i caratteri letti fino a questo momento (anche in più letture precedenti)
        if (readbytes == 0) {
            // Se non ho ancora letto nulla in precedenza devo iniziare con un inizio pacchetto, identificabile da "SYNC"
            // Aspetto un inizio pacchetto, cerco l'ultimo in coda (se ne ho diversi all'interno dello stesso telegramma
            // tanto vale prendere l'ultimo che è il più aggiornato
            t = strstr((char *)(&buf), "SYNC");
            if (t != NULL) {
                // Se trovo un inizio pacchetto lo memorizzo in start
                start = t;
            }
            // Controllo se ci sono altri sync per prendere l'ultimo
            while (t != NULL) {
                t++; // inizio la ricerca dal carattere successivo
                t = strstr(t, "SYNC");
                if (t != NULL) {
                    start = t;
                }
            }
            // A questo punto in start mi trovo il puntatore all'inizio dell'ultimo pacchetto
            scartati = start - (char *)(&buf);
            nletti -= scartati;
        }
        if (((readbytes > 0) && (nletti > 4)) || ((nletti >= 4) && (start[0] == 'S') && (start[1] == 'Y') && (start[2] == 'N') && (start[3] == 'C'))) {
            p1 = start;
            p2 = (char*)&ClientData;
            p2 = p2 + readbytes;
            extradata = ((readbytes + nletti) > sizeof(ClientData));
            // se ho ricevuto più caratteri di quelli che mancano alla fine di un pacchetto
            // leggo solo quelli che servono
            if (extradata) {
                ncopiati = sizeof(ClientData) - readbytes;
                nresto = nletti - ncopiati;
            }
            else {
                ncopiati = nletti;
                nresto = 0;
            }
            memcpy(p2, p1, ncopiati);
            // Se ho completato un pacchetto faccio il refresh dati
            if ((readbytes + ncopiati) == sizeof(ClientData)) {
                readbytes = 0;
                // fMain->MainStatusBar->Refresh();
                if (ClientData.aggiorna_tab_posizioni) {
                    dmDB->AggiornaTabPostazioni();
                    ClientData.ridisegna_mappa = 1;
                }
                if (ClientData.aggiorna_tab_porte) {
                    dmDB->AggiornaTabPorte();
                }

                if (ClientData.aggiorna_tab_tipologia) {
                    dmDB->AggiornaTabTipoArticoli();
                    dmDB->AggiornaTabAnagrafica();
                    dmDB->AggiornaTabTipologiaCorsia();
                }
                if (ClientData.ridisegna_mappa) {
                    MainForm->AggiornaMappa();
                    // spostato qui per aggiornare i carrelli solo con l'aggiornamappa ...

                }
                for (agv = 1; agv <= NAGV; agv++) {
                    if (ClientData.DatiAgv[agv].disegna_agv) {
                        MainForm->DisegnaCarrello(agv);
                    }
                }
                if ((ClientData.aggiorna_allarmi) || (prima_connessione))
                    dmDB->ImpostaAllarmiAttiviClient();
                // ... oppure lasciare qui l'aggiornamento dei carrelli ma trovare un meccanismo che non li aggiorni sempre
                prima_connessione = 0;
            }
            else {
                readbytes = readbytes + nletti;
            }
            // Se restano dei caratteri me ne sbatto
        }
    }
    catch (...) {};
}

// ---------------------------------------------------------------------------
void TSocketDataModule::Invia(AnsiString telegramma)
{
    if (SeiConnesso()) {
        ClientSocket->Socket->SendText(telegramma);
        Sleep(250);
    }
}
// ---------------------------------------------------------------------------

void __fastcall TSocketDataModule::ClientSocketDisconnect(TObject *Sender, TCustomWinSocket *Socket)
{
    Disconnetti();
}
// ---------------------------------------------------------------------------

void TSocketDataModule::InviaMissioneClient(int pick, int drop, int tm, int agv) {
    AnsiString s;
    s.printf("MSGTYPE=NEWMIS|PICK=%d|DROP=%d|TM=%d|AGV=%d|", pick, drop, tm, agv);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::InviaStandByClient(int agv, int stand_by) {
    AnsiString s;
    s.printf("MSGTYPE=STANDBY|AGV=%d|VAL=%d|", agv, stand_by);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::ScriviPhoenixClient(int modulo, int addr, int bit, int value) {
    AnsiString s;
    s.printf("MSGTYPE=SETRESETBITPHOENIX|MODULO=%d|ADDR=%d|BIT=%d|VAL=%d|", modulo, addr, bit, value);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::SimulaInputPhoenix(int modulo, int addr, int bit, int value) {
    AnsiString s;
    s.printf("MSGTYPE=SIMULAINPUTPHOENIX|MODULO=%d|ADDR=%d|BIT=%d|VAL=%d|", modulo, addr, bit, value);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::InviaSimulazioneInputPlc(int pos_plc,int byte, int value, int idx) {
    AnsiString s;
	s.printf("MSGTYPE=SETSIMULAINPUTPLC|POSPLC=%d|BYTE=%d|VAL=%d|IDX=%d|", pos_plc,byte, value, idx);
    Invia(s);
    dmDB->LogMsg(s);
}
void TSocketDataModule::InviaSimulazioneInputIdUdc(int pos_plc, int value) {
    AnsiString s;
	s.printf("MSGTYPE=SETSIMULAINPUTUDC|POSPLC=%d|VAL=%d|", pos_plc, value);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::InviaSimulazioneInputPorta(int IDPorta,int byte, int value) {
    AnsiString s;
	s.printf("MSGTYPE=SETSIMULAINPUTPORTA|IDPORTA=%d|BYTE=%d|VAL=%d|", IDPorta,byte, value);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::InviaUscitePlc(int pos_plc,int byte, int value, int idx) {
    AnsiString s;
	s.printf("MSGTYPE=SETUSCITEPLC|POSPLC=%d|BYTE=%d|VAL=%d|IDX=%d|", pos_plc, byte, value, idx);
    Invia(s);
    dmDB->LogMsg(s);
}
void TSocketDataModule::InviaUscitePorta(int idporta,int byte, int value) {
    AnsiString s;
	s.printf("MSGTYPE=SETUSCITEPORTE|IDPORTA=%d|BYTE=%d|VAL=%d|", idporta, byte, value);
    Invia(s);
    dmDB->LogMsg(s);
}
void TSocketDataModule::InviaUscitePortaFumo(int idporta,int byte, int value) {
    AnsiString s;
	s.printf("MSGTYPE=SETUSCITEPORTAFUMO|IDPORTA=%d|BYTE=%d|VAL=%d|", idporta, byte, value);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::ScriviBytePlc(int plc, int db, int dw, int value) {
    AnsiString s;
    s.printf("MSGTYPE=WRITEBYTE|PLC=%d|DB=%d|DW=%d|BYTE=%d|", plc, db, dw, value);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::ScriviWordPlc(int plc, int db, int dw, int value) {
    AnsiString s;
    s.printf("MSGTYPE=WRITEWORD|PLC=%d|DB=%d|DW=%d|WORD=%d|", plc, db, dw, value);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::ScriviDWordPlc(int plc, int db, int dw, int value) {
    AnsiString s;
	s.printf("MSGTYPE=WRITEDWORD|PLC=%d|DB=%d|DW=%d|WORD=%d|", plc, db, dw, value);
    Invia(s);
}

void TSocketDataModule::ScriviBufferPlc(int plc, int db, int dw, int numbyte, AnsiString buffer) {
    AnsiString s;
    s.printf("MSGTYPE=WRITEBUFFER|PLC=%d|DB=%d|DW=%d|N=%d|BUFFER=%s|", plc, db, dw, numbyte, buffer.c_str());
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::ScriviStringaHexPlc(int plc, int db, int dw, int numbyte, AnsiString stringa_hex) {
    AnsiString s, logs[4];
    s.printf("MSGTYPE=WRITESTRINGHEX|PLC=%d|DB=%d|DW=%d|N=%d|STRINGAHEX=%s|", plc, db, dw, numbyte, stringa_hex.c_str());
    Invia(s);
    logs[0] = s.SubString(1, 100);
    logs[1] = s.SubString(101, 100);
    logs[2] = s.SubString(201, 100);
    logs[3] = s.SubString(301, 100);
    dmDB->LogMsg(logs[0] + " " + logs[1] + " " + logs[2] + " " + logs[3]);
}
void TSocketDataModule::InviaRichiestaSimulazione(int simula) {
	AnsiString s;
    s.printf("MSGTYPE=SIMULA|VAL=%d|", simula);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::InviaAbortAll(int resetprenotazioni) {
    AnsiString s;
    s.printf("MSGTYPE=ABORTALL|VAL=%d|", resetprenotazioni);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::InviaRichiestaStart(int start) {
    AnsiString s;
    s.printf("MSGTYPE=START|VAL=%d|", start);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::InviaStatoPortaFumo(int porta, int stato) {
    AnsiString s;
    s.printf("MSGTYPE=STATOPORTAFUMO|PORTA=%d|VAL=%d|", porta, stato);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::InviaStatoPorta(int porta, int stato) {
    AnsiString s;
    s.printf("MSGTYPE=STATOPORTA|PORTA=%d|VAL=%d|", porta, stato);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::InviaAggiornamentoTabella(int idtabella) {
    AnsiString s;
    s.printf("MSGTYPE=AGGIORNATAB|VAL=%d|", idtabella);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::InviaAggiornamentoParametri(int idparametro) {
    AnsiString s;
    s.printf("MSGTYPE=AGGIORNAPARAMETRI|VAL=%d|", idparametro);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::InviaAggiornamentoCB(int idparametro) {
    AnsiString s;
    s.printf("MSGTYPE=AGGIORNACB|VAL=%d|", idparametro);
    Invia(s);
    dmDB->LogMsg(s);
}


void TSocketDataModule::InviaAggiornamentoProduzione() {
    AnsiString s;
    s.printf("MSGTYPE=AGGIORNAPRODUZIONE|");
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::InviaIdUdcAgv(int idudc, int agv) {
    AnsiString s;
    s.printf("MSGTYPE=AGGIORNAUDCAGV|AGV=%d|VAL=%d|", agv, idudc);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::EscludiAgvDaSupervisione(int idagv) {
    AnsiString s;
    s.printf("MSGTYPE=ESCLUDIAGV|VAL=%d|", idagv);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::InviaScritturaModBus(int IDPLPC,int reg, int value) {
    AnsiString s;
	s.printf("MSGTYPE=SCRIVIREGISTROMODBUS|POSPLC=%d|REG=%d|VAL=%d|", IDPLPC,reg, value);
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::SimulaRegistroModBus(int IDPLPC,int reg, int value) {
    AnsiString s;
	s.printf("MSGTYPE=SIMULAREGISTROMODBUS|POSPLC=%d|REG=%d|VAL=%d|", IDPLPC,reg, value);
    Invia(s);
    dmDB->LogMsg(s);
}


void TSocketDataModule::MissionePrelievoManuale(int buttontag) {
	AnsiString s;
	s.printf("MSGTYPE=MISSPRELMAN|VAL=%d|", buttontag);
	Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::InviaAggiornamentoPLC() {
    AnsiString s;
    s.printf("MSGTYPE=AGGIORNAPOSPLC|");
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::ResettaPLC() {
    AnsiString s;
    s.printf("MSGTYPE=RESETTAPLC|");
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::InviaLeggiStatoAGV() {
    AnsiString s;
    s.printf("MSGTYPE=LEGGISTATOAGV|");
    Invia(s);
    dmDB->LogMsg(s);
}

void TSocketDataModule::InviaRichiestaGiorno(int start) {
    AnsiString s;
    s.printf("MSGTYPE=GIORNO|VAL=%d|", start);
    Invia(s);
    dmDB->LogMsg(s);
}





