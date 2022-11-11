// ---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "DB.h"
#include "ExtraFunction.h"
#include "LogTxt.h"
#include "inifiles.hpp"
#include "time.h"
#include "main.h"
#include "percorsiprototipo.h"
#include "dmFunzioniComuniClientServer.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TdmDB *dmDB;
AnsiString logstr;
extern DATILGV tab;
int lunghezza_campi[NCAMPI] = {10, 10, 2, 20, 20};
int indice_inizio_campi[NCAMPI] = {0, 10, 20, 22, 42};
int tipologia_campi[NCAMPI] = {6, 6, 2, 6, 6}; // 1 byte ,2 int/word,3 doppia word,4 float,5 double ,6 char,7 string con primi 2 byte per lunghezza max e lunghezza stringa

// ---------------------------------------------------------------------------
void TdmDB::InizializzaVariabili() {
    // visto che questo modulo è presente sia nel client che nel serve inizializzo le varibili comuni
    // direttamente all'avvio dei programmi
    int i, j;

    bit[0] = 1;
    bit[1] = 2;
    bit[2] = 4;
    bit[3] = 8;
    bit[4] = 16;
    bit[5] = 32;
    bit[6] = 64;
    bit[7] = 128;
    bitAnd[0] = 0xFE;
    bitAnd[1] = 0xFD;
    bitAnd[2] = 0xFB;
    bitAnd[3] = 0xF7;
    bitAnd[4] = 0xEF;
    bitAnd[5] = 0xDF;
    bitAnd[6] = 0xBF;
    bitAnd[7] = 0x7F;

    PWD = "proxaut";
    num_porte = 0; // leggo da db
    quota_prel_default = 150;
    quota_prel_ribalatatore = 1100;
    quota_dep_default = 450;
    lettostatoagv = 0;
    lettoparametri = 0;
    ActualUser = NomeClientServer; // "NONE";

    StrTipoProdottoNULLSuPlc = ""; // metto 20 null
    format_data_ora_database = "dd/mm/yyyy hh':'mm':'ss";
    PosPesoCampione = 999;
    TolleranzaPesoCampione = 10;
    ClientData.ParametriFunzionali.Lastidudc = 0;
    // inizializzo strutture dati
    for (i = 1; i <= NAGV; i++) {
        ClientData.DatiAgv[i].num_allarme_consenso = ((i - 1) * 100) + 97;
        ClientData.DatiAgv[i].num_allarme_consenso_porta = ((i - 1) * 100) + 91;
        ClientData.DatiAgv[i].num_allarme_carico = ((i - 1) * 100) + 92;
    }
}

Variant ReadField(TADOQuery *q, AnsiString f) {
    Variant res = "";

    if (!(q->IsEmpty() || q->Eof || q->FieldByName(f)->IsNull))
        res = q->FieldByName(f)->Value;
    return res;
}

// ---------------------------------------------------------------------------
int ReadInt(TADOQuery *q, AnsiString f) {
    int res = 0;

    if (!(q->IsEmpty() || q->Eof || q->FieldByName(f)->IsNull))
        res = q->FieldByName(f)->Value;
    return res;
}
// ---------------------------------------------------------------------------

AnsiString ReadString(TADOQuery *q, AnsiString f) {
    AnsiString res = "";

    if (!(q->IsEmpty() || q->Eof || q->FieldByName(f)->IsNull))
        res = q->FieldByName(f)->AsString;
    return res;
}

// ---------------------------------------------------------------------------
float ReadFloat(TADOQuery *q, AnsiString f) {
    float res = 0.0;

    if (!(q->IsEmpty() || q->Eof || q->FieldByName(f)->IsNull))
        res = q->FieldByName(f)->AsFloat;
    return res;
}

// ---------------------------------------------------------------------------
__fastcall TdmDB::TdmDB(TComponent* Owner) : TDataModule(Owner) {
    TIniFile * ini;
    AnsiString ConnectionString, ConnectionString2, nomecl, nom_file_exe;
    int plc, i, j, server = 0;
    int numeroutente;
    t = 0;
    ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    nom_file_exe = Application->ExeName.UpperCase();
    if (nom_file_exe.Pos("SERVER")) {
        server = 1;
    }
    ADOConnection1->Close();
    ConnectionString = ini->ReadString("Database", "ConnectionString", "");
    nomecl = ini->ReadString("CLIENT", "ClientName", "");
    // NB: TRASFORMARE USERNUM IN POTENZA DEL 2 OVVERO   3   = 2^3 =8 ( where (utenti&8=8) )
    numeroutente = StrToInt(ini->ReadString("CLIENT", "UserNum", "0")); // se non trova nulla assegna 0 (bit 0)
    UserNum = String(pow(2, (numeroutente)));
    UserZona = numeroutente;
    ZonaString = UpperCase(Trim(ini->ReadString("CLIENT", "Zona", "")));

    ConnectionString2 = ini->ReadString("DatabaseExtra", "ConnectionString", "");
    Lingua = UpperCase(Trim(ini->ReadString("Language", "Lingua", "")));
    delete ini;
    if (ConnectionString != "")
        ADOConnection1->ConnectionString = ConnectionString;
    if ((nomecl != "") && (!server))
        NomeClientServer = nomecl;
    else
        NomeClientServer = "Server";
    if (DebugHook) {
        pwdlevel = 2;
        // Simula = true ;
    }
    else {
        pwdlevel = 0;
    }

    if (ConnectionString2 != "") {
        ADOConnectionExtra->ConnectionString = ConnectionString2;
    }

    InizializzaVariabili();

    TimerConnectTimer(this);
    // TimerConnect->Enabled = true;

}

AnsiString TdmDB::RitornaOrarioDaSecondi(int secondi) {
    int ore = 0;
    int minuti = 0;
    int sec;
    char res[10];
    AnsiString result = "00:00:00";
    div_t x;
    try {
        if (secondi >= 3600) {
            x = div(secondi, 3600);
            ore = x.quot;
            secondi = x.rem;
        }
        else
            ore = 0;
        if (secondi >= 60) {
            x = div(secondi, 60);
            minuti = x.quot;
            secondi = x.rem;
        }
        else
            minuti = 0;
        sprintf(res, "%02d:%02d:%02d", ore, minuti, secondi);
        result = res;
    }
    catch (...) {
        ADOConnection1->Close();
        LogMsg("Eccezione calcolo RitornaOrarioDaSecondi");
    }
    return result;
}

AnsiString TdmDB::AggiungiRimuoviSecondiAOrario(AnsiString ora, int secondi, bool rimuovi) {
    int ore = 0;
    int minuti = 0;
    int sec;
    char res[10];
    AnsiString result = "00:00:00";
    div_t x;
    try {
        if (secondi >= 3600) {
            x = div(secondi, 3600);
            ore = x.quot;
            secondi = x.rem;
        }
        else
            ore = 0;
        if (secondi >= 60) {
            x = div(secondi, 60);
            minuti = x.quot;
            secondi = x.rem;
        }
        else
            minuti = 0;
        sprintf(res, "%02d:%02d:%02d", ore, minuti, secondi);
        result = res;
        if (rimuovi)
            result = TimeToStr(StrToTime(ora) - StrToTime(result)); // ritorno l'ora in cui si esegue
        else
            result = TimeToStr(StrToTime(ora) + StrToTime(result)); // ritorno l'ora in cui si esegue
    }
    catch (...) {
        ADOConnection1->Close();
        LogMsg("Eccezione calcolo tempo");
    }
    return result;
}

AnsiString TdmDB::AggiungiRimuoviSecondiADataOra(AnsiString ora, int secondi, bool rimuovi) {
    int ore = 0;
    int minuti = 0;
    int sec;
    char res[10];
    AnsiString result = "00:00:00";
    div_t x;
    try {
        if (secondi >= 3600) {
            x = div(secondi, 3600);
            ore = x.quot;
            secondi = x.rem;
        }
        else
            ore = 0;
        if (secondi >= 60) {
            x = div(secondi, 60);
            minuti = x.quot;
            secondi = x.rem;
        }
        else
            minuti = 0;
        sprintf(res, "%02d:%02d:%02d", ore, minuti, secondi);
        result = res;
        if (rimuovi)
            result = DateTimeToStr(StrToDateTime(ora) - StrToTime(result)); // ritorno l'ora in cui si esegue
        else
            result = DateTimeToStr(StrToDateTime(ora) + StrToTime(result)); // ritorno l'ora in cui si esegue
    }
    catch (...) {
        ADOConnection1->Close();
        LogMsg("Eccezione calcolo tempo");
    }
    return result;
}

int TdmDB::AggiornaCampoTabella(AnsiString Tabella, AnsiString CampoChiave, int ValoreChiave, AnsiString CampoModifica, AnsiString ValoreModifica,
    bool isstring) {
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;
    AnsiString Val;

    if (!ADOConnection1->Connected)
        return 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        if (isstring) {
            Val = "'" + ValoreModifica + "'";
        }
        else {
            Val = ValoreModifica;
        }
        strsql.printf("UPDATE %s SET %s = %s WHERE %s = %d", Tabella, CampoModifica, Val, CampoChiave, ValoreChiave);
        ADOQuery->SQL->Text = strsql;
        res = ADOQuery->ExecSQL();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}
// ---------------------------------------------------------------------------

