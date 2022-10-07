// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "port.h"
#include "SocketComunicazioneClient.h"
#include "db.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPortStatus *FormPortStatus;

// ---------------------------------------------------------------------------
__fastcall TFormPortStatus::TFormPortStatus(TComponent* Owner) : TForm(Owner) {}

// ---------------------------------------------------------------------------

void __fastcall TFormPortStatus::FormActivate(TObject *Sender) {
    activate = true;

    Label4->Caption = IntToStr(idport);
    Label10->Caption = dmDB->TabPorte[IntToStr(idport)]["NOMEPORTA"];
    Edit1->Enabled = dmDB->pwdlevel;
    Edit3->Enabled = dmDB->pwdlevel;
    Edit4->Enabled = dmDB->pwdlevel;
    GroupBox1->Enabled = (ClientData.ParametriFunzionali.Simula || dmDB->pwdlevel);
    GroupBox2->Enabled = ClientData.ParametriFunzionali.Simula;
    Edit1->Text = dmDB->PosizioniPortaAperta(idport);
    Edit2->Text = dmDB->TabPorte[IntToStr(idport)]["DESCRIZIONE"];
    Edit3->Text = dmDB->PosizioniRichiestaAperturaPorta(idport);
    Edit4->Text = dmDB->StrConsensoPorta(idport);
    RadioGroup2->ItemIndex = dmDB->StatoPorta(idport);
    RadioGroup2->Enabled = dmDB->pwdlevel;
    SpeedButton1->Down = !ClientData.ParametriFunzionali.Simula;
    forzatura_attiva = false;
    Timer1Timer(this);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPortStatus::Timer1Timer(TObject *Sender) {
    // aggiorno status
    int i;
    Timer1->Enabled = false;
    if (!forzatura_attiva) {
        if ((activate) || (SpeedButton1->Down) || ((!dmDB->pwdlevel) && (!ClientData.ParametriFunzionali.Simula))) {
            CheckBox1->Checked = ClientData.DatiPorte[idport].ApriPorta;
            CheckBox2->Checked = ClientData.DatiPorte[idport].ChiudiPorta;
            CheckBox3->Checked = ClientData.DatiPorte[idport].IngombroAGV;
            CheckBox4->Checked = ClientData.DatiPorte[idport].AGVInZona;
            ckInput1->Checked = ClientData.DatiPorte[idport].PortaAperta;
            ckInput2->Checked = ClientData.DatiPorte[idport].PortaChiusa;
            ckInput3->Checked = ClientData.DatiPorte[idport].PortaReady;
        }
        count_forzatura_attiva = 0;
    }
    else {
        if (count_forzatura_attiva > 10) {
            count_forzatura_attiva = 0;
            forzatura_attiva = false;
        }
        else {
            count_forzatura_attiva++;
        }
    }
    Edit3->Enabled = dmDB->pwdlevel;
    Edit4->Enabled = dmDB->pwdlevel;

    activate = false;
    Timer1->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPortStatus::btOkClick(TObject *Sender) {
    AnsiString stringa;
    dmDB->QueryPos->Close();
    dmDB->QueryPos->SQL->Clear();
    stringa = "Update Porte Set Descrizione = '" + Edit2->Text + "'";
    stringa = stringa + " , PosApriPorta = '" + Edit3->Text + "'";
    if (dmDB->pwdlevel) {
        stringa = stringa + " , PosPortaAperta = '" + Edit1->Text + "'";
        stringa = stringa + " , StrConsenso = '" + Edit4->Text + "'";
    }
    stringa = stringa + " where IdPorta = " + IntToStr(idport);
    dmDB->QueryPos->SQL->Append(stringa);
    dmDB->QueryPos->ExecSQL();
    dmDB->QueryPos->Close();
    dmDB->aggiorna_tab_porte_locale = 1;
    dmDB->LogMsg("Aggiorna porte " + stringa);
    Close();
}

// ---------------------------------------------------------------------------

void __fastcall TFormPortStatus::FormClose(TObject *Sender, TCloseAction &Action) {
    Timer1->Enabled = false;

}

// ---------------------------------------------------------------------------
void __fastcall TFormPortStatus::FormDeactivate(TObject *Sender) {
    Close();
}

// ---------------------------------------------------------------------------

void __fastcall TFormPortStatus::btEsciClick(TObject *Sender) {
    Close();
}
// ---------------------------------------------------------------------------

void __fastcall TFormPortStatus::RadioGroup2Click(TObject *Sender) {
    AnsiString stringa, str;
    if (!activate) {
        SocketDataModule->InviaStatoPorta(idport, RadioGroup2->ItemIndex);
        dmDB->LogMsg("Modificata Modalità Porta " + IntToStr(idport) + " in : " + IntToStr(RadioGroup2->ItemIndex));
    }

}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

void __fastcall TFormPortStatus::ckInput1Click(TObject *Sender) {
    int i, val;
    TCheckBox *Check;
    if ((!activate) && (ClientData.ParametriFunzionali.Simula)) {
        val = 0;
        for (i = 0; i < 3; i++) {
            Check = (TCheckBox*) FindComponent("ckInput" + IntToStr(i + 1));
            if ((Check != NULL) && (Check->Checked))
                val |= dmDB->bit[i];
        }
        ClientData.DatiPorte[idport].forzature_attive = 1;
        SocketDataModule->InviaSimulazioneInputPorta(idport, 0, val);

    }

}
// ---------------------------------------------------------------------------
/*
 void __fastcall TFormPortStatus::BitBtn2Click(TObject *Sender) {
 int i, val;
 TCheckBox *Check;
 if ((!activate) && (dmDB->pwdlevel)) {
 val = 0;
 for (i = 0; i < 2; i++) {
 Check = (TCheckBox*) FindComponent("CheckBox" + IntToStr(i + 1));
 if ((Check != NULL) && (Check->Checked))
 val |= dmDB->bit[i];
 }
 ClientData.DatiPorte[idport].forzature_attive = 1;
 SocketDataModule->InviaUscitePorta(idport, 0, val);
 }
 }
 */

void __fastcall TFormPortStatus::btSetOutClick(TObject *Sender) {
    int i, val;
    TCheckBox *Check;
    if ((!activate) && (dmDB->pwdlevel)) {
        SocketDataModule->ScriviPhoenixClient(ClientData.DatiPorte[idport].id_thread_plc, ClientData.DatiPorte[idport].DW_Addr_ApriPorta, ClientData.DatiPorte[idport].Bit_ApriPorta, CheckBox1->Checked);
        SocketDataModule->ScriviPhoenixClient(ClientData.DatiPorte[idport].id_thread_plc, ClientData.DatiPorte[idport].DW_Addr_ChiudiPorta, ClientData.DatiPorte[idport].Bit_ChiudiPorta, CheckBox2->Checked);
        SocketDataModule->ScriviPhoenixClient(ClientData.DatiPorte[idport].id_thread_plc, ClientData.DatiPorte[idport].DW_Addr_IngombroAGV, ClientData.DatiPorte[idport].Bit_IngombroAGV, CheckBox3->Checked);
        SocketDataModule->ScriviPhoenixClient(ClientData.DatiPorte[idport].id_thread_plc, ClientData.DatiPorte[idport].DW_Addr_AGVInZona, ClientData.DatiPorte[idport].Bit_AGVInZona, CheckBox4->Checked);
        ClientData.DatiPorte[idport].forzature_attive = 1;

    }

}
// ---------------------------------------------------------------------------
