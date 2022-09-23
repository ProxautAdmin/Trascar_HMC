// ---------------------------------------------------------------------------

#pragma hdrstop

#include <vcl.h>
#include "stdio.h"
#include <System.IOUtils.hpp>
#include <System.SyncObjs.hpp>
#include "db.h"
#include "main.h"
#include "ExtraFunction.h"
#include <IdMessageBuilder.hpp>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TdmExtraFunction *dmExtraFunction;

// ---------------------------------------------------------------------------
__fastcall TdmExtraFunction::TdmExtraFunction(TComponent* Owner)
    : TDataModule(Owner)
{
}

// ---------------------------------------------------------------------------
AnsiString TdmExtraFunction::nomeIstanza(AnsiString strsql) {
    // restituisce il nome dell'istanza dalla stringa passata
    int startpos, endpos;
    startpos = AnsiPos("Source", strsql) + 7;
    strsql = strsql.SubString(AnsiPos("Source", strsql) + 7, strsql.Length() - startpos + 1);

    endpos = (AnsiPos(";", strsql) - 1 < strsql.Length() ? AnsiPos(";", strsql) - 1 : strsql.Length());
    strsql = strsql.SubString(1, endpos);

    // strsql=  dmDB->ADOConnection1->ConnectionString.SubString(1,AnsiPos(";", dmDB->ADOConnection1->ConnectionString.SubString(AnsiPos("Source", dmDB->ADOConnection1->ConnectionString)+7, dmDB->ADOConnection1->ConnectionString.Length()-(AnsiPos("Source",dmDB->ADOConnection1->ConnectionString )+7)) )-1);
    return strsql;
}

AnsiString TdmExtraFunction::PadS(AnsiString strt, int lenp, AnsiString padc) {
    // allinea la stringa a sinistra riempendo per la lunghezza lenp con la stringa padc
    AnsiString ret;
    for (int i = 1; i <= lenp - strt.Length(); i++) {
        ret = ret + padc;
    }
    ret = ret + strt;
    return ret;
}

AnsiString TdmExtraFunction::PadR(AnsiString strt, int lenp, AnsiString padc) {
    // allinea la stringa a sinistra riempendo per la lunghezza lenp con la stringa padc
    AnsiString ret;
    for (int i = 1; i <= lenp - strt.Length(); i++) {
        ret = ret + padc;
    }
    ret = strt + ret;
    return ret;
}

AnsiString TdmExtraFunction::StringNumToLetter(AnsiString stringnum) {
    // allinea la stringa a sinistra riempendo per la lunghezza lenp con la stringa padc
    AnsiString ret = "";

    if (stringnum == "1")
        ret = "A";
    else if (stringnum == "2")
        ret = "B";
    else if (stringnum == "3")
        ret = "C";
    else if (stringnum == "4")
        ret = "D";
    else if (stringnum == "5")
        ret = "E";
    else if (stringnum == "6")
        ret = "F";

    return ret;

}

void TdmExtraFunction::EsportaExcel(AnsiString SelectStr, AnsiString Nomefile) {
    FILE *esporta;
    AnsiString filename, logstr = "", msgstringa, sss;
    boolean status = false;
    if (SelectStr != "") {
        if (!DirectoryExists("Export")) {
            CreateDir("Export");
        }
        filename = "Export\\" + Nomefile + ".csv";
        if (FileExists(filename))
            DeleteFile(filename);
        if ((esporta = fopen(filename.c_str(), "a+")) != NULL) {
            if (dmDB->ADOConnection1->Connected) {
                dmDB->QueryPos->Close();
                dmDB->QueryPos->SQL->Clear();
                dmDB->QueryPos->SQL->Append(SelectStr);
                dmDB->QueryPos->Open();
                if (dmDB->QueryPos->RecordCount > 0) {
                    // header
                    logstr = "";
                    for (int j = 0; j < dmDB->QueryPos->FieldCount; j++) {
                        sss = dmDB->QueryPos->FieldList->Fields[j]->FieldName;
                        logstr = logstr + sss + ";";
                    }
                    fprintf(esporta, "%s\n", logstr.c_str());

                    dmDB->QueryPos->First();
                    while (!dmDB->QueryPos->Eof) {
                        logstr = "";
                        for (int j = 0; j < dmDB->QueryPos->FieldCount; j++) {
                            sss = StringReplace(dmDB->QueryPos->FieldList->Fields[j]->AsString, ",", "_", TReplaceFlags() << rfReplaceAll);
                            sss = StringReplace(dmDB->QueryPos->FieldList->Fields[j]->AsString, " ", "_", TReplaceFlags() << rfReplaceAll);

                            logstr = logstr + sss + ";";
                        }
                        fprintf(esporta, "%s\n", logstr.c_str());
                        dmDB->QueryPos->Next();
                    }
                }
                dmDB->QueryPos->Close();
                fflush(esporta);
                fclose(esporta);
                status = true;
            }
        }
    }
    if (status) {
        msgstringa = "File " + filename + " esportato correttamente";
        Application->MessageBox(L"File generato correttamente", MB_OK);
    }
    else
        Application->MessageBox(L"C'e' stato un errore nell'esportazione", L"File non creato", MB_OK);
}