void TdmDB::CaricaTabella(AnsiString TableName, TRecordList &RecList) {
    AnsiString strsql, campo;
    TADOQuery *ADOQuery;
    Variant k;
    int j, i = 0;
    AnsiString val;

    if (!ADOConnection1->Connected)
        return;
    RecList.clear();
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT * FROM %s", TableName.c_str());
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        while (!ADOQuery->Eof) {
            for (j = 0; j < ADOQuery->FieldCount; j++) {
                campo = ADOQuery->FieldList->Fields[j]->FieldName.UpperCase();
                val = ADOQuery->FieldList->Fields[j]->AsString;
                RecList[i][campo] = val;
            }
            ADOQuery->Next();
            i++;
        }
        ADOQuery->Close();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

void TdmDB::FullTabella(AnsiString TableName, TRecordList &RecList) {
    AnsiString strsql, campo;
    TADOQuery *ADOQuery;
    Variant k;
    int j, i = 0;
    AnsiString val;

    if (!ADOConnection1->Connected)
        return;
    RecList.clear();
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("%s", TableName.c_str());
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        while (!ADOQuery->Eof) {
            for (j = 0; j < ADOQuery->FieldCount; j++) {
                campo = ADOQuery->FieldList->Fields[j]->FieldName.UpperCase();
                val = ADOQuery->FieldList->Fields[j]->AsString;
                RecList[i][campo] = val;
            }
            ADOQuery->Next();
            i++;
        }
        ADOQuery->Close();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

void TdmDB::CaricaTabellaK(AnsiString TableName, AnsiString KeyField, TIndexList &RecList) {
    AnsiString strsql, campo, chiave, valore;
    TADOQuery *ADOQuery;
    Variant k;
    int j, i = 0;

    if (!ADOConnection1->Connected)
        return;
    RecList.clear();
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT * FROM %s", TableName.c_str());
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        while (!ADOQuery->Eof) {
            for (j = 0; j < ADOQuery->FieldCount; j++) {
                chiave = ADOQuery->FieldByName(KeyField)->AsString;
                campo = ADOQuery->FieldList->Fields[j]->FieldName.UpperCase();
                valore = ADOQuery->FieldList->Fields[j]->AsString;
                RecList[chiave][campo] = valore;
            }
            ADOQuery->Next();
            i++;
        }
        ADOQuery->Close();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

void __fastcall TdmDB::TimerConnectTimer(TObject *Sender) {
    int i;
    TimerConnect->Enabled = false;
    if (ADOConnection1->Connected) {
        // if (!verificaConnessioneDB())
        // DisconnettiDB();
        if (!lettostatoagv) {
            LeggiStatoAgv();
            LeggiParametri();
            // MainForm->cbNuovaGestione->Checked = ClientData.nuova_gestione;
            AggiornaTabPostazioni();
            AggiornaNomePiani();
            AggiornaTabPorte();
            // dmDB->AggiornaTabTipoArticoli();
            // AggiornaTabAnagrafica();
            dmDB->AggiornaTabTipologiaCorsia();
            dmDB->AggiornaTabTipoUdc();
            dmDB->AggiornaStrutturaZone();
            CaricaTabellaK("AllarmiMessageBar order by IdMessaggio", "IdMessaggio", TabMessaggiAllarme);
            // ABILITARE SOLO CON EMAIL. CI VORREBBE LA GESTIONE A PRAMETRI....
            // dmExtraFunction->CheckAlarm();

        }

    }
    else {
        ConnettiDB();
    }
    TimerConnect->Enabled = true;
}
// ---------------------------------------------------------------------------

void TdmDB::ConnettiDB() {
    int m, p;

    try {
        ADOConnection1->Close();
        ADOConnection1->Open();
        LogTxt("Database Locale connected");
        // LeggiParametri(); // leggo parametri all'avvio
    }
    catch (...) {
        ADOConnection1->Connected = false;
    }
}
// ---------------------------------------------------------------------------

void TdmDB::DisconnettiDB() {
    ADOConnection1->Close();
    ADOConnection1->Connected = false;
    LogTxt("Database Locale disconnected");
}

// ---------------------------------------------------------------------------
int TdmDB::verificaConnessioneDB() {
    AnsiString strsql, articolo, pos;
    TADOQuery *ADOQuery;
    int res = 0;

    if (!ADOConnection1->Connected)
        return 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql = "SELECT * FROM INFORMATION_SCHEMA.TABLES";
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Close();
        res = 1;
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}
// ---------------------------------------------------------------------------

int TdmDB::LeggiCampoTabella(AnsiString Tabella, AnsiString CampoDaLeggere, AnsiString ClauslaWhere, AnsiString &Valore) {
    AnsiString strsql;

    TADOQuery *ADOQuery;

    int res = 0;

    if (!ADOConnection1->Connected)

        return res;
    Valore = ""; // Inizializzazione valore di ritorno

    // Creazione stringa Query

    strsql.printf("SELECT %s FROM %s %s", CampoDaLeggere.c_str(), Tabella.c_str(), ClauslaWhere.c_str());

    try {

        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        // Verifica risultati
        if (!ADOQuery->Eof) {
            Valore = ADOQuery->FieldByName(CampoDaLeggere)->AsString;
        }
    }
    catch (Exception &E) {
        // return E
        ADOConnection1->Close();
        res = -1;
    }

    delete ADOQuery;

    return res;

}

// ---------------------------------------------------------------------------

void TdmDB::AcquisisciSegnalazioneAttiva(int nmsg) {
    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("UPDATE Segnalazioni SET Acquisito = GetDate() WHERE (N = '%d') AND Acquisito IS NULL", nmsg);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

void TdmDB::AcquisisciSegnalazioneAttiva() {
    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT TOP 1 * FROM Segnalazioni WHERE Acquisito IS NULL ORDER BY DataOra");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        if (!ADOQuery->Eof) {
            ADOQuery->Edit();
            ADOQuery->FieldByName("Acquisito")->AsDateTime = Now();
            ADOQuery->Post();
        }
        ADOQuery->Close();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

void TdmDB::LeggiSegnalazioneAttiva(AnsiString &msg, int &all) {

    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected)
            return;
        msg = "";
        all = 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT TOP 1 * FROM Segnalazioni WHERE Acquisito IS NULL ORDER BY DataOra");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        if (!ADOQuery->Eof) {
            msg = ReadString(ADOQuery, "Messaggio");
            all = ReadInt(ADOQuery, "Allarme");
        }
        ADOQuery->Close();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

void TdmDB::AcquisisciSegnalazioni() {
    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("UPDATE Segnalazioni SET Acquisito = GetDate() WHERE Acquisito IS NULL");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

void TdmDB::Segnalazione(int nall, AnsiString msg, int all, int popup, int popupstation) {
    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        // strsql.printf("SELECT TOP (1) * FROM Segnalazioni WHERE (Messaggio = '%s') AND ((Acquisito IS NULL) OR (GETDATE() - Acquisito < 1.0 / 24.0 / 20.0))", msg);
        strsql.printf("SELECT TOP (1) * FROM Segnalazioni WHERE (Messaggio = '%s') AND (Acquisito IS NULL)", msg);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        if (!ADOQuery->Eof) {
            // Messaggio già inviato
            ADOQuery->Close();
            delete ADOQuery;
            return;
        }
        ADOQuery->Close();
        strsql.printf("INSERT INTO Segnalazioni (N, Messaggio, Allarme, Popup, StationPopup) VALUES (%d, '%s', %d, %d, %d)", nall, msg.c_str(), all, popup,
            popupstation);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

void TdmDB::Log(AnsiString Sorgente, AnsiString Tipo, AnsiString Evento) {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;

    try {
        if (!ADOConnection1->Connected)
            return;
        if (Evento.Length() > 200)
            Evento.SetLength(200);
        while (Evento.Pos("'"))
            Evento[Evento.Pos("'")] = '*';
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT TOP (1) * FROM Storico ORDER BY DataOra DESC");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        if (!ADOQuery->Eof) {
            // Messaggio già loggato
            ev = ReadString(ADOQuery, "Evento");
            if (ev == Evento) {
                ADOQuery->Close();
                delete ADOQuery;
                return;
            }
        }
        ADOQuery->Close();
        strsql.printf("INSERT INTO Storico (Sorgente, Tipo, Evento) VALUES ('%s', '%s', '%s')", Sorgente, Tipo, Evento.c_str());
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
    }
    catch (...) {
    }
    delete ADOQuery;
}

void TdmDB::LogMsg(AnsiString Descrizione) {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev, oradel, Statostr, Deststr, Misstr, UDCstr;
    Word Year, Month, Day;
    int nagv;
    // oradel = DateToStr(Now() - 30) ;
    if (!ADOConnection1->Connected)
        return;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        ADOQuery->Close();
        if (Descrizione.Length() > 400)
            Descrizione.SetLength(400);
        while (Descrizione.Pos("'"))
            Descrizione[Descrizione.Pos("'")] = '*';
        Statostr = "Pos : ";
        Deststr = "Dest : ";
        Misstr = "IdMis : ";
        UDCstr = "UDC : ";
        for (nagv = 1; nagv <= NAGV; nagv++) {
            Statostr = Statostr + IntToStr(ClientData.DatiAgv[nagv].pos) + " ; ";
            Deststr = Deststr + IntToStr(ClientData.DatiAgv[nagv].dest) + " ; ";
            Misstr = Misstr + IntToStr(ClientData.DatiAgv[nagv].idmis) + " ; ";
            // Totemstr = Totemstr + IntToStr(ClientData.DatiAgv[nagv].idUDC) + " ; ";
        }
        Statostr = Statostr.SubString(1, 70);
        Deststr = Deststr.SubString(1, 70);
        Misstr = Misstr.SubString(1, 70);
        strsql.printf
            ("Insert Into Log ( DataOra , Descrizione , UserName ,PosizioniAgv,DestAgv ,IdMissioneAgv,UDCAgv,Sorgente )"
            " VALUES (GetDate(),'%s','%s','%s','%s','%s','%s','%s')", Descrizione.c_str(), ActualUser.c_str(), Statostr.c_str(), Deststr.c_str(),
            Misstr.c_str(), UDCstr.c_str(), NomeClientServer.c_str());
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
    }
    catch (...) {
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

void TdmDB::LogCom(AnsiString Telegramma) {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;

    try {
        if ((!ADOConnection1->Connected) || (!log_comunicazioni))
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("INSERT INTO LOG_COMUNICAZIONE " "(Origine, Destinazione, Tipo, Telegramma) " "VALUES ('%s','%s','%s','%s')", Telegramma.SubString(7, 2),
            Telegramma.SubString(9, 2), Telegramma.SubString(12, 2), Telegramma.c_str());
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
    }
    catch (...) {
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

void TdmDB::PuliziaStorici() {
    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("DELETE FROM Allarmi WHERE (LogOp = 0 AND (DataOra < (GetDate() - 30))) OR (LogOp = 1 AND (DataOra < (GetDate() - 3)))");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
        strsql.printf("DELETE FROM Missioni WHERE (Generata < (GetDate() - 30))");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
        strsql.printf("DELETE FROM Log WHERE (DataOra < (GetDate() - 2))");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
        dmDB->LogMsg("Pulizia storici");
    }
    catch (...) {
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

void __fastcall TdmDB::ADOConnection1AfterConnect(TObject *Sender) {
    if (ADOConnection1->Connected) {
        /* Log("SERVER", "EVENT", "Database locale connected");
         CaricaTabellaK("Postazioni WHERE SELLA IS NOT NULL AND Nodo > 0", "Sella", TabSelle);
         CaricaTabellaK("Postazioni", "Nodo", TabNodi);
         CaricaTabellaK("Postazioni", "ID_Postazione", TabPostazioni); */
        // CaricaTabellaK("Posizioni", "Pos", TabPostazioni);
        aggiorna_tab_posizioni_locale = true;
    }
}
// ---------------------------------------------------------------------------

void TdmDB::LeggiParametri(int id_parametro) {
    /*
     PARAMETRI:
     01-09 missions settings
     10-19 priorita'
     30-39 posizioni caricabatteria
     50-79 Parametri funzionali vari (peso ecc.)
     80-99 Settaggi interni (traccia, info ecc)
     */
    AnsiString strsql, strlog = "";
    TADOQuery *ADOQuery;
    int res = 0, id = 0;
    try {
        if (!ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql = "Select * from Parametri ";
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->First();
        while (!ADOQuery->Eof) {
            if ((id_parametro == 0) || (id_parametro == ADOQuery->FieldByName("Idparametro")->AsInteger)) {
                id = ADOQuery->FieldByName("IdParametro")->AsInteger;
                if (id <= 9)
                    agv_abilitati_per_missione[id] = ADOQuery->FieldByName("IntValue")->AsInteger;
                else if (id <= 19)
                    priorita_missioni[id - 10] = ADOQuery->FieldByName("IntValue")->AsInteger;
                else if (id == 30)
                    cb_statici = ADOQuery->FieldByName("IntValue")->AsInteger;
                else if (id <= 39)
                    lista_cb[id - 30] = ADOQuery->FieldByName("IntValue")->AsInteger;
                else if (id <= 79) {
                    if (id == 50)
                        PesoCampione = ADOQuery->FieldByName("IntValue")->AsInteger;
                    if (id == 51)
                        MinutiPesoCampione = ADOQuery->FieldByName("IntValue")->AsInteger;
                    if (id == 52)
                        PrioritaPesoCampione = ADOQuery->FieldByName("IntValue")->AsInteger;
                    if (id == 53)
                        TolleranzaPesoCampione = ADOQuery->FieldByName("IntValue")->AsInteger;
                    if (id == 55)
                        ClientData.ParametriFunzionali.TempoRicarica = ADOQuery->FieldByName("IntValue")->AsInteger;
                    if (id == 56)
                        ClientData.ParametriFunzionali.NumMissioniLimite = ADOQuery->FieldByName("IntValue")->AsInteger;
                    if (id == 57)
                        ClientData.ParametriFunzionali.Giorno = ADOQuery->FieldByName("IntValue")->AsInteger;
                }
                else if (id <= 99) {
                    if (id == 80)
                        abilitatracciaagv = ADOQuery->FieldByName("IntValue")->AsInteger;
                    if (id == 81)
                        abilitainfobox = ADOQuery->FieldByName("IntValue")->AsInteger;
                    if (id == 83)
                        ClientData.ParametriFunzionali.AbilitaCheckGiornoNotte = ADOQuery->FieldByName("IntValue")->AsInteger;
                    if (id == 90)
                        ClientData.ParametriFunzionali.altezza_pallet = ADOQuery->FieldByName("IntValue")->AsInteger;
                }
            }
            ADOQuery->Next();
        }
        ADOQuery->Close();
        lettoparametri = true;
        // AggiornaTabParametri();

        num_parametri = id;
    }
    catch (...) {
    }
    delete ADOQuery;
}

int TdmDB::LeggiParametro(int id_parametro) {
    AnsiString strsql, strlog = "";
    TADOQuery *ADOQuery;
    int res = 0, id = 0;
    try {
        if (!ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql = "Select intvalue from Parametri where idparametro=" + IntToStr(id_parametro);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->First();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("intvalue")->AsInteger;
        }
        ADOQuery->Close();
        lettoparametri = true;
        // AggiornaTabParametri();

        num_parametri = id;
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDB::AggiornaParametri(int id_parametro, int val, AnsiString valstr) {
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0, richiesta = 0, i, generata = 0, escludi = 0, chiamate = 0;
    AnsiString Val;

    if (!ADOConnection1->Connected)
        return 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("UPDATE Parametri SET IntValue = %d ,StringValue = '%s' WHERE IdParametro = %d", val, valstr.c_str(), id_parametro);
        ADOQuery->SQL->Text = strsql;
        res = ADOQuery->ExecSQL();
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

void TdmDB::LeggiStatoAgv() {
    AnsiString stringa, TempDataOra, allarmi, str1;
    int pos, nagv, escludi, stato;
    TADOQuery *Query2;
    char st[20];

    if (!ADOConnection1->Connected)
        return;
    try {
        Query2 = new TADOQuery(NULL);
        Query2->Connection = ADOConnection1;
        nagv = 1;
        stringa = "Select * From StatoAgv ";
        str1 = AnsiString::StringOfChar('0', 99);
        try {
            Query2->Close();
            Query2->SQL->Clear();
            Query2->SQL->Append(stringa);
            Query2->Open();
            if (Query2->RecordCount > 0) {
                Query2->First();
                while (!Query2->Eof) {
                    nagv = Query2->FieldByName("id")->AsInteger;
                    ClientData.DatiAgv[nagv].pos = Query2->FieldByName("pos")->AsInteger;
                    ClientData.DatiAgv[nagv].posprec = Query2->FieldByName("posprec")->AsInteger;
                    ClientData.DatiAgv[nagv].load = Query2->FieldByName("loaded")->AsInteger;
                    ClientData.DatiAgv[nagv].generata = Query2->FieldByName("Generata")->AsInteger;
                    ClientData.DatiAgv[nagv].tipomis = Query2->FieldByName("Tipo_Missione")->AsInteger;
                    ClientData.DatiAgv[nagv].idmis = Query2->FieldByName("idmis")->AsInteger;
                    ClientData.DatiAgv[nagv].LivelloBatteria = Query2->FieldByName("livellobatt")->AsInteger;
                    ClientData.DatiAgv[nagv].dest = Query2->FieldByName("dest")->AsInteger;
                    ClientData.DatiAgv[nagv].prel = Query2->FieldByName("prelievo")->AsInteger;
                    ClientData.DatiAgv[nagv].dep = Query2->FieldByName("deposito")->AsInteger;
                    ClientData.DatiAgv[nagv].DatiUDC.IDUDC = Query2->FieldByName("IdUDC")->AsInteger;
                    // ClientData.DatiAgv[nagv].codice_pallet = Query2->FieldByName("IdUDC")->AsInteger;
                    ClientData.DatiAgv[nagv].consensodato = Query2->FieldByName("ConsensoDato")->AsInteger;
                    ClientData.DatiAgv[nagv].bitbattbassa = Query2->FieldByName("BattBassa")->AsInteger;
                    ClientData.DatiAgv[nagv].peso_campione_non_corretto = Query2->FieldByName("Peso_Campione_Non_Ok")->AsInteger;
                    ClientData.DatiAgv[nagv].tipoAGV = Query2->FieldByName("TipoAGV")->AsInteger;
                    ClientData.DatiAgv[nagv].sospeso = Query2->FieldByName("sospeso")->AsInteger;
                    // ClientData.DatiAgv[nagv].tipopallet = Query2->FieldByName("TipoPallet")->AsInteger ;
                    if (nagv == 1) {
                        ClientData.ParametriFunzionali.AbilitaMissioni = Query2->FieldByName("Abilitazioni")->AsInteger & dmDB->bit[0];
                    }
                    stato = Query2->FieldByName("stato")->AsInteger;
                    ClientData.DatiAgv[nagv].registro5 = Query2->FieldByName("registro5")->AsInteger;
                    allarmi = TrimRight(Query2->FieldByName("Allarmi")->AsString);
                    ClientData.DatiAgv[nagv].manuale = stato & bit[0];
                    ClientData.DatiAgv[nagv].alarm = stato & bit[1];
                    ClientData.DatiAgv[nagv].semiautomatico = stato & bit[2];
                    ClientData.DatiAgv[nagv].stand_by_antincendio = stato & bit[3];
                    ClientData.DatiAgv[nagv].stand_by = ClientData.DatiAgv[nagv].registro5 & dmDB->bit[0];
                    ClientData.DatiAgv[nagv].bumper_bypass = ClientData.DatiAgv[nagv].registro5 & dmDB->bit[1];
                    ClientData.DatiAgv[nagv].forchedx = Query2->FieldByName("ForcheDX")->AsInteger;
                    ClientData.DatiAgv[nagv].forchesx = Query2->FieldByName("ForcheSX")->AsInteger;
                    ClientData.DatiAgv[nagv].latoforcheprel = Query2->FieldByName("LatoForchePrel")->AsInteger;
                    ClientData.DatiAgv[nagv].latoforchedep = Query2->FieldByName("LatoForcheDep")->AsInteger;
                    ClientData.DatiAgv[nagv].nodopassaggioprel = Query2->FieldByName("nodopassaggioprel")->AsInteger;
                    ClientData.DatiAgv[nagv].nodopassaggiodep = Query2->FieldByName("nodopassaggiodep")->AsInteger;
                    ClientData.DatiAgv[nagv].fermaagv = Query2->FieldByName("FermaAGV")->AsInteger;
                    ClientData.DatiAgv[nagv].standby_operatore = Query2->FieldByName("StandByOperatore")->AsInteger;

                    strcpy(ClientData.DatiAgv[nagv].allarme, allarmi.c_str());
                    strcpy(ClientData.DatiAgv[nagv].allarmiPrec, str1.c_str());
                    TempDataOra = Query2->FieldByName("UltimaMissione")->AsString;
                    // strcpy(ClientData.DatiAgv[nagv].DataOraUltimaMissione, TempDataOra.c_str());
                    Query2->Next();
                }
            }
            Query2->Close();
            dmDB->GestioneAllarmi(0);
            lettostatoagv = 1;
            // MainForm->DatiPlc[1].CodicePallet = ReturnCodiceDaIdArticolo(MainForm->DatiPlc[1].idarticolo,MainForm->DatiPlc[1].Lotto,MainForm->DatiPlc[1].h_pallet) ;
            LogMsg("Avvio Server Supervisione");
        }
        catch (...) {
            LogMsg("Eccezione su leggi stato agv");
            Query2->Close();
        }
    }
    catch (...) {
    }
    delete Query2;
}

void TdmDB::AggiornaStatoAgv(int agv) {
    AnsiString stringa, allarmi, idpal;
    int pos, idmis, livello, i, j, escludi, stato, statoplc = 0, abilitazioni = 0;
    TADOQuery *Query2;
    int res = 0;

    if ((!ADOConnection1->Connected) || (!lettostatoagv))
        return;
    try {
        Query2 = new TADOQuery(NULL);
        Query2->Connection = ADOConnection1;
        /* if (MainForm->AbilitaMissioni)
         abilitazioni |= MainForm->bit[0] ;
         if (MainForm->escludi_pioggia)
         abilitazioni |= bit[1] ;
         if (MainForm->forza_sel_pioggia)
         abilitazioni |= bit[2] ; */
        // abilitazioni |= MainForm->DatiPlc[1].escludiPorte ;
        for (i = 1; i <= NAGV; i++) {
            if ((i == agv) || (agv == 0)) {
                pos = ClientData.DatiAgv[i].pos;
                // idmis = ClientData.DatiAgv[i].idmis ;
                livello = ClientData.DatiAgv[i].LivelloBatteria;
                allarmi = ClientData.DatiAgv[i].allarme; // salvo allarme cos' uando riavvio rimetto in barra gli allarmi agv
                stato = 0;
                if (ClientData.DatiAgv[i].manuale)
                    stato |= bit[0];
                if (ClientData.DatiAgv[i].alarm)
                    stato |= bit[1];
                if (ClientData.DatiAgv[i].semiautomatico)
                    stato |= bit[2];
                if (ClientData.DatiAgv[i].stand_by_antincendio)
                    stato |= bit[3];
                if (allarmi.Length() > 100)
                    allarmi.SetLength(100);
                // ora = DataOraSql() ;
                stringa = " UPDATE StatoAgv set pos = " + IntToStr(pos);
                stringa = stringa + ", posprec = " + IntToStr(ClientData.DatiAgv[i].posprec) + ", prelievo = " + IntToStr(ClientData.DatiAgv[i].prel);
                stringa = stringa + ", deposito = " + IntToStr(ClientData.DatiAgv[i].dep);
                stringa = stringa + ", generata = " + IntToStr(ClientData.DatiAgv[i].generata);
                stringa = stringa + ", Tipo_Missione = " + IntToStr(ClientData.DatiAgv[i].tipomis);
                stringa = stringa + ", stato = " + IntToStr(stato);
                stringa = stringa + ", dest = " + IntToStr(ClientData.DatiAgv[i].dest);
                stringa = stringa + ", loaded = " + IntToStr(ClientData.DatiAgv[i].load) + ", livellobatt = " + IntToStr(livello);
                stringa = stringa + ", idmis = " + IntToStr(ClientData.DatiAgv[i].idmis);
                /* stringa = stringa + ", corsiadep = "+IntToStr(ClientData.DatiAgv[i].corsiadep);
                 stringa = stringa + ", pianodep = "+IntToStr(ClientData.DatiAgv[i].pianodep);
                 stringa = stringa + ", HCorsia = "+IntToStr(ClientData.DatiAgv[i].h_corsia); */
                stringa = stringa + ", registro5 = " + IntToStr(ClientData.DatiAgv[i].registro5);
                stringa = stringa + ", ConsensoDato = " + IntToStr(ClientData.DatiAgv[i].consensodato);
                stringa = stringa + ", BattBassa = " + IntToStr(ClientData.DatiAgv[i].bitbattbassa);
                stringa = stringa + ", Peso_Campione_Non_Ok = " + IntToStr(ClientData.DatiAgv[i].peso_campione_non_corretto);
                stringa = stringa + ", sospeso = " + IntToStr(ClientData.DatiAgv[i].sospeso);
                stringa = stringa + ", IdUDC = " + IntToStr(ClientData.DatiAgv[i].DatiUDC.IDUDC);
                // stringa = stringa + ", IdUDC = " + IntToStr(ClientData.DatiAgv[i].codice_pallet);
                // stringa = stringa + ", UltimaMissione = '" + String(ClientData.DatiAgv[i].DataOraUltimaMissione) + "'";
                stringa = stringa + ", ForcheDX = " + IntToStr(ClientData.DatiAgv[i].forchedx);
                stringa = stringa + ", ForcheSX = " + IntToStr(ClientData.DatiAgv[i].forchesx);
                stringa = stringa + ", latoforcheprel = " + IntToStr(ClientData.DatiAgv[i].latoforcheprel);
                stringa = stringa + ", latoforchedep = " + IntToStr(ClientData.DatiAgv[i].latoforchedep);
                stringa = stringa + ", NodoPassaggioPrel = " + IntToStr(ClientData.DatiAgv[i].nodopassaggioprel);
                stringa = stringa + ", NodoPassaggioDep = " + IntToStr(ClientData.DatiAgv[i].nodopassaggiodep);
                stringa = stringa + ", FermaAGV = " + IntToStr(ClientData.DatiAgv[i].fermaagv);
                // stringa = stringa + ", TipoPallet = "+IntToStr(ClientData.DatiAgv[i].tipopallet);
                stringa = stringa + ", StandByOperatore = " + IntToStr(ClientData.DatiAgv[i].standby_operatore);

                if (i == 1) {
                    statoplc = 0;
                    if (ClientData.ParametriFunzionali.AbilitaMissioni)
                        abilitazioni |= dmDB->bit[0];
                    stringa = stringa + ", Abilitazioni = " + IntToStr(abilitazioni);
                }
                stringa = stringa + ", Allarmi = '" + allarmi + "' where id = " + IntToStr(i);

                try {
                    Query2->Close();
                    Query2->SQL->Clear();
                    Query2->SQL->Append(stringa);
                    // MainForm->LogMsg(stringa);
                    Query2->ExecSQL();
                    Query2->Close();
                }
                catch (...) {
                    LogMsg("Eccezzione su Aggiorno stato agv");
                    Query2->Close();
                }
            }
        }
    }
    catch (...) {
    }
    delete Query2;

}

void TdmDB::GestioneAllarmi(int agv) {
    int nagv, aggiorna_allarmi = 0;

    AnsiString str1, messalarm, StringaMail, data, ora, stringa, Statostr;
    // Variant locvalues[4];
    TLocateOptions Opts;
    AnsiString oradel, stringreplace;
    Word Year, Month, Day;
    TADOQuery *QueryAllarmi, *QueryUpdate;
    str1 = AnsiString::StringOfChar('0', 99);
    int i, n, allarme_ancora_attivo;

    DecodeDate((Now() - 30), Year, Month, Day);
    oradel = IntToStr(Day) + "." + IntToStr(Month) + "." + IntToStr(Year); // il separatore per le query sulle tabelle di BDE è il . e non la barra /
    // oradel = DateToStr(Now() - 30) ;
    if (ADOConnection1->Connected) {
        try {
            QueryAllarmi = new TADOQuery(NULL);
            QueryAllarmi->Connection = ADOConnection1;
            QueryUpdate = new TADOQuery(NULL);
            QueryUpdate->Connection = ADOConnection1;
            /* QueryAllarmi->Close();
             QueryAllarmi->SQL->Clear();
             stringa.printf("delete from Allarmi where LogOp = 0 AND DataOra < '%s'",+oradel.c_str());
             QueryAllarmi->SQL->Append(stringa);
             QueryAllarmi->ExecSQL(); */
            QueryAllarmi->Close();
            for (nagv = 1; nagv <= NAGV; nagv++) {
                Statostr += "Agv " + IntToStr(nagv) + " in pos " + IntToStr(ClientData.DatiAgv[nagv].pos) + " dest " + IntToStr(ClientData.DatiAgv[nagv].dest)
                    + " ; ";
            }

            for (nagv = 1; nagv <= NAGV; nagv++) {
                allarme_ancora_attivo = 0;
                if ((nagv == agv) || (agv == 0)) {
                    if (strcmp(ClientData.DatiAgv[nagv].allarme, "NONE")) { // se ho allarme tara devo entrare qua lo stesso
                        strcpy(ClientData.DatiAgv[nagv].allarmi, str1.c_str());
                        ClientData.DatiAgv[nagv].allarmi[99] = '0';
                        if (strcmp(ClientData.DatiAgv[nagv].allarme, "NONE")) { // se non ho allarmi lascio la stringa tutta a 0
                            for (i = 0; i < strlen(ClientData.DatiAgv[nagv].allarme); i += 2) {
                                sscanf(ClientData.DatiAgv[nagv].allarme + i, "%2X", &n);
                                if ((n > 0) && (n < 100)) {
                                    ClientData.DatiAgv[nagv].allarmi[n] = '1';
                                }
                            }
                        }
                        for (n = 0; n < 100; n++) {
                            if ((ClientData.DatiAgv[nagv].allarmiPrec[n] == '0') &&
                                (ClientData.DatiAgv[nagv].allarmiPrec[n] != ClientData.DatiAgv[nagv].allarmi[n])) {
                                if ((n != 95) && (!ClientData.DatiAgv[nagv].manuale)) { // discrimino cb difettoso e allarmi manuali
                                    if (n < 10)
                                        messalarm = IntToStr(nagv) + "0" + IntToStr(n);
                                    else
                                        messalarm = IntToStr(nagv) + IntToStr(n);
                                    // controllo che l'allare precedente sia diverso da questo
                                    if (ClientData.DatiAgv[nagv].idmis > 0) {
                                        stringa = "Select * From Allarmi where Nallarme = '" + IntToStr(n) + "' AND Posizione =" +
                                            IntToStr(ClientData.DatiAgv[nagv].pos);
                                        stringa += " AND Missione =" + IntToStr(ClientData.DatiAgv[nagv].idmis);
                                    }
                                    else {
                                        DecodeDate((Now()), Year, Month, Day);
                                        data = IntToStr(Day) + "." + IntToStr(Month) + "." + IntToStr(Year);
                                        // il separatore per le query sulle tabelle di BDE è il . e non la barra /
                                        ora = TimeToStr(Time() - StrToTime("00:05:00"));
                                        stringa = "Select * From Allarmi where Nallarme = '" + IntToStr(n) + "' AND Posizione =" +
                                            IntToStr(ClientData.DatiAgv[nagv].pos);
                                        stringa += " AND Destinazione =" + IntToStr(ClientData.DatiAgv[nagv].dest) + " AND DataOra > '" + data + " " +
                                            ora + "'";
                                    }
                                    MainForm->MessageBar1->Messages->Strings[StrToInt(messalarm)] =
                                        "Agv " + IntToStr(nagv) + " in Alarm : " + MainForm->MessageBar1->Messages->Strings[n];
                                    if ((lettostatoagv) && (n != 23)) { // non salvo lo stand by
                                        try {
                                            QueryAllarmi->Close();
                                            QueryAllarmi->SQL->Clear();
                                            QueryAllarmi->SQL->Append(stringa);
                                            QueryAllarmi->Open();
                                            if (!QueryAllarmi->RecordCount) {
                                                QueryAllarmi->Close();
                                                QueryAllarmi->SQL->Clear();
                                                stringreplace = StringReplace(MainForm->MessageBar1->Messages->Strings[StrToInt(messalarm)], "'", "*", TReplaceFlags() << rfReplaceAll);
                                                stringa = "Insert Into Allarmi ( DataOra , Descrizione , Posizione,PosPrec ,Destinazione,Missione,Agv,NAllarme,Ripristinato,StatoAgv,LogOp )";
                                                stringa = stringa + " VALUES (GetDate(),'" + stringreplace + "'," +
                                                    IntToStr(ClientData.DatiAgv[nagv].pos) + "," + IntToStr(ClientData.DatiAgv[nagv].posprec) + "," +
                                                    IntToStr(ClientData.DatiAgv[nagv].dest) + "," + IntToStr(ClientData.DatiAgv[nagv].idmis) + ",'" + IntToStr(nagv) +
                                                    "','" + IntToStr(n) + "',0,'" + Statostr + "',0)";
                                                QueryAllarmi->SQL->Append(stringa);
                                                QueryAllarmi->ExecSQL();
                                            }
                                            QueryAllarmi->Close();
                                        }
                                        catch (...) {
                                            LogMsg("Exception Insert Alarm");
                                            QueryAllarmi->Close();
                                        }
                                    }

                                    ClientData.DatiAgv[nagv].allarmiPrec[n] = ClientData.DatiAgv[nagv].allarmi[n];
                                    if (n != 23) { // non imposto allarme su cb non funzionante
                                        ClientData.DatiAgv[nagv].alarm = true;
                                        allarme_ancora_attivo = 1;
                                    }
                                    MainForm->MessageBar1->AddMsgNo(StrToInt(messalarm));
                                    aggiorna_allarmi = 1;
                                }
                            }
                            else if ((ClientData.DatiAgv[nagv].allarmiPrec[n] == '1') &&
                                (ClientData.DatiAgv[nagv].allarmiPrec[n] != ClientData.DatiAgv[nagv].allarmi[n])) {
                                if (n < 10)
                                    messalarm = IntToStr(nagv) + "0" + IntToStr(n);
                                else
                                    messalarm = IntToStr(nagv) + IntToStr(n);
                                ClientData.DatiAgv[nagv].allarmiPrec[n] = ClientData.DatiAgv[nagv].allarmi[n];
                                MainForm->MessageBar1->DelMsgNo(StrToInt(messalarm));
                                aggiorna_allarmi = 1;
                                QueryAllarmi->Close();
                                QueryAllarmi->SQL->Clear();
                                try {
                                    // UPDATE    Alarms   SET Time = CONVERT(varchar, GETDATE() - DateTime, 108)  //108 è il parametro per hh:mm:ss
                                    stringa = "Select * From Allarmi where NAllarme = '" + IntToStr(n) + "' AND Agv = '" + IntToStr(nagv) +
                                        "' AND Tempo IS NULL ";
                                    QueryAllarmi->SQL->Append(stringa);
                                    QueryAllarmi->Open();
                                    if (QueryAllarmi->RecordCount) {
                                        while (!QueryAllarmi->Eof) {
                                            // stringa = stringa +" VALUES (GetDate(),'"+MainForm->MessageBar1->Messages->Strings[StrToInt(messalarm)]+"',"+IntToStr(ClientData.DatiAgv[nagv].pos)+","+IntToStr(ClientData.DatiAgv[nagv].posprec)+","+IntToStr(ClientData.DatiAgv[nagv].dest)+","+IntToStr(ClientData.DatiAgv[nagv].idmis)+",'"+IntToStr(nagv)+"','"+IntToStr(n)+"',0)";
                                            ora = ReturnTimeDaDateTime(DateTimeToStr(QueryAllarmi->FieldByName("DataOra")->AsDateTime));
                                            stringa = "Update Allarmi Set DataOraRipristino = GetDate() , Tempo = '" + ora + "' , Ripristinato = ";
                                            if (ClientData.DatiAgv[nagv].manuale)
                                                stringa = stringa + " 1 ";
                                            else if (ClientData.DatiAgv[nagv].bumper_bypass)
                                                stringa = stringa + " 3 ";
                                            else
                                                stringa = stringa + " 2 ";
                                            stringa = stringa + " where NAllarme = '" + IntToStr(n) + "' AND Agv = '" + IntToStr(nagv) + "' AND Tempo IS NULL ";
                                            QueryUpdate->Close();
                                            QueryUpdate->SQL->Clear();
                                            QueryUpdate->SQL->Append(stringa);
                                            QueryUpdate->ExecSQL();
                                            QueryUpdate->Close();
                                            QueryAllarmi->Next();
                                        }
                                    }
                                    // stringa = stringa +" where NAllarme = '"+IntToStr(i)+"' AND Agv = '"+IntToStr(nagv)+"' AND Tempo IS NULL ";
                                    // QueryAllarmi->SQL->Append(stringa) ;
                                    // QueryAllarmi->ExecSQL();
                                    QueryAllarmi->Close();
                                }
                                catch (...) {
                                    LogMsg("Error Save Restarted Alarm 1 ");
                                    QueryAllarmi->Close();
                                }
                            }
                            else if ((n != 23) && (ClientData.DatiAgv[nagv].allarmiPrec[n] == '1')) {
                                allarme_ancora_attivo = 1;
                                // discrimino allarme stand_by, se vado in stand_by e ho un altro allarme, uando va via l'altro mi rimane in stand by ma la vaiabile alarm è altma
                            }
                        }
                        if ((!allarme_ancora_attivo) && (ClientData.DatiAgv[nagv].alarm)) {
                            ClientData.DatiAgv[nagv].alarm = false;
                        }
                    }
                    else {
                        if (strcmp(ClientData.DatiAgv[nagv].allarmiPrec, str1.c_str())) {
                            for (i = 0; i < 100; i++) {
                                if (ClientData.DatiAgv[nagv].allarmiPrec[i] == '1') {
                                    if (i < 10)
                                        messalarm = IntToStr(nagv) + "0" + IntToStr(i);
                                    else
                                        messalarm = IntToStr(nagv) + IntToStr(i);
                                    MainForm->MessageBar1->DelMsgNo(StrToInt(messalarm));
                                    QueryAllarmi->Close();
                                    QueryAllarmi->SQL->Clear();
                                    aggiorna_allarmi = 1;
                                    try {
                                        stringa = "Select * From Allarmi where NAllarme = '" + IntToStr(i) + "' AND Agv = '" + IntToStr(nagv) +
                                            "' AND Tempo IS NULL ";
                                        QueryAllarmi->SQL->Append(stringa);
                                        QueryAllarmi->Open();
                                        if (QueryAllarmi->RecordCount) {
                                            while (!QueryAllarmi->Eof) {
                                                // stringa = stringa +" VALUES (GetDate(),'"+MainForm->MessageBar1->Messages->Strings[StrToInt(messalarm)]+"',"+IntToStr(ClientData.DatiAgv[nagv].pos)+","+IntToStr(ClientData.DatiAgv[nagv].posprec)+","+IntToStr(ClientData.DatiAgv[nagv].dest)+","+IntToStr(ClientData.DatiAgv[nagv].idmis)+",'"+IntToStr(nagv)+"','"+IntToStr(n)+"',0)";
                                                ora = ReturnTimeDaDateTime(DateTimeToStr(QueryAllarmi->FieldByName("DataOra")->AsDateTime));
                                                stringa = "Update Allarmi Set DataOraRipristino = GetDate() , Tempo = '" + ora + "' , Ripristinato = ";
                                                if (ClientData.DatiAgv[nagv].manuale)
                                                    stringa = stringa + " 1 ";
                                                else if (ClientData.DatiAgv[nagv].bumper_bypass)
                                                    stringa = stringa + " 3 ";
                                                else
                                                    stringa = stringa + " 2 ";
                                                stringa = stringa + " where NAllarme = '" + IntToStr(i) + "' AND Agv = '" + IntToStr(nagv) + "' AND Tempo IS NULL ";
                                                QueryUpdate->Close();
                                                QueryUpdate->SQL->Clear();
                                                QueryUpdate->SQL->Append(stringa);
                                                QueryUpdate->ExecSQL();
                                                QueryUpdate->Close();
                                                QueryAllarmi->Next();
                                            }
                                        }
                                        // stringa = stringa +" where NAllarme = '"+IntToStr(i)+"' AND Agv = '"+IntToStr(nagv)+"' AND Tempo IS NULL ";
                                        // QueryAllarmi->SQL->Append(stringa) ;
                                        // QueryAllarmi->ExecSQL();
                                        QueryAllarmi->Close();
                                    }
                                    catch (...) {
                                        LogMsg("Error Save Restarted Alarm 2 ");
                                        QueryAllarmi->Close();
                                    }
                                }
                            }
                            /* if ((ClientData.DatiAgv[nagv].alarm)&&(program == 1))
                             LogMsg("Agv "+IntToStr(nagv) + " non in allarme "); */
                        }
                        strcpy(ClientData.DatiAgv[nagv].allarmi, str1.c_str());
                        strcpy(ClientData.DatiAgv[nagv].allarmiPrec, str1.c_str());
                        ClientData.DatiAgv[nagv].allarmi[99] = '0';
                        ClientData.DatiAgv[nagv].allarmiPrec[99] = '0';
                        ClientData.DatiAgv[nagv].alarm = false;

                    }
                }
            }
        }
        catch (...) {
        }
        delete QueryAllarmi;
        delete QueryUpdate;
        if (aggiorna_allarmi)
            AggiornaAllarmiAttivi();
    }
    // InvioMail(ListaAllarmi->Text);
}

void TdmDB::AggiornaAllarmiAttivi() {

    AnsiString strsql, numalarm, descalarm, stringreplace;
    if (ADOConnection1->Connected) {
        QueryMessAlarm->Close();
        QueryMessAlarm->SQL->Clear();
        QueryMessAlarm->SQL->Append("Delete ActiveAlarm");
        QueryMessAlarm->ExecSQL();
        QueryMessAlarm->Close();
        for (int i = 0; i < MainForm->MessageBar1->ActMsg->Count; i++) {
            numalarm = MainForm->MessageBar1->ActMsg->Strings[i];
            descalarm = MainForm->MessageBar1->Messages->Strings[StrToInt(numalarm)];
            QueryMessAlarm->Close();
            QueryMessAlarm->SQL->Clear();
            stringreplace = StringReplace(descalarm, "'", "*", TReplaceFlags() << rfReplaceAll);
            strsql.printf("INSERT INTO ActiveAlarm(Description, AlarmNumber, Active) VALUES('%s', % d, % d)", stringreplace, numalarm.ToIntDef(0), 1);
            QueryMessAlarm->SQL->Append(strsql);
            QueryMessAlarm->ExecSQL();
            QueryMessAlarm->Close();
        }
        ClientData.aggiorna_allarmi = 1;
    }

}

void TdmDB::ImpostaAllarmiAttiviClient() {

    AnsiString stringa, numalarm, descalarm;
    if (ADOConnection1->Connected) {
        MainForm->MessageBar1->Clear();
        /* in olli lo scorro, vediamo se qua va bene la clear
         if (MainForm->MessageBar1->ActMsg->Count > 0 ) {
         while (MainForm->MessageBar1->ActMsg->Count > 0) {
         numalarm = MainForm->MessageBar1->ActMsg->Strings[0] ;
         MainForm->MessageBar1->DelMsgNo(StrToInt(numalarm)) ;
         }
         } */

        QueryMessAlarm->Close();
        QueryMessAlarm->SQL->Clear();
        QueryMessAlarm->SQL->Append("Select * from ActiveAlarm where Active = 1");
        QueryMessAlarm->Open();
        if (QueryMessAlarm->RecordCount > 0) { // ritorno se il telaio deve essere pesato
            QueryMessAlarm->First();
            while (!QueryMessAlarm->Eof) {
                if (QueryMessAlarm->FieldByName("AlarmNumber")->AsInteger > 0) {
                    MainForm->MessageBar1->Messages->Strings[QueryMessAlarm->FieldByName("AlarmNumber")->AsInteger] =
                        QueryMessAlarm->FieldByName("Description")->AsString;
                    MainForm->MessageBar1->AddMsgNo(QueryMessAlarm->FieldByName("AlarmNumber")->AsInteger);
                }
                else {
                    // LogMsg("Ricevuto allarme sbagliato n ° "+QueryMessAlarm->FieldByName("AlarmNumber")->AsString+" su client");
                }
                QueryMessAlarm->Next();
            }
        }
        QueryMessAlarm->Close();
    }
}

int TdmDB::RicercaUltimaMissioneInCorsoDaPosPrelAPosDep(int posprel, int posdep) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = " Select * from Missioni where PosPrel = " + IntToStr(posprel) + " AND PosDep = " + IntToStr(posdep) + " order by Generata Desc ";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            ADOQuery->First();
            if (ADOQuery->FieldByName("prelevato")->AsString == "") {
                res = ADOQuery->FieldByName("Id")->AsInteger;
            }
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {

    }
    delete ADOQuery;
    return res;
}

int TdmDB::AggiornaDestinazioneMissione(int id, int pos_dep) {
    AnsiString strsql, time;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql = "UPDATE Missioni SET posdep = " + IntToStr(pos_dep);
        strsql += " where id = " + IntToStr(id);
        ADOQuery->SQL->Text = strsql;
        res = ADOQuery->ExecSQL();
        dmDB->LogMsg(strsql + ", result : " + IntToStr(res));
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

AnsiString TdmDB::ReturnTimeMission(int idmis) {
    AnsiString stringa, time = " 00 : 00 : 00 ", result;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa = "Select * from missioni where Id = " + IntToStr(idmis);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            if (ADOQuery->FieldByName("Iniziata")->AsString != "")
                time = TimeToStr(Now() - ADOQuery->FieldByName("Iniziata")->AsDateTime);
            else
                time = " 00 : 00 : 00 ";
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    // result = FormatDateTime(format_ora_database, StrToDateTime(time));
    result = time;
    // se serve formattare data ora result = FormattaDataOra(time) ;
    delete ADOQuery;
    return result;
}

AnsiString TdmDB::FormattaDataOra(TDateTime DataOra) {
    AnsiString result;
    try {
        result = FormatDateTime(format_data_ora_database, StrToDateTime(DataOra));
    }
    catch (...) {
        result = "00 : 00 : 00";
    }
    return result;
}

AnsiString TdmDB::ReturnTimeDaDateTime(AnsiString DataOra) {
    AnsiString stringa, time = "00 : 00 : 00";
    AnsiString strsql;
    int res = 0;

    try {
        time = TimeToStr(Now() - StrToDateTime(DataOra));
    }
    catch (...) {
        time = "00 : 00 : 00";
    }
    return time;
}

int TdmDB::ReturnFilaDaPos(int pos) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select fila, pos from Posizioni where Pos = " + IntToStr(pos);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("fila")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

AnsiString TdmDB::ReturnNomeCorsiaDaPos(int pos) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    AnsiString res = "";

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select nomecorsia from Posizioni where Pos = " + IntToStr(pos);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("nomecorsia")->AsString;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

AnsiString TdmDB::ReturnZonaDaPos(int pos) {
    AnsiString stringa, res = " ";
    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select Zona from Posizioni where Pos = " + IntToStr(pos);

        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("Zona")->AsString;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDB::ReturnCorsiaDaDestraDaPos(int pos) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = " Select CorsiaInizioDx from Corsie where Fila = " + IntToStr(FilaPosizione(pos));

        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("CorsiaInizioDx")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

AnsiString TdmDB::ReturnNomePosDaPos(int pos, AnsiString & nomecorsia) {
    AnsiString stringa, res = " ";
    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        nomecorsia = " ";
        stringa = " Select CustomerPos,NomeCorsia from Posizioni where Pos = " + IntToStr(pos);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("CustomerPos")->AsString;
            nomecorsia = ADOQuery->FieldByName("NomeCorsia")->AsString;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDB::ReturnCorsiaDaNomeCorsia(AnsiString nomecorsia) {
    AnsiString stringa;
    AnsiString strsql;
    int res = 0;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = " Select fila from corsie where LTrim(RTrim(Nomecorsia)) = '" + Trim(nomecorsia) + "'";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("fila")->AsInteger;

        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDB::ReturnPosDaNomePosinPosizioni(AnsiString Nomepos) {
    AnsiString stringa;
    AnsiString strsql;
    int res = 0;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa = " Select pos,NomeCorsia from Posizioni where LTrim(RTrim(CustomerPos)) = '" + Trim(Nomepos) + "' ";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("Pos")->AsInteger;

        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDB::RitornaPosPianoDaNomePos(AnsiString Nomepos, int &piano) {
    AnsiString stringa;
    AnsiString strsql;
    int res = 0;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa = " Select pos, piano from piani where LTrim(RTrim(NomePos)) = '" + Trim(Nomepos) + "' ";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("Pos")->AsInteger;
            piano = ADOQuery->FieldByName("piano")->AsInteger;

        }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDB::ReturnPianoDaNomePosinPiani(AnsiString Nomepos) {
    AnsiString stringa;
    AnsiString strsql;
    int res = 1;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa = "Select piano from piani where LTrim(RTrim(NomePos)) = '" + Trim(Nomepos) + "' ";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("piano")->AsInteger;
        }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

AnsiString TdmDB::ReturnNomePosDaPosePiano(int pos, int piano) {
    AnsiString stringa;
    AnsiString strsql, res = "";
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa = " Select NomePos from piani where Pos = " + IntToStr(pos) + " and Piano = " + IntToStr(piano);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("NomePos")->AsString;

        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

AnsiString TdmDB::ReturnUdcDaPosePiano(int pos, int piano) {
    AnsiString stringa;
    AnsiString strsql, res = "";
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa = "Select CodUdc from piani_view where Pos = " + IntToStr(pos) + " and Piano = " + IntToStr(piano);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = Trim(ADOQuery->FieldByName("CodUdc")->AsString);
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

void TdmDB::SvuotaCorsia(int corsia) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = " UPDATE Corsie SET Piena = 0, InUsoPrelievo = 0, InUsoDeposito = 0, "; // vedere se azzerare il codice o no CodiceArticolo ='', ";
        stringa += "Vuota = 1"; // , Alveoli_Massimi = "+IntToStr(alveolitot);
        stringa += " where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();

        dmDB->LogMsg("Svuotata Corsia " + IntToStr(corsia));
        stringa = "UPDATE piani SET IdUDC = 0";
        stringa += " where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        delete ADOQuery;

        aggiorna_tab_posizioni_locale = true;
        dmDB->LogMsg("Svuoto corsia " + String(corsia) + " e piani relativi");
    }
    catch (...) {
        dmDB->LogMsg("Problema nello svuotare la corsia: " + stringa);
    }

}

void TdmDB::ImpostaCodiceArticoloCorsia(int corsia, AnsiString Codice) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE Corsie SET CodiceArticolo = '" + Codice + "' , InUsoDeposito = 1 ";
        stringa += " where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Modificato Codice su corsia  " + IntToStr(corsia) + " , Codice " + Codice);
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }

}

void TdmDB::SvuotaPos(int pos) {
    AnsiString stringa, CodiceUdc;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected)
            return;

        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE piani SET IDUDC = 0";
        stringa += " where pos = " + IntToStr(pos);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Svuotata Posizione " + IntToStr(pos));
        delete ADOQuery;
        // pulisco rifordine se i piani stanno a zero
        SettaRifOrdineSuPos(pos, "");
        /*
         if (FilaPosizione(pos)) {
         SettaCorsiaPiena(FilaPosizione(pos), 0); // resetto che è piena
         }
         */
        aggiorna_tab_posizioni_locale = true;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        dmDB->LogMsg("Problemi per eliminazione UDC da posizione= " + String(pos));
        dmDB->QueryPos->Close();
    }
}

void TdmDB::SvuotaPiano(int pos, int piano) {
    AnsiString stringa, CodiceUdc;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected)
            return;

        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE piani SET IDUDC = 0";
        stringa += " where pos = " + IntToStr(pos) + " and piano= " + IntToStr(piano);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Svuotato piano " + IntToStr(piano) + " in posizione " + IntToStr(pos));
        delete ADOQuery;
        // pulisco rifordine se i piani stanno a zero
        SettaRifOrdineSuPos(pos, "");
        aggiorna_tab_posizioni_locale = true;
    }
    catch (...) {
        dmDB->LogMsg("Problemi per eliminazione UDC da posizione= " + String(pos));
        dmDB->QueryPos->Close();
    }
}

void TdmDB::EliminaUDCdaPosizione(int UDC) {
    AnsiString stringa, CodiceUdc;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected)
            return;

        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE piani SET IDUDC = 0";
        stringa += " where IDUDC = " + IntToStr(UDC);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Eliminato da Posizione UDC" + IntToStr(UDC));
        delete ADOQuery;
        /*
         if (FilaPosizione(pos)) {
         SettaCorsiaPiena(FilaPosizione(pos), 0); // resetto che è piena
         }
         */
        aggiorna_tab_posizioni_locale = true;
    }
    catch (...) {
        dmDB->LogMsg("Problemi per eliminazione UDC " + String(UDC));
        dmDB->QueryPos->Close();
    }
}

void TdmDB::InsertUDCInPosizione(int UDC, int pos) {
    AnsiString stringa, CodiceUdc;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected)
            return;

        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE piani SET IDUDC = " + IntToStr(UDC);
        stringa += " where pos = " + IntToStr(pos) + " and piano=1";
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        // dmDB->LogMsg("Inserito UDC ID " + IntToStr(UDC) + " in Posizione " + IntToStr(pos));
        delete ADOQuery;
        aggiorna_tab_posizioni_locale = true;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
}

void TdmDB::InsertArticoloPosizione(int idarticolo, int idartudc) {
    AnsiString stringa, CodiceUdc;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected)
            return;

        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE ArticoliUDC SET IDArticolo = " + IntToStr(idarticolo);
        stringa += " where idartudc = " + IntToStr(idartudc);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Inserito idarticolo " + IntToStr(idarticolo) + " in ArtUDC " + IntToStr(idartudc));
        delete ADOQuery;
        aggiorna_tab_posizioni_locale = true;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
}

void TdmDB::PrenotaCorsia(int corsia, int prenota) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE Corsie SET CorsiaPrenotata = " + IntToStr(prenota);
        stringa += " where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Prenotata Corsia " + IntToStr(corsia) + ", prenota : " + IntToStr(prenota));
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }

}

void TdmDB::PrenotaPos(int pos, int prenota, int corsiadaprenotare) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res, pren;
    if (!ADOConnection1->Connected)
        return;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        pren = prenota;
        stringa = "UPDATE Posizioni SET Prenotata = " + IntToStr(pren);
        stringa += " where Pos = " + IntToStr(pos) + " AND Fila = " + IntToStr(corsiadaprenotare);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Prenotata Pos " + IntToStr(pos) + " fila " + IntToStr(corsiadaprenotare) + ", prenota : " + IntToStr(pren));

        /* if (corsiadaprenotare)
         PrenotaCorsia(corsiadaprenotare, prenota); */
        aggiorna_tab_posizioni_locale = true;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
}

int TdmDB::UpdatePos(int pos, int prenota, int escludi, int TipoUDC, int Tipoposizione, int Selezionata) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0, pren;
    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa.printf("UPDATE Posizioni SET Prenotata = %d, Disabilita = %d, TipoUDC = %d, Tipoposizione = %d, Selezionata = %d where Pos = %d ",
            prenota, escludi, TipoUDC, Tipoposizione, Selezionata, pos);

        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg(stringa);
        // dmDB->LogMsg("Update Pos "+IntToStr(pos)+", prenota : "+IntToStr(prenota)+", escludi : "+IntToStr(escludi)+", udc : "+CodiceUdc+", priorchiamata : "+IntToStr(priorita)+", secondi : "+IntToStr(secondi));
        delete ADOQuery;
        aggiorna_tab_posizioni_locale = true;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

int TdmDB::UpdatePosNoStato(int pos, int IdUDC, int hprel, int hdep, int piano) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0, pren;
    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa.printf("UPDATE piani_view SET " " IDUDC = %d, " " HPrel = %d, " " HDep = %d  where Pos = %d and Piano = %d ",
            IdUDC, hprel, hdep, pos, piano);

        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg(stringa);
        // dmDB->LogMsg("Update Pos "+IntToStr(pos)+", prenota : "+IntToStr(prenota)+", escludi : "+IntToStr(escludi)+", udc : "+CodiceUdc+", priorchiamata : "+IntToStr(priorita)+", secondi : "+IntToStr(secondi));
        delete ADOQuery;
        aggiorna_tab_posizioni_locale = true;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

int TdmDB::UpdateAltezzaPiano(int pos, int piano, int corsia, int hprel, int hdep)
{
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0, pren;
    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa.printf("UPDATE piani_view SET " " HPrel = %d, " " HDep = %d  WHERE Pos = %d AND piano = %d AND fila = %d",
            hprel, hdep, pos, piano, corsia);

        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg(stringa);
        // dmDB->LogMsg("Update Pos "+IntToStr(pos)+", prenota : "+IntToStr(prenota)+", escludi : "+IntToStr(escludi)+", udc : "+CodiceUdc+", priorchiamata : "+IntToStr(priorita)+", secondi : "+IntToStr(secondi));
        delete ADOQuery;
        aggiorna_tab_posizioni_locale = true;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

int TdmDB::UpdatePiano(int pos, int piano, int IdUDC, int hprel, int hdep, AnsiString tipopiano, int disabilitata, int artudc) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0, pren;
    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa.printf("UPDATE piani SET IdUDC = %d, HPrel = %d, HDep = %d, disabilitata=%d, tipopiano=%d where Pos = %d and piano = %d ",
            IdUDC, hprel, hdep, disabilitata, artudc, pos, piano);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg(stringa);
        delete ADOQuery;
        aggiorna_tab_posizioni_locale = true;
    }
    catch (...) {
        dmDB->LogMsg("Errore in stringa " + stringa);
    }
    return res;
}

