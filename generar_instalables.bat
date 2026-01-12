@echo off
REM Script para generar instalable Windows desde MSYS2
REM Ejecutar desde MSYS2 MinGW 64-bit terminal

echo ==========================================
echo   GENERACION DE INSTALABLE WINDOWS
echo ==========================================
echo.

cd build_windows

REM Verificar si el ejecutable existe
if not exist "bin\chaos-crew.exe" (
    echo Compilando proyecto...
    cmake --build . -j
    if errorlevel 1 (
        echo Error al compilar
        exit /b 1
    )
)

REM Generar ZIP
echo Creando paquete ZIP...
cpack -G ZIP

if errorlevel 1 (
    echo Error al generar instalable
    exit /b 1
) else (
    echo.
    echo ==========================================
    echo Instalable generado en: build_windows\packages\
    echo ==========================================
    dir packages\*.zip
)

cd ..
pause
