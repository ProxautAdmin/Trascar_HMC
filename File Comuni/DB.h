// ---------------------------------------------------------------------------

#ifndef DBH
#define DBH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <ExtCtrls.hpp>
#include <Data.DB.hpp>
#include <map>
#include <vector>
#include "clientdata.h"
#include <map>
#include <vector>
#define NCAMPI	5
// ---------------------------------------------------------------------------

typedef struct {
	int posprel;
	int piano_prel;
	int h_prel;
	int tipo_pallet;
	int destinazione;
	int posdep;
	int piano_dep;
	int h_dep;
	int Agv_associati;
	int Agv;
	int tipo_mis;
	int priorita;
	int idcentromissioni;
	int idgestionale;
	int idudc;
	int idudc2;
	int tipologia_mov;
	int tipologia_mis;
	int tipoudc;
	int AltPosPrel;
	int AltPosDep;
	int nodopassaggioprel;
    int nodopassaggiodep;
	int Lotto;
	AnsiString posprelcliente;
	AnsiString posdepcliente;
    int latoforcheprel;
    int latoforchedep;
    // mod corsia
    int corsia_prel;
    int corsia_dep;

    int da_etichettare_m;

    int altezza_udc;
} TMissione;

typedef std::map<int, std::map<AnsiString, AnsiString> >TRecordList;
typedef std::map<AnsiString, std::map<AnsiString, AnsiString> >TIndexList;
typedef std::map<AnsiString, std::map<AnsiString, AnsiString> >::iterator IndexListIterator;

// ---------------------------------------------------------------------------
class TdmDB : public TDataModule
{
__published: // IDE-managed Components

	TADOConnection *ADOConnection1;
	TTimer *TimerConnect;
	TADOQuery *QueryMissioniSocket;
	TADOTable *AlarmTable;
	TADOTable *MissTable;
	TADOQuery *QueryNewAlarm;
	TADOQuery *QueryPos;
	TADOQuery *QueryMappa;
	TADOQuery *QueryCorsie;
	TTimer *TimerPassword;
	TADOQuery *QueryUDC;
	TADOQuery *QueryMessAlarm;
	TADOQuery *QueryPeso;
	TADOQuery *QueryTipologie;

	TADOConnection *ADOConnectionExtra;
	TADOQuery *QueryCentroMissioniSocket;
	TADOQuery *QueryTabelle;

	void __fastcall TimerConnectTimer(TObject *Sender);
	void __fastcall ADOConnection1AfterConnect(TObject *Sender);
	void __fastcall ADOConnection1ConnectComplete(TADOConnection *Connection, Error * const Error,
		TEventStatus &EventStatus);
	void __fastcall TimerPasswordTimer(TObject *Sender);

private: // User declarations
		public : // User declarations
	__fastcall TdmDB(TComponent* Owner);

	struct StrutturaStringa {
		int ID;
		int Tipo;
		AnsiString Nome;
		int StartString;
		int LenString;
	};

	StrutturaStringa Struttura[50];

	struct StrutturaParametriFunzionali {
		int TempoRicarica;
		int MescolaCeste;
		int zonascelta;

        int altezza_pallet_basso;
	};

	StrutturaParametriFunzionali ParametriFunzionali;

