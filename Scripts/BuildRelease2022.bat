@echo off
:Compile

cd ..
call vendor\premake\premake5.exe vs2022

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
echo "Starting Build for all Projects with proposed changes" 
echo . 
devenv "Wiwa.sln" /build Release
echo "All builds completed."
rmdir /s bin
rmdir /s bin-int
call vendor\premake\premake5.exe clean
pause

:Done