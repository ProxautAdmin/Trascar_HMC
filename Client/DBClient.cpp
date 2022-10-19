// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBClient.h"
#include "dmFunzioniComuniClientServer.h"
#include "clientdata.h"
#include "ExtraFunction.h"
#include "main.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TdmDBClient *dmDBClient;
extern int indice_inizio_campi[NCAMPI];

// ---------------------------------------------------------------------------
__fastcall TdmDBClient::TdmDBClient(TComponent* Owner)
    : TDataModule(Owner)
{
}

void TdmDBClient::InserisciCorsiePostazioniDatabase() {
    // per postazioni singole o singole file
    AnsiString strsql, nomecorsia, zona, poscliente, nomepiano, nomecorsia_table;
    int pos_opposta, pos_in_fila_opposta, tipologia, res, i, j, k, z, fila, verticale, CorsiaInizioDx, wshape, hshape, n_file_corsia, n_alveoli, pos_prec, next_pos, posopposta, pos;
    int numerofila, pos_in_fila, posinputplc, startfila, ord_corsia, ord_corsia_opposta, tipo_posizione, numfile, step, inizio, piani, dalbassoinalto;
    int numfilastart, fine, idzona, tipoposizione;
    int hprel, hdep;
    int h_prel[] = {70, 580, 1100};
    int h_dep[] = {450, 720, 1230};
    int Plc_In[] = {210, 220, 300};
    int Plc_Out[] = {200, 310};
    int abilitata = 0, k_pos, dasxadx, pos020;
    TADOQuery *ADOQuery;
    if (Application->MessageBox(L"Vuoi creare le posizioni?", L"Conferma!!!", MB_YESNO) == IDYES) {
        try {
            if (!dmDB->ADOConnection1->Connected)
                return;

            ADOQuery = new TADOQuery(NULL);
            ADOQuery->Connection = dmDB->ADOConnection1;
            ADOQuery->SQL->Clear();

            for (i = 1; i <= 10; i++) {
                switch (i) {
                case 1:
                    // Zona A
                    idzona = 1;
                    zona = "A";
                    nomecorsia = "A";
                    inizio = 100; // far partire da pos=pos-1
                    startfila = 20; // imppostazione partenza fila manuale
                    n_alveoli = 7;
                    numfile = 1;
                    step = 1;
                    fine = inizio + (numfile * n_alveoli); // solo per la cancellazione dei piani. Semmai settare a mano
                    tipo_posizione = TIPOLOGIA_PRODOTTOFINITO;
                    posinputplc = 0;
                    ord_corsia = 0; // ordinamento descrizione righe , 0= parto dal numero più basso
                    ord_corsia_opposta = 1; // parto dal piu basso 121
                    dalbassoinalto = 0; // 0: inverte ordine crs 1: segue ordine crs     NB:se orizzontale e' dxsx
                    piani = 1;
                    wshape = 35;
                    hshape = 20;
                    abilitata = 1;
                    CorsiaInizioDx = 0; // per disegno stato_pos =1 inizio a dx, =0 inizio a sx     NB se orizzontale e' bassoalto
                    break;
                case 2:
                    // Zona I
                    idzona = 9;
                    zona = "I";
                    nomecorsia = "I";
                    inizio = 200; // far partire da pos=pos-1
                    startfila = 80; // imppostazione partenza fila manuale
                    n_alveoli = 10;
                    numfile = 1;
                    step = 1;
                    fine = inizio + (numfile * n_alveoli); // solo per la cancellazione dei piani. Semmai settare a mano
                    tipo_posizione = TIPOLOGIA_MATERIEPRIME;
                    posinputplc = 0;
                    ord_corsia = 0; // ordinamento descrizione righe , 0= parto dal numero più basso
                    ord_corsia_opposta = 1; // parto dal piu basso 121
                    dalbassoinalto = 0; // 0: inverte ordine crs 1: segue ordine crs     NB:se orizzontale e' dxsx
                    piani = 1;
                    wshape = 35;
                    hshape = 20;
                    abilitata = 1;
                    CorsiaInizioDx = 0; // per disegno stato_pos =1 inizio a dx, =0 inizio a sx     NB se orizzontale e' bassoalto
                    break;
                case 3:
                    // Zona G
                    idzona = 7;
                    zona = "G";
                    nomecorsia = "G";
                    inizio = 300; // far partire da pos=pos-1
                    startfila = 50; // imppostazione partenza fila manuale
                    n_alveoli = 13;
                    numfile = 1;
                    step = 1;
                    fine = inizio + (numfile * n_alveoli); // solo per la cancellazione dei piani. Semmai settare a mano
                    tipo_posizione = TIPOLOGIA_PALLET;
                    posinputplc = 0;
                    ord_corsia = 0; // ordinamento descrizione righe , 0= parto dal numero più basso
                    ord_corsia_opposta = 1; // parto dal piu basso 121
                    dalbassoinalto = 0; // 0: inverte ordine crs 1: segue ordine crs     NB:se orizzontale e' dxsx
                    piani = 10;
                    wshape = 35;
                    hshape = 20;
                    abilitata = 1;
                    CorsiaInizioDx = 0; // per disegno stato_pos =1 inizio a dx, =0 inizio a sx     NB se orizzontale e' bassoalto
                    break;
                case 4:
                    // Zona H
                    idzona = 8;
                    zona = "H";
                    nomecorsia = "H";
                    inizio = 400; // far partire da pos=pos-1
                    startfila = 100; // imppostazione partenza fila manuale
                    n_alveoli = 4;
                    numfile = 1;
                    step = 1;
                    fine = inizio + (numfile * n_alveoli); // solo per la cancellazione dei piani. Semmai settare a mano
                    tipo_posizione = TIPOLOGIA_TERRA;
                    posinputplc = 0;
                    ord_corsia = 0; // ordinamento descrizione righe , 0= parto dal numero più basso
                    ord_corsia_opposta = 1; // parto dal piu basso 121
                    dalbassoinalto = 0; // 0: inverte ordine crs 1: segue ordine crs     NB:se orizzontale e' dxsx
                    piani = 2;
                    wshape = 25;
                    hshape = 25;
                    abilitata = 1;
                    CorsiaInizioDx = 0; // per disegno stato_pos =1 inizio a dx, =0 inizio a sx     NB se orizzontale e' bassoalto
                    break;
                case 5:
                    // Zona D
                    idzona = 4;
                    zona = "D";
                    nomecorsia = "D";
                    inizio = 500; // far partire da pos=pos-1
                    startfila = 200; // imppostazione partenza fila manuale
                    n_alveoli = 12;
                    numfile = 5;
                    step = 20;
                    fine = inizio + (numfile * n_alveoli); // solo per la cancellazione dei piani. Semmai settare a mano
                    tipo_posizione = TIPOLOGIA_TERRA;
                    posinputplc = 0;
                    ord_corsia = 0; // ordinamento descrizione righe , 0= parto dal numero più basso
                    ord_corsia_opposta = 1; // parto dal piu basso 121
                    dalbassoinalto = 0; // 0: inverte ordine crs 1: segue ordine crs     NB:se orizzontale e' dxsx
                    piani = 1;
                    wshape = 25;
                    hshape = 25;
                    abilitata = 1;
                    CorsiaInizioDx = 0; // per disegno stato_pos =1 inizio a dx, =0 inizio a sx     NB se orizzontale e' bassoalto
                    break;
                case 6:
                    // Zona F
                    idzona = 6;
                    zona = "F";
                    nomecorsia = "F";
                    inizio = 700; // far partire da pos=pos-1
                    startfila = 500; // imppostazione partenza fila manuale
                    n_alveoli = 2;
                    numfile = 1;
                    step = 1;
                    fine = inizio + (numfile * n_alveoli); // solo per la cancellazione dei piani. Semmai settare a mano
                    tipo_posizione = TIPOLOGIA_TERRA;
                    posinputplc = 0;
                    ord_corsia = 0; // ordinamento descrizione righe , 0= parto dal numero più basso
                    ord_corsia_opposta = 1; // parto dal piu basso 121
                    dalbassoinalto = 0; // 0: inverte ordine crs 1: segue ordine crs     NB:se orizzontale e' dxsx
                    piani = 1;
                    wshape = 25;
                    hshape = 25;
                    abilitata = 1;
                    CorsiaInizioDx = 0; // per disegno stato_pos =1 inizio a dx, =0 inizio a sx     NB se orizzontale e' bassoalto
                    break;
                case 7:
                    // Zona J (A2)
                    idzona = 10;
                    zona = "J";
                    nomecorsia = "J";
                    inizio = 110; // far partire da pos=pos-1
                    startfila = 30; // imppostazione partenza fila manuale
                    n_alveoli = 9;
                    numfile = 1;
                    step = 1;
                    fine = inizio + (numfile * n_alveoli); // solo per la cancellazione dei piani. Semmai settare a mano
                    tipo_posizione = TIPOLOGIA_MATERIEPRIME;
                    posinputplc = 0;
                    ord_corsia = 0; // ordinamento descrizione righe , 0= parto dal numero più basso
                    ord_corsia_opposta = 1; // parto dal piu basso 121
                    dalbassoinalto = 0; // 0: inverte ordine crs 1: segue ordine crs     NB:se orizzontale e' dxsx
                    piani = 1;
                    wshape = 35;
                    hshape = 20;
                    abilitata = 1;
                    CorsiaInizioDx = 0; // per disegno stato_pos =1 inizio a dx, =0 inizio a sx     NB se orizzontale e' bassoalto
                    break;
                case 8:
                    // Zona C    uscita
                    idzona = 3;
                    zona = "C";
                    nomecorsia = "C";
                    inizio = 600; // far partire da pos=pos-1
                    startfila = 300; // imppostazione partenza fila manuale
                    n_alveoli = 1;
                    numfile = 1;
                    step = 1;
                    fine = inizio + (numfile * n_alveoli); // solo per la cancellazione dei piani. Semmai settare a mano
                    tipo_posizione = TIPO_PLC;
                    posinputplc = TIPOLOGIA_PRELIEVO;
                    ord_corsia = 0; // ordinamento descrizione righe , 0= parto dal numero più basso
                    ord_corsia_opposta = 1; // parto dal piu basso 121
                    dalbassoinalto = 0; // 0: inverte ordine crs 1: segue ordine crs     NB:se orizzontale e' dxsx
                    piani = 1;
                    wshape = 25;
                    hshape = 25;
                    abilitata = 1;
                    CorsiaInizioDx = 0; // per disegno stato_pos =1 inizio a dx, =0 inizio a sx     NB se orizzontale e' bassoalto
                    break;
                case 9:
                    // Zona B    ingresso
                    idzona = 2;
                    zona = "B";
                    nomecorsia = "B";
                    inizio = 601; // far partire da pos=pos-1
                    startfila = 301; // imppostazione partenza fila manuale
                    n_alveoli = 1;
                    numfile = 1;
                    step = 1;
                    fine = inizio + (numfile * n_alveoli); // solo per la cancellazione dei piani. Semmai settare a mano
                    tipo_posizione = TIPO_PLC;
                    posinputplc = TIPOLOGIA_DEPOSITO;
                    ord_corsia = 0; // ordinamento descrizione righe , 0= parto dal numero più basso
                    ord_corsia_opposta = 1; // parto dal piu basso 121
                    dalbassoinalto = 0; // 0: inverte ordine crs 1: segue ordine crs     NB:se orizzontale e' dxsx
                    piani = 1;
                    wshape = 25;
                    hshape = 25;
                    abilitata = 1;
                    CorsiaInizioDx = 0; // per disegno stato_pos =1 inizio a dx, =0 inizio a sx     NB se orizzontale e' bassoalto
                    break;
                case 10:
                    // Zona E    ingresso
                    idzona = 5;
                    zona = "E";
                    nomecorsia = "E";
                    inizio = 602; // far partire da pos=pos-1
                    startfila = 302; // imppostazione partenza fila manuale
                    n_alveoli = 1;
                    numfile = 1;
                    step = 1;
                    fine = inizio + (numfile * n_alveoli); // solo per la cancellazione dei piani. Semmai settare a mano
                    tipo_posizione = TIPO_PLC;
                    posinputplc = TIPOLOGIA_DEPOSITO;
                    ord_corsia = 0; // ordinamento descrizione righe , 0= parto dal numero più basso
                    ord_corsia_opposta = 1; // parto dal piu basso 121
                    dalbassoinalto = 0; // 0: inverte ordine crs 1: segue ordine crs     NB:se orizzontale e' dxsx
                    piani = 1;
                    wshape = 25;
                    hshape = 25;
                    abilitata = 1;
                    CorsiaInizioDx = 0; // per disegno stato_pos =1 inizio a dx, =0 inizio a sx     NB se orizzontale e' bassoalto
                    break;

                }
                ADOQuery = new TADOQuery(NULL);
                ADOQuery->Connection = dmDB->ADOConnection1;
                ADOQuery->SQL->Clear();
                ADOQuery->SQL->Append("delete from Corsie where  nomestanza = '" + nomecorsia + "'");
                res = ADOQuery->ExecSQL();
                ADOQuery->Close();
                ADOQuery->SQL->Clear();
                ADOQuery->SQL->Append("delete from Posizioni where idzona = " + IntToStr(idzona));
                res = ADOQuery->ExecSQL();
                ADOQuery->Close();
                ADOQuery->SQL->Clear();
                ADOQuery->SQL->Append("delete from Piani where (pos>=" + String(inizio + 1) + " and pos<=" + String(fine) + ")");
                res = ADOQuery->ExecSQL();
                ADOQuery->Close();
                tipologia = posinputplc;
                verticale = 1;

                pos_opposta = 0;
                for (j = 0; j < numfile; j++) {
                    if (dalbassoinalto > 0)
                        numerofila = numfile - (j + 1);
                    else
                        numerofila = j;
                    numerofila = numerofila + startfila;

                    nomecorsia_table = zona + nomecorsia + dmExtraFunction->PadS((numerofila) * 10, 3, "0") + "00";
                    strsql.printf("INSERT INTO Corsie "
                        "(Fila,NomeCorsia,Abilitata,InUsoPrelievo,CorsiaPrenotata,Piena,Vuota,InUsoDeposito,Priorita,NumPos,Tipologia,OrdinamentoCorsia,OrdinamentoCorsiaOpposta"
                        ",CodArt,AutoTipologia,NomeStanza,FilaInCorsia,CorsiaInizioDx) "
                        "VALUES (%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d"
                        ",%d,%d,'%s',%d,%d)"
                        , (numerofila)
                        , nomecorsia_table.c_str()
                        , abilitata
                        , 0
                        , 0
                        , 0
                        , 0
                        , 0
                        , 0
                        , n_alveoli
                        , tipologia
                        , ord_corsia
                        , ord_corsia_opposta
                        , 0
                        , 0
                        , nomecorsia.c_str()
                        , (numerofila - startfila)
                        , CorsiaInizioDx
                        );
                    ADOQuery->SQL->Clear();
                    ADOQuery->SQL->Text = strsql;
                    res = ADOQuery->ExecSQL();
                    ADOQuery->Close();

                    pos_in_fila = n_alveoli;
                    pos_in_fila_opposta = 0;
                    pos_prec = 0;
                    next_pos = 0; // da remmare quando lo si gestira'
                    for (k = 1; k <= n_alveoli; k++) {
                        // con kk giro il nome al contrario iniziando dal piu' alto
                        if (dasxadx == 1)
                            k_pos = (n_alveoli + 1) - k;
                        else
                            k_pos = k;

                        pos = (inizio + k_pos) + (j * step);
                        poscliente = zona + nomecorsia + dmExtraFunction->PadS((numerofila) * 10, 3, "0") + dmExtraFunction->PadS(k, 2, "0");
                        if ((i == 2) && (k >= n_alveoli-2)) {
                            tipoposizione = TIPOLOGIA_SCARTO;
                        }
                        else if ((i == 7) && (k == n_alveoli)) {
                            tipoposizione = TIPOLOGIA_SCARTO;
                        }
                        else {
                            tipoposizione = tipo_posizione;
                        }

                        strsql.printf("INSERT INTO Posizioni "
                            "(Pos,PosOpposta,CustomerPos,Prenotata,Disabilita,TipoPosizione"
                            ",Height,Width,Fila,PrevPosition"
                            ",NextPosition,PosInFila,PosInFilaOpposta,IDZona,PosInputPlc)"
                            " VALUES (%d,%d,'%s',%d,%d,%d,"
                            "%d,%d,%d,%d,"
                            "%d,%d,%d,%d,%d)"
                            , pos
                            , pos_opposta
                            , poscliente.c_str()
                            , 0
                            , 0
                            , tipoposizione
                            , hshape
                            , wshape
                            // , nomecorsia_table.c_str()
                            , numerofila
                            , pos_prec
                            , next_pos
                            , pos_in_fila
                            , pos_in_fila_opposta
                            , idzona
                            , posinputplc
                            );
                        ADOQuery->SQL->Clear();
                        ADOQuery->SQL->Text = strsql;
                        res = ADOQuery->ExecSQL();
                        ADOQuery->Close();
                        // pos_prec=pos;

                        // piani
                        for (z = 1; z <= piani; z++) {
                            nomepiano = zona + nomecorsia + dmExtraFunction->PadS((numerofila) * 10, 3, "0") + dmExtraFunction->PadS(z + (k - 1) * piani, 2, "0");
                            if (i == 3) {
                                hprel = (144 * z) - 40;
                                hdep = (144 * z) + 150;
                            }
                            else {
                                hprel = h_prel[z - 1];
                                hdep = h_dep[z - 1];
                            }
                            strsql.printf("INSERT INTO piani "
                                "(piano,pos,idudc,nomepos,hprel,hdep"
                                ",prenotata,disabilitata)"
                                " VALUES (%d,%d,%d,'%s',%d,%d,%d,%d)"
                                , z
                                , pos
                                , 0
                                , nomepiano.c_str()
                                , hprel
                                , hdep
                                , 0
                                , 0
                                );
                            ADOQuery->SQL->Clear();
                            ADOQuery->SQL->Text = strsql;
                            res = ADOQuery->ExecSQL();
                            ADOQuery->Close();
                        }

                    }
                }
            }
            ADOQuery->SQL->Clear();
            ADOQuery->Close();

        }
        catch (...) {
        }
        delete ADOQuery;
        Application->MessageBox(L"Creazione posizioni effettuata", L"Lavoro terminato", MB_OK);
    }
}

