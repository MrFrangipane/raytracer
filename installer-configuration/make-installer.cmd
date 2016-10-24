@echo off

REM root
set ROOT=D:\dev\raytracer

REM Copy Files to pre-zip resolution folder
rd /S /Q %ROOT%\raytracer\installer-configuration\pre-zip
mkdir %ROOT%\raytracer\installer-configuration\pre-zip
mkdir %ROOT%\raytracer\installer-configuration\pre-zip\maxscript
copy %ROOT%\builds\Desktop_Qt_5_7_0_MSVC2015_64bit-Release\release\frangiray.exe %ROOT%\raytracer\installer-configuration\pre-zip\
copy %ROOT%\raytracer\maxscript\*.* %ROOT%\raytracer\installer-configuration\pre-zip\maxscript\
copy %ROOT%\raytracer\scene.json %ROOT%\raytracer\installer-configuration\pre-zip

REM Resolve Dependencies
windeployqt %ROOT%\raytracer\installer-configuration\pre-zip\frangiray.exe

REM Zip Datas
call "C:\Program Files\7-Zip\7z.exe" a "%ROOT%\raytracer\installer-configuration\packages\com.mrfrangipane.raytracer\data\data.7z" "%ROOT%\raytracer\installer-configuration\pre-zip\*"

REM Create Installer
C:\Qt\Tools\QtInstallerFramework\2.0\bin\binarycreator.exe -c %ROOT%\raytracer\installer-configuration\config\config.xml -p %ROOT%\raytracer\installer-configuration\packages "%ROOT%\frangiray-setup.exe"

REM Delete zip and pre zip
del /F "%ROOT%\raytracer\installer-configuration\packages\com.mrfrangipane.raytracer\data\data.7z"
rd /S /Q %ROOT%\raytracer\installer-configuration\pre-zip

pause