	void PuliziaStorici();
	void ConnettiDB();
	void DisconnettiDB();
	int verificaConnessioneDB();
	AnsiString AggiungiRimuoviSecondiAOrario(AnsiString ora, int secondi, bool rimuovi);
	AnsiString AggiungiRimuoviSecondiADataOra(AnsiString ora, int secondi, bool rimuovi);
	void CaricaTabella(AnsiString TableName, TRecordList &RecList);
	void CaricaTabellaK(AnsiString TableName, AnsiString KeyField, TIndexList &RecList);
	int AggiornaCampoTabella(AnsiString Tabella, AnsiString CampoChiave, int ValoreChiave, AnsiString CampoModifica, AnsiString ValoreModifica, bool isstring);
	int LeggiCampoTabella(AnsiString Tabella, AnsiString CampoDaLeggere, AnsiString ClauslaWhere, AnsiString &Valore);
	void Segnalazione(int n, AnsiString msg, int all, int popup = 0, int popupstation = 0);
	void LeggiSegnalazioneAttiva(AnsiString &msg, int &all);
	void AcquisisciSegnalazioni();
	void AcquisisciSegnalazioneAttiva(int nmsg);
	void AcquisisciSegnalazioneAttiva();
	void Log(AnsiString Sorgente, AnsiString Tipo, AnsiString Evento);
	void LogCom(AnsiString Telegramma);
	void Log_MISS(int agv, int posizione, int destinazione, AnsiString comando, AnsiString posbloccate);
	int AdattatoreRichiesto(AnsiString TipoMacchina, int ID_Macchina, int ID_TipoPallet);
	void LogMsg(AnsiString Descrizione);
	void LeggiParametri(int id_parametro = 0);
	void LeggiStatoAgv();
	void AggiornaStatoAgv(int agv);
	void GestioneAllarmi(int agv);
	void AggiornaAllarmiAttivi();
	void ImpostaAllarmiAttiviClient();
	int AggiornaParametri(int id_parametro, int val, AnsiString valstr = "");
	int AggiornaDestinazioneMissione(int id, int pos_dep);
	void SvuotaPos(int pos);
	int ReturnFilaDaPos(int pos);
	void SvuotaCorsia(int corsia);
	void ImpostaCodiceArticoloCorsia(int corsia, AnsiString Codice);
	void PrenotaCorsia(int corsia, int prenota);
	void PrenotaPos(int pos, int prenota, int corsiadaprenotare=0);
	void SettaCorsiaPiena(int corsia, int setta);
	void SettaCorsiaVuota(int corsia, int setta);
	int ContaPosizioniLiberePerTipoPallet(int tipologia);
	void SettaCorsiaInUso(int corsia, int prelievo, int setta, AnsiString CodiceArticolo = "", AnsiString Lotto = "");
	void AbilitaCorsia(int corsia, int setta);
	int CodicePresenteInArchivio(AnsiString CodiceArticolo);
	AnsiString ReturnZonaDaPos(int pos);
	AnsiString ReturnTimeDaDateTime(AnsiString DataOra);
	AnsiString ReturnTimeMission(int idmis);
	AnsiString RitornaOrarioDaSecondi(int secondi);
	int ReturnPosDaNomePosinPosizioni(AnsiString Nomepos);
	int ReturnPosDaNomePosinPiani(AnsiString Nomepos);
	int ControllaCorsiaAncoraPiena(int corsia);
	int UpdatePos(int pos, int prenota, int escludi, int IdTotem, int hprel, int hdep);
	int UpdateCorsia(int corsia, int prenota, int abilitata, int piena, int vuota, int inusoprel, int inusodep, int prior, int tipologia, int IgnoraRifOrdine, int depselettivo, AnsiString Annotazioni, AnsiString CodiceArticolo = "",
		AnsiString Lotto = "");
	void InsertUDCInPosizione(int UDC, int pos);
	AnsiString NomePosizione(int pos);
	AnsiString NomeCorsiaDaPosizione(int pos);
	AnsiString ZonaPosizione(int pos);
	AnsiString ReturnNomePosDaPos(int pos, AnsiString &nomecorsia);
	void ArticoloPrelevatoDepositato(int pos, int idudc, int piano, int corsia);
	AnsiString PosizioniRichiestaAperturaPorta(int porta);
	AnsiString PosizioniPortaAperta(int porta);
	AnsiString StrConsensoPorta(int porta);
	void InizializzaVariabili();
	int StatoPorta(int porta);
	int ReturnIdPortaDaStringaConsensoPorta(AnsiString str_consenso);
	int FilaPosizione(int pos);
	int ControllaUtentePassword(AnsiString Utente, AnsiString Password);
	int CreaIdTipologia();
	int ArticoloPresenteInArchivio(int idarticolo);
	int UpdatePosNoStato(int pos, int IdUDC, int hprel, int hdep, int piano);
	int UpdateSoloStato(int pos, int prenota, int escludi);
	void AggiornaTabPostazioni();
	void AggiornaTabPorte();
	void AggiornaStatoPorteStruttura();
	void AggiornaTabTipoArticoli();
	int ReturnIdTipologiaDaCodice(AnsiString codice);
	int RitornaTipologiaCorsia(int corsia);
	int ControllaDepositoOk(int pos, int posopposta, int mismanuale = 0);
	int ControllaPrelievoOk(int pos, int posopposta, int mismanuale = 0);
	int OrdinamentoFila(int corsia);
	int OrdinamentoFilaPosOpposta(int corsia);
	int ControllaMissioneOk(int mismanuale, int &pos_prel, int &pos_dep);
	int ReturnColoreTipologiaDaId(int idtipologia);
	void AggiornaTabAnagrafica();
	int CreaIdArticoloAnagrafica();
	int CreaIdArtUdc();
	int PosOpposta(int pos);
	AnsiString ReturnStringaHexDatiDaIdUdc(int idudc);
	int ResettaProduzioneInEsecuzione();
	int PosizionePrenotata(int pos);
	int CorsiaPrenotata(int corsia);
	int UDCPresenteInMagazzino(int idtotem);
	int RicercaUltimaMissioneInCorsoDaPosPrelAPosDep(int posprel, int posdep);
	int ControllaTotemCongelato(int idtotem);
	int CodicePresenteInAnagrafica(AnsiString Codice);
	void AggiornaTabTipologiaCorsia();
	AnsiString ReturnCodiceTipoUDCDaId(int idtipologia);
	AnsiString ReturnCodiceTipoArticoloDaId(int idtipologia);
    int RitornaTipologia(int pos) ;

