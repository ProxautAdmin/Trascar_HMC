@ECHO OFF
SETLOCAL
SET ISTANZA=CPCPROXAUT\SQLEXPRESS2019
SET PERCORSO=C:\Proxaut\Backup\SQLBackup\
REM Get date in format YYYY-MM-DD (assumes the locale is the United States)
FOR /F "tokens=1,2,3,4 delims=/ " %%A IN ('Date /T') DO SET NowDate=%%D-%%B-%%C
REM Build a list of databases to backup
SET DBList=%SystemDrive%SQLDBList.txt
SqlCmd -E -S %ISTANZA% -h-1 -W -Q "SET NoCount ON; SELECT Name FROM master.dbo.sysDatabases WHERE [Name] NOT IN ('master','model','msdb','tempdb')" > "%DBList%"
REM Backup each database, prepending the date to the filename
FOR /F "tokens=*" %%I IN (%DBList%) DO (
ECHO Backing up database: %%I
SqlCmd -E -S %ISTANZA% -Q "BACKUP DATABASE [%%I] TO Disk='%PERCORSO%%%I.bak' with INIT"
ECHO.
)
REM Clean up the temp file
IF EXIST "%DBList%" DEL /F /Q "%DBList%"
forfiles /P %PERCORSO% /S /M *.BAK /D -30 /C "cmd /c del @PATH"
ENDLOCAL
