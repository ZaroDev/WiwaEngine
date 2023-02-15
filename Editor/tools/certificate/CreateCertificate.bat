@echo off
set crt_ext=.cer
set pfx_ext=.pfx
set pvk_ext=.pvk

set name=%1%
set password=%2%

set cert_data=%3%

set crt_name=%name%%crt_ext%
set pfx_name=%name%%pfx_ext%
set pvk_name=%name%%pvk_ext%

IF EXIST %crt_name% DEL /F %crt_name%
IF EXIST %pfx_name% DEL /F %pfx_name%
IF EXIST %pvk_name% DEL /F %pvk_name%

echo Creating cert...
makecert.exe -r -n %cert_data% -pe -ss CA -sky signature -m 1 -a sha256 -len 2048 -sv %pvk_name%  %crt_name%
echo Creating pfx...
pvk2pfx.exe /pvk %pvk_name%  /spc %crt_name%  /pfx %pfx_name%  /pi %password% -f