cd ..
call vendor\premake\premake5.exe vs2019
cd Editor
call ..\vendor\premake\premake5.exe vs2022
PAUSE