void TdmDBClient::InserisciPLCDatabase() {
    // per postazioni singole o singole file
    AnsiString strsql, nomecorsia, zona, poscliente, nomepiano, nomecorsia_table, stringapos;
    AnsiString stringadel;
    int pos_opposta, pos_in_fila_opposta, tipologia, res, i, j, k = 1, z, fila, verticale, CorsiaInizioDx, wshape, hshape, n_file_corsia, n_alveoli, pos_prec, next_pos, posopposta, pos;
    int numerofila, pos_in_fila, posinputplc, startfila, ord_corsia, ord_corsia_opposta, tipo_posizione, numfile, step, inizio, piani, dalbassoinalto;
    int numfilastart, fine, lato, nodofermata, deltax, deltay, tipopiano = 0, tabella = 0, varavanti;
    int kstart, kend;
    int pianistart, pianiend, tab_car, tab_scar;
    int h_prel[] = {40, 2640, 3520, 5260, 0, 0, 0, 0};
    int h_dep[] = {80, 2680, 3560, 5300, 0, 0, 0, 0};
    int tab_carico[] = {0, 0, 1, 2, 3, 4, 5, 6, 7};
    int tab_scarico[] = {0, 0, 1, 2, 3, 4, 5, 6, 7};
    ///
    int h_prel_rulliere_deposito[] = {1770, 4000, 6380};
    int h_dep_rulliere_deposito[] = {1770, 4000, 6380};
    ///
    int h_dep_rulliere_deposito_3a8[] = {1770, 3800, 5900}; // mod 15/02/22 3850->3800
    int h_prel_rulliere_deposito_3a8[] = {1770, 3800, 5900}; // mod 15/02/22 3850->3800
    ///
    int h_prel_picking[] = {1300}; // mod 15/02/22 1400->1300
    int h_dep_picking[] = {1300}; // mod 15/02/22 1400->1300
    ///
    int h_prel_baia_prelievo_101[] = {1341};
    int h_dep_baia_prelievo_101[] = {1341};
    ///
    int h_prel_baia_prelievo_20[] = {1580};
    int h_dep_baia_prelievo_20[] = {1580};
    ///
    int hprelterra, hdepterra;
    int abilitata = 0, k_pos, dasxadx, indice = 0;
    TADOQuery *ADOQuery;
    if (Application->MessageBox(L"Vuoi creare i PLC?", L"Conferma!!!", MB_YESNO) == IDYES) {
        try {
            if (!dmDB->ADOConnection1->Connected)
                return;

            ADOQuery = new TADOQuery(NULL);
            ADOQuery->Connection = dmDB->ADOConnection1;
            ADOQuery->SQL->Clear();

            for (i = 1;
                i <= 5;
                i++) {
                switch (i) {
                case 1:
                    zona = "DP"; // deposito rulliere
                    nomecorsia = "04-";
                    inizio = 0; // far partire da pos=pos-1
                    startfila = 20; // imppostazione partenza fila manuale
                    n_alveoli = 1;
                    numfile = 8;
                    step = n_alveoli;
                    tipo_posizione = 1;
                    ord_corsia = 0; // ordinamento descrizione righe , 0= parto dal numero più basso
                    ord_corsia_opposta = 1; // parto dal piu basso 121
                    dalbassoinalto = 0; // 0: inverte ordine crs 1: segue ordine crs     NB:se orizzontale e' dxsx
                    piani = 3;
                    wshape = 1250;
                    hshape = 1100;
                    abilitata = 1;
                    posinputplc = 90;
                    CorsiaInizioDx = 0; // per disegno stato_pos =1 inizio a dx, =0 inizio a sx     NB se orizzontale e' bassoalto
                    fine = inizio + n_alveoli + numfile; // solo per la cancellazione dei piani. Semmai settare a mano
                    lato = 1; // dx
                    nodofermata = 1400;
                    deltay = 1200;
                    deltax = 1200;
                    stringadel = " where pos>=1 and pos<=8";
                    break;
                case 2:
                    zona = "PR"; // prelievo rulliera
                    // nomecorsia = "52R";
                    nomecorsia = "02-"; // zona, cella, corsia, piano
                    inizio = 19; // far partire da pos=pos-1  // !!! CREDO SIA NECESSARIO FARE 10-1 PERCHE' NELLA FUNZIONE SUCCESSIVA PARTE DAL N+1
                    startfila = 30; // imppostazione partenza fila manuale
                    n_alveoli = 1;
                    numfile = 1;
                    step = n_alveoli;
                    tipo_posizione = 2;
                    ord_corsia = 0; // ordinamento descrizione righe , 0= parto dal numero più basso
                    ord_corsia_opposta = 1; // parto dal piu basso 121
                    dalbassoinalto = 0; // 0: inverte ordine crs 1: segue ordine crs     NB:se orizzontale e' dxsx
                    piani = 1;
                    wshape = 1250;
                    hshape = 1100;
                    abilitata = 1;
                    posinputplc = 91;
                    CorsiaInizioDx = 0; // per disegno stato_pos =1 inizio a dx, =0 inizio a sx     NB se orizzontale e' bassoalto
                    fine = inizio + n_alveoli + numfile; // solo per la cancellazione dei piani. Semmai settare a mano
                    lato = 1; // dx
                    nodofermata = 1400;
                    deltay = 1200;
                    deltax = 1200;
                    stringadel = " where pos=20";
                    break;
                case 3:
                    zona = "DP"; // zona picking
                    // nomecorsia = "53R";
                    nomecorsia = "05-";
                    inizio = 10; // far partire da pos=pos-1
                    startfila = 40; // imppostazione partenza fila manuale
                    n_alveoli = 1;
                    numfile = 3;
                    step = n_alveoli;
                    tipo_posizione = 3;
                    ord_corsia = 0; // ordinamento descrizione righe , 0= parto dal numero più basso
                    ord_corsia_opposta = 1; // parto dal piu basso 121
                    dalbassoinalto = 0; // 0: inverte ordine crs 1: segue ordine crs     NB:se orizzontale e' dxsx
                    piani = 1;
                    wshape = 1250;
                    hshape = 1100;
                    abilitata = 1;
                    posinputplc = 92;
                    CorsiaInizioDx = 0; // per disegno stato_pos =1 inizio a dx, =0 inizio a sx     NB se orizzontale e' bassoalto
                    fine = inizio + n_alveoli + numfile; // solo per la cancellazione dei piani. Semmai settare a mano
                    lato = 1; // dx
                    nodofermata = 1400;
                    deltay = 1200;
                    deltax = 1200;
                    stringadel = " where pos>=11 and pos<=13";
                    break;
                case 4:
                    zona = "PP"; // prelievo principale
                    // nomecorsia = "53R";
                    nomecorsia = "01-";
                    inizio = 100; // far partire da pos=pos-1  pos 103
                    startfila = 50; // imppostazione partenza fila manuale
                    n_alveoli = 1;
                    numfile = 1;
                    step = n_alveoli;
                    tipo_posizione = 4;
                    ord_corsia = 0; // ordinamento descrizione righe , 0= parto dal numero più basso
                    ord_corsia_opposta = 1; // parto dal piu basso 121
                    dalbassoinalto = 0; // 0: inverte ordine crs 1: segue ordine crs     NB:se orizzontale e' dxsx
                    piani = 1;
                    wshape = 1250;
                    hshape = 1100;
                    abilitata = 1;
                    posinputplc = 93;
                    CorsiaInizioDx = 0; // per disegno stato_pos =1 inizio a dx, =0 inizio a sx     NB se orizzontale e' bassoalto
                    fine = inizio + n_alveoli + numfile; // solo per la cancellazione dei piani. Semmai settare a mano
                    lato = 1; // dx
                    nodofermata = 1400;
                    deltay = -1000;
                    deltax = 1600;
                    stringadel = " where pos=101";
                    break;
                case 5:
                    zona = "ET"; // Etichettatrice
                    nomecorsia = "ETI";
                    inizio = 9; // far partire da pos=pos-1
                    startfila = 60; // imppostazione partenza fila manuale
                    n_alveoli = 1;
                    numfile = 1;
                    step = n_alveoli;
                    tipo_posizione = 5;
                    ord_corsia = 0; // ordinamento descrizione righe , 0= parto dal numero più basso
                    ord_corsia_opposta = 1; // parto dal piu basso 121
                    dalbassoinalto = 0; // 0: inverte ordine crs 1: segue ordine crs     NB:se orizzontale e' dxsx
                    piani = 1;
                    wshape = 1250;
                    hshape = 1100;
                    abilitata = 1;
                    posinputplc = 94;
                    CorsiaInizioDx = 0; // per disegno stato_pos =1 inizio a dx, =0 inizio a sx     NB se orizzontale e' bassoalto
                    fine = inizio + n_alveoli + numfile; // solo per la cancellazione dei piani. Semmai settare a mano
                    lato = 1; // dx
                    nodofermata = 1400;
                    deltay = 1200;
                    deltax = 1200;
                    stringadel = " where pos=10";
                    break;
                    // !!! AGGIUNGERE UN CASE PER LE POSIZIONI 11-12-13 DI DEPOSITO (?)
                }
                ADOQuery = new TADOQuery(NULL);
                ADOQuery->Connection = dmDB->ADOConnection1;
                ADOQuery->SQL->Clear();
                ADOQuery->SQL->Append("delete from Corsie where tipologia=" + IntToStr(tipo_posizione));
                res = ADOQuery->ExecSQL();
                ADOQuery->Close();
                ADOQuery->SQL->Clear();
                ADOQuery->SQL->Append("delete from Posizioni where posinputplc=" + IntToStr(posinputplc));
                res = ADOQuery->ExecSQL();
                ADOQuery->Close();
                ADOQuery->SQL->Clear();
                ADOQuery->SQL->Append("delete from Piani " + stringadel);
                res = ADOQuery->ExecSQL();
                ADOQuery->Close();

                // var fisse
                tipologia = tipo_posizione;
                verticale = 1;
                pos_opposta = 0;

                for (j = 0; j < numfile; j++) {
                    ord_corsia = (j / 2) + 1;
                    if (dalbassoinalto > 0)
                        numerofila = numfile - (j + 1);
                    else
                        numerofila = j;
                    numerofila = numerofila + startfila;
                    // nomecorsia_table = nomecorsia + IntToStr(j + 1); // dmExtraFunction->PadS(, 2, "0");
                    nomecorsia_table = nomecorsia + "0" + IntToStr(j + 1) + "-";
                    if (tipo_posizione == 1)
                    {
                        nomecorsia_table = nomecorsia + "01-";
                    }
                    if (tipo_posizione == 2)
                    {
                        nomecorsia_table = nomecorsia + "03-";
                    }
                    if (tipo_posizione == 3)
                    {
                        nomecorsia_table = nomecorsia + "01-";
                    }
                    if (tipo_posizione == 4)
                    {
                        nomecorsia_table = nomecorsia + "10-";
                    }
                    strsql.printf("INSERT INTO Corsie "
                        "(Fila, NomeCorsia, Zona, Abilitata, InUsoPrelievo, CorsiaPrenotata, Piena, Vuota, InUsoDeposito, Priorita, NumPos, Tipologia, OrdinamentoCorsia, OrdinamentoCorsiaOpposta "
                        ", CodArt, AutoTipologia, NomeStanza, FilaInCorsia, CorsiaInizioDx, lato)"
                        " VALUES(%d, '%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', %d, %d, %d)"
                        , (numerofila)
                        , nomecorsia_table.c_str()
                        , zona.c_str()
                        , abilitata
                        , 0
                        , 0
                        , 0
                        , 0
                        , 0
                        , 0
                        , n_alveoli
                        , tipologia
                        , ord_corsia
                        , ord_corsia_opposta
                        , 0
                        , 0
                        , nomecorsia.c_str()
                        , (numerofila - startfila) + 1
                        , CorsiaInizioDx
                        , lato
                        );
                    ADOQuery->SQL->Clear();
                    ADOQuery->SQL->Text = strsql;
                    res = ADOQuery->ExecSQL();
                    ADOQuery->Close();

                    indice = 0;
                    //// ECCEZIONI
                    // inizio numerazione alveoli
                    kstart = 1;
                    kend = n_alveoli;
                    for (k = kstart; k <= kend; k++) {
                        pos = inizio + k + j;
                        pos_prec = (k == 0 ? 0 : pos - 1);
                        next_pos = (k == numfile ? 0 : pos + 1);
                        pos_opposta = inizio + (kend - k) + 1;
                        pos_in_fila = k;
                        pos_in_fila_opposta = inizio + (kend - k) + 1;
                        nodofermata = pos; // 2000 + k + ((j / 2) * 100); non chiaro questo campo
                        // nomecorsiaint = (nomecorsia == " SX " ? (((k - 1) / 3) * 2) + 1 : (((k - 1) / 3) * 2) + 2);
                        poscliente = nomecorsia_table + "0" + IntToStr(k) + "-"; // dmExtraFunction->PadS(IntToStr(k), 2, "0") + "-" + dmExtraFunction->PadS(IntToStr(j + 1), 2, "0") + "-00"; ;
                        if (tipo_posizione == 1)
                        {
                            poscliente = nomecorsia_table + "0" + IntToStr(pos) + "-";
                        }
                        if (tipo_posizione == 3)
                        {
                            if (pos == 13) // !! da mettere a posto
                            {
                                poscliente = nomecorsia_table + "0" + IntToStr(1) + "-";
                            }
                            else if (pos == 12)
                            {
                                poscliente = nomecorsia_table + "0" + IntToStr(2) + "-";
                            }
                            else if (pos == 11)
                            {
                                poscliente = nomecorsia_table + "0" + IntToStr(3) + "-";
                            }
                        }

                        strsql.printf("INSERT INTO Posizioni "
                            "(Pos, PosOpposta, CustomerPos, Prenotata, Disabilita, TipoPosizione "
                            ", Height, Width, NomeCorsia, Fila, PrevPosition "
                            ", NextPosition, PosInFila, PosInFilaOpposta, Zona, PosInputPlc, nodofermata, deltax, deltay,lato,corridoio)"
                            " VALUES(%d, %d, '%s', %d, %d, %d, "
                            " %d, %d, '%s', %d, %d, "
                            " %d, %d, %d, '%s', %d, %d, %d, %d, %d, %d)"
                            , pos
                            , pos_opposta
                            , poscliente.c_str()
                            , 0
                            , 0
                            , tipo_posizione
                            , hshape
                            , wshape
                            , nomecorsia_table.c_str()
                            , numerofila
                            , pos_prec
                            , next_pos
                            , pos_in_fila
                            , pos_in_fila_opposta
                            , zona.c_str()
                            , posinputplc
                            , nodofermata
                            , deltax
                            , deltay
                            , lato
                            , 0
                            );
                        ADOQuery->SQL->Clear();
                        ADOQuery->SQL->Text = strsql;
                        res = ADOQuery->ExecSQL();
                        ADOQuery->Close();
                        // pos_prec=pos;

                        // PIANI
                        pianistart = 1;
                        pianiend = piani;

                        for (z = pianistart; z <= pianiend; z++) {
                            // Tabella
                            /* tabella = (z == 1 ? 0 : 1);
                             nomepiano = poscliente + "0" + IntToStr(z);
                             if (lato == 1) {
                             tab_car = tab_carico[z];
                             tab_scar = tab_scarico[z];
                             }
                             else {
                             tab_car = tab_carico[z + 4];
                             tab_scar = tab_scarico[z + 4];
                             } */
                            // altezza prelievo e deposito
                            if (pos == 101)
                            {
                                h_prel[z - 1] = h_prel_baia_prelievo_101[z - 1];
                                h_dep[z - 1] = h_dep_baia_prelievo_101[z - 1];
                            }
                            else if (pos == 20)
                            {
                                h_prel[z - 1] = h_prel_baia_prelievo_20[z - 1];
                                h_dep[z - 1] = h_dep_baia_prelievo_20[z - 1]; ;
                            }
                            else if (pos >= 1 && pos <= 8)
                            {
                                h_prel[z - 1] = h_prel_rulliere_deposito[z - 1];
                                h_dep[z - 1] = h_dep_rulliere_deposito[z - 1];
                                if (pos >= 3 && pos <= 8)
                                {
                                    h_prel[z - 1] = h_dep_rulliere_deposito_3a8[z - 1];
                                    h_dep[z - 1] = h_prel_rulliere_deposito_3a8[z - 1];
                                }
                            }
                            else if (pos >= 11 && pos <= 13)
                            {
                                h_prel[z - 1] = h_prel_picking[z - 1];
                                h_dep[z - 1] = h_dep_picking[z - 1];
                            }
                            // modifica tabella
                            nomepiano = poscliente + "0" + IntToStr(z);
                            if (lato == 1)
                            {
                                if (z == 1)
                                {
                                    tabella = 0;
                                }
                                else if (z == 2)
                                {
                                    tabella = 1;
                                }
                                else if (z == 3)
                                {
                                    tabella = 2;
                                }
                            }

                            strsql.printf("INSERT INTO piani "
                                "(piano, pos, idudc, nomepos, hprel, hdep "
                                ", prenotata, disabilitata, tabella, tipopiano,tabcarico,tabscarico,fila)"
                                " VALUES(%d, %d, %d, '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d)"
                                , z
                                , pos
                                , 0
                                , nomepiano.c_str()
                                , h_prel[z - 1]
                                , h_dep[z - 1]
                                , 0
                                , 0
                                , tabella
                                , tipopiano
                                , tab_car
                                , tab_scar
                                , numerofila
                                );
                            ADOQuery->SQL->Clear();
                            ADOQuery->SQL->Text = strsql;
                            res = ADOQuery->ExecSQL();
                            ADOQuery->Close();
                        }

                    }
                }
            }
            ADOQuery->SQL->Clear();
            ADOQuery->Close();

        }
        catch (...) {
            dmDB->LogMsg("i=" + IntToStr(i) + " j=" + IntToStr(j) + " k=" + IntToStr(k) + " z=" + IntToStr(z));
        }
        delete ADOQuery;
        Application->MessageBox(L"Creazione posizioni effettuata", L"Lavoro terminato", MB_OK);
    }
}

