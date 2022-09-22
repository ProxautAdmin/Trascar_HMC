// ---------------------------------------------------------------------------

#ifndef DBClientH
#define DBClientH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "db.h"

class TdmDBClient : public TDataModule
{
__published: // IDE-managed Components
        private : // User declarations
    public : // User declarations
        __fastcall TdmDBClient(TComponent* Owner);
    void InserisciCorsiePostazioniDatabase();
    int ReturnNextFilaDaPos(int pos);
    int ReturnPrevFilaDaPos(int pos);
    AnsiString ReturnDescTipoUDC(int tipo);
    AnsiString ReturnDescStatoUDC(int tipo);
    int ReturnaColoreDaUDC(int idudc);
    int SetColorePos(int idudc, long &cornice);
    AnsiString ReturnCodiceArticoliUDC(int IDArtUDC);
    int ReturnCodStatoUDCdaIDUDC(int idudc);
    int CercaUDCinPosizione(int idudc);
    int CercaArticoloinArtUDC(int idarticolo);
    void EliminaArticolodaArtUDC(int idartudc);
    int ReturnNewIDARTUDC();
    int RitornaNumpos(AnsiString nomeCorsia);
    AnsiString RitornaCustPosinPosizione(int pos);
    int RitornaMaxIDParametro();
    int contaPianiconUDC(int pos);
    int CercaZona(int xx, int yy);
    int TornaUDCinPosizione(int pos);
    int ReturnPrimaPosDaFila(int fila);
    void InserisciPostazioniFunzione(TPosizioni Posizioni);
    void InserisciPLCDatabase();

};

// ---------------------------------------------------------------------------
extern PACKAGE TdmDBClient *dmDBClient;
// ---------------------------------------------------------------------------
#endif
