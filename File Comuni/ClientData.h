// ---------------------------------------------------------------------------
#ifndef ClientDataH
#define ClientDataH
// ---------------------------------------------------------------------------
#define NAGV	1

#define PLC_PHOENIX	1   //impostato fisso = 3 in plcphoenixthread.cpp percio' controllare che non lo superi
#define PLC_MODBUS	0   //SISTEMARE ANCHE n_plc IN threadmodbusclient

#define NUM_PORTE	    1
#define NUM_POSTAZIONI  10
#define NUM_ZONE        1
#define NUM_SEMAFORI	2
#define NUM_PULSANTI    0
#define NUM_SENSORI     0
#define TEMPO_SENSORE   10
#define TEMPO_PULSANTE  2

#define NODOSTOP    1451
#define NODOSTOPIN  1404
#define NODOSTOPINOUT  1450

// ---------------------------------------------------------------------------
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
// ---------------------------------------------------------------------------

#define STRUTTURA_DATI_UDC_PLC	62

#define ALTEZZA_PRELIEVO			70
#define ALTEZZA_DEPOSITO			150

#define ESITO_MISSIONE_INTERMEDIA_COMPLETATA  3
#define ESITO_MISSIONE_COMPLETATA  	6
#define ESITO_MISSIONE_ANNULLATA	11

#define ID_MISSIONE_CREATA_DA_SUPERVISORE	2
#define ID_MISSIONE_INIZIATA_DA_AGV			3
#define ID_MISSIONE_PRELEVATA_DA_AGV		4
#define ID_MISSIONE_COMPLETATA  			5
#define ID_MISSIONE_ABORTITA_SENZA_PALLET   6
#define ID_MISSIONE_ABORTITA_CON_PALLET     7

// RICORDARSI DI SETTARLA anche NELLA PLCtHREAD!!!!!!!   #define	N_PLC 1
#define	N_PLC 1

#define NUMPIANI_VUOTI   10
#define NUMPIANI_PREL   1
#define NUMPIANI_DEP    1
#define NUMFILE 10
#define IDX_PLCPRELIEVO    1   //idx prelievo
#define IDX_PLCDEPOSITO    1   //idx Deposito
#define NUM_PRELIEVOPLC 1   //quanti prelievi
#define NUM_DEPOSITOPLC 2   //quanti depositi

#define TIPO_PLC 91// tipologia plc

#define TIPOLOGIA_DEPOSITO  10   // deposito
#define TIPOLOGIA_PRELIEVO  11   // prelievo
#define TIPOLOGIA_SCARICO   20
#define TIPOLOGIA_TERRA     1
#define TIPOLOGIA_SCARTO    2
#define TIPOLOGIA_PRODOTTOFINITO    3
#define TIPOLOGIA_MATERIEPRIME    4
#define TIPOLOGIA_PALLET    5

const AnsiString Impianto = "Trascar_HMC";
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
typedef struct {
    AnsiString zona;
    AnsiString nomecorsia;
    int indicefila; // per numerare la fila. Sara' indicefila*50
    int inizio;
    int n_alveoli;
    int numfile;
    int tipo_posizione;
    int ord_corsia; // ordinamento descrizione righe, 0= deposito dal numero piu' alto 1= deposito dal numero piu' basso
    int ord_corsia_opposta; // parto dal piu basso 121
    int dalbassoinalto; // 0: inverte ordine crs 1: segue ordine crs
    int step;
    int piani;
    int wshape;
    int hshape;
    int abilitata;
    int CorsiaInizioDx; // per disegno stato_pos =1 inizio a dx, =0 inizio a sx
    int dasxadx; // 0:numera da dx a sx. 1=da sx a dx
} TPosizioni;

typedef struct {
    int IDArticolo;
    char CodArt[30];
    char Descrizione[100];
    char Descrizione2[30];
    int IDTipoArticolo;
    int PezziPerPiano;
    int PianiPerUDC;
    int Peso;
} TArticoli;

typedef struct {
    int IDUDC;
    int CodTipoUDC;
    int IdArtUDC;
    TArticoli Articolo;
    char Lotto[20];
    int tara;
    int tara_pezzi;
    int CodStato;
    int pesoiniziale;
    int pesoattuale;
    int Riservato;
    int Parziale;
    int IndiceImpilabilita;
} TUDC;

