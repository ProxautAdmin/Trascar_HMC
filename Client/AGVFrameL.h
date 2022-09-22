//---------------------------------------------------------------------------

#ifndef AGVFrameLH
#define AGVFrameLH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TfrAgvL : public TFrame
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *pAGV;
	TLabeledEdit *lePosizione;
	TLabeledEdit *leDestinazione;
	TLabeledEdit *leMissione;
	TImage *Image1;
	TLabeledEdit *leLivBatt;
	TPopupMenu *Forzature;
	TMenuItem *ForzaIdCoilsuAgv1;
	TGroupBox *GroupBox4;
	TCheckBox *cbAlarm;
	TCheckBox *cbCarica;
	TCheckBox *cbMan;
	TCheckBox *cbAuto;
	TCheckBox *cbBattBassa;
	TCheckBox *cbSemiAuto;
	TCheckBox *cbLoaded;
	TPanel *pInMis;
	TCheckBox *cbInMissione;
    TGroupBox *GroupBox1;
    TLabeledEdit *leTipoUDC;
    TLabeledEdit *leIDUDC;
	TLabel *lbTipoCarica;
    TCheckBox *cbSospeso;
    TPanel *Panel2;
    TCheckBox *cbForcheDX;
    TCheckBox *cbForcheSX;
    TLabeledEdit *leLatoForchePrel;
    TLabeledEdit *leLatoForcheDep;
    TLabeledEdit *leNodoPassaggioPrel;
    TLabeledEdit *leNodoPassaggioDep;
	TLabeledEdit *EditAltezzaPallet;
	TCheckBox *ckbForcheInTrazione;
	TCheckBox *ckbDaEtichettare;
	TCheckBox *ckbStandbyAccesso;
	void __fastcall ForzaIdCoilsuAgv1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrAgvL(TComponent* Owner);
	bool aggiornamento ;
	int idagv ;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrAgvL *frAgvL;
//---------------------------------------------------------------------------
#endif