void TdmExtraFunction::ComboNumProg(TComboBox * tcbCombo, int nProg) {
    // riempio codice articolo combo box
    TComboBox *cbCombo;
    cbCombo = tcbCombo;
    cbCombo->Items->Clear();
    for (int i = 0; i <= nProg; i++) {
        cbCombo->Items->Append(String(i));
    }
    cbCombo->Text = "0";
}

void TdmExtraFunction::ComboNomeCorsia(TComboBox * tcbCombo, int piena, AnsiString appendRecord, AnsiString valoredef) {
    // riempio codice articolo combo box
    TComboBox *cbCombo;
    cbCombo = tcbCombo;
    cbCombo->Items->Clear();
    try {
        if (appendRecord != "")
            cbCombo->Items->Append(appendRecord);
        if (dmDB->ADOConnection1->Connected) {
            dmDB->QueryPos->Close();
            dmDB->QueryPos->SQL->Clear();
            if (piena)
                dmDB->QueryPos->SQL->Append("Select NomeCorsia from Corsie where ((abilitata =1) and (Vuota = 0) AND (NomeCorsia IS NOT NULL) ) group by nomecorsia order by NomeCorsia");
            else
                dmDB->QueryPos->SQL->Append("Select NomeCorsia from Corsie where ((abilitata =1) and (piena = 0) AND (NomeCorsia IS NOT NULL) ) group by nomecorsia order by NomeCorsia");
            dmDB->QueryPos->Open();
            if (dmDB->QueryPos->RecordCount > 0) {
                dmDB->QueryPos->First();
                if (valoredef == "")
                    valoredef = dmDB->QueryPos->FieldByName("NomeCorsia")->AsString;
                while (!dmDB->QueryPos->Eof) {
                    cbCombo->Items->Append(dmDB->QueryPos->FieldByName("NomeCorsia")->AsString);
                    dmDB->QueryPos->Next();
                }
            }
            dmDB->QueryPos->Close();
            cbCombo->Text = valoredef;
        }
    }
    catch (...) {
    }
}

void TdmExtraFunction::ComboNumFileCorsia(TComboBox * tcbCombo, AnsiString Nomecorsia, AnsiString appendRecord) {
    // riempio codice articolo combo box
    TComboBox *cbCombo;
    cbCombo = tcbCombo;
    cbCombo->Items->Clear();
    AnsiString valoredef = "";
    try {
        if (appendRecord != "")
            cbCombo->Items->Append(appendRecord);
        if (dmDB->ADOConnection1->Connected) {
            dmDB->QueryPos->Close();
            dmDB->QueryPos->SQL->Clear();
            dmDB->QueryPos->SQL->Append("Select FilaInCorsia from Corsie where NomeCorsia = '" + Nomecorsia + "' order by filaincorsia");
            dmDB->QueryPos->Open();
            if (dmDB->QueryPos->RecordCount > 0) {
                dmDB->QueryPos->First();
                while (!dmDB->QueryPos->Eof) {
                    cbCombo->Items->Append(dmDB->QueryPos->FieldByName("FilaInCorsia")->AsString);
                    dmDB->QueryPos->Next();
                }
            }
            dmDB->QueryPos->Close();
            cbCombo->Text = valoredef;
        }
    }
    catch (...) {
    }
}

