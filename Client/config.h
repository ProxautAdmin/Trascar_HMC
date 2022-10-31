//---------------------------------------------------------------------------

#ifndef configH
#define configH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "ChildWin.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "DB.h"
#include "DBClient.h"
#include "selectAgvframe.h"
#include "configCBFrame.h"
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrConfig : public TMDIChild
{
__published:	// IDE-managed Components
    TPanel *pMain;
	TBitBtn *BitBtnChange;
    TPanel *pAttivita;
    TPanel *pTitoloSettaggio;
    TBitBtn *BitBtnPos;
    TBitBtn *btStoricizza;
    TGroupBox *GroupBox1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TComboBox *cbDaGiorno1;
    TComboBox *cbAGiorno1;
    TDateTimePicker *DaOra1;
    TDateTimePicker *AOra1;
    TTimer *Timer1;
    TPanel *pMissioni;
    TfrSelectAgv *frSelectAgv1;
    TfrSelectAgv *frSelectAgv2;
    TfrSelectAgv *frSelectAgv3;
    TfrSelectAgv *frSelectAgv4;
    TfrSelectAgv *frSelectAgv5;
    TPanel *pSettaggiImpianto;
	TLabeledEdit *editAltezzaPalletBasso;
    TPanel *pAGV;
    TPanel *pBatteria;
    TPanel *Panel2;
    TfrCB *frCB1;
    TPanel *pExtra;
    TCheckBox *cAGVMov;
    TCheckBox *cInfoBox;
    TCheckBox *ckbForzaPorta;
    TBitBtn *btnForzaPorta;
    TPanel *pPesi;
    TLabel *Label1;
    TLabeledEdit *lePesoCamp;
    TLabeledEdit *lePesoFreq;
    TLabeledEdit *leTolleranza;
    TComboBox *cbPrioritaPeso;
    TCheckBox *cMissioniPeso;
    TPanel *pTitleAGV;
    TPanel *pGestione;
    TBitBtn *btResetPos;
    TBitBtn *btResetAGV;
    TLabeledEdit *leNumMissioni;
    TCheckBox *ckAbilitaCheckGiornoNotte;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall BitBtnChangeClick(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall BitBtnPosClick(TObject *Sender);
    void __fastcall btStoricizzaClick(TObject *Sender);
    void __fastcall btServiceClick(TObject *Sender);
	void __fastcall btnForzaPortaClick(TObject *Sender);
    void __fastcall btResetPosClick(TObject *Sender);
    void __fastcall btResetAGVClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrConfig(TComponent* Owner);
	int Val_Init_Params[99],num_params, numframe;
    AnsiString  Str_Init_Params[99]  ;
    int output[0];
};
//---------------------------------------------------------------------------
extern PACKAGE TfrConfig *frConfig;
//---------------------------------------------------------------------------
#endif
