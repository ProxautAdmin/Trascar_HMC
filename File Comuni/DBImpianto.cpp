// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBImpianto.h"
#include "db.h"
#include "ExtraFunction.h"
#include "clientdata.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TdmDBImpianto *dmDBImpianto;

// ---------------------------------------------------------------------------
__fastcall TdmDBImpianto::TdmDBImpianto(TComponent* Owner)
    : TDataModule(Owner)
{
}
// ---------------------------------------------------------------------------

void TdmDBImpianto::TornaPosPianoDaIDUDC(int idudc, int &pos, int &piano) {
    TADOQuery *ADOQuery;
    AnsiString strsql;
    pos = 0;
    piano = 0;

    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql.printf("SELECT pos, piano FROM UDC_view where IDUDC=%d", idudc);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        if (ADOQuery->RecordCount) {
            pos = ADOQuery->FieldByName("pos")->AsInteger;
            piano = ADOQuery->FieldByName("piano")->AsInteger;
        }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    // ultimo_id_articolo = idsrticolo ; // me lo salvo perchè se creo un articolo in manuale e un oin auto non ddeve avere l ostesso id
    return;
}
/*
 int TdmDBImpianto::TornaTabella(int pos, int piano) {
 int ret = 0;
 TADOQuery *ADOQuery;
 AnsiString strsql;

 if (!dmDB->ADOConnection1->Connected)
 return 0;
 try {
 ADOQuery = new TADOQuery(NULL);
 ADOQuery->Connection = dmDB->ADOConnection1;
 strsql.printf("SELECT tabella FROM piani where pos=%d and piano=%d", pos, piano);
 ADOQuery->SQL->Text = strsql;
 ADOQuery->Open();
 ADOQuery->Last();
 if (ADOQuery->RecordCount) {
 ret = ADOQuery->FieldByName("tabella")->AsInteger;
 }
 ADOQuery->Close();
 delete ADOQuery;
 }
 catch (...) {
 }
 return ret;
 } */

int TdmDBImpianto::TornaTabella(int pos, int piano, int fila) {
    int ret = 0;
    TADOQuery *ADOQuery;
    AnsiString strsql;

    if (!dmDB->ADOConnection1->Connected)
        return 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql.printf("SELECT tabella FROM piani where pos=%d and piano=%d and fila=%d", pos, piano, fila); // fila dispari piano 1 tabella 0, fila pari piano 1 tabella 1
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        if (ADOQuery->RecordCount) {
            ret = ADOQuery->FieldByName("tabella")->AsInteger;
        }
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return ret;
}

int TdmDBImpianto::TornaPosPianoDaNomePos(AnsiString Nomepos, int &pos, int &piano) {
    int ret = 0;
    TADOQuery *ADOQuery;
    AnsiString strsql;
    pos = 0;
    piano = 0;

    if (!dmDB->ADOConnection1->Connected)
        return 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql.printf("SELECT pos, piano FROM piani where ltrim(rtrim(nomepos))='%s'", Nomepos.Trim());
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        if (ADOQuery->RecordCount) {
            pos = ADOQuery->FieldByName("pos")->AsInteger;
            piano = ADOQuery->FieldByName("piano")->AsInteger;
        }
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return ret;
}

int TdmDBImpianto::RitornaLatoForche(int pos, int corsia, int isprel) {
    int ret = 0;
    AnsiString strsql;
    TADOQuery *ADOQuery;

    if (!dmDB->ADOConnection1->Connected)
        return 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql.printf("SELECT pos, LATO FROM posizioni where pos=%d and fila=%d", pos, corsia);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        if (ADOQuery->RecordCount) {
            if ((ADOQuery->FieldByName("pos")->AsInteger)) {
                ret = ADOQuery->FieldByName("lato")->AsInteger;
            }
        }
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return ret;
}

