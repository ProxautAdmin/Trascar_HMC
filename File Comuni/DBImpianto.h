// ---------------------------------------------------------------------------

#ifndef DBImpiantoH
#define DBImpiantoH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>

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
    void TornaPosDepLibera(AnsiString Zona, int &pos, int &piano, int eccezione=0);
    int PianiOccupatiPerPos(int pos);
    void PrelievoVuoti(int idzona, int &pos, int &pianiocc);
    int TornaPosVuoteZona(int idzona);
    int CercaPrelievoH();
    int AggiornaUDCPosizioni(int pos, int IDUDC, int piano=0) ;
    int CercaPrelievo(AnsiString Zona, int tipoposizione=0)   ;

};

// ---------------------------------------------------------------------------
extern PACKAGE TdmDBImpianto *dmDBImpianto;
// ---------------------------------------------------------------------------
#endif