int TdmDBClient::ReturnPrevFilaDaPos(int pos) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!dmDB->ADOConnection1->Connected) {
            return 0;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;

        stringa = "Select fila from Corsie where fila <" + IntToStr(pos) + " order by fila DESC";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("fila")->AsInteger;
        }
        else
            res = pos;
        ADOQuery->Close();

    }
    catch (...) {
        dmDB->ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}

int TdmDBClient::ReturnNextFilaDaPos(int pos) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!dmDB->ADOConnection1->Connected) {
            return 0;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;

        stringa = "Select fila from Corsie where fila >" + IntToStr(pos) + " order by fila ";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("fila")->AsInteger;
        }
        else
            res = pos;
        ADOQuery->Close();

    }
    catch (...) {
        dmDB->ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}

AnsiString TdmDBClient::ReturnDescTipoUDC(int tipo) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    AnsiString res = "";
    try {
        if (!dmDB->ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select Descrizione from TABTIPOUDC where IDTipoUDC = " + IntToStr(tipo);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("Descrizione")->AsString;
        }
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return res;
}

AnsiString TdmDBClient::ReturnDescStatoUDC(int tipo) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    AnsiString res = "";
    try {
        if (!dmDB->ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select Descrizione from TabStatoUDC where codstato = " + IntToStr(tipo);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("Descrizione")->AsString;
        }
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return res;
}

