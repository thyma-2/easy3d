@setlocal enableextensions
@cd /d "%~dp0"
set a=%cd%
cd "C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin"
gcc.exe -g %a%\main.c %a%\easy3d\engine.c %a%\easy3d\object.c %a%\land.c -lgdi32 -lm -o %a%\output.exe
cd %a%
output.exe
pause