int TdmDB::UpdateSoloStato(int pos, int prenota, int escludi) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0, pren;
    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa.printf("UPDATE Posizioni SET Prenotata = %d, Disabilita = %d  where Pos = %d ",
            prenota, escludi, pos);

        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg(stringa);
        delete ADOQuery;
        aggiorna_tab_posizioni_locale = true;
    }
    catch (...) {
    }
    return res;
}

int TdmDB::UpdateCorsia(int corsia, int prenota, int abilitata, int piena, int vuota, int inusoprel, int inusodep, int prior, int tipologia,
    int IgnoraRifOrdine, int depselettivo, AnsiString annotazioni, AnsiString CodiceArticolo, AnsiString Lotto) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0, pren;
    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa.printf
            ("UPDATE Corsie SET " " CorsiaPrenotata = %d, " " Abilitata = %d, " " Piena = %d, " " Vuota = %d, " " InUsoPrelievo = %d, "
            " InUsoDeposito = %d, " " Tipologia = %d, " " Priorita = %d, " " depositoselettivo = %d, " " annotazioni = '%s', " " IgnoraRifOrdine=%d where Fila = %d ", prenota,
            abilitata, piena, vuota, inusoprel, inusodep, tipologia, prior, depselettivo, annotazioni, IgnoraRifOrdine, corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg(stringa);
        // dmDB->LogMsg("Update Corsia "+IntToStr(corsia)+", prenota : "+IntToStr(prenota)+", abilitata : "+IntToStr(abilitata)+", piena : "+IntToStr(piena)+", vuota : "+IntToStr(vuota)+", priorita : "+IntToStr(prior)+", inusodep : "+IntToStr(inusodep));
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

void TdmDB::SettaCorsiaPiena(int corsia, int setta) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE Corsie SET Piena = " + IntToStr(setta);
        if (setta)
            stringa += ", InUsoDeposito = 0 ";
        stringa += " where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Settato Corsia Piena su Fila " + IntToStr(corsia) + ", piena : " + IntToStr(setta));
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }

}