typedef struct {
    int pos;
    int idmis;
    int dest;
    int posprec;
    int statoagv;
    int tipomis;
    int battbassa;
    int load;
    int manuale;
    int alarm;
    int generata;
    int incarica;
    char allarme[100];
    char allarmi[100];
    char allarmiPrec[100];
    int nodoa;
    int nodob;
    int richiestaconsenso;
    int consensodato;
    int primo_ciclo_consenso;
    int LivelloBatteria;
    int stand_by;
    int bumper_bypass;
    int registro5;
    int DestinazioneDaAGV; // tasti premuti su agv
    int bitbattbassa;
    int prel;
    int dep;
    int annullamissione;
    int semiautomatico;
    int allarme_consenso;
    int allarme_consenso_porta;
    int allarme_carico;
    int allarme_batteriabassa;
    int num_allarme_consenso;
    int num_allarme_consenso_porta;
    int num_allarme_carico;
    int peso_campione_non_corretto;
    int disegna_agv; // invio al client messaggio di ridisegnare l'agv
    int richiestaconsensonodo;
    int consensonododato;
    int stand_by_antincendio;
    int StandByOperatore;
    int sospeso;
    char str_consenso[100];
    int tipoAGV;
    int h_prel;
    int h_dep;
    char DataOraUltimaMissione[15];
    TUDC DatiUDC;
    int forchedx;
    int forchesx;
    int latoforcheprel;
    int latoforchedep;
    int nodopassaggioprel;
    int nodopassaggiodep;
    int fermaagv;
    int piano_prelievo;
    int piano_deposito;
    char op_richiestaconsensonodo;
    int forche_in_trazione;
    int da_etichettare;
    int standby_operatore;
    int altezza_pallet;
    int tipo_pallet;
} TAGV;

typedef struct {
    int tipomodulo; // 1:plc, 2:phoenix
    int id_thread_plc;
    int PlcConnesso;
    int DBInput;
    int DBOutput;
    int ByteInput; //
    int DW_Addr_PortaAperta;
    int DW_Addr_PortaChiusa;
    int DW_Addr_PortaTFuocoAperta;
    int DW_Addr_PortaReady;
    int DW_Addr_OkAccessoAgv;
    int Bit_PortaAperta;
    int Bit_PortaChiusa;
    int Bit_PortaTFuocoAperta;
    int Bit_PortaReady;
    int Bit_OkAccessoAgv;
    int DW_Addr_ApriPorta;
    int DW_Addr_ChiudiPorta;
    int DW_Addr_IngombroAGV;
    int DW_Addr_AGVInZona;
    int Bit_ApriPorta;
    int Bit_ChiudiPorta;
    int Bit_IngombroAGV;
    int Bit_AGVInZona;
    int Bit_Memoria_Apertura_Operatore;
    // modifico le uscite solo quando cambia l'intero byte
    int PortaAperta;
    int PortaChiusa;
    int PortaTFuoco;
    int PortaReady;
    int OkAccessoAgv;
    int ApriPorta;
    int ChiudiPorta;
    int IngombroAGV;
    int AGVInZona;
    int RichiestaAttiva;
    int Stato;
    int forzature_attive;
    int count_forzature;
    int id;
    int count_comando;
    int memoria_apertura_operatore;
    int tipoporta;
} TPorte;

struct Semaforo {
    int id;
    int tipomodulo; // 1:plc, 2:phoenix
    int id_thread_plc;
    int PlcConnesso;
    int DBOutput;
    int DW_Addr_LuceVerde;
    int DW_Addr_LuceRossa;
    int Bit_LuceVerde;
    int Bit_LuceRossa;
    int DW_Addr_richiesta_ingresso;
    int Bit_richiesta_ingresso;
    int richiestaIngresso;
    int posx;
    int posy;
    int height;
    int width;
    int LuceVerde;
    int LuceGialla;
    int LuceRossa;
    int Allarme;
    int Stato;
    int oggetto_creato;
    int forzatura_attiva;
};