AnsiString TdmExtraFunction::RiempiTipoUDC(TComboBox * tcbCombo, AnsiString cerca) {
    // riempio codice articolo combo box
    TComboBox *cbCombo;
    cbCombo = tcbCombo;
    AnsiString stringa, sval, ret = "";
    tcbCombo->Items->Clear();
    stringa = "Select * from TabTipoUDC order by IDTipoUDC ";
    try {
        dmDB->QueryTipologie->Close();
        dmDB->QueryTipologie->SQL->Clear();
        dmDB->QueryTipologie->SQL->Append(stringa);
        dmDB->QueryTipologie->Open();
        if (dmDB->QueryTipologie->RecordCount) {
            dmDB->QueryTipologie->First();
            while (!dmDB->QueryTipologie->Eof) {
                sval = Trim(dmExtraFunction->PadS(dmDB->QueryTipologie->FieldByName("IDTipoUDC")->AsString, 2, "0") + " - ");
                if (dmDB->Lingua == "ITA")
                    sval = Trim(sval + dmDB->QueryTipologie->FieldByName("DESCRIZIONE")->AsString);
                else if (dmDB->Lingua == "ENG")
                    sval = Trim(sval + dmDB->QueryTipologie->FieldByName("DESCL2")->AsString);
                else if (dmDB->Lingua == "GER")
                    sval = Trim(sval + dmDB->QueryTipologie->FieldByName("DESCL3")->AsString);

                // sval = Trim(dmExtraFunction->PadS(dmDB->QueryTipologie->FieldByName("IDTipoUDC")->AsString, 2, "0") + " - " + dmDB->QueryTipologie->FieldByName("DESCRIZIONE")->AsString);

                tcbCombo->Items->Append(sval);
                if (dmExtraFunction->PadS(cerca, 2, "0") == sval.SubString(1, 2))
                    ret = sval;
                dmDB->QueryTipologie->Next();
            }
        }
        dmDB->QueryTipologie->Close();
    }
    catch (...) {
        dmDB->LogMsg("Eccezzione su RiempiTipoUDC");
    }
    return ret;
}

void TdmExtraFunction::ComboScelte(TComboBox * tcbCombo, AnsiString appendRecord) {
    // riempio codice articolo combo box
    TComboBox *cbCombo;
    cbCombo = tcbCombo;
    cbCombo->Items->Clear();
    AnsiString valoredef = "0";

    cbCombo->Items->Append("0");
    cbCombo->Items->Append("1");
    cbCombo->Items->Append("3");
    cbCombo->Text = valoredef;

}

AnsiString TdmExtraFunction::RiempiTipoStatus(TComboBox * cbStatus, AnsiString cerca) {
    // riempio codice articolo combo box
    AnsiString stringa, sval, ret = "";
    cbStatus->Items->Clear();
    stringa = "Select * from TabStatoUDC order by codstato ";
    try {
        dmDB->QueryTipologie->Close();
        dmDB->QueryTipologie->SQL->Clear();
        dmDB->QueryTipologie->SQL->Append(stringa);
        dmDB->QueryTipologie->Open();
        if (dmDB->QueryTipologie->RecordCount) {
            dmDB->QueryTipologie->First();
            while (!dmDB->QueryTipologie->Eof) {
                sval = Trim(dmExtraFunction->PadS(dmDB->QueryTipologie->FieldByName("codstato")->AsString, 2, "0") + " - ");
                if (dmDB->Lingua == "ITA")
                    sval = Trim(sval + dmDB->QueryTipologie->FieldByName("DESCRIZIONE")->AsString);
                else if (dmDB->Lingua == "ENG")
                    sval = Trim(sval + dmDB->QueryTipologie->FieldByName("DESCL1")->AsString);
                else if (dmDB->Lingua == "GER")
                    sval = Trim(sval + dmDB->QueryTipologie->FieldByName("DESCL2")->AsString);
                // sval = Trim(dmExtraFunction->PadS(dmDB->QueryTipologie->FieldByName("codstato")->AsString, 2, "0") + " - " + dmDB->QueryTipologie->FieldByName("DESCRIZIONE")->AsString);
                cbStatus->Items->Append(sval);
                if (dmExtraFunction->PadS(cerca, 2, "0") == sval.SubString(1, 2))
                    ret = sval;
                dmDB->QueryTipologie->Next();
            }
        }
        dmDB->QueryTipologie->Close();
    }
    catch (...) {
        dmDB->LogMsg("Eccezzione su RiempiTipoUDC");
    }
    return ret;
}