int TdmDBImpianto::AggiornaStatoMissioniWMS(int id, int stato) {
    AnsiString strsql, time;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        if (stato == 0) {
            strsql = "UPDATE MissioniWMS SET stato = stato + 1 ";
            // fare lettura stato
            stato++;
        }

        strsql = "UPDATE MissioniWMS SET stato = " + IntToStr(stato);

        strsql = strsql + " where progressivo = " + IntToStr(id);
        ADOQuery->SQL->Text = strsql;
        res = ADOQuery->ExecSQL();
        dmDB->LogMsg(strsql + " , result : " + IntToStr(res));
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDBImpianto::AggiornaAgvMissioniWMS(int id, int agv)
{
    AnsiString strsql, time;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql = "UPDATE TabellaWMS SET AGV = " + IntToStr(agv);
        strsql = strsql + " where progressivo = " + IntToStr(id);
        ADOQuery->SQL->Text = strsql;
        res = ADOQuery->ExecSQL();
        dmDB->LogMsg(strsql + " , result : " + IntToStr(res));
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

void TdmDBImpianto::RitornaPosPianodaNomePos(AnsiString NomePos, int &pos, int &piano, int &corsia) { // mod corsia
    int ret = 0;
    AnsiString strsql;
    TADOQuery *ADOQuery;

    pos = 0;
    piano = 0;

    if (!dmDB->ADOConnection1->Connected)
        return;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql.printf("SELECT pos, piano, fila FROM piani where nomepos='%s'", NomePos);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->First();
        if (ADOQuery->RecordCount) {
            pos = ADOQuery->FieldByName("pos")->AsInteger;
            piano = ADOQuery->FieldByName("piano")->AsInteger;
            corsia = ADOQuery->FieldByName("fila")->AsInteger;
        }
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return;
}

void TdmDBImpianto::AnnullaMissione() {
    AnsiString strsql;
    TADOQuery *ADOQuery;

    if (!dmDB->ADOConnection1->Connected)
        return;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql.printf("UPDATE tabellaWMS SET StatoMissione = 10, DataOraFineMissione = GetDate() WHERE MissioneAnnullata=1 and StatoMissione=0");
        ADOQuery->SQL->Text = strsql;
        ADOQuery->ExecSQL();
        ADOQuery->Close();
    }
    catch (...) {

    }
    delete ADOQuery;
}

int TdmDBImpianto::AggiornaStatoEtichettatura(int agv)
{
    AnsiString strsql, time;
    TADOQuery *ADOQuery;
    int res = 0, ret = 0;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;

        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql = "SELECT IDWMS,Id FROM Missioni_View where Id = " + IntToStr(ClientData.DatiAgv[agv].idmis);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->First();
        if (ADOQuery->RecordCount)
        {
            if ((ADOQuery->FieldByName("Id")->AsInteger))
            {
                ret = ADOQuery->FieldByName("IDWMS")->AsInteger;
            }
        }
        strsql = "UPDATE TabellaWMS SET EtichettaApplicata = 1 ";
        strsql = strsql + ", DataOraEtichetta = GetDate()";
        strsql = strsql + " where progressivo = " + IntToStr(ret);
        ADOQuery->SQL->Text = strsql;
        res = ADOQuery->ExecSQL();
        dmDB->LogMsg(strsql + " , result : " + IntToStr(res));
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDBImpianto::RitornaPosizioneChiamata(int pos) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select chiamataPos from piani_view where Pos = " + IntToStr(pos);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0)
            res = ADOQuery->FieldByName("chiamataPos")->AsInteger;

        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDBImpianto::RitornaUDCPosizione(int pos) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select idudc from piani_view where Pos = " + IntToStr(pos);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0)
            res = ADOQuery->FieldByName("idudc")->AsInteger;

        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDBImpianto::RitornaIdPlcDaPosizione(int posizione, int tipo_missione, int piano) {
    if (tipo_missione == 1) // missione di prelievo
    {
        for (int i = 1; i <= N_PLC; i++) {
            for (int j = 1; j <= NUM_PRELIEVOPLC; j++) {
                for (int p = 1; p <= NUMPIANI_PREL; p++) {
                    if ((ClientData.Plc[i].Prelievo[j][p].pos == posizione) && (p == piano)) {
                        return j;
                    }
                }
            }
        }
    }
    else if (tipo_missione == 2) // missione di deposito
    {
        for (int i = 1; i <= N_PLC; i++) {
            for (int j = 1; j <= NUM_DEPOSITOPLC; j++) {
                for (int p = 1; p <= NUMPIANI_DEP; p++) {
                    if ((ClientData.Plc[i].Deposito[j][p].pos == posizione) && (p == piano)) {
                        return j;
                    }
                }
            }
        }
    }
    return 0;
}

int TdmDBImpianto::AggiornaSelezionePosizioni(AnsiString Zona, int pos, int stato) {
    AnsiString strsql, time;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql = "UPDATE piani_view SET selezionata = (Case when pos=" + IntToStr(pos) + " then " + IntToStr(stato);
        strsql = strsql + " else 0 end)";
        strsql = strsql + " where Zona = '" + Zona + "' ";
        ADOQuery->SQL->Text = strsql;
        res = ADOQuery->ExecSQL();
        dmDB->LogMsg(strsql + " , result : " + IntToStr(res));
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

void TdmDBImpianto::LeggiSelezionataDaZona(AnsiString Zona, int &pos, int &piano) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    pos = 0;
    piano = 0;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select top 1 pos, piano from piani_view where Zona = '" + Zona + "'";
        stringa += " selezionata=1 and prenotata=0 and disabilitata=0 and pos_prenotata=0 and pos_disabilitata=0 and corsiaprenotata=0 ";
        stringa += " order by pos, piano desc";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            pos = ADOQuery->FieldByName("pos")->AsInteger;
            piano = ADOQuery->FieldByName("piano")->AsInteger;
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return;
}

void TdmDBImpianto::ResettaPLC() {
    // dmDBImpianto->SettaPLCPrenotata(1000, 0);
    // dmDBImpianto->SettaPLCPrenotata(1100, 0);
    // dmDBImpianto->SettaPLCPrenotata(1200, 0);
    // dmDBImpianto->SettaPLCPrenotata(1300, 0);

}

void TdmDBImpianto::SettaPLCInit() {
    TADOQuery *ADOQuery;
    AnsiString strsql;

    if (!dmDB->ADOConnection1->Connected)
        return;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql = "select pos, prenotata, pos_prenotata, disabilitata, pos_disabilita, idudc from piani_view where pos>=1000 and pos<=1300";
        strsql += " order by pos ";
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->First();
        while (!ADOQuery->Eof) {
            SettaPLCPrenotata(ADOQuery->FieldByName("pos")->AsInteger, ADOQuery->FieldByName("prenotata")->AsInteger | ADOQuery->FieldByName("pos_prenotata")->AsInteger);
            SettaPLCEsclusa(ADOQuery->FieldByName("pos")->AsInteger, ADOQuery->FieldByName("disabilitata")->AsInteger | ADOQuery->FieldByName("pos_disabilita")->AsInteger);
            SettaPLCIDUDC(ADOQuery->FieldByName("pos")->AsInteger, ADOQuery->FieldByName("idudc")->AsInteger);
            ADOQuery->Next();
        }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return;
}

void TdmDBImpianto::SettaPLCEsclusa(int pos, int val) {
    /*
     for (int i = IDX_DEPOSITOROCKWELL; i < NUMPLCROCKWELL_DEPOSITO + IDX_DEPOSITOROCKWELL; i++) {
     for (int j = 1; j <= NUM_DEPOSITOROCKVELL; j++) {
     if (ClientData.EIPlc[i].Deposito[j].pos == pos) {
     ClientData.EIPlc[i].Deposito[j].esclusa = val;
     break;
     }

     }
     }
     for (int i = IDX_PRELIEVOROCKWELL; i < NUMPLCROCKWELL_PRELIEVO + IDX_PRELIEVOROCKWELL; i++) {
     for (int j = 1; j <= NUM_PRELIEVOROCKWELL; j++) {
     if (ClientData.EIPlc[i].Prelievo[j].pos == pos) {
     ClientData.EIPlc[i].Prelievo[j].esclusa = val;
     break;
     }

     }
     }
     */
}

void TdmDBImpianto::SettaPLCPrenotata(int pos, int val) {
    /*
     for (int i = IDX_DEPOSITOROCKWELL; i < NUMPLCROCKWELL_DEPOSITO + IDX_DEPOSITOROCKWELL; i++) {
     for (int j = 1; j <= NUM_DEPOSITOROCKVELL; j++) {
     if (ClientData.EIPlc[i].Deposito[j].pos == pos) {
     ClientData.EIPlc[i].Deposito[j].generata = val;
     break;
     }

     }
     }
     for (int i = IDX_PRELIEVOROCKWELL; i <= NUMPLCROCKWELL_PRELIEVO + IDX_PRELIEVOROCKWELL; i++) {
     for (int j = 1; j <= NUM_PRELIEVOROCKWELL; j++) {
     if (ClientData.EIPlc[i].Prelievo[j].pos == pos) {
     ClientData.EIPlc[i].Prelievo[j].generata = val;
     break;
     }

     }
     }
     */
}

void TdmDBImpianto::SettaPLCIDUDC(int pos, int val) {
    /*
     for (int i = IDX_DEPOSITOROCKWELL; i < NUMPLCROCKWELL_DEPOSITO + IDX_DEPOSITOROCKWELL; i++) {
     for (int j = 1; j <= NUM_DEPOSITOROCKVELL; j++) {
     if (ClientData.EIPlc[i].Deposito[j].pos == pos) {
     ClientData.EIPlc[i].Deposito[j].idudc = val;
     break;
     }

     }
     }
     for (int i = IDX_PRELIEVOROCKWELL; i < NUMPLCROCKWELL_PRELIEVO + IDX_PRELIEVOROCKWELL; i++) {
     for (int j = 1; j <= NUM_PRELIEVOROCKWELL; j++) {
     if (ClientData.EIPlc[i].Prelievo[j].pos == pos) {
     ClientData.EIPlc[i].Prelievo[j].idudc = val;
     break;
     }

     }
     }
     */
}

void TdmDBImpianto::TornaPosDepLibera(AnsiString Zona, int &pos, int &piano, int tipoposizione) {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev, TP, selectpiano = " ";
    pos = 0;
    piano = 0;

    if (tipoposizione == 0)
        ev = " ";
    else
        ev = " tipoposizione= " + IntToStr(tipoposizione) + " and ";

    if (tipoposizione == TIPOLOGIA_SCARTO)
        selectpiano = " piano=1 and ";

    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        // fa cagare ma va bene cosi'
        if ((Zona == "B") || (Zona == "C") || (Zona == "E"))
            strsql.printf("Select top 1 pos, piano from piani_view where %s  ISNULL(disabilitata,0)=0 and ISNULL(pos_disabilita,0)=0 and zona='%s' order by pos, piano", ev, Zona);
        else
            strsql.printf("Select top 1 pos, piano from piani_view where %s idudc=0 and %s ISNULL(prenotata,0)=0 and ISNULL(disabilitata,0)=0 and ISNULL(pos_prenotata,0)=0 and ISNULL(pos_disabilita,0)=0 and ISNULL(selezionata,0)=0 and zona='%s' order by (SELECT COUNT(*) AS Expr1 FROM dbo.Piani AS Piani_1 WHERE (pos = dbo.piani_view.Pos) AND (IDUDC <> 0)) desc,pos, piano",
            ev, selectpiano, Zona);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        if (ADOQuery->RecordCount) {
            pos = ADOQuery->FieldByName("pos")->AsInteger;
            piano = ADOQuery->FieldByName("piano")->AsInteger;
        }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return;
}

void TdmDBImpianto::TornaPosDepLiberaH(AnsiString Zona, int IDUDC, int &pos, int &piano) {
    TADOQuery *ADOQuery;
    AnsiString strsql, ev, TP;
    pos = 0;
    piano = 0;

    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        // strsql.printf(order by (SELECT COUNT(*) AS Expr1 FROM dbo.Piani AS Piani_1 WHERE (pos = dbo.piani_view.Pos) AND (IDUDC <> 0)) desc,pos, piano", Zona);
        strsql = "Select top 1 pos, piano from piani_view where idudc=0 and ISNULL(prenotata,0)=0 and ISNULL(disabilitata,0)=0 and ISNULL(pos_prenotata,0)=0 and ISNULL(pos_disabilita,0)=0 and ISNULL(selezionata,0)=0 and zona='" +
            Zona + "' ";
        strsql += " and ((select idudc FROM dbo.Piani AS Piani_1 WHERE  (pos = dbo.piani_view.Pos) AND (piano = dbo.piani_view.piano-1))=" + IntToStr(IDUDC) + " or (piano=1)) ";
        strsql += " order by (SELECT COUNT(*) AS Expr1 FROM dbo.Piani AS Piani_1 WHERE (pos = dbo.piani_view.Pos) AND (IDUDC <> 0)) desc,pos, piano ";
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        if (ADOQuery->RecordCount) {
            pos = ADOQuery->FieldByName("pos")->AsInteger;
            piano = ADOQuery->FieldByName("piano")->AsInteger;
        }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return;
}

int TdmDBImpianto::PianiOccupatiPerPos(int pos) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "select pos , ";
        stringa += " (SELECT COUNT(*) AS Expr1 FROM dbo.Piani WHERE (pos = dbo.Posizioni.Pos)) AS npiani, ";
        stringa += " (SELECT COUNT(*) AS Expr1 FROM dbo.Piani AS Piani_1 WHERE (pos = dbo.Posizioni.Pos) AND (IDUDC <> 0)) AS npianiocc ";
        stringa += " where Pos = " + IntToStr(pos);
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0)
            res = ADOQuery->FieldByName("idudc")->AsInteger;

        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

void TdmDBImpianto::PrelievoVuoti(int idzona, int &pos, int &pianiocc) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;
    pos = 0;
    pianiocc = 0;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "select top 1 pos , ";
        stringa += " (SELECT COUNT(*) AS Expr1 FROM dbo.Piani WHERE (pos = dbo.Piani_View.Pos)) AS npiani, ";
        stringa += " (SELECT COUNT(*) AS Expr1 FROM dbo.Piani AS Piani_1 WHERE (pos = dbo.Piani_View.Pos) AND (IDUDC <> 0)) AS npianiocc ";
        if (pos > 0) {
            stringa += " from Piani_View where Pos = " + IntToStr(pos);
        }
        else {
            stringa += " from Piani_View where idzona=" + IntToStr(idzona) + " and piano =1 and  idudc>0 ";
            stringa += " and tipoposizione = " + IntToStr(TIPOLOGIA_PALLET);
            stringa += " and ISNULL(prenotata,0)=0 and ISNULL(disabilitata,0)=0 and ISNULL(pos_prenotata,0)=0 and ISNULL(pos_disabilita,0)=0 and ISNULL(selezionata,0)=0 ";
            stringa += " order by npianiocc, pos desc";
        }
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0)
            pos = ADOQuery->FieldByName("pos")->AsInteger;
        pianiocc = ADOQuery->FieldByName("npianiocc")->AsInteger;

        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return;
}

