// ---------------------------------------------------------------------------

#ifndef MainH
#define MainH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.AppEvnts.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdExplicitTLSClientServerBase.hpp>
#include <IdFTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <Vcl.Imaging.GIFImg.hpp>
#include "MessageBar.h"
#include <Vcl.Imaging.jpeg.hpp>
#include "ClientData.h"
#include "MessageBar.h"
#include "threadmodbusclient.h"
#include <Vcl.Imaging.jpeg.hpp>

// #include <System.ImageList.hpp>
// ---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published: // IDE-managed Components

    TPopupMenu *PopupMenu1;
    TMenuItem *Exit1;
    TMenuItem *Apri1;
    TPanel *Panel1;
    TStaticText *StaticText1;
    TGroupBox *GroupBox1;
    TShape *Shape1;
    TGroupBox *GroupBox2;
    TMemo *Memo1;
    TPanel *Panel2;
    TBitBtn *BitBtn1;
    TImageList *ImageList1;
    TShape *Shape0;
    TLabel *Label7;
    TTimer *Timer1;
    TApplicationEvents *ApplicationEvents1;
    TShape *Shape2;
    TLabel *Label2;
    TBitBtn *BitBtn2;
    TMessageBar *MessageBar1;
    TTrayIcon *TrayIcon1;
    TLabel *Label6;
    TLabel *T1;
    TLabel *T2;
    TLabel *WPlc1;
    TLabel *WPc1;
    TLabel *WPlc2;
    TLabel *WPc2;
    TBitBtn *BitBtn3;
    TLabel *Label4;
    TLabel *Label8;
    TPanel *Panel3;
    TBitBtn *btStart;
    TBitBtn *btStop;
    TImage *Image5;
    TPanel *Panel5;
    TImage *Image1;
    TShape *Shape5;
    TLabel *Label1;

    void __fastcall Exit1Click(TObject *Sender);
    void __fastcall Apri1Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall ApplicationEvents1Minimize(TObject *Sender);
    void __fastcall ApplicationEvents1Restore(TObject *Sender);
    void __fastcall TrayIcon1DblClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall BitBtn3Click(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall btStartClick(TObject *Sender);
    void __fastcall btStopClick(TObject *Sender);
	void __fastcall BitBtn7Click(TObject *Sender);
	void __fastcall BitBtn6Click(TObject *Sender);
	void __fastcall BitBtn4Click(TObject *Sender);
	void __fastcall BitBtn5Click(TObject *Sender);

private: // User declarations
        public : // User declarations
    __fastcall TMainForm(TComponent* Owner);
    void AccendiLed(int n, TColor c);
    void VisTCiclo(int n, AnsiString val);
    void VisWatchDog(int n, AnsiString valplc, AnsiString valpc);
    void MemoLog(AnsiString Evento);
    void LogStatoAgv();
    void LeggiParametri();

    // ModuloPhoenix DatiPhoenix[2] ;  messo nel clientdata.h
    // PlcVemacArelle PlcVemacArelleIngresso , PlcVemacArelleUscita ;  messo in clientdata
    unsigned char bit[8], bitAnd[8];
    int count_inizio, id_plc_thread_arelle;
    int ridisegna_mappa;
    int aggiorna_allarmi;
    int invia_richiesta_simulazione;
    int contatore;
    bool Richiesta_chiusura;
    bool Avvio, close_program;

    AnsiString strcons[NAGV + 1], Cons_BatOld[NAGV + 1], Cons_Bat[NAGV + 1];

    MODBUS Register_MODBUS;
};

// ---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
// ---------------------------------------------------------------------------
#endif