AnsiString TdmExtraFunction::RiempiTipoMissione(TComboBox * tcbCombo, AnsiString cerca) {
    // riempio codice articolo combo box
    TComboBox *cbCombo;
    cbCombo = tcbCombo;
    AnsiString stringa, sval, ret = "";
    tcbCombo->Items->Clear();
    stringa = "Select * from TabTipoMissione order by ID ";
    try {
        dmDB->QueryTipologie->Close();
        dmDB->QueryTipologie->SQL->Clear();
        dmDB->QueryTipologie->SQL->Append(stringa);
        dmDB->QueryTipologie->Open();
        if (dmDB->QueryTipologie->RecordCount) {

            dmDB->QueryTipologie->First();
            while (!dmDB->QueryTipologie->Eof) {
                sval = Trim(dmExtraFunction->PadS(dmDB->QueryTipologie->FieldByName("ID")->AsString, 2, "0") + " - " + dmDB->QueryTipologie->FieldByName("DESC")->AsString);
                tcbCombo->Items->Append(sval);
                if (dmExtraFunction->PadS(cerca, 2, "0") == sval.SubString(1, 2))
                    ret = sval;
                dmDB->QueryTipologie->Next();
            }
        }
        dmDB->QueryTipologie->Close();
    }
    catch (...) {
        dmDB->LogMsg("Eccezzione su RiempiTipoMissione");
    }
    return ret;
}

AnsiString TdmExtraFunction::RiempiTipoMovimento(TComboBox * tcbCombo, AnsiString cerca) {
    // riempio codice articolo combo box
    TComboBox *cbCombo;
    cbCombo = tcbCombo;
    AnsiString stringa, sval, ret = "";
    tcbCombo->Items->Clear();
    stringa = "Select * from TabTipoMovimento order by ID ";
    try {
        dmDB->QueryTipologie->Close();
        dmDB->QueryTipologie->SQL->Clear();
        dmDB->QueryTipologie->SQL->Append(stringa);
        dmDB->QueryTipologie->Open();
        if (dmDB->QueryTipologie->RecordCount) {
            dmDB->QueryTipologie->First();
            while (!dmDB->QueryTipologie->Eof) {
                sval = Trim(dmExtraFunction->PadS(dmDB->QueryTipologie->FieldByName("ID")->AsString, 2, "0") + " - ");
                if (dmDB->Lingua == "ITA")
                    sval = Trim(sval + dmDB->QueryTipologie->FieldByName("DESCRIZIONE")->AsString);
                else if (dmDB->Lingua == "ENG")
                    sval = Trim(sval + dmDB->QueryTipologie->FieldByName("DESCL1")->AsString);
                else if (dmDB->Lingua == "GER")
                    sval = Trim(sval + dmDB->QueryTipologie->FieldByName("DESCL2")->AsString);
                tcbCombo->Items->Append(sval);
                if (dmExtraFunction->PadS(cerca, 2, "0") == sval.SubString(1, 2))
                    ret = sval;
                dmDB->QueryTipologie->Next();
            }
        }
        dmDB->QueryTipologie->Close();
    }
    catch (...) {
        dmDB->LogMsg("Eccezzione su RiempiTipoMissione");
    }
    return ret;
}

AnsiString TdmExtraFunction::RiempiEsitoMissione(TComboBox * tcbCombo, AnsiString cerca) {
    // riempio codice articolo combo box
    TComboBox *cbCombo;
    cbCombo = tcbCombo;
    AnsiString stringa, sval, ret = "";
    tcbCombo->Items->Clear();
    stringa = "Select * from TabEsitoMissione order by ID ";
    try {
        dmDB->QueryTipologie->Close();
        dmDB->QueryTipologie->SQL->Clear();
        dmDB->QueryTipologie->SQL->Append(stringa);
        dmDB->QueryTipologie->Open();
        if (dmDB->QueryTipologie->RecordCount) {
            dmDB->QueryTipologie->First();
            while (!dmDB->QueryTipologie->Eof) {
                sval = Trim(dmDB->QueryTipologie->FieldByName("DESCRIZIONE")->AsString);
                tcbCombo->Items->Append(sval);
                if (cerca == sval)
                    ret = sval;
                dmDB->QueryTipologie->Next();
            }
        }
        dmDB->QueryTipologie->Close();
    }
    catch (...) {
        dmDB->LogMsg("Eccezzione su RiempiTipoMissione");
    }
    return ret;
}