int TdmDBImpianto::TornaPosVuoteZona(int idzona) {
    AnsiString stringa;
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "select pos from piani_view ";
        stringa += " where idudc=0 and idzona=" + IntToStr(idzona);
        stringa += " and ISNULL(prenotata,0)=0 and ISNULL(disabilitata,0)=0 and ISNULL(pos_prenotata,0)=0 and ISNULL(pos_disabilita,0)=0 and ISNULL(selezionata,0)=0 ";
        stringa += " order by pos , piano";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0)
            res = ADOQuery->FieldByName("pos")->AsInteger;

        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDBImpianto::CercaPrelievoH() {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "select DISTINCT top 1 pos from piani_view where ";
        stringa += " (SELECT COUNT(*) AS Expr1 FROM dbo.Piani AS Piani_1 WHERE (pos = dbo.piani_view.Pos) AND (IDUDC <> 0))=2 ";
        stringa += " and zona='H' ";
        stringa += " order by pos ",
            ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0)
            res = ADOQuery->FieldByName("pos")->AsInteger;

        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDBImpianto::AggiornaUDCPosizioni(int pos, int IDUDC, int piano) {
    AnsiString strsql, time;
    TADOQuery *ADOQuery;
    int res = 0;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return 0;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql = "UPDATE piani_view SET IDUDC = (Case when piano<=" + IntToStr(piano) + " then " + IntToStr(IDUDC);
        strsql = strsql + " else 0 end)";
        strsql = strsql + " where pos = " + IntToStr(pos);
        ADOQuery->SQL->Text = strsql;
        res = ADOQuery->ExecSQL();
        dmDB->LogMsg(strsql + " , result : " + IntToStr(res));
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDBImpianto::CercaPrelievo(AnsiString Zona, int tipoposizione) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "select DISTINCT top 1 pos from piani_view where ";

        if (tipoposizione > 0)
            stringa += " tipoposizione= " + IntToStr(tipoposizione) + " and ";

        stringa += " idudc>0 and zona='" + Zona + "' ";
        stringa += " and ISNULL(disabilitata,0)=0 and ISNULL(pos_prenotata,0)=0 and ISNULL(pos_disabilita,0)=0 and ISNULL(selezionata,0)=0 ";
        stringa += " order by pos ";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0)
            res = ADOQuery->FieldByName("pos")->AsInteger;

        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDBImpianto::CercaPrelievoF() {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "select DISTINCT top 1 pos from piani_view where ";
        stringa += " idudc>0 and zona='F' ";
        stringa += " and ISNULL(disabilitata,0)=0 and ISNULL(pos_prenotata,0)=0 and ISNULL(pos_disabilita,0)=0 and ISNULL(selezionata,0)=1 ";
        stringa += " order by pos ";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0)
            res = ADOQuery->FieldByName("pos")->AsInteger;

        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

