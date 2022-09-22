// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "segnaliBaia.h"
// #include "dmphoenix.h"
#include "db.h"
#include "clientdata.h"
#include "main.h"
#include "SocketComunicazioneClient.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChildWin"
#pragma resource "*.dfm"
TfSegnaliBaia *fSegnaliBaia;
extern char N_carrelli;

// ---------------------------------------------------------------------------
__fastcall TfSegnaliBaia::TfSegnaliBaia(TComponent* Owner) : TMDIChild(Owner) {
    int i, j;

    for (i = 1; i <= 4; i++) {
        for (j = 0; j <= 7; j++) {
            InputName[i][j] = "Input " + IntToStr(i) + " - Bit " + IntToStr(j) + " ";
            OutputName[i][j] = "Output " + IntToStr(i) + " - Bit " + IntToStr(j) + " ";
        }
    }
    // 1
    // InputName[1][0] = "";
    // InputName[1][1] = "";
    // InputName[1][2] = "";
    // InputName[1][3] = "";
    // InputName[1][4] = "";
    // InputName[1][5] = "";
    InputName[1][6] = "Zona A - Porta 2 aperta";
    InputName[1][7] = "Zona A - Porta 2 chiusa";
    // InputName[2][0] = "";
    // InputName[2][1] = "";
    // InputName[2][2] = "";
    // InputName[2][3] = "";
    // InputName[2][4] = "";
    // InputName[2][5] = "";
    // InputName[2][6] = "";
    // InputName[2][7] = "";
    // InputName[3][0] = "";
    // InputName[3][1] = "";
    // InputName[3][2] = "";
    // InputName[3][3] = "";
    // InputName[3][4] = "";
    // InputName[3][5] = "";
    InputName[3][6] = "Zona B Porta 1 aperta ";
    InputName[3][7] = "Zona B Porta 1 chiusa ";
    InputName[4][0] = "Zona B Sbarra 1 (int) aperta ";
    InputName[4][1] = "Zona B Sbarra 2 (est) aperta ";
    InputName[4][2] = "Zona B Sbarra 1 (int) chiusa ";
    InputName[4][3] = "Zona B Sbarra 2 (est) chiusa ";
    InputName[4][4] = "Generatore Frequenza 1 ";
    InputName[4][5] = "Generatore Frequenza 2 ";
    InputName[4][6] = "Generatore Frequenza 3 ";
    InputName[4][7] = "Generatore Frequenza 4 ";
    // 2
    // InputName[2][4] = "Zona B Pos.1 - Chiama vuoto ";
    // InputName[2][5] = "Zona B Pos.1 - Chiama pieno ";
    // InputName[2][6] = "Zona B Pos.1 - Pronto vuoto";
    // InputName[2][7] = "Sbarra 1 ";
    // InputName[3][0] = "Sbarra 2 ";
    // InputName[3][1] = "non assegnato ";

    // OutputName[1][0] = " ";
    // OutputName[1][1] = " ";
    // OutputName[1][2] = " ";
    // OutputName[1][3] = " ";
    // OutputName[1][4] = " ";
    // OutputName[1][5] = " ";
    // OutputName[1][6] = " ";
    // OutputName[1][7] = " ";
    // OutputName[2][0] = " ";
    OutputName[2][1] = "Zona A Apri porta 2 ";
    OutputName[2][2] = "Zona A Chiudi porta 2 ";
    // OutputName[2][3] = " ";
    // OutputName[2][4] = " ";
    // OutputName[2][5] = " ";
    // OutputName[2][6] = " ";
    // OutputName[2][7] = " ";
    // OutputName[3][0] = " ";
    // OutputName[3][1] = " ";
    // OutputName[3][2] = " ";
    // OutputName[3][3] = " ";
    // OutputName[3][0] = " ";
    OutputName[4][1] = "Zona B Apri porta 1 ";
    OutputName[4][2] = "Zona B Chiudi porta 1 ";
    // OutputName[4][3] = " ";
    OutputName[4][4] = "Zona B Chiudi Sbarra 1 (int) ";
    OutputName[4][5] = "Zona B Chiudi Sbarra 2 (est) ";
    // OutputName[4][6] = " ";
    // OutputName[4][7] = " ";
}