	int CreaIdUDC();
	int IdUDCDaPosizione(int pos);
	int RitornaTipologiadaCorsia(int corsia);
	int InsertUpdateArticoloUDC(int id_articolo, int pos_dep, int IDArticolo, AnsiString hex_string, AnsiString CorsiaDeposito, int ultimoidudc, AnsiString Codice);
	int RitornaPosInputPlc(int pos);
	// int InviaCentroMissione(int pos, int posdep, int tipomissione, int agvdaspostare, int codtipomissione) ;
	int CreaIdCentroMissione();
	int GeneraCentroMissione(TCentroMissione CentroMis); // int pos_prel, int pos_dep, int tipo_mis, int agv, int priorita, int idUDC, int codtipomissione, AnsiString CorsiaDeposito, AnsiString id_transaktion = "", int posnr = 0);
	int ReturnUltimoIdUdcDaArticoloUdc(int IdArticoloUdc);
	int AggiornaUltimoIdUdcPerArticoloUdc(int IdArticoloUdc, int IdUdc);
	int AggiornaStatoUdc(int stato, int IdUdc);
	int CorsiaAbilitata(int corsia);
	int PosizioneEsclusa(int pos);
	// void SvuotaStruturaCentroMissioni(int agv);
	AnsiString FormattaDataOra(TDateTime DataOra);
	int RicercaDepositoUdcinCorsia(AnsiString NomeCorsia, int &piano);
	int ReturnUltimoIdUdceDestDaIdArticolo(int idarticolo, AnsiString &CorsiaDest);
	AnsiString ReturnStringaHexDatiDaIdArtUdc(int idartudc);
	void AggiornaTabTipoUdc();
	int AgvSuBaia(int baia);
	int AgvInPosCb(int pos);
	int AgvInDestCb(int agv);
	int AgvInPos(int pos);
	int AgvInDest(int pos);
	void AggiornaTabParametri();
	int ReturnCorsiaDaDestraDaPos(int pos);
	AnsiString ReturnCodiceArticoloDaIdArticolo(int idarticolo);
	AnsiString ReturnCodiceArticoloDaIdUdc(int idudc);
	int ReturnIdArticoloPrdottoDaIdArtUdc(int IdArticoloUdc);
	int PresenzaCentroMissione(int pos, int stato);
	int ContaPosizioniOccupateCorsia(AnsiString NomeCorsia, int Fila);
	int PrimoIdUdcTrovatoInCorsia(AnsiString NomeCorsia, int Fila);
	void InsertArticoloPosizione(int idarticolo, int idartudc);
	void AggiornaStrutturaZone();
	void EliminaUDCdaPosizione(int UDC);
	int UltimoIDMissione();
	AnsiString RitornaCodiceUDCdaUDC(int UDC);
	int ReturnCorsiaDaNomeCorsia(AnsiString nomecorsia);
	int RitornaAltezzedaPosizione(int pos, int piano, AnsiString stringaprelievo);
	int PrimoPianoOk(int pos);
	int UpdatePiano(int pos, int piano, int IdUDC, int hprel, int hdep);
	AnsiString ReturnNomePosDaPosePiano(int pos, int piano);
	AnsiString ReturnUdcDaPosePiano(int pos, int piano);
	int ReturnUltimoIdCentroMissione();
	AnsiString ControllaCorsiaDestinazione(AnsiString Corsia);
	AnsiString StrConsensoPortaTFuoco(int porta);
	AnsiString ReturnNomeCorsiaDaPos(int pos);
	int AggiornaTipoUdc(int tipo, int IdUdc);