void TdmDB::SettaCorsiaVuota(int corsia, int setta) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE Corsie SET Vuota = " + IntToStr(setta);
        stringa += " where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Settato Corsia Vuota su Fila " + IntToStr(corsia) + ", valore : " + IntToStr(setta));
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }

}

int TdmDB::ControllaCorsiaAncoraPiena(int corsia) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 1;
    try {
        if (!ADOConnection1->Connected)
            return 1;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select fila from PosizioniConDatiUDC where npianiocc > npiani AND Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        res = ADOQuery->RecordCount;
        ADOQuery->Close();
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

int TdmDB::RitornaTipologiaCorsia(int corsia) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!ADOConnection1->Connected)
            return 1;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = " Select * from Corsie where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("Tipologia")->AsInteger;
        }
        ADOQuery->Close();
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

int TdmDB::ContaPosizioniLiberePerTipoPallet(int tipologia) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 1;
    try {
        if (!ADOConnection1->Connected)
            return 1;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select Count(*) from Corsie inner join Posizioni on Corsie.Fila = Posizioni.Fila where Abilitata = 1 And Disabilita = 0 AND IdUDC = 0 AND Tipologia = " + IntToStr(tipologia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        res = ADOQuery->RecordCount;
        ADOQuery->Close();
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

int TdmDB::ContaPosizioniOccupateCorsia(AnsiString NomeCorsia, int Fila) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select * from Corsie  inner join Posizioni on Corsie.NomeCorsia = Posizioni.NomeCorsia where Corsie.NomeCorsia ='" + NomeCorsia + "' AND IdUDC > 0 ";
        if (Fila > 0)
            stringa = stringa + " AND Corsie.FilaInCorsia = " + IntToStr(Fila);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        res = ADOQuery->RecordCount;
        ADOQuery->Close();
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

int TdmDB::PrimoIdUdcTrovatoInCorsia(AnsiString NomeCorsia, int Fila) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select IdUdc from Corsie inner join Posizioni on Corsie.NomeCorsia = Posizioni.NomeCorsia where Corsie.NomeCorsia ='" + NomeCorsia + "' AND IdUDC > 0 ";
        if (Fila > 0)
            stringa = stringa + " AND Corsie.FilaInCorsia = " + IntToStr(Fila);
        stringa = stringa + " order by Pos ";
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            ADOQuery->First();
            res = ADOQuery->FieldByName("IdUdc")->AsInteger;
        }
        ADOQuery->Close();
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

void TdmDB::AbilitaCorsia(int corsia, int setta) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE Corsie SET Abilitata = " + IntToStr(setta);
        stringa += " where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Settato Abilitata su Fila " + IntToStr(corsia) + ", valore : " + IntToStr(setta));
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
}

