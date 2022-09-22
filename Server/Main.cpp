// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "logtxt.h"
#include "DB.h"
#include "percorsiprototipo.h"
#include "socket.h"
#include "supervisionesocket.h"
#include "login.h"
#include "dmFunzioniComuniClientServer.h"
#include "PLCThread.h"
#include "PLCphoenixThread.h"
#include "SocketComunicazioneServer.h"
#include "dmgestione.h"
#include "dmgestionedatabase.h"
#include "ExtraFunction.h"
#include "datamodulecomandiagv.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MessageBar"
#pragma link "MessageBar"
#pragma resource "*.dfm"
char File_name[50];
int MISSIONS_PORT = 20000; /* porta su cui AgvX è server per ricezione missioni */
int MISS_STATE_PORT = 20001; /* porta su cui AgvX invia stato missioni a AgvOrd */
int AGV_REC_PORT = 20002; /* porta su cui AgvX è server per ricevere eventi Agv */
int AGV_SEND_PORT = 20003; /* porta su cui AgvX invia comandi Agv a AgvComm */
char HOST_COM_ADDRESS[20] = "127.0.0.1";
char HOST_ORD_ADDRESS[20] = "127.0.0.1";
char HOST_AGVX_ADDRESS[20] = "127.0.0.1";

DATILGV tab;

CLIENTSOCKET *sock;
CLIENTSOCKET *cs;
SUPERVISIONEMISS_STATESOCKET *ss;

TMainForm *MainForm;
TClientData ClientData;

// ---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
    int lang, i, j, plc;
    TCursor Save_Cursor = Screen->Cursor;
    FILE *f;
    char s[100];
    double fXo, fYo;
    double fH, fW, fFH, fFW;
    ShowWindow(Handle, SW_HIDE);

    Richiesta_chiusura = false;
    LogTxt("*** Avvio ***");
    Avvio = true;

    dmDB->LeggiParametri(82);

    LeggiParametri();
    tab.Load1("Trascar_HMC_prx.crs");

    for (i = 0; i < N_PLC; i++) {
        PLCThread[i] = new TPLCThread(true, i);
        PLCThread[i]->Resume();
    }

    PLCThread[0]->dbwatchdog_pc = 2000; // se presente abilito funzione watchdog   // DB DA METTERE A POSTO
    PLCThread[0]->wordwatchdog_pc = 0;
    PLCThread[0]->bit_watchdog_pc = 0;

    PLCThread[0]->dbwatchdog_plc = 2001;
    PLCThread[0]->wordwatchdog_plc = 0;
    PLCThread[0]->bit_watchdog_plc = 0;

    PLCThread[0]->typewatchdog = 1; // tipo 1 lo associo al pc che copia il watchdog del plc
    PLCThread[0]->num_alarm_watchdog = 101;

    for (int i = 0; i <= PLC_PHOENIX; i++) {
        PLCPhoenixThread[i] = new Phoenix_Thread(i + 1, true);
        PLCPhoenixThread[i]->Resume();
    }

    cs = new CLIENTSOCKET(HOST_AGVX_ADDRESS, MISSIONS_PORT);
    sock = new CLIENTSOCKET(HOST_COM_ADDRESS);
    ss = new SUPERVISIONEMISS_STATESOCKET;

    contatore = 0;
}

// ---------------------------------------------------------------------------
void TMainForm::LeggiParametri() {
    FILE *f;
    char s[80];

    int n;

    if ((f = fopen("params.txt", "r")) == NULL) {
        strcpy(File_name, "Trascar_HMC_prx.crs");
        return;

    }
    rewind(f);

    while (!feof(f)) {
        if (fscanf(f, "%s", s) != 1)
            break;
        if (!strcmp(s, "File_name")) {
            fscanf(f, "%s", File_name);
        }
        else if (!strcmp(s, "MISSIONS_PORT")) {
            fscanf(f, "%d", &MISSIONS_PORT);
        }
        else if (!strcmp(s, "MISS_STATE_PORT")) {
            fscanf(f, "%d", &MISS_STATE_PORT);
        }
        else if (!strcmp(s, "AGV_REC_PORT")) {
            fscanf(f, "%d", &AGV_REC_PORT);
        }
        else if (!strcmp(s, "AGV_SEND_PORT")) {
            fscanf(f, "%d", &AGV_SEND_PORT);
        }
        else if (!strcmp(s, "HOST_COM_ADDRESS")) {
            fscanf(f, "%s", HOST_COM_ADDRESS);
        }
        else if (!strcmp(s, "HOST_ORD_ADDRESS")) {
            fscanf(f, "%s", HOST_ORD_ADDRESS);
        }
        else if (!strcmp(s, "HOST_AGVX_ADDRESS")) {
            fscanf(f, "%s", HOST_AGVX_ADDRESS);
        }
    }
    fclose(f);
}