	int RitornaTipoUDCdaBitechProxaut(int id);
	int ControllaSopraESotto(int pos, int piano);
	int SettaAllarmePos(int pos, int allarme);
	AnsiString RitornaCodiceUDCdaPosinMissione(int pos);
	void SettaRifOrdineSuPos(int pos, AnsiString RifOrdine);
	int RitornaPosPianoDaNomePos(AnsiString Nomepos, int &piano);
	int RitornaDepositoSelettivo(AnsiString Corsia);
	void SvuotaPiano(int pos, int piano);
	void MissioneElaborataSuAgvX(int idmiss, int causale);
	int PosPresenteMissioneAttiva(int pos);
	int RitornaZonaDaPos(int pos);
	int RitornaPosizioneDaPos(int pos);
	int ReturnIDUdcDaPosePiano(int pos, int piano);
	int RitornaNumPosDaPos(int pos);
	void SettaPosSelezionata(int pos, int setta, int zona = 0);
	int RitornaPosizioneSelezionata(int pos);
	int RitornaPosizioneDisabilitata(int pos);
	int RitornaPosizionePrenotata(int pos);
	void SettaPosPrenotata(int pos, int setta);
	void SettaDestinazioneInZona(int fila, int setta);
	int LeggiDestinazioneInZona(int fila);
	int ContaMissioniAttive(int agv = 0);
	void ImpostaChiamataPosizione(int pos, int stato);
	AnsiString NomePos(int pos);

	int RitornaOrdinamentoDaPos(int pos);

	int RitornaTipoUDCDaPos(int pos);
	int ControllaPosizioneOK(int pos, int piano);
	int ReturnPianoDaNomePosinPiani(AnsiString Nomepos);
	void SettaPianoSelezionato(int pos, int piano, int setta, int zona = 0);
	int RitornaUDCdaCodUDC(AnsiString CodUdc);
	void FullTabellaK(AnsiString TableName, AnsiString KeyField, TIndexList &RecList);
	void AggiornaNomePiani();
	int RitornaTipoPianoDaNomePosinPiani(AnsiString Nomepos) ;
	int RitornaUDCdaPosPiano(int pos, int piano) ;
    void AggiornaTabPosizioniUDC();
    int TipoPlc(int pos) ;
    int RitornaAltezzadaPosizioneNuovo(int pos, int piano, AnsiString stringaprelievo, int corsia);
    int UpdateAltezzaPiano(int pos, int piano, int corsia, int hprel, int hdep);
    int InsertUpdateUDCeArticolo(TUDC UDC, TArticoli &Articolo) ;
    int InsertUpdateUDC(TUDC UDC);
    int UDCPresenteInArchivio(int idudc, int &idarticolo) ;
    int InsertArticoli(TArticoli &Articoli) ;
    int LeggiParametro(int id_parametro) ;
    void FullTabella(AnsiString TableName, TRecordList &RecList);
    AnsiString DescrizioneArticolo(int IDArticolo) ;
    int IDUDCdaIDArticolo(int IDArticolo);
    void LeggiStrutturaUdc(TUDC & UDC) ;
    int IDUDCdaCodart(AnsiString CodArt) ;

	AnsiString Lingua;

	TDateTime t;
	/* TRecordList TabListaMissioni;
	 TRecordList TabListaMissioniAssegnate ; */
	TIndexList TabStrutturaZone, TabPostazioni, TabPorte, TipologiaArticoli, TabNomePiani, TabTipoUDC, TipologiaCorsia, TabParametri, TabMessaggiAllarme;
    TIndexList TabPosizioniUDC, TabArticoli;
	int peso_min, diametro_min, escludi_missioni, log_comunicazioni, lettostatoagv, lettoparametri;
	int tempo_prelievo, tempo_deposito, ultimo_id_articolo, pwdlevel, num_porte, quota_prel_ribalatatore, quota_prel_default, quota_dep_default;
	bool aggiornamentolista, aggiorna_tab_posizioni_locale, aggiorna_tab_porte_locale, aggiorna_tab_portefumo_locale, aggiorna_tab_tipologia_locale, aggiorna_tab_parametri;
	unsigned char bit[8], bitAnd[8];
	int last_corsia_udc, num_parametri;

	int agv_abilitati_per_missione[11], priorita_missioni[11], lista_cb[NAGV + 1], cb_statici, Abilita_Controllo_TabMovSpostamento, Abilita_Controllo_TabMovPesate;

	int abilitatracciaagv, abilitainfobox, nuovagestione;
	int PesoCampione, MinutiPesoCampione, PrioritaPesoCampione, PosPesoCampione, TolleranzaPesoCampione;
	AnsiString format_data_ora_database, ultima_data_priorita, ActualUser, PWD, NomeClientServer, StrTipoProdottoNULLSuPlc, CodiceCorsiaVascheVuoteNonLavate, CodiceCorsiaVascheVuoteLavate;
	AnsiString CodiceCorsiaVascheAutomatico, CodiceVascaVuota, DataOraChiamataRibaltatori[4];
	AnsiString UserNum, UserZona, ZonaString;
};

// ---------------------------------------------------------------------------
extern PACKAGE TdmDB *dmDB;
extern AnsiString logstr;
// ---------------------------------------------------------------------------
#endif
