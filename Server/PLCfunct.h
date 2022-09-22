//---------------------------------------------------------------------------

#ifndef PLCfunctH
#define PLCfunctH

#define BYTE0	0x000000FF
#define BYTE1	0x0000FF00
#define BYTE2	0x00FF0000
#define BYTE3	0xFF000000

#define BIT0	0x0001
#define BIT1	0x0002
#define BIT2	0x0004
#define BIT3	0x0008
#define BIT4	0x0010
#define BIT5	0x0020
#define BIT6	0x0040
#define BIT7	0x0080
#define BIT8	0x0100
#define BIT9	0x0200
#define BIT10	0x0400
#define BIT11	0x0800
#define BIT12	0x1000
#define BIT13	0x2000
#define BIT14	0x4000
#define BIT15	0x8000

#define CheckBit(w, b)	((w) & (b))

//---------------------------------------------------------------------------
int DBD(char *buffer);
int DBW(char *buffer);
void FormatDBD(int n, char *buffer);
void FormatDBW(int n, char *buffer);
char GetBit(unsigned char value, unsigned char bit);
double GetTimer(char *buffer);
void FormatTimer(double t, char *buffer);
//---------------------------------------------------------------------------

#endif
