echo set current directory
cd /d %~dp0

mkdir ..\build

cd /d ..\build

cmake  -G "Visual Studio 17 2022" -A x64 -D EDITOR=OFF ../
pause