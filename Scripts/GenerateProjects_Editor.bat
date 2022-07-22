echo set current directory
cd /d %~dp0

mkdir ..\build\Editor
cd /d ..\build\Editor

cmake -G "Visual Studio 17 2022" -A x64 -D EDITOR=ON ../../

pause