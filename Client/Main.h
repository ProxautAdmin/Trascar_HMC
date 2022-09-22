// ----------------------------------------------------------------------------
#ifndef MainH
#define MainH
// ----------------------------------------------------------------------------
#include <System.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Messages.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <StdCtrls.hpp>
#include <Menus.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <SysUtils.hpp>
#include <Windows.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <StdActns.hpp>
#include <ToolWin.hpp>
#include <Vcl.Imaging.GIFImg.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include "ChildWin.h"
#include "MessageBar.h"
#include <time.h>
#include <Vcl.Imaging.jpeg.hpp>
#include "RotImg.hpp"
#include "MyShape.h"
#include "MyConnector.h"
#include "clientdata.h"
#include "frame_info.h"
#include <Vcl.Touch.GestureMgr.hpp>
#include <Vcl.Grids.hpp>
// #include <System.ImageList.hpp>
#define NativeScreenHeight    800    //aggiornamento automatico definizione layout pagine

// ----------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:
	TTimer *TimerPwd;
	TTimer *TimerOra;
	TImageList *ImageList1;
	TImageList *ImageList2;
	TPanel *Panel2;
	TBitBtn *bStatoAGV;
	TBitBtn *bCMissioni;
	TBitBtn *bStorici;
	TBitBtn *bPwd;
	TBitBtn *bEsci;
	TMessageBar *MessageBar1;
	TImage *Image2;
	TScrollBox *ScrollBox1;
	TImage *Image1;
	TPopupMenu *PopupMenuPos;
	TRotateImage *img1;
	TPopupMenu *PopupMenu1;
	TPanel *Panel4;
	TStatusBar *MainStatusBar;
	TPanel *Panel3;
	TSpeedButton *ZoomOutSpeedButton;
	TSpeedButton *ExtendSpeedButton;
	TSpeedButton *ZoomInSpeedButton;
	TPanel *QuotaY;
	TPanel *QuotaX;
	TTimer *TimerSinottico;
	TBitBtn *sSinottico;
	TBitBtn *BitBtn4;
	TBitBtn *BitBtn5;
	TBitBtn *BitBtn8;
	TPanel *pPassword;
	TSpeedButton *SpSimulazione;
	TBitBtn *BitBtn12;
	TImageList *ImageListMenu;
	TPopupMenu *PopupMenu;
	TBitBtn *bMissioni;
	TTimer *TimerInfo;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TBitBtn *btFastSim;
	TPanel *PanSemaforo1;
	TShape *sRichiesta1;
	TShape *sVerde1;
	TShape *sRosso1;
	TPanel *Panel5;
	TEdit *edMisC;
	TLabel *lbTimeToMission;
	TLabel *Label3;
	TLabel *lbTimeToCentroMission;
	TEdit *edCentroMis;
	TLabel *Label5;
	TLabel *lbAGV1Status;
	TSplitter *Splitter1;
	TGestureManager *GestureManager1;
	TShape *sGiallo1;
	TPanel *PanSemaforo2;
	TShape *sRichiesta2;
	TShape *sVerde2;
	TShape *sRosso2;
	TShape *sGiallo2;
	TGroupBox *gbMissioniManuali;
	TLabel *Label1;
	TLabel *Label2;
	TEdit *edPosPrel;
	TEdit *edPosDep;
	TEdit *edPianoPrel;
	TEdit *edPianoDep;
	TButton *btPulisci;
	TButton *btEseguiMissioneManuale;
    TLabel *Label4;
    TBitBtn *btAnagrafiche;
    TSpeedButton *sbMissioniManuali;
    TPanel *pPLC;
    TLabel *Label6;
    TLabel *Label8;
    TLabel *Label9;
    TGroupBox *gbMatter;
    TCheckBox *cbokDep1;
    TCheckBox *cbAllarmeDep1;
    TCheckBox *cbOkIngressoDep1;
    TGroupBox *gbPLCOut;
    TCheckBox *cbRichiestaingDep1;
    TCheckBox *cbAGVinIngombroDep1;
    TCheckBox *cbAGVRulliinMotoDep1;
    TGroupBox *GroupBox4;
    TCheckBox *cbokDep2;
    TCheckBox *cbAllarmeDep2;
    TCheckBox *cbOkIngressoDep2;
    TGroupBox *GroupBox5;
    TCheckBox *cbRichiestaingDep2;
    TCheckBox *cbAGVinIngombroDep2;
    TCheckBox *cbAGVRulliinMotoDep2;
    TGroupBox *GroupBox6;
    TCheckBox *cbokPrel1;
    TCheckBox *cbAllarmePrel1;
    TCheckBox *cbOkIngressoPrel1;
    TGroupBox *GroupBox7;
    TCheckBox *cbAGVinIngombroPrel1;
    TCheckBox *cbRichiestaingPrel1;
    TCheckBox *cbAGVRulliinMotoPrel1;
    TCheckBox *cbChiamaPrel1;
    TCheckBox *cbPermessoDep1;
    TGroupBox *GroupBox3;
    TBitBtn *btGiorno;
    TBitBtn *btNotte;
    TCheckBox *cbPermessoDep2;

	void __fastcall bEsciClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall TimerOraTimer(TObject *Sender);
	void __fastcall TimerPwdTimer(TObject *Sender);
	void __fastcall bPwdClick(TObject *Sender);
	void __fastcall MainStatusBarDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall Image4Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall sSinotticoClick(TObject *Sender);
	void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall ShapePosMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall ShapePosMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ShapePortaMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ExcludePosition1Click(TObject *Sender);
	void __fastcall RemoveExclusion1Click(TObject *Sender);
	void __fastcall ZoomInSpeedButtonClick(TObject *Sender);
	void __fastcall ExtendSpeedButtonClick(TObject *Sender);
	void __fastcall ZoomOutSpeedButtonClick(TObject *Sender);
	void __fastcall img2MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled);
	void __fastcall MoveMis1Click(TObject *Sender);
	void __fastcall DropMis1Click(TObject *Sender);
	void __fastcall MoveAgv1Click(TObject *Sender);
	void __fastcall DropAgv1Click(TObject *Sender);
	void __fastcall StandBy1Click(TObject *Sender);
	void __fastcall TimerSinotticoTimer(TObject *Sender);
	void __fastcall Svuotacorsia1Click(TObject *Sender);
	void __fastcall DisabilitaCorsia1Click(TObject *Sender);
	void __fastcall Abilitacorsia1Click(TObject *Sender);
	void __fastcall SpSimulazioneClick(TObject *Sender);
	void __fastcall BitBtn4Click(TObject *Sender);
	void __fastcall BitBtn5Click(TObject *Sender);
	void __fastcall svuotapos1Click(TObject *Sender);
	// void __fastcall CreaMissionePick1Click(TObject *Sender);
	// void __fastcall CreaMissioneDrop1Click(TObject *Sender);
	void __fastcall BitBtn8Click(TObject *Sender);
	void __fastcall escludiagv1Click(TObject *Sender);
	void __fastcall creamisvuoto1Click(TObject *Sender);
	void __fastcall BitBtn12Click(TObject *Sender);
	void __fastcall ArticoliUDCClick(TObject *Sender);
	void __fastcall InsertUDC1Click(TObject * Sender);
	void __fastcall AllarmiAttiviClick(TObject * Sender);
	void __fastcall StoricoAllarmiClick(TObject * Sender);
	void __fastcall bStoriciMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall MissioniClick(TObject * Sender);
	void __fastcall CentroMissioniClick(TObject * Sender);
	void __fastcall bStatoAGVMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall StatoAGVClick(TObject * Sender);
	void __fastcall StatoBatteriaClick(TObject * Sender);
	void __fastcall PrelevaUDC1Click(TObject * Sender);
	void __fastcall DepositaUDC1Click(TObject * Sender);
	void __fastcall PesaUDC1Click(TObject * Sender);
	void __fastcall MoveMisMan1Click(TObject * Sender);
	void __fastcall ArticoliUDCMultipliClick(TObject * Sender);
	void __fastcall rimuovi_prenotazione1Click(TObject * Sender);
	void __fastcall CentraleUDCClick(TObject * Sender);
	void __fastcall MisManSpost1Click(TObject * Sender);
	void __fastcall MisManSpostPLCClick(TObject * Sender);
	void __fastcall btFastSimClick(TObject *Sender);
	void __fastcall TimerInfoTimer(TObject *Sender);
	void __fastcall Splitter1Moved(TObject *Sender);
	void __fastcall Splitter1CanResize(TObject *Sender, int &NewSize, bool &Accept);
	void __fastcall ScrollBox1EndDrag(TObject *Sender, TObject *Target, int X, int Y);
	void __fastcall LAbalAGVMouseUp(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btPrelievo1Click(TObject *Sender);
	void __fastcall btPrelievo1ClickOLD(TObject *Sender);
	void __fastcall btPulisciClick(TObject *Sender);
	void __fastcall btEseguiMissioneManualeClick(TObject *Sender);
    void __fastcall btAnagraficheMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall GestioneUtentiClick(TObject * Sender) ;
    void __fastcall UDCClick(TObject * Sender) ;
    void __fastcall sbMissioniManualiClick(TObject *Sender);
    void __fastcall btGiornoClick(TObject *Sender);
    void __fastcall btNotteClick(TObject *Sender);

private:
	template<class T>
	void __fastcall CreateMDIChild(T *form, String Name, String Caption);

public:
	virtual __fastcall TMainForm(TComponent *Owner);
	void __fastcall AfterConstruction(void);
	void __fastcall SecondoPianoAGVClick(TObject * Sender);
	void SetVisible();
	void CambiaLingua(int lang, bool warning = true);
	void SavePassword();
	void LoadPassword();
	void UpdControls();
	void LeggiParametri();
	void AggiornaMappa();
	void CreaAggiornaPorteSuMappa();
	void EscludiPosSelezionate(int pos, int escludi);
	void DisegnaCarrello(int car);
	int RicercaImgLayout(int posx, int posy);
	int RicercaPortaLayout(int posx, int posy);
	void VisualizzaPercorsoAgv(int nagv = 0);
	int RicercaAGVLayout(int posx, int posy);
	void CreaAggiornaPLCSuMappa();
	void CreaAggiornaCBSuMappa();
	void CreaAggiornaLavatriciSuMappa();
	void VisualizzaPosPlc(ModuloPhoenix &StructPlc);
	void CreaAggiornaSemaforiSuMappa();
	int ProporzionaX(int xx);
	int ProporzionaY(int yy);
    void RiepilogoSegnali() ;

	// ModuloPhoenix DatiPhoenix[2] ;
	// PlcVemacArelle PlcVemacArelleIngresso , PlcVemacArelleUscita ;
	int all;
	AnsiString imgfile_agv[8]; // come N_Carrelli

	AnsiString PWD, RobotAddress;
	int baiaformSegnali, TouchPanel, stationnumber, EnableManuals, countwatchdog, actual, prev, num_stazioni;
	bool formbaiaattivo, forzature_attive;
	AnsiString msg, timestr, NomeClient, format_data_ora_datatbase;
	int tempo_carica, init_mappa, tempo_carica_batt_bassa, percentuale_batt_bassa, percentuale_batt_ok, poscb;
	int zoom, count_inizio, alarmmissioni, chiusura_applicazione, num_pallet_minimi, tempo_pallet, count_cb, count_forzature;
	double Xo, Yo, FormH, FormW, H, W;
	int recnum;
	int pos_udc, piano_udc, corsia_udc;
	// variabili per zoom e selezione
	int agvdaspostare, Contatore, x1, x2, y1, y2, Contatoresel, x1sel, x2sel, y1sel, y2sel, xmouse, ymouse, countmousemove, countwheel;
	int widthextend, heightextend, ultimapospassata, ridisegnapalsel;
	int invia_richiesta_simulazione, spostaframe, aggiornamappa, aggiornaagv, h_corsia[5], quota_dep_terra, offset_dep, pos_mappa_no_db[4], num_pos_no_database;
	int aggiorna_stato_pos, prelievo_missione_manuale, count_aggiorna_mappa_client;
	int trova_udc, count_trova_udc;
	bool ret, mousedown, retsel, mousedownsel, sync_lamp;

	TTime ScrollMouse;

	// TMyConnector *ToPickAgv1, *ToPickAgv2, *ToDropAgv1, *ToDropAgv2, *ToDropPosPrel;
	TMyConnector *ToMoveAgv;
	clock_t time_ultimo_ridisegno;
};

// ----------------------------------------------------------------------------
extern TMainForm *MainForm;
extern TMDIChild *__fastcall MDIChildCreate(void);
// ----------------------------------------------------------------------------
#endif