int TdmDBClient::ReturnaColoreDaUDC(int idudc) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select * from UDCLIST where idudc = " + IntToStr(idudc);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            if (ADOQuery->FieldByName("Stato")->AsInteger == 1) // vuoto da lavare
                    res = ADOQuery->FieldByName("Coloretipocorsiapuliti")->AsInteger;
            else if (ADOQuery->FieldByName("Stato")->AsInteger == 2) // vuoto lavato
                    res = ADOQuery->FieldByName("Coloretipocorsiasporchi")->AsInteger;
            else if (ADOQuery->FieldByName("Stato")->AsInteger == 3) { // pieno
                if (ADOQuery->FieldByName("CodTipoUdc")->AsInteger == 1)
                    res = dmDB->TipologiaCorsia[20]["COLORE"].ToIntDef(0);
                else if (ADOQuery->FieldByName("CodTipoUdc")->AsInteger == 2)
                    res = dmDB->TipologiaCorsia[21]["COLORE"].ToIntDef(0);
                else
                    res = clWhite;
            }
            else // se non rientra in nessun casistica
                    res = clWhite;
        }
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return res;
}

int TdmDBClient::SetColorePos(int idudc, long &cornice) {
    AnsiString stringa, campo;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select * from UDCLIST where idudc = " + IntToStr(idudc);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            if (ADOQuery->FieldByName("Stato")->AsInteger == 1) // vuoto da lavare
                    res = ADOQuery->FieldByName("Coloretipocorsiapuliti")->AsInteger;
            else if (ADOQuery->FieldByName("Stato")->AsInteger == 2) // vuoto lavato
                    res = ADOQuery->FieldByName("Coloretipocorsiasporchi")->AsInteger;
            else if (ADOQuery->FieldByName("Stato")->AsInteger == 3) { // pieno
                campo = "Fase" + dmExtraFunction->PadS(ADOQuery->FieldByName("fase")->AsInteger, 2, "0") + "colore";
                res = ADOQuery->FieldByName("Colorearticolo")->AsLargeInt;
                cornice = ADOQuery->FieldByName(campo)->AsInteger;
            }
            else // se non rientra in nessun casistica
                    res = clWhite;
        }
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return res;
}