AnsiString TdmDBImpianto::TornaDescrizionedaZonaA1(AnsiString CodArt, int &impila) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    AnsiString res = "";
    impila = 0;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select DescrizioneArticolo, sovrapposto from ZonaA1 where articolo = '" + CodArt + "'";
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("DescrizioneArticolo")->AsString;
            impila = ADOQuery->FieldByName("sovrapposto")->AsInteger;
        }
        ADOQuery->Close();
        delete ADOQuery;
    }
    catch (...) {
    }
    return res;
}

int TdmDBImpianto::TornaIndiceImpilabilitadaIDUDC(int idudc) {
    TADOQuery *ADOQuery;
    AnsiString strsql;
    int res = 0;

    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql.printf("SELECT IndiceImpilabilita FROM UDC_view where IDUDC=%d", idudc);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("IndiceImpilabilita")->AsInteger;
        }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDBImpianto::ClonaHMC_ORDINI_IN_LAVORAZIONE(AnsiString posizione) {
    TADOQuery *ADOQuery;
    AnsiString strsql;
    int res = 1;
    // NB: pensare di mettere un loop se record =0
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        if (dmDB->ADOConnection1->Connected) {
            ADOQuery->Close();
            ADOQuery->SQL->Clear();
            ADOQuery->SQL->Append("Delete HMC_ORDINI_IN_LAVORAZIONE_Copia where posizione='" + posizione + "'");
            ADOQuery->ExecSQL();
            ADOQuery->Close();
            ADOQuery->Close();
            ADOQuery->SQL->Clear();
            strsql = "INSERT INTO HMC_ORDINI_IN_LAVORAZIONE_Copia select * from HMC_ORDINI_IN_LAVORAZIONE where posizione='" + posizione + "'";
            ADOQuery->SQL->Append(strsql);
            ADOQuery->ExecSQL();
            ADOQuery->Close();
        }

    }
    catch (...) {
        res = 0;
    }
    delete ADOQuery;

    return res;
}

