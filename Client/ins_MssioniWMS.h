//---------------------------------------------------------------------------

#ifndef ins_MssioniWMSH
#define ins_MssioniWMSH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CSPIN.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <Mask.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFIns_MissioniWMS : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TADOQuery *Query;
    TEdit *edDataOraCreazioneWMS;
    TLabel *Label8;
    TEdit *edCorsiaDeposito;
    TLabel *Label7;
    TEdit *edPesoUDC;
    TLabel *Label5;
    TEdit *edIDUDC;
    TEdit *edPosizioneDeposito;
    TEdit *edPosizionePrelievo;
    TLabel *Label3;
    TLabel *Label2;
    TLabel *Label1;
    TEdit *edTipoMissione;
    TLabel *Label4;
    TLabel *Label6;
    TEdit *edID;
    TLabel *Label9;
    TEdit *edPriorita;
	TLabel *Label10;
    TEdit *edTipoUDC;
	TLabel *Label11;
    TEdit *edStatoMissioneWMS;
    TEdit *edIDAgv;
    TLabel *Label12;
    TEdit *edStatoMissioneProxaut;
    TLabel *Label13;
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
    __fastcall TFIns_MissioniWMS(TComponent* Owner);
	bool insert ;
	int idtabella ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFIns_MissioniWMS *FIns_MissioniWMS;
//---------------------------------------------------------------------------
#endif