AnsiString TdmDBClient::ReturnCodiceArticoliUDC(int IDArtUDC) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    AnsiString res = "";
    try {
        if (!dmDB->ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select codice from ARTICOLIUDC where IDArtUDC = " + IntToStr(IDArtUDC);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("Codice")->AsString;
        }
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return res;
}

int TdmDBClient::ReturnCodStatoUDCdaIDUDC(int idudc) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select codtipoudc from UDC where idudc = " + IntToStr(idudc);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("codtipoudc")->AsInteger;
        }
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return res;
}

int TdmDBClient::CercaUDCinPosizione(int idudc) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select pos from PIANI where idudc = " + IntToStr(idudc) + " and idudc>0";
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount)
            res = ADOQuery->FieldByName("pos")->AsInteger;
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return res;
}

int TdmDBClient::TornaUDCinPosizione(int pos) {
    // torna l'udc nel piano piu' alto
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select top 1 idudc from PIANI where pos = " + IntToStr(pos) + " and idudc>0 order by piano DESC";
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount)
            res = ADOQuery->FieldByName("idudc")->AsInteger;
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return res;
}

int TdmDBClient::CercaArticoloinArtUDC(int idarticolo) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select idartudc from articoliudc where idarticolo = " + IntToStr(idarticolo);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount)
            res = ADOQuery->FieldByName("idartudc")->AsInteger;
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return res;
}

