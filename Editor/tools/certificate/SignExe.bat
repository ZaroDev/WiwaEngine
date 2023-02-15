@echo off

set name=%1%
set exec=%2%
set password=%3%

set pfx_name=%name%.pfx

signtool sign /f %pfx_name% /p %password% /fd SHA256 %exec%