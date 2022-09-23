// ---------------------------------------------------------------------------

#ifndef ExtraFunctionH
#define ExtraFunctionH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdExplicitTLSClientServerBase.hpp>
#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdMessageClient.hpp>
#include <IdSMTP.hpp>
#include <IdSMTPBase.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdMessage.hpp>

// ---------------------------------------------------------------------------
class TdmExtraFunction : public TDataModule
{
__published: // IDE-managed Components
    TIdSSLIOHandlerSocketOpenSSL *IdSSLIOHandlerSocketOpenSSL1;
    TIdSMTP *IdSMTP1;
    TIdMessage *IdMessage1;

        private : // User declarations
    public : // User declarations
        __fastcall TdmExtraFunction(TComponent* Owner);
    AnsiString nomeIstanza(AnsiString strsql);
	AnsiString PadS(AnsiString strt, int lenp, AnsiString padc);
	AnsiString PadR(AnsiString strt, int lenp, AnsiString padc)  ;
    void EsportaExcel(AnsiString SelectStr, AnsiString Nomefile);
    void ComboNumProg(TComboBox * tcbCombo, int nProg);
	void ComboNomeCorsia(TComboBox * tcbCombo, int piena, AnsiString appendRecord, AnsiString valoredef="");
    void ComboScelte(TComboBox * tcbCombo, AnsiString appendRecord);
    AnsiString RiempiTipoUDC(TComboBox * tcbCombo, AnsiString cerca);
    AnsiString RiempiTipoStatus(TComboBox * cbStatus, AnsiString cerca);
    AnsiString RiempiTipoMissione(TComboBox * tcbCombo, AnsiString cerca);
    AnsiString RiempiTipoMovimento(TComboBox * tcbCombo, AnsiString cerca);
	AnsiString RiempiEsitoMissione(TComboBox * tcbCombo, AnsiString cerca);
	AnsiString RiempiStatoMissione(TComboBox * tcbCombo, AnsiString cerca) ;
	void ComboNumFileCorsia(TComboBox * tcbCombo, AnsiString Nomecorsia, AnsiString appendRecord);
	int StoricizzaTabelle(AnsiString tabella, AnsiString tabellaDest, AnsiString campo, AnsiString periodo, int periododaeliminare);
    int StoricizzaTutto(AnsiString periodo, int periododaeliminare);
    AnsiString RiempiTipologiaCorsia(TComboBox * tcbCombo, AnsiString cerca)    ;
    int bitRead(int value, int bit);
    int bitSet(int value, int bit);
    int bitClear(int value, int bit);
    int bitWrite(int value, int bit, int bitvalue);
	float ProporzionaZoom(int zz, float parametro)   ;
    int SendEmail(AnsiString message) ;
    void CheckAlarm();
	AnsiString StringNumToLetter(AnsiString stringnum)    ;
    AnsiString RiempiGiorniSettimana(TComboBox * tcbCombo, AnsiString cerca) ;
    void StringToChar(AnsiString varString, unsigned char *varChar) ;
    int VerificaUtentePerZona(int zonaint) ;
    int Constrain(int vval, int mmin, int mmax);
    bool IsNum(char c) ;
    void ComboScelte(TComboBox * tcbCombo, int numero, int def, int start=0);


};

// ---------------------------------------------------------------------------
extern PACKAGE TdmExtraFunction *dmExtraFunction;
// ---------------------------------------------------------------------------
#endif
