// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ins_MssioniWMS.h"
#include "db.h"
#include "ExtraFunction.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFIns_MissioniWMS *FIns_MissioniWMS;

// ---------------------------------------------------------------------------
__fastcall TFIns_MissioniWMS::TFIns_MissioniWMS(TComponent* Owner)
    : TForm(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TFIns_MissioniWMS::BitBtn2Click(TObject *Sender)
{
    Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFIns_MissioniWMS::BitBtn1Click(TObject *Sender)
{
    AnsiString stringa, ora1, ora2, ora3, timemax, timemin;
    int ok;
    int maxfasciinpos = 0;


    if (insert) {
		stringa = "Insert Into WMSToProxaut (DataOraCreazioneWMS,PosizionePrelievo,PosizioneDeposito,CorsiaDeposito, StatoMissioneWMS,IDUDC,PesoUDC, TipoUDC, TipoMissione, Priorita, IDAgv, StatoMissioneProxaut)";
        stringa = stringa + " VALUES ('" + edDataOraCreazioneWMS->Text.UpperCase()+"', '" + edPosizionePrelievo->Text.UpperCase() + "','" + edPosizioneDeposito->Text.UpperCase() + "','" + edCorsiaDeposito->Text.UpperCase();
		stringa = stringa + "'," + edStatoMissioneWMS->Text.ToIntDef(0) + "," + edIDUDC->Text.ToIntDef(0) + "," + edPesoUDC->Text.ToIntDef(0) + "," + edTipoUDC->Text.ToIntDef(0) + ",'" +edTipoMissione->Text.ToIntDef(0) + "'," +edPriorita->Text.ToIntDef(0) + ","+edIDAgv->Text.ToIntDef(0)+ ","+edStatoMissioneProxaut->Text.ToIntDef(0)+")";
    }
    else {
        stringa = "Update WMSToProxaut set DataOraCreazioneWMS='" + edDataOraCreazioneWMS->Text.UpperCase() + "', PosizioneDeposito = '" + edPosizioneDeposito->Text.UpperCase() + "', CorsiaDeposito = '" + edCorsiaDeposito->Text.UpperCase() + "', ";
        stringa = stringa + " IDUDC = '" + edIDUDC->Text.UpperCase() + "', PesoUDC=" + edPesoUDC->Text.ToIntDef(0) + ", TipoUDC=" + edTipoUDC->Text.ToIntDef(0) + ", TipoMissione=" + edTipoMissione->Text.ToIntDef(0) + ", Priorita=" + edPriorita->Text.ToIntDef(0)+ ", IDAgv=" + edIDAgv->Text.UpperCase()+ ", ";
        stringa = stringa + " PosizionePrelievo = '" + edPosizionePrelievo->Text.UpperCase() + "', StatoMissioneWMS = " + edStatoMissioneWMS->Text.ToIntDef(0) + ", StatoMissioneProxaut = " + edStatoMissioneProxaut->Text.ToIntDef(0) ;
        stringa = stringa + " where Id = " + edID->Text.ToIntDef(0);
    }

    try {
        Query->Close();
        Query->SQL->Clear();
        Query->SQL->Append(stringa);
        Query->ExecSQL();
        Query->Close();
        dmDB->LogMsg(stringa);
        // dmDB->AggiornaTabAnagrafica();
        Close();
    }
    catch (...) {
        dmDB->LogMsg("Eccezione insert/update Missioni WMS");
        Query->Close();
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFIns_MissioniWMS::FormClose(TObject *Sender,
    TCloseAction &Action)
{
    insert = false;
    // FormAnagraficaArticoliUDC->FormActivate(this);

}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

