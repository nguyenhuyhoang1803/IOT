param (
    [string]$Bai = ""
)

Write-Host "====================================================================" -ForegroundColor Cyan
Write-Host "       CONG CU CHUYEN BAI MO PHONG WOKWI (ESP32 LAB 2)" -ForegroundColor Cyan
Write-Host "====================================================================" -ForegroundColor Cyan

if (-not $Bai) {
    Write-Host "Hay chon bai ban muon mo phong (1 - 8):" -ForegroundColor Yellow
    Write-Host "  [1] Bai 1: Hai LED nhap nhay doc lap (GPIO 2, GPIO 4)"
    Write-Host "  [2] Bai 2: Button Debounce dao trang thai 2 LED (GPIO 18, 4, 5)"
    Write-Host "  [3] Bai 3: Heartbeat LED (GPIO 2) + Serial Uptime"
    Write-Host "  [4] Bai 4: DHT11 (GPIO 23) + Button khong chan (GPIO 18)"
    Write-Host "  [5] Bai 5: 3 tac vu song song + Do Loop count/s"
    Write-Host "  [6] Bai 6: Den giao thong FSM (GPIO 25, 26, 27)"
    Write-Host "  [7] Bai 7: Canh bao nhiet do DHT11 + Buzzer (GPIO 15)"
    Write-Host "  [8] Bai 8: LEDC PWM 5 muc do sang + Heartbeat (GPIO 4, 2, 18)"
    Write-Host ""
    $Bai = Read-Host "Nhap so bai muon chay (1-8)"
}

$baiMap = @{
    "1" = "bai01"; "bai01" = "bai01";
    "2" = "bai02"; "bai02" = "bai02";
    "3" = "bai03"; "bai03" = "bai03";
    "4" = "bai04"; "bai04" = "bai04";
    "5" = "bai05"; "bai05" = "bai05";
    "6" = "bai06"; "bai06" = "bai06";
    "7" = "bai07"; "bai07" = "bai07";
    "8" = "bai08"; "bai08" = "bai08";
}

if (-not $baiMap.ContainsKey($Bai)) {
    Write-Host "[LOI] Lua chon khong hop le! Vui long nhap tu 1 den 8." -ForegroundColor Red
    exit 1
}

$target = $baiMap[$Bai]
Write-Host "`n[*] Dang chuyen sang: $target..." -ForegroundColor Green

# 1. Cap nhat wokwi.toml
$wokwiContentLab = @"
[wokwi]
version = 1
firmware = ".pio/build/$target/firmware.bin"
elf = ".pio/build/$target/firmware.elf"
"@
Set-Content -Path "d:\IOT\lab2_iot\wokwi.toml" -Value $wokwiContentLab

$wokwiContentRoot = @"
[wokwi]
version = 1
firmware = "lab2_iot/.pio/build/$target/firmware.bin"
elf = "lab2_iot/.pio/build/$target/firmware.elf"
"@
Set-Content -Path "d:\IOT\wokwi.toml" -Value $wokwiContentRoot

# 2. Sao chep diagram.json
$diagramSource = "d:\IOT\lab2_iot\wokwi\$target\diagram.json"
if (Test-Path $diagramSource) {
    Copy-Item -Path $diagramSource -Destination "d:\IOT\lab2_iot\diagram.json" -Force
    Copy-Item -Path $diagramSource -Destination "d:\IOT\diagram.json" -Force
    Write-Host "[*] Da cap nhat so do mach: diagram.json <- wokwi/$target/diagram.json" -ForegroundColor Green
}

# 3. Kiem tra firmware
$firmwarePath = "d:\IOT\lab2_iot\.pio\build\$target\firmware.bin"
if (-not (Test-Path $firmwarePath)) {
    Write-Host "[*] Chua co firmware cho $target. Dang tu dong bien dich bang PlatformIO..." -ForegroundColor Yellow
    $pio = "$HOME\.platformio\penv\Scripts\pio.exe"
    & $pio run -d "d:\IOT\lab2_iot" -e $target
} else {
    Write-Host "[*] Firmware da san sang tai: .pio/build/$target/firmware.bin" -ForegroundColor Green
}

Write-Host "`n====================================================================" -ForegroundColor Cyan
Write-Host "[THANH CONG] Da chuyen sang $target!" -ForegroundColor Green
Write-Host "`nCAC BUOC TIEP THEO TRONG VS CODE:" -ForegroundColor Yellow
Write-Host "  1. Mo file 'diagram.json' o thu muc goc lab2_iot trong VS Code."
Write-Host "  2. Nhan nut PLAY (tam giac mau xanh la cay) o goc tren de CHAY MO PHONG."
Write-Host "  3. Mo tab 'Wokwi' hoac Terminal de xem Serial Monitor (115200 baud)."
Write-Host "====================================================================`n" -ForegroundColor Cyan

