// ---------------------------------------------------------------------------

#pragma hdrstop

#include "dmFunzioniComuniClientServer.h"
#include <stdio.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TdmFunzioni *dmFunzioni;

// ---------------------------------------------------------------------------
__fastcall TdmFunzioni::TdmFunzioni(TComponent* Owner)
    : TDataModule(Owner)
{
}

// ---------------------------------------------------------------------------
void TdmFunzioni::DaEsadecimaleAChar(AnsiString var_hex, unsigned char *data)
{
    int i, val, count;
    count = 0;
    for (i = 0; i < var_hex.Length(); i += 2) {
        sscanf(var_hex.c_str() + i, "%2X", &val); // leggo 2 caratteri della stringa che corrispondono al valore del byte in hex
        data[count] = val;
        count++;
    }
}

void TdmFunzioni::DaEsadecimaleInvertitoAChar(AnsiString var_hex, unsigned char *data)
{
    // inverto la stringa esadecimale partendo dal fondo (caloclo dei numeri float-real e double)
    int i, val, count;
    count = 0;
    for (i = var_hex.Length() - 2; i >= 0; i -= 2) {
        sscanf(var_hex.c_str() + i, "%2X", &val); // leggo 2 caratteri della stringa che corrispondono al valore del byte in hex
        data[count] = val;
        count++;
    }
}

AnsiString TdmFunzioni::DaIntegerAHex(int val, int num_hex)
{
    // ritorno stringa esadecimale (di 2,4,6 caratteri in base a num_hex) da intero
    AnsiString stringa = "", format_str;
    format_str = "%0" + IntToStr(num_hex) + "X";
    stringa.printf(format_str.c_str(), val);
    return stringa;
}

AnsiString TdmFunzioni::DaStringaAHex(AnsiString val, int lunghezza_str)
{
    // ritorno stringa esadecimale (di 2,4,6 caratteri in base a num_hex) da intero
    char car;
    int i, lung, lung_str;
    AnsiString stringa = "";
    if (lunghezza_str == 0) {
        lung = val.Length();
        lung_str = val.Length();
    }
    else {
        lung = lunghezza_str;
        lung_str = val.Length();
    }
    for (i = 0; i < lung; i++) {
        if (i < lung_str) {
            sscanf(val.c_str() + i, "%c", &car); // leggo 2 caratteri della stringa che corrispondono al valore del byte in hex
            stringa = stringa + DaIntegerAHex(car, 2);
        }
        else {
            stringa = stringa + DaIntegerAHex(0, 2); // inserisco carattere nullo
        }
    }
    // stringa.printf("%"+IntToStr(num_hex)+"X",val);
    return stringa;
}

AnsiString TdmFunzioni::DaTipoStringPlcAHex(AnsiString val, int lunghezza_str_max)
{
    // ritorno stringa esadecimale da tipo string su plc
    // il tipo string ha la lunghezza della stringa max e la lunghezza della stringa attuale nei primi 2 byte
    // Esempio Var String[10] , quindi i byte impegnati sono sempre 2 in piu
    // byte[0] = 10 lunghezza max stringa
    // byte[1] = 4  lunghezza attuale stringa (4 , CIAO)
    // byte[2] = C
    // byte[3] = I
    // byte[4] = A
    // byte[5] = O
    // byte[6] = 0
    // byte[7] = 0
    // byte[8] = 0
    // byte[9] = 0
    // byte[10] = 0
    // byte[11] = 0

    char car;
    int i, lung, lung_str;
    AnsiString stringa = "";
    if (lunghezza_str_max == 0) {
        lung = val.Length();
        lung_str = val.Length();
    }
    else {
        lung = lunghezza_str_max;
        lung_str = val.Length();
    }
    stringa = stringa + DaIntegerAHex(lung, 2);
    stringa = stringa + DaIntegerAHex(lung_str, 2);
    for (i = 0; i < lung; i++) {
        if (i < lung_str) {
            sscanf(val.c_str() + i, "%c", &car); // leggo 2 caratteri della stringa che corrispondono al valore del byte in hex
            stringa = stringa + DaIntegerAHex(car, 2);
        }
        else {
            stringa = stringa + DaIntegerAHex(0, 2); // inserisco carattere nullo
        }
    }
    // stringa.printf("%"+IntToStr(num_hex)+"X",val);
    return stringa;
}

AnsiString TdmFunzioni::RitornaStringaFloatConSeparatoreGiusto(AnsiString val, char separatore_forzato) {

    if ((FormatSettings.DecimalSeparator == '.') || (separatore_forzato == '.')) {
        while (val.Pos(",")) // modifico , con .
                val[val.Pos(",")] = '.';
    }
    else {
        while (val.Pos(".")) // modifico . con ,
                val[val.Pos(".")] = ',';
    }
    return val;
}

double TdmFunzioni::ReturnDoubleDaBuffer(unsigned char *data) {
    int i;
    union {
        double val;
        unsigned char buf[8];
    } buftodouble;
    for (i = 0; i < 8; i++) {
        buftodouble.buf[i] = data[7 - i]; // per calcolare un dato float o double devo invertire i 4-8 byte partendo dall'ultimo
    }
    // memcpy(buftodouble.buf,data,8);
    // memcpy(&hextodouble.val, hextodouble.buf, sizeof (double));
    return buftodouble.val;

}

