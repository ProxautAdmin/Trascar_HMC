// ---------------------------------------------------------------------------

#ifndef DBImpiantoH
#define DBImpiantoH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "clientdata.h"

// ---------------------------------------------------------------------------
class TdmDBImpianto : public TDataModule
{
__published: // IDE-managed Components
        private : // User declarations
    public : // User declarations
        __fastcall TdmDBImpianto(TComponent* Owner);
    void TornaPosPianoDaIDUDC(int idudc, int &pos, int &piano);
    int TornaTabella(int pos, int piano, int fila);
    int TornaPosPianoDaNomePos(AnsiString Nomepos, int &pos, int &piano);
    int RitornaLatoForche(int pos, int corsia, int isprel = 0);
    int AggiornaStatoMissioniWMS(int id, int stato);
    void RitornaPosPianodaNomePos(AnsiString NomePos, int &pos, int &piano, int &corsia);
    void AnnullaMissione();

    int AggiornaAgvMissioniWMS(int id, int agv);

    int AggiornaStatoEtichettatura(int agv);
    int RitornaPosizioneChiamata(int pos);
    int RitornaUDCPosizione(int pos);
    int RitornaIdPlcDaPosizione(int posizione, int tipo_missione, int piano);
    int AggiornaSelezionePosizioni(AnsiString Zona, int pos, int stato);
    void LeggiSelezionataDaZona(AnsiString Zona, int &pos, int &piano);
    void ResettaPLC();
    void SettaPLCInit();
    void SettaPLCEsclusa(int pos, int val);
    void SettaPLCPrenotata(int pos, int val);
    void SettaPLCIDUDC(int pos, int val);
    void TornaPosDepLibera(AnsiString Zona, int &pos, int &piano, int tipoposizione = 0);
    int PianiOccupatiPerPos(int pos);
    void PrelievoVuoti(int idzona, int &pos, int &pianiocc);
    int TornaPosVuoteZona(int idzona);
    int CercaPrelievoH();
    int AggiornaUDCPosizioni(int pos, int IDUDC, int piano = 0);
    int CercaPrelievo(AnsiString Zona, int tipoposizione = 0);
    void TornaPosDepLiberaH(AnsiString Zona, int IDUDC, int &pos, int &piano);
    AnsiString TornaDescrizionedaZonaA1(AnsiString CodArt, int &impila);
    int TornaIndiceImpilabilitadaIDUDC(int idudc);
    int ClonaHMC_ORDINI_IN_LAVORAZIONE(AnsiString posizione);
    int CheckZonaUtente(AnsiString zonadacontrollare);
    AnsiString TornaCodartConRigaDaHMC_ORDINI_IN_LAVORAZIONE(int riga, TUDC &UDC);
    int CercaPrelievoF();
    int CercaUDCinH(int udc);
    int GeneraCMDaHaBFineProduzione(int val);
    AnsiString TornaArticoloDaHMC_ORDINI_IN_LAVORAZIONEcopia(AnsiString posizione) ;

};

// ---------------------------------------------------------------------------
extern PACKAGE TdmDBImpianto *dmDBImpianto;
// ---------------------------------------------------------------------------
#endif