struct TSensori {
    int id;
    int tipomodulo; // 1:plc, 2:phoenix
    int id_thread_plc;
    int PlcConnesso;
    int DBInput;
    int DBOutput;
    char Dati[60];
    int DW_Addr_Lettura;
    int Bit_Lettura;
    int DW_Addr_Scrittura;
    int Bit_Scrittura;
    int posx;
    int posy;
    int height;
    int width;
    int PresenzaAllarme;
    int Valore;
    int Stato;
    int oggetto_creato;
    int forzatura_attiva;
};

struct ModuloPhoenix {
    int forzatura_attiva;
    int pos;
    int ChiamataPrelievo;
    int ChiamataTelaioPieno;
    int ChiamataDeposito;
    int generata;
    int esclusa;
    int porta_aperta[11];
    int addr_porta_aperta[11];
    int bit_porta_aperta[11];
    int apri_porta[11];
    int addr_apri_porta[11];
    int bit_apri_porta[11];
    int chiudi_porta[11];
    int addr_chiudi_porta[11];
    int bit_chiudi_porta[11];
    int richiesta_porta_attiva[11];
    int stato_porta[11]; // esclusione, manuale,automatica ecc..
    int PlcConnesso;
    int input[10];
    int output[10];
    int forzatura_input;
    int test_lampade;
    int count_forzatura;
    // AnsiString InputName[5][8],OutputName[5][8] ;
};

// struttura centromissioni
typedef struct {
    int posprel;
    int posdep;
    int h_prel;
    int h_dep;
    int Agv;
    int IDUDC;
    int TipoMissione;
    int Priorita;
    int CodTipoMovimento;
    int CodTipoMissione;
    int FilaInCorsiaDeposito;
    int FilaInCorsiaPrelievo;
    int TipoUDC;
    int pianoprel;
    int pianodep;
    int stato;
    int DestinazioneModuli;
    int TabellaDeposito;
    AnsiString CorsiaDeposito;
    AnsiString ZonaPrelievo;
    AnsiString ZonaDeposito;
} TCentroMissione;

struct StrutturaDatiPulsante {
    int dbInput;
    int byteInput;
    int byteOutput;
    int bitInput;
    int bitOutput;
    int pos;

};

struct DatiPostazione {
    int dbInput;
    int byteInput;
    int byteOutput;
    int bitAGVFuoriIngombro;
    int bitUDCPrelevato;
    int generata;
    int generataCM;
    int esclusa;
    char nomepos[20];
    int pos;
    int piano;
    int indice;
    int tipo;
    int Pulsante; // ingresso segnale
    int StatoPulsante; // 0:spento 1:acceso 2:lampeggio
    int ContatorePulsante;
    StrutturaDatiPulsante DatiPulsante;
    int contatore;
    int selezionato;
    // in
    int presenzaUDC;
    char CodUDC[20];
    // out
    int AGVFuoriIngombro;
    int UDCPrelevato;
    //
    int uscite, uscitedascrivere;
    int IDUDC;
};

struct DatiZona {
    // mettere identificatore se prelievo o deposito
    int id_thread_plc;
    int watchdog_plc;
    int watchdog_pc;
    int PlcConnesso;
    int CycleTime;
    int Error;
    int forzature_attive;
    int count_forzature;
    int identificativo_struttura;
    int numpostazioni;
    int TipoUDC;
    int postazioneStart;

    DatiPostazione Postazione[NUM_POSTAZIONI + 1];
};

struct StrutturaParametriFunzionali {
    int PosAdiacenti;
    int NumMissioniLimite;
    int PosUnload;
    int SecondiEmail;
    int NodoEscluso;
    int AbilitaMissioni;
    int Lastidudc;
    int Simula;
    int Giorno;
    int TempoRicarica;
    int MescolaCeste;
    int zonascelta;
    int altezza_pallet;
};

struct DatiPrelievo {
    int idplc;
    int dbInput;
    int dbOutput;