void TdmDB::SettaCorsiaInUso(int corsia, int prelievo, int setta, AnsiString CodiceArticolo, AnsiString Lotto) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res, resetta;
    try {
        if ((!ADOConnection1->Connected) || (corsia == 0))
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        if (setta)
            resetta = 0;
        else
            resetta = 1;
        if (prelievo)
            stringa = "UPDATE Corsie SET InUsoPrelievo = " + IntToStr(setta) + ", InUsoDeposito = " + IntToStr(resetta);
        // controllare se la corsia risulta vuota
        else
            stringa = "UPDATE Corsie SET InUsoDeposito = " + IntToStr(setta) + ", InUsoPrelievo = " + IntToStr(resetta) + ", Vuota = 0 ";
        /* if ((!prelievo) && (setta) && (CodiceArticolo != "")) {
         stringa = stringa + ", CodiceArticolo = '" + CodiceArticolo + "' ";
         }
         if ((!prelievo) && (setta) && (Lotto != "")) {
         stringa = stringa + ", Lotto = '" + Lotto + "' ";
         } */
        stringa += " where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg(stringa);
        if (prelievo)
            dmDB->LogMsg("Settato Corsia in uso per prelievo su Fila " + IntToStr(corsia) + ", valore : " + IntToStr(setta) + " in uso deposito, valore : " +
            IntToStr(resetta));
        else
            dmDB->LogMsg("Settato Corsia in uso per deposito su Fila " + IntToStr(corsia) + ", valore : " + IntToStr(setta) + " in uso prelievo, valore : " +
            IntToStr(resetta));
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }

}

int TdmDB::RicercaDepositoUdcinCorsia(AnsiString NomeCorsia, int &piano) {

    AnsiString stringa, stringapos, Vassoio;
    int pos = 0, pianicorsia, ordinamento_dec = 0, fila, filapos, prenotazioni;
    bool trovata = false, cambiofila;
    QueryCorsie->Close();
    QueryCorsie->SQL->Clear();

    piano = 0;
    // stringa = " select * from Corsie where Fila > 0 AND Fila <= "+IntToStr(last_corsia_udc)+" AND Abilitata = 1 AND Piena = 0 AND(InUsoPrelievo = 0 Or Tipologia < 3) AND Tipologia = "+IntToStr(tipologia);
    stringa = "select * from Corsie where LTrim(RTrim(NomeCorsia)) = '" + Trim(NomeCorsia) + "' AND Abilitata = 1 AND Piena = 0 ";
    // tolgo AND ( InUsoPrelievo = 0 Or Tipologia < 3 )
    stringa = stringa + " order by InUsoDeposito DESC, InUsoPrelievo, Priorita, Fila ";

    QueryCorsie->SQL->Append(stringa);
    QueryCorsie->Open();
    if (QueryCorsie->RecordCount) {
        QueryCorsie->First();
        fila = 0;
        pos = 0;
        cambiofila = false;
        while ((!QueryCorsie->Eof) && (!trovata)) {
            prenotazioni = 0;
            QueryPos->Close();
            QueryPos->SQL->Clear();
            cambiofila = false;
            filapos = QueryCorsie->FieldByName("FILA")->AsInteger;
            ordinamento_dec = QueryCorsie->FieldByName("OrdinamentoCorsia")->AsInteger;
            stringapos = "select * from Posizioni inner join Piani on Piani.Pos = Posizioni.Pos where Posizioni.Fila = " + IntToStr(filapos);
            if (ordinamento_dec)
                stringapos = stringapos + "  order by Posizioni.Pos DESC, Piani.Piano desc ";
            else
                stringapos = stringapos + " order by Posizioni.Pos, Piani.Piano desc ";
            QueryPos->SQL->Append(stringapos);
            QueryPos->Open();
            if (QueryPos->RecordCount) {
                QueryPos->First();
                while ((!QueryPos->Eof) && (!trovata) && (!cambiofila)) {
                    if ((!QueryPos->FieldByName("IdUDC")->AsInteger) && (!QueryPos->FieldByName("Prenotata")->AsInteger)) {
                        // posizione vuota e disponibile  e piano ok    (disabilitata si riferisce alla pos, disabilita al piano)
                        if ((!QueryPos->FieldByName("Disabilitata")->AsInteger) && (!QueryPos->FieldByName("Disabilita")->AsInteger)) {
                            pos = QueryPos->FieldByName("Pos")->AsInteger;
                            piano = QueryPos->FieldByName("Piano")->AsInteger;
                            filapos = QueryPos->FieldByName("Fila")->AsInteger;
                        }
                    }
                    else {
                        if (QueryPos->FieldByName("Prenotata")->AsInteger) {
                            pos = -1;
                            trovata = true;
                        }
                        else if (pos) {
                            trovata = true;
                        }
                        else {
                            // con questa impostazione fa il cambio di corsia
                            if (!pos) {
                                cambiofila = true;
                            }
                            /*
                             // con questa impostazione non cambia mai corsia
                             pos = 0;
                             trovata = true;
                             */
                        }
                    }
                    QueryPos->Next();
                }
                if ((QueryPos->Eof) && (!trovata) && (pos)) // corsia vuota
                        trovata = true;
                if ((QueryPos->Eof) && (!trovata) && (!pos)) // corsia piena
                        SettaCorsiaPiena(filapos, 1);

            }
            QueryCorsie->Next(); // se cambio la fila ricontrollo la prima posizione se è libera
        }
    }
    QueryPos->Close();
    QueryCorsie->Close();
    return pos;
}

void TdmDB::ArticoloPrelevatoDepositato(int pos, int idudc, int piano, int corsia) {

    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    if (!ADOConnection1->Connected)
        return;
    try {

        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        // se piano=0 update tutta la pos
        if (piano == 0) {
            stringa = "Update piani_view set IdUDC = " + IntToStr(idudc) + " where Pos = " + IntToStr(pos);
        }
        else {
            stringa = "Update piani_view set IdUDC = " + IntToStr(idudc) + " where Pos = " + IntToStr(pos) + " and piano= " + IntToStr(piano) + " and fila = " + IntToStr(corsia); //

        } // + " Or PosOpposta = " + IntToStr(pos);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg(stringa + " res query : " + IntToStr(res));

        aggiorna_tab_posizioni_locale = true;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {

    }
    delete ADOQuery;
}

void __fastcall TdmDB::ADOConnection1ConnectComplete(TADOConnection * Connection, Error* const Error, TEventStatus & EventStatus) {
    // imposto dataora nel formato giorno/mese/anno perchè ogni versione sql in base al paese è diversa e nei filtri dopo fa casino
    try {
        QueryPos->Close();
        QueryPos->SQL->Clear();
        QueryPos->SQL->Append("set DATEFORMAT dmy"); // setto formato database giorno/mese/anno
        QueryPos->ExecSQL();
    }
    catch (...) {
        ADOConnection1->Close();
    }

}

// ---------------------------------------------------------------------------
/* int TdmDB::AggiornaPrioritaChiamataPerDataInListaMissione() {
 AnsiString strsql;
 TADOQuery *ADOQuery;
 int res = 0;
 //ogni volta che la richiamo aggiungo 1 a priorità data cos' la piu vecchia è sempre piu prioritaria
 // funzione DATEADD, aggiunge secondi ad una data sintassi DATEADD(second,n°secondi,Data)
 try {
 //data = FormatDateTime(MainForm->format_data_ora_database, StrToDateTime(data)) ;
 ADOQuery = new TADOQuery(NULL);
 ADOQuery->Connection = ADOConnection1;
 strsql.printf("UPDATE ListaMissioni SET Priorita = (Priorita + 1) , DataOraCambioPriorita = DATEADD(second,SecondiScattoPriorita,GETDATE()) WHERE DataOraCambioPriorita IS NOT NULL AND DataOraCambioPriorita < GetDate()");
 ADOQuery->SQL->Text = strsql;
 res = ADOQuery->ExecSQL();
 //dmDB->LogMsg(strsql + " , result : "+IntToStr(res));
 } catch(Exception &E) {
 }
 delete ADOQuery;
 return res;
 }
 */

void TdmDB::AggiornaTabPostazioni() {
    CaricaTabellaK("Posizioni order by pos", "Pos", TabPostazioni);
    ///   CaricaTabellaK("PosizioniVasche inner join Vasche on PosizioniVasche.IdVasca = Vasche.IdVasca order by Pos", "Pos", TabPostazioniVasche);
    ///AggiornaStrutturaPLC();
}

void TdmDB::AggiornaTabPosizioniUDC() {
    CaricaTabellaK("Posizioni order by pos", "Pos", TabPosizioniUDC);
}

void TdmDB::AggiornaNomePiani() {
    CaricaTabellaK("piani order by nomepos", "NomePos", TabNomePiani);
}

void TdmDB::AggiornaTabPorte() {
    CaricaTabellaK("Porte order by IdPorta", "IdPorta", TabPorte);
    AggiornaStatoPorteStruttura();
}

void TdmDB::AggiornaTabTipoArticoli() {
    CaricaTabellaK("TipologiaArticoli order by IdTipologia", "IdTipologia", TipologiaArticoli);
}

void TdmDB::AggiornaTabTipoUdc() {
    CaricaTabellaK("TabTipoUDC order by IdTipoUdc", "IdTipoUdc", TabTipoUDC);
}

void TdmDB::AggiornaTabParametri() {
    CaricaTabellaK("Parametri order by IdParametro", "IdParametro", TabParametri);
}

void TdmDB::AggiornaTabTipologiaCorsia() {
    CaricaTabellaK("TabTipologiaCorsia order by CodTipo", "CodTipo", TipologiaCorsia);
}

void TdmDB::AggiornaTabAnagrafica() {
    CaricaTabellaK("Articoli order by IdArticolo", "IdArticolo", TabArticoli);
}

AnsiString TdmDB::DescrizioneArticolo(int IDArticolo) {
    return TabArticoli[IntToStr(IDArticolo)]["DESCRIZIONE"];
}

AnsiString TdmDB::NomePosizione(int pos) {
    return TabPostazioni[IntToStr(pos)]["CUSTOMERPOS"];
}

AnsiString TdmDB::NomeCorsiaDaPosizione(int pos) {
    return TabPostazioni[IntToStr(pos)]["NOMECORSIA"];
}

AnsiString TdmDB::ZonaPosizione(int pos) {
    return TabPostazioni[IntToStr(pos)]["ZONA"];
}

int TdmDB::IdUDCDaPosizione(int pos) {
    return TabPostazioni[IntToStr(pos)]["IDUDC"].ToIntDef(0);
}

AnsiString TdmDB::NomePos(int pos) {
    AnsiString ret;
    ret = TabPostazioni[IntToStr(pos)]["CUSTOMERPOS"];
    if ((ret == "") || (ret.SubString(1, 1) == "O") || (ret.SubString(1, 1) == "I")) {
        ret = IntToStr(pos);
    }
    return ret;
}

int TdmDB::PosOpposta(int pos) {
    IndexListIterator i;
    int res = 0; // ricerco se la posizone passta è una posizione opposta rispetto quella utilizzata

    for (i = TabPostazioni.begin(); i != TabPostazioni.end(); i++) {
        if (i->second["POSOPPOSTA"].ToIntDef(0) == pos) {
            res = i->second["POS"].ToIntDef(0);
            break;
        }
    }

    return res;
}

int TdmDB::RitornaTipoPianoDaNomePosinPiani(AnsiString Nomepos) {
    AnsiString stringa;
    AnsiString strsql;
    IndexListIterator i;
    int res = 0;
    for (i = TabNomePiani.begin(); i != TabNomePiani.end(); i++) {
        stringa = Trim(i->second["NOMEPOS"]);
        if (stringa == Nomepos) {
            res = i->second["TIPOPIANO"].ToIntDef(0);
            break;
        }
    }
    return res;
}

int TdmDB::FilaPosizione(int pos) {
    int pos_opposta;
    pos_opposta = 0; // PosOpposta(pos);
    if (pos_opposta)
        return TabPostazioni[IntToStr(pos_opposta)]["FILA"].ToIntDef(0);
    else
        return TabPostazioni[IntToStr(pos)]["FILA"].ToIntDef(0);
}

int TdmDB::PosizionePrenotata(int pos) {
    int pos_opposta;
    pos_opposta = PosOpposta(pos);
    if (pos_opposta)
        return TabPostazioni[IntToStr(pos_opposta)]["PRENOTATA"].ToIntDef(0);
    else
        return TabPostazioni[IntToStr(pos)]["PRENOTATA"].ToIntDef(0);
}

int TdmDB::PosizioneEsclusa(int pos) {
    int pos_opposta;
    pos_opposta = PosOpposta(pos);
    if (pos_opposta)
        return TabPostazioni[IntToStr(pos_opposta)]["DISABILITA"].ToIntDef(0);
    else
        return TabPostazioni[IntToStr(pos)]["DISABILITA"].ToIntDef(0);
}

int TdmDB::CorsiaPrenotata(int corsia) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!ADOConnection1->Connected)
            return 1;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select * from Corsie where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("CorsiaPrenotata")->AsInteger;
        }
        ADOQuery->Close();
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

int TdmDB::CorsiaAbilitata(int corsia) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!ADOConnection1->Connected)
            return 1;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select * from Corsie where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("Abilitata")->AsInteger;
        }
        ADOQuery->Close();
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

int TdmDB::RitornaTipologiadaCorsia(int corsia) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!ADOConnection1->Connected)
            return 1;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select * from Corsie where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("Tipologia")->AsInteger;
        }
        ADOQuery->Close();
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

int TdmDB::OrdinamentoFila(int corsia) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!ADOConnection1->Connected)
            return 1;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select * from Corsie where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("OrdinamentoCorsia")->AsInteger;
        }
        ADOQuery->Close();
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

int TdmDB::OrdinamentoFilaPosOpposta(int corsia) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!ADOConnection1->Connected)
            return 1;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select ORDINAMENTOCORSIAOPPOSTA from Corsie where Fila = " + IntToStr(corsia);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("ORDINAMENTOCORSIAOPPOSTA")->AsInteger;
        }
        ADOQuery->Close();
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

int TdmDB::RitornaPosInputPlc(int pos) {
    return TabPostazioni[IntToStr(pos)]["POSINPUTPLC"].ToIntDef(0);
}

int TdmDB::RitornaTipologia(int pos) {
    return TabPostazioni[IntToStr(pos)]["TIPOLOGIA"].ToIntDef(0);
}

AnsiString TdmDB::PosizioniRichiestaAperturaPorta(int porta) {
    return TabPorte[IntToStr(porta)]["POSAPRIPORTA"];
}

AnsiString TdmDB::PosizioniPortaAperta(int porta) {
    return TabPorte[IntToStr(porta)]["POSPORTAAPERTA"];
}

AnsiString TdmDB::StrConsensoPorta(int porta) {
    return TabPorte[IntToStr(porta)]["STRCONSENSO"];
}

AnsiString TdmDB::StrConsensoPortaTFuoco(int porta) {
    return TabPorte[IntToStr(porta)]["POSTAGLIAFUOCO"];
}

int TdmDB::StatoPorta(int porta) {
    return TabPorte[IntToStr(porta)]["STATO"].ToIntDef(0);
}

AnsiString TdmDB::ReturnCodiceTipoArticoloDaId(int idtipologia) {
    return TipologiaArticoli[IntToStr(idtipologia)]["IDARTICOLO"];
}

AnsiString TdmDB::ReturnCodiceTipoUDCDaId(int idtipologia) {
    return TabTipoUDC[IntToStr(idtipologia)]["TIPOUDC"];
}

int TdmDB::ReturnColoreTipologiaDaId(int codtipo) {
    return TipologiaCorsia[IntToStr(codtipo)]["COLORE"].ToIntDef(255);
}

int TdmDB::TipoPlc(int pos) {
    return TabPostazioni[IntToStr(pos)]["TIPOPOSIZIONE"].ToIntDef(0);
}

void TdmDB::AggiornaStrutturaZone() {
    CaricaTabellaK("StrutturaZone where TopX IS NOT NULL order by id ", "ID", TabStrutturaZone);
}

int TdmDB::ReturnIdTipologiaDaCodice(AnsiString codice) {
    IndexListIterator i;
    int res = 0;
    for (i = TipologiaArticoli.begin();
        i != TipologiaArticoli.end();
        i++) {
        if (i->second["TIPOARTICOLO"] == codice) {
            res = i->second["TIPOARTICOLO"].ToIntDef(0);
            break;
        }
    }
    return res;
}

void TdmDB::AggiornaStatoPorteStruttura() {
    IndexListIterator i;
    int res = 0;
    /* for (i = TabPorte.begin(); i != TabPorte.end(); i++) {
     ClientData.DatiPorte[i->second["IDPORTA"].ToIntDef(1)].Stato = i->second["STATO"].ToIntDef(0);
     } */

}
/*
 void TdmDB::AggiornaStatoPorteFumoStruttura() {
 IndexListIterator i;
 int j, res = 0;
 for (i = TabPorteFumo.begin(); i != TabPorteFumo.end(); i++) {
 for (j = 1; j <= 2; j++) {
 ClientData.DatiPorteFumo[i->second["IDPORTA"].ToIntDef(1)].Stato = i->second["STATO"].ToIntDef(0);
 }
 }
 } */

int TdmDB::ReturnIdPortaDaStringaConsensoPorta(AnsiString str_consenso) {
    IndexListIterator i;
    int res = 0;
    for (i = TabPorte.begin(); i != TabPorte.end(); i++) {
        if (i->second["STRCONSENSO"] == str_consenso) {
            res = i->second["IDPORTA"].ToIntDef(0);
            break;
        }
    }
    return res;
}

int TdmDB::ControllaUtentePassword(AnsiString Utente, AnsiString Password) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!ADOConnection1->Connected)
            return 0;
        if (Utente == "SIMULA") {
            MainForm->invia_richiesta_simulazione = 1;
            return 1;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        if (Password.UpperCase() == "*STARTUP") // startup iniziale
                stringa = "Select * from Login where UPPER(UserId) = '" + Utente.UpperCase() + "'";
        else
            stringa = "Select * from Login where UPPER(UserId) = '" + Utente.UpperCase() + "' AND password = '" + Password.UpperCase() + "'";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("IdLogin")->AsInteger;
            pwdlevel = res;
            ActualUser = Utente;
            // assehna la zona solo se specificato nel record altrimenti usa il valore nel.ini
            if ((ADOQuery->FieldByName("Zona")->AsString.Trim() != NULL) && (ADOQuery->FieldByName("Zona")->AsString.Trim() != ""))
                ZonaString = ADOQuery->FieldByName("Zona")->AsString.Trim();
        }
        else {
            res = 0;
            pwdlevel = res;
            ActualUser = "NONE";
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}