void __fastcall TMainForm::Exit1Click(TObject *Sender) {
    LogTxt("*** Chiusura ***");
#ifdef ISSERVICE
    WinExec("net stop Service", SW_HIDE);
#else
    Richiesta_chiusura = true;
    Close();
#endif
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::Apri1Click(TObject *Sender) {
    Application->Restore();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BitBtn1Click(TObject *Sender) {
    LogTxt("*** Chiusura ***");
#ifdef ISSERVICE
    WinExec("net stop Service", SW_HIDE);
#else
    Richiesta_chiusura = true;
    Close();
#endif
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    /* Application->MainFormOnTaskBar = false;
     Hide();
     WindowState = wsMinimized; */
    CanClose = Richiesta_chiusura;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    int uscita = 0;
    Timer1->Enabled = false;
    close_program = true;
    SocketDataModule->TimerSendClientData->Enabled = false;
    if (dmDB->lettostatoagv)
        dmDB->AggiornaStatoAgv(0);
    dmDB->LogMsg("Server Chiuso");

    for (int p = 0; p < N_PLC; p++) {
        try {
            PLCThread[p]->StopThread();
            while (!PLCThread[p]->Suspended) {
                Sleep(3);
            }
        }
        catch (...) {
        }
        delete PLCThread[p];
    }

    delete ss;
    delete cs;
    delete sock;
  //  PLCThread[0]->StopThread();
    for (int i = 0; i < PLC_PHOENIX; i++) {
        PLCPhoenixThread[i]->StopThread();
    }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::Timer1Timer(TObject *Sender) {
    int tempval = 0;
    TEdit *Ed;
    if (Avvio) {
        /* Application->MainFormOnTaskBar = false;
         Hide();
         WindowState = wsMinimized; */
        Avvio = false;
        count_inizio = 0;
    }

    else if (WindowState == wsNormal) {
        AccendiLed(0, (dmDB->ADOConnection1->Connected ? clLime : clRed));
        // plc
        for (int p = 0; p < N_PLC; p++) {
            AccendiLed(p + 1, (PLCThread[p]->connected ? clLime : clRed));
        }

        for (int p = 0; p < N_PLC; p++) {
            VisTCiclo(p + 1, PLCThread[p]->CycleTime);
            VisWatchDog(p + 1, IntToStr(PLCThread[p]->watchdog_plc), IntToStr(PLCThread[p]->watchdog_pc));
            Shape1->Hint = PLCThread[p]->ErrorString;
        }
        // plc porte
        for (int p = 0; p < PLC_PHOENIX; p++) {
            AccendiLed(p + 5, (PLCPhoenixThread[p]->connesso ? clLime : clRed));
        }


        Label8->Caption = SocketDataModule->ServerSocket1->Socket->ActiveConnections;
        StaticText1->Caption = "Server Agv Proxaut " + FormatDateTime("dd/mm/yyyy h':'mm':'ss", Now());
    }
    if (count_inizio <= 20)
        count_inizio++;
    else {
        /* if (RadioGroup1->ItemIndex == 0) {
         unsigned char buf[8] ;
         int id_edit = 5 ;
         for(int i =0 ; i<8 ; i++) {
         Ed = (TEdit *) FindComponent("Edit"+IntToStr(id_edit));
         buf[i] = PLCThread[0]->DB[184][46+i];
         Ed->Text =  buf[i];
         id_edit++ ;
         }
         Edit1->Text = dmFunzioni->ReturnDoubleDaBuffer(buf) ;
         } */
        // Memo1->Lines->Append(dmExtraFunction->bitSet(tempval, contatore % 16));
    }
    contatore++;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::ApplicationEvents1Minimize(TObject *Sender)
{
    /* Application->MainFormOnTaskBar = false;
     Hide();
     WindowState = wsMinimized; */
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::ApplicationEvents1Restore(TObject *Sender)
{
    Application->MainFormOnTaskBar = true;
    Show();
    WindowState = wsNormal;
    Application->BringToFront();
}
// ---------------------------------------------------------------------------

void TMainForm::MemoLog(AnsiString Evento)
{
    int i;
    /* if (Memo1->Lines->Count > 5000) {
     for (i = 0 ; i< 1000 ; i++)
     Memo1->Lines->Delete(0);
     }
     Memo1->Lines->Append(Evento); */
    dmDB->Log("SERVER", "EVENT", Evento);
}
// ---------------------------------------------------------------------------

void TMainForm::LogStatoAgv()
{
    TADOQuery *ADOQuery;
    AnsiString strsql, ev;
    /* int i ;
     try {
     ev = "";
     for (i = 0; i < NAGV; i++) {
     strsql.printf("AGV %d, Dest %d,stato %d,coil %s ;", i+1, dmTrafficManager->DatiAGV[i].Destinazione, ((dmTrafficManager->DatiAGV[i].StatoCarico == 'F') ? 1 : 0),dmDB->LeggiCoilSuAGV(i+1));
     ev += strsql ;
     }
     dmDB->Log("SERVER","EVENT", ev) ;
     } catch(...) {
     } */
    // delete ADOQuery;
}

void __fastcall TMainForm::TrayIcon1DblClick(TObject *Sender)
{
    if (WindowState == wsMinimized) {
        Application->MainFormOnTaskBar = true;
        Show();
        WindowState = wsNormal;
        Application->BringToFront();
    }
    else {
        /* Application->MainFormOnTaskBar = false;
         Hide();
         WindowState = wsMinimized; */
    }

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BitBtn3Click(TObject *Sender)
{
    FormLogin->Show();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BitBtn2Click(TObject *Sender)
{
    dmDB->aggiorna_tab_posizioni_locale = 1; // azzero eventuali agigonramenti client
    dmDB->aggiorna_tab_porte_locale = 1; // azzero eventuali agigonramenti client
    ClientData.aggiorna_allarmi = 1;

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::btStartClick(TObject *Sender)
{
    ClientData.ParametriFunzionali.AbilitaMissioni = true;
    dmDB->LogMsg("Ricevuto Richiesta di Start da server ");
    btStart->Enabled = false;
    btStop->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::btStopClick(TObject *Sender)
{
    ClientData.ParametriFunzionali.AbilitaMissioni = false;
    dmDB->LogMsg("Ricevuto Richiesta di Stop da server ");
    btStart->Enabled = true;
    btStop->Enabled = false;
}

// ---------------------------------------------------------------------------

void TMainForm::AccendiLed(int n, TColor c) {
    TShape *s;

    s = (TShape*)(FindComponent("Shape" + IntToStr(n)));
    if (dynamic_cast<TShape*>(s) == NULL) {
        return;
    }
    s->Brush->Color = c;
}
// ---------------------------------------------------------------------------

void TMainForm::VisTCiclo(int n, AnsiString val) {
    TLabel *s;

    s = (TLabel*)(FindComponent("T" + IntToStr(n)));
    if (dynamic_cast<TLabel*>(s) == NULL) {
        return;
    }
    s->Caption = val;
}

void TMainForm::VisWatchDog(int n, AnsiString valplc, AnsiString valpc) {
    TLabel *s;

    s = (TLabel*)(FindComponent("WPlc" + IntToStr(n)));
    if (dynamic_cast<TLabel*>(s) == NULL) {
        return;
    }
    s->Caption = valplc;

    s = (TLabel*)(FindComponent("WPc" + IntToStr(n)));
    if (dynamic_cast<TLabel*>(s) == NULL) {
        return;
    }
    s->Caption = valpc;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BitBtn7Click(TObject *Sender)
{
    dmComandiAgv->InvioGenVar(1, "latofork", 'I', 1);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BitBtn6Click(TObject *Sender)
{
    dmComandiAgv->InvioGenVar(1, "latofork", 'I', 2);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BitBtn4Click(TObject *Sender)
{
    dmComandiAgv->InvioGenVar(2, "latofork", 'I', 1);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BitBtn5Click(TObject *Sender)
{
    dmComandiAgv->InvioGenVar(2, "latofork", 'I', 2);
}
// ---------------------------------------------------------------------------
