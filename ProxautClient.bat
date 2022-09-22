xcopy "\\Pan-Room-Supervision\Proxaut\BimboMexLGV\Client\ProxautClient.exe" "C:\Proxaut\BimboMexLGV\Client\ProxautClient.exe" /e/y
xcopy "\\Pan-Room-Supervision\Proxaut\BimboMexLGV\Client\*.crs" "C:\Proxaut\BimboMexLGV\Client\" /e/y
xcopy "\\Pan-Room-Supervision\Proxaut\BimboMexLGV\Client\allarmi.txt" "C:\Proxaut\BimboMexLGV\Client\" /e/y
xcopy "\\Pan-Room-Supervision\Proxaut\BimboMexLGV\Client\*.bmp" "C:\Proxaut\BimboMexLGV\Client\" /e/y


cd "C:\Proxaut\BimboMexLGV\Client\"
start "" "C:\Proxaut\BimboMexLGV\Client\ProxautClient.exe"