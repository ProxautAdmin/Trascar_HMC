//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frame_modulo.h"
#include "dmFunzioniComuniClientServer.h"
#include "db.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrameModulo *FrameModulo;
extern int lunghezza_campi[NCAMPI] ;
extern int indice_inizio_campi[NCAMPI]   ;
extern int tipologia_campi[NCAMPI]   ; //1 byte ,2 int/word,3 doppia word,4 float,5 double ,6 char,7 string con primi 2 byte per lunghezza max e lunghezza stringa
//---------------------------------------------------------------------------
__fastcall TFrameModulo::TFrameModulo(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void TFrameModulo::RiempiCampi(AnsiString struttura_hex)
{
    AnsiString strsql, stringa;
	int i, indice_buffer;
	unsigned char buffer[STRUTTURA_DATI_UDC_PLC] ;
	TEdit *frLoad;

	try {
		dmFunzioni->DaEsadecimaleAChar(struttura_hex, buffer) ;
		for (i = 0; i < 3; i++) {    //visualizzo solo lotto e materiale
			frLoad = (TEdit*)(FindComponent("editDato" + IntToStr(i+1)));
			if (frLoad != NULL) {
				if (tipologia_campi[i] == 1) {
					//byte)
					frLoad->Text = IntToStr(dmFunzioni->DaBufferAByte(buffer + indice_inizio_campi[i]));
				}else if (tipologia_campi[i] == 2) {
					// word
					frLoad->Text = IntToStr(dmFunzioni->DaBufferAWord(buffer + indice_inizio_campi[i]));
				}else if (tipologia_campi[i] == 3) {
					//doppia word
					frLoad->Text = IntToStr(dmFunzioni->DaBufferADoppiaWord(buffer + indice_inizio_campi[i]));
				}else if (tipologia_campi[i] == 4) {
					//float
					//frLoad->Text = IntToStr(dmFunzioni->DaBufferADoppiaWord(buffer + indice_inizio_campi[i]));
				}else if (tipologia_campi[i] == 5) {
					//double
					frLoad->Text = dmFunzioni->ReturnDoubleDaBuffer(buffer + indice_inizio_campi[i]);
				}else if (tipologia_campi[i] == 6) {
					//char
					frLoad->Text = dmFunzioni->DaBufferAStringa(buffer + indice_inizio_campi[i],lunghezza_campi[i]) ;
				}else if (tipologia_campi[i] == 7) {
					//string
					// byte 0 lunghezza max string
					//byte 1 lunghezza attuale string
					frLoad->Text = dmFunzioni->DaBufferAStringa(buffer + indice_inizio_campi[i]+2,lunghezza_campi[i]) ;
				}
			}
		}
		for (i = 0 ; i<20;i++ ) { //divido i 20 caratteri del certificto in ogni singolo char
			frLoad = (TEdit*)(FindComponent("eC" + IntToStr(i+1)));
			if (frLoad != NULL)
				frLoad->Text = dmFunzioni->DaBufferAStringa(buffer + indice_inizio_campi[3]+i,1) ;
		}
		for (i = 0 ; i<20;i++ ) { //divido i 20 caratteri della provenienza in ogni singolo char
			frLoad = (TEdit*)(FindComponent("eP" + IntToStr(i+1)));
			if (frLoad != NULL)
				frLoad->Text = dmFunzioni->DaBufferAStringa(buffer + indice_inizio_campi[4]+i,1) ;
		}
	}catch (...) {

	}
}
void __fastcall TFrameModulo::BitBtn1Click(TObject *Sender)
{
	// modifico
}
//---------------------------------------------------------------------------