void __fastcall TfSegnaliBaia::FormActivate(TObject * Sender) {
    int i, j, indice;
    activate = 1;
    forzadati = 0;
    TPanel *Pan;
    baia = 0; // MainForm->baiaformSegnali ;
    // TabControl1->TabIndex = baia - 1 ;
    for (i = 1; i <= 4; i++) {
        if (TabControl1->TabIndex == 0) {
            switch (i) {
            case 1:
                indice = 1;
                break;
            case 2:
                indice = 5;
                break;
            case 3:
                indice = 4;
                break;
            case 4:
                indice = 3;
                break;
            }
        }
        else if (TabControl1->TabIndex == 1) {
            switch (i) {
            case 1:
                indice = 2;
                break;
            case 2:
                indice = 9;
                break;
            case 3:
                indice = 8;
                break;
            case 4:
                indice = 7;
                break;
            }
        }
        for (j = 0; j <= 7; j++) {
            Pan = (TPanel*) FindComponent("pName" + IntToStr(i) + "_" + IntToStr(j));
            if (Pan != Null) {
                Pan->Caption = InputName[i][j];
                if (Pan->Caption.SubString(1, 5) == "Input")
                    Pan->Font->Color = clScrollBar;
                else
                    Pan->Font->Color = clWindowText;
            }
            Pan = (TPanel*) FindComponent("pNameOutput" + IntToStr(i) + "_" + IntToStr(j));
            if (Pan != Null) {
                Pan->Caption = OutputName[i][j];
                if (Pan->Caption.SubString(1, 6) == "Output")
                    Pan->Font->Color = clScrollBar;
                else
                    Pan->Font->Color = clWindowText;
            }
        }
    }
    Timer1Timer(this);
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
void TfSegnaliBaia::ColoraShape(int stato, TShape *t, TColor ColorON, TColor ColorOFF) {
    if (stato) {
        t->Brush->Color = ColorON;
    }
    else {
        t->Brush->Color = ColorOFF;
    }
}
// ---------------------------------------------------------------------------

void TfSegnaliBaia::ComandaPresenza(int modulo, int nled, int stato, bool input) {
    TShape *Led;
    bool led_trovato;
    led_trovato = false;
    if (input) {
        Led = (TShape*) FindComponent("sInputLed" + IntToStr(modulo) + "_" + IntToStr(nled));
        if (Led != NULL)
            led_trovato = true;
    }
    else {
        Led = (TShape*) FindComponent("sOutputLed" + IntToStr(modulo) + "_" + IntToStr(nled));
        if (Led != NULL)
            led_trovato = true;
    }

    if (led_trovato)
        ColoraShape(stato, Led);
}

void __fastcall TfSegnaliBaia::sOutputLed1_0MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    TShape *Led;
    int comando = 0;
    // forzo uscita su plc phoenix
    if (dmDB->pwdlevel) {
        // posso forzare i segnali
        // ClientData.DatiPhoenix[baia].forzatura_attiva = 1 ; //mi segno che sto forzando segnali
        Led = (TShape*) Sender;
        if (Led != NULL) {
            if (Led->Brush->Color == clGray)
                comando = 1;
            // DataModulePhoenix->ScriviPhoenix(Led->Tag,StrToInt(Led->Hint),comando,baia) ;
            // PLCPhoenixThread[baia]->ScriviPhoenix(Led->Tag,StrToInt(Led->Hint),comando) ;
            SocketDataModule->ScriviPhoenixClient(0, StrToInt(Led->Hint), Led->Tag, comando);
            dmDB->LogMsg("Forzato Segnale su Modulo Phoenix , Nome Segnale : " + Led->Name + " , valore : " + IntToStr(comando));
        }
    }
}
// ---------------------------------------------------------------------------

