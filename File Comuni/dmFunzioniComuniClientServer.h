// ---------------------------------------------------------------------------

#ifndef dmFunzioniComuniClientServerH
#define dmFunzioniComuniClientServerH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>

// ---------------------------------------------------------------------------
class TdmFunzioni : public TDataModule
{
__published: // IDE-managed Components
        private : // User declarations
    public : // User declarations
        __fastcall TdmFunzioni(TComponent* Owner);
    void DaEsadecimaleAChar(AnsiString var_hex, unsigned char *data);
    void DaEsadecimaleInvertitoAChar(AnsiString var_hex, unsigned char *data);
    AnsiString DaIntegerAHex(int val, int num_hex);
    AnsiString DaStringaAHex(AnsiString val, int lunghezza_str = 0);
    AnsiString RitornaStringaFloatConSeparatoreGiusto(AnsiString val, char separatore_forzato = '0');
    double ReturnDoubleDaBuffer(unsigned char data[8]);
    double ReturnDoubleDaHex(AnsiString strhex);
    AnsiString DaBufferAStringa(unsigned char *data, int lunghezza_str);
    AnsiString DaTipoStringPlcAHex(AnsiString val, int lunghezza_str);
    AnsiString RitornaHexDaDouble(AnsiString valdouble);
    int DaBufferAByte(unsigned char *data);
    int DaBufferAWord(unsigned char *data);
    int DaBufferADoppiaWord(unsigned char *data);
    int DaEsadecimaleAByte(AnsiString var_hex);
    int DaEsadecimaleAWord(AnsiString var_hex);
    int DaEsadecimaleADoppiaWord(AnsiString var_hex);
    AnsiString DaBufferAHex(unsigned char *data, int lunghezza_buffer);
    void StringToChar(AnsiString strdaconv, unsigned char *data);
    int CompilaDoppiaWord(int wordsx, int worddx);
    void DividiWord(int parola, int &bytedx, int &bytesx);
    int CompilaWord(int intsx, int intdx);

};

// ---------------------------------------------------------------------------
extern PACKAGE TdmFunzioni *dmFunzioni;
// ---------------------------------------------------------------------------
#endif