AnsiString TdmExtraFunction::RiempiStatoMissione(TComboBox * tcbCombo, AnsiString cerca) {
    // riempio codice articolo combo box
    TComboBox *cbCombo;
    cbCombo = tcbCombo;
    AnsiString stringa, sval, ret = "";
    tcbCombo->Items->Clear();
    stringa = "Select * from TabStatoMissione order by ID ";
    try {
        dmDB->QueryTipologie->Close();
        dmDB->QueryTipologie->SQL->Clear();
        dmDB->QueryTipologie->SQL->Append(stringa);
        dmDB->QueryTipologie->Open();
        if (dmDB->QueryTipologie->RecordCount) {
            dmDB->QueryTipologie->First();
            while (!dmDB->QueryTipologie->Eof) {
                sval = Trim(dmExtraFunction->PadS(dmDB->QueryTipologie->FieldByName("ID")->AsString, 2, "0") + " - " + dmDB->QueryTipologie->FieldByName("DESC")->AsString);
                tcbCombo->Items->Append(sval);
                if (dmExtraFunction->PadS(cerca, 2, "0") == sval.SubString(1, 2))
                    ret = sval;
                dmDB->QueryTipologie->Next();
            }
        }
        dmDB->QueryTipologie->Close();
    }
    catch (...) {
        dmDB->LogMsg("Eccezzione su RiempiTipoMissione");

    }
    return ret;
}

AnsiString TdmExtraFunction::RiempiTipologiaCorsia(TComboBox * tcbCombo, AnsiString cerca) {
    // riempio codice articolo combo box
    AnsiString stringa, sval, ret = "";
    TComboBox *cbCombo;
    cbCombo = tcbCombo;
    cbCombo->Items->Clear();
    stringa = "Select * from TabTipologiaCorsia where (codtipo<20 or  codtipo>=99) order by CodTipo ";
    try {
        dmDB->QueryTipologie->Close();
        dmDB->QueryTipologie->SQL->Clear();
        dmDB->QueryTipologie->SQL->Append(stringa);
        dmDB->QueryTipologie->Open();
        if (dmDB->QueryTipologie->RecordCount) {
            dmDB->QueryTipologie->First();
            while (!dmDB->QueryTipologie->Eof) {

                sval = dmExtraFunction->PadS(dmDB->QueryTipologie->FieldByName("CodTipo")->AsString, 2, "0") + " - ";
                if (dmDB->Lingua == "ITA")
                    sval = Trim(sval + dmDB->QueryTipologie->FieldByName("DESCRIZIONE")->AsString);
                else if (dmDB->Lingua == "ENG")
                    sval = Trim(sval + dmDB->QueryTipologie->FieldByName("DESCL1")->AsString);
                else if (dmDB->Lingua == "GER")
                    sval = Trim(sval + dmDB->QueryTipologie->FieldByName("DESCL2")->AsString);

                // sval = dmExtraFunction->PadS(dmDB->QueryTipologie->FieldByName("CodTipo")->AsString, 2, "0") + " - " + dmDB->QueryTipologie->FieldByName("DESCRIZIONE")->AsString;
                cbCombo->Items->Append(sval);
                if (dmExtraFunction->PadS(cerca, 2, "0") == sval.SubString(1, 2))
                    ret = sval;
                dmDB->QueryTipologie->Next();
            }
        }
        dmDB->QueryTipologie->Close();
    }
    catch (...) {
        dmDB->LogMsg("Eccezzione su RiempiTipologia Corsia");
    }
    return ret;
}

