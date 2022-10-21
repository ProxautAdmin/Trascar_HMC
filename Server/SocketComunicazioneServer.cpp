// ---------------------------------------------------------------------------
#pragma hdrstop

#include <vcl.h>
#include <stdio.h>

#include <time.h>
#include <inifiles.hpp>
#include "SocketComunicazioneServer.h"
#include "DB.h"
#include "Main.h"
#include "LogTxt.h"
#include "ClientData.h"
#include "PLCThread.h"
#include "PLCPhoenixThread.h"
#include "datamodulecomandiagv.h"
#include "dmgestione.h"
#include "dmgestioneplc.h"
#include "dmFunzioniComuniClientServer.h"
#include "dmgestionedatabase.h"
#include "DBImpianto.h"
#include "dmgestioneporte.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSocketDataModule *SocketDataModule;

// ---------------------------------------------------------------------------
__fastcall TSocketDataModule::TSocketDataModule(TComponent* Owner)
    : TDataModule(Owner)
{
}
// ---------------------------------------------------------------------------

typedef struct {
    unsigned short l_onoff;
    unsigned short l_linger;
} TMyLinger;

void __fastcall TSocketDataModule::DataModuleCreate(TObject *Sender)
{
    TMyLinger l;
    TIniFile *Ini;

    l.l_onoff = 0;
    l.l_linger = 0;
    Ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    ServerSocket1->Port = Ini->ReadInteger("Socket", "PortNumber", 11000);
    delete Ini;
    ServerSocket1->Open();
    setsockopt((SOCKET)ServerSocket1->Socket->SocketHandle, SOL_SOCKET, SO_LINGER, (char*)&l, 4);
    TimerSendClientData->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TSocketDataModule::ServerSocket1ClientError(
    TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
    int &ErrorCode)
{
    ErrorCode = 0;
}
// ---------------------------------------------------------------------------

void __fastcall TSocketDataModule::TimerSendClientDataTimer(
    TObject *Sender)
{
    int i, nagv;

    TimerSendClientData->Enabled = false;
    if (MainForm->count_inizio > 20) {
        UpdateClientData();
        strcpy(ClientData.Sync, "SYNC");
        // aggiorno i database qua così copo invio di aggiornare ai client
        if (dmDB->aggiorna_tab_posizioni_locale) {
            dmDB->AggiornaTabPostazioni();
            dmDBServer->AggiornaStrutturaPLC();
        }
        if (dmDB->aggiorna_tab_porte_locale) {
            dmDB->AggiornaTabPorte();
        }
        if (dmDB->aggiorna_tab_tipologia_locale) {
            // dmDB->AggiornaTabTipologia();
            // dmDB->AggiornaTabAnagrafica() ;
        }
        if (dmDB->aggiorna_tab_parametri) {
            dmDB->AggiornaTabParametri();
        }

        ClientData.aggiorna_tab_posizioni = dmDB->aggiorna_tab_posizioni_locale;
        ClientData.aggiorna_tab_porte = dmDB->aggiorna_tab_porte_locale;
        ClientData.aggiorna_tab_tipologia = dmDB->aggiorna_tab_tipologia_locale;
        ClientData.aggiorna_tab_parametri = dmDB->aggiorna_tab_parametri;
        for (i = 0; i < ServerSocket1->Socket->ActiveConnections; i++) {
            ServerSocket1->Socket->Connections[i]->SendBuf(&ClientData, sizeof(ClientData));
        }
        if (ServerSocket1->Socket->ActiveConnections) {
            dmDB->aggiorna_tab_posizioni_locale = 0; // azzero eventuali agigonramenti client
            dmDB->aggiorna_tab_porte_locale = 0; // azzero eventuali agigonramenti client
            dmDB->aggiorna_tab_tipologia_locale = 0; // azzero eventuali agigonramenti client
            dmDB->aggiorna_tab_parametri = 0;
            ClientData.ridisegna_mappa = 0;
            ClientData.aggiorna_allarmi = 0;
            for (nagv = 1; nagv <= NAGV; nagv++)
                ClientData.DatiAgv[nagv].disegna_agv = 0;
        }
    }
    TimerSendClientData->Enabled = true;
}
// ---------------------------------------------------------------------------

void TSocketDataModule::UpdateClientData()
{
    int i;
    // ClientData.ModBusIn.watchdog = ++ClientData.ModBusIn.watchdog % 1000;
    // ClientData.ModBusOut.watchdog = ++ClientData.ModBusOut.watchdog % 1000;

    // la struttura dati plc vemac e la struttura dati plcphoenix è direttamente aggiornata dal server
    // quindi non devo assegnarla a nessuno, in clientdata.PlcVemacArelleUscita ecc ho i dati del plc aggiornati
    /*
     memcpy(ClientData.Plc[0].DB[0], PLCThread[0]->DB[84], sizeof(ClientData.Plc[0].DB[0])); //passo solo da bd 84 e la db 85 che sono le uniche che uso
     memcpy(ClientData.Plc[1].DB[0], PLCThread[1]->DB[84], sizeof(ClientData.Plc[1].DB[0])); //passo solo da bd 84 e la db 85 che sono le uniche che uso
     memcpy(ClientData.Plc[2].DB[0], PLCThread[2]->DB[84], sizeof(ClientData.Plc[2].DB[0])); //passo solo da bd 84 e la db 85 che sono le uniche che uso
     */

    /*
     for(i=1 ; i<10; i++) {
     ClientData.DatiPhoenix[0].input[i] = MainForm->DatiPhoenix[1].input[i];
     ClientData.DatiPhoenix[0].output[i] = MainForm->DatiPhoenix[1].output[i];
     }
     */

}
// ---------------------------------------------------------------------------

AnsiString TSocketDataModule::Parser(AnsiString NomeCampo, AnsiString Telegramma, AnsiString Separatore)
{
    int inizio, fine, cl;
    AnsiString tg, res;

    res = "";
    tg = Telegramma;
    inizio = tg.Pos(NomeCampo);
    if (inizio > 0) {
        cl = NomeCampo.Length();
        tg.Delete(1, inizio + cl - 1);
        fine = tg.Pos(Separatore);
        if (fine <= 0) {
            fine = tg.Length();
        }
        if (fine > 0) {
            res = tg.SubString(1, fine - 1);
        }
    }
    return res;
}
// ---------------------------------------------------------------------------

void __fastcall TSocketDataModule::ServerSocket1ClientRead(TObject *Sender,
    TCustomWinSocket *Socket)
{
    AnsiString Answer, s, MsgType, buffer;
    int n, val, plc, db, dw, bit, addr, value, idporta, agv, pos_plc, i, byte, reg, idx;
    TMissione m;
    unsigned char dati_codice_articolo[20];
    unsigned char dati_udc[STRUTTURA_DATI_UDC_PLC];
    Answer = Socket->ReceiveText();
    LogTxt(Answer.c_str());
    MsgType = Parser("MSGTYPE=", Answer, "|");
    do {
        /* if (MsgType == "PUSH") {
         s = Parser("AGV=", Answer, "|");
         n = s.ToIntDef(1);
         s = Parser("DEST=", Answer, "|");
         val = s.ToIntDef(0);
         dmTrafficManager->GeneraMissione(n, val, "PH", 0);
         } else if (MsgType == "PULSANTE") {
         s = Parser("SELLA=", Answer, "|");
         dmEvents->GestionePulsantePremuto(dmDB->IndiceSella(s));
         } else if (MsgType == "LEGGIPARAMS") {
         dmDB->LeggiParametri();
         } else if (MsgType == "LUCEPULSANTE") {
         s = Parser("SELLA=", Answer, "|");
         dmEvents->AccendiLucePulsante(dmDB->IndiceSella(s));
         } else */
        if (MsgType == "SETBIT") {
            s = Parser("PLC=", Answer, "|");
            plc = s.ToIntDef(0);
            s = Parser("DB=", Answer, "|");
            db = s.ToIntDef(0);
            s = Parser("DW=", Answer, "|");
            dw = s.ToIntDef(0);
            s = Parser("BIT=", Answer, "|");
            bit = s.ToIntDef(0);
            // ???     PLCThread[plc]->SetBit(db, dw, bit);
        }
        else if (MsgType == "RESETBIT") {
            s = Parser("PLC=", Answer, "|");
            plc = s.ToIntDef(0);
            s = Parser("DB=", Answer, "|");
            db = s.ToIntDef(0);
            s = Parser("DW=", Answer, "|");
            dw = s.ToIntDef(0);
            s = Parser("BIT=", Answer, "|");
            bit = s.ToIntDef(0);
            // ???      PLCThread[plc]->ResetBit(db, dw, bit);
        }
        else if (MsgType == "WRITEBYTE") {
            s = Parser("PLC=", Answer, "|");
            plc = s.ToIntDef(0);
            s = Parser("DB=", Answer, "|");
            db = s.ToIntDef(0);
            s = Parser("DW=", Answer, "|");
            dw = s.ToIntDef(0);
            s = Parser("BYTE=", Answer, "|");
            value = s.ToIntDef(0);
            // ???     PLCThread[plc]->WriteByte(db, dw, value, ClientData.Simula);
            dmDB->LogMsg("Ricevuta Richiesta forzatura byte ( Plc " + IntToStr(plc) + " , DB : " + IntToStr(db) + " , dw " + IntToStr(dw) + " , value : " + IntToStr(value));
        }
        else if (MsgType == "WRITEWORD") {
            s = Parser("PLC=", Answer, "|");
            plc = s.ToIntDef(0);
            s = Parser("DB=", Answer, "|");
            db = s.ToIntDef(0);
            s = Parser("DW=", Answer, "|");
            dw = s.ToIntDef(0);
            s = Parser("WORD=", Answer, "|");
            value = s.ToIntDef(0);
            // ??????      PLCThread[plc]->WriteWord(db, dw, value, ClientData.Simula);
            dmDB->LogMsg("Ricevuta Richiesta forzatura word ( Plc " + IntToStr(plc) + " , DB : " + IntToStr(db) + " , dw " + IntToStr(dw) + " , value : " + IntToStr(value));
        }
        else if (MsgType == "WRITEDWORD") {
            s = Parser("PLC=", Answer, "|");
            plc = s.ToIntDef(0);
            s = Parser("DB=", Answer, "|");
            db = s.ToIntDef(0);
            s = Parser("DW=", Answer, "|");
            dw = s.ToIntDef(0);
            s = Parser("WORD=", Answer, "|");
            value = s.ToIntDef(0);
            // ?????      PLCThread[plc]->WriteDWord(db, dw, value, ClientData.Simula);
            dmDB->LogMsg("Ricevuta Richiesta forzatura doppia word ( Plc " + IntToStr(plc) + " , DB : " + IntToStr(db) + " , dw " + IntToStr(dw) + " , value : " + IntToStr(value));
        }
        else if (MsgType == "WRITEBUFFER") {
            s = Parser("PLC=", Answer, "|");
            plc = s.ToIntDef(0);
            s = Parser("DB=", Answer, "|");
            db = s.ToIntDef(0);
            s = Parser("DW=", Answer, "|");
            dw = s.ToIntDef(0);
            s = Parser("N=", Answer, "|");
            n = s.ToIntDef(0);
            s = Parser("BUFFER=", Answer, "|");
            buffer = s;
            // ?????     PLCThread[plc]->WriteBuffer(db, dw, n, buffer.c_str(), ClientData.Simula);
            dmDB->LogMsg("Ricevuta Richiesta forzatura Buffer ( Plc " + IntToStr(plc) + " , DB : " + IntToStr(db) + " , dw " + IntToStr(dw) + " , nbyte : " + IntToStr(n) + " VALUE :" + buffer);
        }
        else if (MsgType == "WRITESTRINGHEX") {
            s = Parser("PLC=", Answer, "|");
            plc = s.ToIntDef(0);
            s = Parser("DB=", Answer, "|");
            db = s.ToIntDef(0);
            s = Parser("DW=", Answer, "|");
            dw = s.ToIntDef(0);
            s = Parser("N=", Answer, "|");
            n = s.ToIntDef(0);
            s = Parser("STRINGAHEX=", Answer, "|");
            buffer = s;
            dmFunzioni->DaEsadecimaleAChar(buffer, dati_udc);
            // ??????        PLCThread[plc]->WriteBuffer(db, dw, n, dati_udc, ClientData.Simula);
            dmDB->LogMsg("Ricevuta Richiesta forzatura Buffer ( Plc " + IntToStr(plc) + " , DB : " + IntToStr(db) + " , dw " + IntToStr(dw) + " , nbyte : " + IntToStr(n) + " VALUE :" + buffer);
        }
        else if (MsgType == "SETRESETBITPHOENIX") {
            s = Parser("MODULO=", Answer, "|");
            plc = s.ToIntDef(0);
            s = Parser("ADDR=", Answer, "|");
            addr = s.ToIntDef(1);
            s = Parser("BIT=", Answer, "|");
            bit = s.ToIntDef(0);
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            // ClientData.DatiPhoenix[0].forzatura_attiva = 1 ;
            PLCPhoenixThread[plc]->ScriviPhoenix(bit, addr, value);
            dmDB->LogMsg("Ricevuta Richiesta forzatura output Plc Phoenix ( ADDR " + IntToStr(addr) + " , BIT " + IntToStr(bit) + " , Value : " + IntToStr(value) + " )");
        }
        else if (MsgType == "SIMULAINPUTPHOENIX") {
            s = Parser("MODULO=", Answer, "|");
            plc = s.ToIntDef(0);
            s = Parser("ADDR=", Answer, "|");
            addr = s.ToIntDef(1);
            s = Parser("BIT=", Answer, "|");
            bit = s.ToIntDef(0);
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            if ((ClientData.ParametriFunzionali.Simula) && (bit >= 0) && (bit <= 7)) {
                if (value)
                    PLCPhoenixThread[plc]->IngressiPhoenix[addr] |= dmDB->bit[bit];
                else
                    PLCPhoenixThread[plc]->IngressiPhoenix[addr] &= dmDB->bitAnd[bit];
                dmDB->LogMsg("Ricevuta Richiesta forzatura input Plc Phoenix ( ID " + IntToStr(plc) + " , ADDR : " + IntToStr(addr) + " , Bit " + IntToStr(bit) + " , Value : " + IntToStr(value));
            }
        }
        else if (MsgType == "SETSIMULAINPUTUDC") {
            s = Parser("POSPLC=", Answer, "|");
            pos_plc = s.ToIntDef(0);
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            dmDB->LogMsg("Ricevuta Richiesta forzatura id Articolo Udc su PosPlc " + IntToStr(pos_plc) + " , Value : " + IntToStr(value));
            // DMPlcTGW->ModificaByteStrutturaPlc(1, pos_plc, 2, value);
        }
        else if (MsgType == "SETSIMULAINPUTPLC") {
            s = Parser("POSPLC=", Answer, "|");
            pos_plc = s.ToIntDef(0);
            s = Parser("BYTE=", Answer, "|");
            byte = s.ToIntDef(0);
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            s = Parser("IDX=", Answer, "|");
            idx = s.ToIntDef(0);
            dmDB->LogMsg("Ricevuta Richiesta forzatura input Plc ,PosPlc " + IntToStr(pos_plc) + ",Byte " + IntToStr(byte) + " , Value : " + IntToStr(value) + " , IDX : " + IntToStr(idx));
            DMPlc->ModificaByteStrutturaPlc(1, pos_plc, byte, value, idx);
        }
        else if (MsgType == "SETSIMULAINPUTPORTA") { // !! RIMETTERE SE CI SONO PORTE
            s = Parser("IDPORTA=", Answer, "|");
            pos_plc = s.ToIntDef(0);
            s = Parser("BYTE=", Answer, "|");
            byte = s.ToIntDef(0);
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            dmDB->LogMsg("Ricevuta Richiesta forzatura input Porta ,IdPorta " + IntToStr(pos_plc) + " ) Value : " + IntToStr(value));
            DMGestione_Porte->ModificaByteStrutturaPorte(1, pos_plc, byte, value);
        }
        else if (MsgType == "SETUSCITEPLC") {
            s = Parser("POSPLC=", Answer, "|");
            pos_plc = s.ToIntDef(0);
            s = Parser("BYTE=", Answer, "|");
            byte = s.ToIntDef(0);
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            s = Parser("IDX=", Answer, "|");
            idx = s.ToIntDef(0);
            dmDB->LogMsg("Ricevuta Richiesta forzatura uscitedascrivere Plc ,PosPlc " + IntToStr(pos_plc) + " ) Value : " + IntToStr(value));
            DMPlc->ModificaByteStrutturaPlc(0, pos_plc, byte, value, idx);
        }
        else if (MsgType == "SETUSCITEPORTE") { // !! RIMETTERE SE CI SONO PORTE
            s = Parser("IDPORTA=", Answer, "|");
            pos_plc = s.ToIntDef(0);
            s = Parser("BYTE=", Answer, "|");
            byte = s.ToIntDef(0);
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            dmDB->LogMsg("Ricevuta Richiesta forzatura Output Porta ,IdPorta " + IntToStr(pos_plc) + " ) Value : " + IntToStr(value));
            DMGestione_Porte->ModificaByteStrutturaPorte(0, pos_plc, byte, value);
        }
        else if (MsgType == "REFRESHPORTE") {
            dmDB->aggiorna_tab_porte_locale = 1;
        }
        else if (MsgType == "REFRESHPOS") {
            dmDB->aggiorna_tab_posizioni_locale = 1;
        }
        else if (MsgType == "SIMULA") {
            // vedere se fare un tsto sirettamente sul server
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            ClientData.ParametriFunzionali.Simula = value;
        }
        else if (MsgType == "ABORTALL") {
            // vedere se fare un tsto sirettamente sul server
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            dmComandiAgv->InviaAbortAll();
            dmDB->aggiorna_tab_posizioni_locale = 1;
        }
        else if (MsgType == "START") {
            // vedere se fare un tsto sirettamente sul server
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            ClientData.ParametriFunzionali.AbilitaMissioni = value;
            dmDB->LogMsg("Ricevuto Richiesta di Start/Stop , Value : " + IntToStr(value));
        }
        else if (MsgType == "AGGIORNATAB") {
            // vedere se fare un tsto sirettamente sul server
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            if ((value == 1) || (value == 4))
                dmDB->aggiorna_tab_posizioni_locale = 1;
            if (value == 2)
                dmDB->aggiorna_tab_porte_locale = 1;
            if (value == 3)
                dmDB->aggiorna_tab_tipologia_locale = 1;
            ClientData.ridisegna_mappa = 1; // in ogni caso ridisegno la mappa nei client
        }
        else if (MsgType == "STATOPORTA") {
            // vedere se fare un tsto sirettamente sul server
            s = Parser("PORTA=", Answer, "|");
            idporta = s.ToIntDef(0);
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            dmDBServer->UpdateStatoPorta(idporta, value);
        }
        else if (MsgType == "STANDBY") {
            // vedere se fare un tsto sirettamente sul server
            s = Parser("AGV=", Answer, "|");
            agv = s.ToIntDef(0);
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            dmComandiAgv->StandByAgv(agv, value);
        }
        else if (MsgType == "AGGIORNAUDCAGV") { // !!! UTILE IN CASO DI PERDITA DI UDC ???
            // vedere se fare un tsto sirettamente sul server
            s = Parser("AGV=", Answer, "|");
            agv = s.ToIntDef(0);
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            if ((agv > 0) && (agv <= NAGV)) {
                ClientData.DatiAgv[agv].DatiUDC.IDUDC = value;
                // dmDB->LeggiStrutturaUdc(ClientData.DatiAgv[agv].DatiUDC);
                dmDB->LogMsg("Ricevuta Richiesta forzatura IdUDc " + IntToStr(value) + "su Agv " + IntToStr(agv));
            }
        }
        else if (MsgType == "ESCLUDIAGV") {
            // vedere se fare un tsto sirettamente sul server
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            dmComandiAgv->EscludiAgv(value);
        }
        else if (MsgType == "NEWMIS") {
            s = Parser("PICK=", Answer, "|");
            m.posprel = s.ToIntDef(0);
            s = Parser("DROP=", Answer, "|");
            m.posdep = s.ToIntDef(1);
            s = Parser("TM=", Answer, "|");
            m.tipo_mis = s.ToIntDef(0);
            s = Parser("AGV=", Answer, "|");
            agv = s.ToIntDef(0);
            dmDB->LogMsg("Ricevuta Richiesta nuova missione : Prel : " + IntToStr(m.posprel) + " , dep " + IntToStr(m.posdep));
            if ((m.tipo_mis != 0) || (dmDB->ControllaMissioneOk(1, m.posprel, m.posdep))) {
                if ((m.posprel > 0) && (m.posdep > 0)) {
                    m.piano_prel = 0;
                    m.h_prel = 0;
                    m.piano_dep = 0;
                    m.h_dep = 0;
                    m.priorita = 9;
                    m.idcentromissioni = 0;
                    m.tipologia_mis = m.tipo_mis;
                    m.tipologia_mov = m.tipo_mis;
                    m.idgestionale=0;
                    m.nodopassaggiodep=0;
                    m.nodopassaggioprel=0;
                    m.latoforchedep=0;
                    m.latoforcheprel=0;
                    m.idudc = ClientData.DatiAgv[agv].DatiUDC.IDUDC;
                    dmDBServer->GeneraMissione(m);
                }
            }
        }
        else if (MsgType == "AGGIORNAPARAMETRI") {
            // vedere se fare un tsto sirettamente sul server
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            dmDB->LeggiParametri(0); // leggo tutti i parametri, invio stato cb ad agvx
        }
        else if (MsgType == "AGGIORNACB") {
            // vedere se fare un tsto sirettamente sul server
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            dmComandiAgv->ModificaCaricaBatterieAgvX();
        }
        else if (MsgType == "AGGIORNAPRODUZIONE") {
            // inviare ricetta produzione a plc vemac
            // DMGestioneEventi->InviaScritturaProduzioneSuPlc() ;
            dmDB->LogMsg("Ricevuta Richiesta invio Aggiornamento Produzione su Plc ");

        }
        else if (MsgType == "MISSPRELMAN") {
            // Missioni manualiprelievo
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            dmDBServer->MissionePrelievoManuale(value);
        }
        else if (MsgType == "SCRIVIREGISTROMODBUS") {
            s = Parser("POSPLC=", Answer, "|");
            pos_plc = s.ToIntDef(0);
            s = Parser("REG=", Answer, "|");
            reg = s.ToIntDef(0);
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            // PLCThreadModbus[pos_plc]->ScriviRegistro(reg, value);
            dmDB->LogMsg("Scrivi ModBus su PosPlc " + IntToStr(pos_plc) + " , Registro : " + IntToStr(reg) + " , Value : " + IntToStr(value));

        }
        else if (MsgType == "SIMULAREGISTROMODBUS") {
            s = Parser("POSPLC=", Answer, "|");
            pos_plc = s.ToIntDef(0);
            s = Parser("REG=", Answer, "|");
            reg = s.ToIntDef(0);
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            // OK COME SIMULAZIONE IN LETTURA?????
            // PLCThreadModbus[pos_plc]->Register_Plc.Register[reg] = value;
            dmDB->LogMsg("Simula ModBus su PosPlc " + IntToStr(pos_plc) + " , Registro : " + IntToStr(reg) + " , Value : " + IntToStr(value));

        }
        else if (MsgType == "RESETTAPLC") {
            dmDBImpianto->ResettaPLC(); // resetta plc BIMBO
        }
        else if (MsgType == "AGGIORNAPOSPLC") {
            dmDBImpianto->SettaPLCInit();
        }
        else if (MsgType == "LEGGISTATOAGV") {
            dmDB->LeggiStatoAgv(); // leggo STATO AGV
        }
        else if (MsgType == "GIORNO") {
            // vedere se fare un tsto sirettamente sul server
            s = Parser("VAL=", Answer, "|");
            value = s.ToIntDef(0);
            dmDB->AggiornaParametri(57, value, "");
            dmDB->LeggiParametri(0);
            dmDB->LogMsg("Cambio stato Giorno , Value : " + IntToStr(value));
        }

        MsgType = Parser("|MSGTYPE=", Answer, "|");
        if (MsgType != "") {
            Answer = Answer.SubString(Answer.Pos("|MSGTYPE=") + 1, Answer.Length());
        }
    }
    while (MsgType != "");
}
// ---------------------------------------------------------------------------

void __fastcall TSocketDataModule::DataModuleDestroy(TObject *Sender)
{
    ServerSocket1->Close();
}
// ---------------------------------------------------------------------------
