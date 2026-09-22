@echo off
setlocal enabledelayedexpansion

echo ====================================================================
echo        CONG CU CHUYEN BAI MO PHONG WOKWI - ESP32 LAB 2
echo ====================================================================

set "CHOICE=%~1"

if "%CHOICE%"=="" (
    echo Hay chon bai ban muon mo phong 1 - 8:
    echo   [1] Bai 1 - Hai LED nhap nhay doc lap - GPIO 2, GPIO 4
    echo   [2] Bai 2 - Button Debounce dao trang thai 2 LED - GPIO 18, 4, 5
    echo   [3] Bai 3 - Heartbeat LED GPIO 2 + Serial Uptime
    echo   [4] Bai 4 - DHT11 GPIO 23 + Button khong chan GPIO 18
    echo   [5] Bai 5 - 3 tac vu song song + Do Loop count per second
    echo   [6] Bai 6 - Den giao thong FSM - GPIO 25, 26, 27
    echo   [7] Bai 7 - Canh bao nhiet do DHT11 + Buzzer GPIO 15
    echo   [8] Bai 8 - LEDC PWM 5 muc do sang + Heartbeat GPIO 4, 2, 18
    echo.
    set /p "CHOICE=Nhap so bai muon chay (1-8): "
)

REM Chuan hoa chuoi chon
if "%CHOICE%"=="1" set "BAI=bai01"
if "%CHOICE%"=="2" set "BAI=bai02"
if "%CHOICE%"=="3" set "BAI=bai03"
if "%CHOICE%"=="4" set "BAI=bai04"
if "%CHOICE%"=="5" set "BAI=bai05"
if "%CHOICE%"=="6" set "BAI=bai06"
if "%CHOICE%"=="7" set "BAI=bai07"
if "%CHOICE%"=="8" set "BAI=bai08"
if "%CHOICE%"=="bai01" set "BAI=bai01"
if "%CHOICE%"=="bai02" set "BAI=bai02"
if "%CHOICE%"=="bai03" set "BAI=bai03"
if "%CHOICE%"=="bai04" set "BAI=bai04"
if "%CHOICE%"=="bai05" set "BAI=bai05"
if "%CHOICE%"=="bai06" set "BAI=bai06"
if "%CHOICE%"=="bai07" set "BAI=bai07"
if "%CHOICE%"=="bai08" set "BAI=bai08"

if "%BAI%"=="" (
    echo [LOI] Lua chon khong hop le! Vui long nhap tu 1 den 8.
    exit /b 1
)

echo.
echo [*] Dang chuyen sang: %BAI%...

REM Xac dinh thu muc chua lab2_iot
set "ROOT_DIR=%~dp0"
if exist "%ROOT_DIR%lab2_iot" (
    set "LAB_DIR=%ROOT_DIR%lab2_iot"
) else (
    set "LAB_DIR=%ROOT_DIR%"
)

REM 1. Cap nhat file wokwi.toml o ca 2 noi
(
    echo [wokwi]
    echo version = 1
    echo firmware = "lab2_iot/.pio/build/%BAI%/firmware.bin"
    echo elf = "lab2_iot/.pio/build/%BAI%/firmware.elf"
) > "%ROOT_DIR%wokwi.toml"

(
    echo [wokwi]
    echo version = 1
    echo firmware = ".pio/build/%BAI%/firmware.bin"
    echo elf = ".pio/build/%BAI%/firmware.elf"
) > "%LAB_DIR%\wokwi.toml"

REM 2. Sao chep so do mach diagram.json o ca 2 noi
if exist "%LAB_DIR%\wokwi\%BAI%\diagram.json" (
    copy /y "%LAB_DIR%\wokwi\%BAI%\diagram.json" "%ROOT_DIR%diagram.json" > nul
    copy /y "%LAB_DIR%\wokwi\%BAI%\diagram.json" "%LAB_DIR%\diagram.json" > nul
    echo [*] Da cap nhat so do mach: diagram.json ^<- wokwi\%BAI%\diagram.json
) else (
    echo [CANH BAO] Khong tim thay wokwi\%BAI%\diagram.json
)

REM 3. Kiem tra xem firmware da duoc bien dich chua
if not exist "%LAB_DIR%\.pio\build\%BAI%\firmware.bin" (
    echo [*] Chua co firmware cho %BAI%. Dang tu dong bien dich bang PlatformIO...
    call "%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run -d "%LAB_DIR%" -e %BAI%
) else (
    echo [*] Firmware da san sang tai: %LAB_DIR%\.pio\build\%BAI%\firmware.bin
)

echo.
echo ====================================================================
echo [THANH CONG] Da chuyen sang %BAI%!
echo.
echo CAC BUOC TIEP THEO TRONG VS CODE:
echo   1. Mo file "diagram.json" trong VS Code (o thu muc D:\IOT).
echo   2. Nhan nut PLAY (tam giac mau xanh la cay) de CHAY MO PHONG.
echo   3. Mo tab "Terminal" hoac "Output" (chong Wokwi Simulator) de xem Serial Monitor.
echo ====================================================================
echo.

