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
    int PosPrel_FuoriIngombro[] = {611};
    int PosDep[] = {602, 603};
    int PosDep_FuoriIngombro[] = {612, 613};
    // inizializzo struttura
    for (int i = 1; i <= N_PLC; i++) {
        // num. plc
        ClientData.Plc[i].id_thread_plc = i - 1;
        // zona
        ClientData.Plc[i].Zona = i; // uso futuro?
        // postazioni
        ClientData.Plc[i].SegnaliGenerali[1].dbInput = 0;
        ClientData.Plc[i].SegnaliGenerali[1].dbOutput = 0;

        ClientData.Plc[i].SegnaliGenerali[1].byteOutput_accesso = 0;
        ClientData.Plc[i].SegnaliGenerali[1].bitOutput_conferma_accesso = 0;

        ClientData.Plc[i].SegnaliGenerali[1].byteInput_accesso = 0;
        ClientData.Plc[i].SegnaliGenerali[1].bitInput_conferma_accesso = 0;
        ClientData.Plc[i].SegnaliGenerali[1].bitInput_accesso_terminato = 0;

        for (int j = 1; j <= NUM_PRELIEVOPLC; j++) {
            for (int p = 1; p <= NUMPIANI_PREL; p++) {
                // PRELIEVO
                ClientData.Plc[i].Prelievo[j][p].idplc = IDX_PLCPRELIEVO;
                ClientData.Plc[i].Prelievo[j][p].dbInput = 0;
                ClientData.Plc[i].Prelievo[j][p].dbOutput = 0;
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
                ClientData.Plc[i].Prelievo[j][p].bit_ProntaAlPrelievo = 2;
                ClientData.Plc[i].Prelievo[j][p].bit_OkPrelievo = 3;
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
                ClientData.Plc[i].Deposito[j][p].dbInput = 0;
                ClientData.Plc[i].Deposito[j][p].dbOutput = 0;
                //
                ClientData.Plc[i].Deposito[j][p].pos = PosDep[j - 1];
                ClientData.Plc[i].Deposito[j][p].pos_fuori_ingombro = PosDep_FuoriIngombro[j - 1];
                // in
                ClientData.Plc[i].Deposito[j][p].byteInput = 0;
                ClientData.Plc[i].Deposito[j][p].bit_Ready = 0;
                ClientData.Plc[i].Deposito[j][p].bit_InAllarme = 1;
                ClientData.Plc[i].Deposito[j][p].bit_ProntaAlDeposito = 2;
                ClientData.Plc[i].Deposito[j][p].bit_OkDeposito = 3;
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
    // TimerCarScar->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TDMPlc::TimerEventiTimer(TObject *Sender) {
    int i, j, agv, k, anomalia_generatori;

    if ((MainForm->count_inizio < 19) || (MainForm->close_program))
        return;

    TimerEventi->Enabled = false;
    if ((PLCThread[0]->connected) || (ClientData.ParametriFunzionali.Simula))
    {
        if (dmDB->lettostatoagv) {
            for (i = 1; i <= N_PLC; i++)
            {
                for (j = 1; j <= NUM_PRELIEVOPLC; j++) {
                    for (int p = 1; p <= NUMPIANI_PREL; p++) {
                        if (!ClientData.Plc[i].forzature_attive) {
                            GestioneRichiestaIngressoBaiaPrelievo(ClientData.Plc[i].Prelievo[j][p]);
                            GestioneIngombroAgvPrelievo(ClientData.Plc[i].Prelievo[j][p]);
                            ResettaRichiestaIngressoBaiaPrelievo(ClientData.Plc[i].Prelievo[j][p]);
                        }
                    }
                }
                for (j = 1; j <= NUM_DEPOSITOPLC; j++) {
                    for (int p = 1; p <= NUMPIANI_DEP; p++) {
                        if (!ClientData.Plc[i].forzature_attive) {
                            GestioneRichiestaIngressoBaiaDeposito(ClientData.Plc[i].Deposito[j][p]);
                            GestioneIngombroAgvDeposito(ClientData.Plc[i].Deposito[j][p]);
                            ResettaRichiestaIngressoBaiaDeposito(ClientData.Plc[i].Deposito[j][p]);
                        }
                    }
                }
                if (!ClientData.Plc[i].forzature_attive) {
                    GestioneStandbyEmergenza(ClientData.Plc[i].SegnaliGenerali[1]);
                }
            }
            // }
        }
    }
    TimerEventi->Enabled = true;
}

// ---------------------------------------------------------------------------

void __fastcall TDMPlc::TimerCarScarTimer(TObject *Sender)
{
    /* if (dmDB->lettostatoagv) {
     if (ClientData.ParametriFunzionali.AbilitaMissioni) {
     for (int i = 1; i <= N_PLC; i++) {
     for (int j = 1; j <= NUM_PRELIEVOPLC; j++) {
     if (!ClientData.Plc[i].forzature_attive)
     ControlloStatoPrelievi(ClientData.Plc[i], ClientData.Plc[i].Prelievo[j]);
     }
     for (int j = 1; j <= NUM_DEPOSITOPLC; j++) {
     for (int p = 1; p <= NUMPIANI; p++) {
     if (!ClientData.Plc[i].forzature_attive)
     ControlloStatoDepositi(ClientData.Plc[i], ClientData.Plc[i].Deposito[j][p]);
     }
     }
     }
     }
     } */
}
// ---------------------------------------------------------------------------

void TDMPlc::CaricaStatoPosizioneInStruttura() {
    /*
     TIndexList TabPos;
     int ppos, indice;
     dmDB->FullTabellaK("select  pos, IDUDC, Prenotata, Disabilitata, Pos_Prenotata, Pos_Disabilita FROM Piani_View where tipoposizione=97 or tipoposizione=98 ORDER BY pos",
     "pos", TabPos);
     for (int i = 1; i <= NUM_ZONE; i++) {
     for (int j = 1; j <= ClientData.Zona[i].numpostazioni; j++) {
     ppos = ClientData.Zona[i].Postazione[j].pos;
     ClientData.Zona[i].Postazione[j].esclusa = TabPos[ppos]["DISABILITATA"].ToIntDef(0) | TabPos[ppos]["POS_DISABILITA"].ToIntDef(0);
     ClientData.Zona[i].Postazione[j].selezionato = TabPos[ppos]["PRENOTATA"].ToIntDef(0) | TabPos[ppos]["POS_PRENOTATA"].ToIntDef(0);
     ClientData.Zona[i].Postazione[j].IDUDC = TabPos[ppos]["IDUDC"].ToIntDef(0);
     }
     }
     // contatore
     for (int i = 1; i <= NUM_ZONE; i++) {
     for (int j = 1; j <= ClientData.Zona[i].numpostazioni; j++) {
     if (ClientData.Zona[1].Postazione[j].presenzaUDC == 1) {
     ClientData.Zona[1].Postazione[j].contatore++;
     }
     else {
     ClientData.Zona[1].Postazione[j].contatore = 0;
     }
     }
     }
     */
}

void __fastcall TDMPlc::TimerPlcTimer(TObject * Sender) {
    // imposto datyi in memoria da Plc
    int i, j, indice, valdascrivere, bitout;
    static int lampeggio = 0;

    if ((MainForm->count_inizio < 10) || (!dmDB->lettostatoagv))
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

        CompilaStrutturaSegnaliGenerali(ClientData.Plc[i], ClientData.Plc[i].SegnaliGenerali[1]);
    }

    // in
    lampeggio = ++lampeggio % 2;
    TimerPlc->Enabled = true;
}

// ---------------------------------------------------------------------------

void TDMPlc::CompilaStrutturaPrelievo(DatiPlc & Plc, DatiPrelievo & Prelievo) {
    AnsiString stringa = "";
    int i, j, indice_byte;
    // compilo i dati della struttura plc passata
    Plc.PlcConnesso = PLCThread[Plc.id_thread_plc]->connected;
    Plc.Error = PLCThread[Plc.id_thread_plc]->PLCError;
    Plc.CycleTime = PLCThread[Plc.id_thread_plc]->tempo_ciclo_int;
    Plc.watchdog_plc = PLCThread[Plc.id_thread_plc]->watchdog_plc;
    Plc.watchdog_pc = PLCThread[Plc.id_thread_plc]->watchdog_pc;

    if ((PLCThread[Plc.id_thread_plc]->connected) || (ClientData.ParametriFunzionali.Simula))
    {
        if ((PLCThread[Plc.id_thread_plc]->connected) && (!ClientData.ParametriFunzionali.Simula))
        {
            // raw data
            Prelievo.StatoInput[0] = PLCThread[Plc.id_thread_plc]->DB[Prelievo.dbInput][Prelievo.byteInput];
            Prelievo.StatoInput[1] = (PLCThread[Plc.id_thread_plc]->DB[Prelievo.dbInput][Prelievo.byteInput + 4] * 256)
                + PLCThread[Plc.id_thread_plc]->DB[Prelievo.dbInput][Prelievo.byteInput + 4 + 1];
            Prelievo.StatoInput[2] = 0;
            Prelievo.StatoInput[3] = 0;
        }
        // in
        Prelievo.Ready = Prelievo.StatoInput[0] & dmDB->bit[Prelievo.bit_Ready];
        Prelievo.InAllarme = Prelievo.StatoInput[0] & dmDB->bit[Prelievo.bit_InAllarme];
        Prelievo.ProntaAlPrelievo = Prelievo.StatoInput[0] & dmDB->bit[Prelievo.bit_ProntaAlPrelievo];
        Prelievo.OkPrelievo = Prelievo.StatoInput[0] & dmDB->bit[Prelievo.bit_OkPrelievo];
        // Prelievo.PalletID = Prelievo.StatoInput[1];

        // out
        if ((PLCThread[Plc.id_thread_plc]->connected) && (!ClientData.ParametriFunzionali.Simula))
        {
            // lettura missione da plc
            Prelievo.uscite[0] = PLCThread[Plc.id_thread_plc]->DB[Prelievo.dbOutput][Prelievo.byteOutput];
        }

        if (MainForm->count_inizio < 20)
        {
            Prelievo.uscitedascrivere[0] = Prelievo.uscite[0];
        }
        Prelievo.RichiestaAbilitaPrel = Prelievo.uscitedascrivere[0] & dmDB->bit[Prelievo.bit_RichiestaAbilitaPrel];
        Prelievo.AGVInIngombro = Prelievo.uscitedascrivere[0] & dmDB->bit[Prelievo.bit_AGVInIngombro];

        if (MainForm->count_inizio > 20)
        {
            if (Prelievo.uscitedascrivere[0] != Prelievo.uscite[0])
            {
                if (PLCThread[Plc.id_thread_plc]->connected)
                {
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
            Deposito.StatoInput[bytedipartenza + 1] = PLCThread[Plc.id_thread_plc]->DB[Deposito.dbInput][Deposito.byteInput + 4];
            Deposito.StatoInput[bytedipartenza + 2] = 0;
            Deposito.StatoInput[bytedipartenza + 3] = 0;
        }
                Deposito.Ready = ((Deposito.StatoInput[bytedipartenza] & dmDB->bit[Deposito.bit_Ready]) == 0 ? 0 : 1);
        Deposito.InAllarme = ((Deposito.StatoInput[bytedipartenza] & dmDB->bit[Deposito.bit_InAllarme]) == 0 ? 0 : 1);
        Deposito.ProntaAlDeposito = ((Deposito.StatoInput[bytedipartenza] & dmDB->bit[Deposito.bit_ProntaAlDeposito]) == 0 ? 0 : 1);
        Deposito.OkDeposito = ((Deposito.StatoInput[bytedipartenza] & dmDB->bit[Deposito.bit_OkDeposito]) == 0 ? 0 : 1);

        // out
        if ((PLCThread[Plc.id_thread_plc]->connected) && (!ClientData.ParametriFunzionali.Simula)) {
            // lettura missione da plc
            Deposito.uscite[0] = PLCThread[Plc.id_thread_plc]->DB[Deposito.dbOutput][Deposito.byteOutput];
            Deposito.uscite[1] = PLCThread[Plc.id_thread_plc]->DB[Deposito.dbOutput][Deposito.byteOutput + 4] * 256 +
                PLCThread[Plc.id_thread_plc]->DB[Deposito.dbOutput][Deposito.byteOutput + 4 + 1];
        }

        if (MainForm->count_inizio < 20) {
            Deposito.uscitedascrivere[0] = Deposito.uscite[0];
            Deposito.uscitedascrivere[1] = Deposito.uscite[1];
        }
        Deposito.RichiestaAbilitaDep = Deposito.uscitedascrivere[0] & dmDB->bit[Deposito.bit_RichiestaAbilitaDep];
        Deposito.AGVInIngombro = Deposito.uscitedascrivere[0] & dmDB->bit[Deposito.bit_AGVInIngombro];
     //   Deposito.PalletID = Deposito.uscitedascrivere[1];

        // gestione watchdog
        // gestisco uscite :
        if (MainForm->count_inizio > 20)
        {
            if (Deposito.uscitedascrivere[0] != Deposito.uscite[0])
            {
                if (PLCThread[Plc.id_thread_plc]->connected) {
                    PLCThread[Plc.id_thread_plc]->WriteByte(Deposito.dbOutput, Deposito.byteOutput, Deposito.uscitedascrivere[0], ClientData.ParametriFunzionali.Simula);
                }
                else if (ClientData.ParametriFunzionali.Simula)
                    Deposito.uscite[0] = Deposito.uscitedascrivere[0];
                dmDB->LogMsg("Modificato statouscite deposito" + IntToStr(Deposito.pos) + " su Plc pos " + IntToStr(Deposito.pos) + " , valore prec : " + IntToStr(Deposito.uscite[0]) + " , nuovo : " +
                    IntToStr(Deposito.uscitedascrivere[0]));
            }
            if (Deposito.uscitedascrivere[1] != Deposito.uscite[1])
            {
                if (PLCThread[Plc.id_thread_plc]->connected)
                {
                    PLCThread[Plc.id_thread_plc]->WriteWord(Deposito.dbOutput, Deposito.byteOutput + 4, Deposito.uscitedascrivere[1], ClientData.ParametriFunzionali.Simula);
                }
                else if (ClientData.ParametriFunzionali.Simula)
                    Deposito.uscite[1] = Deposito.uscitedascrivere[1];
                dmDB->LogMsg("Modificato idpallet deposito" + IntToStr(Deposito.pos) + " su Plc pos " + IntToStr(Deposito.pos) + " , valore prec : " + IntToStr(Deposito.uscite[1]) + " , nuovo : " +
                    IntToStr(Deposito.uscitedascrivere[1]));
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

void TDMPlc::CompilaStrutturaSegnaliGenerali(DatiPlc & Plc, DatiSegnaliGenerali & SegnaliGenerali)
{
    AnsiString stringa = "";
    int i, j, indice_byte;
    // compilo i dati della struttura plc passata
    Plc.PlcConnesso = PLCThread[Plc.id_thread_plc]->connected;
    Plc.Error = PLCThread[Plc.id_thread_plc]->PLCError;
    Plc.CycleTime = PLCThread[Plc.id_thread_plc]->tempo_ciclo_int;
    Plc.watchdog_plc = PLCThread[Plc.id_thread_plc]->watchdog_plc;
    Plc.watchdog_pc = PLCThread[Plc.id_thread_plc]->watchdog_pc;

    if ((PLCThread[Plc.id_thread_plc]->connected) || (ClientData.ParametriFunzionali.Simula)) {
        if ((PLCThread[Plc.id_thread_plc]->connected) && (!ClientData.ParametriFunzionali.Simula)) {
            // raw data
            SegnaliGenerali.StatoInput[0] = PLCThread[Plc.id_thread_plc]->DB[SegnaliGenerali.dbInput][SegnaliGenerali.byteInput_accesso];
            SegnaliGenerali.StatoInput[1] = 0;
            SegnaliGenerali.StatoInput[2] = 0;
            SegnaliGenerali.StatoInput[3] = 0;
        }
        // in
        SegnaliGenerali.Input_conferma_accesso = SegnaliGenerali.StatoInput[0] & dmDB->bit[SegnaliGenerali.bitInput_conferma_accesso]; // dmExtraFunction->bitWrite(Deposito.BaiaScaricoInEmergenza, 0, Deposito.StatoInput[0] & dmDB->bit[0]);
        SegnaliGenerali.Input_accesso_terminato = SegnaliGenerali.StatoInput[0] & dmDB->bit[SegnaliGenerali.bitInput_accesso_terminato];

        // out
        if ((PLCThread[Plc.id_thread_plc]->connected) && (!ClientData.ParametriFunzionali.Simula))
        {
            // lettura delle uscite sul PLC
            SegnaliGenerali.uscite[0] = PLCThread[Plc.id_thread_plc]->DB[SegnaliGenerali.dbOutput][SegnaliGenerali.byteOutput_accesso];
        }

        if (MainForm->count_inizio < 20)
        {
            // inizializzo nei primi 20 secondi a ciò che è già scritto sul PLC
            SegnaliGenerali.uscitedascrivere[0] = SegnaliGenerali.uscite[0];
        }
        SegnaliGenerali.Output_conferma_accesso = SegnaliGenerali.uscitedascrivere[0] & dmDB->bit[SegnaliGenerali.bitOutput_conferma_accesso];

        // gestisco uscite :
        if (MainForm->count_inizio > 20)
        {
            if (SegnaliGenerali.uscitedascrivere[0] != SegnaliGenerali.uscite[0])
            {
                if (PLCThread[Plc.id_thread_plc]->connected)
                {
                    PLCThread[Plc.id_thread_plc]->WriteByte(SegnaliGenerali.dbOutput, SegnaliGenerali.byteOutput_accesso, SegnaliGenerali.uscitedascrivere[0], ClientData.ParametriFunzionali.Simula);
                }
                else if (ClientData.ParametriFunzionali.Simula)
                    SegnaliGenerali.uscite[0] = SegnaliGenerali.uscitedascrivere[0];
                dmDB->LogMsg("Modificato statouscite su Plc segnali generali , valore prec : " + IntToStr(SegnaliGenerali.uscite[0]) + " , nuovo : " + IntToStr(SegnaliGenerali.uscitedascrivere[0]));
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

void __fastcall TDMPlc::TimerWatchDogPlcTimer(TObject * Sender) {
    // gestisco tutti i watchdog dei plc presenti, anche le porte sono qui
    int i;

    if ((MainForm->count_inizio < 15) || (MainForm->close_program))
    {
        return;
    }

    TimerWatchDogPlc->Enabled = false;
    for (i = 0; i < N_PLC; i++)
    {
        if ((PLCThread[i]->connected) && (!ClientData.ParametriFunzionali.Simula))
        {
            PLCThread[i]->watchdog_plc = PLCThread[i]->DB[PLCThread[i]->dbwatchdog_plc][PLCThread[i]->wordwatchdog_plc] & dmDB->bit[PLCThread[0]->bit_watchdog_plc];
            PLCThread[i]->watchdog_pc = PLCThread[i]->DB[PLCThread[i]->dbwatchdog_pc][PLCThread[i]->wordwatchdog_pc] & dmDB->bit[PLCThread[0]->bit_watchdog_pc];

            if ((PLCThread[i]->watchdog_plc != PLCThread[i]->watchdog_pc) && (PLCThread[i]->connected))
            {
                PLCThread[i]->WriteByte(PLCThread[i]->dbwatchdog_pc, PLCThread[i]->wordwatchdog_pc, PLCThread[i]->watchdog_plc, ClientData.ParametriFunzionali.Simula);
                if (PLCThread[i]->alarm_watchdog)
                {
                    MainForm->MessageBar1->DelMsgNo(PLCThread[i]->num_alarm_watchdog);
                    PLCThread[i]->alarm_watchdog = 0;
                    dmDB->AggiornaAllarmiAttivi();
                }
                PLCThread[i]->count_alarm_watchdog = 0;
            }
            else
            {
                PLCThread[i]->count_alarm_watchdog++;
                if (PLCThread[i]->count_alarm_watchdog >= 20)
                {
                    if (!PLCThread[i]->alarm_watchdog)
                    {
                        MainForm->MessageBar1->AddMsgNo(PLCThread[i]->num_alarm_watchdog);
                        PLCThread[i]->alarm_watchdog = 1;
                        dmDB->AggiornaAllarmiAttivi();
                    }
                    PLCThread[i]->count_alarm_watchdog = 0;
                }
            }
        }
    }
    TimerWatchDogPlc->Enabled = true;
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

void TDMPlc::ResettaRichiestaIngressoBaiaPrelievo(DatiPrelievo & Prelievo)
{
    int conta_agv = 0;
    for (int agv = 1; agv <= NAGV; agv++)
    {
        if ((!ClientData.DatiAgv[agv].richiestaconsensonodo) && (!ClientData.DatiAgv[agv].consensonododato)) // ClientData.DatiAgv[agv].pos != Prelievo.pos
        {
            conta_agv++;
        }
    }
    if ((Prelievo.RichiestaAbilitaPrel) && (conta_agv == NAGV))
    {
        Prelievo.uscitedascrivere[0] = Prelievo.uscitedascrivere[0] & dmDB->bitAnd[Prelievo.bit_RichiestaAbilitaPrel];
        dmDB->LogMsg("Resetto Richiesta Ingresso Agv su baia prelievo " + IntToStr(Prelievo.pos) + " perchè rimasta appesa e nessun agv con richiesta attiva");
    }
}

void TDMPlc::ResettaRichiestaIngressoBaiaDeposito(DatiDeposito & Deposito)
{
    int conta_agv = 0;
    for (int agv = 1; agv <= NAGV; agv++)
    {
        if ((!ClientData.DatiAgv[agv].richiestaconsensonodo) && (!ClientData.DatiAgv[agv].consensonododato))
        {
            conta_agv++;
        }
        if ((Deposito.RichiestaAbilitaDep) && (conta_agv == NAGV))
        {
            Deposito.uscitedascrivere[0] = Deposito.uscitedascrivere[0] & dmDB->bitAnd[Deposito.bit_RichiestaAbilitaDep];
            dmDB->LogMsg("Resetto Richiesta Ingresso Agv su baia deposito " + IntToStr(Deposito.pos) + " perchè rimasta appesa e nessun agv con richiesta attiva");
        }
    }
}

int TDMPlc::AgvInIngombroPosizionePrelievo(DatiPrelievo & Prelievo) // VERSIONE VECCHIA
{
    for (int agv = 1;
        agv <= NAGV;
        agv++)
    {
        if ((Prelievo.pos == ClientData.DatiAgv[agv].pos) && (ClientData.DatiAgv[agv].idmis))
        {
            if (ClientData.DatiAgv[agv].forchedx)
            {
                return 1;
            }
            else if (ClientData.DatiAgv[agv].forchesx)
            {
                return 2;
            }

        }
    }
    return 0;
}

int TDMPlc::AgvInIngombroPosizionePrelievoNuovo(DatiPrelievo & Prelievo) // VERSIONE PRELIEVO INGOMBRO FORCHE
{
    for (int agv = 1;
        agv <= NAGV;
        agv++)
    {
        if ((Prelievo.pos == ClientData.DatiAgv[agv].pos) &&
            (Prelievo.pos == ClientData.DatiAgv[agv].prel) && (!ClientData.DatiAgv[agv].load))
        {
            if ((ClientData.DatiAgv[agv].forche_in_trazione == 1))
            {
                return 1;
            }
        }
    }
    return 0;
}

int TDMPlc::AgvInIngombroPosizioneDeposito(DatiDeposito & Deposito) // VERSIONE VECCHIA
{
    for (int agv = 1; agv <= NAGV; agv++)
    {
        if ((Deposito.pos == ClientData.DatiAgv[agv].pos) && (Deposito.piano == ClientData.DatiAgv[agv].piano_deposito)
            && (Deposito.pos == ClientData.DatiAgv[agv].dep))
        {
            if (ClientData.DatiAgv[agv].forchedx)
            {
                return 1;
            }
            else if (ClientData.DatiAgv[agv].forchesx)
            {
                return 2;
            }
        }
    }
    return 0;
}

int TDMPlc::AgvInIngombroPosizioneDepositoNuovo(DatiDeposito & Deposito) // VERSIONE CON INGOMBRO FORCHE
{
    for (int agv = 1; agv <= NAGV; agv++)
    {
        if ((Deposito.pos == ClientData.DatiAgv[agv].pos) && (Deposito.piano == ClientData.DatiAgv[agv].piano_deposito)
            && (Deposito.pos == ClientData.DatiAgv[agv].dep))
        {
            if ((ClientData.DatiAgv[agv].forche_in_trazione == 1))
            {
                return 1;
            }
        }
    }
    return 0;
}

void TDMPlc::GestioneIngombroAgvPrelievo(DatiPrelievo & Prelievo)
{
    int i, ingombro_agv;
    AnsiString posAgv = "";

    for (int agv = 1; agv <= NAGV; agv++)
    {
        posAgv = posAgv + " Agv " + IntToStr(agv) + " in pos " + IntToStr(ClientData.DatiAgv[agv].pos) + " ; Forchefuori (" + IntToStr(ClientData.DatiAgv[agv].forchedx) + "-" + IntToStr(ClientData.DatiAgv[agv].forchesx) + ")";
    }
    // ingombro_agv = AgvInIngombroPosizionePrelievo(Prelievo);  // MODIFICA INGOMBRO
    ingombro_agv = AgvInIngombroPosizionePrelievoNuovo(Prelievo);
    if (ingombro_agv == 1)
    {
        if ((!Prelievo.AGVInIngombro)) // && (Prelievo.AbilitaPrelievo))   // MODIFICA INGOMBRO
        {
            Prelievo.uscitedascrivere[0] = Prelievo.uscitedascrivere[0] | dmDB->bit[Prelievo.bit_AGVInIngombro];
            dmDB->LogMsg("Setto ingombro Agv su rulliera prelievo posizione " + IntToStr(Prelievo.pos) + " , " + posAgv);
        }
    }
    else if (ingombro_agv == 2)
    {
        if ((!Prelievo.AGVInIngombro)) // && (Prelievo.AbilitaPrelievo))  // MODIFICA INGOMBRO
        {
            Prelievo.uscitedascrivere[0] = Prelievo.uscitedascrivere[0] | dmDB->bit[Prelievo.bit_AGVInIngombro];
            dmDB->LogMsg("Setto ingombro Agv su rulliera prelievo posizione " + IntToStr(Prelievo.pos) + " , " + posAgv);
        }
    }
    else {
        if (Prelievo.AGVInIngombro)
        {
            Prelievo.uscitedascrivere[0] = Prelievo.uscitedascrivere[0] & dmDB->bitAnd[Prelievo.bit_AGVInIngombro];
            dmDB->LogMsg("Resetto ingombro Agv su rulliera prelievo posizione " + IntToStr(Prelievo.pos) + " , " + posAgv);
        }
    }
}

void TDMPlc::GestioneIngombroAgvDeposito(DatiDeposito & Deposito)
{
    int i, ingombro_agv;
    AnsiString posAgv = "";
    int id_pallet = 0;
    for (int agv = 1; agv <= NAGV; agv++)
    {
        posAgv = posAgv + " Agv " + IntToStr(agv) + " in pos " + IntToStr(ClientData.DatiAgv[agv].pos) + " ; Forchefuori (" + IntToStr(ClientData.DatiAgv[agv].forchedx) + "-" + IntToStr(ClientData.DatiAgv[agv].forchesx) + ")";
    }
    // ingombro_agv = AgvInIngombroPosizioneDeposito(Deposito);  // MODIFICA INGOMBRO
    ingombro_agv = AgvInIngombroPosizioneDepositoNuovo(Deposito);
    if (ingombro_agv == 1)
    {
        if ((!Deposito.AGVInIngombro)) // && (Deposito.AbilitaDeposito)) // MODIFICA INGOMBRO
        {
            Deposito.uscitedascrivere[0] = Deposito.uscitedascrivere[0] | dmDB->bit[Deposito.bit_AGVInIngombro];
            dmDB->LogMsg("Setto ingombro Agv su rulliera deposito posizione " + IntToStr(Deposito.pos) + " , " + posAgv);
        }
    }
    else if (ingombro_agv == 2) {
        if ((!Deposito.AGVInIngombro)) // && (Deposito.AbilitaDeposito))     // MODIFICA INGOMBRO
        {
            Deposito.uscitedascrivere[0] = Deposito.uscitedascrivere[0] | dmDB->bit[Deposito.bit_AGVInIngombro];
            dmDB->LogMsg("Setto ingombro Agv su rulliera deposito posizione " + IntToStr(Deposito.pos) + " , " + posAgv);
        }
    }
    else {
        if (Deposito.AGVInIngombro) {
            Deposito.uscitedascrivere[0] = Deposito.uscitedascrivere[0] & dmDB->bitAnd[Deposito.bit_AGVInIngombro];
            Deposito.uscitedascrivere[1] = 0;
            dmDB->LogMsg("Resetto ingombro Agv su rulliera deposito posizione " + IntToStr(Deposito.pos) + " , " + posAgv);
        }
    }
}

void TDMPlc::GestioneRichiestaIngressoBaiaPrelievo(DatiPrelievo & Prelievo)
{
    Evento ev;
    int agv_in_richiesta = 0;
    for (int agv = 1; agv <= NAGV; agv++)
    {
        if ((ClientData.DatiAgv[agv].richiestaconsensonodo) &&
            (!ClientData.DatiAgv[agv].consensonododato))
        {
            if (ClientData.DatiAgv[agv].op_richiestaconsensonodo == 'P')
            {
                if (ClientData.DatiAgv[agv].nodoa == Prelievo.pos)
                {
                    if ((!Prelievo.OkPrelievo) && (!Prelievo.RichiestaAbilitaPrel))
                    {
                        Prelievo.uscitedascrivere[0] = Prelievo.uscitedascrivere[0] | dmDB->bit[Prelievo.bit_RichiestaAbilitaPrel];
                        dmDB->LogMsg("Set RichiestaAbilitaPrel su posizione " + IntToStr(Prelievo.pos));
                    }
                    else if ((Prelievo.OkPrelievo) && (Prelievo.ProntaAlPrelievo) && (ClientData.DatiAgv[agv].pos == ClientData.DatiAgv[agv].nodoa))
                    {
                        // consenso
                        ClientData.DatiAgv[agv].richiestaconsensonodo = 0;
                        ClientData.DatiAgv[agv].consensonododato = ClientData.DatiAgv[agv].nodoa;
                        dmDB->LogMsg("Consenso ingresso per prelievo su rulliera " + IntToStr(Prelievo.pos) + " inviato ad Agv " + IntToStr(agv) + " in pos " +
                            IntToStr(ClientData.DatiAgv[agv].pos) + " ,dest " + IntToStr(ClientData.DatiAgv[agv].dest));
                        // resetto richiesta
                        /* Prelievo.uscitedascrivere[0] = Prelievo.uscitedascrivere[0] & dmDB->bitAnd[Prelievo.bit_RichiestaAbilitaPrel];
                         dmDB->LogMsg("Resetto richiesta ingresso per prelievo su rulliera " + IntToStr(Prelievo.pos) + " inviato ad Agv " + IntToStr(agv) + " in pos " +
                         IntToStr(ClientData.DatiAgv[agv].pos) + " ,dest " + IntToStr(ClientData.DatiAgv[agv].dest)); */
                        sprintf(ev, "O,N,%d,%d,%c,%d", agv, ClientData.DatiAgv[agv].nodoa, ClientData.DatiAgv[agv].op_richiestaconsensonodo, 1);
                        cs->SendEv(ev);
                    }
                    else if (ClientData.DatiAgv[agv].pos == ClientData.DatiAgv[agv].nodoa)
                    {
                        if (!ClientData.DatiAgv[agv].allarme_consenso)
                        {
                            MainForm->MessageBar1->Messages->Strings[ClientData.DatiAgv[agv].num_allarme_consenso] = "AGV in Attesa PRELIEVO, AGV " + IntToStr(agv);
                            MainForm->MessageBar1->AddMsgNo(ClientData.DatiAgv[agv].num_allarme_consenso);
                            ClientData.DatiAgv[agv].allarme_consenso = 1;
                            dmDB->AggiornaAllarmiAttivi();
                        }
                    }
                }

            }
        }
        // azzero allarme consenso se non ho la richiesta di consenso
        else if (ClientData.DatiAgv[agv].allarme_consenso)
        {
            MainForm->MessageBar1->DelMsgNo(ClientData.DatiAgv[agv].num_allarme_consenso);
            ClientData.DatiAgv[agv].allarme_consenso = 0;
            dmDB->AggiornaAllarmiAttivi();
        }
        //
        if ((ClientData.DatiAgv[agv].consensonododato == Prelievo.pos) && (ClientData.DatiAgv[agv].pos == Prelievo.pos))
        {
            // richiesta attiva

            if (!Prelievo.OkPrelievo)
            {
                if ((!ClientData.DatiAgv[agv].stand_by) && (Prelievo.AGVInIngombro))
                {
                    dmComandiAgv->StandByAgv(agv, 1);
                    dmDB->LogMsg("Fermo AGV " + IntToStr(agv) + " in standby pos " + IntToStr(ClientData.DatiAgv[agv].pos) + " perchè consenso ingresso basso");
                }
            }
            else
            {
                if (ClientData.DatiAgv[agv].stand_by)
                {
                    dmComandiAgv->StandByAgv(agv, 0);
                    dmDB->LogMsg("Resetto standby in pos " + IntToStr(ClientData.DatiAgv[agv].pos) + " per agv " + IntToStr(agv) + " perchè consenso ingresso tornato alto ");
                }
            }
        }
        if (((ClientData.DatiAgv[agv].richiestaconsensonodo) && (Prelievo.pos == ClientData.DatiAgv[agv].nodoa))
            || (ClientData.DatiAgv[agv].consensonododato == Prelievo.pos))
            agv_in_richiesta = 1;
    }
    if ((Prelievo.RichiestaAbilitaPrel) && (!agv_in_richiesta) && (!Prelievo.AGVInIngombro)) // eventualmente aggiungere non in ingombro
        // resetto richiesta
    {
        // resetto richiesta
        Prelievo.uscitedascrivere[0] = Prelievo.uscitedascrivere[0] & dmDB->bitAnd[Prelievo.bit_RichiestaAbilitaPrel];
        dmDB->LogMsg("Resetto richiesta ingresso per prelievo su rulliera " + IntToStr(Prelievo.pos));
    }
}

void TDMPlc::GestioneRichiestaIngressoBaiaDeposito(DatiDeposito & Deposito)
{
    Evento ev;
    int agv_in_richiesta = 0;
    for (int agv = 1; agv <= NAGV; agv++)
    {
        if ((ClientData.DatiAgv[agv].richiestaconsensonodo) &&
            (!ClientData.DatiAgv[agv].consensonododato))
        {
            if (ClientData.DatiAgv[agv].op_richiestaconsensonodo == 'D')
            {
                if ((ClientData.DatiAgv[agv].nodoa == Deposito.pos) && (ClientData.DatiAgv[agv].piano_deposito == Deposito.piano))
                {
                    if ((!Deposito.OkDeposito) && (!Deposito.RichiestaAbilitaDep))
                    {
                        Deposito.uscitedascrivere[0] = Deposito.uscitedascrivere[0] | dmDB->bit[Deposito.bit_RichiestaAbilitaDep];
                        dmDB->LogMsg("Set RichiestaAbilitaDep su posizione " + IntToStr(Deposito.pos));
                        // scrittura id pallet
                        // Deposito.uscitedascrivere[1] = ClientData.DatiAgv[agv].codice_pallet;
                        Deposito.uscitedascrivere[1] = ClientData.DatiAgv[agv].DatiUDC.IDUDC;
                        dmDB->LogMsg("Scrivo codice pallet " + IntToStr(ClientData.DatiAgv[agv].DatiUDC.IDUDC) + " rulliera deposito " + IntToStr(Deposito.pos));
                    }
                    else if ((Deposito.OkDeposito) && (Deposito.ProntaAlDeposito) && (ClientData.DatiAgv[agv].pos == ClientData.DatiAgv[agv].nodoa))
                    {
                        ClientData.DatiAgv[agv].richiestaconsensonodo = 0;
                        ClientData.DatiAgv[agv].consensonododato = ClientData.DatiAgv[agv].nodoa;
                        // Deposito.uscitedascrivere[0] = Deposito.uscitedascrivere[0] & dmDB->bitAnd[Deposito.bit_RichiestaAbilitaDep];
                        dmDB->LogMsg("Consenso deposito su rulliera " + IntToStr(Deposito.pos) + " AGV = " + IntToStr(agv) + " POS " +
                            IntToStr(ClientData.DatiAgv[agv].pos) + " DEST " + IntToStr(ClientData.DatiAgv[agv].dest) + " PIANO " + IntToStr(ClientData.DatiAgv[agv].piano_deposito));
                        /* Deposito.uscitedascrivere[0] = Deposito.uscitedascrivere[0] & dmDB->bitAnd[Deposito.bit_RichiestaAbilitaDep];
                         dmDB->LogMsg("Resetto richiesta deposito su rulliera " + IntToStr(Deposito.pos) + " AGV = " + IntToStr(agv) + " POS " +
                         IntToStr(ClientData.DatiAgv[agv].pos) + " DEST " + IntToStr(ClientData.DatiAgv[agv].dest) + " PIANO " + IntToStr(ClientData.DatiAgv[agv].piano_deposito)); */
                        sprintf(ev, "O,N,%d,%d,%c,%d", agv, ClientData.DatiAgv[agv].nodoa, ClientData.DatiAgv[agv].op_richiestaconsensonodo, 1);
                        cs->SendEv(ev);
                    }
                    else if (ClientData.DatiAgv[agv].pos == ClientData.DatiAgv[agv].nodoa)
                    {
                        if (!ClientData.DatiAgv[agv].allarme_consenso)
                        {
                            MainForm->MessageBar1->Messages->Strings[ClientData.DatiAgv[agv].num_allarme_consenso] = "Agv in Attesa Ok deposito, AGV " + IntToStr(agv);
                            MainForm->MessageBar1->AddMsgNo(ClientData.DatiAgv[agv].num_allarme_consenso);
                            ClientData.DatiAgv[agv].allarme_consenso = 1;
                            dmDB->AggiornaAllarmiAttivi();
                        }
                    }
                }
            }
        }
        // azzero allarme consenso se non ho la richiesta di consenso
        else if (ClientData.DatiAgv[agv].allarme_consenso)
        {
            MainForm->MessageBar1->DelMsgNo(ClientData.DatiAgv[agv].num_allarme_consenso);
            ClientData.DatiAgv[agv].allarme_consenso = 0;
            dmDB->AggiornaAllarmiAttivi();
        }
        //
        if ((ClientData.DatiAgv[agv].consensonododato == Deposito.pos) && (ClientData.DatiAgv[agv].piano_deposito == Deposito.piano))
        {
            if (!Deposito.OkDeposito)
            {
                if ((!ClientData.DatiAgv[agv].stand_by) && (Deposito.AGVInIngombro))
                {
                    dmComandiAgv->StandByAgv(agv, 1);
                    dmDB->LogMsg("Fermo AGV " + IntToStr(agv) + " in standby pos " + IntToStr(ClientData.DatiAgv[agv].pos) + " perchè consenso ingresso basso");
                }
            }
            else
            {
                if (ClientData.DatiAgv[agv].stand_by)
                {
                    dmComandiAgv->StandByAgv(agv, 0);
                    dmDB->LogMsg("Resetto standby in pos " + IntToStr(ClientData.DatiAgv[agv].pos) + " per agv " + IntToStr(agv) + " perchè consenso ingresso tornato alto ");
                }
            }
        }
        if (((ClientData.DatiAgv[agv].richiestaconsensonodo) && (Deposito.pos == ClientData.DatiAgv[agv].nodoa) && (ClientData.DatiAgv[agv].piano_deposito == Deposito.piano))
            || (ClientData.DatiAgv[agv].consensonododato == Deposito.pos))
            agv_in_richiesta = 1;
    }
    if ((Deposito.RichiestaAbilitaDep) && (!agv_in_richiesta) && (!Deposito.AGVInIngombro)) // eventualmente aggiungere non in ingombro
        // resetto richiesta
    {
        // resetto richiesta
        Deposito.uscitedascrivere[0] = Deposito.uscitedascrivere[0] & dmDB->bitAnd[Deposito.bit_RichiestaAbilitaDep];
        dmDB->LogMsg("Resetto richiesta ingresso per prelievo su rulliera " + IntToStr(Deposito.pos));
    }

}

int TDMPlc::ScriviIdPalletPosizioneDiDeposito(int piano, int posizione, int agv)
{
    int idPlc = 0;
    idPlc = dmDBImpianto->RitornaIdPlcDaPosizione(posizione, 2, piano);
    ClientData.Plc[1].Deposito[idPlc][piano].uscitedascrivere[1] = ClientData.DatiAgv[agv].DatiUDC.IDUDC;
    dmDB->LogMsg("Scrivo id pallet " + IntToStr(ClientData.DatiAgv[agv].DatiUDC.IDUDC) + " a posizione di deposito");
}

int TDMPlc::InserisciIdPalletInAgv(int piano, int posizione, int agv) {
    int idPlc = 0;
    idPlc = dmDBImpianto->RitornaIdPlcDaPosizione(posizione, 1, 1);
    ClientData.DatiAgv[agv].DatiUDC.IDUDC = ClientData.Plc[1].Prelievo[idPlc][piano].PalletID;
}

void TDMPlc::GestioneStandbyEmergenza(DatiSegnaliGenerali & SegnaliGenerali) // modificata solo qua
{
    if ((SegnaliGenerali.Input_conferma_accesso) && (!SegnaliGenerali.Input_accesso_terminato))
    {
        int count = 0;
        for (int agv = 1; agv <= NAGV; agv++) {
            if ((!ClientData.DatiAgv[agv].manuale) && (!ClientData.DatiAgv[agv].pos > 0)) {
                if (!ClientData.DatiAgv[agv].standby_operatore) {
                    dmComandiAgv->StandByAgvOperatore(agv, 1);
                }
            }
            count++;
        }
        if ((count == NAGV) && (!SegnaliGenerali.Output_conferma_accesso))
        {
            SegnaliGenerali.uscitedascrivere[0] = SegnaliGenerali.uscitedascrivere[0] | dmDB->bit[SegnaliGenerali.bitOutput_conferma_accesso];
            dmDB->LogMsg("Mando in standby AGV per richiesta accesso");
        }
    }

    if (((SegnaliGenerali.Input_accesso_terminato) && (SegnaliGenerali.Input_conferma_accesso)) ||
        ((!SegnaliGenerali.Input_accesso_terminato) && (!SegnaliGenerali.Input_conferma_accesso)))
    {
        for (int agv = 1; agv <= NAGV; agv++)
        {
            if (ClientData.DatiAgv[agv].standby_operatore)
            {
                dmComandiAgv->StandByAgvOperatore(agv, 0);
                dmDB->LogMsg("Resetto standby AGV per richiesta accesso");
            }
        }
        if (SegnaliGenerali.Output_conferma_accesso)
            SegnaliGenerali.uscitedascrivere[0] = SegnaliGenerali.uscitedascrivere[0] & dmDB->bitAnd[SegnaliGenerali.bitOutput_conferma_accesso];
    }
}