int TdmExtraFunction::StoricizzaTabelle(AnsiString tabella, AnsiString tabellaDest, AnsiString campo, AnsiString periodo, int periododaeliminare) {
    AnsiString strsql;
    TADOQuery *ADOQuery;
    int res = 0;
    /*
     ATTENZIONE: se c'e' una identity column serve prima disattivarla con
     SET IDENTITY_INSERT History_batteria ON
     e alla fine
     SET IDENTITY_INSERT History_batteria OFF
     in questo caso ho tolto l'identity column dalla tabella di destinazione
     */
    if (!dmDB->ADOConnection1->Connected)
        return 0;
    try {
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        // trasferisco record piu' vecchia di mesidaeliminare mesi
        strsql = "INSERT INTO " + tabellaDest + " SELECT * FROM " + tabella + " WHERE  " + campo + " < DATEADD(" + periodo + ", -" + String(periododaeliminare) + ", GETDATE())";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(strsql);
        ADOQuery->ExecSQL();
        dmDB->LogMsg(strsql);
        // elimino
        strsql = "DELETE FROM " + tabella + " WHERE  " + campo + " < DATEADD(" + periodo + ", -" + String(periododaeliminare) + ", GETDATE())";
        ADOQuery->Close();
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Append(strsql);
        ADOQuery->ExecSQL();
        dmDB->LogMsg(strsql);
        res = 1;
    }
    catch (...) {
        // ADOConnection1->Close();
        dmDB->LogMsg("Eccezione storicizzazione tabella " + tabella + ": " + strsql);
    }
    delete ADOQuery;
    return res;
}

int TdmExtraFunction::StoricizzaTutto(AnsiString periodo, int periododaeliminare) {
    int cont = 0;
    cont = cont + StoricizzaTabelle("Log", "History_Log", "dataora", periodo, 1);
    cont = cont + StoricizzaTabelle("allarmi", "History_Allarmi", "dataora", periodo, periododaeliminare);
    cont = cont + StoricizzaTabelle("batteria", "History_Batteria", "dataora", periodo, periododaeliminare);
    cont = cont + StoricizzaTabelle("CentroMissioni", "History_CentroMissioni", "fine", periodo, periododaeliminare);
    cont = cont + StoricizzaTabelle("missioni", "History_missioni", "generata", periodo, periododaeliminare);
    return cont;
}

int TdmExtraFunction::bitRead(int value, int bit) {
    // leggi bit
    return (((value) >> (bit)) & 0x01);
}

int TdmExtraFunction::bitSet(int value, int bit) {
    // setta il bit a 1
    return ((value) |= (1UL << (bit)));
}

int TdmExtraFunction::bitClear(int value, int bit) {
    // setta il bit a 0
    return ((value) &= ~(1UL << (bit)));
}

int TdmExtraFunction::bitWrite(int value, int bit, int bitvalue) {
    // setta il bit scelto al valore impostato
    return (bitvalue ? bitSet(value, bit) : bitClear(value, bit));
}

float TdmExtraFunction::ProporzionaZoom(int zz, float parametro) {
    float ret = 1;
    ret = zz * parametro;
    if (ret < 1)
        ret = 1;
    return ret;
}

int TdmExtraFunction::SendEmail(AnsiString message) {
    // https://www.youtube.com/watch?v=Wa-lNd82X8Y&t=203s
    int ret = 0;
    IdSMTP1->Host = "smtp.gmail.com";
    IdSMTP1->Username = "proxaut.impianti@gmail.com";
    IdSMTP1->Password = "proxAut2!";

    IdMessage1->Clear();
    IdMessage1->Recipients->EMailAddresses = "scordamaglia.m@proxaut.com";
    IdMessage1->Subject = "Test " + FormatDateTime("d mmmm yyyy h':'mm':'ss", Now());
    IdMessage1->Body->Text = message + " - " + FormatDateTime("d mmmm yyyy h':'mm':'ss", Now());

    TIdMessageBuilderPlain* MyMessage = new TIdMessageBuilderPlain();
    try {
        IdSMTP1->Connect();
        IdSMTP1->Send(IdMessage1);
        ret = 1;
    }
    catch (...) {
        ShowMessage("error");
        ret = 0;
    }
    IdSMTP1->Disconnect();
    delete MyMessage;
    return ret;
}

