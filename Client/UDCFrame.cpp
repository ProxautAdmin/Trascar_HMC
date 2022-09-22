// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UDCFrame.h"
#include "db.h"
#include "main.h"
#include "SocketComunicazioneClient.h"
#include "dmFunzioniComuniClientServer.h"
//#include "anagrafica_articoliUDC.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "frame_modulo"
#pragma resource "*.dfm"
TfUDCSmall *fUDCSmall;
extern int lunghezza_campi[NCAMPI] ;
extern int indice_inizio_campi[NCAMPI]   ;
extern int tipologia_campi[NCAMPI]   ; //1 byte ,2 int/word,3 doppia word,4 float,5 double ,6 char,7 string con primi 2 byte per lunghezza max e lunghezza stringa

// ---------------------------------------------------------------------------
__fastcall TfUDCSmall::TfUDCSmall(TComponent* Owner)
    : TFrame(Owner)
{
}

// ---------------------------------------------------------------------------
void TfUDCSmall::RiempiCampi(int idartudc, AnsiString struttura_hex, bool activate)
{
    AnsiString strsql, stringa, struttura_hex_modulo, CorsiaDest;
    int status = 0, i, j, indice_struttura_hex_modulo, indice_ricetta, indice_riempimento, byte_riempimento[4];
    TCheckBox *cb;
    TFrameModulo *frModule;

    TEdit *frLoad;

    eIDArtUDC->Text = idartudc;
	eDest->Text = "0";
	if (activate) {
        eLastUdc->Text = dmDB->ReturnUltimoIdUdceDestDaIdArticolo(idartudc, CorsiaDest);
        eDest->Text = CorsiaDest;
    }
	for (i = 1; i <= 1; i++) {
        frModule = (TFrameModulo*) FindComponent("FrameModulo" + IntToStr(i));
		if (frModule != NULL) {
			indice_struttura_hex_modulo = 1 ; // in hex la struttura è lunga il doppio
            struttura_hex_modulo = struttura_hex.SubString(indice_struttura_hex_modulo, STRUTTURA_DATI_UDC_PLC * 2);
			frModule->GroupBox1->Caption = "Dati Prodotto ";
            frModule->RiempiCampi(struttura_hex_modulo);
        }
    }
}

void __fastcall TfUDCSmall::BitBtn1Click(TObject *Sender)
{
    int i, j, byte_riempimento[3], scrivi_plc;
    AnsiString stringa, prodotto, str_hex_to_write, str_hex_to_write2;
    TEdit *frLoad;
    TFrameModulo *frModule;
    TCheckBox *cb;
    TBitBtn *But;
    But = (TBitBtn*) Sender;
    if (But->Caption.Pos("Plc"))
        scrivi_plc = 1;
    else
        scrivi_plc = 0;

    // forzo segnali in scrittura.
    // Nel caso i segnali siano di lettura utilizzo ugualmente la write word che va a+
    // modificare il valore direttamente in PLCThread così viene riletto
    // calcolo stringa esadecimale per scrivere tutti i dati in un unica volta

	str_hex_to_write = "";
	str_hex_to_write = str_hex_to_write + dmFunzioni->DaStringaAHex(FrameModulo1->editDato1->Text, 10) ;
	str_hex_to_write = str_hex_to_write + dmFunzioni->DaStringaAHex(FrameModulo1->editDato2->Text, 10) ;
	str_hex_to_write = str_hex_to_write + dmFunzioni->DaIntegerAHex(FrameModulo1->editDato3->Text.ToIntDef(0), 4);
	for(i=0 ; i<20;i++) {
		frLoad = (TEdit*)(FrameModulo1->FindComponent("eC" + IntToStr(i+1)));
		if (frLoad != NULL)
			str_hex_to_write = str_hex_to_write + dmFunzioni->DaStringaAHex(frLoad->Text, 1) ;
	}
	for(i=0 ; i<20;i++) {
		frLoad = (TEdit*)(FrameModulo1->FindComponent("eP" + IntToStr(i+1)));
		if (frLoad != NULL)
			str_hex_to_write = str_hex_to_write + dmFunzioni->DaStringaAHex(frLoad->Text, 1) ;
	}

	//
	if ((eIDArtUDC->Text.ToIntDef(0) > 0) && (!scrivi_plc)) {
		// aggiorno database
		dmDB->InsertUpdateArticoloUDC(eIDArtUDC->Text.ToIntDef(0), 0, 0, str_hex_to_write, eDest->Text, eLastUdc->Text.ToIntDef(0),"");
	}
	else if (scrivi_plc){
		// scrivo dati su plc
		SocketDataModule->ScriviStringaHexPlc(idplc, db_strhex, dw_strhex, numbyte, str_hex_to_write);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfUDCSmall::BtnWriteUdcPlcClick(TObject *Sender) {
	if (BtnWriteUdcPlc->Caption.Pos("Write"))
		SocketDataModule->ScriviDWordPlc(idplc, db_strhex, dw_strhex + numbyte, eIDArtUDC->Text.ToIntDef(0));
	else
		SocketDataModule->InviaSimulazioneInputIdUdc(idplc, eIDArtUDC->Text.ToIntDef(0)); //in idplc ho la pos

}
// ---------------------------------------------------------------------------

void __fastcall TfUDCSmall::sbSearchClick(TObject *Sender) {
    AnsiString struttura_hex;
    FormAnagraficaArticoliUDC->id_art_udc = 0;
    FormAnagraficaArticoliUDC->BitBtn1->Caption = "Inserisci Id In Posizione";
    FormAnagraficaArticoliUDC->ShowModal();
    if (FormAnagraficaArticoliUDC->id_art_udc) {
        dmDB->ReturnStringaHexDatiDaIdArtUdc(FormAnagraficaArticoliUDC->id_art_udc);
        RiempiCampi(FormAnagraficaArticoliUDC->id_art_udc, struttura_hex, true);
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfUDCSmall::BitBtnNewClick(TObject *Sender)
{
      eIDArtUDC->Text = dmDB->CreaIdArtUdc();
      BitBtnNew->Enabled=false;
}
//---------------------------------------------------------------------------

