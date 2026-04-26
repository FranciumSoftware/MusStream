# MusStream WiX Installer

This folder contains the WiX Toolset project for creating an MSI installer for the MusStream application.

## Prerequisites

- WiX Toolset installed (download from https://wixtoolset.org/)
- The application must be built in Release mode (appMusStream.exe should exist in ..\build\Desktop_Qt_6_11_0_MinGW_64_bit-Release\)

## Files

- `Product.wxs`: WiX source file defining the installer
- `MusStreamInstaller.wixproj`: MSBuild project file
- `build.bat`: Batch script to build the MSI

## Building the Installer

### Option 1: Using the batch script
Run `build.bat` in this folder.

### Option 2: Using MSBuild
```
msbuild MusStreamInstaller.wixproj
```

### Option 3: Manual commands
```
candle.exe Product.wxs -out obj\Product.wixobj
light.exe obj\Product.wixobj -out MusStreamInstaller.msi
```

## Notes

- Update the GUIDs in Product.wxs with unique values
- Add more components for Qt libraries, data files, etc. as needed
- The installer installs to Program Files\MusStream by default