int TdmDBImpianto::CheckZonaUtente(AnsiString zonadacontrollare) {
    int ret = 0;
    // comunque se Z fai tutto, 99 puo' essere qualunque valore >0
    if (dmDB->ZonaString == "Z")
        ret = 99;
    else
        ret = dmDB->ZonaString.Pos(zonadacontrollare);

    return ret;
}

AnsiString TdmDBImpianto::TornaCodartConRigaDaHMC_ORDINI_IN_LAVORAZIONE(int riga, TUDC &UDC) {
    TADOQuery *ADOQuery;
    AnsiString strsql;
    AnsiString res = "";

    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        strsql.printf("SELECT * FROM HMC_ORDINI_IN_LAVORAZIONE_Copia where Posizione=%d", riga);
        ADOQuery->SQL->Text = strsql;
        ADOQuery->Open();
        ADOQuery->Last();
        if (ADOQuery->RecordCount) {
            res = ADOQuery->FieldByName("articolo")->AsString;
            dmExtraFunction->StringToChar(ADOQuery->FieldByName("articolo")->AsString, UDC.Articolo.CodArt);
            dmExtraFunction->StringToChar(ADOQuery->FieldByName("descrizione articolo")->AsString, UDC.Articolo.Descrizione);
            UDC.IndiceImpilabilita = ADOQuery->FieldByName("sovrapposto")->AsInteger;

        }
        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDBImpianto::CercaUDCinH(int udc) {
    AnsiString stringa;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "select DISTINCT top 1 pos from piani_view where ";
        stringa += " (SELECT COUNT(*) AS Expr1 FROM dbo.Piani AS Piani_1 WHERE (pos = dbo.piani_view.Pos) AND (IDUDC <> 0))=1 ";
        stringa += " and idudc = " + IntToStr(udc);
        stringa += " and zona='H' ";
        stringa += " order by pos ";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0)
            res = ADOQuery->FieldByName("pos")->AsInteger;

        ADOQuery->Close();
    }
    catch (...) {
    }
    delete ADOQuery;
    return res;
}

