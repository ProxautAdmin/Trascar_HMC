// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "SocketComunicazioneClient.h"
#include "DB.h"
#include "DataExchange.h"
#include "ExtraFunction.h"
#include "percorsiprototipo.h"
#include "chiusura.h"
#include "msg.h"
#include "config.h"
#include "allarmi.h"
#include "allarmiAttivi.h"
#include "Batterie.h"
#include "missioni.h"
// #include "DBExtra.h"
#include "CentroMissioni.h"
#include "agv.h"
#include "login.h"
#include "port.h"
#include "log.h"

#include "GestMissioni.h"
#include "stato_pos.h"
#include "PosDepositoPLC.h"
#include "PosPrelievoPLC.h"

#include "Password.h"
#include "dmFunzioniComuniClientServer.h"
#include "DBImpianto.h"
#include "udclist.h"
#include "GestUser.h"
#include "GestAGVMidi.h"

#include <System.DateUtils.hpp>
#include <inifiles.hpp>
#include <stdio.h>
#include <dialogs.hpp>
// char N_carrelli = 7;
DATILGV tab;

TPaintBox *PaintBox1, *ZoomBox;
// ---------------------------------------------------------------------------
#pragma link "MessageBar"
#pragma link "RotImg"
#pragma link "MyShape"
#pragma link "frame_info"
#pragma resource "*.dfm"
TMainForm *MainForm;
TClientData ClientData;
TCentroMissione CentroMissione;
TMissione MissioneMan;

// ---------------------------------------------------------------------------
void TMainForm::LeggiParametri() {
    FILE *f;
    char s[80];
    int n;

}

void TMainForm::DisegnaCarrello(int car) {
    double x, y, deltax, deltay;
    TRotateImage *imgCarr;
    TLabel *LabelAGV;
    int n;
    AnsiString filename, str_hint;

    imgCarr = (TRotateImage*) FindComponent("img" + IntToStr(car));
    if (ClientData.DatiAgv[car].load) {
        filename = "AGVL"; // + IntToStr(car) + "L";
    }
    else {
        filename = "AGV"; // + IntToStr(car);
    }
    if ((ClientData.DatiAgv[car].alarm) || (ClientData.DatiAgv[car].stand_by)) {
        filename += "A";
    }
    else if ((ClientData.DatiAgv[car].manuale)) {
        filename += "M";
    }

    filename += ".bmp";
    if (FileExists(filename)) {
        imgCarr->Picture->LoadFromFile(filename);
    }
    if (ClientData.DatiAgv[car].pos != 0) {
        if (tab.mappa_nodi.find(ClientData.DatiAgv[car].pos) != tab.mappa_nodi.end()) {
            x = Xo + ((Image1->Width * tab.mappa_nodi[ClientData.DatiAgv[car].pos].posx) / W) + Image1->Left;
            y = Yo - ((Image1->Height * tab.mappa_nodi[ClientData.DatiAgv[car].pos].posy) / H) + Image1->Top;
            // Modifica per Bricoman

            if (imgCarr->Angle == 0) {
                deltax = +Image1->Width / 70;
                deltay = -(Image1->Height / 10);
            }
            else if (imgCarr->Angle == 90) {
                deltax = -(Image1->Width / 10);
                deltay = 0;
            }
            if (imgCarr->Angle == 180) {
                deltax = -Image1->Width / 70;
                deltay = +(Image1->Height / 10);
            }
            if (imgCarr->Angle == 270) {
                deltax = (Image1->Width / 10);
                deltay = 0;
            }
            //

            imgCarr->Angle = tab.mappa_nodi[ClientData.DatiAgv[car].pos].ang;
            // di base 60:56
            imgCarr->Width = ((imgCarr->Angle == 0) || (imgCarr->Angle == 180) ? 60 : 60) * dmExtraFunction->ProporzionaZoom(zoom, 1.0f);
            imgCarr->Height = ((imgCarr->Angle == 90) || (imgCarr->Angle == 270) ? 60 : 60) * dmExtraFunction->ProporzionaZoom(zoom, 1.0f);
            imgCarr->Top = (y - imgCarr->Height / 2); // + deltay;
            imgCarr->Left = (x - imgCarr->Width / 2); // + deltax;
            imgCarr->BringToFront(); // dovrebbe ripristinare il primo piano al primo refresh nel caso in cui sia stato messo dietro

            imgCarr->Visible = true;

            str_hint = "Agv " + IntToStr(car) + " ; IdUdc " + IntToStr(ClientData.DatiAgv[car].DatiUDC.IDUDC) + " ; IdArtUdc " + IntToStr(ClientData.DatiAgv[car].DatiUDC.IdArtUDC);
            if (ClientData.DatiAgv[car].manuale) {
                str_hint = str_hint + " in Manual Mode ";
            }
            else if (ClientData.DatiAgv[car].alarm) {
                str_hint = str_hint + " Alarm ";
                for (n = 0; n < 100; n++) {
                    if (ClientData.DatiAgv[car].allarmi[n] == '1')
                        str_hint = str_hint + " - " + MainForm->MessageBar1->Messages->Strings[n];
                }
            }
            imgCarr->Hint = str_hint;
            // LABEL
            LabelAGV = (TLabel*) ScrollBox1->FindComponent("LabelAGV" + IntToStr(car));
            if (LabelAGV == NULL) {
                LabelAGV = new TLabel(ScrollBox1);
                LabelAGV->Parent = ScrollBox1;
                LabelAGV->Name = "LabelAGV" + IntToStr(car);
                LabelAGV->AutoSize = true;
                LabelAGV->Font->Name = "Tahoma";
                LabelAGV->Font->Size = 14;
                LabelAGV->Caption = IntToStr(car);
                LabelAGV->Visible = true;
                LabelAGV->Font->Color = clYellow;
                LabelAGV->Transparent = true;
                LabelAGV->Tag = car;
                LabelAGV->Color = clBlack;
                LabelAGV->OnMouseUp = LAbalAGVMouseUp; // FONDAMENTALE per attivare il tasto dx anche su label
                LabelAGV->Font->Style = TFontStyles() << fsBold;
                LabelAGV->PopupMenu = PopupMenu1; // FONDAMENTALE per attivare il tasto dx anche su label
            }
            if (LabelAGV != NULL) {
                if ((imgCarr->Angle >= 0) && (imgCarr->Angle < 90)) {
                    LabelAGV->Top = imgCarr->Top + ((imgCarr->Height / 2) - LabelAGV->Height / 2);
                    LabelAGV->Left = imgCarr->Left + imgCarr->Width / 1.5f;
                }
                else if ((imgCarr->Angle >= 90) && (imgCarr->Angle < 180)) {
                    LabelAGV->Top = imgCarr->Top * 1.01f;
                    LabelAGV->Left = imgCarr->Left + ((imgCarr->Width - LabelAGV->Width) / 2);
                }
                if ((imgCarr->Angle >= 180) && (imgCarr->Angle < 270)) { // ok
                    LabelAGV->Top = imgCarr->Top + ((imgCarr->Height / 2) - LabelAGV->Height / 2);
                    LabelAGV->Left = imgCarr->Left + (imgCarr->Width / 8);
                }
                if (imgCarr->Angle >= 270) {
                    LabelAGV->Top = imgCarr->Top + imgCarr->Height * 0.55f; // * 1.5f;
                    LabelAGV->Left = imgCarr->Left + ((imgCarr->Width - LabelAGV->Width) / 2);
                }
                LabelAGV->BringToFront();
                LabelAGV->Font->Size = 14 * zoom;
                LabelAGV->Visible = imgCarr->Visible;
            }
        }
        else
            imgCarr->Visible = false;
    }
    else {
        imgCarr->Visible = false;
    }
    if (dmDB->abilitatracciaagv)
        VisualizzaPercorsoAgv(car);
}

void __fastcall TMainForm::LAbalAGVMouseUp(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    // FONDAMENTALE per attivare il tasto dx anche su label
    TLabel *LabAGV;
    TShiftState app;
    app << ssMiddle;
    int pos_sel;
    LabAGV = (TLabel*) Sender;
    if ((Button == mbRight) && (LabAGV != NULL)) {
        if (LabAGV->Tag == 1)
        {
            img2MouseUp(img1, mbRight, app, 0, 0);
        }
    }
}

void TMainForm::VisualizzaPercorsoAgv(int nagv) {
    TMyConnector *ToPickAgv, *ToDropAgv;
    TRotateImage *imgCarr;
    TMyShape *Pos;
    int agv;
    // return; // vediamo se utilizzarlo
    for (agv = 1; agv <= NAGV; agv++) {
        if ((agv == nagv) || (nagv == 0)) {
            ToPickAgv = (TMyConnector*) ScrollBox1->FindComponent("ToMoveAgv" + IntToStr(agv));
            // ToDropAgv = (TMyConnector*) ScrollBox1->FindComponent("ToDropAgv" + IntToStr(agv));
            imgCarr = (TRotateImage*) FindComponent("img" + IntToStr(agv));
            if (ToPickAgv == NULL) { // capita che quando chiudo il programma rientri qui come first, da capire come mai
                ToMoveAgv = new TMyConnector(ScrollBox1);
                ToMoveAgv->Parent = ScrollBox1;
                ToMoveAgv->Name = "ToMoveAgv" + IntToStr(agv);
                ToMoveAgv->Visible = false;
                ToMoveAgv->Width = 1;
                ToMoveAgv->Height = 1;
                // ToMoveAgv->Pen->Color = clLime;
                ToMoveAgv->Pen->Width = 3;
            }
            if (ToPickAgv != NULL) {
                if ((imgCarr == NULL) || (!imgCarr->Visible) || (ClientData.DatiAgv[agv].load) || (!ClientData.DatiAgv[agv].pos) ||
                    ((ClientData.DatiAgv[agv].prel == ClientData.DatiAgv[agv].pos) && (ClientData.DatiAgv[agv].prel)) ||
                    ((ClientData.DatiAgv[agv].dest == ClientData.DatiAgv[agv].pos) && (ClientData.DatiAgv[agv].dest)) ||
                    (!ClientData.DatiAgv[agv].prel && !ClientData.DatiAgv[agv].dest)) {
                    ToPickAgv->Tag = 0;
                    ToPickAgv->Visible = false;
                }
                else {
                    if ((ToPickAgv->Tag != ClientData.DatiAgv[agv].prel) && (ClientData.DatiAgv[agv].prel) && ((ClientData.DatiAgv[agv].tipomis >= 0) && (ClientData.DatiAgv[agv].tipomis <= 3))) {
                        Pos = (TMyShape*) ScrollBox1->FindComponent("Pos" + IntToStr(ClientData.DatiAgv[agv].prel));
                        if (Pos == NULL)
                            Pos = (TMyShape*) FindComponent("Pos" + IntToStr(ClientData.DatiAgv[agv].prel));
                        if (Pos != NULL) {
                            ToPickAgv->Control1 = imgCarr;
                            ToPickAgv->Control2 = Pos;
                            ToPickAgv->Tag = ClientData.DatiAgv[agv].prel;
                            ToPickAgv->Pen->Color = clLime;
                            ToPickAgv->Pen->Width = 3;
                            ToPickAgv->Pen->Style = psSolid;
                            ToPickAgv->Visible = true;
                        }
                        else {
                            ToPickAgv->Tag = 0;
                            ToPickAgv->Visible = false;
                        }
                    }
                    else if ((ToPickAgv->Tag != ClientData.DatiAgv[agv].dest) && (ClientData.DatiAgv[agv].dest)) {
                        Pos = (TMyShape*) ScrollBox1->FindComponent("Pos" + IntToStr(ClientData.DatiAgv[agv].dest));
                        if (Pos == NULL)
                            Pos = (TMyShape*) FindComponent("Pos" + IntToStr(ClientData.DatiAgv[agv].dest));
                        if (Pos != NULL) {
                            ToPickAgv->Control1 = imgCarr;
                            ToPickAgv->Control2 = Pos;
                            ToPickAgv->Pen->Color = clRed;
                            ToPickAgv->Pen->Width = 1;
                            ToPickAgv->Pen->Style = psDashDot; // ATTENZIONE! FUNZIONA SOLO CON PEN->WIDTH = 1
                            ToPickAgv->Tag = ClientData.DatiAgv[agv].dest;
                            ToPickAgv->Visible = true;
                        }
                        else {
                            ToPickAgv->Tag = 0;
                            ToPickAgv->Visible = false;
                        }
                    }
                }
                if ((ClientData.DatiAgv[agv].dep) && ((ClientData.DatiAgv[agv].load) || (ClientData.DatiAgv[agv].tipomis == 4)) && (ClientData.DatiAgv[agv].pos) && (ClientData.DatiAgv[agv].dep != ClientData.DatiAgv[agv].prel)) {
                    if (ToPickAgv->Tag != ClientData.DatiAgv[agv].dep) {
                        Pos = (TMyShape*) ScrollBox1->FindComponent("Pos" + IntToStr(ClientData.DatiAgv[agv].dep));
                        if (Pos == NULL)
                            Pos = (TMyShape*) FindComponent("Pos" + IntToStr(ClientData.DatiAgv[agv].dep));
                        if (Pos != NULL) {
                            ToPickAgv->Control1 = imgCarr;
                            ToPickAgv->Control2 = Pos;
                            ToPickAgv->Pen->Color = clBlue;
                            ToPickAgv->Pen->Width = 3;
                            ToPickAgv->Pen->Style = psSolid;
                            ToPickAgv->Tag = ClientData.DatiAgv[agv].dep;
                            ToPickAgv->Visible = true;
                        }
                        else {
                            ToPickAgv->Tag = 0;
                            ToPickAgv->Visible = false;
                        }
                    }
                }

            }

        }
    }

}

__fastcall TMainForm::TMainForm(TComponent *Owner)
    : TForm(Owner)
{
}

