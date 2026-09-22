param (
    [string]$Bai = ""
)

Write-Host "====================================================================" -ForegroundColor Cyan
Write-Host "       CONG CU CHUYEN BAI MO PHONG WOKWI (ESP32 LAB 1)" -ForegroundColor Cyan
Write-Host "====================================================================" -ForegroundColor Cyan

if (-not $Bai) {
    Write-Host "Hay chon bai ban muon mo phong:" -ForegroundColor Yellow
    Write-Host "  [1]  Bai 1: Mo phong Den Giao Thong (GPIO 27, 26, 25)"
    Write-Host "  [2]  Bai 2: Button dieu khien LED truc tiep (GPIO 18, 2)"
    Write-Host "  [3]  Bai 3: Cong tac thong minh Toggle + Debounce millis (GPIO 18, 2)"
    Write-Host "  [4]  Bai 4: Canh bao nhiet do qua nguong DHT22 (GPIO 23, 2)"
    Write-Host "  [5]  Bai 5: Hien thi thoi gian Uptime tren LCD 16x2 I2C"
    Write-Host "  [h1] Homework 1: Mini Weather Station (DHT22 + LCD + Button + LED)"
    Write-Host "  [h2] Homework 2: Stopwatch MM:SS:ms (LCD + 2 Buttons A & B)"
    Write-Host "  [h3] Homework 3: Simple LCD Menu 3 trang (LCD + DHT22 + Button)"
    Write-Host ""
    $Bai = Read-Host "Nhap ma bai muon chay (1-5 hoac h1-h3)"
}

$baiMap = @{
    "1" = "bai01"; "bai01" = "bai01"; "bai1" = "bai01";
    "2" = "bai02"; "bai02" = "bai02"; "bai2" = "bai02";
    "3" = "bai03"; "bai03" = "bai03"; "bai3" = "bai03";
    "4" = "bai04"; "bai04" = "bai04"; "bai4" = "bai04";
    "5" = "bai05"; "bai05" = "bai05"; "bai5" = "bai05";
    "h1" = "homework01"; "hw1" = "homework01"; "homework01" = "homework01";
    "h2" = "homework02"; "hw2" = "homework02"; "homework02" = "homework02";
    "h3" = "homework03"; "hw3" = "homework03"; "homework03" = "homework03";
}

$lowerKey = $Bai.ToLower()
if (-not $baiMap.ContainsKey($lowerKey)) {
    Write-Host "[LOI] Lua chon khong hop le! Vui long nhap 1..5 hoac h1..h3." -ForegroundColor Red
    exit 1
}

$target = $baiMap[$lowerKey]
Write-Host "`n[*] Dang chuyen sang: $target..." -ForegroundColor Green

$labDir = $PSScriptRoot
$rootDir = (Get-Item $labDir).Parent.FullName

# 1. Cap nhat wokwi.toml
$wokwiContentLab = @"
[wokwi]
version = 1
firmware = ".pio/build/$target/firmware.bin"
elf = ".pio/build/$target/firmware.elf"
"@
Set-Content -Path "$labDir\wokwi.toml" -Value $wokwiContentLab

if (Test-Path "$rootDir\lab1_iot") {
    $wokwiContentRoot = @"
[wokwi]
version = 1
firmware = "lab1_iot/.pio/build/$target/firmware.bin"
elf = "lab1_iot/.pio/build/$target/firmware.elf"
"@
    Set-Content -Path "$rootDir\wokwi.toml" -Value $wokwiContentRoot
}

# 2. Sao chep diagram.json
$diagramSource = "$labDir\wokwi\$target\diagram.json"
if (Test-Path $diagramSource) {
    Copy-Item -Path $diagramSource -Destination "$labDir\diagram.json" -Force
    if (Test-Path "$rootDir\lab1_iot") {
        Copy-Item -Path $diagramSource -Destination "$rootDir\diagram.json" -Force
    }
    Write-Host "[*] Da cap nhat so do mach: diagram.json <- wokwi/$target/diagram.json" -ForegroundColor Green
}

# 3. Kiem tra firmware
$firmwarePath = "$labDir\.pio\build\$target\firmware.bin"
if (-not (Test-Path $firmwarePath)) {
    Write-Host "[*] Chua co firmware cho $target. Dang tu dong bien dich bang PlatformIO..." -ForegroundColor Yellow
    $pio = "$HOME\.platformio\penv\Scripts\pio.exe"
    & $pio run -d "$labDir" -e $target
} else {
    Write-Host "[*] Firmware da san sang tai: .pio/build/$target/firmware.bin" -ForegroundColor Green
}

Write-Host "`n====================================================================" -ForegroundColor Cyan
Write-Host "[THANH CONG] Da chuyen sang $target!" -ForegroundColor Green
Write-Host "`nCAC BUOC TIEP THEO TRONG VS CODE:" -ForegroundColor Yellow
Write-Host "  1. Mo file 'diagram.json' trong VS Code."
Write-Host "  2. Nhan nut PLAY (tam giac mau xanh la cay) de CHAY MO PHONG."
Write-Host "  3. Mo tab 'Terminal' hoac 'Output' (Wokwi Simulator) de xem Serial."
Write-Host "====================================================================`n" -ForegroundColor Cyan

