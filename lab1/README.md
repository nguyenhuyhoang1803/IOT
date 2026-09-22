# LAB 1: LẬP TRÌNH CƠ BẢN ESP32 VỚI ARDUINO FRAMEWORK & MÔ PHỎNG WOKWI

Chào mừng bạn đến với **Lab 1 môn Internet of Things (IoT)** sử dụng vi điều khiển **ESP32** trên nền tảng **PlatformIO + VS Code** kết hợp mô phỏng trực quan **Wokwi Simulator**.

Dự án được thiết kế chạy **100% mô phỏng ảo**, không yêu cầu phần cứng thật, không cần cáp nạp hay cổng COM.

---

## MỤC LỤC

1. [Cấu trúc thư mục & Môi trường](#1-cấu-trúc-thư-mục--môi-trường)
2. [Hướng dẫn cài đặt & Cách chạy mô phỏng](#2-hướng-dẫn-cài-đặt--cách-chạy-mô-phỏng)
3. [Bảng tổng hợp GPIO](#3-bảng-tổng-hợp-gpio)
4. [Chi tiết các bài thực hành (Bài 1 – Bài 5)](#4-chi-tiết-các-bài-thực-hành-bài-1--bài-5)
   - [Bài 1: Mô phỏng Đèn Giao Thông](#bài-1-mô-phỏng-đèn-giao-thông)
   - [Bài 2: Button điều khiển LED trực tiếp](#bài-2-button-điều-khiển-led-trực-tiếp)
   - [Bài 3: Công tắc thông minh (Toggle + Debounce)](#bài-3-công-tắc-thông-minh-toggle--debounce)
   - [Bài 4: Cảnh báo nhiệt độ quá ngưỡng (DHT22)](#bài-4-cảnh-báo-nhiệt-độ-quá-ngưỡng-dht22)
   - [Bài 5: Hiển thị Uptime trên LCD 16x2 I2C](#bài-5-hiển-thị-uptime-trên-lcd-16x2-i2c)
5. [Chi tiết các bài tập về nhà (Homework 1 – Homework 3)](#5-chi-tiết-các-bài-tập-về-nhà-homework-1--homework-3)
   - [Homework 1: Trạm thời tiết mini (Smart Thermostat)](#homework-1-trạm-thời-tiết-mini-smart-thermostat)
   - [Homework 2: Đồng hồ bấm giờ thể thao (Stopwatch MM:SS:ms)](#homework-2-đồng-hồ-bấm-giờ-thể-thao-stopwatch-mmssms)
   - [Homework 3: Menu 3 trang chuyển đổi trên LCD 16x2](#homework-3-menu-3-trang-chuyển-đổi-trên-lcd-16x2)

---

## 1. CẤU TRÚC THƯ MỤC & MÔI TRƯỜNG

```text
D:\IOT\lab1_iot/
├── platformio.ini         # Cấu hình 8 environment độc lập cho PlatformIO
├── README.md              # Hướng dẫn toàn diện cho sinh viên
├── diagram.json           # Sơ đồ mạch Wokwi đang kích hoạt
├── wokwi.toml             # Chỉ định file firmware nạp vào Wokwi
├── switch_bai.bat         # Script chuyển đổi nhanh bài thực hành (.bat)
├── switch_bai.ps1         # Script chuyển đổi nhanh bài thực hành (.ps1)
│
├── bai01/main.cpp         # Bài 1: Đèn giao thông
├── bai02/main.cpp         # Bài 2: Button điều khiển LED trực tiếp
├── bai03/main.cpp         # Bài 3: Công tắc thông minh Toggle + Debounce
├── bai04/main.cpp         # Bài 4: Cảnh báo nhiệt độ DHT22
├── bai05/main.cpp         # Bài 5: Uptime trên LCD 16x2 I2C
│
├── homework01/main.cpp    # HW 1: Weather Station (5s đổi độ F, LED độ ẩm)
├── homework02/main.cpp    # HW 2: Stopwatch MM:SS:ms (Button A & B)
├── homework03/main.cpp    # HW 3: Menu LCD 3 trang
│
└── wokwi/                 # Chứa sơ đồ diagram.json chuẩn cho từng bài
```

---

## 2. HƯỚNG DẪN CÀI ĐẶT & CÁCH CHẠY MÔ PHỎNG

### Yêu cầu môi trường:
1. **VS Code** đã cài Extension **PlatformIO IDE** và **Wokwi Simulator**.
2. Đã kích hoạt License Wokwi miễn phí (nếu VS Code yêu cầu, bấm F1 -> `Wokwi: Request a free trial license`).

### Các bước chạy một bài bất kỳ:
1. **Bước 1: Mở Terminal tại thư mục `lab1_iot` và chạy lệnh chuyển bài:**
   * Để chạy **Bài 1**: `.\switch_bai.bat 1`
   * Để chạy **Bài 2**: `.\switch_bai.bat 2`
   * Để chạy **Bài 3**: `.\switch_bai.bat 3`
   * Để chạy **Bài 4**: `.\switch_bai.bat 4`
   * Để chạy **Bài 5**: `.\switch_bai.bat 5`
   * Để chạy **Homework 1**: `.\switch_bai.bat h1`
   * Để chạy **Homework 2**: `.\switch_bai.bat h2`
   * Để chạy **Homework 3**: `.\switch_bai.bat h3`
   *(Script sẽ tự động sao chép đúng sơ đồ linh kiện và tự động biên dịch mã nguồn nếu chưa có firmware).*

2. **Bước 2: Mở file sơ đồ mạch:**
   * Trong thanh Explorer của VS Code, click mở file **`diagram.json`** ở thư mục gốc `lab1_iot`.

3. **Bước 3: Bắt đầu mô phỏng:**
   * Nhấn nút **Play ▶ (tam giác màu xanh lá cây)** trên giao diện Wokwi.
   * Mở tab **Terminal** hoặc tab **Output** (chọn kênh *Wokwi Simulator*) để theo dõi dữ liệu in ra Serial Monitor (Baud rate: **115200**).

4. **Khi muốn đổi sang bài khác:**
   * Bấm nút **Stop ⏹** trên Wokwi.
   * Chạy lại `.\switch_bai.bat [mã bài]`.
   * Đóng tab `diagram.json` cũ rồi mở lại để Wokwi cập nhật linh kiện mới.
   * Bấm **Play ▶** tiếp tục chạy.

---

## 3. BẢNG TỔNG HỢP GPIO

| Linh kiện | Chân nối trên ESP32 | Ghi chú cấu hình |
| :--- | :--- | :--- |
| **Green LED** | `GPIO 27` | OUTPUT, qua điện trở 220Ω |
| **Yellow LED** | `GPIO 26` | OUTPUT, qua điện trở 220Ω |
| **Red LED** | `GPIO 25` | OUTPUT, qua điện trở 220Ω |
| **Standard LED** | `GPIO 2` | OUTPUT, qua điện trở 220Ω (hoặc LED onboard) |
| **Push Button (Chính / Button A)** | `GPIO 18` | `INPUT_PULLUP` (Nhấn = LOW, Thả = HIGH) |
| **Push Button (Phụ / Button B)** | `GPIO 19` | `INPUT_PULLUP` (Nhấn = LOW, Thả = HIGH) |
| **DHT22 Data** | `GPIO 23` | Nối nguồn 3.3V và GND |
| **LCD 16x2 I2C SDA** | `GPIO 21` | Nối nguồn 5V/3.3V và GND |
| **LCD 16x2 I2C SCL** | `GPIO 22` | Địa chỉ I2C mặc định `0x27` |

---

## 4. CHI TIẾT CÁC BÀI THỰC HÀNH (BÀI 1 – BÀI 5)

---

### Bài 1: Mô phỏng Đèn Giao Thông
* **Mục tiêu:** Nắm vững cấu hình nhiều chân GPIO OUTPUT và sử dụng hàm `delay()` để định thời gian.
* **Linh kiện:** ESP32 DevKit, 3 LED (Xanh lá, Vàng, Đỏ), 3 điện trở 220Ω.
* **Sơ đồ đấu dây:**
  * Green LED: Anode (+) -> Trở 220Ω -> `GPIO 27`, Cathode (-) -> `GND`.
  * Yellow LED: Anode (+) -> Trở 220Ω -> `GPIO 26`, Cathode (-) -> `GND`.
  * Red LED: Anode (+) -> Trở 220Ω -> `GPIO 25`, Cathode (-) -> `GND`.
* **Nguyên lý:**
  1. Đèn Xanh bật sáng 3 giây (`delay(3000)`), Đèn Vàng và Đỏ tắt.
  2. Đèn Vàng bật sáng 1 giây (`delay(1000)`), Đèn Xanh và Đỏ tắt.
  3. Đèn Đỏ bật sáng 3 giây (`delay(3000)`), Đèn Xanh và Vàng tắt.
  4. Lặp lại chu trình.
* **Thao tác test:**
  * Bấm Play ▶ trên Wokwi.
  * Quan sát 3 đèn LED chuyển trạng thái nhịp nhàng.
  * Serial Monitor in tuần tự:
    ```text
    GREEN - 3 seconds
    YELLOW - 1 second
    RED - 3 seconds
    ```

---

### Bài 2: Button điều khiển LED trực tiếp
* **Mục tiêu:** Sử dụng Digital Input với chế độ `INPUT_PULLUP` và điều kiện `if...else`.
* **Linh kiện:** ESP32 DevKit, 1 Nút bấm (Push Button), 1 LED Xanh, 1 điện trở 220Ω.
* **Sơ đồ đấu dây:**
  * Button: Chân 1 -> `GPIO 18`, Chân 2 -> `GND`.
  * LED: Anode (+) -> Trở 220Ω -> `GPIO 2`, Cathode (-) -> `GND`.
* **Nguyên lý:**
  * Sử dụng điện trở nội kéo lên `INPUT_PULLUP`:
    * Khi **nhấn giữ nút**: Mạch khép kín xuống GND -> `digitalRead()` trả về `LOW (0)` -> Bật LED (`HIGH`).
    * Khi **thả nút**: Chân được kéo lên 3.3V -> `digitalRead()` trả về `HIGH (1)` -> Tắt LED (`LOW`).
* **Thao tác test:**
  * Bấm và **nhấn giữ chuột** vào nút bấm trên Wokwi -> LED sáng, Serial in: `Button PRESSED -> LED ON`.
  * **Thả chuột ra** -> LED tắt ngay, Serial in: `Button RELEASED -> LED OFF`.

---

### Bài 3: Công tắc thông minh (Toggle + Debounce)
* **Mục tiêu:** Biến nút bấm nhấn nhả thành công tắc bật/tắt (Toggle Switch) và khử rung phím (Debounce) bằng `millis()`, **không dùng `delay()`**.
* **Linh kiện:** ESP32 DevKit, 1 Nút bấm, 1 LED Xanh, 1 điện trở 220Ω.
* **Sơ đồ đấu dây:**
  * Button: Chân 1 -> `GPIO 18`, Chân 2 -> `GND`.
  * LED: Anode (+) -> Trở 220Ω -> `GPIO 2`, Cathode (-) -> `GND`.
* **Nguyên lý:**
  * Sử dụng thuật toán debounce đo chênh lệch thời gian `millis() - lastDebounceTime >= 50ms`.
  * Nhận diện sườn xuống (Falling Edge) khi nút chuyển từ HIGH sang LOW để đảo trạng thái `ledState = !ledState`.
  * Dù người dùng giữ nút lâu, LED chỉ đổi trạng thái đúng 1 lần cho mỗi lần nhấn.
* **Thao tác test:**
  * Nhấn nút lần 1 -> LED BẬT, Serial in: `Button pressed -> LED ON`.
  * Nhấn nút lần 2 -> LED TẮT, Serial in: `Button pressed -> LED OFF`.
  * Nhấn và giữ lì nút -> LED giữ nguyên trạng thái, không bị chớp giật liên tục.

---

### Bài 4: Cảnh báo nhiệt độ quá ngưỡng (DHT22)
* **Mục tiêu:** Giao tiếp cảm biến kỹ thuật số DHT22 và điều khiển LED cảnh báo nhấp nháy non-blocking.
* **Linh kiện:** ESP32 DevKit, Cảm biến nhiệt ẩm DHT22, 1 LED Đỏ, 1 điện trở 220Ω.
* **Sơ đồ đấu dây:**
  * DHT22: `VCC` -> `3.3V`, `GND` -> `GND`, `SDA/DATA` -> `GPIO 23`.
  * LED: Anode (+) -> Trở 220Ω -> `GPIO 2`, Cathode (-) -> `GND`.
* **Nguyên lý:**
  * Cứ mỗi 2000 ms, ESP32 đọc nhiệt độ và độ ẩm từ DHT22.
  * Nếu nhiệt độ **> 30.0°C**: Kích hoạt cờ cảnh báo `isAlarmActive = true` -> LED nhấp nháy liên tục mỗi 250 ms bằng `millis()`.
  * Nếu nhiệt độ **<= 30.0°C**: `isAlarmActive = false` -> LED TẮT hoàn toàn.
  * Nếu cảm biến lỗi (`isnan`): Báo lỗi lên Serial và tắt LED để đảm bảo an toàn.
* **Thao tác test trên Wokwi:**
  * **Test 1:** Ban đầu nhiệt độ mặc định 28°C -> Serial in `NORMAL`, LED tắt.
  * **Test 2:** Click vào cảm biến DHT22, kéo thanh trượt nhiệt độ lên **35°C** -> Sau tối đa 2 giây, Serial in `WARNING`, LED đỏ nhấp nháy liên tục (250ms ON / 250ms OFF).
  * **Test 3:** Kéo thanh trượt nhiệt độ về **29°C** -> Sau tối đa 2 giây, Serial in `NORMAL`, LED đỏ tắt hoàn toàn.

---

### Bài 5: Hiển thị Uptime trên LCD 16x2 I2C
* **Mục tiêu:** Giao tiếp màn hình LCD qua chuẩn I2C, hiển thị thời gian hoạt động của hệ thống, chống chớp màn hình (anti-flicker) và xử lý chuỗi ký tự dư thừa.
* **Linh kiện:** ESP32 DevKit, Module màn hình LCD 16x2 kèm mạch I2C (PCF8574).
* **Sơ đồ đấu dây:**
  * LCD `GND` -> `GND`.
  * LCD `VCC` -> `5V` (hoặc `3.3V`).
  * LCD `SDA` -> `GPIO 21`.
  * LCD `SCL` -> `GPIO 22`.
* **Nguyên lý:**
  * Khởi tạo thư viện `LiquidCrystal_I2C lcd(0x27, 16, 2)`.
  * Dòng 1 ghi cố định trong `setup()`: `Thoi gian chay:`.
  * Dòng 2 ghi: `[số giây] giay        `.
  * Chỉ gọi hàm in ra LCD khi `currentSeconds != lastSeconds` (mỗi 1 giây một lần). Không dùng `delay(1000)`.
  * Khoảng trắng đệm giúp xóa sạch các chữ số dư khi số giây đổi từ 3 chữ số về 1 chữ số.
* **Thao tác test:**
  * Bấm Play ▶ trên Wokwi.
  * Màn hình LCD hiển thị:
    ```text
    Thoi gian chay:
    5 giay
    ```
  * Số giây nhảy đều đặn từng giây, chữ hiển thị rõ nét, hoàn toàn không bị chớp giật.

---

## 5. CHI TIẾT CÁC BÀI TẬP VỀ NHÀ (HOMEWORK 1 – HOMEWORK 3)

---

### Homework 1: Trạm thời tiết mini (Smart Thermostat)
* **Mục tiêu:** Tích hợp đa thiết bị: DHT22, LCD 16x2 I2C, Button chuyển đổi độ F trong 5 giây non-blocking và LED cảnh báo độ ẩm cao.
* **Linh kiện:** ESP32, DHT22 (`GPIO 23`), LCD 16x2 I2C (`GPIO 21, 22`), Button (`GPIO 18`), LED Đỏ (`GPIO 2`).
* **Quy tắc hoạt động:**
  1. Trạng thái bình thường:
     * Dòng 1 LCD: `Temp: xx.x C`
     * Dòng 2 LCD: `Hum:  xx.x %`
  2. Khi nhấn nút bấm (GPIO 18):
     * Màn hình chuyển sang hiển thị nhiệt độ Fahrenheit (°F): `F = (C * 1.8) + 32`.
     * Hiển thị độ F trong **đúng 5 giây** (`5000 ms`), sau đó tự động trở lại độ C.
     * **Tuyệt đối không dùng `delay(5000)`**, dùng `millis()` để hệ thống vẫn đọc cảm biến ngầm.
  3. Cảnh báo độ ẩm:
     * Nếu **Độ ẩm > 80.0%**: LED đỏ BẬT sáng.
     * Nếu **Độ ẩm <= 80.0%**: LED đỏ TẮT.
* **Thao tác test trên Wokwi:**
  * Click vào DHT22, kéo Độ ẩm lên **90%** -> LED đỏ lập tức bật sáng. Kéo về **70%** -> LED đỏ tắt.
  * Click vào nút bấm màu xanh dương -> Màn hình đổi sang đơn vị `F` (ví dụ: `Temp: 78.8 F`). Đợi đủ 5 giây, màn hình tự động chuyển về `Temp: 26.0 C`.

---

### Homework 2: Đồng hồ bấm giờ thể thao (Stopwatch MM:SS:ms)
* **Mục tiêu:** Thiết kế đồng hồ bấm giờ chính xác cao với máy trạng thái (`STOPPED`, `RUNNING`) và 2 nút bấm độc lập.
* **Linh kiện:** ESP32, LCD 16x2 I2C (`GPIO 21, 22`), Button A (`GPIO 18`), Button B (`GPIO 19`).
* **Quy cách hiển thị:**
  * Dòng 1: `STOPWATCH: [STATUS]` (STOP, RUN, PAUSE, RESET).
  * Dòng 2: `   MM:SS:ms   ` (ví dụ: `00:00:000`, `00:05:231`).
* **Quy tắc nút bấm:**
  * **Button A (Start / Pause):**
    * Khi đang `STOPPED` -> Bắt đầu chạy (`START`), thời gian tăng dần từ mốc đã tích lũy.
    * Khi đang `RUNNING` -> Tạm dừng (`PAUSE`), bảo lưu thời gian chính xác.
    * Nhấn lại -> Chạy tiếp tục (`RESUME`), không bị gián đoạn hay reset về 0.
  * **Button B (Reset):**
    * **CHỈ CHO PHÉP RESET KHI ĐANG STOPPED**: Đưa đồng hồ về `00:00:000`.
    * **KHI ĐANG CHẠY (RUNNING)**: Nút B bị vô hiệu hóa hoàn toàn, bấm không có tác dụng.
* **Thao tác test trên Wokwi:**
  * Bấm Button A (màu xanh lá) -> Đồng hồ chạy đếm mili-giây mượt mà.
  * Bấm Button B (màu đỏ) khi đồng hồ đang chạy -> Đồng hồ **vẫn tiếp tục chạy**, không bị reset.
  * Bấm Button A để dừng (Pause) -> Đồng hồ đứng yên tại giá trị hiện tại.
  * Bấm Button B -> Đồng hồ được reset về `00:00:000`.
  * Bấm Button A chạy tiếp từ 0.

---

### Homework 3: Menu 3 trang chuyển đổi trên LCD 16x2
* **Mục tiêu:** Xây dựng hệ thống menu đa trang điều hướng tuần tự bằng 1 nút bấm với cấu trúc `switch-case`.
* **Linh kiện:** ESP32, LCD 16x2 I2C (`GPIO 21, 22`), Cảm biến DHT22 (`GPIO 23`), Button (`GPIO 18`).
* **Nội dung 3 trang (Page):**
  * **Trang 1:**
    ```text
    Nhiet do:
    27.0 C
    ```
  * **Trang 2:**
    ```text
    Do am:
    68.0 %
    ```
  * **Trang 3:**
    ```text
    Trang thai:
    OK
    ```
* **Quy tắc chuyển trang:**
  * Nút bấm có debounce 50 ms.
  * Logic chuyển trang bắt buộc:
    ```cpp
    page++;
    if (page > 3) {
        page = 1;
    }
    ```
  * Mỗi lần nhấn chuyển đúng 1 trang: Trang 1 -> Trang 2 -> Trang 3 -> Trang 1.
  * Cảm biến DHT22 được đọc định kỳ mỗi 2 giây và tự động cập nhật số liệu mới nếu người dùng đang ở Trang 1 hoặc Trang 2.
* **Thao tác test trên Wokwi:**
  * Bấm nút màu tím (GPIO 18) lần 1 -> Chuyển sang Trang 2 (Độ ẩm).
  * Bấm lần 2 -> Chuyển sang Trang 3 (Trạng thái: OK).
  * Bấm lần 3 -> Quay trở lại Trang 1 (Nhiệt độ).
  * Kéo nhiệt độ/độ ẩm trên DHT22 -> Số liệu trên màn hình tự động cập nhật chính xác.

---

## 6. LỜI KẾT & LƯU Ý KỸ THUẬT

* **Bảo vệ phần cứng thực tế:** Khi lắp ráp trên mạch thật, luôn nhớ mắc nối tiếp điện trở hạn dòng 220Ω - 330Ω cho các chân LED để tránh làm cháy cổng GPIO của ESP32.
* **Chống dội phím (Debounce):** Tất cả nút bấm trong các bài từ Bài 3 trở đi đều được xử lý chống dội bằng `millis()`. Đây là tiêu chuẩn vàng trong lập trình nhúng, giúp hệ thống không bị đóng băng giao diện.
* **Mô phỏng Wokwi:** Nếu mở giao diện Wokwi mà linh kiện bị lệch màn hình, bạn có thể dùng con lăn chuột để zoom nhỏ lại và kéo rê mạch vào giữa khung nhìn.

