@echo off
REM Build script for MusStream WiX Installer

echo Building MusStream MSI Installer...

REM Check if WiX Toolset is installed
where candle >nul 2>nul
if %errorlevel% neq 0 (
    echo Error: WiX Toolset not found. Please install WiX Toolset from https://wixtoolset.org/
    pause
    exit /b 1
)

REM Create obj directory if it doesn't exist
if not exist obj mkdir obj

REM Compile the WiX source
candle.exe Product.wxs -out obj\Product.wixobj

REM Link to create MSI
light.exe obj\Product.wixobj -out MusStreamInstaller.msi

echo Build complete. MSI created as MusStreamInstaller.msi
pause