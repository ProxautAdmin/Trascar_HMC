// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frame_info.h"
#include "dmFunzioniComuniClientServer.h"
#include "main.h"
#include "db.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrameInfo *FrameInfo;

// ---------------------------------------------------------------------------
__fastcall TFrameInfo::TFrameInfo(TComponent* Owner)
    : TFrame(Owner)
{
}

// ---------------------------------------------------------------------------
void TFrameInfo::RiempiCampi(AnsiString struttura_hex)
{

}

void __fastcall TFrameInfo::TimerInfoTimer(TObject *Sender)
{
    AnsiString TempString = "";
    this->TimerInfo->Enabled = false;
    if (dmDB->ADOConnection1->Connected) {
        // toProx
        TempString = "";
        dmDB->QueryPos->Close();
        dmDB->QueryPos->SQL->Clear();
        dmDB->QueryPos->SQL->Append("select TOP 1 ELAB_OK, progressivo, dataora, rifgalileo, idmissione from Bitech_Proxaut where elab_OK='GO' order by progressivo DESC");
        dmDB->QueryPos->Open();
        if (dmDB->QueryPos->RecordCount > 0) {
            dmDB->QueryPos->First();
            TempString = "ID:" + dmDB->QueryPos->FieldByName("progressivo")->AsString + "  IDMis: " + dmDB->QueryPos->FieldByName("idmissione")->AsString + "  Gslileo: " + dmDB->QueryPos->FieldByName("rifgalileo")->AsString;

        }
        if (TempString != edToProx->Text) {
            edToProx->Text = TempString;
            if (TempString == "")
                lbTimeToProx->Caption = " -  -  -";
            else
                lbTimeToProx->Caption = dmDB->QueryPos->FieldByName("dataora")->AsString;
        }

        // centromissioni
        dmDB->QueryPos->Close();
        dmDB->QueryPos->SQL->Clear();
        dmDB->QueryPos->SQL->Append("Select Top 1 id, posprel,IDUDC,generata from centromissioni order by generata DESC");
        dmDB->QueryPos->Open();
        if (dmDB->QueryPos->RecordCount > 0) {
            dmDB->QueryPos->First();
            edMisC->Text = "ID:" + dmDB->QueryPos->FieldByName("id")->AsString + "  Prel: " + dmDB->QueryPos->FieldByName("posprel")->AsString + "  IDUDC: " + dmDB->QueryPos->FieldByName("IDUDC")->AsString;
            lbTimeToMission->Caption = dmDB->QueryPos->FieldByName("generata")->AsString;
        }

        // doppi udc
        TempString = "";
        dmDB->QueryPos->Close();
        dmDB->QueryPos->SQL->Clear();
        dmDB->QueryPos->SQL->Append("select Pos, (Ltrim(RTrim(NomePos))) as NomePos, fila, nomecorsia, idudc from piani_view where idudc in (select idudc from piani group by idudc having count(idudc) > 1) and idudc > 0  order by idudc desc ");
        dmDB->QueryPos->Open();
        while (!dmDB->QueryPos->Eof) {
            TempString = TempString + "UDC : " + dmDB->QueryPos->FieldByName("IDUDC")->AsString + " Pos : " + dmDB->QueryPos->FieldByName("Pos")->AsString + " row : " + dmDB->QueryPos->FieldByName("fila")->AsString + " - ";
            dmDB->QueryPos->Next();
        }
        if (TempString != edDouble->Text) {
            edDouble->Text = TempString;
            if (TempString == "")
                lbTimeDouble->Caption = " -  -  -";
            else
                lbTimeDouble->Caption = DateToStr(Now()) + " " + TimeToStr(Now());
        }

    }

    // FrameInfo->edMisC->Text=" Prel : "+IntToStr(CentroMis.posprel)+" Dep : " +IntToStr(CentroMis.posdep)+" Tipo Mis : " +IntToStr(CentroMis.CodTipoMissione);
    dmDB->QueryPos->Close();
    lbCont->Caption = ccont;
    ccont++;
    this->TimerInfo->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TFrameInfo::Button1Click(TObject *Sender)
{
    // cerco udc su mappa
    if (Edit1->Text.ToIntDef(0)) {
        // MainForm->trova_udc = Edit1->Text.ToIntDef(0);
        // MainForm->AggiornaMappa();
    }

}
// ---------------------------------------------------------------------------

void __fastcall TFrameInfo::Button3Click(TObject *Sender)
{
    MainForm->trova_udc = 0;

    MainForm->AggiornaMappa();

}
// ---------------------------------------------------------------------------
void __fastcall TFrameInfo::btZona1Click(TObject *Sender)
{
          TShiftState Shift;
    bool Hand;
    TPoint MP;
    MP.x = 700; // centro il mouse
    MP.y = 450;
    MainForm->countwheel = 3;
     MainForm->zoom=2;
    MainForm->FormMouseWheel(MainForm, Shift, 130, MP, Hand);
}
//---------------------------------------------------------------------------

void __fastcall TFrameInfo::btZona2Click(TObject *Sender)
{
             TShiftState Shift;
    bool Hand;
    TPoint MP;
    MP.x = 300; // centro il mouse
    MP.y = 100;
    MainForm->countwheel = 3;
     MainForm->zoom=2;
    MainForm->FormMouseWheel(MainForm, Shift, 130, MP, Hand);
}
//---------------------------------------------------------------------------

