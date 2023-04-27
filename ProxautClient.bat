SET FROMPATH=D:\Proxaut\Trascar_HMC\Client
SET TOPATH=C:\Proxaut\Trascar_HMC\Client
xcopy "%FROMPATH%\ProxautClient.exe" "%TOPATH%\ProxautClient.exe" /e/y
xcopy "%FROMPATH%\\*.crs" "%TOPATH%\Client\" /e/y
xcopy "%FROMPATH%\\allarmi.txt" "%TOPATH%\" /e/y
xcopy "%FROMPATH%\\*.bmp" "%TOPATH%\" /e/y


cd "%FROMPATH%"
start "" "%FROMPATH%\ProxautClient.exe"