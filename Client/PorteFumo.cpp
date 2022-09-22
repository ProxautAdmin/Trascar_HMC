// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PorteFumo.h"
#include "SocketComunicazioneClient.h"
#include "db.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormStatusPorteFumo *FormStatusPorteFumo;

// ---------------------------------------------------------------------------
__fastcall TFormStatusPorteFumo::TFormStatusPorteFumo(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
void __fastcall TFormStatusPorteFumo::Timer1Timer(TObject *Sender)
{
    // aggiorno status
    int i;
    Timer1->Enabled = false;
    if (!forzatura_attiva) {
        if ((activate) || (SpeedButton1->Down) || ((!dmDB->pwdlevel) && (!ClientData.Simula))) {
            cbInput1->Checked = ClientData.DatiPorteFumo[idport].PortaAperta[1];
            cbInput2->Checked = ClientData.DatiPorteFumo[idport].PortaChiusa[1];
            cbInput3->Checked = ClientData.DatiPorteFumo[idport].PortaAperta[2];
            cbInput4->Checked = ClientData.DatiPorteFumo[idport].PortaChiusa[2];
            eInput1->Text = ClientData.DatiPorteFumo[idport].StatoClima;
            eInput2->Text = ClientData.DatiPorteFumo[idport].StatoPorta[1];
            eInput3->Text = ClientData.DatiPorteFumo[idport].StatoPorta[2];
			eInput4->Text = ClientData.DatiPorteFumo[idport].StatoCaricoScarico;
			RadioGroup1->ItemIndex = ClientData.DatiPorteFumo[idport].uscitedascrivere;

        }
        count_forzatura_attiva = 0;
    }
    else {
        if (count_forzatura_attiva > 10) {
            count_forzatura_attiva = 0;
            forzatura_attiva = false;
        }
        else
            count_forzatura_attiva++;
    }
    activate = false;
    Timer1->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormStatusPorteFumo::BitBtn1Click(TObject *Sender)
{
    AnsiString stringa;
    dmDB->QueryPos->Close();
    dmDB->QueryPos->SQL->Clear();
    stringa = "Update PorteFumo Set Descrizione = '" + Edit2->Text + "'";
	stringa = stringa + " , PosApriPorta = '" + Edit1->Text + "'";
	if (dmDB->pwdlevel) {
		stringa = stringa + " , PosPortaAperta = '" + Edit3->Text + "'";
        stringa = stringa + " , StrConsenso = '" + Edit4->Text + "'";
    }
	stringa = stringa + " where IdPorta = " + IntToStr(idport);
	dmDB->QueryPos->SQL->Append(stringa);
    dmDB->QueryPos->ExecSQL();
    dmDB->QueryPos->Close();
        dmDB->LogMsg("Aggiorna porte fumo " + stringa);
    dmDB->aggiorna_tab_portefumo_locale = 1;
    Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormStatusPorteFumo::FormActivate(TObject *Sender)
{
    activate = true;

    Label4->Caption = IntToStr(idport);
    Label10->Caption = dmDB->TabPorteFumo[IntToStr(idport)]["NOMEPORTA"];

    Edit4->Enabled = dmDB->pwdlevel;
    GroupBox2->Enabled = (ClientData.Simula || dmDB->pwdlevel);
	Edit1->Text = dmDB->PosizioniRichiestaAperturaPortaFumo(idport);
	Edit2->Text = dmDB->TabPorteFumo[IntToStr(idport)]["DESCRIZIONE"];
	Edit3->Text = dmDB->PosizioniPortaFumoAperta(idport);
	Edit4->Text = dmDB->StrConsensoPortaFumo(idport);
    RadioGroup2->ItemIndex = dmDB->StatoPortaFumo(idport);
    RadioGroup2->Enabled = dmDB->pwdlevel;
    SpeedButton1->Down = !ClientData.Simula;
    forzatura_attiva = false;
    Timer1Timer(this);
}

// ---------------------------------------------------------------------------
void __fastcall TFormStatusPorteFumo::FormClose(TObject *Sender,
    TCloseAction &Action)
{
    Timer1->Enabled = false;

}

// ---------------------------------------------------------------------------
void __fastcall TFormStatusPorteFumo::FormDeactivate(TObject *Sender)
{
    Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormStatusPorteFumo::RadioGroup1Click(TObject *Sender)
{
	if ((!activate) && (dmDB->pwdlevel)) {
        forzatura_attiva = true;
		SocketDataModule->InviaUscitePortaFumo(idport, 0, RadioGroup1->ItemIndex);
	}

}
// ---------------------------------------------------------------------------

void __fastcall TFormStatusPorteFumo::BitBtn3Click(TObject *Sender)
{
    Close();
}
// ---------------------------------------------------------------------------

void __fastcall TFormStatusPorteFumo::RadioGroup2Click(TObject *Sender)
{
    AnsiString stringa, str;
    if (!activate) {
        SocketDataModule->InviaStatoPortaFumo(idport, RadioGroup2->ItemIndex);
        dmDB->LogMsg("Modificata Modalità Porta " + IntToStr(idport) + " in : " + IntToStr(RadioGroup2->ItemIndex));
    }

}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

void __fastcall TFormStatusPorteFumo::CheckBox1Click(TObject *Sender)
{
    if ((!activate) && (dmDB->pwdlevel)) {
        forzatura_attiva = true;
        ///????    SocketDataModule->ScriviPhoenixClient(0, ClientData.DatiPorte[idport].DW_Addr_ApriPorta, ClientData.DatiPorte[idport].Bit_ApriPorta, CheckBox1->Checked);
    }

}
// ---------------------------------------------------------------------------

void __fastcall TFormStatusPorteFumo::CheckBox2Click(TObject *Sender)
{
	if ((!activate) && (dmDB->pwdlevel)) {
		forzatura_attiva = true;
        ///?      SocketDataModule->ScriviPhoenixClient(0, ClientData.DatiPorte[idport].DW_Addr_ChiudiPorta, ClientData.DatiPorte[idport].Bit_ChiudiPorta, CheckBox2->Checked);
    }

}
// ---------------------------------------------------------------------------
