@echo off
:: On sauvegarde le dossier actuel (ton projet)
pushd "%~dp0"

:: Configuration de Visual Studio (les guillemets sont cruciaux ici)
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: Configuration de Qt
call "C:\Qt\6.11.0\msvc2022_64\bin\qtenv2.bat"

:: On revient de force dans le dossier du projet au cas où qtenv2 l'aurait changé
popd
pushd "%~dp0"

echo --- CONFIGURATION ---
:: On utilise des points pour le dossier courant, CMake les comprendra 
:: car on a fait un popd/pushd juste avant.
cmake -S . -B build -G Ninja

echo --- COMPILATION ---
cmake --build build --parallel

:: On libère le dossier
popd