// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
// ---------------------------------------------------------------------------
USEFORM("Main.cpp", MainForm);
USEFORM("SocketComunicazioneServer.cpp", SocketDataModule); /* TDataModule: File Type */
USEFORM("..\File Comuni\dmFunzioniComuniClientServer.cpp", dmFunzioni); /* TDataModule: File Type */
USEFORM("..\File Comuni\ExtraFunction.cpp", dmExtraFunction); /* TDataModule: File Type */
USEFORM("..\File Comuni\login.cpp", FormLogin);
USEFORM("..\File Comuni\DB.cpp", dmDB); /* TDataModule: File Type */
USEFORM("..\File Comuni\DBImpianto.cpp", dmDBImpianto); /* TDataModule: File Type */
USEFORM("dmgestionedatabase.cpp", dmDBServer); /* TDataModule: File Type */
USEFORM("dmgestioneplc.cpp", DMPlc); /* TDataModule: File Type */
USEFORM("dmgestioneporte.cpp", DMGestione_Porte); /* TDataModule: File Type */
USEFORM("datamodulecomandiagv.cpp", dmComandiAgv); /* TDataModule: File Type */
USEFORM("datamodulemodbustcp.cpp", DataModuleModBus); /* TDataModule: File Type */
USEFORM("dmgestione.cpp", DMGestioneEventi); /* TDataModule: File Type */
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    HANDLE mutex;
    try
    {
        UnicodeString MutexName = ExtractFileName(Application->ExeName).UpperCase();

        mutex = OpenMutex(MUTEX_ALL_ACCESS, false, MutexName.c_str());
        if (mutex == NULL) {
            mutex = CreateMutex(NULL, true, MutexName.c_str());
       //     ShowMessage("Ok");
            if (GetLastError() == ERROR_ALREADY_EXISTS) { // did the mutex already exist?
                ShowMessage("Lancio programma con errore");
                return -1;
            }
        }
        else {
            ShowMessage("Programma gia' in esecuzione");
          exit(EXIT_SUCCESS);    //good
         //  exit(EXIT_FAILURE);
       //    return -1
        }
        Application->Initialize();
        Application->MainFormOnTaskBar = true;
        Application->CreateForm(__classid(TdmDB), &dmDB);
         Application->CreateForm(__classid(TMainForm), &MainForm);
         Application->CreateForm(__classid(TSocketDataModule), &SocketDataModule);
         Application->CreateForm(__classid(TdmDBServer), &dmDBServer);
         Application->CreateForm(__classid(TdmComandiAgv), &dmComandiAgv);
         Application->CreateForm(__classid(TDMGestioneEventi), &DMGestioneEventi);
         Application->CreateForm(__classid(TDMGestione_Porte), &DMGestione_Porte);
         Application->CreateForm(__classid(TdmFunzioni), &dmFunzioni);
         Application->CreateForm(__classid(TdmExtraFunction), &dmExtraFunction);
         Application->CreateForm(__classid(TFormLogin), &FormLogin);
         Application->CreateForm(__classid(TDMPlc), &DMPlc);
         Application->CreateForm(__classid(TdmDBImpianto), &dmDBImpianto);
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
    ReleaseMutex(mutex);
    return 0;
}
// ---------------------------------------------------------------------------