void TdmDBClient::EliminaArticolodaArtUDC(int idartudc) {
    AnsiString stringa;
    int ok, idart_presente;
    stringa = "Update articoliudc set idarticolo = 0 where idartudc = " + String(idartudc);
    try {
        if (!dmDB->ADOConnection1->Connected)
            return;
        dmDB->QueryPos->Close();
        dmDB->QueryPos->SQL->Clear();
        dmDB->QueryPos->SQL->Append(stringa);
        dmDB->QueryPos->ExecSQL();
        dmDB->QueryPos->Close();
        dmDB->LogMsg(stringa);
        dmDB->QueryPos->Close();
    }
    catch (...) {
        dmDB->LogMsg("Problemi per eliminazione articolo da ArtUDC = " + String(idartudc));
        dmDB->QueryPos->Close();
    }

}

int TdmDBClient::ReturnNewIDARTUDC() {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 1;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select top 1 idArtudc from ARTicoliUDC order by idartudc DESC ";
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("idartudc")->AsInteger;
            res++;
        }
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return res;
}

int TdmDBClient::RitornaMaxIDParametro() {
    TADOQuery *ADOQuery;
    AnsiString strsql;

    int posnr = 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql.printf("SELECT TOP 1 idparametro FROM parametri order by idparametro desc ");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->First();
        posnr = ADOQuery->FieldByName("idparametro")->AsInteger;
        posnr++;
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return posnr;
}