int TdmDBImpianto::GeneraCMDaHaBFineProduzione(int val) {
    TCentroMissione cm;
    AnsiString strsql;
    int tipo_mis, agv;
    int id_articolo, IDPlc;
    TUDC UDCMissione;
    int i, ok_genera = 0, idudc, idudc2, res = 0, piano_dep = 0;
    int idx_plc, idx;

    try {
        idx = 1; // fisso
        idx_plc = IDX_PLCDEPOSITO; // metti che ci siano altri indici cosi' lo tratto come voglio
        if (1 == 1) { // ((ClientData.Plc[idx_plc].Deposito[idx][1].Ready) && (!ClientData.Plc[idx_plc].Deposito[idx][1].InAllarme) && (ClientData.Plc[idx_plc].Deposito[idx][1].ProntaAlDeposito)) {
            if (1 == 1) { // (dmDB->PresenzaCentroMissione(ClientData.Plc[idx_plc].Deposito[idx][1].pos, 0) == 0) {
                if (1 == 1) { // if (dmDB->PosPresenteMissioneAttiva(ClientData.Plc[idx_plc].Deposito[idx][1].pos) == 0) {
                    // per limitare la generazione delle missioni un ciclo alla volta, controllo che ok_genera=0, in questo modo scorre i record finche'
                    // o sono finiti o ne ha trovato uno buono, altrimenti potrebbe fermarsi a generare anche se potrebbe proseguire
                    if (ok_genera == 0) {
                        cm.TipoMissione = 0;
                        cm.CodTipoMovimento = 0;
                        cm.CodTipoMissione = 1;
                        cm.Agv = 1;
                        cm.IDUDC = val; // ???
                        cm.TipoUDC = 0; // ??
                        cm.stato = 0;
                        cm.Priorita = dmDB->priorita_missioni[3];

                        cm.posdep = ClientData.Plc[idx_plc].Deposito[idx][1].pos;
                        cm.pianodep = 1;
                        cm.h_dep = dmDB->RitornaAltezzedaPosizione(cm.posdep, cm.pianodep, "HDEP");
                        // cm.corsia_prel = dmDB->FilaPosizione(m.posprel);

                        if ((cm.posdep > 0) && (cm.pianodep > 0) && (cm.h_dep > 0)) {
                            cm.posprel = dmDBImpianto->CercaUDCinH(cm.IDUDC);
                            if (cm.posprel > 0) {
                                cm.pianoprel = 1;
                                cm.h_prel = dmDB->RitornaAltezzedaPosizione(cm.posprel, cm.pianoprel, "HPREL");
                                if (dmDB->PresenzaCentroMissione(cm.posprel, 0) == 0) {
                                    if (dmDB->PosPresenteMissioneAttiva(cm.posprel) == 0) {
                                        res = dmDB->GeneraCentroMissione(cm);
                                    }
                                }
                                if (res > 0) {
                                    // dmDBServer->AggiornaStatoCentroMissioni(m.idcentromissioni, 1);
                                    ok_genera = 1;
                                }
                            }
                        }
                    }
                }
            }

        }

    }
    catch (...) {}
    return res;
}

AnsiString TdmDBImpianto::TornaArticoloDaHMC_ORDINI_IN_LAVORAZIONEcopia(AnsiString posizione) {
    AnsiString stringa, res = "";
    AnsiString strsql;
    TADOQuery *ADOQuery;

    try {
        if (!dmDB->ADOConnection1->Connected)
            return res;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "SELECT * FROM HMC_ORDINI_IN_LAVORAZIONE_Copia where Posizione='" + posizione + "'";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(stringa);
        ADOQuery->Open();
        if (ADOQuery->RecordCount > 0) {
            res = ADOQuery->FieldByName("Articolo")->AsString.Trim();
        }
        ADOQuery->Close();
        // MainForm->LogMsg(stringa);
    }
    catch (...) {

    }
    delete ADOQuery;
    return res;
}