double TdmFunzioni::ReturnDoubleDaHex(AnsiString strhex) {
    union {
        double val;
        unsigned char buf[8];
    } buftodouble;
    DaEsadecimaleInvertitoAChar(strhex, buftodouble.buf);
    // memcpy(&hextodouble.val, hextodouble.buf, sizeof (double));
    return buftodouble.val;

}

AnsiString TdmFunzioni::RitornaHexDaDouble(AnsiString valdouble) {
    int i;
    double val;
    unsigned char *bytes;
    AnsiString res, result;
    // valdouble = dmFunzioni->RitornaStringaFloatConSeparatoreGiusto(Edit1->Text) ;
    val = valdouble.ToDouble();
    bytes = (unsigned char *)&val;
    result = "";
    for (i = sizeof(val) - 1; i >= 0; i--) { // in hex devo invertire l'ordine partendo dal fondo
        res.printf("%02X", bytes[i]);
        result = result + res;
    }
    return result;
}

AnsiString TdmFunzioni::DaBufferAStringa(unsigned char *data, int lunghezza_str)
{
    // ritorno stringa esadecimale (di 2,4,6 caratteri in base a num_hex) da intero
    char car;
    int j;
    AnsiString stringa = "", str_res = "";
    for (j = 0; j < lunghezza_str; j++) {
        stringa = "";
        stringa.printf("%c", data[j]);
        if ((stringa != "") && (data[j] != 0))
            str_res = str_res + stringa;
    }
    return str_res;
}

int TdmFunzioni::DaBufferAByte(unsigned char *data)
{
    // ritorno stringa esadecimale (di 2,4,6 caratteri in base a num_hex) da intero
    char car;
    int res;
    res = data[0];
    return res;
}

int TdmFunzioni::DaBufferAWord(unsigned char *data)
{
    // ritorno stringa esadecimale (di 2,4,6 caratteri in base a num_hex) da intero
    char car;
    int res;
    res = (data[0] * 256) + data[1];
    return res;
}

int TdmFunzioni::DaBufferADoppiaWord(unsigned char *data)
{
    // ritorno stringa esadecimale (di 2,4,6 caratteri in base a num_hex) da intero
    char car;
    int res;
    res = (data[0] * 256 * 256 * 256) + (data[1] * 256 * 256) + (data[2] * 256) + data[3];
    return res;
}

int TdmFunzioni::DaEsadecimaleAByte(AnsiString var_hex)
{
    int i, val, count, res;
    unsigned char data[1];
    count = 0;
    for (i = 0; i < var_hex.Length(); i += 2) {
        sscanf(var_hex.c_str() + i, "%2X", &val); // leggo 2 caratteri della stringa che corrispondono al valore del byte in hex
        data[count] = val;
        count++;
    }
    res = data[0];
    return res;
}

int TdmFunzioni::DaEsadecimaleAWord(AnsiString var_hex)
{
    int i, val, count, res;
    unsigned char data[2];
    count = 0;
    for (i = 0; i < 4; i += 2) {
        sscanf(var_hex.c_str() + i, "%2X", &val); // leggo 2 caratteri della stringa che corrispondono al valore del byte in hex
        data[count] = val;
        count++;
    }
    res = (data[0] * 256) + data[1];
    return res;
}

int TdmFunzioni::DaEsadecimaleADoppiaWord(AnsiString var_hex)
{
    int i, val, count, res;
    unsigned char data[4];
    count = 0;
    for (i = 0; i < 8; i += 2) {
        sscanf(var_hex.c_str() + i, "%2X", &val); // leggo 2 caratteri della stringa che corrispondono al valore del byte in hex
        data[count] = val;
        count++;
    }
    res = (data[0] * 256 * 256 * 256) + (data[1] * 256 * 256) + (data[2] * 256) + data[3];
    return res;
}

AnsiString TdmFunzioni::DaBufferAHex(unsigned char *data, int lunghezza_buffer) {
    AnsiString stringa = "", result;
    int i;
    for (i = 0; i < lunghezza_buffer; i++) {
        stringa.printf("%02X", data[i]);
        result = result + stringa;
    }
    return result;
}

void TdmFunzioni::StringToChar(AnsiString strdaconv, unsigned char *data) {
    AnsiString strpos = "";
    strpos = String(strdaconv); // serve nel caso non sia un AnsiString ma un UnicodeString
    sprintf(data, "%s", strpos.c_str());
}

int TdmFunzioni::CompilaDoppiaWord(int wordsx, int worddx)
{
    // compilo doppia word da word singole
    char car;
    int res;
    res = (wordsx * 256 * 256) + worddx;
    return res;
}

void TdmFunzioni::DividiWord(int parola, int &bytedx, int &bytesx) {
    bytedx = parola & 0xFF;
    bytesx = (parola >> 8) & 0xFF;
}

int TdmFunzioni::CompilaWord(int intsx, int intdx)
{
    // compilo doppia word da word singole
    char car;
    int res;
    res = (intsx  * 256) + intdx;
    return res;
}


