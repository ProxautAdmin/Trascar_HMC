@ECHO OFF
SETLOCAL
SET ISTANZA=SCORDAPROXAUT17\EXPRESS2017
REM SET PERCORSO=C:\Dati_SQL\Backup\
SET PERCORSO=C:\Dati_SQL\Backup\
SET DDATE=%date:~6,4%-%date:~3,2%-%date:~0,2%
REM Get date in format YYYY-MM-DD (assumes the locale is the United States)
FOR /F "tokens=1,2,3,4 delims=/ " %%A IN ('Date /T') DO SET NowDate=%%D-%%B-%%C
REM Build a list of databases to backup
SET DBList=%SystemDrive%SQLDBList.txt
SqlCmd -S %ISTANZA% -h-1 -W -Q "SET NoCount ON; SELECT Name FROM master.dbo.sysDatabases WHERE [Name] NOT IN ('master','model','msdb','tempdb')" > "%DBList%"
REM Backup each database, prepending the date to the filename
FOR /F "tokens=*" %%I IN (%DBList%) DO (
ECHO Backing up database: %%I
SqlCmd -S %ISTANZA% -Q "BACKUP DATABASE [%%I] TO Disk='%PERCORSO%%%I_%DDATE%.bak' with INIT"
ECHO.
)
REM Clean up the temp file
IF EXIST "%DBList%" DEL /F /Q "%DBList%"
forfiles /P %PERCORSO% /S /M *.BAK /D -30 /C "cmd /c del @PATH"
ENDLOCAL




