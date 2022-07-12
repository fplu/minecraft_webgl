@echo off
@echo Compilation en cours, merci de patienter...
@echo.

SET CURRENT_DIR=%~dp0
SET CURRENT_DIR=%CURRENT_DIR:~0,-1%
cd..
cd ..
echo cd %CURRENT_DIR% >compile.bat
echo @echo on >>compile.bat
echo emcc index.c -s WASM=1 -O3 -o index.js -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=1 >>compile.bat
	
REM Si on veut effectuer une autre action ou changer les param de compil on peut 

cd C:\aaa\emscripten

emcmdprompt.bat
