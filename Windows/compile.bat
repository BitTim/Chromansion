@echo off
echo Creating icon
windres icon.rc -O coff -o icon.res
echo Icon created!
echo .
echo Compiling main.cpp
g++ main.cpp icon.res -IC:\Dev\SDL2-mingw32-32bit\include -LC:\Dev\SDL2-mingw32-32bit\lib -w -Wl,-subsystem,windows -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -o main.exe
echo Compiled to main.exe!
pause