// ---------------------------------------------------------------------------
template<class T>
void __fastcall TMainForm::CreateMDIChild(T *f, String Name, String Caption) {
    bool exists = false; // mod
    int i;

    ScrollBox1->Visible = false;
    // gbInfo->Visible = false&dmDB->abilitainfobox;
    SetVisible();
    SendMessage(ClientHandle, WM_SETREDRAW, FALSE, 0);
    for (i = MDIChildCount - 1; i >= 0; i--) {
        if (MDIChildren[i]->Name != Name) {
            ((TMDIChild*)MDIChildren[i])->SetInvisible();
            MDIChildren[i]->Close();
        }
    }
    for (i = 0; i < MDIChildCount; i++) {
        if (MDIChildren[i]->Name == Name) {
            MDIChildren[i]->Caption = Caption;
            ((T*)MDIChildren[i])->PanelTitle->Caption = Caption;
            ShowWindow(MDIChildren[i]->Handle, SW_MAXIMIZE);
            ShowWindow(MDIChildren[i]->Handle, SW_MAXIMIZE);
            ((TMDIChild*)MDIChildren[i])->Mostra();
            exists = true;
            break;
        }
    }
    if (!exists) {
        Application->CreateForm(__classid(T), &f);
        f->Name = Name;
        f->Caption = Caption;
        f->PanelTitle->Caption = Caption;
        ShowWindow(f->Handle, SW_MAXIMIZE);
        f->Mostra();
        if (Screen->Height < NativeScreenHeight)
            f->ScaleBy(Screen->Height - Panel2->Height - MainStatusBar->Height,
            NativeScreenHeight - Panel2->Height - MainStatusBar->Height);
        f->WindowState = wsMinimized;
        f->WindowState = wsMaximized;
    }
    SendMessage(ClientHandle, WM_SETREDRAW, TRUE, 0);
    RedrawWindow(ClientHandle, NULL, 0, RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_NOINTERNALPAINT);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::bEsciClick(TObject *Sender)
{
    Close();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    int res;
    if ((dmDB->pwdlevel) || (1 == 1)) { // forzo sempre ok
        res = ChiusuraForm->ShowModal();
        CanClose = ((res == IDYES) || (res == IDOK));
        chiusura_applicazione = CanClose;
        if (CanClose) {
            ShowCursor(true);
        }
        if (res == IDYES) {
            HANDLE hToken;
            TOKEN_PRIVILEGES tkp;

            // Get a token for this process.
            if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
                // Get the LUID for the shutdown privilege.
                LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

                tkp.PrivilegeCount = 1; // one privilege to set
                tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

                // Get the shutdown privilege for this process.
                AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

                ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0);
            }
        }

    }
    else {
        CanClose = false;
        chiusura_applicazione = CanClose;
        ShowMessage("Per uscire dal programma inserisci la password ");
    }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender) {
    int lang, i, j;
    TIniFile *Ini;
    TCursor Save_Cursor = Screen->Cursor;
    FILE *f;
    char s[100];
    AnsiString Imp;
    double fXo, fYo;
    double fH, fW, fFH, fFW;
    Screen->Cursor = crHourGlass; // Show hourglass cursor
    ShowWindow(Handle, SW_HIDE);
    PWD = "*STARTUP";
    if (DebugHook) {
        dmDB->pwdlevel = dmDB->ControllaUtentePassword(dmDB->ActualUser, PWD);
        // dmDB->pwdlevel = 2;
    }
    else {
        dmDB->pwdlevel = 0;
    }

    LoadPassword();
    UpdControls();
    Visible = false;
    Screen->Cursor = Save_Cursor;
    baiaformSegnali = 0;
    Imp = Impianto + "_prx.crs";
    tab.Load1(Imp.c_str());
    f = fopen(Imp.c_str(), "r");
    time_ultimo_ridisegno = clock();

    if (f == NULL) {
        // exit(-1);
    }
    else {

        fseek(f, 0, SEEK_SET);
        // leggo i valori di altezza e larghezza dell'impianto e il punto d'origine
        while (!feof(f)) {
            fscanf(f, "%s", s);
            if (!strcmp(s, "H")) {
                fscanf(f, "%lf", &fH);
                H = fH;
            }
            else if (!strcmp(s, "W")) {
                fscanf(f, "%lf", &fW);
                W = fW;
            }
            else if (!strcmp(s, "Xo")) {
                fscanf(f, "%lf", &fXo);
                Xo = fXo;
            }
            else if (!strcmp(s, "Yo")) {
                fscanf(f, "%lf", &fYo);
                Yo = fYo;
            }
            else if (!strcmp(s, "FH")) {
                fscanf(f, "%lf", &fFH);
                FormH = fFH;
            }
            else if (!strcmp(s, "FW")) {
                fscanf(f, "%lf", &fFW);
                FormW = fFW;
            }
        }

        /* Xo = (Xo * Image1->Width) / FormW;    //Ricalcolo l'origine in base alla dimensione della finestra
         Yo = (Yo * Image1->Height) / FormH;
         FormH = Image1->Height;
         FormW = Image1->Width; */
    }
    fclose(f);

    count_inizio = 0;
    init_mappa = 0;
    chiusura_applicazione = 0;
    count_aggiorna_mappa_client = 0;
    num_pos_no_database = 3;
    format_data_ora_datatbase = "dd/mm/yyyy hh':'mm':'ss";
    TimerOraTimer(Sender);
    prelievo_missione_manuale = 0;
    TimerSinottico->Enabled = true;
    lbAGV1Status->Transparent = false;

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::AfterConstruction(void)
{
    FFormState = FFormState >> fsVisible;
    TForm::AfterConstruction();
}
// ---------------------------------------------------------------------------

void TMainForm::SetVisible()
{
    FFormState << fsVisible;
}
// ---------------------------------------------------------------------------

void TMainForm::SavePassword() {
    /* FILE *f;
     char s[100] = "";
     int i;

     if (f = fopen("pwd.txt", "w")) {
     for (i = 0; i < PWD.Length(); i++) {
     s[i] = PWD.c_str()[i] + 1 - (i % 3);
     }
     fprintf(f, "%s\n", s);
     fclose(f);
     } */
}
// ---------------------------------------------------------------------------

void TMainForm::LoadPassword() {
    /* FILE *f;
     char s[100] = "";
     int i;

     if (f = fopen("pwd.txt", "r")) {
     fscanf(f, "%s", s);
     PWD = "";
     for (i = 0; i < strlen(s); i++) {
     PWD += (char)(s[i] - 1 + (i % 3));
     }
     fclose(f);
     } */
}
// ---------------------------------------------------------------------------

void TMainForm::UpdControls() {
    TBorderIcons tempBI = BorderIcons;
    MainStatusBar->Refresh();
    // se si vuole attivare mettere tastiera di input
    /*
     if (dmDB->pwdlevel == 0) {
     tempBI >> biMaximize >> biMinimize;
     }
     else {
     tempBI << biMaximize << biMinimize;
     }
     */
    // BRICOMAN LASCIO SEMPRE ATTIVI
    tempBI << biMaximize << biMinimize;
    BorderIcons = tempBI;

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::TimerPwdTimer(TObject *Sender)
{

    /* if ((!FindWindowA("TAppBuilder", NULL)) && TouchPanel) {
     LockKeyboard();
     ShowCursor(false);
     } */
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::TimerOraTimer(TObject *Sender) {
    TMyShape *Porta, *PosCella, *PortaFumo;
    TRotateImage *RotImg;
    IndexListIterator i;
    int idporta, j, k, top_pos_lavatrice1, colore;
    AnsiString stringa, lotto;
    TimerOra->Enabled = false;
    timestr = FormatDateTime("dddd d mmmm yyyy h':'mm':'ss", Now());
    // timestr = dmDB->AggiungiRimuoviSecondiADataOra( DateTimeToStr(Now()),5000,true);
    // dmDB->LeggiSegnalazioneAttiva(msg, all);
    MainStatusBar->Refresh();
    try {
        if (MainForm->count_inizio <= 20) {
            // imgLogo->Angle = (((int) imgLogo->Angle + 30) % 360);
            if (count_inizio == 1) {
                // Image1->Align = alNone;
                // messo cosi' per la partenza dove l'immagine sforava lo schermo
                Image1->Height = ScrollBox1->Height * 0.99f;
                Image1->Width = ScrollBox1->Width * 0.99f;
            }
            else if (count_inizio == 2) {
                zoom = 1;
                Xo = (Xo * Image1->Width) / FormW; // Ricalcolo l'origine in base alla dimensione della finestra
                Yo = (Yo * Image1->Height) / FormH;
                FormH = Image1->Height;
                FormW = Image1->Width;
                widthextend = Image1->Width;
                heightextend = Image1->Height;
            }
            else if ((!init_mappa) && (count_inizio > 5)) {
                AggiornaMappa();
            }
            if ((count_inizio > 5) && (!Panel2->Enabled) && (dmDB->lettostatoagv)) {
                Panel2->Enabled = true;
                aggiornaagv = 1;
            }
            if (count_inizio == 20) {
                for (j = 1; j <= NAGV; j++) {
                    RotImg = (TRotateImage*)(FindComponent("img" + IntToStr(j)));
                    // Togliere per getione label dinamica LabelAGV
                    // if (RotImg != NULL)
                    // RotImg->BringToFront();
                }
            }
            count_inizio++;
        }
        else {
            // visualizzo stato chiamate e rotalamr
            if (count_aggiorna_mappa_client > 60) {
                AggiornaMappa();
                dmDB->ImpostaAllarmiAttiviClient();
            }
            else
                count_aggiorna_mappa_client++;
        }
        // elimino tasti per piccoli terminali. Con psw admin tornano
        if ((Image1->Width <= 1000) && (!dmDB->pwdlevel)) {
            BitBtn12->Visible = false;
        }
        else {
            // btPhoenix->Visible = true;
            // BitBtn1->Visible = true;
            // btDati->Visible = true;
            BitBtn12->Visible = true;
        }

        if ((!MDIChildCount) && (!MainForm->ScrollBox1->Visible)) {
            MainForm->ScrollBox1->Visible = true;
            // gbInfo->Visible = true&dmDB->abilitainfobox; ;
            if (count_inizio > 20)
                AggiornaMappa();
        }

        if (dmDB->aggiorna_tab_posizioni_locale) {
            dmDB->AggiornaTabPostazioni();
            dmDB->aggiorna_tab_posizioni_locale = 0;
            if (count_inizio > 20) {
                aggiornamappa = 1;
                SocketDataModule->InviaAggiornamentoTabella(1);

            }
        }
        if (dmDB->aggiorna_tab_porte_locale) {
            dmDB->AggiornaTabPorte();
            dmDB->aggiorna_tab_porte_locale = 0;
            if (count_inizio > 20) {
                SocketDataModule->InviaAggiornamentoTabella(2);
                // aggiornamappa = 1 ;
            }
        }

        if (dmDB->aggiorna_tab_tipologia_locale) {
            dmDB->AggiornaTabTipoArticoli();
            dmDB->AggiornaTabAnagrafica();
            dmDB->AggiornaTabTipologiaCorsia();
            dmDB->aggiorna_tab_tipologia_locale = 0;
            if (count_inizio > 20) {
                SocketDataModule->InviaAggiornamentoTabella(3);
                // aggiornamappa = 1 ;
            }
        }
        if (MainForm->ScrollBox1->Visible) {
            if (aggiornamappa) {
                AggiornaMappa();
            }
            if (aggiornaagv) {
                for (j = 1; j <= NAGV; j++) {
                    DisegnaCarrello(j);
                }
                // VisualizzaPercorsoAgv(0) ;
            }
            aggiornamappa = 0;
            aggiornaagv = 0;
            // aggiorno stato porte e posizioni plc
            for (i = dmDB->TabPorte.begin(); i != dmDB->TabPorte.end(); i++) {
                Porta = (TMyShape*) ScrollBox1->FindComponent("Porta" + i->second["IDPORTA"]);
                if (Porta != NULL) {
                    idporta = i->second["IDPORTA"].ToIntDef(0);
                    if (i->second["STATO"] == 0) {
                        Porta->Text = "?";
                        Porta->Pen->Color = clYellow;
                        Porta->Pen->Width = 3;
                    }
                    else if (i->second["STATO"] == 1) {
                        Porta->Text = "A";
                        if (ClientData.DatiPorte[idporta].memoria_apertura_operatore) {
                            Porta->Pen->Color = clYellow;
                            Porta->Pen->Width = 3;
                        }
                        else {
                            Porta->Pen->Color = clBlack;
                            Porta->Pen->Width = 1;
                        }
                    }
                    else if (i->second["STATO"] == 2) {
                        Porta->Text = "M";
                        Porta->Pen->Color = clYellow;
                        Porta->Pen->Width = 3;
                    }
                    else if (i->second["STATO"] == 3) {
                        Porta->Text = "D";
                        Porta->Pen->Color = clRed;
                        Porta->Pen->Width = 3;
                    }
                    if ((ClientData.DatiPorte[idporta].ChiudiPorta) && (i->second["STATO"] == 1)) {
                        if (sync_lamp)
                            Porta->Brush->Color = clGreen;
                        else
                            Porta->Brush->Style = bsClear;
                    }
                    else if (ClientData.DatiPorte[idporta].PortaAperta) {
                        Porta->Brush->Color = clLime;
                    }
                    else {
                        if (ClientData.DatiPorte[idporta].ApriPorta) {
                            if (sync_lamp)
                                Porta->Brush->Color = clLime;
                            else
                                Porta->Brush->Style = bsClear;
                        }
                        else {
                            Porta->Brush->Color = clGray;
                        }

                    }
                }
            }
        }
    }
    catch (...) {
    }
    // BitBtn10->Visible = (dmDB->pwdlevel && dmDB->ActualUser == "PROXAUT");
    // BitBtn3->Visible = (dmDB->pwdlevel && dmDB->ActualUser == "PROXAUT");
    // BitBtn6->Visible = (dmDB->pwdlevel && dmDB->ActualUser == "PROXAUT") ;
    sync_lamp = !sync_lamp;
    TimerOra->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::bPwdClick(TObject *Sender) {

    if ((dmDB->ActualUser != "NONE") || (dmDB->pwdlevel)) {
        dmDB->ActualUser = "NONE";
        dmDB->pwdlevel = 0;
    }
    FormLogin->ShowModal();
    UpdControls();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::MainStatusBarDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect)
    // void __fastcall TMainForm::MainStatusBarDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const System::Types::TRect &Rect)
{
    static int n = 0;
    static int n2 = 0;
    static int prev = 0;
    int index_plc_display;
    int totphx;
    TCanvas *pCanvas = StatusBar->Canvas;
    int t, l;

    switch (Panel->Index) {
    case 0:
        StatusBar->Panels->Items[0]->Text = FormatDateTime("d mmmm yyyy h':'mm':'ss", Now()); // timestr;
        pCanvas->Brush->Color = clBtnFace;
        pCanvas->Font->Color = clBlack;
        pCanvas->FillRect(Rect);
        l = (Rect.Width() - pCanvas->TextWidth(Panel->Text)) / 2;
        t = (Rect.Height() - pCanvas->TextHeight(Panel->Text)) / 2;
        pCanvas->TextOut(Rect.left + l, Rect.top + t, Panel->Text);
        break;
    case 1:
        switch (dmDB->pwdlevel) {
        case 0:
            n = 0;
            break;
        case 1:
            n = 1;
            break;
        case 2:
            n = 2;
            break;
        default:
            n = 2;
        }
        l = (Rect.Width() - ImageList1->Width) / 2;
        t = (Rect.Height() - ImageList1->Height) / 2;
        ImageList1->Draw(StatusBar->Canvas, Rect.left + l, Rect.top + t, n);
        break;
    case 2:
        /* if (ClientData.ModBusIn.watchdog != prev) {
         n2 = 1 - n2;
         prev = ClientData.ModBusIn.watchdog;
         countwatchdog = 0;
         }
         else {
         countwatchdog++;
         } */
        l = (Rect.Width() - ImageList2->Width) / 2;
        t = (Rect.Height() - ImageList2->Height) / 2;
        ImageList2->Draw(StatusBar->Canvas, Rect.left + l, Rect.top + t, n2);
        break;
    case 3:
        if (SocketDataModule->SeiConnesso()) {
            Panel->Text = "Server Attivo";
            pCanvas->Brush->Color = clLime;
            pCanvas->Font->Color = clBlack;
        }
        else {
            Panel->Text = "Server Disattivato";
            pCanvas->Brush->Color = clRed;
            pCanvas->Font->Color = clWhite;
        }
        pCanvas->FillRect(Rect);
        l = (Rect.Width() - pCanvas->TextWidth(Panel->Text)) / 2;
        t = (Rect.Height() - pCanvas->TextHeight(Panel->Text)) / 2;
        pCanvas->TextOut(Rect.left + l, Rect.top + t, Panel->Text);
        break;
    case 4:
        if (ClientData.Plc[1].PlcConnesso) {
            Panel->Text = "PLC Rulliera OK";
            if (!ClientData.ParametriFunzionali.Simula) {
                pCanvas->Brush->Color = clLime;
                pCanvas->Font->Color = clBlack;
            }
        }
        else {
            Panel->Text = "PLC Rulliera KO";
            if (!ClientData.ParametriFunzionali.Simula) {
                pCanvas->Brush->Color = clRed;
                pCanvas->Font->Color = clWhite;
            }

        }
        if (ClientData.ParametriFunzionali.Simula) {
            pCanvas->Brush->Color = clAqua;
            pCanvas->Font->Color = clBlue;
        }
        pCanvas->FillRect(Rect);
        l = (Rect.Width() - pCanvas->TextWidth(Panel->Text)) / 2;
        t = (Rect.Height() - pCanvas->TextHeight(Panel->Text)) / 2;
        pCanvas->TextOut(Rect.left + l, Rect.top + t, Panel->Text);
        break;
    case 5:
        if (ClientData.DatiPorte[1].PlcConnesso) {
            Panel->Text = "Porta OK";
            if (!ClientData.ParametriFunzionali.Simula) {
                pCanvas->Brush->Color = clLime;
                pCanvas->Font->Color = clBlack;
            }
        }
        else {
            Panel->Text = "Porta KO";
            if (!ClientData.ParametriFunzionali.Simula) {
                pCanvas->Brush->Color = clRed;
                pCanvas->Font->Color = clWhite;
            }

        }
        if (ClientData.ParametriFunzionali.Simula) {
            pCanvas->Brush->Color = clAqua;
            pCanvas->Font->Color = clBlue;
        }
        pCanvas->FillRect(Rect);
        l = (Rect.Width() - pCanvas->TextWidth(Panel->Text)) / 2;
        t = (Rect.Height() - pCanvas->TextHeight(Panel->Text)) / 2;
        pCanvas->TextOut(Rect.left + l, Rect.top + t, Panel->Text);
        break;

    case 6:
        // db
        if (dmDB->ADOConnection1->Connected) {
            Panel->Text = "DB: " + dmExtraFunction->nomeIstanza(dmDB->ADOConnection1->ConnectionString);
            pCanvas->Brush->Color = clLime;
            pCanvas->Font->Color = clBlack;
        }
        else {
            Panel->Text = "DB Error";
            pCanvas->Brush->Color = clRed;
            pCanvas->Font->Color = clWhite;
        }
        pCanvas->FillRect(Rect);
        l = (Rect.Width() - pCanvas->TextWidth(Panel->Text)) / 2;
        t = (Rect.Height() - pCanvas->TextHeight(Panel->Text)) / 2;
        pCanvas->TextOut(Rect.left + l, Rect.top + t, Panel->Text);
        break;
    case 7:
        if (ClientData.ParametriFunzionali.AbilitaMissioni) {
            pCanvas->Brush->Color = clLime;
            pCanvas->Font->Color = clBlack;
            Panel->Text = "Missioni Abilitate";
            BitBtn4->Enabled = false;
            BitBtn5->Enabled = true;
        }
        else {
            pCanvas->Brush->Color = clRed;
            pCanvas->Font->Color = clWhite;
            Panel->Text = "Missioni Disabilitate";
            BitBtn4->Enabled = true;
            BitBtn5->Enabled = false;
        }
        pCanvas->FillRect(Rect);
        l = (Rect.Width() - pCanvas->TextWidth(Panel->Text)) / 2;
        t = (Rect.Height() - pCanvas->TextHeight(Panel->Text)) / 2;
        pCanvas->TextOut(Rect.left + l, Rect.top + t, Panel->Text);
        // modifico vis password qui
        if (dmDB->ActualUser == "NONE") {
            pPassword->Color = clBtnFace;
            pPassword->Font->Color = clBlack;
        }
        else {
            pPassword->Color = clGradientInactiveCaption;
            pPassword->Font->Color = clBlue;
        }
        pPassword->Caption = "USER : " + dmDB->ActualUser + " (" + dmDB->ZonaString + ")"; // " (" + dmDB->pwdlevel + ")";
        break;
    case 8:
        if (ClientData.ParametriFunzionali.Giorno) {
            pCanvas->Brush->Color = clWebOrange;
            pCanvas->Font->Color = clBlack;
            Panel->Text = "Stato giorno";
            btGiorno->Enabled = false;
            btNotte->Enabled = true;
        }
        else {
            pCanvas->Brush->Color = clBlue;
            pCanvas->Font->Color = clWhite;
            Panel->Text = "Stato Notte";
            btGiorno->Enabled = true;
            btNotte->Enabled = false;
        }
        pCanvas->FillRect(Rect);
        l = (Rect.Width() - pCanvas->TextWidth(Panel->Text)) / 2;
        t = (Rect.Height() - pCanvas->TextHeight(Panel->Text)) / 2;
        pCanvas->TextOut(Rect.left + l, Rect.top + t, Panel->Text);

        break;

    }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::Image4Click(TObject *Sender)
{
    if (BorderStyle == bsNone) {
        BorderStyle = bsSizeable;
    }
    else {
        BorderStyle = bsNone;
        ShowWindow(Handle, SW_MAXIMIZE);
        ShowWindow(Handle, SW_MAXIMIZE);
    }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    int i;
    dmDB->LogMsg("Disconnessione Client");
    dmDB->LogMsg("Client Chiuso");
}
// ---------------------------------------------------------------------------

void TMainForm::AggiornaMappa() {
    AnsiString stringa, stringa2, str;
    double x, y, posx, posy, xselezione1, xselezione2, yselezione1, yselezione2;
    int imgw, imgh, countrecord, changestep, nobarra, escludi = 0, prenotata = 0, cellaTop, cellaLeft, xi, variantetop, varianteleft;
    int pos, piano, numpallet = 0, idimg, tipologia, posorigin, colore, rullierapronta, tipologia_corsia, pianioccupati = 0, allarme = 0;
    int constcolor;
    long cornice;
    TMyShape *PosCella;
    // if (count_aggiorna_mappa_client < 60)
    if (1 == 1)
        stringa = "SELECT * FROM  PosizioniCondatiUDC  order by pos ";
    // stringa = "SELECT * FROM  Posizioni INNER JOIN Corsie ON Posizioni.Fila = Corsie.Fila order by pos";
    else
        stringa = "SELECT * FROM  Posizioni_VIEW where (utenti&" + dmDB->UserNum + "=" + dmDB->UserNum + ") order by pos desc";
    if (dmDB->ADOConnection1->Connected) {
        nobarra = 0;
        if ((!nobarra) && (PaintBox1 != NULL)) {
            delete PaintBox1;
            PaintBox1 = NULL;
        }
        else if ((Contatore > 0) && (ret) && (PaintBox1 != NULL)) {
            PaintBox1->Canvas->Pen->Mode = pmNot;
            PaintBox1->Canvas->Pen->Color = clWhite;
            PaintBox1->Canvas->Brush->Style = bsClear;
            PaintBox1->Canvas->Rectangle(x1sel, y1sel, x2sel, y2sel);
        }
        try {
            dmDB->QueryPos->Close();
            dmDB->QueryPos->SQL->Clear();
            dmDB->QueryPos->SQL->Append(stringa);
            dmDB->QueryPos->Open();
            countrecord = 1;
            if (dmDB->QueryPos->RecordCount > 0) {
                dmDB->QueryPos->First();
                while (!dmDB->QueryPos->Eof) {
                    pos = dmDB->QueryPos->FieldByName("Pos")->AsInteger;
                    idimg = dmDB->QueryPos->FieldByName("Pos")->AsInteger;
                    numpallet = 1;
                    allarme = 0;
                    tipologia_corsia = dmDB->QueryPos->FieldByName("Tipologia")->AsInteger;

                    PosCella = (TMyShape*) ScrollBox1->FindComponent("Pos" + String(pos));
                    if (PosCella == NULL) {
                        // Lab->SendToBack();
                        PosCella = new TMyShape(ScrollBox1);
                        PosCella->Parent = ScrollBox1;
                        PosCella->Name = "Pos" + String(pos);
                        // stringa2 = (dmDB->QueryPos->FieldByName("IDUDC")->AsInteger > 0 ?  "- ID: "+dmDB->QueryPos->FieldByName("IDUDC")->AsString : "");
                        PosCella->Hint = "Pos " + String(pos) + " - " + dmDB->QueryPos->FieldByName("CustomerPos")->AsString;
                        // PosCella->Hint = dmDB->QueryPos->FieldByName("CustomerPos")->AsString.SubString(1, 1) + dmDB->QueryPos->FieldByName("CustomerPos")->AsString.SubString(4, 1);
                        PosCella->Shape = mstRoundRect; // mstRectangle;
                        PosCella->Tag = idimg;
                        PosCella->Font->Name = "Arial Narrow";
                        PosCella->Font->Size = 12;
                        PosCella->ShowHint = true;
                        PosCella->OnMouseMove = ShapePosMouseMove;
                        PosCella->OnMouseUp = ShapePosMouseUp;
                        PosCella->Brush->Style = bsClear;
                        // MappaPosizioni[idimg] = PosCella ;

                    }
                    else {
                        // PosCella->Hint = "Pos " + String(pos) + " - " + dmDB->QueryPos->FieldByName("CustomerPos")->AsString;
                        // + " ; IdTotem "+ dmDB->QueryPos->FieldByName("IdTotem")->AsString + " ; Type "+ dmDB->QueryPos->FieldByName("CodiceTipologia")->AsString;
                    }
                    if (PosCella != NULL) {
                        imgw = dmDB->QueryPos->FieldByName("Width")->AsInteger;
                        imgh = dmDB->QueryPos->FieldByName("Height")->AsInteger;
                        if ((imgw * zoom) != PosCella->Width) {
                            PosCella->Width = imgw * zoom;
                            PosCella->Height = imgh * zoom;
                        }
                        PosCella->Font->Size = min(8 + (zoom * 2), 12);

                        if (tab.mappa_nodi.find(pos) != tab.mappa_nodi.end()) {
                            x = Xo + ((Image1->Width * tab.mappa_nodi[pos].posx) / W) + Image1->Left;
                            y = Yo - ((Image1->Height * tab.mappa_nodi[pos].posy) / H) + Image1->Top;
                            variantetop = ((dmDB->QueryPos->FieldByName("DeltaY")->AsInteger * PosCella->Height) / H);
                            varianteleft = ((dmDB->QueryPos->FieldByName("DeltaX")->AsInteger * PosCella->Width) / W);
                            cellaTop = (y - PosCella->Height / 2) + variantetop;
                            cellaLeft = (x - PosCella->Width / 2) + varianteleft;
                            if ((cellaTop != PosCella->Top) || (cellaLeft != PosCella->Left)) {
                                PosCella->Top = cellaTop;
                                PosCella->Left = cellaLeft;
                            }
                        }
                        else {
                            PosCella->Visible = false;
                        }

                        prenotata = dmDB->QueryPos->FieldByName("Prenotata")->AsInteger;
                        escludi = dmDB->QueryPos->FieldByName("disabilita")->AsInteger + (1 - dmDB->QueryPos->FieldByName("abilitata")->AsInteger);
                        cornice = 0;
                        // pen per tipo posizione
                        if (dmDB->QueryPos->FieldByName("posinputplc")->AsInteger) {
                            if (dmDB->QueryPos->FieldByName("posinputplc")->AsInteger == TIPOLOGIA_DEPOSITO) { // unstacker
                                cornice = clLime;
                                PosCella->Pen->Width = 4;
                            }
                            else if (dmDB->QueryPos->FieldByName("posinputplc")->AsInteger == TIPOLOGIA_PRELIEVO) { // stacker
                                cornice = 0x00FFFFAA;
                                PosCella->Pen->Width = 4;
                            }
                            /*
                             else if (dmDB->QueryPos->FieldByName("tipoposizione")->AsInteger == TIPOLOGIA_BUFFER) {
                             cornice = clWhite;
                             PosCella->Pen->Width = 3;
                             }
                             */
                        }
                        else {
                            if (dmDB->QueryPos->FieldByName("tipoposizione")->AsInteger > 0) {
                                constcolor = 256 / NUMFILE; // diviso numero di pan max
                                // cornice = 11796480 - (255 + 256 * (constcolor * (dmDB->QueryPos->FieldByName("tipoposizione")->AsInteger - 1)));
                                cornice = dmDB->QueryPos->FieldByName("colore")->AsInteger;
                                PosCella->Pen->Width = 3;
                            }

                            else {
                                cornice = 0x00001;
                                PosCella->Pen->Width = 3;
                            }

                        }
                        pianioccupati = dmDB->QueryPos->FieldByName("npianiocc")->AsInteger;

                        if (dmDB->QueryPos->FieldByName("selezionata")->AsInteger > 0) {
                            // valutare
                            PosCella->Text = pianioccupati;
                            PosCella->Brush->Style = bsSolid;
                            PosCella->Brush->Color = clLime;
                        }
                        else if (pianioccupati > 0) {
                            if (dmDB->QueryPos->FieldByName("npiani")->AsInteger > 2)
                                constcolor = 256 / NUMPIANI_VUOTI; // diviso numero di piani max
                            else
                                constcolor = 256 / 4;
                            PosCella->Brush->Style = bsSolid;
                            PosCella->Brush->Color = 65790 - (255 + 256 * (constcolor * (pianioccupati - 1)));
                            PosCella->Text = pianioccupati;

                        }
                        else {
                            PosCella->Brush->Style = bsClear;
                            // PosCella->Brush->Color = clWhite;
                            PosCella->Text = pos; // dmDB->QueryPos->FieldByName("CustomerPos")->AsString ;

                            // PosCella->Text = dmDB->QueryPos->FieldByName("CustomerPos")->AsString;
                        }

                        if ((prelievo_missione_manuale > 0) && ((prelievo_missione_manuale == pos) || (prelievo_missione_manuale == dmDB->QueryPos->FieldByName("PosOpposta")->AsInteger))) {
                            cornice = clFuchsia;
                            PosCella->Pen->Width = 2;
                        }
                        else if (prenotata) {
                            cornice = clBlue;
                            PosCella->Pen->Width = 5;
                        }
                        else if (escludi) {
                            cornice = clRed;
                            PosCella->Pen->Width = 3;
                        }
                        else if (allarme) {
                            PosCella->Brush->Color = clBlack;
                            cornice = clFuchsia;
                            PosCella->Pen->Width = 2;
                        }

                        if (cornice != 0) {
                            PosCella->Pen->Color = cornice;
                            // PosCella->Pen->Width = 6;
                        }
                        else {
                            PosCella->Pen->Color = clBlack;
                            PosCella->Pen->Width = 2;
                        }
                    }
                    xselezione1 = min(x1sel, x2sel) + Image1->Left;
                    xselezione2 = max(x1sel, x2sel) + Image1->Left;
                    yselezione1 = min(y1sel, y2sel) + Image1->Top;
                    yselezione2 = max(y1sel, y2sel) + Image1->Top;

                    if ((PosCella->Top >= yselezione1) && (PosCella->Top <= yselezione2) && (PosCella->Left >= xselezione1) && (PosCella->Left <= xselezione2)) {
                        // if ((y >= yselezione1) &&(y <= yselezione2)&&(x >= xselezione1) &&(x <= xselezione2)) {
                        // sono dentro la selezione
                        if (PosCella->Pen->Color != clGray) {
                            PosCella->Pen->Color = clGray;
                            PosCella->Pen->Width = 3;
                            ridisegnapalsel = 1;
                        }
                    }
                    else if (PosCella->Pen->Color == clGray) {
                        PosCella->Pen->Color = clBlack;
                        PosCella->Pen->Width = 1;
                        ridisegnapalsel = 1;
                    }

                    numpallet = 0;
                    prenotata = 0;
                    allarme = 0;
                    dmDB->QueryPos->Next();
                }
            }
            dmDB->QueryPos->Close();

            init_mappa = 1;
            if (PaintBox1 == NULL) {
                PaintBox1 = new TPaintBox(ScrollBox1);
                PaintBox1->Parent = ScrollBox1;
                PaintBox1->OnMouseDown = Image1->OnMouseDown;
                PaintBox1->OnMouseMove = Image1->OnMouseMove;
                PaintBox1->OnDragOver = Image1->OnDragOver;
                PaintBox1->SendToBack();
                PaintBox1->Align = alClient;
            }

            CreaAggiornaPorteSuMappa();
            // riattivare???
            // CreaAggiornaPLCSuMappa();
            CreaAggiornaCBSuMappa();
            CreaAggiornaSemaforiSuMappa();
            x1sel = 0;
            x2sel = 0;
            y1sel = 0;
            y2sel = 0;
            // ScrollBox1->Show();

        }
        catch (...) {
        }
    }
    time_ultimo_ridisegno = clock();
    count_aggiorna_mappa_client = 0;

}

void TMainForm::CreaAggiornaPLCSuMappa() {
    AnsiString stringa, stringa2, str;
    double x, y, posx, posy, xselezione1, xselezione2, yselezione1, yselezione2;
    int imgw, imgh, countrecord, changestep, nobarra, escludi = 0, prenotata = 0, cellaTop, cellaLeft, i, variantetop, varianteleft; ;
    int pos, piano, numpallet = 0, idimg, tipo_posizione;
    TMyShape *PosCella;
    stringa = "SELECT * FROM  Posizioni where tipoposizione=98 or tipoposizione=97  order by pos";
    if (dmDB->ADOConnection1->Connected) {
        try {
            dmDB->QueryPos->Close();
            dmDB->QueryPos->SQL->Clear();
            dmDB->QueryPos->SQL->Append(stringa);
            dmDB->QueryPos->Open();
            if (dmDB->QueryPos->RecordCount > 0) {
                dmDB->QueryPos->First();
                while (!dmDB->QueryPos->Eof) {
                    pos = dmDB->QueryPos->FieldByName("Pos")->AsInteger;
                    tipo_posizione = dmDB->QueryPos->FieldByName("tipoposizione")->AsInteger;

                    if ((dmDB->QueryPos->FieldByName("Disabilita")->AsInteger))
                        escludi = 1;
                    else
                        escludi = 0;
                    PosCella = (TMyShape*) ScrollBox1->FindComponent("Pos" + String(pos));
                    if (PosCella == NULL) {
                        // Lab->SendToBack();
                        PosCella = new TMyShape(ScrollBox1);
                        PosCella->Parent = ScrollBox1;
                        PosCella->Name = "Pos." + String(pos);
                        PosCella->Shape = mstRectangle;
                        // PosCella->Text = IntToStr(idimg) ;
                        PosCella->Tag = pos;
                        PosCella->Font->Name = "Arial Narrow";
                        PosCella->Font->Size = 8;
                        PosCella->ShowHint = true;
                        PosCella->OnMouseMove = ShapePosMouseMove;
                        PosCella->OnMouseUp = ShapePosMouseUp;
                        PosCella->Brush->Style = bsClear;
                        // MappaPosizioni[idimg] = PosCella ;

                    }
                    if (PosCella != NULL) {
                        PosCella->Hint = PosCella->Name + " - PLC Tipo " + dmDB->QueryPos->FieldByName("PosInputPlc")->AsString;
                        imgw = dmDB->QueryPos->FieldByName("Width")->AsInteger;
                        imgh = dmDB->QueryPos->FieldByName("Height")->AsInteger;
                        imgw = ((Image1->Width * imgw) / W); // imgw * dmExtraFunction->ProporzionaZoom(zoom, 0.75f);
                        imgh = ((Image1->Height * imgh) / H);
                        if ((imgw * zoom) != PosCella->Width) {
                            PosCella->Width = imgw * zoom;
                            PosCella->Height = imgh * zoom;
                        }

                        PosCella->Font->Size = min(4 + (zoom * 2), 10);

                        if (tab.mappa_nodi.find(pos) != tab.mappa_nodi.end()) {
                            x = Xo + ((Image1->Width * tab.mappa_nodi[pos].posx) / W) + Image1->Left;
                            y = Yo - ((Image1->Height * tab.mappa_nodi[pos].posy) / H) + Image1->Top;
                            variantetop = ((dmDB->QueryPos->FieldByName("DeltaY")->AsInteger * PosCella->Height) / H);
                            varianteleft = ((dmDB->QueryPos->FieldByName("DeltaX")->AsInteger * PosCella->Width) / W);
                            cellaTop = (y - PosCella->Height / 2) + variantetop;
                            cellaLeft = (x - PosCella->Width / 2) + varianteleft;
                            if ((cellaTop != PosCella->Top) || (cellaLeft != PosCella->Left)) {
                                PosCella->Top = cellaTop;
                                PosCella->Left = cellaLeft;
                            }
                        }
                        else
                            PosCella->Visible = false;

                        xselezione1 = min(x1sel, x2sel) + Image1->Left;
                        xselezione2 = max(x1sel, x2sel) + Image1->Left;
                        yselezione1 = min(y1sel, y2sel) + Image1->Top;
                        yselezione2 = max(y1sel, y2sel) + Image1->Top;

                        if ((PosCella->Top >= yselezione1) && (PosCella->Top <= yselezione2) && (PosCella->Left >= xselezione1) && (PosCella->Left <= xselezione2)) {
                            // if ((y >= yselezione1) &&(y <= yselezione2)&&(x >= xselezione1) &&(x <= xselezione2)) {
                            // sono dentro la selezione
                            if (PosCella->Pen->Color != clGray) {
                                PosCella->Pen->Color = clGray;
                                PosCella->Pen->Width = 3;
                                ridisegnapalsel = 1;
                            }
                        }
                        else if (PosCella->Pen->Color == clGray) {
                            PosCella->Pen->Color = clBlack;
                            PosCella->Pen->Width = 1;
                            ridisegnapalsel = 1;
                        }

                    }
                    numpallet = 0;
                    prenotata = 0;
                    dmDB->QueryPos->Next();
                }
            }
            dmDB->QueryPos->Close();
        }
        catch (...) {
        }
    }

}

void TMainForm::CreaAggiornaCBSuMappa() {
    AnsiString stringa, strhint, str;
    double x, y, posx, posy, xselezione1, xselezione2, yselezione1, yselezione2;
    int imgw, imgh, countrecord, changestep, nobarra, escludi = 0, prenotata = 0, cellaTop, cellaLeft, i, variantetop, varianteleft;
    int pos, piano, numpallet = 0, idimg, tipo_posizione;
    TMyShape *PosCella;
    stringa = "SELECT * FROM  Posizioni where posinputplc>=96  order by pos";
    if (dmDB->ADOConnection1->Connected) {
        try {
            dmDB->QueryPos->Close();
            dmDB->QueryPos->SQL->Clear();
            dmDB->QueryPos->SQL->Append(stringa);
            dmDB->QueryPos->Open();
            if (dmDB->QueryPos->RecordCount > 0) {
                dmDB->QueryPos->First();
                while (!dmDB->QueryPos->Eof) {
                    pos = dmDB->QueryPos->FieldByName("Pos")->AsInteger;
                    tipo_posizione = dmDB->QueryPos->FieldByName("tipoposizione")->AsInteger;

                    if ((dmDB->QueryPos->FieldByName("Disabilita")->AsInteger))
                        escludi = 1;
                    else
                        escludi = 0;
                    PosCella = (TMyShape*) ScrollBox1->FindComponent("CB" + String(pos));

                    if (PosCella == NULL) {
                        // Lab->SendToBack();
                        PosCella = new TMyShape(ScrollBox1);
                        PosCella->Parent = ScrollBox1;
                        PosCella->Name = "CB" + String(pos);
                        PosCella->Hint = String(pos);
                        PosCella->Shape = mstRectangle;
                        // PosCella->Text = IntToStr(idimg) ;
                        PosCella->Tag = pos;
                        PosCella->Font->Name = "Arial Narrow ";
                        PosCella->Font->Size = 8;
                        PosCella->ShowHint = true;
                        PosCella->OnMouseMove = NULL; // ShapePosMouseMove;
                        PosCella->OnMouseUp = NULL; // ShapePosMouseUp;
                        PosCella->Brush->Style = bsSolid;
                    }
                    if (PosCella != NULL) {
                        imgw = dmDB->QueryPos->FieldByName("Width")->AsInteger;
                        imgh = dmDB->QueryPos->FieldByName("Height")->AsInteger;
                        imgw = ((Image1->Width * imgw) / W); // imgw * dmExtraFunction->ProporzionaZoom(zoom, 0.75f);
                        imgh = ((Image1->Height * imgh) / H);
                        if ((imgw * zoom) != PosCella->Width) {
                            PosCella->Width = imgw * zoom;
                            PosCella->Height = imgh * zoom;
                        }
                        if (escludi)
                            PosCella->Brush->Color = clRed;
                        else
                            PosCella->Brush->Color = clPurple;

                        PosCella->Font->Size = min(4 + (zoom * 2), 10);
                        if (tab.mappa_nodi.find(pos) != tab.mappa_nodi.end()) {
                            // se si vuole dare la posizione dal nodo
                            x = Xo + ((Image1->Width * tab.mappa_nodi[pos].posx) / W) + Image1->Left;
                            y = Yo - ((Image1->Height * tab.mappa_nodi[pos].posy) / H) + Image1->Top;
                            variantetop = ((dmDB->QueryPos->FieldByName("DeltaY")->AsInteger * PosCella->Height) / H);
                            varianteleft = ((dmDB->QueryPos->FieldByName("DeltaX")->AsInteger * PosCella->Width) / W);
                            cellaTop = (y - PosCella->Height / 2) + variantetop;
                            cellaLeft = (x - PosCella->Width / 2) + varianteleft;
                            if ((cellaTop != PosCella->Top) || (cellaLeft != PosCella->Left)) {
                                PosCella->Top = cellaTop;
                                PosCella->Left = cellaLeft;
                            }
                        }
                        else
                            PosCella->Visible = false;
                    }
                    dmDB->QueryPos->Next();
                }
            }
            dmDB->QueryPos->Close();
        }
        catch (...) {
        }
    }

}

void TMainForm::CreaAggiornaPorteSuMappa() {
    AnsiString stringa, stringa2, str;
    double x, y, posx, posy, xselezione1, xselezione2, yselezione1, yselezione2;
    int imgw, imgh, countrecord, changestep, nobarra, escludi = 0, prenotata = 0, cellaTop, cellaLeft, i;
    int pos, piano, numpallet = 0, idimg, tipologia, numarelle;
    TMyShape *PosCella;
    /* TLabel *Lab ;
     TTime TempoDisegnaMappa ; */
    // ridisegnapalsel = 0 ;
    // stringa = "Select * From Posizioni inner join Corsie on Posizioni.Fila = Corsie.Fila order by Corsie.Fila, Pos, Piano ";
    // stringa = "Select * From Corsie inner join Posizioni on Corsie.Fila = Posizioni.Fila inner join Udc on(Posizioni.IdArticolo1 = Udc.IdArticolo OR Posizioni.IdArticolo2 = Udc.IdArticolo OR Posizioni.IdArticolo3 = Udc.IdArticolo OR Posizioni.IdArticolo4 = Udc.IdArticolo) order by Posizioni.Fila, Pos ";
    stringa = "Select * From Porte order by idporta ";
    if (dmDB->ADOConnection1->Connected) {
        try {
            dmDB->QueryPos->Close();
            dmDB->QueryPos->SQL->Clear();
            dmDB->QueryPos->SQL->Append(stringa);
            dmDB->QueryPos->Open();
            if (dmDB->QueryPos->RecordCount > 0) {
                dmDB->QueryPos->First();
                while (!dmDB->QueryPos->Eof) {
                    idimg = dmDB->QueryPos->FieldByName("IdPorta")->AsInteger;
                    PosCella = (TMyShape*) ScrollBox1->FindComponent("Porta" + dmDB->QueryPos->FieldByName("IdPorta")->AsString);
                    if (PosCella == NULL) {
                        // Lab->SendToBack();
                        PosCella = new TMyShape(ScrollBox1);
                        PosCella->Parent = ScrollBox1;
                        PosCella->Name = "Porta" + dmDB->QueryPos->FieldByName("IdPorta")->AsString;
                        PosCella->Hint = dmDB->QueryPos->FieldByName("Descrizione")->AsString;
                        PosCella->Shape = mstRectangle;
                        PosCella->Text = "";
                        PosCella->Tag = idimg;
                        PosCella->Font->Name = "Arial Narrow ";
                        PosCella->Font->Size = 8;
                        PosCella->ShowHint = true;
                        PosCella->OnMouseMove = ShapePosMouseMove;
                        PosCella->OnMouseUp = ShapePortaMouseUp;
                    }
                    if (PosCella != NULL) {
                        imgw = dmDB->QueryPos->FieldByName("Width")->AsInteger;
                        imgh = dmDB->QueryPos->FieldByName("Height")->AsInteger;
                        posx = dmDB->QueryPos->FieldByName("PosX")->AsInteger;
                        posy = dmDB->QueryPos->FieldByName("PosY")->AsInteger;
                        if ((imgw * zoom) != PosCella->Width) {
                            PosCella->Width = ((Image1->Width * imgw) / W); // imgw * dmExtraFunction->ProporzionaZoom(zoom, 0.75f);
                            PosCella->Height = ((Image1->Height * imgh) / H); // imgh * dmExtraFunction->ProporzionaZoom(zoom, 0.75f);
                        }
                        if (zoom == 1)
                            PosCella->Font->Size = 4;
                        else if (zoom == 2)
                            PosCella->Font->Size = 6;
                        else if (zoom > 2)
                            PosCella->Font->Size = 8;
                        x = Xo + ((Image1->Width * posx) / W) + Image1->Left;
                        y = Yo - ((Image1->Height * posy) / H) + Image1->Top;

                        cellaTop = y;

                        cellaLeft = x;
                        if ((cellaTop != PosCella->Top) ||
                            (cellaLeft != PosCella->Left)) {
                            PosCella->Top = cellaTop;
                            PosCella->Left = cellaLeft;
                        }
                    }
                    dmDB->QueryPos->Next();
                }
            }
            dmDB->QueryPos->Close();
        }
        catch (...) {
        }
    }
}

void TMainForm::CreaAggiornaSemaforiSuMappa() {
    double x, y, posx, posy;
    int cellaTop, cellaLeft, i = 1;
    AnsiString stringa;
    TPanel *PanSemaforo;
    TShape *OggettoShape;
    TShape *Pos;

    /* stringa = "Select * From semafori order by id ";
     if (dmDB->ADOConnection1->Connected) {
     try {
     dmDB->QueryPos->Close();
     dmDB->QueryPos->SQL->Clear();
     dmDB->QueryPos->SQL->Append(stringa);
     dmDB->QueryPos->Open();
     if (dmDB->QueryPos->RecordCount > 0) {
     // dmDB->QueryPos->First();
     while (!dmDB->QueryPos->Eof) {
     PanSemaforo = (TPanel*) FindComponent("PanSemaforo" + dmDB->QueryPos->FieldByName("Id")->AsString);
     if (PanSemaforo != NULL) {
     x = Xo + ((Image1->Width * dmDB->QueryPos->FieldByName("PosX")->AsInteger) / W) + Image1->Left;
     y = Yo - ((Image1->Height * dmDB->QueryPos->FieldByName("PosY")->AsInteger) / H) + Image1->Top;
     cellaTop = y - dmDB->QueryPos->FieldByName("height")->AsInteger / 2;
     cellaLeft = x - dmDB->QueryPos->FieldByName("width")->AsInteger / 2;

     if ((cellaTop != PanSemaforo->Top) || (cellaLeft != PanSemaforo->Left)) {
     PanSemaforo->Top = cellaTop;
     PanSemaforo->Left = cellaLeft;
     }
     if (!PanSemaforo->Visible)
     PanSemaforo->Visible = true;
     ClientData.DatiSemafori[i].oggetto_creato = 1;
     }
     OggettoShape = (TShape*) FindComponent("sGiallo" + IntToStr(i));
     if (OggettoShape != NULL) {
     if (ClientData.DatiSemafori[i].LuceGialla) {
     OggettoShape->Brush->Color = clYellow;
     }
     else
     OggettoShape->Brush->Style = bsClear;
     }
     OggettoShape = (TShape*) FindComponent("sVerde" + IntToStr(i));
     if (OggettoShape != NULL) {
     if (ClientData.DatiSemafori[i].LuceVerde) {
     OggettoShape->Brush->Color = clLime;
     }
     else
     OggettoShape->Brush->Style = bsClear;
     }
     OggettoShape = (TShape*) FindComponent("sRosso" + IntToStr(i));
     if (OggettoShape != NULL) {
     if (ClientData.DatiSemafori[i].LuceRossa) {
     OggettoShape->Brush->Color = clRed;
     }
     else
     OggettoShape->Brush->Style = bsClear;
     }
     OggettoShape = (TShape*) FindComponent("sRichiesta" + IntToStr(i));
     if (OggettoShape != NULL) {
     if (ClientData.DatiSemafori[i].Allarme) {
     OggettoShape->Brush->Color = clBlue;
     }
     else
     OggettoShape->Brush->Style = bsClear;
     }
     i++;
     dmDB->QueryPos->Next();
     }
     }
     }
     catch (...) {
     }
     }
     */
}

void __fastcall TMainForm::ShapePortaMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    TMyShape *Pos;
    // apro il popup
    int tipopos, i, popupok = 0, idplc;
    PopupMenuPos->Items->Clear();
    Pos = (TMyShape*) Sender;
    if (Pos != NULL) {
        if (Button == mbRight) {
            // vedere se abilitare/disabilitare la porta col tasto destro
        }
        else if (Button == mbLeft) {
            if (Pos->Tag) {
                FormPortStatus->idport = Pos->Tag;
                FormPortStatus->Show();
            }
        }
    }
}

void __fastcall TMainForm::ShapePosMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    TMyShape *Pos;
    // apro il popup
    int tipopos, i, popupok = 0, tipoplc, corsia, tipologia_corsia, zonadapos, autorizzazione;
    PopupMenuPos->Items->Clear();
    Pos = (TMyShape*) Sender;
    if (dmDB->ZonaString != "XXX") {
        if (Pos != NULL) {
            corsia = dmDB->TabPostazioni[IntToStr(Pos->Tag)]["FILA"].ToIntDef(0);
            if (Button == mbRight) {
                if ((mousedown) && (x1 != x2) && (y1 != y2) && (Contatore > 0) && (ret) && (PaintBox1 != NULL)) {
                    if (Pos != NULL) {
                        X = Pos->Left + X;
                        Y = Pos->Top + Y;
                        Image1MouseUp(Sender, Button, Shift, X, Y);
                    }
                }
                else {
                    /*
                     if (agvdaspostare) {
                     TMenuItem *NewItem1 = new TMenuItem(this); // create the separator
                     NewItem1->Caption = "Sposto Lgv " + IntToStr(agvdaspostare) + " in Posizione " + IntToStr(Pos->Tag) + "? ";
                     NewItem1->Tag = Pos->Tag;
                     NewItem1->OnClick = MoveAgv1Click;
                     PopupMenuPos->Items->Add(NewItem1);
                     if (ClientData.DatiAgv[agvdaspostare].load) {
                     TMenuItem *NewItem2 = new TMenuItem(this); // create the separator
                     NewItem2->Caption = "Sposto Lgv " + IntToStr(agvdaspostare) + " in Posizione " + IntToStr(Pos->Tag) + " e deposito ? ";
                     NewItem2->Tag = Pos->Tag;
                     NewItem2->OnClick = DropAgv1Click;
                     PopupMenuPos->Items->Add(NewItem2);
                     }
                     popupok = 1;
                     }
                     */
                    TMenuItem *NewItem2 = new TMenuItem(this); // create the separator
                    NewItem2->Caption = ("Disabilita le posizioni selezionate");
                    NewItem2->Tag = Pos->Tag;
                    NewItem2->OnClick = ExcludePosition1Click;
                    PopupMenuPos->Items->Add(NewItem2);
                    TMenuItem *NewItem3 = new TMenuItem(this); // create the separator
                    NewItem3->Caption = ("Abilita le posizioni selezionate");
                    NewItem3->Tag = Pos->Tag;
                    NewItem3->OnClick = RemoveExclusion1Click;
                    PopupMenuPos->Items->Add(NewItem3);
                    PopupMenuPos->Items->InsertNewLineAfter(NewItem3);
                    // ---- sezione extra posizioni contestuali
                    // se non e' un plc / lavatrice
                    if (!dmDB->RitornaPosInputPlc(Pos->Tag)) {
                        if (dmDB->pwdlevel) {
                            TMenuItem *NewItem4 = new TMenuItem(this); // create the separator
                            NewItem4->Caption = DataForm->Traduci("MAINMENU", 7); // " Empty Lane";
                            NewItem4->Tag = Pos->Tag;
                            NewItem4->OnClick = Svuotacorsia1Click;
                            PopupMenuPos->Items->Add(NewItem4);
                        }
                        TMenuItem *NewItem5 = new TMenuItem(this); // create the separator
                        NewItem5->Caption = ("Disabilita Corsia");
                        NewItem5->Tag = Pos->Tag;
                        NewItem5->OnClick = DisabilitaCorsia1Click;
                        PopupMenuPos->Items->Add(NewItem5);
                        TMenuItem *NewItem6 = new TMenuItem(this); // create the separator
                        NewItem6->Caption = ("Abilita Corsia");
                        NewItem6->Tag = Pos->Tag;
                        NewItem6->OnClick = Abilitacorsia1Click;
                        PopupMenuPos->Items->Add(NewItem6);
                        PopupMenuPos->Items->InsertNewLineAfter(NewItem6);

                        // c'e' un udc in posizione
                        if (dmDBClient->TornaUDCinPosizione(Pos->Tag)) {
                            // 8
                            TMenuItem *NewItem8 = new TMenuItem(this); // create the separator
                            NewItem8->Caption = DataForm->Traduci("MAINMENU", 15); // "Empty Position";
                            NewItem8->Tag = Pos->Tag;
                            NewItem8->OnClick = svuotapos1Click;
                            PopupMenuPos->Items->Add(NewItem8);

                            // 9 spostamento
                            if (CentroMissione.posprel == 0) {
                                TMenuItem *NewItem9 = new TMenuItem(this); // create the separator
                                NewItem9->Caption = "Crea Missione di spostamento ";
                                NewItem9->Tag = Pos->Tag; // mi salvo l'id dell'agv
                                NewItem9->OnClick = MisManSpost1Click;
                                PopupMenuPos->Items->Add(NewItem9);
                            }
                        }
                        // senza udc in poszione    MA INZIO PRELIEVO PER SPOSTAMENTO
                        if ((CentroMissione.posprel != 0) && (CentroMissione.CodTipoMovimento == 3)) {
                            TMenuItem *NewItem9 = new TMenuItem(this); // create the separator
                            NewItem9->Caption = "Preleva da " + String(CentroMissione.posprel) + ", ora scegli destinazione ";
                            NewItem9->Tag = Pos->Tag; // mi salvo l'id dell'agv
                            NewItem9->OnClick = MisManSpost1Click;
                            PopupMenuPos->Items->Add(NewItem9);
                        }
                    }
                    else {
                        if (dmDB->pwdlevel) {

                            TMenuItem *NewItem4 = new TMenuItem(this); // create the separator
                            NewItem4->Caption = "Crea Missione di Prelievo da PLC " + IntToStr(Pos->Tag);
                            NewItem4->Tag = Pos->Tag;
                            NewItem4->OnClick = MisManSpostPLCClick;
                            PopupMenuPos->Items->Add(NewItem4);

                        }
                    }
                    // se  non e' un plc / lavatrice  e c'e' un udc
                    if (dmDB->IdUDCDaPosizione(Pos->Tag)) {
                        // 9:prel spostamento
                        TMenuItem *NewItem10 = new TMenuItem(this); // create the separator
                        NewItem10->Caption = DataForm->Traduci("MAINMENU", 18); // " sposta UDC";
                        NewItem10->Tag = Pos->Tag;
                        NewItem10->Hint = "1";
                        NewItem10->OnClick = PrelevaUDC1Click;
                        PopupMenuPos->Items->Add(NewItem10);
                    }
                    // se e' stata segnato il prelievo dalla missione manuale
                    else if ((CentroMissione.posprel != 0) && (!dmDB->IdUDCDaPosizione(Pos->Tag))) {
                        // 9:dep
                        TMenuItem *NewItem10 = new TMenuItem(this); // create the separator
                        NewItem10->Caption = DataForm->Traduci("MAINMENU", 19); // " deposita UDC da spostamento";
                        NewItem10->Tag = Pos->Tag;
                        NewItem10->Hint = "1";
                        NewItem10->OnClick = DepositaUDC1Click;
                        PopupMenuPos->Items->Add(NewItem10);
                    }

                    popupok = 1;

                    if (popupok) {
                        PopupMenuPos->Tag = Pos->Tag;
                        PopupMenuPos->Popup(Left + X + Pos->Left, Top + Y + Pos->Top + Panel2->Height);
                    }
                }

            }
            else if (Button == mbLeft) {
                agvdaspostare = 0;
                if (Pos->Tag) {
                    tipoplc = dmDB->RitornaPosInputPlc(Pos->Tag);
                    pos_udc = Pos->Tag;
                    if (tipoplc == TIPOLOGIA_PRELIEVO) {
                        FormPosPrelPLC->pos = pos_udc;
                        FormPosPrelPLC->ShowModal();

                    }
                    else if (tipoplc == TIPOLOGIA_DEPOSITO) {
                        FormPosDepPLC->pos = pos_udc;
                        FormPosDepPLC->ShowModal();
                    }
                    else {
                        corsia_udc = dmDB->ReturnFilaDaPos(pos_udc);
                        CreateMDIChild(fCorsia, "fCorsia", "Gestione posizione");
                    }
                }
            }
            //
            if (prelievo_missione_manuale) {
                prelievo_missione_manuale = 0;
                AggiornaMappa();
            }
            // spostaframe = 0 ;
            // ridisegnapalsel = 0;
        }
    }

}

void __fastcall TMainForm::ShapePosMouseMove(TObject * Sender,
    TShiftState Shift, int X, int Y)
{
    //
    TMyShape *Shape;
    Shape = (TMyShape*) Sender;
    if ((Shape != NULL) && (Shape->Tag)) {
        if (Shape->Tag != ultimapospassata) {
            // ultimapospassata = Shape->Tag ;
            // AggiornaDatiPos(ultimapospassata) ;
        }
    }
}

void __fastcall TMainForm::sSinotticoClick(TObject * Sender)
{
    // dmDB->InserisciCorsiePostazioniDatabase();
    int i;
    for (i = MDIChildCount - 1; i >= 0; i--) {
        if (MDIChildren[i]->Name != Name) {
            ((TMDIChild*)MDIChildren[i])->SetInvisible();
            MDIChildren[i]->Close();
        }
    }
    AggiornaMappa();
    // CreateMDIChild(fCorsia," fCorsia","Lane and Position Status");
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::Image1MouseDown(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    try {
        if (Button == mbMiddle) {
            xmouse = 0;
            ymouse = 0;
            countmousemove = 0;
            Screen->Cursor = crHandPoint;
            Screen->Tag = 0;
        }
        else if (Button == mbLeft) {
            retsel = false;
            x1sel = X;
            y1sel = Y;
            x2sel = x1sel;
            y2sel = y1sel;
            mousedownsel = true;
            Contatoresel = 0;
        }
        else if ((Button == mbRight) && (zoom < 6)) {
            ret = false;
            x1 = X;
            y1 = Y;
            x2 = x1;
            y2 = y1;
            mousedown = true;
            Contatore = 0;
        }
        else if ((Button == mbRight) && (zoom >= 6)) {
            ExtendSpeedButtonClick(this);
        }
    }
    catch (...) {
    }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::Image1MouseUp(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    bool Hand;
    TPoint MP;
    float RapportoOriz, RapportoVert, zoomprec;

    try {
        if (Screen->Cursor == crHandPoint)
            Screen->Cursor = crDefault;

        if ((((mousedown) && (x1 != x2) && (y1 != y2) && (Button == mbRight)) ||
                ((mousedownsel) && (x1sel != x2sel) && (y1sel != y2sel) && (Button == mbLeft))) && (PaintBox1 != NULL)) {
            // disegno zoom in base a rettangolo
            if (Button == mbRight) {
                // col destro zoom
                if ((zoom <= 1) || (zoom >= 6))
                    zoomprec = zoom - 1;
                else
                    zoomprec = zoom;
                zoom = 2;
                Image1->Width = widthextend * zoom;
                Image1->Height = heightextend * zoom;
                Image1->Top = 0;
                Image1->Left = 0;
                ScrollBox1->HorzScrollBar->Range = Image1->Width;
                ScrollBox1->VertScrollBar->Range = Image1->Height;
                if (x1 > x2)
                    x1 = x2; // + (abs(x1-x2)/2);
                else
                    x1 = x1; // + (abs(x2-x1)/2);
                if (y1 > y2)
                    y1 = y2; // +(abs(y1-y2)/2);
                else
                    y1 = y1; // +(abs(y2-y1)/2);
                ScrollBox1->HorzScrollBar->Position = x1 * (zoom - zoomprec);
                ScrollBox1->VertScrollBar->Position = y1 * (zoom - zoomprec);
                Xo = (Xo * Image1->Width) / FormW; // Ricalcolo l'origine in base alla dimensione della finestra
                Yo = (Yo * Image1->Height) / FormH;
                FormH = Image1->Height;
                FormW = Image1->Width;
                AggiornaMappa();
            }
            else if (Button == mbLeft) {
                // col sinistro seleziono
                AggiornaMappa();
            }
        }
        else if (ridisegnapalsel) {
            ridisegnapalsel = 0;
            AggiornaMappa();
        }
    }
    catch (...) {
    }
    agvdaspostare = 0;
    mousedown = false;
    mousedownsel = false;

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::Image1MouseMove(TObject * Sender, TShiftState Shift, int X, int Y) {
    TShiftState app, appleft, appright;
    TImage *CellaImg;
    AnsiString str;
    int cz;
    TLabel *Lab;
    app << ssMiddle;
    TImage *Img;
    TRotateImage *MyImg;
    TMyShape *MyPos;
    try {

        if ((Shift == app) && (Screen->Cursor == crHandPoint)) {
            if (String(Sender->ClassName()) == "TMyShape") {
                MyPos = (TMyShape*) Sender;
                if (MyPos != NULL) {
                    X = MyPos->Left + X;
                    Y = MyPos->Top + Y;
                }
                // cerco l'immagine
            }
            if (xmouse == 0) {
                xmouse = X;
                ymouse = Y;
                return;
            }
            if ((countmousemove >= 1) || (ymouse == 0)) {
                // Image1->Hide();
                if (xmouse < X)
                    ScrollBox1->HorzScrollBar->Position -= abs(X - xmouse);
                else if (xmouse > X)
                    ScrollBox1->HorzScrollBar->Position += abs(X - xmouse);
                if (ymouse < Y)
                    ScrollBox1->VertScrollBar->Position -= abs(Y - ymouse);
                else if (ymouse > Y)
                    ScrollBox1->VertScrollBar->Position += abs(Y - ymouse);
                countmousemove = 0;
                // Image1->Show();
            }
            else
                countmousemove++;
        }
        else {
            if (String(Sender->ClassName()) == "TMyShape") {
                MyPos = (TMyShape*) Sender;
                if (MyPos != NULL) {
                    X = MyPos->Left + X;
                    Y = MyPos->Top + Y;
                }
                // cerco l'immagine
            }
            else if (String(Sender->ClassName()) == "TRotateImage") {
                MyImg = (TRotateImage*) Sender;
                if (MyImg != NULL) {
                    X = MyImg->Left + X;
                    Y = MyImg->Top + Y;
                }
                // cerco l'immagine
            }
            appleft << ssLeft;
            if (Shift == appleft) {
                if ((Contatore > 0) && (ret) && (PaintBox1 != NULL)) {
                    PaintBox1->Canvas->Pen->Mode = pmNot;
                    PaintBox1->Canvas->Pen->Color = clWhite;
                    PaintBox1->Canvas->Brush->Style = bsClear;
                    PaintBox1->Canvas->Rectangle(x1sel, y1sel, x2sel, y2sel);
                }
                if (PaintBox1 != NULL) {
                    PaintBox1->Canvas->Pen->Style = psDot;
                    PaintBox1->Canvas->Pen->Color = clBlack;
                    PaintBox1->Canvas->Brush->Style = bsClear;
                    PaintBox1->Canvas->Rectangle(x1sel, y1sel, X, Y);
                    x2sel = X;
                    y2sel = Y;
                    ret = true;
                    Contatore++;
                }
            }
            appright << ssRight;
            if ((Shift == appright) && (zoom < 2)) {
                if ((Contatore > 0) && (ret) && (PaintBox1 != NULL)) {
                    PaintBox1->Canvas->Pen->Mode = pmNot;
                    PaintBox1->Canvas->Pen->Color = clWhite;
                    PaintBox1->Canvas->Brush->Style = bsClear;
                    PaintBox1->Canvas->Rectangle(x1, y1, x2, y2);
                }
                if (PaintBox1 != NULL) {
                    PaintBox1->Canvas->Pen->Style = psDot;
                    PaintBox1->Canvas->Pen->Color = clBlack;
                    PaintBox1->Canvas->Brush->Style = bsClear;
                    PaintBox1->Canvas->Rectangle(x1, y1, X, Y);
                    x2 = X;
                    y2 = Y;
                    ret = true;
                    Contatore++;
                }
            }

        }

        QuotaX->Caption = "X :" + IntToStr(X);
        QuotaY->Caption = "Y : " + IntToStr(Y);
        ultimapospassata = 0;
    }
    catch (...) {
    }
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::ExcludePosition1Click(TObject * Sender)
{
    int escludi;
    escludi = dmDB->bit[0] | dmDB->bit[1];
    EscludiPosSelezionate(PopupMenuPos->Tag, escludi);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::RemoveExclusion1Click(TObject * Sender)
{
    EscludiPosSelezionate(PopupMenuPos->Tag, 0);
}

void __fastcall TMainForm::ZoomInSpeedButtonClick(TObject * Sender)
{
    TShiftState Shift;
    bool Hand;
    TPoint MP;
    MP.x = 700; // centro il mouse
    MP.y = 450;
    countwheel = 3;
    FormMouseWheel(this, Shift, 130, MP, Hand);

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::ExtendSpeedButtonClick(TObject * Sender) {
    TShiftState Shift;
    bool Hand;
    TTime tempo;
    TPoint MP;
    tempo = Time() - StrToTime("00 : 00 : 01");
    if (tempo > ScrollMouse) {
        MP.x = 700; // centro il mouse
        MP.y = 450;
        countwheel = 3;
        Image1->Width = widthextend * 2;
        Image1->Height = heightextend * 2;
        zoom = 2;
        FormMouseWheel(this, Shift, -130, MP, Hand);
    }
    else
        MainForm->AggiornaMappa();

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::ZoomOutSpeedButtonClick(TObject * Sender) {
    TShiftState Shift;
    bool Hand;
    TPoint MP;
    MP.x = 700; // centro il mouse
    MP.y = 450;
    countwheel = 3;
    FormMouseWheel(this, Shift, -130, MP, Hand);

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::img2MouseUp(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    TRotateImage *Img;
    int pos_sel;
    Img = (TRotateImage*) Sender;
    PopupMenu1->Items->Clear();
    if (dmDB->ZonaString != "XXX") {
        if ((Button == mbRight) && (Img != NULL)) {
            if ((!ClientData.DatiAgv[Img->Tag].manuale) && (!ClientData.DatiAgv[Img->Tag].alarm) && (ClientData.DatiAgv[Img->Tag].pos > 0) && (!ClientData.DatiAgv[Img->Tag].idmis)) {
                // abilitare quando è pronta sull'agv
                TMenuItem *NewItem1 = new TMenuItem(this); // create the separator
                NewItem1->Caption = "Crea missione di movimento su posizione per AGV n° " + IntToStr(Img->Tag) + ".";
                NewItem1->Tag = Img->Tag; // mi salvo l'id dell'agv
                NewItem1->OnClick = MoveMis1Click;
                PopupMenu1->Items->Add(NewItem1);
            }
            else if ((!ClientData.DatiAgv[Img->Tag].manuale) && (dmDB->pwdlevel) && (!ClientData.DatiAgv[Img->Tag].alarm) && (ClientData.DatiAgv[Img->Tag].pos > 0) && (ClientData.DatiAgv[Img->Tag].load) && (!ClientData.DatiAgv[Img->Tag].idmis))
            {
                // abilitare quando è pronta sull'agv
                TMenuItem *NewItem2 = new TMenuItem(this); // create the separator
                NewItem2->Caption = "Crea Missione di Deposito per AGV n° " + IntToStr(Img->Tag) + "!";
                NewItem2->Tag = Img->Tag; // mi salvo l'id dell'agv
                NewItem2->OnClick = DropMis1Click;
                PopupMenu1->Items->Add(NewItem2);
            }
            if ((!ClientData.DatiAgv[Img->Tag].manuale) && (!ClientData.DatiAgv[Img->Tag].alarm) && (ClientData.DatiAgv[Img->Tag].pos > 0) && (!ClientData.DatiAgv[Img->Tag].idmis)) {
                // abilitare quando è pronta sull'agv
                TMenuItem *NewItem1 = new TMenuItem(this); // create the separator
                NewItem1->Caption = "Crea missione movimento su nodo per AGV n° " + IntToStr(Img->Tag) + ".";
                NewItem1->Tag = Img->Tag; // mi salvo l'id dell'agv
                NewItem1->OnClick = MoveMisMan1Click;
                PopupMenu1->Items->Add(NewItem1);
                PopupMenu1->Items->InsertNewLineAfter(NewItem1);
            }

            if ((!ClientData.DatiAgv[Img->Tag].manuale) && (dmDB->pwdlevel) && ((!ClientData.DatiAgv[Img->Tag].alarm) || (ClientData.DatiAgv[Img->Tag].stand_by)) && (ClientData.DatiAgv[Img->Tag].pos > 0)) {
                // abilitare quando è pronta sull'agv
                TMenuItem *NewItem5 = new TMenuItem(this); // create the separator
                if (!ClientData.DatiAgv[Img->Tag].stand_by)
                    NewItem5->Caption = "Stand By AGV n° " + IntToStr(Img->Tag) + "!";
                else
                    NewItem5->Caption = "Restart AGV n° " + IntToStr(Img->Tag) + "!";
                NewItem5->Tag = Img->Tag; // mi salvo l'id dell'agv
                NewItem5->OnClick = StandBy1Click;
                PopupMenu1->Items->Add(NewItem5);
            }

            if ((ClientData.DatiAgv[Img->Tag].pos > 0) && (dmDB->pwdlevel)) {
                // abilitare quando è pronta sull'agv
                TMenuItem *NewItem6 = new TMenuItem(this); // create the separator
                NewItem6->Caption = "Escludi Lgv n° " + IntToStr(Img->Tag) + " dal sistema";
                NewItem6->Tag = Img->Tag; // mi salvo l'id dell'agv
                NewItem6->OnClick = escludiagv1Click;
                PopupMenu1->Items->Add(NewItem6);
                PopupMenu1->Items->InsertNewLineAfter(NewItem6);
            }
            TMenuItem *NewItem7 = new TMenuItem(this); // create the separator
            NewItem7->Caption = "Porta AGV n° " + IntToStr(Img->Tag) + " in secondo piano";
            NewItem7->Tag = Img->Tag; // mi salvo l'id dell'agv
            NewItem7->OnClick = SecondoPianoAGVClick;
            PopupMenu1->Items->Add(NewItem7);
            pos_sel = RicercaImgLayout(Img->Left + X, Img->Top + Y);
        }
        else if (Img != NULL) {
            agvdaspostare = 0;
            pos_sel = RicercaImgLayout(Img->Left + X, Img->Top + Y);
            if (pos_sel > 0) {
                if ((!dmDB->RitornaPosInputPlc(pos_sel)) && (pos_sel != dmDB->PosPesoCampione)) {
                    pos_udc = pos_sel;
                    corsia_udc = dmDB->ReturnFilaDaPos(pos_udc);
                    // corsia_udc = StrToInt(Pos->Name.SubString(Pos->Name.Pos("_"),1));//dmDB->ReturnFilaDaPos(pos_udc);
                    // CreateMDIChild(fCorsia, "fCorsia", "Room and Position Status UDC");
                }
                else if ((dmDB->RitornaPosInputPlc(pos_sel) < 50) && (dmDB->RitornaPosInputPlc(pos_sel) > 0)) {
                    // FormPlcCarico->pos = pos_sel;
                    // FormPlcCarico->Show();
                }
            }
        }

        else {
            pos_sel = RicercaPortaLayout(Img->Left + X, Img->Top + Y);
            /* if (pos_sel > 0) {
             FormPortStatus->idport = pos_sel;
             FormPortStatus->Show();
             }
             else { */
            pos_sel = RicercaAGVLayout(Img->Left + X, Img->Top + Y);
            if (pos_sel > 0) {
                fAGV->Show();
            }
            // }
        }
    }
}

// ---------------------------------------------------------------------------
void TMainForm::EscludiPosSelezionate(int pos, int escludi) {
    // escludo pos selezionate
    AnsiString stringa;
    TMyShape *Pal;
    bool sel = false, primo = true;
    stringa = "Update Posizioni Set Disabilita = " + IntToStr(escludi) + "where Pos = " + IntToStr(pos);
    // Image1->Visible = false ;
    dmDB->QueryMappa->Close();
    dmDB->QueryMappa->SQL->Clear();
    dmDB->QueryMappa->SQL->Append("Select * from Posizioni where Pos<>" + IntToStr(pos));
    dmDB->QueryMappa->Open();
    if (dmDB->QueryMappa->RecordCount) {
        dmDB->QueryMappa->First();
        while (!dmDB->QueryMappa->Eof) {
            Pal = (TMyShape*) ScrollBox1->FindComponent("Pos" + dmDB->QueryMappa->FieldByName("Pos")->AsString);
            if (Pal != NULL) {
                if (Pal->Pen->Color == clGray) {
                    stringa = stringa + " OR ";
                    stringa = stringa + " Pos = " + dmDB->QueryMappa->FieldByName("Pos")->AsString;
                    if (primo)
                        primo = false;
                    sel = true;
                }
            }
            dmDB->QueryMappa->Next();
        }
    }

    // Image1->Visible = true ;
    dmDB->QueryPos->Close();
    dmDB->QueryPos->SQL->Clear();
    dmDB->QueryPos->SQL->Append(stringa);
    dmDB->QueryPos->ExecSQL();
    dmDB->LogMsg(stringa);
    dmDB->QueryMappa->Close();
    dmDB->LogMsg("Escludi posizioni selezionate " + stringa);
    ridisegnapalsel = 0;
    /* if (MainForm->program != 1)
     MainForm->AggiornaMappaServer(5); */
    // aggiornamappa = 1 ;
    SocketDataModule->InviaAggiornamentoTabella(1);
    if (escludi)
        dmGestMissioni->GestisciMissioniPeso(pos);

}

void __fastcall TMainForm::FormMouseWheel(TObject * Sender, TShiftState Shift, int WheelDelta, TPoint & MousePos, bool &Handled) {
    // zoom
    int propy, propx, i;
    TTime tempo;
    TImage *Img;
    bool disegna;
    disegna = false;
    tempo = Time() - StrToTime("00 : 00 : 01");
    if (tempo > ScrollMouse) {
        if (WheelDelta >= 120) {
            if (countwheel > 2) {
                if (Image1->Width < 3000) {
                    MousePos.y = MousePos.y - (Height - ClientHeight) - Panel2->Top;
                    propy = MousePos.y;
                    propx = MousePos.x;
                    Image1->Width *= 2;
                    Image1->Height *= 2;
                    ScrollBox1->VertScrollBar->Range = Image1->Height;
                    ScrollBox1->HorzScrollBar->Range = Image1->Width;
                    ScrollBox1->HorzScrollBar->Position *= 2;
                    ScrollBox1->VertScrollBar->Position *= 2;
                    zoom *= 2;
                    disegna = true;
                    // MousePos.x = MousePos.x  *2  ;
                    ScrollBox1->HorzScrollBar->Position += propx; // ScrollBox1->HorzScrollBar->Position+MousePos.x;
                    ScrollBox1->VertScrollBar->Position += propy; // ScrollBox1->VertScrollBar->Position+MousePos.y;
                }
                ScrollMouse = Time();
                countwheel = 0;
            }
            else
                countwheel++;
        }
        else if ((WheelDelta <= -120) && (Image1->Width > 1400)) {
            if (countwheel > 2) {
                // Image1->Width = Image1->Width / 2;
                // Image1->Height = Image1->Height / 2;
                // Image1->Align = alClient ;
                MousePos.y = MousePos.y - (Height - ClientHeight) - Panel2->Top;
                if (((Image1->Width / 2) < widthextend) || ((Image1->Height / 2) < heightextend)) {
                    Image1->Width = widthextend;
                    Image1->Height = heightextend;
                }
                else {
                    Image1->Width /= 2;
                    Image1->Height /= 2;
                }
                // PanScroll->Top = 0 - ScrollBox1->VertScrollBar->Position;
                // PanScroll->Left = 0 - ScrollBox1->HorzScrollBar->Position;
                ScrollBox1->VertScrollBar->Range = Image1->Height;
                ScrollBox1->HorzScrollBar->Range = Image1->Width;
                ScrollBox1->HorzScrollBar->Position /= 2;
                ScrollBox1->VertScrollBar->Position /= 2;
                Image1->Top = 0;
                Image1->Left = 0;
                if (zoom >= 2)
                    zoom /= 2;
                disegna = true;
                Image1->Align = alNone;
                ScrollMouse = Time();
                countwheel = 0;
            }
            else
                countwheel++;
        }
        if (disegna) {
            Xo = (Xo * Image1->Width) / FormW; // Ricalcolo l'origine in base alla dimensione della finestra
            Yo = (Yo * Image1->Height) / FormH;
            FormH = Image1->Height;
            FormW = Image1->Width;
            for (i = 1; i <= NAGV; i++)
                DisegnaCarrello(i);
            AggiornaMappa();
            /*
             if (zoom == 1)
             gbInfo->Visible = true&dmDB->abilitainfobox;
             else if (zoom >= 2)
             gbInfo->Visible = false&dmDB->abilitainfobox;
             */
        }
        // CreaShapePos();
    }
    else
        countwheel = 0;

}

int TMainForm::RicercaImgLayout(int posx, int posy) {
    AnsiString stringa, stringa2, str;
    double x, y;
    int pos_return = 0;
    TMyShape *Pos;
    stringa = "Select * From Posizioni order by Pos ";

    if (dmDB->ADOConnection1->Connected) {
        try {
            dmDB->QueryPos->Close();
            dmDB->QueryPos->SQL->Clear();
            dmDB->QueryPos->SQL->Append(stringa);
            dmDB->QueryPos->Open();
            if (dmDB->QueryPos->RecordCount > 0) {
                dmDB->QueryPos->First();
                while ((!dmDB->QueryPos->Eof) && (pos_return == 0)) {
                    Pos = (TMyShape*) ScrollBox1->FindComponent("Pos" + dmDB->QueryPos->FieldByName("Pos")->AsString);
                    if (Pos) {
                        if (((posy >= Pos->Top) && (posy <= (Pos->Top + Pos->Height))) &&
                            ((posx >= Pos->Left) && (posx <= (Pos->Left + Pos->Width)))) { // trovata cella
                            pos_return = dmDB->QueryPos->FieldByName("Pos")->AsInteger;
                        }
                    }
                    dmDB->QueryPos->Next();
                }
            }
            dmDB->QueryPos->Close();
        }
        catch (...) {
        }
    }
    if (!pos_return) {
        stringa = "Select PosInputPLC, POS From Posizioni where PosInputPLC > 0 ";

        if (dmDB->ADOConnection1->Connected) {
            try {
                dmDB->QueryPos->Close();
                dmDB->QueryPos->SQL->Clear();
                dmDB->QueryPos->SQL->Append(stringa);
                dmDB->QueryPos->Open();
                if (dmDB->QueryPos->RecordCount > 0) {
                    dmDB->QueryPos->First();
                    while ((!dmDB->QueryPos->Eof) && (pos_return == 0)) {
                        Pos = (TMyShape*) ScrollBox1->FindComponent("Pos" + dmDB->QueryPos->FieldByName("Pos")->AsString);
                        if (Pos) {
                            if (((posy >= Pos->Top) && (posy <= (Pos->Top + Pos->Height))) &&
                                ((posx >= Pos->Left) && (posx <= (Pos->Left + Pos->Width)))) { // trovata cella
                                pos_return = dmDB->QueryPos->FieldByName("Pos")->AsInteger;
                            }
                        }
                        dmDB->QueryPos->Next();
                    }
                }
                dmDB->QueryPos->Close();
            }
            catch (...) {
            }
        }
    }

    return pos_return;
}

int TMainForm::RicercaPortaLayout(int posx, int posy) {
    AnsiString stringa, stringa2, str;
    double x, y;
    int pos_return = 0;
    TMyShape *Pos;
    stringa = "Select * From Porte order by IdPorta ";

    if (dmDB->ADOConnection1->Connected) {
        try {
            dmDB->QueryPos->Close();
            dmDB->QueryPos->SQL->Clear();
            dmDB->QueryPos->SQL->Append(stringa);
            dmDB->QueryPos->Open();
            if (dmDB->QueryPos->RecordCount > 0) {
                dmDB->QueryPos->First();
                while ((!dmDB->QueryPos->Eof) && (pos_return == 0)) {
                    Pos = (TMyShape*) ScrollBox1->FindComponent("Porta" + dmDB->QueryPos->FieldByName("IdPorta")->AsString);
                    if (Pos) {
                        if (((posy >= Pos->Top) && (posy <= (Pos->Top + Pos->Height))) &&
                            ((posx >= Pos->Left) && (posx <= (Pos->Left + Pos->Width)))) { // trovata cella
                            pos_return = dmDB->QueryPos->FieldByName("IdPorta")->AsInteger;
                        }
                    }
                    dmDB->QueryPos->Next();
                }
            }
            dmDB->QueryPos->Close();
        }
        catch (...) {
        }
    }
    return pos_return;
}

int TMainForm::RicercaAGVLayout(int posx, int posy) {
    AnsiString stringa, stringa2, str;
    double x, y;
    int pos_return = 0;
    TMyShape *Pos;

    stringa = "Select ID, POS From STATOAGV ";
    if (dmDB->ADOConnection1->Connected) {
        try {
            dmDB->QueryPos->Close();
            dmDB->QueryPos->SQL->Clear();
            dmDB->QueryPos->SQL->Append(stringa);
            dmDB->QueryPos->Open();
            if (dmDB->QueryPos->RecordCount > 0) {
                dmDB->QueryPos->First();
                while ((!dmDB->QueryPos->Eof) && (pos_return == 0)) {
                    Pos = (TMyShape*) ScrollBox1->FindComponent("Agv" + dmDB->QueryPos->FieldByName("ID")->AsString);
                    if (Pos) {
                        if (((posy >= Pos->Top) && (posy <= (Pos->Top + Pos->Height))) &&
                            ((posx >= Pos->Left) && (posx <= (Pos->Left + Pos->Width)))) { // trovata cella
                            pos_return = dmDB->QueryPos->FieldByName("Pos")->AsInteger;
                        }
                    }
                    dmDB->QueryPos->Next();
                }
            }
            dmDB->QueryPos->Close();
        }
        catch (...) {
        }
    }

    return pos_return;
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::MoveMis1Click(TObject * Sender)
{
    //
    int agv;
    TMenuItem *Item = (TMenuItem*) Sender;
    if (Item != NULL) {
        ShowMessage("Seleziona col TASTO DESTRO la destinazione per l'AGV n. " + IntToStr(Item->Tag) + ".");
        agvdaspostare = Item->Tag;
    }

}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::MoveMisMan1Click(TObject * Sender) {

    TMenuItem *Item = (TMenuItem*) Sender;
    // dmGestMissioni->MoveMisMan1Click(Item);
    dmGestMissioni->MoveMissioneManClick(Item);

}

// ---------------------------------------------------------------------------

void __fastcall TMainForm::MisManSpost1Click(TObject * Sender) {

    TMenuItem *Item = (TMenuItem*) Sender;
    dmGestMissioni->MisManSpost1Click(Item);

}

void __fastcall TMainForm::MisManSpostPLCClick(TObject * Sender) {

    TMenuItem *Item = (TMenuItem*) Sender;
    dmGestMissioni->MisManSpostPLCClick(Item);

}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::DropMis1Click(TObject * Sender)
{
    //
    int agv;
    TMenuItem *Item = (TMenuItem*) Sender;
    if (Item != NULL) {
        ShowMessage("Select drop position for Lgv " + IntToStr(Item->Tag) + "!!!(right mouse button)                          ");
        agvdaspostare = Item->Tag;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::MoveAgv1Click(TObject * Sender)
{
    TMenuItem *Item = (TMenuItem*) Sender;
    dmGestMissioni->MoveAgv1Click(Item, agvdaspostare);

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::DropAgv1Click(TObject * Sender)
{

    TMenuItem *Item = (TMenuItem*) Sender;
    dmGestMissioni->DropAgv1Click(Item, agvdaspostare);

}

void __fastcall TMainForm::StandBy1Click(TObject * Sender)
{
    //
    AnsiString s;
    TMenuItem *Item = (TMenuItem*) Sender;
    if (Item != NULL) {
        SocketDataModule->InviaStandByClient(Item->Tag, !ClientData.DatiAgv[Item->Tag].stand_by);
        /* s.printf("MSGTYPE = STANDBY | AGV = % d | VAL = % d ",Item->Tag, !DatiAgv[Item->Tag].stand_by);
         SocketDataModule->Invia(s); */
        // dmComandiAgv->StandByAgv(Item->Tag,!DatiAgv[Item->Tag].stand_by) ;
        /* else
         SocketDataModuleClient->Invia("MSGTYPE = STANDBY | AGV = "+IntToStr(Item->Tag)+"| VAL = "+IntToStr(!DatiAgv[Item->Tag].stand_by)+"|");
         */
    }

}

void TMainForm::VisualizzaPosPlc(ModuloPhoenix &StructPlc) {

    double x, y, posx, posy, xselezione1, xselezione2, yselezione1, yselezione2;
    TMyShape *PosCella;
    PosCella = (TMyShape*) ScrollBox1->FindComponent("Pos" + IntToStr(StructPlc.pos));
    if (PosCella != NULL) {
        if (StructPlc.ChiamataPrelievo) {
            if (StructPlc.ChiamataTelaioPieno) {
                PosCella->Brush->Color = clYellow; // telaio pienodmDB->ReturnColoreTipologiaDaId(TOTEM_ARELLE_FRESCO_VUOTO);
                if (dmDB->TabPostazioni[StructPlc.pos]["IDUDC"].ToIntDef(0))
                    PosCella->Text = dmDB->TabPostazioni[StructPlc.pos]["IDUDC"];
                else
                    PosCella->Text = "X";
            }
            else {
                PosCella->Brush->Color = clSilver; // telaio vuoto
                PosCella->Text = dmDB->TabPostazioni[StructPlc.pos]["IDUDC"];
            }
        }
        else if (StructPlc.ChiamataDeposito) {
            if (StructPlc.ChiamataTelaioPieno) {
                PosCella->Brush->Color = clLime; // telaio pienodmDB->ReturnColoreTipologiaDaId(TOTEM_ARELLE_FRESCO_VUOTO);
            }
            else {
                PosCella->Brush->Color = clSilver; // telaio vuoto dmDB->ReturnColoreTipologiaDaId(TOTEM_ARELLE_CONGELATO_VUOTO);
            }
            PosCella->Text = dmDB->TabPostazioni[StructPlc.pos]["IDUDC"];
        }
        else {
            PosCella->Text = dmDB->TabPostazioni[StructPlc.pos]["IDUDC"];
            if (PosCella->Text.ToIntDef(0) > 0) {
                PosCella->Brush->Style = bsSolid;
                PosCella->Brush->Color = dmDBClient->ReturnaColoreDaUDC(PosCella->Text.ToIntDef(0));
                // PosCella->Pen->Color = dmDBClient->ReturnaColoreDaUDC(PosCella->Text.ToIntDef(0));
                // PosCella->Pen->Width = 3;

            }
            else
                PosCella->Brush->Style = bsClear;
        }
        if (PosCella->Pen->Color != clGray) {
            if (StructPlc.generata) {
                PosCella->Pen->Color = clBlue;
                PosCella->Pen->Width = 3;
            }
            else if (StructPlc.esclusa) {
                PosCella->Pen->Color = clRed;
                PosCella->Pen->Width = 3;
            }
            else {
                PosCella->Pen->Color = clBlack;
                PosCella->Pen->Width = 1;
            }
        }
    }
}

void __fastcall TMainForm::Svuotacorsia1Click(TObject * Sender)
{
    //
    int corsia;
	if (Application->MessageBox(L"Sei sicuro di voler svuotare la fila ? ", L"Conferma !!!", MB_YESNO) == IDYES) {
        corsia = dmDB->ReturnFilaDaPos(PopupMenuPos->Tag);
        dmDB->SvuotaCorsia(corsia);
        // aggiornamappa = 1 ;
    }

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::DisabilitaCorsia1Click(TObject * Sender)
{
    //
    int corsia = dmDB->ReturnFilaDaPos(PopupMenuPos->Tag);
    dmDB->AbilitaCorsia(corsia, 0);
    if (SocketDataModule->SeiConnesso())
        dmDB->aggiorna_tab_posizioni_locale = true; // aggiorno anche gli altri client
    else
        AggiornaMappa(); // = 1;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::Abilitacorsia1Click(TObject * Sender) {
    //
    int corsia = dmDB->ReturnFilaDaPos(PopupMenuPos->Tag);
    dmDB->AbilitaCorsia(corsia, 1);
    if (SocketDataModule->SeiConnesso())
        dmDB->aggiorna_tab_posizioni_locale = true; // aggiorno anche gli altri client
    else
        AggiornaMappa(); // = 1;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::SpSimulazioneClick(TObject * Sender)
{
    invia_richiesta_simulazione = 1;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BitBtn4Click(TObject * Sender)
{
    // if (dmDB->pwdlevel) {
    // start
    SocketDataModule->InviaRichiestaStart(1);
    // }
    // else {
    // ShowMessage("Per far ripartire le missioni devi inserire la password");
    // }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BitBtn5Click(TObject * Sender)
{
    if (dmDB->pwdlevel) {
        // stop
        SocketDataModule->InviaRichiestaStart(0);
    }
    else {
        ShowMessage("Per fermare le missioni devi inserire la password");
    }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::svuotapos1Click(TObject * Sender)
{
    //
    TMenuItem *Item = (TMenuItem*) Sender;
    if ((Item != NULL) && (Item->Tag)) {
		if (Application->MessageBox(L"Sei sicuro di voler svuotare la posizione selezionata ? ", L"Conferma !!!", MB_YESNO) == IDYES) {
			dmDB->SvuotaPos(Item->Tag);
            // aggiornamappa = 1 ;
        }
	}

}

void __fastcall TMainForm::rimuovi_prenotazione1Click(TObject * Sender)
{
    //
    TMenuItem *Item = (TMenuItem*) Sender;
    if ((Item != NULL) && (Item->Tag)) {
        if (Application->MessageBox(L"Are you sure you want to remove the reservation for this Position ? ", L"Confirm !!!", MB_YESNO) == IDYES) {
            dmDB->PrenotaPos(Item->Tag, 0, dmDB->FilaPosizione(Item->Tag));
            // aggiornamappa = 1 ;
        }
    }

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BitBtn8Click(TObject * Sender)
{
    CreateMDIChild(frConfig, "frConfig", "Setup");

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::escludiagv1Click(TObject * Sender)
{
    //
    AnsiString s;
    TMenuItem *Item = (TMenuItem*) Sender;
    if (Item != NULL) {
        if (dmDB->pwdlevel) {
            if (Application->MessageBox(L"Are you sure to Remove Lgv from the System ? ? ? ? ? ", L"Confirm !!!", MB_YESNO) == IDYES) {
                dmDB->LogMsg("Inviato escludi agv " + IntToStr(Item->Tag) + "da client");
                SocketDataModule->EscludiAgvDaSupervisione(Item->Tag);
            }
        }
        else
            ShowMessage("Only Logged User");
    }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::creamisvuoto1Click(TObject * Sender)
{

    int posprel;
    AnsiString res;
    TMenuItem *Item = (TMenuItem*) Sender;
    AnsiString posman;
    int posint;

    if (Item != NULL) {
        posman = InputBox(L"Inserire Tipologia da prelevare ", L"Da 1 a 6 ", L"0");
        posint = posman.ToIntDef(0);
        if ((posint >= 1) && (posint <= 6)) {
            if (Item->Tag == 1)
                ////              posprel = dmDB->RicercaPrelievoUDC(posint, 0);
                res = "Pos Prelievo Trovata : " + IntToStr(posprel);
            ShowMessage(res);
        }
    }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BitBtn12Click(TObject * Sender)
{
    CreateMDIChild(fLog, "fLog", "Operations Log");
}

// ---------------------------------------------------------------------------

void __fastcall TMainForm::ArticoliUDCClick(TObject * Sender) {
    // FormAnagraficaArticoliUDC->Show();
}

void __fastcall TMainForm::CentraleUDCClick(TObject * Sender) {
    // FormCentralAnag->Show();
}

void __fastcall TMainForm::InsertUDC1Click(TObject * Sender) {
    //
    TMenuItem *Item = (TMenuItem*) Sender;
    dmGestMissioni->InsertUDC1Click(Item, agvdaspostare);

}

void __fastcall TMainForm::PrelevaUDC1Click(TObject * Sender) {

    TMenuItem *Item = (TMenuItem*) Sender;
    if (Item != NULL) {
        // dmDB->SvuotaStruturaCentroMissioni(0);
        CentroMissione.posprel = Item->Tag;
        CentroMissione.TipoMissione = 0;
        ShowMessage("Seleziona destinazione per udc pos " + IntToStr(Item->Tag) + "!!!(tasto dx del mouse)");
    }
}

void __fastcall TMainForm::DepositaUDC1Click(TObject * Sender) {
    TMenuItem *Item = (TMenuItem*) Sender;
    dmGestMissioni->DepositaUDC1Click(Item);

}

void __fastcall TMainForm::PesaUDC1Click(TObject * Sender) {

    TMenuItem *Item = (TMenuItem*) Sender;
    dmGestMissioni->PesaUDC1Click(Item);

}

void __fastcall TMainForm::bStoriciMouseUp(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    PopupMenu->Items->Clear();
    TMenuItem *NewItem1 = new TMenuItem(this); // create the separator
    NewItem1->Caption = "Storico Allarmi";
    NewItem1->Tag = 1;
    ImageListMenu->Draw(NewItem1->Bitmap->Canvas, X + bStorici->Left, Y + bStorici->Height, 1); // non va
    NewItem1->OnClick = StoricoAllarmiClick;
    PopupMenu->Items->Add(NewItem1);
    TMenuItem *NewItem2 = new TMenuItem(this); // create the separator
    NewItem2->Caption = "Allarmi Attivi";
    NewItem2->Tag = 2;
    NewItem2->OnClick = AllarmiAttiviClick;
    PopupMenu->Items->Add(NewItem2);

    PopupMenu->Popup(X + bStorici->Left, Y + bStorici->Height);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::StoricoAllarmiClick(TObject * Sender) {
    CreateMDIChild(fAllarmi, "fAllarmi", "Historic Alarms");
}

void __fastcall TMainForm::AllarmiAttiviClick(TObject * Sender) {
    FormAllarmiAttivi->Show();
}

void __fastcall TMainForm::ArticoliUDCMultipliClick(TObject * Sender) {
    // FormArtUDCDuplicati->Show();
}

void __fastcall TMainForm::MissioniClick(TObject * Sender) {
    CreateMDIChild(fMissioni, "fMissioni", "Storico Missioni");
}

void __fastcall TMainForm::CentroMissioniClick(TObject * Sender) {
    CreateMDIChild(fCentroMissioni, "fCentroMissioni", "Storico Centro Missioni");
    // CreateMDIChild(fDBExtra, "fDBExtra", "Missioni WMS");
}

void __fastcall TMainForm::bStatoAGVMouseUp(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {

    StatoAGVClick(this);
    /*
     PopupMenu->Items->Clear();
     TMenuItem *NewItem1 = new TMenuItem(this); // create the separator
     NewItem1->Caption = "Stato AGV";
     NewItem1->Tag = 1;
     ImageListMenu->Draw(NewItem1->Bitmap->Canvas, X + bStatoAGV->Left, Y + bStatoAGV->Height, 1); // non va
     NewItem1->OnClick = StatoAGVClick;
     PopupMenu->Items->Add(NewItem1);
     TMenuItem *NewItem2 = new TMenuItem(this); // create the separator
     NewItem2->Caption = "Log Batterie";
     NewItem2->Tag = 2;
     NewItem2->OnClick = StatoBatteriaClick;
     PopupMenu->Items->Add(NewItem2);
     PopupMenu->Popup(X + bStatoAGV->Left, Y + bStatoAGV->Height);
     */

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::StatoAGVClick(TObject * Sender) {
    CreateMDIChild(fAGV, "fAGV", "AGV State");
}

void __fastcall TMainForm::StatoBatteriaClick(TObject * Sender) {
    CreateMDIChild(fBatterie, "fBatterie", "Batteries Log");
}

void __fastcall TMainForm::btFastSimClick(TObject * Sender)
{
    invia_richiesta_simulazione = 1;
    dmDB->ActualUser = "PROXAUT";
    dmDB->pwdlevel = 10;
}

// ---------------------------------------------------------------------------

void __fastcall TMainForm::SecondoPianoAGVClick(TObject * Sender) {
    AnsiString s;
    TRotateImage *RotImg;
    TMenuItem *Item = (TMenuItem*) Sender;
    if (Item != NULL) {
        RotImg = (TRotateImage*)(FindComponent("img" + IntToStr(Item->Tag)));
        if (RotImg != NULL)
            RotImg->SendToBack();

    }
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::Splitter1Moved(TObject *Sender)
{

    Image1->Height = ScrollBox1->Height * 0.99f;
    Image1->Width = ScrollBox1->Width * 0.99f;

    zoom = 1;
    Xo = (Xo * Image1->Width) / FormW; // Ricalcolo l'origine in base alla dimensione della finestra
    Yo = (Yo * Image1->Height) / FormH;
    FormH = Image1->Height;
    FormW = Image1->Width;
    widthextend = Image1->Width;
    heightextend = Image1->Height;

    ExtendSpeedButtonClick(this);
    // AggiornaMappa();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::Splitter1CanResize(TObject *Sender, int &NewSize, bool &Accept)
{
    if (NewSize > 268) {
        Accept = false;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::ScrollBox1EndDrag(TObject *Sender, TObject *Target, int X, int Y) {
    Application->MessageBox(L"Siamo qui", L"Bene", MB_OK);
}
// ---------------------------------------------------------------------------

int TMainForm::ProporzionaX(int xx) {
    int ret = 0;
    ret = Xo + ((Image1->Width * xx / MainForm->W) + Image1->Left);

    return ret;
}

int TMainForm::ProporzionaY(int yy) {
    int ret = 0;
    ret = Yo - ((Image1->Height * yy / H) + Image1->Top);
    return ret;
}

void __fastcall TMainForm::btPrelievo1ClickOLD(TObject *Sender) {
    // VECCHIA GESTIONE MANUALE IN CM NON SI USA PIU!!
    /* AnsiString stringa, Nomepos;
     int ok;
     TCentroMissione CentroMis;
     TButton *Button;
     Button = (TButton*) Sender;
     if (Button != NULL) {
     CentroMis.posprel = Button->Tag;
     CentroMis.pianoprel = 1;
     if (CentroMis.posprel > 0) {
     CentroMis.h_prel = dmDB->RitornaAltezzedaPosizione(CentroMis.posprel, CentroMis.pianoprel, "HPREL");
     Nomepos = dmDB->ReturnUdcDaPosePiano(CentroMis.posprel, 1);
     dmDBImpianto->TornaPosPianoDaNomePos(Nomepos.Trim(), CentroMis.posdep, CentroMis.pianodep);
     CentroMis.h_dep = dmDB->RitornaAltezzedaPosizione(CentroMis.posdep, CentroMis.pianodep, "HDEP");
     if (CentroMis.posdep) {
     CentroMis.DestinazioneModuli = 0; // non serve
     CentroMis.CodTipoMissione = 0; //
     CentroMis.CodTipoMovimento = 0;
     CentroMis.TipoMissione = 0;
     CentroMis.CorsiaDeposito = "";
     CentroMis.IDUDC = dmDB->RitornaUDCdaCodUDC(Nomepos);
     CentroMis.TipoUDC = 1; // da gestire?
     CentroMis.Priorita = 1;
     CentroMis.Agv = 0;
     CentroMis.stato = 0;
     dmDB->GeneraCentroMissione(CentroMis);
     dmDB->LogMsg("Generata Missione di prelievo da InOut pos." + IntToStr(CentroMis.posprel) + " a pos." + IntToStr(CentroMis.posdep) + " (" + Nomepos + ")");
     // dmDB->AggiornaTabAnagrafica();
     }
     }
     } */
}

// ---------------------------------------------------------------------------

void __fastcall TMainForm::btPrelievo1Click(TObject *Sender) {
    // creao missione manuale. Chiedo il codice basandomi sull'udc che trovo, dato l'ìnput lo scrivo in posizione poi genero la missione con quello
    /* TButton *Button;
     Button = (TButton*) Sender;
     int posprel, pianoprel = 1, idudc, posdep, pianodep;
     UnicodeString Nomepos;
     if (Button != NULL) {
     // se non lo trovo lo domando
     posprel = Button->Tag;
     // leggo udc in posprelievo a terra
     idudc = dmDB->RitornaUDCdaPosPiano(posprel, pianoprel);
     Nomepos = dmDB->ReturnUdcDaPosePiano(posprel, pianoprel);
     InputQuery("Destinazione per esteso", "Inserire posizione destinazione", Nomepos);
     Nomepos = Nomepos.Trim();
     dmDBImpianto->TornaPosPianoDaNomePos(Nomepos, posdep, pianodep);
     // se la nuova posizione e' giusta
     if (posdep > 0) {
     // creo o ottengo idudc
     idudc = 0; // dmDBImpianto->UDCdaBarcode(Nomepos);
     dmDB->ArticoloPrelevatoDepositato(posprel, idudc, pianoprel);
     SocketDataModule->MissionePrelievoManuale(posprel);
     }
     else {
     ShowMessage("Prelievo manuale non effettuato, posizione non corretta");
     }

     } */
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::TimerSinotticoTimer(TObject * Sender) {
    // visualizzo eventi sinottico
    int i;
    int indice;
    static int UDCbarcodedastampare = 0;
    AnsiString countdown;
    AnsiString TempC;
    TLabel *LabelAGV;
    TEdit *EditPiste;
    TButton *Tasto;
    TCheckBox *CBox;

    if (!dmDB->lettostatoagv)
        return;

    TimerSinottico->Enabled = false;

    if ((ClientData.ParametriFunzionali.Simula) || (DebugHook)) {
        if (ClientData.ParametriFunzionali.Simula)
            SpSimulazione->Caption = "RES ";
        else
            SpSimulazione->Caption = "SIM ";
        SpSimulazione->Visible = true;
    }
    else
        SpSimulazione->Visible = false;
    if (invia_richiesta_simulazione) {
        SocketDataModule->InviaRichiestaSimulazione(!ClientData.ParametriFunzionali.Simula);
        invia_richiesta_simulazione = 0;
    }

    // gestione allarmi AGV a video
    for (i = 1; i <= NAGV; i++) {
        LabelAGV = (TLabel*) this->FindComponent("lbAGV" + IntToStr(i) + "Status");

        if (LabelAGV != NULL) {
            if (ClientData.DatiAgv[i].alarm) {
                // lbAGV1Status->Transparent = false;
                LabelAGV->Color = clRed;
                LabelAGV->Font->Color = clWhite;
                LabelAGV->Caption = "AGV " + String(i) + " Allarme " + String(StrToIntDef("0x" + String(ClientData.DatiAgv[i].allarme), 0)) + ", pos " + IntToStr(ClientData.DatiAgv[i].pos);
            }
            else {
                if (ClientData.DatiAgv[i].idmis) {
                    LabelAGV->Color = clLime;
                    LabelAGV->Font->Color = clBlack;
                    // !!! DA RIGUARDARE QUESTA COSA
                    LabelAGV->Caption = "Miss. " + String(ClientData.DatiAgv[i].idmis) + " dest " + IntToStr(ClientData.DatiAgv[i].dest);
                    // LabelAGV->Caption = "AGV " + String(i) + ", Miss. " + String(ClientData.DatiAgv[i].idmis) + " dest " + dmDB->NomePos(ClientData.DatiAgv[i].dest);
                }
                else if (ClientData.DatiAgv[i].incarica) {
                    LabelAGV->Color = clYellow;
                    LabelAGV->Font->Color = clBlack;
                    LabelAGV->Caption = "AGV " + String(i) + " in carica pos " + IntToStr(ClientData.DatiAgv[i].pos);
                }
                else if (ClientData.DatiAgv[i].sospeso) {
                    LabelAGV->Color = clTeal;
                    LabelAGV->Font->Color = clWhite;
                    LabelAGV->Caption = "AGV " + String(i) + " in stato sospeso";
                }
                else {
                    LabelAGV->Color = clAqua;
                    LabelAGV->Font->Color = clBlack;

                    LabelAGV->Caption = "AGV " + String(i) + ", pos " + IntToStr(ClientData.DatiAgv[i].pos);

                }
            }
        }
    }
    // gestione stato piste
    /* for (i = 1; i <= NUM_SENSORI; i++) {
     EditPiste = (TEdit*) this->FindComponent("edP" + IntToStr(i));
     if (EditPiste != NULL) {
     EditPiste->Color = (ClientData.Sensori[i].Stato ? clLime : clRed);
     }
     }

     // Gestion ModBus
     for (i = 1; i <= 8; i++) {
     CBox = (TCheckBox*) this->FindComponent("cbPalletIN" + IntToStr(i));
     if (CBox != NULL) {
     indice = dmDBImpianto->TornaPostazioneIDDaIndiceTipo(i, 1);
     if ((indice > 0) && (indice <= NUM_POSTAZIONI)) {
     CBox->Checked = ClientData.Zona[1].Postazione[indice].presenzaUDC;
     }
     }
     }

     for (i = 1; i <= 8; i++) {
     CBox = (TCheckBox*) this->FindComponent("cbPalletINOUT" + IntToStr(i));
     indice = dmDBImpianto->TornaPostazioneIDDaIndiceTipo(i, 2);
     if ((indice > 0) && (indice <= NUM_POSTAZIONI)) {
     CBox->Checked = ClientData.Zona[1].Postazione[indice].presenzaUDC;
     }
     } */
    // w1+lamp
    /* for (i = 1; i <= 6; i++) {
     CBox = (TCheckBox*) this->FindComponent("cbAllarmiW1" + IntToStr(i));
     CBox->Checked = dmExtraFunction->bitRead(ClientData.ModBusIn.AllarmiW1, i - 1);
     }
     cbLamp->Checked = ClientData.ModBusIn.LampadaAllarme; */

    // pulsanti
    /* cbPulsanti1->Checked = ClientData.Zona[1].Postazione[dmDBImpianto->TornaPostazioneIDDaPos(11)].Pulsante;
     cbPulsanti2->Checked = ClientData.Zona[1].Postazione[dmDBImpianto->TornaPostazioneIDDaPos(12)].Pulsante;
     cbPulsanti3->Checked = ClientData.Zona[1].Postazione[dmDBImpianto->TornaPostazioneIDDaPos(1)].Pulsante;
     cbPulsanti4->Checked = ClientData.Zona[1].Postazione[dmDBImpianto->TornaPostazioneIDDaPos(2)].Pulsante;
     cbPulsanti5->Checked = ClientData.Zona[1].Postazione[dmDBImpianto->TornaPostazioneIDDaPos(3)].Pulsante;

     // gestione tasti manuali prelievo
     for (i = 1; i <= 3; i++) {
     Tasto = (TButton*) this->FindComponent("btPrelievo" + IntToStr(i));
     indice = dmDBImpianto->TornaPostazioneIDDaPos(i);
     if ((indice > 0) && (indice <= NUM_POSTAZIONI)) {
     Tasto->Enabled = ClientData.Zona[1].Postazione[indice].presenzaUDC;
     }
     } */

    // info missione manuale
    edPosPrel->Text = CentroMissione.posprel;
    edPianoPrel->Text = CentroMissione.pianoprel;
    edPosDep->Text = CentroMissione.posdep;
    edPianoDep->Text = CentroMissione.pianodep;

    // gbMissioniManuali->Visible = ClientData.ParametriFunzionali.Simula & dmDB->pwdlevel;

    TimerSinottico->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::TimerInfoTimer(TObject * Sender) {
    AnsiString TempString = "";
    static int contatore = 0;
    TCheckBox *cb;

    if (MainForm->count_inizio <= 10)
        return;

    TimerInfo->Enabled = false;
    // aggiorno ogni 10 secondi
    if (contatore == 0) {
        // centromissioni
        try {
            dmDB->QueryPos->Close();
            dmDB->QueryPos->SQL->Clear();
            dmDB->QueryPos->SQL->Append("Select Top 1 id, posprel,posdep,generata from centromissioni where stato=0 order by generata");
            dmDB->QueryPos->Open();
            if (dmDB->QueryPos->RecordCount > 0) {
                dmDB->QueryPos->First();
                edCentroMis->Text = "ID:" + dmDB->QueryPos->FieldByName("id")->AsString + "  Prel: " + dmDB->NomePos(dmDB->QueryPos->FieldByName("posprel")->AsInteger) + "  Dep: " +
                    dmDB->NomePos(dmDB->QueryPos->FieldByName("posdep")->AsInteger);
                lbTimeToCentroMission->Caption = dmDB->QueryPos->FieldByName("generata")->AsString;
            }
            else {
                edCentroMis->Text = "";
                lbTimeToCentroMission->Caption = "  -  -  -";
            }
        }
        catch (...) {
            edCentroMis->Text = "E R R O R";
            lbTimeToCentroMission->Caption = "  -  -  -";
        }
        // missioni
        try {
            dmDB->QueryPos->Close();
            dmDB->QueryPos->SQL->Clear();
            dmDB->QueryPos->SQL->Append("Select Top 1 id, agv, posprel,posdep,generata from missioni order by generata DESC");
            dmDB->QueryPos->Open();
            if (dmDB->QueryPos->RecordCount > 0) {
                dmDB->QueryPos->First();
                // edMisC->Text = "ID:" + dmDB->QueryPos->FieldByName("id")->AsString + " AGV " + dmDB->QueryPos->FieldByName("agv")->AsString + ",  Prel: " + dmDB->NomePos(dmDB->QueryPos->FieldByName("posprel")->AsInteger) + "  Dep: " +

                edMisC->Text = "ID:" + dmDB->QueryPos->FieldByName("id")->AsString + "  Prel: " + dmDB->NomePos(dmDB->QueryPos->FieldByName("posprel")->AsInteger) + "  Dep: " +
                    dmDB->NomePos(dmDB->QueryPos->FieldByName("posdep")->AsInteger);
                lbTimeToMission->Caption = dmDB->QueryPos->FieldByName("generata")->AsString;
            }
            dmDB->QueryPos->Close();

            if ((CentroMissione.posprel > 0) && (CentroMissione.posdep > 0)) {
                btEseguiMissioneManuale->Enabled = true;
            }
            else {
                btEseguiMissioneManuale->Enabled = false;
            }
        }
        catch (...) {
            edMisC->Text = "E R R O R";
            lbTimeToMission->Caption = "  -  -  -";
        }
    }
    RiepilogoSegnali();

    contatore = ++contatore % 10;
    TimerInfo->Enabled = true;
}

void TMainForm::RiepilogoSegnali() {
    int idplc;
    // PRELIEVO
    idplc = ClientData.Plc[1].Prelievo[1][1].idplc;
    // IN
    cbokPrel1->Checked = ClientData.Plc[idplc].Prelievo[1][1].Ready;
    cbAllarmePrel1->Checked = ClientData.Plc[idplc].Prelievo[1][1].InAllarme;
    cbChiamaPrel1->Checked = ClientData.Plc[idplc].Prelievo[1][1].ProntaAlPrelievo;
    cbOkIngressoPrel1->Checked = ClientData.Plc[idplc].Prelievo[1][1].OkPrelievo;
    // out
    cbAGVinIngombroPrel1->Checked = ClientData.Plc[idplc].Prelievo[1][1].AGVInIngombro;
    cbRichiestaingPrel1->Checked = ClientData.Plc[idplc].Prelievo[1][1].RichiestaAbilitaPrel;

    // DEPOSITO  1
    idplc = ClientData.Plc[1].Deposito[1][1].idplc;
    // IN
    cbokDep1->Checked = ClientData.Plc[idplc].Deposito[1][1].Ready;
    cbAllarmeDep1->Checked = ClientData.Plc[idplc].Deposito[1][1].InAllarme;
    cbPermessoDep1->Checked = ClientData.Plc[idplc].Deposito[1][1].ProntaAlDeposito;
    cbOkIngressoDep1->Checked = ClientData.Plc[idplc].Deposito[1][1].OkDeposito;
    // out
    cbAGVinIngombroDep1->Checked = ClientData.Plc[idplc].Deposito[1][1].AGVInIngombro;
    cbRichiestaingDep1->Checked = ClientData.Plc[idplc].Deposito[1][1].RichiestaAbilitaDep;

    // DEPOSITO  2
    idplc = ClientData.Plc[1].Deposito[2][1].idplc;
    // IN
    cbokDep2->Checked = ClientData.Plc[idplc].Deposito[2][1].Ready;
    cbAllarmeDep2->Checked = ClientData.Plc[idplc].Deposito[2][1].InAllarme;
    cbPermessoDep2->Checked = ClientData.Plc[idplc].Deposito[2][1].ProntaAlDeposito;
    cbOkIngressoDep2->Checked = ClientData.Plc[idplc].Deposito[2][1].OkDeposito;
    // out
    cbAGVinIngombroDep2->Checked = ClientData.Plc[idplc].Deposito[2][1].AGVInIngombro;
    cbRichiestaingDep2->Checked = ClientData.Plc[idplc].Deposito[2][1].RichiestaAbilitaDep;

}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::btPulisciClick(TObject * Sender)
{
    CentroMissione.posprel = 0;
    CentroMissione.pianoprel = 0;
    CentroMissione.posdep = 0;
    CentroMissione.pianodep = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::btEseguiMissioneManualeClick(TObject * Sender)
{
    TMenuItem *NewItemx = new TMenuItem(this);
    NewItemx->Tag = MainForm->pos_udc;
    dmGestMissioni->MisManSpostPLCClick(NewItemx);
    delete NewItemx;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::btAnagraficheMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{

    PopupMenu->Items->Clear();
    TMenuItem *NewItem1 = new TMenuItem(this); // create the separator
    NewItem1->Caption = "Gestione UDC";
    NewItem1->Tag = 1;
    ImageListMenu->Draw(NewItem1->Bitmap->Canvas, X + bStorici->Left, Y + btAnagrafiche->Height, 1); // non va
    NewItem1->OnClick = UDCClick;
    PopupMenu->Items->Add(NewItem1);

    TMenuItem *NewItem2 = new TMenuItem(this); // create the separator
    NewItem2->Caption = "Gestione Utenti";
    NewItem2->Tag = 2;
    NewItem2->OnClick = GestioneUtentiClick;
    PopupMenu->Items->Add(NewItem2);

    PopupMenu->Popup(btAnagrafiche->Left + btAnagrafiche->Width*0.5f, btAnagrafiche->Top + btAnagrafiche->Height*1.3f);

}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::GestioneUtentiClick(TObject * Sender) {
    if (dmDB->pwdlevel == 9) {
        FrmGestUser->ShowModal();
    }
    else
        ShowMessage("Operazione permessa solo a utenti di Livello 9");
}

void __fastcall TMainForm::UDCClick(TObject * Sender) {
    FormUDCList->blocca_insert = true;
    FormUDCList->Show();
}

void __fastcall TMainForm::sbMissioniManualiClick(TObject *Sender)
{
    CreateMDIChild(fGestAGVMidi, "fGestAGVMidi", "Prelievo / Destinazione AGV");
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::btGiornoClick(TObject *Sender)
{
    SocketDataModule->InviaRichiestaGiorno(1);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::btNotteClick(TObject *Sender)
{
    SocketDataModule->InviaRichiestaGiorno(0);
}
// ---------------------------------------------------------------------------