    int byteInput;
    int byteOutput;
    int TagIn;
    int TagOut;
    int count;
    char Dati[6]; // da eliminare leggendo StatoInput. Da tenere gestito per gli output
    int StatoInput[99]; // tutti i byte d'ingresso. Lavoro sempre qui, sia che si tratti di input reali che simulati
    int generata;
    int esclusa;
    int selezionata;
    int pos;
    int pos_fuori_ingombro;
    int numpiani;
    int TipoUDC;
    // in
    int bit_Ready;
    int Ready;
    int bit_InAllarme;
    int InAllarme;
    int bit_ProntaAlPrelievo;
    int ProntaAlPrelievo;
    int bit_OkPrelievo;
    int OkPrelievo;
    int PalletID;
    // out
    int bit_RichiestaAbilitaPrel;
    int RichiestaAbilitaPrel;
    int bit_AGVInIngombro;
    int AGVInIngombro;
    //
    int uscite[4], uscitedascrivere[4];
    int idudc;
    int codartudc;
};

struct DatiDeposito {
    int idplc;
    int dbInput;
    int dbOutput;

    int byteInput;
    int byteOutput;
    int TagIn;
    int TagOut;
    char Dati[6]; // da eliminare leggendo StatoInput. Da tenere gestito per gli output
    int StatoInput[99]; // tutti i byte d'ingresso. Lavoro sempre qui, sia che si tratti di input reali che simulati
    int generata;
    int esclusa;
    int selezionata;
    int pos;
    int pos_fuori_ingombro;
    int numpiani;
    int piano;
    int TipoPosPrelievo; // 1:con tasti,2 con DB
    // in
    int bit_Ready;
    int Ready;
    int bit_InAllarme;
    int InAllarme;
    int bit_ProntaAlDeposito;
    int ProntaAlDeposito;
    int bit_OkDeposito;
    int OkDeposito;
    // out
    int bit_RichiestaAbilitaDep;
    int RichiestaAbilitaDep;
    int bit_AGVInIngombro;
    int AGVInIngombro;
    int PalletID;
    //
    int uscite[4], uscitedascrivere[4];
    int count_fine_carico;
    int idudc;
    int codartudc;
};

struct DatiSegnaliGenerali {
    int dbInput;
    int dbOutput;

    char Dati[6]; // da eliminare leggendo StatoInput. Da tenere gestito per gli output
    int StatoInput[20]; // tutti i byte d'ingresso. Lavoro sempre qui, sia che si tratti di input reali che simulati

    // out
    int byteOutput_accesso;
    int bitOutput_conferma_accesso;
    int Output_conferma_accesso;
    // in
    int byteInput_accesso;
    int bitInput_conferma_accesso;
    int Input_conferma_accesso;

    int bitInput_accesso_terminato;
    int Input_accesso_terminato;
    //
    int uscite[4], uscitedascrivere[4];
    int count_fine_carico;
    int idudc;
    int codartudc;
};

struct DatiPlc {
    int id_thread_plc;

    int watchdog_plc;
    int watchdog_pc;
    int bit_watchdog_pc;
    int bit_watchdog_plc;

    int watchdog_pc_da_scrivere;

    int PlcConnesso;
    int CycleTime;
    int Error;
    int Zona;

    int forzature_attive;
    int count_forzature;
    int identificativo_struttura;

    DatiPrelievo Prelievo[NUM_PRELIEVOPLC + 1][NUMPIANI_PREL + 1];
    DatiDeposito Deposito[NUM_DEPOSITOPLC + 1][NUMPIANI_DEP + 1];

    DatiSegnaliGenerali SegnaliGenerali[2];
};

// ---------------------------------------------------------------------------
typedef struct {
    // Inizio pacchetto per sincronismo trasmissione dati

    char Sync[5];
    // Generali
    int ridisegna_mappa;
    int aggiorna_allarmi;
    int aggiorna_tab_posizioni;
    int aggiorna_tab_porte;
    int aggiorna_tab_tipologia;
    int aggiorna_tab_parametri;

    // semafori
    Semaforo DatiSemafori[NUM_SEMAFORI + 1];
    // Sensori
    // TSensori Sensori[NUM_SENSORI + 1];
    // Porte
    TPorte DatiPorte[NUM_PORTE + 1];
    // AGV
    TAGV DatiAgv[NAGV + 1];
    DatiZona Zona[NUM_ZONE + 1];
    // PLC
    DatiPlc Plc[N_PLC + 1];
    StrutturaParametriFunzionali ParametriFunzionali;
} TClientData;

// ---------------------------------------------------------------------------
extern TClientData ClientData;

// ---------------------------------------------------------------------------
#endif