void __fastcall TdmDB::TimerPasswordTimer(TObject * Sender) {
    ActualUser = "NONE";
    pwdlevel = 0;
}

// ---------------------------------------------------------------------------
int TdmDB::CreaIdUDC() {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;
    TLocateOptions Opts;
    Opts.Clear();
    int idsrticolo = 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT TOP 1 IDUDC FROM UDC order by IdUDC DESC");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        idsrticolo = ADOQuery->FieldByName("IDUDC")->AsInteger;
        /* if (ultimo_id_articolo > idsrticolo) {
         idsrticolo = ultimo_id_articolo ;
         }else { */
        do {
            if (idsrticolo > 4000000000) {
                idsrticolo = 1;
            }
            else
                idsrticolo++;
        }
        while (ADOQuery->Locate("IDUDC", idsrticolo, Opts));
        // }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    // ultimo_id_articolo = idsrticolo ; // me lo salvo perchè se creo un articolo in manuale e un oin auto non ddeve avere l ostesso id
    return idsrticolo;
}

int TdmDB::UDCPresenteInMagazzino(int idudc) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select Pos from piani where IdUDC = " + IntToStr(idudc) + " and IDUDC>0";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("Pos")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {

    }
    delete ADOQuery;
    return res;
}

int TdmDB::CreaIdArticoloAnagrafica() {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;
    TLocateOptions Opts;
    Opts.Clear();
    int idsrticolo = 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT TOP 1 idarticolo FROM Articoli order by IdArticolo DESC");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->First();
        idsrticolo = ADOQuery->FieldByName("IdArticolo")->AsInteger;
        /* if (ultimo_id_articolo > idsrticolo) {
         idsrticolo = ultimo_id_articolo ;
         }else { */
        do {
            if (idsrticolo > 4000000000) {
                idsrticolo = 1;
            }
            else
                idsrticolo++;
        }
        while (ADOQuery->Locate("IdArticolo", idsrticolo, Opts));
        // }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    // ultimo_id_articolo = idsrticolo ; // me lo salvo perchè se creo un articolo in manuale e un oin auto non ddeve avere l ostesso id
    return idsrticolo;
}

int TdmDB::CreaIdTipologia() {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;
    TLocateOptions Opts;
    Opts.Clear();
    int idsrticolo = 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT IdTipologia FROM TipologiaArticoli order by IdTipologia");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        idsrticolo = ADOQuery->FieldByName("IdTipologia")->AsInteger;
        /* if (ultimo_id_articolo > idsrticolo) {
         idsrticolo = ultimo_id_articolo ;
         }else { */
        do {
            if (idsrticolo > 4000000000) {
                idsrticolo = 1;
            }
            else
                idsrticolo++;
        }
        while (ADOQuery->Locate("IdTipologia", idsrticolo, Opts));
        // }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    // ultimo_id_articolo = idsrticolo ; // me lo salvo perchè se creo un articolo in manuale e un oin auto non ddeve avere l ostesso id
    return idsrticolo;
}

int TdmDB::CreaIdArtUdc() {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;
    TLocateOptions Opts;
    Opts.Clear();
    int idsrticolo = 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT IDArtUDC FROM ArticoliUDC order by IDArtUDC");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        idsrticolo = ADOQuery->FieldByName("IDArtUDC")->AsInteger;
        /* if (ultimo_id_articolo > idsrticolo) {
         idsrticolo = ultimo_id_articolo ;
         }else { */
        do {
            if (idsrticolo > 4000000000) {
                idsrticolo = 1;
            }
            else
                idsrticolo++;
        }
        while (ADOQuery->Locate("IDArtUDC", idsrticolo, Opts));
        // }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    // ultimo_id_articolo = idsrticolo ; // me lo salvo perchè se creo un articolo in manuale e un oin auto non ddeve avere l ostesso id
    return idsrticolo;
}

int TdmDB::ArticoloPresenteInArchivio(int idarticolo) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select * from ArticoliUDC where IdArtUdc = " + IntToStr(idarticolo);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("IdArtUdc")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {

    }
    delete ADOQuery;
    return res;
}

int TdmDB::ReturnUltimoIdUdceDestDaIdArticolo(int idarticolo, AnsiString & CorsiaDest) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;
    CorsiaDest = "";
    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select * from ArticoliUDC where IdArtUdc = " + IntToStr(idarticolo);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("UltimoIdUdc")->AsInteger;
            CorsiaDest = ADOQuery->FieldByName("CorsiaDeposito")->AsString;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {

    }
    delete ADOQuery;
    return res;
}

int TdmDB::CodicePresenteInAnagrafica(AnsiString Codice) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select * from Articoli where IDArticolo = '" + Codice + "'";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("IdArticolo")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {

    }
    delete ADOQuery;
    return res;
}

AnsiString TdmDB::ReturnCodiceArticoloDaIdArticolo(int idarticolo) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    AnsiString res = "Nessuno";

    try {
        if (!ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select * from Articoli where IdArticolo = " + IntToStr(idarticolo);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            // res = ADOQuery->FieldByName("Codice")->AsString;
            res = ADOQuery->FieldByName("Nome")->AsString; // utlizzo la colonna Nome
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {

    }
    delete ADOQuery;
    return res;
}

AnsiString TdmDB::ReturnCodiceArticoloDaIdUdc(int idudc) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    AnsiString res = "Nessuno";

    try {
        if (!ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "SELECT Nome FROM UDC inner join articoliudc on udc.idartudc = articoliudc.idartudc inner join articoli on articoliudc.idarticolo = articoli.idarticolo where idudc = " + IntToStr(idudc);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            // res = ADOQuery->FieldByName("Codice")->AsString;
            res = ADOQuery->FieldByName("Nome")->AsString; // utlizzo la colonna Nome
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {

    }
    delete ADOQuery;
    return res;
}

int TdmDB::ControllaDepositoOk(int pos, int posopposta, int mismanuale) {
    AnsiString stringa, stringapos, Vassoio;
    int pos_return = 0, fila, ordinamento_desc, identificativo_struttura_plc;
    bool trovata = false;
    QueryPos->Close();
    QueryPos->SQL->Clear();
    identificativo_struttura_plc = RitornaPosInputPlc(pos);
    // if ((identificativo_struttura_plc) &&  ((ControllaChiamataOkVemac(identificativo_struttura_plc, false)) || (mismanuale)))
    if ((identificativo_struttura_plc) && mismanuale)
        return pos; // consento missioni manuali anche senza idtotem e idvasca
    fila = FilaPosizione(pos);
    if (((fila > 0) && (CorsiaPrenotata(fila))) || (PosizionePrenotata(pos)))
        return -1;
    if (posopposta)
        ordinamento_desc = OrdinamentoFilaPosOpposta(fila);
    else
        ordinamento_desc = OrdinamentoFila(fila);

    if (fila == 0)
        stringapos = "select * from PosizioniCONDATIUDC where Pos = " + IntToStr(pos);
    else
        stringapos = "select * from PosizioniCONDATIUDC where Fila = " + IntToStr(fila);

    stringapos = stringapos + " order by Pos ";
    if (ordinamento_desc)
        stringapos = stringapos + "DESC ";
    QueryPos->SQL->Append(stringapos);
    QueryPos->Open();
    if (QueryPos->RecordCount) {
        QueryPos->First();
        while ((!QueryPos->Eof) && (!trovata)) {
            if ((QueryPos->FieldByName("NPIANIOCC")->AsInteger < QueryPos->FieldByName("NPIANI")->AsInteger) && (!QueryPos->FieldByName("Prenotata")->AsInteger)) {
                // posizione vuota e disponibile
                if (((pos == QueryPos->FieldByName("Pos")->AsInteger) || ((posopposta) && (pos == QueryPos->FieldByName("PosOpposta")->AsInteger))) &&
                    ((!QueryPos->FieldByName("Disabilita")->AsInteger) || (mismanuale))) {
                    trovata = true;
                    if (posopposta)
                        pos_return = QueryPos->FieldByName("PosinfilaoppostaOpposta")->AsInteger;
                    else
                        pos_return = pos;
                }

            }
            else {
                pos_return = 0;
                // trovata = true;
            }
            QueryPos->Next();
        }
    }
    QueryPos->Close();
    return pos_return;
}

int TdmDB::ControllaPrelievoOk(int pos, int posopposta, int mismanuale) {

    AnsiString stringa, stringapos;
    int pos_return = 0, fila, ordinamento_desc, identificativo_struttura_plc;
    bool trovata = false;
    QueryPos->Close();
    QueryPos->SQL->Clear();
    identificativo_struttura_plc = RitornaPosInputPlc(pos);
    // if ((identificativo_struttura_plc) &&  ((ControllaChiamataOkVemac(identificativo_struttura_plc, true)) || (mismanuale)))
    if (mismanuale)
        return pos; // consento missioni manuali anche senza idtotem e idvasca
    fila = FilaPosizione(pos);
    if (((fila > 0) && (CorsiaPrenotata(fila))) || (PosizionePrenotata(pos)))
        return 0;
    else if ((fila > 0) && (CorsiaAbilitata(fila)) && (PosizioneEsclusa(pos)))
        return pos;
    if (posopposta)
        ordinamento_desc = OrdinamentoFilaPosOpposta(fila);
    else
        ordinamento_desc = OrdinamentoFila(fila);

    if (fila == 0)
        stringapos = "select * from PosizioniConDatiUDC where Pos = " + IntToStr(pos) + " OR PosInFilaOpposta = " + IntToStr(pos);
    else
        stringapos = "select * from PosizioniConDatiUDC where Fila = " + IntToStr(fila);

    stringapos = stringapos + " order by Pos ";
    if (ordinamento_desc)
        stringapos = stringapos + "DESC ";
    QueryPos->SQL->Append(stringapos);
    QueryPos->Open();
    if (QueryPos->RecordCount) {
        QueryPos->First();
        while ((!QueryPos->Eof) && (!trovata)) {
            if ((QueryPos->FieldByName("Npianiocc")->AsInteger > 0) && (QueryPos->FieldByName("posinputplc")->AsInteger == 0) &&
                // ATTENZIONE!!! INSERIRE CONTROLLO CHE IL TIPO PALLET SIA = A TIPO POSTAZIONE
                (!QueryPos->FieldByName("Prenotata")->AsInteger)) {
                // posizione vuota e disponibile
                if (pos == QueryPos->FieldByName("Pos")->AsInteger) {
                    pos_return = 0; // pos vuota
                    trovata = true;
                }
                QueryPos->Next();
            }
            else {
                /// controllare PosInFilaOpposta
                if (((pos == QueryPos->FieldByName("Pos")->AsInteger) || ((posopposta) && (pos == QueryPos->FieldByName("PosInFilaOpposta")->AsInteger))) &&
                    ((!QueryPos->FieldByName("Disabilita")->AsInteger) || (mismanuale)) && // le missioni manuali le consenso con pos disabilitata
                    (!QueryPos->FieldByName("Prenotata")->AsInteger)) {
                    if ((posopposta))
                        pos_return = QueryPos->FieldByName("PosInFilaOpposta")->AsInteger;
                    else
                        pos_return = pos;
                }
                else
                    pos_return = 0;
                trovata = true;
            }
        }
    }
    QueryPos->Close();
    return pos_return;
}

int TdmDB::ControllaMissioneOk(int mismanuale, int &pos_prel, int &pos_dep) {
    int pos_prel_ok, pos_dep_ok;
    /*
     if (((pos_prel <= last_pos_udc) && (pos_dep > last_pos_udc)) ||
     ((pos_prel > last_pos_udc) && (pos_dep <= last_pos_udc))) {
     pos_prel = 0;
     pos_dep = 0;
     LogMsg("Richiesta missione non possibile : Prelievo " + IntToStr(pos_prel) + " , Deposito " + IntToStr(pos_dep));
     return 0;
     }
     else
     */ {
        pos_prel_ok = ControllaPrelievoOk(pos_prel, 0, mismanuale);
        if (!pos_prel_ok)
            pos_prel_ok = ControllaPrelievoOk(pos_prel, 1, mismanuale); // guardo se posso prelevare
        pos_dep_ok = ControllaDepositoOk(pos_dep, 0, mismanuale);
        if (!pos_dep_ok)
            pos_dep_ok = ControllaDepositoOk(pos_dep, 1, mismanuale); // guardo se posso prelevare
        pos_prel = pos_prel_ok; // ritorno alla funzione le posizioni eventualmente aggiiornate con la opposta
        pos_dep = pos_dep_ok;
        return (pos_prel_ok && pos_dep_ok);
}
}

AnsiString TdmDB::ReturnStringaHexDatiDaIdUdc(int idudc) {
    AnsiString stringa, res = "";
    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select HexStringa from Udc inner join ArticoliUdc on UDC.IdArtUdc = ArticoliUDC.IdArtUdc where UDC.IdArtUdc > 0 AND UDC.IdUdc = " +
            IntToStr(idudc);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("HexStringa")->AsString;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {

    }
    delete ADOQuery;
    return res;
}

AnsiString TdmDB::ReturnStringaHexDatiDaIdArtUdc(int idartudc) {
    AnsiString stringa, res = "";
    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select HexStringa from ArticoliUdc where IdArtUdc > 0 AND IdArtUdc = " +
            IntToStr(idartudc);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("HexStringa")->AsString;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {

    }
    delete ADOQuery;
    return res;
}

int TdmDB::CreaIdCentroMissione() {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;
    TLocateOptions Opts;
    Opts.Clear();
    int idmiss = 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT TOP 1 ID FROM CentroMissioni order by Generata desc");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->First();
        idmiss = ADOQuery->FieldByName("Id")->AsInteger;
        do {
            if (idmiss == 32678) {
                idmiss = 1;
            }
            else
                idmiss++;
        }
        while (ADOQuery->Locate("Id", idmiss, Opts));
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return idmiss;
}

int TdmDB::ReturnUltimoIdCentroMissione() {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;

    int idmiss = 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT TOP 1 ID FROM CentroMissioni order by id desc");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->First();

        if (ADOQuery->RecordCount > 0) {
            idmiss = ADOQuery->FieldByName("Id")->AsInteger;
        }

        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return idmiss;
}

int TdmDB::PresenzaCentroMissione(int pos, int stato) {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;
    TLocateOptions Opts;
    Opts.Clear();
    int res = 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql = "SELECT TOP 1 ID FROM CentroMissioni where (fine is null) and stato=" + IntToStr(stato) + " and ((posprel =" + IntToStr(pos) + ") or (posdep = " + IntToStr(pos) + "))";
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();

        res = ADOQuery->RecordCount;
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDB::PosPresenteMissioneAttiva(int pos) {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;
    TLocateOptions Opts;
    Opts.Clear();
    int res = 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql = "SELECT TOP 1 ID FROM Missioni where ((fine is null) and ((posprel =" + IntToStr(pos) + ") or (posdep = " + IntToStr(pos) + ")))";
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();

        res = ADOQuery->RecordCount;
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDB::GeneraCentroMissione(TCentroMissione CentroMis) {
    int res = 0, pos_prel_ricerca, pos_dep_ricerca, tipologiamissione;
    TADOQuery *ADOQuery;
    AnsiString strsql, nomeprel, nomedep, nomecorsia;
    int IdArtUdc = 0;
    bool eseguimissione = true;
    TUDC UdcMissione;

    try {
        if (!ADOConnection1->Connected)
            return 0;

        if (CentroMis.IDUDC == 0) {
            CentroMis.IDUDC = TabPostazioni[IntToStr(CentroMis.posprel)]["IDUDC"].ToIntDef(0);
        }

        UdcMissione.IDUDC = CentroMis.IDUDC;
        IdArtUdc = UdcMissione.Articolo.IDArticolo;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        if ((CentroMis.posprel > 0) || (CentroMis.posdep > 0) || (CentroMis.ZonaPrelievo != "") || (CentroMis.ZonaDeposito != "")) {
            ADOQuery = new TADOQuery(NULL);
            ADOQuery->Connection = ADOConnection1;
            strsql.printf
                ("INSERT INTO CentroMissioni (POSPREL ,POSDEP, GENERATA,TIPOMISSIONE,PRIORITA,AGV,idUDC, "
                " CodTipoMovimento, codtipomissione, CorsiaDeposito,stato,FilaInCorsiaDeposito,HPrel,HDep,pianoprel,pianodep,TipoUDC,destinazionemoduli,ZonaPrelievo, ZonaDeposito)"
                " VALUES (%d,%d,%s,%d,%d,%d,%d,%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s')"
                , CentroMis.posprel
                , CentroMis.posdep
                , "GetDate()"
                , CentroMis.TipoMissione
                , CentroMis.Priorita
                , CentroMis.Agv
                , CentroMis.IDUDC
                , CentroMis.CodTipoMovimento
                , CentroMis.CodTipoMissione
                , CentroMis.CorsiaDeposito.c_str()
                , CentroMis.stato
                , CentroMis.FilaInCorsiaDeposito
                , CentroMis.h_prel
                , CentroMis.h_dep
                , CentroMis.pianoprel
                , CentroMis.pianodep
                , CentroMis.TipoUDC
                , CentroMis.DestinazioneModuli
                , CentroMis.ZonaPrelievo.c_str()
                , CentroMis.ZonaDeposito.c_str()
                );
            ADOQuery->SQL->Text = strsql;
            res = ADOQuery->ExecSQL();
            if (res > 0) {
                dmDB->SettaPosSelezionata(CentroMis.posprel, 1, 0);
                dmDB->SettaPosSelezionata(CentroMis.posdep, 1, 0);
            }
            strsql = "Centro missione creata , result : " + IntToStr(res);
        }
        else {
            strsql = "pos_prel (" + IntToStr(CentroMis.posprel) + ") o pos_dep " + IntToStr(CentroMis.posdep) + " non corretto in genera centro missioni";
        }
        LogMsg(strsql);
        delete ADOQuery;
    }
    catch (...) {
    }
    return res;
}

int TdmDB::InsertUpdateArticoloUDC(int id_articolo, int pos_dep, int IDArticolo, AnsiString hex_string, AnsiString CorsiaDeposito, int ultimoidudc, AnsiString Codice) {
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0, dacreare = 0, virgola = 0;
    AnsiString Val;

    if (!ADOConnection1->Connected)
        return 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        if (id_articolo == 0) {
            dacreare = 1;
            id_articolo = CreaIdArtUdc();
        }
        if ((dacreare) || (!ArticoloPresenteInArchivio(id_articolo))) {
            ADOQuery = new TADOQuery(NULL);
            ADOQuery->Connection = ADOConnection1;
            if (IDArticolo < 0)
                IDArticolo = 0;
            if (pos_dep < 0)
                pos_dep = 0;
            strsql.printf("INSERT INTO ArticoliUDC (IdArtUdc, PosDest,IDArticolo,HEXStringa ,CorsiaDeposito,UltimoIdUdc,codice)" " VALUES (%d,%d,%d,'%s','%s',%d,'%s')"
                , id_articolo, pos_dep, IDArticolo, hex_string.c_str(), CorsiaDeposito.c_str(), ultimoidudc, Codice);
            ADOQuery->SQL->Text = strsql;
            res = ADOQuery->ExecSQL();
            if (res > 0)
                res = id_articolo;
            ADOQuery->Close();
        }
        else {
            strsql = "UPDATE ArticoliUDC SET ";
            if (pos_dep > -1) {
                if (virgola)
                    strsql = strsql + " , ";
                strsql = strsql + "PosDest = " + IntToStr(pos_dep);
                virgola = true;
            }
            if (IDArticolo > -1) {
                if (virgola)
                    strsql = strsql + " , ";
                strsql = strsql + "IDArticolo = " + IntToStr(IDArticolo);
                virgola = true;
            }
            if (hex_string != "") {
                if (virgola)
                    strsql = strsql + " , ";
                strsql = strsql + "HEXStringa = '" + hex_string + "'";
                virgola = true;
            }
            if (Codice != "") {
                if (virgola)
                    strsql = strsql + " , ";
                strsql = strsql + "codice = '" + Codice + "'";
                virgola = true;
            }
            if (CorsiaDeposito != "") {
                if (virgola)
                    strsql = strsql + " , ";
                strsql = strsql + "CorsiaDeposito = '" + CorsiaDeposito + "'";
                virgola = true;
            }
            if (ultimoidudc > -1) {
                if (virgola)
                    strsql = strsql + " , ";
                strsql = strsql + "ultimoidudc = " + IntToStr(ultimoidudc);
                virgola = true;
            }
            strsql = strsql + " where IdArtUdc = " + IntToStr(id_articolo);
            ADOQuery->SQL->Text = strsql;
            res = ADOQuery->ExecSQL();
            if (res > 0)
                res = id_articolo;
        }
        LogMsg(strsql);
    }
    catch (...) {
        // ADOConnection1->Close();
        LogMsg("Eccezione creazione ArticoloUDC, stringa :" + strsql);
        res = 0;
    }
    delete ADOQuery;
    aggiorna_tab_posizioni_locale = 1;
    return res;
}

int TdmDB::ReturnIdArticoloPrdottoDaIdArtUdc(int IdArticoloUdc) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select * from ArticoliUdc where IDArtUDC = " + IntToStr(IdArticoloUdc);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("IDArticolo")->AsInteger;
        }
        ADOQuery->Close();
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

int TdmDB::ReturnUltimoIdUdcDaArticoloUdc(int IdArticoloUdc) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select * from ArticoliUdc where IDArtUDC = " + IntToStr(IdArticoloUdc);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("UltimoIdUdc")->AsInteger;
        }
        ADOQuery->Close();
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

int TdmDB::AggiornaUltimoIdUdcPerArticoloUdc(int IdArticoloUdc, int IdUdc) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Update ArticoliUdc set UltimoIdUdc = " + IntToStr(IdUdc) + " where IDArtUDC = " + IntToStr(IdArticoloUdc);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->ExecSQL();
        ADOQuery->Close();
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

int TdmDB::AggiornaStatoUdc(int stato, int IdUdc) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Update Udc set Stato = " + IntToStr(stato) + " where IdUdc = " + IntToStr(IdUdc);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->ExecSQL();
        ADOQuery->Close();
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

int TdmDB::AggiornaTipoUdc(int tipo, int IdUdc) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Update Udc set codtipoudc = " + IntToStr(tipo) + " where IdUdc = " + IntToStr(IdUdc);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->ExecSQL();
        ADOQuery->Close();
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

int TdmDB::AgvSuBaia(int baia) {
    int i, res = 0;

    /* for ( i = 1 ; i<= NAGV ; i++) {
     if ((MainForm->DatiAgv[i].pos > 0 )&&
     ((MainForm->DatiAgv[i].pos == MainForm->DatiBaie[baia].pos_prel )||
     (MainForm->DatiAgv[i].pos == MainForm->DatiBaie[baia].pos_dep[1] )||
     (MainForm->DatiAgv[i].pos == MainForm->DatiBaie[baia].pos_dep[2] )||
     (MainForm->DatiAgv[i].pos == MainForm->DatiBaie[baia].pos_dep[3] ))) {
     res = i ;
     }
     } */
    return res;
}

int TdmDB::AgvInPosCb(int agv) {
    int i, j, res = 0;

    /* for ( i = 1 ; i<= NAGV ; i++) {
     if ((agv == i)||(agv == 0)) {
     for ( j = 1 ; j<= MainForm->num_pos_cb ; j++) {
     if (MainForm->DatiAgv[i].pos == MainForm->poscb[j] ) {
     res =  MainForm->poscb[j] ;
     break ;
     }
     }
     }
     } */
    return res;
}

int TdmDB::AgvInPos(int pos) {
    int i, res = 0;

    for (i = 1;
        i <= NAGV;
        i++) {
        if (ClientData.DatiAgv[i].pos == pos) {
            res = i;
            break;
        }
    }
    return res;
}

int TdmDB::AgvInDest(int pos) {
    int i, res = 0;

    for (i = 1;
        i <= NAGV;
        i++) {
        if (ClientData.DatiAgv[i].dest == pos) {
            res = i;
            break;
        }
    }
    return res;
}

int TdmDB::AgvInDestCb(int agv) {
    int i, j, res = 0;

    /* for ( i = 1 ; i<= NAGV ; i++) {
     if ((agv == i)||(agv == 0)) {
     for ( j = 1 ; j<= MainForm->num_pos_cb ; j++) {
     if ((ClientData.DatiAgv[i].generata)&&(ClientData.DatiAgv[i].dest == MainForm->poscb[j] )) {
     res =  MainForm->poscb[j] ;
     break ;
     }
     }
     }
     } */
    return res;
}

int TdmDB::UltimoIDMissione() {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;
    TLocateOptions Opts;
    Opts.Clear();
    int idmis = 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT TOP 1 ID FROM Missioni order by ID DESC");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        idmis = ADOQuery->FieldByName("ID")->AsInteger;

        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    // ultimo_id_articolo = idsrticolo ; // me lo salvo perchè se creo un articolo in manuale e un oin auto non ddeve avere l ostesso id
    return idmis;
}

AnsiString TdmDB::RitornaCodiceUDCdaUDC(int UDC) {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;

    AnsiString idmis = "";
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT CodUDC FROM UDC where IDUDC=%d", UDC);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        if (ADOQuery->RecordCount) {
            idmis = ADOQuery->FieldByName("CODUDC")->AsString.Trim();
        }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    // ultimo_id_articolo = idsrticolo ; // me lo salvo perchè se creo un articolo in manuale e un oin auto non ddeve avere l ostesso id
    return idmis;
}

