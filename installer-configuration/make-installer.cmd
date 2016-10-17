@echo off

REM root
set ROOT=D:\dev\raytracer

REM Resolve Dependencies
windeployqt %ROOT%\builds\Desktop_Qt_5_7_0_MSVC2015_64bit-Release\release\ray-tracer.exe

REM Zip Datas
"C:\Program Files\7-Zip\7z.exe" a "%ROOT%\raytracer\installer-configuration\packages\com.mrfrangipane.raytracer\data\data.7z" "%ROOT%\builds\Desktop_Qt_5_7_0_MSVC2015_64bit-Release\release\*"

REM Create Installer
C:\Qt\Tools\QtInstallerFramework\2.0\bin\binarycreator.exe -c %ROOT%\raytracer\installer-configuration\config\config.xml -p %ROOT%\raytracer\installer-configuration\packages "%ROOT%\frangiray-setup.exe"

pause