// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
// ---------------------------------------------------------------------------
USEFORM("tipologia.cpp", FormTipologia);
USEFORM("stato_pos.cpp", fCorsia);
USEFORM("SocketComunicazioneClient.cpp", SocketDataModule); /* TDataModule: File Type */
USEFORM("UDCDuplicati.cpp", FormUDCDuplicati);
USEFORM("ZonaA2Frame.cpp", frZonaA2); /* TFrame: File Type */
USEFORM("ZonaA1Frame.cpp", frZonaA1); /* TFrame: File Type */
USEFORM("UDClist.cpp", FormUDCList);
USEFORM("selectAgvframe.cpp", frSelectAgv); /* TFrame: File Type */
USEFORM("pos_UDC.cpp", frPosUDC); /* TFrame: File Type */
USEFORM("PosPrelievoPLC.cpp", FormPosPrelPLC);
USEFORM("PosDepositoPLC.cpp", FormPosDepPLC);
USEFORM("segnaliBaia.cpp", fSegnaliBaia);
USEFORM("SceltaUDC.cpp", FormSceltaUDC);
USEFORM("..\File Comuni\DBImpianto.cpp", dmDBImpianto); /* TDataModule: File Type */
USEFORM("..\File Comuni\DB.cpp", dmDB); /* TDataModule: File Type */
USEFORM("..\File Comuni\dmFunzioniComuniClientServer.cpp", dmFunzioni); /* TDataModule: File Type */
USEFORM("..\File Comuni\login.cpp", FormLogin);
USEFORM("..\File Comuni\ExtraFunction.cpp", dmExtraFunction); /* TDataModule: File Type */
USEFORM("ZonaIFrame.cpp", frZonaI); /* TFrame: File Type */
USEFORM("ZonaDFrame.cpp", frZonaD); /* TFrame: File Type */
USEFORM("ZonaCFrame.cpp", frZonaC); /* TFrame: File Type */
USEFORM("ZonaBFrame.cpp", frZonaB); /* TFrame: File Type */
USEFORM("ZonaEFrame.cpp", frZonaE); /* TFrame: File Type */
USEFORM("ZonaHFrame.cpp", frZonaH); /* TFrame: File Type */
USEFORM("ZonaGFrame.cpp", frZonaG); /* TFrame: File Type */
USEFORM("ZonaFFrame.cpp", frZonaF); /* TFrame: File Type */
USEFORM("PorteFumo.cpp", FormStatusPorteFumo);
USEFORM("configCBFrame.cpp", frCB); /* TFrame: File Type */
USEFORM("config.cpp", frConfig);
USEFORM("Chiusura.cpp", ChiusuraForm);
USEFORM("DataExchange.cpp", DataForm);
USEFORM("frame_ArticoliSovrapposti.cpp", FrameSovrapposti); /* TFrame: File Type */
USEFORM("DBExtra.cpp", fDBExtra);
USEFORM("DBClient.cpp", dmDBClient); /* TDataModule: File Type */
USEFORM("ChildWin.cpp", MDIChild);
USEFORM("allarmi.cpp", fAllarmi);
USEFORM("AGVFrameL.cpp", frAgvL); /* TFrame: File Type */
USEFORM("AGV.cpp", fAGV);
USEFORM("AllarmiAttivi.cpp", FormAllarmiAttivi);
USEFORM("CentroMissioni.cpp", fCentroMissioni);
USEFORM("Batterie.cpp", fBatterie);
USEFORM("anagrafica_articoli.cpp", FormAnagraficaArticoli);
USEFORM("missioni.cpp", fMissioni);
USEFORM("Main.cpp", MainForm);
USEFORM("log.cpp", fLog);
USEFORM("MSG.cpp", MSGForm);
USEFORM("port.cpp", FormPortStatus);
USEFORM("Password.cpp", PasswordDlg);
USEFORM("ins_Utenti.cpp", FrmInsUtenti);
USEFORM("GestAGVMidi.cpp", fGestAGVMidi);
USEFORM("frame_modulo.cpp", FrameModulo); /* TFrame: File Type */
USEFORM("frame_info.cpp", FrameInfo); /* TFrame: File Type */
USEFORM("GestMissioni.cpp", dmGestMissioni); /* TDataModule: File Type */
USEFORM("ins_UDC.cpp", FormInsUDC);
USEFORM("ins_MssioniWMS.cpp", FIns_MissioniWMS);
USEFORM("GestUser.cpp", FrmGestUser);
USEFORM("frame_ArticoliMP.cpp", FrameMatPrime); /* TFrame: File Type */
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
        Application->Initialize();
        Application->MainFormOnTaskBar = true;
        Application->CreateForm(__classid(TdmDB), &dmDB);
         Application->CreateForm(__classid(TMainForm), &MainForm);
         Application->CreateForm(__classid(TSocketDataModule), &SocketDataModule);
         Application->CreateForm(__classid(TDataForm), &DataForm);
         Application->CreateForm(__classid(TFormLogin), &FormLogin);
         Application->CreateForm(__classid(TdmDBClient), &dmDBClient);
         Application->CreateForm(__classid(TdmGestMissioni), &dmGestMissioni);
         Application->CreateForm(__classid(TChiusuraForm), &ChiusuraForm);
         Application->CreateForm(__classid(TMSGForm), &MSGForm);
         Application->CreateForm(__classid(TFormAllarmiAttivi), &FormAllarmiAttivi);
         Application->CreateForm(__classid(TdmExtraFunction), &dmExtraFunction);
         Application->CreateForm(__classid(TdmFunzioni), &dmFunzioni);
         Application->CreateForm(__classid(TPasswordDlg), &PasswordDlg);
         Application->CreateForm(__classid(TFrmInsUtenti), &FrmInsUtenti);
         Application->CreateForm(__classid(TdmDBImpianto), &dmDBImpianto);
         Application->CreateForm(__classid(TFormPortStatus), &FormPortStatus);
         Application->CreateForm(__classid(TFormPosDepPLC), &FormPosDepPLC);
         Application->CreateForm(__classid(TFormPosPrelPLC), &FormPosPrelPLC);
         Application->CreateForm(__classid(TFormUDCList), &FormUDCList);
         Application->CreateForm(__classid(TFrmGestUser), &FrmGestUser);
         Application->CreateForm(__classid(TFormInsUDC), &FormInsUDC);
         Application->CreateForm(__classid(TFormAnagraficaArticoli), &FormAnagraficaArticoli);
         Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    catch (...)
    {
        try
        {
            throw Exception("");
        }
        catch (Exception &exception)
        {
            Application->ShowException(&exception);
        }
    }
    return 0;
}
// ---------------------------------------------------------------------------