int TdmDBClient::RitornaNumpos(AnsiString nomeCorsia) {
    int ret = 0;
    try {
        if (dmDB->ADOConnection1->Connected) {
            dmDB->QueryPos->Close();
            dmDB->QueryPos->SQL->Clear();
            dmDB->QueryPos->SQL->Append("Select numpos from Corsie where NomeCorsia = '" + nomeCorsia + "' ");
            dmDB->QueryPos->Open();
            if (dmDB->QueryPos->RecordCount > 0) {
                dmDB->QueryPos->First();
                ret = dmDB->QueryPos->FieldByName("numpos")->AsInteger;
            }
            dmDB->QueryPos->Close();
        }
    }
    catch (...) {
    }
    return ret;
}

AnsiString TdmDBClient::RitornaCustPosinPosizione(int pos) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    AnsiString res = " ";

    try {
        if (!dmDB->ADOConnection1->Connected)
            return " ";
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select customerpos from posizioni where pos = " + IntToStr(pos);
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount)
            res = ADOQuery->FieldByName("customerpos")->AsString;
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return res;

}

int TdmDBClient::contaPianiconUDC(int pos) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select count(*) as conta from piani where pos = " + IntToStr(pos) + " and idudc<>0 ";
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        res = ADOQuery->FieldByName("conta")->AsInteger;
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return res;
}

int TdmDBClient::CercaZona(int xx, int yy) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    int found = 0;

    while ((res < dmDB->TabStrutturaZone.size()) && (!found)) {
        if (((xx >= dmDB->TabStrutturaZone[res]["TOPX"].ToIntDef(0)) && (xx <= dmDB->TabStrutturaZone[res]["BOTX"].ToIntDef(0))) &&
            ((yy >= dmDB->TabStrutturaZone[res]["TOPY"].ToIntDef(0)) && (yy <= dmDB->TabStrutturaZone[res]["BOTY"].ToIntDef(0)))) {
            found = res;
        }
        else
            res++;
    }
    return found;
}

int TdmDBClient::ReturnPrimaPosDaFila(int fila) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!dmDB->ADOConnection1->Connected) {
            return 0;
        }
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;

        stringa = "Select top 1 pos from Posizioni where Fila = " + IntToStr(fila) + " order by Pos";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("pos")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
        // ADOConnection1->Close();
    }
    delete ADOQuery;
    return res;
}

// --------------------------------------------------
// --------- UPDATE POSTAZIONI ----------------------
// --------------------------------------------------

