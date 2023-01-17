@echo off
call tools\premake\premake5.exe %1
call tools\buildsln.bat %2 %3