void TdmExtraFunction::CheckAlarm() {
    AnsiString stringa, dataora;
    TADOQuery *ADOQuery;
    int res = 0;
    try {
        if (!dmDB->ADOConnection1->Connected)
            return;
        ADOQuery = new TADOQuery(NULL);
        ADOQuery->Connection = dmDB->ADOConnection1;
        stringa = "Select AGV, DATAORA FROM Allarmi WHERE (Tempo IS NULL) and (Email IS NULL) ";
        stringa = stringa + " and (dataora < DATEADD(second, 60, GETDATE()))";
        stringa = stringa + " ORDER BY DataOra ";
        ADOQuery->SQL->Clear();
        ADOQuery->SQL->Text = stringa;
        ADOQuery->Open();
        if (ADOQuery->RecordCount) {
            dataora = ADOQuery->FieldByName("AGV")->AsString;
            res = SendEmail("AGV " + ADOQuery->FieldByName("AGV")->AsString + " fermo da almeno 60 secondi");
            // se email ok
            if (res == 1) {
                stringa = "update Allarmi set email=1 WHERE (Tempo IS NULL) and (Email IS NULL) ";
                stringa = stringa + " and (dataora < DATEADD(second, 60, GETDATE()))";
                ADOQuery->SQL->Text = stringa;
                ADOQuery->ExecSQL();
            }
        }
        ADOQuery->Close();
        delete ADOQuery;
        // MainForm->LogMsg(stringa);
    }
    catch (...) {
    }
    return;
}

AnsiString TdmExtraFunction::RiempiGiorniSettimana(TComboBox * tcbCombo, AnsiString cerca) {
    // riempio codice articolo combo box
    TComboBox *cbCombo;
    cbCombo = tcbCombo;
    AnsiString stringa, sval, ret = "";
    tcbCombo->Items->Clear();
    stringa = "Select * from TabGiorniSettimana order by ID ";
    try {
        dmDB->QueryTipologie->Close();
        dmDB->QueryTipologie->SQL->Clear();
        dmDB->QueryTipologie->SQL->Append(stringa);
        dmDB->QueryTipologie->Open();
        if (dmDB->QueryTipologie->RecordCount) {

            dmDB->QueryTipologie->First();
            while (!dmDB->QueryTipologie->Eof) {
                sval = Trim(dmExtraFunction->PadS(dmDB->QueryTipologie->FieldByName("ID")->AsString, 1, "0") + " - " + dmDB->QueryTipologie->FieldByName("DESCL")->AsString);
                tcbCombo->Items->Append(sval);
                if (dmExtraFunction->PadS(cerca, 1, "0") == sval.SubString(1, 1))
                    ret = sval;
                dmDB->QueryTipologie->Next();
            }
        }
        dmDB->QueryTipologie->Close();
    }
    catch (...) {
        dmDB->LogMsg("Eccezione su TabGiorniSettimana");
    }
    return ret;
}

void TdmExtraFunction::StringToChar(AnsiString varString, unsigned char *varChar) {
    // solo per evitare una var temporanea ecc. Non ho trovato uan funzione equivalente
    strcpy(varChar, varString.c_str());
    return;
}

int TdmExtraFunction::VerificaUtentePerZona(int zonaint) {
    int ret = 0;
    AnsiString ZonaUtenteStr;
    if (dmDB->UserZona.ToIntDef(0) > 26)
        ret = 99;
    else {
        ZonaUtenteStr = (char)(zonaint + 64);
        ret = dmDB->ZonaString.Pos(ZonaUtenteStr);
    }
    return ret;
}

int TdmExtraFunction::Constrain(int vval, int mmin, int mmax) {
    int ret = vval;
    vval = (vval < mmin ? mmin : vval);
    vval = (vval > mmax ? mmax : vval);
    return ret;
}

bool TdmExtraFunction::IsNum(char c) {
    return ((c >= '0') && (c <= '9'));
}

void TdmExtraFunction::ComboScelte(TComboBox * tcbCombo, int numero, int def, int start) {
    // riempio codice articolo combo box
    TComboBox *cbCombo;
    cbCombo = tcbCombo;
    cbCombo->Items->Clear();

    for (int j = start; j <= numero; j++) {
        cbCombo->Items->Append(IntToStr(j));
    }
    cbCombo->Text = def;
}
