@echo off
setlocal enabledelayedexpansion

echo ====================================================================
echo        CONG CU CHUYEN BAI MO PHONG WOKWI - ESP32 LAB 1
echo ====================================================================

set "CHOICE=%~1"

if "%CHOICE%"=="" (
    echo Hay chon bai ban muon mo phong:
    echo   [1]  Bai 1 - Mo phong Den Giao Thong - GPIO 27, 26, 25
    echo   [2]  Bai 2 - Button dieu khien LED truc tiep - GPIO 18, 2
    echo   [3]  Bai 3 - Cong tac thong minh Toggle va Debounce millis - GPIO 18, 2
    echo   [4]  Bai 4 - Canh bao nhiet do qua nguong DHT22 - GPIO 23, 2
    echo   [5]  Bai 5 - Hien thi thoi gian Uptime tren LCD 16x2 I2C
    echo   [h1] Homework 1 - Mini Weather Station - DHT22, LCD, Button, LED
    echo   [h2] Homework 2 - Stopwatch MM:SS:ms - LCD, 2 Buttons A va B
    echo   [h3] Homework 3 - Simple LCD Menu 3 trang - LCD, DHT22, Button
    echo.
    set /p "CHOICE=Nhap ma bai muon chay [1-5 hoac h1-h3]: "
)

REM Chuan hoa lua chon
if "%CHOICE%"=="1" set "BAI=bai01"
if "%CHOICE%"=="2" set "BAI=bai02"
if "%CHOICE%"=="3" set "BAI=bai03"
if "%CHOICE%"=="4" set "BAI=bai04"
if "%CHOICE%"=="5" set "BAI=bai05"
if "%CHOICE%"=="bai01" set "BAI=bai01"
if "%CHOICE%"=="bai02" set "BAI=bai02"
if "%CHOICE%"=="bai03" set "BAI=bai03"
if "%CHOICE%"=="bai04" set "BAI=bai04"
if "%CHOICE%"=="bai05" set "BAI=bai05"
if "%CHOICE%"=="bai1" set "BAI=bai01"
if "%CHOICE%"=="bai2" set "BAI=bai02"
if "%CHOICE%"=="bai3" set "BAI=bai03"
if "%CHOICE%"=="bai4" set "BAI=bai04"
if "%CHOICE%"=="bai5" set "BAI=bai05"

if /i "%CHOICE%"=="h1" set "BAI=homework01"
if /i "%CHOICE%"=="h2" set "BAI=homework02"
if /i "%CHOICE%"=="h3" set "BAI=homework03"
if /i "%CHOICE%"=="hw1" set "BAI=homework01"
if /i "%CHOICE%"=="hw2" set "BAI=homework02"
if /i "%CHOICE%"=="hw3" set "BAI=homework03"
if /i "%CHOICE%"=="homework01" set "BAI=homework01"
if /i "%CHOICE%"=="homework02" set "BAI=homework02"
if /i "%CHOICE%"=="homework03" set "BAI=homework03"

if "%BAI%"=="" (
    echo [LOI] Lua chon khong hop le! Vui long nhap 1..5 hoac h1..h3.
    exit /b 1
)

echo.
echo [*] Dang chuyen sang: %BAI%...

REM Xac dinh duong dan hien tai
set "CURR_DIR=%~dp0"

REM 1. Cap nhat wokwi.toml tai thu muc hien tai
(
    echo [wokwi]
    echo version = 1
    echo firmware = ".pio/build/%BAI%/firmware.bin"
    echo elf = ".pio/build/%BAI%/firmware.elf"
) > "%CURR_DIR%wokwi.toml"

REM Neu co thu muc cha D:\IOT thi dong bo luon sang thu muc cha
if exist "%CURR_DIR%..\lab1_iot" (
    (
        echo [wokwi]
        echo version = 1
        echo firmware = "lab1_iot/.pio/build/%BAI%/firmware.bin"
        echo elf = "lab1_iot/.pio/build/%BAI%/firmware.elf"
    ) > "%CURR_DIR%..\wokwi.toml"
)

REM 2. Sao chep so do mach tuong ung
if exist "%CURR_DIR%wokwi\%BAI%\diagram.json" (
    copy /y "%CURR_DIR%wokwi\%BAI%\diagram.json" "%CURR_DIR%diagram.json" > nul
    if exist "%CURR_DIR%..\lab1_iot" (
        copy /y "%CURR_DIR%wokwi\%BAI%\diagram.json" "%CURR_DIR%..\diagram.json" > nul
    )
    echo [*] Da cap nhat so do mach: diagram.json ^<- wokwi\%BAI%\diagram.json
) else (
    echo [CANH BAO] Khong tim thay wokwi\%BAI%\diagram.json
)

REM 3. Kiem tra xem firmware da duoc bien dich chua
if not exist "%CURR_DIR%.pio\build\%BAI%\firmware.bin" (
    echo [*] Chua co firmware cho %BAI%. Dang tu dong bien dich bang PlatformIO...
    call "%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run -d "%CURR_DIR%" -e %BAI%
) else (
    echo [*] Firmware da san sang tai: .pio/build/%BAI%/firmware.bin
)

echo.
echo ====================================================================
echo [THANH CONG] Da chuyen sang %BAI%!
echo.
echo CAC BUOC TIEP THEO TRONG VS CODE:
echo   1. Mo file "diagram.json" trong VS Code.
echo   2. Nhan nut PLAY (tam giac mau xanh la cay) de CHAY MO PHONG.
echo   3. Mo tab "Terminal" hoac "Output" (Wokwi Simulator) de xem Serial.
echo ====================================================================
echo.
