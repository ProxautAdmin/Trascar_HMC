xcopy "D:\Proxaut\Trascar_HMC\Client\ProxautClient.exe" "C:\Proxaut\Trascar_HMC\Client\ProxautClient.exe" /e/y
xcopy "D:\Proxaut\Trascar_HMC\Client\*.crs" "C:\Proxaut\Trascar_HMC\Client\" /e/y
xcopy "D:\Proxaut\Trascar_HMC\Client\allarmi.txt" "C:\Proxaut\Trascar_HMC\Client\" /e/y
xcopy "D:\Proxaut\Trascar_HMC\Client\*.bmp" "C:\Proxaut\Trascar_HMC\Client\" /e/y


cd "C:\Proxaut\Trascar_HMC\Client\"
start "" "C:\Proxaut\Trascar_HMC\Client\ProxautClient.exe"