void __fastcall TfSegnaliBaia::Timer1Timer(TObject *Sender) {
    int i, k, j = 0;
    Timer1->Enabled = false;
    activate = 1;

    // in
    ComandaPresenza(1, 6, ClientData.DatiPorte[2].PortaAperta, true);
    ComandaPresenza(1, 7, ClientData.DatiPorte[2].PortaChiusa, true);
    ComandaPresenza(3, 6, ClientData.DatiPorte[1].PortaAperta, true);
    ComandaPresenza(3, 7, ClientData.DatiPorte[1].PortaChiusa, true);

    ComandaPresenza(4, 0, ClientData.DatiPorte[3].PortaAperta, true);
    ComandaPresenza(4, 1, ClientData.DatiPorte[4].PortaAperta, true);
    ComandaPresenza(4, 2, ClientData.DatiPorte[3].PortaChiusa, true);
    ComandaPresenza(4, 3, ClientData.DatiPorte[4].PortaChiusa, true);

    ComandaPresenza(4, 4, ClientData.Sensori[1].Stato, true);
    ComandaPresenza(4, 5, ClientData.Sensori[2].Stato, true);
    ComandaPresenza(4, 6, ClientData.Sensori[3].Stato, true);
    ComandaPresenza(4, 7, ClientData.Sensori[4].Stato, true);

    // out
    ComandaPresenza(2, 0, ClientData.DatiPorte[2].ApriPorta, false);
    ComandaPresenza(2, 1, ClientData.DatiPorte[2].ChiudiPorta, false);
    ComandaPresenza(4, 0, ClientData.DatiPorte[1].ApriPorta, false);
    ComandaPresenza(4, 1, ClientData.DatiPorte[1].ChiudiPorta, false);
    ComandaPresenza(4, 5, ClientData.DatiPorte[3].ChiudiPorta, false);
    ComandaPresenza(1, 6, ClientData.DatiPorte[4].ChiudiPorta, false);


    activate = 0;
    Timer1->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TfSegnaliBaia::sInputLed1_0MouseUp(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    TShape *Led;
    int comando = 0;
    // forzo uscita su plc phoenix
    if ((dmDB->pwdlevel) || (ClientData.ParametriFunzionali.Simula)) {
        // posso forzare i segnali
        Led = (TShape*) Sender;
        if (Led != NULL) {
            if (Led->Brush->Color == clGray)
                comando = 1;
            if (comando)
                ClientData.DatiPorte[StrToInt(Led->Hint)].DBInput |= dmDB->bit[Led->Tag];
            else
                ClientData.DatiPorte[StrToInt(Led->Hint)].DBInput &= dmDB->bitAnd[Led->Tag];
            SocketDataModule->SimulaInputPhoenix(0, StrToInt(Led->Hint), Led->Tag, comando);

        }
    }

}
// ---------------------------------------------------------------------------

void __fastcall TfSegnaliBaia::TabControl1Change(TObject * Sender) {
    MainForm->baiaformSegnali = TabControl1->TabIndex + 1;
    FormActivate(this);
}
// ---------------------------------------------------------------------------

void __fastcall TfSegnaliBaia::TimerTestPlcTimer(TObject * Sender) {
    int i, k, j, add_uscita_alta, bit_uscita_alta, add;
    AnsiString str_messaggio_display;
    // modifico uscite su plc ,accendo una alla volta tutte le uscite in ordine a tempo
    add_uscita_alta = 0;
    bit_uscita_alta = 8;
    TimerTestPlc->Enabled = false;

    // scorro tutte le uscite e salvo la prima alta
    for (k = 1; k < 3; k++) {
        for (j = 0; j < 8; j++) {
            if (ClientData.DatiPorte[k].PlcConnesso) {
                if (ClientData.DatiPorte[k].DBOutput & dmDB->bit[j]) {
                    add_uscita_alta = k;
                    bit_uscita_alta = j;
                    break;
                }
            }
        }
    }
    // se non ho uscite alte parto dalla prima
    if ((!add_uscita_alta) && (bit_uscita_alta == 8)) {
        add_uscita_alta = 1;
        bit_uscita_alta = 0;
    }
    else {
        // passo all'uscita successiva
        if ((bit_uscita_alta == 7) || ((bit_uscita_alta == 3) && (add_uscita_alta == 1))) {
            if (add_uscita_alta == 2)
                add_uscita_alta = 1;
            else
                add_uscita_alta = 2;
            bit_uscita_alta = 0;
        }
        else
            bit_uscita_alta++;
    }
    if (add_uscita_alta) {
        for (k = 1; k < 3; k++) {
            for (j = 0; j < 8; j++) {
                if (ClientData.DatiPorte[k].PlcConnesso) {
                    if (ClientData.DatiPorte[k].DBOutput & dmDB->bit[j]) {
                        if ((add_uscita_alta != k) || (bit_uscita_alta != j)) {
                            if (k == 2)
                                add = 3;
                            else
                                add = k;
                            // DataModulePhoenix->ScriviPhoenix(j,add,false,i) ;
                            // PLCPhoenixThread[i]->ScriviPhoenix(j,add,false) ;
                            SocketDataModule->ScriviPhoenixClient(i, j, add, false);
                        }
                    }
                    else if ((!(ClientData.DatiPorte[k].DBOutput & dmDB->bit[j])) && (add_uscita_alta == k) && (bit_uscita_alta == j)) {
                        //
                        if (k == 2)
                            add = 3;
                        else
                            add = k;
                        // DataModulePhoenix->ScriviPhoenix(j,add,true,i) ;
                        // PLCPhoenixThread[i]->ScriviPhoenix(j,add,true) ;
                        SocketDataModule->ScriviPhoenixClient(i, j, add, true);
                    }
                }
            }
        }
    }

    // modifico i registri del display
    TimerTestPlc->Enabled = true;

}

// ---------------------------------------------------------------------------