int TdmDB::RitornaUDCdaCodUDC(AnsiString CodUdc) {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;

    int idmis = 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("SELECT idudc FROM UDC where ltrim(rtrim(CodUdc))='%s'", CodUdc.Trim());
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        if (ADOQuery->RecordCount) {
            idmis = ADOQuery->FieldByName("idudc")->AsInteger;
        }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    // ultimo_id_articolo = idsrticolo ; // me lo salvo perchè se creo un articolo in manuale e un oin auto non ddeve avere l ostesso id
    return idmis;
}

AnsiString TdmDB::ControllaCorsiaDestinazione(AnsiString Corsia) {
    TADOQuery *ADOQuery;
    AnsiString strsql;
    AnsiString ret = "";
    // eccezione per destinazione tgw
    if (Corsia.SubString(1, 4) == "TW03") {
        ret = Corsia.Trim();
        // ret = ret.SubString1, ret.Length() - 2) + "00";
    }
    else {
        try {
            ADOQuery = new TADOQuery(NULL);
            ADOQuery->Connection = ADOConnection1;
            strsql = "SELECT nomecorsia FROM corsie where LTrim(RTrim(nomecorsia))='" + Trim(Corsia) + "'";
            ADOQuery->SQL->Text = strsql;
            ADOQuery->Open();
            ADOQuery->Last();

            if (ADOQuery->RecordCount) {
                ret = ADOQuery->FieldByName("nomecorsia")->AsString;
            }
            ADOQuery->Close();
        }
        catch (...) {
        }
        delete ADOQuery;
        // ultimo_id_articolo = idsrticolo ; // me lo salvo perchè se creo un articolo in manuale e un oin auto non ddeve avere l ostesso id

    }

    return ret;
}

int TdmDB::RitornaDepositoSelettivo(AnsiString Corsia) {
    TADOQuery *ADOQuery;
    AnsiString strsql;
    int ret = 0;

    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql = "SELECT depositoselettivo FROM corsie where LTrim(RTrim(nomecorsia))='" + Trim(Corsia) + "'";
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();

        if (ADOQuery->RecordCount) {
            ret = ADOQuery->FieldByName("depositoselettivo")->AsInteger;
        }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    // ultimo_id_articolo = idsrticolo ; // me lo salvo perchè se creo un articolo in manuale e un oin auto non ddeve avere l ostesso id

    return ret;
}

int TdmDB::RitornaAltezzedaPosizione(int pos, int piano, AnsiString stringaprelievo) {
    int res = 0;
    TRecordList TabPiani;
    dmDB->CaricaTabella("piani_view where pos = " + String(pos) + " order by piano", TabPiani);
    res = TabPiani[piano - 1][stringaprelievo].ToIntDef(0);
    return res;
}

int TdmDB::RitornaAltezzadaPosizioneNuovo(int pos, int piano, AnsiString stringaprelievo, int corsia)
{
    int res = 0;
    TADOQuery *ADOQuery;
    AnsiString strsql;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql = "SELECT " + String(stringaprelievo) + " FROM piani_view WHERE pos = " + String(pos) + " AND fila = " + String(corsia) + " AND piano = " + String(piano);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();

        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName(stringaprelievo)->AsInteger;
        }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;

    return res;
}

int TdmDB::PrimoPianoOk(int pos) {
    int res = 0, cont;
    TADOQuery *ADOQuery;
    AnsiString strsql;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql = "select (SELECT COUNT(*) AS Expr1 FROM dbo.Piani AS Piani_1 WHERE (pos = dbo.Piani_View.Pos) AND (IDUDC <> 0)) as npianiocc,";
        strsql += " (SELECT COUNT(*) AS Expr1 FROM dbo.Piani WHERE (pos = dbo.Piani_View.Pos)) AS npiani from piani_view ";
        strsql += " Where pos=" + String(pos);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->First();

        if (ADOQuery->RecordCount) {
            if (ADOQuery->FieldByName("npianiocc")->AsInteger + 1 <= ADOQuery->FieldByName("npiani")->AsInteger)
                res = ADOQuery->FieldByName("npianiocc")->AsInteger + 1;
        }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;

    return res;
}

int TdmDB::ControllaSopraESotto(int pos, int piano) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0, sopraok = 0, sottook = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        // sopra
        stringa = "select count (*) as conta from piani where pos=" + String(pos) + " and idudc>0 and piano>" + String(piano);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        res = ADOQuery->FieldByName("conta")->AsInteger;
        // sopra deve contare 0 pallet
        if (res == 0)
            sopraok = 1;
        // sotto
        stringa = "select count (*) as conta from piani where pos=" + String(pos) + " and idudc>0 and piano<" + String(piano);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        res = ADOQuery->FieldByName("conta")->AsInteger;
        if (res == piano - 1)
            sottook = 1;
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
        SettaAllarmePos(pos, (sottook + sopraok) % 2);
    }
    catch (...) {

    }
    delete ADOQuery;

    return sottook + sopraok;
}

int TdmDB::SettaAllarmePos(int pos, int allarme) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0, pren;
    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa.printf("UPDATE Posizioni SET " " allarme = %d " " where Pos = %d ", allarme, pos);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return res;
}

AnsiString TdmDB::RitornaCodiceUDCdaPosinMissione(int pos) {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;

    AnsiString codudc = "";
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql.printf("select CODUDC FROM Missioni_View WHERE posprel=%d and (Tempo IS NULL) and (prelevato IS NULL) ORDER BY Generata DESC", pos);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        if (ADOQuery->RecordCount)
            codudc = ADOQuery->FieldByName("CODUDC")->AsString.Trim();
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return codudc;
}

void TdmDB::SettaRifOrdineSuPos(int pos, AnsiString RifOrdine) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE PosizioniConDatiUDC SET RifOrdine = '" + RifOrdine + "'";
        stringa += " where Pos = " + IntToStr(pos) + " and npianiocc=0";
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg(" Setta RifOrdine " + RifOrdine + " su Pos " + IntToStr(pos));
        delete ADOQuery;
        aggiorna_tab_posizioni_locale = true;
    }
    catch (...) {
    }
}

void TdmDB::MissioneElaborataSuAgvX(int idmiss, int causale)
{
    AnsiString str_missione = "";
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;
    /* causali errore missione ricevuta :
     0 missione corretta ricevuta da Agvx
     1 missione assegnata non modificabile generico
     2 missione asssegnata non modificabile per deadlock
     3 missione asssegnata non modificabile per newtask
     4 missione asssegnata non modificabile perchè stato carico cambiato su agv
     5 missione assegnata non modificabile per eccezione
     6 id missione già presente
     7 posizione prelievo o deposito non esistenti
     8 posprel = posdep con tipo missione di movimento
     9 tab carico o scarico non presenti su posprel o posdep
     10 idmissione 0 non accettato
     11 missione di solo prelievo per agv già carico non accettata
     12 missione di solo deposito per agv scarico non accettata
     */
    try {
        if (!ADOConnection1->Connected) {
            return;
        }
        switch (causale) {
        case 0:
            str_missione = "Missione Ok";
            break;
        case 1:
            str_missione = "ERR : 1 Missione assegnata non modificabile generico";
            break;
        case 2:
            str_missione = "ERR : 2 missione asssegnata non modificabile per deadlock ";
            break;
        case 3:
            str_missione = "ERR : 3 missione asssegnata non modificabile per newtask ";
            break;
        case 4:
            str_missione = "ERR : 4 missione asssegnata non modificabile perchè stato carico cambiato su agv";
            break;
        case 5:
            str_missione = "ERR : 5 missione assegnata non modificabile per eccezione ";
            break;
        case 6:
            str_missione = "ERR : 6 id missione già presente ";
            break;
        case 7:
            str_missione = "ERR : 7 posizione prelievo o deposito non esistenti ";
            break;
        case 8:
            str_missione = "ERR : 8 posprel = posdep con tipo missione di movimento ";
            break;
        case 9:
            str_missione = "ERR : 9 tab carico o scarico non presenti su posprel o posdep ";
            break;
        case 10:
            str_missione = "ERR : 10 idmissione 0 non accettato ";
            break;
        case 11:
            str_missione = "ERR : 11 missione di solo prelievo per agv già carico non accettata ";
            break;
        case 12:
            str_missione = "ERR : 12 missione di solo deposito per agv scarico non accettata";
            break;
        }
        str_missione = str_missione.SubString(1, 99);
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql = "UPDATE Missioni SET Risposta_Agvx = '" + str_missione + "'";
        strsql = strsql + " where id = " + IntToStr(idmiss);
        ADOQuery->SQL->Text = strsql;
        res = ADOQuery->ExecSQL();
        if (!causale)
            LogMsg("Missione  " + IntToStr(idmiss) + " elaborata da Agv X correttamente");
        else
            LogMsg("Missione  " + IntToStr(idmiss) + " non elaborata da Agv X correttamente, msg " + str_missione);
        // vedere cosa fare
    }
    catch (...) {
    }
}

