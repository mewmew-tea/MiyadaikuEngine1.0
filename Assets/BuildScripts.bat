echo %~dp0

rem generate CSFileList.txt

set cur=%cd%

cd /d %~dp0

if exist CSFileList.txt del CSFileList.txt

for /r %%f in (*.cs) do (
	echo %%~nf >> CSFileList.txt
)

cd /d %cur%

call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

rem Build by MSBuild
cd %~dp0
MSBuild %~dp0\CSBuild.sln /t:clean;rebuild /p:Configuration=Debug;Platform="x64"
if %ERRORLEVEL% neq 0 (
    echo ErrorLevel:%ERRORLEVEL%
    echo FailedBuild
)