void TdmDBClient::InserisciPostazioniFunzione(TPosizioni Posizioni) {
    AnsiString strsql, nomecorsia, zona, poscliente, nomepiano, nomecorsia_table;
    int pos_opposta, pos_in_fila_opposta, tipologia, res, indicefila, j, k, k_pos, z, fila, verticale, CorsiaInizioDx, wshape, hshape, n_file_corsia, n_alveoli, pos_prec, next_pos, posopposta, pos;
    int numerofila, pos_in_fila, posinputplc, ord_corsia, ord_corsia_opposta, tipo_posizione, numfile, step, inizio, piani, dalbassoinalto, dasxadx = 0;
    int abilitata = 0;
    int h_prel[] = {70, 580, 1100, 1900, 2900};
    int h_dep[] = {450, 720, 1230, 2000, 3000};
    TADOQuery *ADOQuery;

    if (Application->MessageBox(L"Vuoi creare le posizioni?", L"Conferma!!!", MB_YESNO) == IDYES) {
        try {
            if (!dmDB->ADOConnection1->Connected)
                return;

            fila = 1;

            indicefila = Posizioni.indicefila;
            zona = Posizioni.zona;
            nomecorsia = dmExtraFunction->PadS(Posizioni.nomecorsia, 2, "0");
            inizio = Posizioni.inizio - 1;
            n_alveoli = Posizioni.n_alveoli;
            numfile = Posizioni.numfile;
            tipo_posizione = Posizioni.tipo_posizione;
            ord_corsia = Posizioni.ord_corsia; // ordinamento descrizione righe, 0= deposito dal numero piu' alto 1= deposito dal numero piu' basso
            ord_corsia_opposta = Posizioni.ord_corsia_opposta; // parto dal piu basso 121
            dalbassoinalto = Posizioni.dalbassoinalto; // 0: inverte ordine crs 1: segue ordine crs
            step = Posizioni.step;
            piani = Posizioni.piani;
            wshape = Posizioni.wshape;
            hshape = Posizioni.hshape;
            abilitata = Posizioni.abilitata;
            CorsiaInizioDx = Posizioni.CorsiaInizioDx; // per disegno stato_pos =1 inizio a dx, =0 inizio a sx
            dasxadx = Posizioni.dasxadx; // 0:numera da dx a sx. 1=da sx a dx
            tipologia = tipo_posizione;
            verticale = 1;
            pos_opposta = 0;

            ADOQuery = new TADOQuery(NULL);
            ADOQuery->Connection = dmDB->ADOConnection1;
            ADOQuery->SQL->Clear();
            ADOQuery->SQL->Append("delete from Corsie where zona = '" + zona + "' and nomestanza = '" + nomecorsia + "'");
            res = ADOQuery->ExecSQL();
            ADOQuery->Close();
            ADOQuery->SQL->Clear();
            ADOQuery->SQL->Append("delete from Posizioni where posinputplc=0 and zona = '" + zona + "'and substring(customerpos,3,3) = '" + nomecorsia + "'");
            res = ADOQuery->ExecSQL();
            ADOQuery->Close();
            ADOQuery->SQL->Clear();
            ADOQuery->SQL->Append("delete from Piani where (pos>=" + String(inizio + 1) + " and pos<=" + String(inizio + n_alveoli) + ")");
            res = ADOQuery->ExecSQL();
            ADOQuery->Close();

            for (j = 0;
                j < numfile;
                j++) {
                if (dalbassoinalto > 0)
                    numerofila = numfile - (j + 1);
                else
                    numerofila = j;
                nomecorsia_table = zona + nomecorsia + dmExtraFunction->PadS((numerofila + 1) * 10, 3, "0") + "00";
                strsql.printf("INSERT INTO Corsie "
                    "(Fila,NomeCorsia,Zona,Abilitata,InUsoPrelievo,CorsiaPrenotata,Piena,Vuota,InUsoDeposito,Priorita,NumPos,Tipologia,OrdinamentoCorsia,OrdinamentoCorsiaOpposta"
                    ",CodArt,AutoTipologia,NomeStanza,FilaInCorsia,CorsiaInizioDx) "
                    "VALUES (%d,'%s','%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d"
                    ",%d,%d,'%s',%d,%d)"
                    , ((indicefila - 1) * 50) + (numerofila + 1)
                    , nomecorsia_table.c_str()
                    , zona.c_str()
                    , abilitata
                    , 0
                    , 0
                    , 0
                    , 0
                    , 0
                    , 0
                    , n_alveoli
                    , tipologia
                    , ord_corsia
                    , ord_corsia_opposta
                    , 0
                    , 0
                    , nomecorsia.c_str()
                    , numerofila + 1
                    , CorsiaInizioDx
                    );
                ADOQuery->SQL->Clear();
                ADOQuery->SQL->Text = strsql;
                res = ADOQuery->ExecSQL();
                ADOQuery->Close();

                pos_in_fila = n_alveoli;
                pos_in_fila_opposta = 0;
                pos_prec = 0;
                next_pos = 0; // da remmare quando lo si gestira'
                for (k = 1;
                    k <= n_alveoli;
                    k++) {
                    if (dasxadx == 1)
                        k_pos = (n_alveoli + 1) - k;
                    else
                        k_pos = k;
                    pos = inizio + (j * step) + k_pos;
                    poscliente = zona + nomecorsia + dmExtraFunction->PadS((numerofila + 1) * 10, 3, "0") + "00"; // dmExtraFunction->PadS(k, 2, "0");
                    strsql.printf("INSERT INTO Posizioni "
                        "(Pos,PosOpposta,CustomerPos,Prenotata,Disabilita,TipoPosizione"
                        ",Height,Width,NomeCorsia,Fila,PrevPosition"
                        ",NextPosition,PosInFila,PosInFilaOpposta,Zona,PosInputPlc)"
                        " VALUES (%d,%d,'%s',%d,%d,%d,"
                        "%d,%d,'%s',%d,%d,"
                        "%d,%d,%d,'%s',%d)"
                        , pos
                        , pos_opposta
                        , poscliente.c_str()
                        , 0
                        , 0
                        , tipo_posizione
                        , hshape
                        , wshape
                        , nomecorsia_table.c_str()
                        , ((indicefila - 1) * 50) + (numerofila + 1)
                        , pos_prec
                        , next_pos
                        , pos_in_fila
                        , pos_in_fila_opposta
                        , zona.c_str()
                        , 0
                        );
                    ADOQuery->SQL->Clear();
                    ADOQuery->SQL->Text = strsql;
                    res = ADOQuery->ExecSQL();
                    ADOQuery->Close();

                    /////////////// piani
                    for (z = 1;
                        z <= piani;
                        z++) {
                        nomepiano = zona + nomecorsia + dmExtraFunction->PadS((numerofila + 1) * 10, 3, "0") + dmExtraFunction->PadS(z + (k - 1) * piani, 2, "0");

                        strsql.printf("INSERT INTO piani "
                            "(piano,pos,idudc,nomepos,hprel,hdep"
                            ",prenotata,disabilitata)"
                            " VALUES (%d,%d,%d,'%s',%d,%d,%d,%d)"
                            , z
                            , pos
                            , 0
                            , nomepiano.c_str()
                            , h_prel[z - 1]
                            , h_dep[z - 1]
                            , 0
                            , 0
                            );
                        ADOQuery->SQL->Clear();
                        ADOQuery->SQL->Text = strsql;
                        res = ADOQuery->ExecSQL();
                        ADOQuery->Close();
                    }

                }
            }
            ADOQuery->SQL->Clear();
            ADOQuery->Close();

        }
        catch (...) {
        }
        delete ADOQuery;
        Application->MessageBox(L"Creazione posizioni effettuata", L"Lavoro terminato", MB_OK);
    }
}