int TdmDB::RitornaZonaDaPos(int pos) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select fila from Posizioni where Pos = " + IntToStr(pos);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("fila")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDB::RitornaPosizioneDaPos(int pos) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select posinfila from Posizioni where Pos = " + IntToStr(pos);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("posinfila")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDB::RitornaNumPosDaPos(int pos) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select numpos from piani_view where Pos = " + IntToStr(pos);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("numpos")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDB::ReturnIDUdcDaPosePiano(int pos, int piano) {
    AnsiString stringa;
    AnsiString strsql;
    int res = 0;

    TADOQuery *ADOQuery;

    try {
        if (!ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;

        stringa = "Select * from piani inner join Udc on piani.idudc=udc.idudc where piani.Pos = " + IntToStr(pos) + " and piani.Piano = " + IntToStr(piano);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("IdUdc")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

void TdmDB::SettaPosSelezionata(int pos, int setta, int zona) {
    // setta il campo selezionata sulla posizione data. Se viene passata la zone setta su tutta la zona
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        if (zona == 0) {
            stringa = "UPDATE posizioni SET Selezionata = " + IntToStr(setta);
            stringa += " where pos = " + IntToStr(pos);
        }
        else {
            stringa = "UPDATE posizioni SET Selezionata = " + IntToStr(setta);
            stringa += " where tipoposizione = " + IntToStr(zona);
        }
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Settato Selezionata su pos " + IntToStr(pos) + ", valore : " + IntToStr(setta));
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
}

void TdmDB::SettaPosPrenotata(int pos, int setta) {
    // setta il campo selezionata sulla posizione data. Se viene passata la zone setta su tutta la zona
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE posizioni SET prenotata = " + IntToStr(setta);
        stringa += " where pos = " + IntToStr(pos);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Settato Prenotata su pos " + IntToStr(pos) + ", valore : " + IntToStr(setta));
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
}

void TdmDB::SettaDestinazioneInZona(int fila, int setta) {
    // setta il campo selezionata sulla posizione data. Se viene passata la zone setta su tutta la zona
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "UPDATE corsie SET destinazione = " + IntToStr(setta);
        stringa += " where fila = " + IntToStr(fila);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Settato destinazione su fila " + IntToStr(fila) + ", valore : " + IntToStr(setta));
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
}

int TdmDB::RitornaPosizioneDisabilitata(int pos) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select disabilitata, pos_disabilita from piani_view where Pos = " + IntToStr(pos);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            if ((ADOQuery->FieldByName("disabilitata")->AsInteger) || (ADOQuery->FieldByName("pos_disabilita")->AsInteger))
                res = 1;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDB::RitornaPosizionePrenotata(int pos) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select Prenotata, pos_Prenotata from piani_view where Pos = " + IntToStr(pos);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            if ((ADOQuery->FieldByName("Prenotata")->AsInteger) || (ADOQuery->FieldByName("pos_Prenotata")->AsInteger))
                res = 1;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;

}

int TdmDB::RitornaPosizioneSelezionata(int pos) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select Selezionata from piani_view where Pos = " + IntToStr(pos);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0)
            res = ADOQuery->FieldByName("Selezionata")->AsInteger;

        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDB::LeggiDestinazioneInZona(int fila) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select destinazione from corsie where fila = " + IntToStr(fila);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0)
            res = ADOQuery->FieldByName("destinazione")->AsInteger;

        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDB::ContaMissioniAttive(int agv) {
    TADOQuery *ADOQuery;
    AnsiString strsql, clausolawhere = " (fine IS NULL) ";
    int ret = 0;

    if (agv > 0) {
        clausolawhere = clausolawhere + " and agv=" + IntToStr(agv);
    }

    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql = "select count(*) as ccont from missioni where " + clausolawhere;
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        ret = ADOQuery->FieldByName("ccont")->AsInteger;
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return ret;
}

void TdmDB::ImpostaChiamataPosizione(int pos, int stato) {
    int i, posplc, res;
    AnsiString stringa;
    TADOQuery *ADOQuery;

    // setto chiamata sul db
    try {
        if (!dmDB->ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa.printf("UPDATE posizioni SET chiamata = %d where pos = %d  ", stato, pos);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg(stringa);
        delete ADOQuery;
    }
    catch (...) {}
}

int TdmDB::RitornaOrdinamentoDaPos(int pos) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select ordinamentocorsia from piani_view where Pos = " + IntToStr(pos);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("ordinamentocorsia")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDB::RitornaTipoUDCDaPos(int pos) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select tipoudc from Posizioni where Pos = " + IntToStr(pos);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("tipoudc")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDB::ControllaPosizioneOK(int pos, int piano) {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;
    int ret = 0;
    // faccio la select solo se il tipoudc e' per forche lunghe
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql = "select count(*) as ccont from piani_view where pos=" + String(pos) + " and piano=" + String(piano);
        strsql += " and disabilitata=0 and pos_disabilita=0 and abilitata=1 and prenotata=0 and pos_prenotata=0";
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        ret = ADOQuery->FieldByName("ccont")->AsInteger;
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return ret;
}

void TdmDB::SettaPianoSelezionato(int pos, int piano, int setta, int zona) {
    // setta il campo selezionata sulla posizione data. Se viene passata la zone setta su tutta la zona
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res;
    try {
        if (!ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        if (!zona) {
            stringa = "UPDATE piani SET Selezionata = " + IntToStr(setta);
            stringa += " where pos = " + IntToStr(pos);
            stringa += " and piano = " + IntToStr(piano);
        }
        else {
            stringa = "UPDATE piani_view SET Selezionata = " + IntToStr(setta);
            stringa += " where zona = " + IntToStr(zona);
        }

        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        res = ADOQuery->ExecSQL();
        ADOQuery->Close();
        dmDB->LogMsg("Settato piano " + IntToStr(piano) + " su pos " + IntToStr(pos) + ", valore : " + IntToStr(setta));
        delete ADOQuery;
    }
    catch (...) {
    }
}

void TdmDB::FullTabellaK(AnsiString TableName, AnsiString KeyField, TIndexList & RecList) {
    AnsiString strsql, campo, chiave, valore;
    TADOQuery *ADOQuery;
    Variant k;
    int j, i = 0;

    if (!dmDB->ADOConnection1->Connected)
        return;
    RecList.clear();
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql.printf("%s", TableName.c_str());
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        while (!ADOQuery->Eof) {
            for (j = 0; j < ADOQuery->FieldCount; j++) {
                chiave = ADOQuery->FieldByName(KeyField)->AsString;
                campo = ADOQuery->FieldList->Fields[j]->FieldName.UpperCase();
                valore = ADOQuery->FieldList->Fields[j]->AsString;
                RecList[chiave][campo] = valore;
            }
            ADOQuery->Next();
            i++;
        }
        ADOQuery->Close();
    }
    catch (...) {
        dmDB->ADOConnection1->Close();
    }
    delete ADOQuery;
}
// ---------------------------------------------------------------------------

int TdmDB::RitornaUDCdaPosPiano(int pos, int piano) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select idudc from piani_view where Pos = " + IntToStr(pos) + " and piano=" + IntToStr(piano);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0)
            res = ADOQuery->FieldByName("idudc")->AsInteger;

        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDB::InsertUpdateUDCeArticolo(TUDC UDC, TArticoli &Articolo) {
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0, dacreare = 0, idarttemp;
    AnsiString Val, CodUDC = "", CodTipoSpedizione = ""; ;

    if (!ADOConnection1->Connected)
        return 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        if (UDC.IDUDC == 0) {
            UDC.IDUDC = CreaIdUDC();
            dacreare = 1;
        }
        // se CodUDC e' vuoto, inserisco  IDUDc come CodUDC
        if (CodUDC.Trim() == "") {
            CodUDC = String(UDC.IDUDC);
        }
        else {
            CodUDC = String(UDC.Articolo.CodArt);
        }
        Articolo.IDArticolo = UDC.IDUDC;
        if ((dacreare) || (!UDCPresenteInArchivio(UDC.IDUDC, idarttemp))) {
            ADOQuery = new TADOQuery(NULL);
            ADOQuery->Connection = ADOConnection1;
            strsql.printf("INSERT INTO UDC (IDUDC ,CodUDC, CodStatoUDC, IDArticolo, CodTipoUDC, Tara, PesoAttuale, Lotto, IndiceImpilabilita, Parziale, Riservato)"
                " VALUES (%d,'%s',%d,%d,%d,%d,%d,'%s',%d, %d, %d)"
                , UDC.IDUDC
                , CodUDC.c_str()
                , UDC.CodStato
                , Articolo.IDArticolo
                , UDC.CodTipoUDC
                , 0
                , 0
                , UDC.Lotto
                , UDC.IndiceImpilabilita
                , UDC.Parziale
                , UDC.Riservato
                );

            ADOQuery->SQL->Text = strsql;
            res = ADOQuery->ExecSQL();
            if (res) {
                res = UDC.IDUDC; // ritorno ID creato
                // creo articolo
                UDC.Articolo.IDArticolo = InsertArticoli(Articolo);
            }
            ADOQuery->Close();
        }
        else {
            res = UDC.IDUDC;
            if ((UDC.CodStato > 0) && (UDC.Articolo.IDArticolo > 0)) {
                strsql.printf("UPDATE UDC SET " "CodStatoUDC = %d , " "IDArticolo = %d, " "Tara = %d, " "PesoAttuale = %d, " "IndiceImpilabilita = %d, " "Lotto = '%s', " "Parziale =%u, " " Riservato=%d " " where IDUDC = %d "
                    , UDC.CodStato
                    , UDC.Articolo.IDArticolo
                    , 0
                    , 0
                    , UDC.IndiceImpilabilita
                    , UDC.Lotto
                    , UDC.Parziale
                    , UDC.Riservato
                    , UDC.IDUDC);

                ADOQuery->SQL->Text = strsql;
                res = ADOQuery->ExecSQL();
                if (res)
                    res = UDC.IDUDC; // ritorno ID creato
            }
        }
        LogMsg(strsql);
    }
    catch (...) {
        // ADOConnection1->Close();
        LogMsg("Eccezione creazione UDC, stringa :" + strsql);
        UDC.IDUDC = 0;
        res = 0;
    }
    delete ADOQuery;
    aggiorna_tab_posizioni_locale = 1;
    return res;
}

int TdmDB::InsertUpdateUDC(TUDC UDC) {
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0, dacreare = 0, idarttemp;
    AnsiString Val, CodUDC = "", CodTipoSpedizione = ""; ;
    AnsiString descrizione;

    if (!ADOConnection1->Connected)
        return 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        if (UDC.IDUDC == 0) {
            UDC.IDUDC = CreaIdUDC();
            dacreare = 1;
        }
        if (UDC.Articolo.CodArt == NULL) {
            CodUDC = String(UDC.IDUDC);
        }
        else {
            CodUDC = String(UDC.Articolo.CodArt); // dmFunzioni->DaBufferAStringa(UDC.Articolo.CodArt, 20);
        }
        descrizione = String(UDC.Articolo.Descrizione);
        descrizione = StringReplace(descrizione, '"', "_", TReplaceFlags() << rfReplaceAll);
        descrizione = StringReplace(descrizione, "'", "_", TReplaceFlags() << rfReplaceAll);
        descrizione = Trim(descrizione);

        if ((dacreare) || (!UDCPresenteInArchivio(UDC.IDUDC, idarttemp))) {
            ADOQuery = new TADOQuery(NULL);
            ADOQuery->Connection = ADOConnection1;
            strsql.printf("INSERT INTO UDC (IDUDC ,CodUDC, Stato, IDArticolo, CodTipoUDC, Tara, PesoAttuale, Lotto, IndiceImpilabilita, Parziale, Riservato, Descrizione)"
                " VALUES (%d,'%s',%d,%d,%d,%d,%d,'%s',%d, %d, %d,'%s')"
                , UDC.IDUDC
                , CodUDC.c_str()
                , UDC.CodStato
                , UDC.Articolo.IDArticolo
                , UDC.CodTipoUDC
                , 0
                , 0
                , UDC.Lotto
                , UDC.IndiceImpilabilita
                , UDC.Parziale
                , UDC.Riservato
                , descrizione.c_str()
                );

            ADOQuery->SQL->Text = strsql;
            res = ADOQuery->ExecSQL();
            if (res) {
                res = UDC.IDUDC; // ritorno ID creato
            }
            ADOQuery->Close();
        }
        else {
            res = UDC.IDUDC;
            if (1==1)  { //((UDC.CodStato > 0) && (UDC.Articolo.IDArticolo > 0)) {
                strsql.printf("UPDATE UDC SET Descrizione='%s', CodTipoUDC=%d,  CodUDC ='%s', stato = %d , IDArticolo = %d, Tara = %d, PesoAttuale = %d, IndiceImpilabilita = %d, Lotto = '%s', Parziale =%u,  Riservato=%d  where IDUDC = %d "
                    , descrizione.c_str()
                    , UDC.CodTipoUDC
                    , CodUDC.c_str()
                    , UDC.CodStato
                    , UDC.Articolo.IDArticolo
                    , 0
                    , 0
                    , UDC.IndiceImpilabilita
                    , UDC.Lotto
                    , UDC.Parziale
                    , UDC.Riservato
                    , UDC.IDUDC);

                ADOQuery->SQL->Text = strsql;
                res = ADOQuery->ExecSQL();
                if (res)
                    res = UDC.IDUDC; // ritorno ID creato
            }
        }
        LogMsg(strsql);
    }
    catch (...) {
        // ADOConnection1->Close();
        LogMsg("Eccezione creazione UDC, stringa :" + strsql);
        UDC.IDUDC = 0;
        res = 0;
    }
    delete ADOQuery;
    aggiorna_tab_posizioni_locale = 1;
    return res;
}

int TdmDB::UDCPresenteInArchivio(int idudc, int &idarticolo) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        idarticolo = 0;
        stringa = "Select IDUDC,IDArticolo from UDC where IdUDC = " + IntToStr(idudc);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = idudc;
            idarticolo = ADOQuery->FieldByName("IDArticolo")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {

    }
    delete ADOQuery;
    return res;
}

int TdmDB::InsertArticoli(TArticoli &Articoli) {
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0, dacreare = 0, virgola = 0;
    AnsiString Val;

    if (!ADOConnection1->Connected)
        return 0;

    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        if (!ArticoloPresenteInArchivio(Articoli.IDArticolo)) {
            ADOQuery = new TADOQuery(NULL);
            ADOQuery->Connection = ADOConnection1;
            strsql.printf("INSERT INTO Articoli (idarticolo, CodArt, Descrizione, codtipoarticolo, Descrizione2,PezziPerPiano,PianiPerUDC,Peso)"
                " VALUES (%d,'%s','%s',%d,'%s',%d,%d,%d)"
                , Articoli.IDArticolo
                , Articoli.CodArt
                , Articoli.Descrizione
                , Articoli.IDTipoArticolo
                , Articoli.Descrizione2
                , Articoli.PezziPerPiano
                , Articoli.PianiPerUDC
                , Articoli.Peso
                );
            ADOQuery->SQL->Text = strsql;
            res = ADOQuery->ExecSQL();
            if (res > 0)
                res = Articoli.IDArticolo;
            ADOQuery->Close();
        }
        LogMsg(strsql);
    }
    catch (...) {
        // ADOConnection1->Close();
        LogMsg("Eccezione creazione ArticoloUDC, stringa :" + strsql);
        res = 0;
    }
    delete ADOQuery;
    aggiorna_tab_posizioni_locale = 1;
    return res;
}

int TdmDB::IDUDCdaIDArticolo(int IDArticolo) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select IDUDC from UDC_View where IDArticolo = " + IntToStr(IDArticolo);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("IDUDC")->AsInteger;
        }
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return res;
}

void TdmDB::LeggiStrutturaUdc(TUDC & UDC) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    // leggo la struttura idudc dal database

    try {
        if (!ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select * from UDC_view where IdUdc = " + IntToStr(UDC.IDUDC) + " and idudc>0";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            UDC.IDUDC = ADOQuery->FieldByName("IDUDC")->AsInteger;
            UDC.CodTipoUDC = ADOQuery->FieldByName("CodTipoUDC")->AsInteger;
            UDC.IdArtUDC = 0;
            UDC.Articolo.IDArticolo = ADOQuery->FieldByName("IDArticolo")->AsInteger;

            if (ADOQuery->FieldByName("idarticolo")->AsInteger == 0) {
                dmExtraFunction->StringToChar(ADOQuery->FieldByName("descudc")->AsString, UDC.Articolo.Descrizione);
                dmExtraFunction->StringToChar(ADOQuery->FieldByName("CodUDC")->AsString, UDC.Articolo.CodArt);
            }
            else {
                dmExtraFunction->StringToChar(ADOQuery->FieldByName("DescArt")->AsString, UDC.Articolo.Descrizione);
                dmExtraFunction->StringToChar(ADOQuery->FieldByName("codart")->AsString, UDC.Articolo.CodArt);
            }
            UDC.Articolo.IDTipoArticolo = 0; // ??????
            UDC.CodStato = ADOQuery->FieldByName("Stato")->AsInteger;
            UDC.IndiceImpilabilita = ADOQuery->FieldByName("Sovrapposto")->AsString.ToIntDef(0);
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {

    }
    delete ADOQuery;
}

int TdmDB::IDUDCdaCodart(AnsiString CodArt) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select IDUDC from UDC_View where CodArt = '" + CodArt + "' or codudc='" + CodArt + "'";
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("IDUDC")->AsInteger;
        }
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return res;
}

int TdmDB::RitornaLatoForcheDaNomePosizione(AnsiString NomePos) {
    int ret = 0;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int lato = 0;

    if (!dmDB->ADOConnection1->Connected)
        return 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql.printf("SELECT Lato FROM piani_view where NomePos='%s'", NomePos);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->First();
        if (ADOQuery->RecordCount) {
            lato = ADOQuery->FieldByName("Lato")->AsInteger;
        }
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {}
    dmDB->LogMsg("Ritorna lato da posizione " + NomePos + " lato : " + IntToStr(lato));
    return lato;
}

int TdmDB::RitornaAgvDaIdPallet(int idPallet) {
    for (int agv = 1; agv <= NAGV; agv++) {
        if (ClientData.DatiAgv[agv].DatiUDC.IDUDC == idPallet) {
            return agv;
        }
    }
    return 0;
}

int TdmDB::BloccoMissione() {
    int ret = 0, giorno = 0;
    AnsiString ora, orada, oraa;
    TIndexList TabParametri;
    // NON SERVE MA LASCIO A USO FUTURO
    TDateTime adesso;
    WORD adessoY, adessoM, adessoD, Y, M, D, Hour, Min, Sec, MSec;
    adesso = Now();
    DecodeDate(adesso, adessoY, adessoM, adessoD);
    DecodeTime(adesso, Hour, Min, Sec, MSec);
    //
    ora = TimeToStr(Now());
    giorno = DayOfWeek(Now());
    dmDB->CaricaTabellaK("Parametri where idparametro>=60 and idparametro<=75 order by IdParametro", "IdParametro", TabParametri);
    for (int i = 0; i <= 4; i = i + 2) {
        if ((giorno >= TabParametri[60 + i]["INTVALUE"].ToIntDef(0)) && (giorno <= TabParametri[61 + i]["INTVALUE"].ToIntDef(0))) {
            orada = TabParametri[70 + i]["STRINGVALUE"];
            oraa = TabParametri[71 + i]["STRINGVALUE"];
            if ((ora >= orada) && (ora <= oraa)) {
                ret++;
            }
        }
    }
    return ret;
}

int TdmDB::TornaNodoFermataDaPosizioni(int pos) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        stringa = "Select NodoFermata from Posizioni where pos = " + IntToStr(pos);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("NodoFermata")->AsInteger;
        }
        ADOQuery->Close();
    }
    catch (...) {

    }
    delete ADOQuery;
    return res;
}

int TdmDB::PresenzaCentroMissionePerPosizioni(int posprel, int posdep, int stato) {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;
    TLocateOptions Opts;
    Opts.Clear();
    int res = 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = ADOConnection1;
        strsql = "SELECT id FROM CentroMissioni where (fine is null) and stato=" + IntToStr(stato) + " and ((posprel =" + IntToStr(posprel) + ") and (posdep = " + IntToStr(posdep) + "))";
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();

        res = ADOQuery->RecordCount;
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}
