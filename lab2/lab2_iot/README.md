# LAB 2: LẬP TRÌNH NON-BLOCKING VỚI ESP32 (PLATFORMIO & ARDUINO FRAMEWORK)

Chào mừng bạn đến với tài liệu hướng dẫn thực hành **Lab 2 môn Internet of Things (IoT)**.

Mục tiêu trọng tâm của bài thực hành này là nắm vững tư duy **Lập trình không chặn (Non-blocking Programming)**, lập trình hướng sự kiện (**Event-driven**) và điều khiển theo mốc thời gian (**Time-triggered**) bằng cách sử dụng hàm `millis()` và Máy trạng thái hữu hạn (**Finite State Machine - FSM**).

> [!IMPORTANT]
> **NGUYÊN TẮC BẤT DI BẤT DỊCH CỦA LAB 2:**
> **TUYỆT ĐỐI KHÔNG SỬ DỤNG `delay()`** trong bất kỳ bài thực hành nào! Mọi hoạt động định thời phải sử dụng hàm `millis()` với phép tính an toàn tràn số:
> ```cpp
> if (currentMillis - previousMillis >= interval) {
>     previousMillis = currentMillis;
>     // Thực thi tác vụ...
> }
> ```

---

## 1. TỔNG HỢP PHẦN CỨNG & SƠ ĐỒ CHÂN (PINOUT)

| Bài | Tên bài | GPIO sử dụng | Linh kiện cần dùng | Cơ chế điều khiển |
| :---: | :--- | :--- | :--- | :--- |
| **Bài 1** | Hai LED nhấp nháy độc lập | - LED 1: `GPIO 2`<br>- LED 2: `GPIO 4` | 2 LED rời, 2 điện trở (220Ω - 330Ω) | 2 Timer `millis()` độc lập (500 ms & 1200 ms) |
| **Bài 2** | Button debounce bằng `millis()` | - Button: `GPIO 18`<br>- LED 1: `GPIO 4`<br>- LED 2: `GPIO 5` | 1 Push button, 2 LED, 2 điện trở 220Ω | Software Debounce (50 ms) bằng `millis()`, `INPUT_PULLUP` |
| **Bài 3** | Heartbeat LED + Serial Uptime | - Heartbeat LED: `GPIO 2` | 1 LED onboard (hoặc LED rời) | 2 Timer `millis()` độc lập (LED 1s, Serial 3s) |
| **Bài 4** | DHT11 + Button không chặn | - DHT11 Data: `GPIO 23`<br>- Button: `GPIO 18`<br>- LED: `GPIO 2` | 1 Cảm biến DHT11, 1 Button, 1 LED | Đọc DHT11 mỗi 2s, xử lý nút nhấn phản hồi tức thì |
| **Bài 5** | Ba tác vụ song song | - LED 1: `GPIO 2`<br>- LED 2: `GPIO 4`<br>- Button: `GPIO 18` | 2 LED, 1 Button, điện trở | LED1 nhấp nháy 300 ms, Button toggle LED2, đo số vòng loop()/giây |
| **Bài 6** | Đèn giao thông FSM | - LED Đỏ: `GPIO 25`<br>- LED Vàng: `GPIO 26`<br>- LED Xanh: `GPIO 27` | 3 LED (Đỏ, Vàng, Xanh), 3 điện trở 220Ω | Máy trạng thái (FSM): Đỏ (5s) -> Xanh (4s) -> Vàng (2s) |
| **Bài 7** | Cảnh báo nhiệt độ DHT11 + Buzzer | - DHT11 Data: `GPIO 23`<br>- Buzzer: `GPIO 15`<br>- LED: `GPIO 2` | 1 Cảm biến DHT11, 1 Còi Buzzer Active, 1 LED | Đọc DHT (2s), nếu Temp > 30°C còi bíp ON/OFF (200 ms) không chặn |
| **Bài 8** | PWM LED + Button + Heartbeat | - PWM LED: `GPIO 4`<br>- Heartbeat LED: `GPIO 2`<br>- Button: `GPIO 18` | 2 LED, 1 Button, điện trở | ESP32 LEDC PWM (5 mức: 0%, 25%, 50%, 75%, 100%) + Heartbeat 1.5s |

---

## 2. CẤU TRÚC PROJECT

```text
lab2_iot/
├── platformio.ini           # Cấu hình môi trường PlatformIO (env:bai01 -> env:bai08)
├── README.md                # Tài liệu hướng dẫn đầy đủ này
├── bai01/
│   └── main.cpp             # Hai LED nhấp nháy độc lập
├── bai02/
│   └── main.cpp             # Chống dội nút bấm điều khiển 2 LED
├── bai03/
│   └── main.cpp             # Heartbeat LED + In Uptime định kỳ
├── bai04/
│   └── main.cpp             # Đọc DHT11 không chặn kết hợp nút nhấn phản hồi tức thời
├── bai05/
│   └── main.cpp             # Ba tác vụ chạy đồng thời + Đo loop counter
├── bai06/
│   └── main.cpp             # Máy trạng thái Đèn giao thông Đỏ -> Xanh -> Vàng
├── bai07/
│   └── main.cpp             # Giám sát nhiệt độ DHT11 + Còi báo động ngắt quãng
└── bai08/
    └── main.cpp             # Điều khiển độ sáng LED 5 cấp độ bằng phần cứng LEDC PWM
```

---

## 3. HƯỚNG DẪN MÔ PHỎNG WOKWI TRONG VS CODE (KHÔNG CẦN ESP32 THẬT)

Toàn bộ 8 bài của Lab 2 đã được tích hợp đầy đủ sơ đồ mạch mô phỏng Wokwi. Bạn có thể học tập và kiểm tra kết quả ngay trong VS Code mà **không cần bất kỳ phần cứng ESP32 thật nào**, không cần cắm dây và không cần cổng COM.

### 3.1. Cài đặt Extension Wokwi trong VS Code
1. Mở VS Code -> Nhấn tổ hợp phím `Ctrl + Shift + X` để mở bảng **Extensions**.
2. Tìm kiếm từ khóa: `Wokwi Simulator`.
3. Nhấp vào **Install** để cài đặt extension chính thức của Wokwi.

### 3.2. Cách chuyển bài mô phỏng (1 bước cực kỳ đơn giản)
Mở Terminal trong VS Code tại thư mục `lab2_iot` và chạy script chuyển bài có sẵn:

- **Dùng lệnh Batch (Command Prompt hoặc PowerShell)**:
  ```cmd
  .\switch_bai.bat 1    # Chuyển sang Bài 1
  .\switch_bai.bat 2    # Chuyển sang Bài 2
  .\switch_bai.bat 4    # Chuyển sang Bài 4
  ```
  *(Nếu chỉ gõ `.\switch_bai.bat`, chương trình sẽ hiện menu số từ 1 đến 8 để bạn chọn).*

- **Hoặc dùng lệnh PowerShell**:
  ```powershell
  .\switch_bai.ps1 1
  ```

- **Hoặc dùng VS Code Task**:
  Nhấn `Ctrl + Shift + P` -> Gõ `Tasks: Run Task` -> Chọn bài bạn muốn mô phỏng (ví dụ: `Wokwi: Chon Bai 2`).

Script sẽ tự động:
1. Trỏ `wokwi.toml` sang file firmware của bài bạn chọn.
2. Cập nhật sơ đồ mạch `diagram.json` tương ứng với linh kiện của bài đó.
3. Kiểm tra và tự động biên dịch firmware nếu chưa có.

### 3.3. Các thao tác chạy mô phỏng và tương tác linh kiện
1. **Khởi động mô phỏng**:
   - Nhấp đúp chuột mở file `diagram.json` ở thư mục gốc `lab2_iot`.
   - Giao diện trực quan mô phỏng sẽ hiện ra. Nhấp vào nút **PLAY (tam giác màu xanh lá cây)** ở góc trên để bắt đầu chạy.
2. **Xem Serial Monitor**:
   - Cửa sổ Serial Monitor sẽ tự động xuất hiện ở mép dưới của khung mô phỏng (tốc độ 115200 baud). Mọi dữ liệu in ra sẽ hiển thị theo thời gian thực.
3. **Cách nhấn nút (Button)**:
   - Dùng chuột nhấp vào nút nhấn (Pushbutton) trên màn hình để mô phỏng sự kiện nhấn nút (`INPUT_PULLUP` kéo xuống GND). Thả chuột ra là nhả nút.
4. **Cách thay đổi nhiệt độ / độ ẩm trên cảm biến DHT11**:
   - Trong lúc mô phỏng đang chạy, nhấp chuột trái vào cảm biến **DHT11**.
   - Một thanh trượt nhiệt độ (Temperature) và độ ẩm (Humidity) sẽ xuất hiện -> Kéo thanh trượt để thay đổi giá trị theo ý muốn (ví dụ kéo lên > 30°C ở Bài 7 để kích hoạt còi Buzzer).
5. **Dừng mô phỏng**:
   - Nhấp vào nút **STOP / PAUSE (hình vuông đỏ hoặc tạm dừng)** ở thanh công cụ Wokwi.

---

## 4. HƯỚNG DẪN NẠP VÀO BOARD THẬT (NẾU CÓ PHẦN CỨNG)

Nếu bạn có board ESP32 thật và muốn nạp code thực tế:

### 4.1. Chọn Environment
1. Nhìn xuống **thanh Status Bar ở đáy VS Code**.
2. Tìm nút môi trường (mặc định `env:bai01`) và click để chọn bài từ `env:bai01` đến `env:bai08`.

### 4.2. Build và Upload
- **Build (Biên dịch)**: Nhấp dấu **✔** ở Status Bar hoặc gõ `pio run -e bai01`.
- **Upload (Nạp code)**: Nhấp mũi tên **➔** ở Status Bar hoặc gõ `pio run -e bai01 -t upload`.
- **Mở Serial Monitor**: Nhấp biểu tượng **🔌** ở Status Bar hoặc gõ `pio device monitor`.

---

## 4. CHI TIẾT TỪNG BÀI THỰC HÀNH

---

### BÀI 1: HAI LED NHẤP NHÁY ĐỘC LẬP

#### Linh kiện & Sơ đồ nối dây
- **LED 1**: Cực dương (Anode - chân dài) nối vào **GPIO 2**, cực âm (Cathode - chân ngắn) nối qua trở 220Ω về **GND**.
- **LED 2**: Cực dương nối vào **GPIO 4**, cực âm nối qua trở 220Ω về **GND**.

#### Nguyên lý hoạt động
Khởi tạo 2 biến thời gian độc lập: `previousMillisLed1` và `previousMillisLed2`.
Trong mỗi chu kỳ `loop()`:
- Nếu `currentMillis - previousMillisLed1 >= 500`: Đảo trạng thái LED 1 và cập nhật lại mốc thời gian của LED 1.
- Nếu `currentMillis - previousMillisLed2 >= 1200`: Đảo trạng thái LED 2 và cập nhật lại mốc thời gian của LED 2.
Hai tác vụ diễn ra song song, nhịp 500 ms của LED 1 hoàn toàn không bị ảnh hưởng bởi nhịp 1200 ms của LED 2.

#### Kết quả mong đợi trên Serial Monitor
```text
==============================================
Lab 2 - Bai 1: Hai LED nhap nhay doc lap
LED 1 (GPIO 2): chu ky toggle 500 ms
LED 2 (GPIO 4): chu ky toggle 1200 ms
==============================================
[Time: 500 ms] LED 1 (GPIO 2) -> ON
[Time: 1000 ms] LED 1 (GPIO 2) -> OFF
[Time: 1200 ms] LED 2 (GPIO 4) -> ON
[Time: 1500 ms] LED 1 (GPIO 2) -> ON
[Time: 2000 ms] LED 1 (GPIO 2) -> OFF
[Time: 2400 ms] LED 2 (GPIO 4) -> OFF
```

---

### BÀI 2: CHỐNG DỘI NÚT NHẤN (BUTTON DEBOUNCE) BẰNG MILLIS()

#### Linh kiện & Sơ đồ nối dây
- **Nút nhấn (Push Button)**: 1 chân nối vào **GPIO 18**, chân đối diện nối vào **GND**. Sử dụng chế độ nội `INPUT_PULLUP`.
- **LED 1**: Nối vào **GPIO 4** qua trở 220Ω về GND.
- **LED 2**: Nối vào **GPIO 5** qua trở 220Ω về GND.

#### Nguyên lý hoạt động
Khi tiếp điểm cơ khí đóng/mở, tín hiệu điện áp bị rung (dội) trong khoảng 5-20 ms.
Thuật toán lọc dội phần mềm không chặn:
1. `reading`: Đọc trực tiếp chân nút nhấn.
2. Nếu `reading != lastReading`: Reset mốc thời gian `lastDebounceTime = currentMillis`.
3. Khi `(currentMillis - lastDebounceTime) >= DEBOUNCE_DELAY` (50 ms): Tín hiệu đã ổn định hoàn toàn.
4. Nếu trạng thái ổn định mới khác với trạng thái trước (`reading != buttonState`), cập nhật `buttonState`.
5. Bắt cạnh sườn xuống (`buttonState == LOW`): Thực hiện đảo trạng thái 2 LED duy nhất một lần.

#### Kết quả mong đợi trên Serial Monitor
```text
==============================================
Lab 2 - Bai 2: Button Debounce bang millis()
Button: GPIO 18 (INPUT_PULLUP)
LEDs: GPIO 4 & GPIO 5
Quy uoc: Nhan = LOW, Tha = HIGH
==============================================
[Time: 2410 ms] Valid Button Press Detected! LEDs -> ON
[Time: 2750 ms] Button Released.
[Time: 4120 ms] Valid Button Press Detected! LEDs -> OFF
[Time: 4400 ms] Button Released.
```

---

### BÀI 3: HEARTBEAT LED + SERIAL UPTIME

#### Linh kiện & Sơ đồ nối dây
- **Heartbeat LED**: Sử dụng ngay LED xanh tích hợp sẵn trên board ESP32 (**GPIO 2**).

#### Nguyên lý hoạt động
Mô phỏng nhịp tim thiết bị công nghiệp (Watchdog / Liveness indicator).
- Timer 1 (`previousMillisHeartbeat`): Chu kỳ nhấp nháy 1 giây (500 ms sáng, 500 ms tắt).
- Timer 2 (`previousMillisSerial`): Cứ mỗi 3000 ms gửi thông điệp uptime lên Serial Monitor.
Cả 2 tác vụ không dùng delay, đảm bảo hệ thống luôn đáp ứng mượt mà.

#### Kết quả mong đợi trên Serial Monitor
```text
==============================================
Lab 2 - Bai 3: Heartbeat LED + Serial Uptime
Heartbeat LED: GPIO 2 (Chu ky 1000 ms)
Serial log: Moi 3000 ms
==============================================
System running - uptime: 3000 ms
System running - uptime: 6000 ms
System running - uptime: 9000 ms
System running - uptime: 12000 ms
```

---

### BÀI 4: ĐỌC DHT11 KHÔNG CHẶN + NÚT NHẤN PHẢN HỒI TỨC THÌ

#### Linh kiện & Sơ đồ nối dây
- **Module DHT11**:
  - Chân `VCC`: Nối vào nguồn **3.3V** (hoặc 5V tùy loại module).
  - Chân `GND`: Nối vào **GND**.
  - Chân `DATA`: Nối vào **GPIO 23** (đã tích hợp trở kéo lên trên module).
- **Button**: Nối chân **GPIO 18** xuống **GND** (`INPUT_PULLUP`).
- **Indicator LED**: Nối chân **GPIO 2** (LED onboard).

#### Nguyên lý hoạt động
Cảm biến DHT11 cần tối thiểu 1.5 - 2 giây giữa hai lần đọc. Nếu dùng `delay(2000)`, nút bấm sẽ bị "đơ" hoàn toàn trong 2 giây đó.
Bằng cách sử dụng `millis()`:
- Nút bấm được kiểm tra và xử lý ở TẤT CẢ các vòng lặp `loop()`, phản hồi ngay trong vài micro-giây.
- Cảm biến DHT11 chỉ được kích hoạt đọc khi đồng hồ đạt chu kỳ 2000 ms.
- Xử lý ngoại lệ: Sử dụng `isnan()` để phát hiện nếu cảm biến bị tuột dây, in cảnh báo mà không làm treo ESP32.

#### Kết quả mong đợi trên Serial Monitor
```text
==================================================
Lab 2 - Bai 4: DHT11 + Button Non-blocking
DHT11 Data Pin: GPIO 23 (Chu ky doc: 2000 ms)
Button Pin    : GPIO 18 (INPUT_PULLUP, phan hoi tuc thi)
LED Indicator : GPIO 2
==================================================
[Time: 840 ms] Button pressed! Indicator LED -> ON
[Time: 2000 ms] DHT11 -> Nhiet do: 28.5 *C | Do am: 65.0 %
[Time: 2310 ms] Button pressed! Indicator LED -> OFF
[Time: 4000 ms] DHT11 -> Nhiet do: 28.6 *C | Do am: 64.0 %
```

---

### BÀI 5: BA TÁC VỤ SONG SONG TRONG LOOP()

#### Linh kiện & Sơ đồ nối dây
- **LED 1**: Nối vào **GPIO 2** (trở 220Ω về GND).
- **LED 2**: Nối vào **GPIO 4** (trở 220Ω về GND).
- **Button**: Nối vào **GPIO 18** về GND (`INPUT_PULLUP`).

#### Nguyên lý hoạt động
- **Task 1 (Time-triggered)**: Nhấp nháy LED 1 mỗi 300 ms.
- **Task 2 (Event-driven)**: Lắng nghe nút nhấn có debounce, khi nhấn thì đảo trạng thái LED 2.
- **Task 3 (Time-triggered Profiler)**: Biến `loopCounter` tăng liên tục sau mỗi vòng lặp. Cứ mỗi 1000 ms, Task 3 in ra tổng số vòng lặp `loop()` đã chạy trong 1 giây qua và reset về 0.
Kết quả cho thấy `loop()` chạy được hàng trăm nghìn lần/giây, chứng minh hệ thống hoàn toàn không có điểm nghẽn (zero-blocking).

#### Kết quả mong đợi trên Serial Monitor
```text
==========================================================
Lab 2 - Bai 5: Ba tac vu song song (Time-triggered & Event-driven)
Task 1: LED 1 (GPIO 2) nhap nhay moi 300 ms
Task 2: Button (GPIO 18) debounce dao trang thai LED 2 (GPIO 4)
Task 3: Do tan so loop() - bao cao so vong lap moi giay
==========================================================
[Time: 1000 ms] [Task 3] Loop count per second: 421850
[Time: 1450 ms] [Task 2] Button Pressed -> LED 2 toggled to ON
[Time: 2000 ms] [Task 3] Loop count per second: 418290
[Time: 3000 ms] [Task 3] Loop count per second: 423100
```

---

### BÀI 6: ĐÈN GIAO THÔNG FSM (MÁY TRẠNG THÁI HỮU HẠN)

#### Linh kiện & Sơ đồ nối dây
- **LED Đỏ (RED)**: Cực dương nối vào **GPIO 25**, cực âm qua trở 220Ω về **GND**.
- **LED Vàng (YELLOW)**: Cực dương nối vào **GPIO 26**, cực âm qua trở 220Ω về **GND**.
- **LED Xanh (GREEN)**: Cực dương nối vào **GPIO 27**, cực âm qua trở 220Ω về **GND**.

#### Nguyên lý hoạt động
Hệ thống được tổ chức thành một Máy trạng thái hữu hạn (FSM):
- Định nghĩa trạng thái bằng `enum TrafficState { STATE_RED, STATE_GREEN, STATE_YELLOW };`.
- Thời gian duy trì:
  - `STATE_RED`   : 5000 ms
  - `STATE_GREEN` : 4000 ms
  - `STATE_YELLOW`: 2000 ms
- Chu trình chuyển đổi: `RED -> GREEN -> YELLOW -> RED`.
- Khi chuyển trạng thái, hàm `applyLedOutputs()` lập tức bật duy nhất 1 đèn và tắt 2 đèn còn lại.

#### Kết quả mong đợi trên Serial Monitor
```text
==================================================
Lab 2 - Bai 6: Den giao thong State Machine
Chu trinh: RED (5s) -> GREEN (4s) -> YELLOW (2s)
GPIO: RED=25, YELLOW=26, GREEN=27
==================================================
[Time: 0 ms] >>> State Changed -> RED (Duration: 5000 ms)
[Time: 5000 ms] >>> State Changed -> GREEN (Duration: 4000 ms)
[Time: 9000 ms] >>> State Changed -> YELLOW (Duration: 2000 ms)
[Time: 11000 ms] >>> State Changed -> RED (Duration: 5000 ms)
```

---

### BÀI 7: CẢNH BÁO NHIỆT ĐỘ DHT11 + BUZZER NON-BLOCKING

#### Linh kiện & Sơ đồ nối dây
- **DHT11 Data**: Chân dữ liệu nối vào **GPIO 23**, nguồn 3.3V và GND.
- **Buzzer Active**: Cực dương (+) nối vào **GPIO 15**, cực âm (-) nối vào **GND**.
- **LED Cảnh báo**: Sử dụng LED onboard **GPIO 2**.

#### Nguyên lý hoạt động
- **Task cảm biến**: Đọc DHT11 mỗi 2000 ms. So sánh nhiệt độ với ngưỡng 30.0°C.
  - Nếu `Temp > 30.0°C`: Đặt cờ `isAlarmActive = true`.
  - Nếu `Temp <= 30.0°C`: Đặt cờ `isAlarmActive = false` và cưỡng bức tắt còi.
- **Task còi báo động**: Khi `isAlarmActive == true`, còi được đảo trạng thái (Bật/Tắt) mỗi 200 ms dựa trên biến `previousMillisBuzzer`.
- Tuyệt đối không dùng `delay(200)`, việc đọc cảm biến và điều khiển còi chạy hoàn toàn độc lập.
- Nếu DHT11 bị mất kết nối (`isnan`), còi sẽ được giữ tắt an toàn.

#### Kết quả mong đợi trên Serial Monitor
```text
==================================================
Lab 2 - Bai 7: Canh bao nhiet do DHT11 + Buzzer
DHT11 Data Pin: GPIO 23 (Doc moi 2000 ms)
Buzzer Pin    : GPIO 15 (Bip ON/OFF moi 200 ms khi > 30*C)
Nguong bao dong: > 30.0 *C
==================================================
[Time: 2000 ms] DHT11 -> Temp: 28.5 *C | Hum: 65.0 % | Alarm: NORMAL (Nhiet do an toan)
[Time: 4000 ms] DHT11 -> Temp: 31.2 *C | Hum: 68.0 % | Alarm: >>> DANGER! (Nhiet do > 30*C - BUZZER ON) <<<
[Time: 6000 ms] DHT11 -> Temp: 31.0 *C | Hum: 67.0 % | Alarm: >>> DANGER! (Nhiet do > 30*C - BUZZER ON) <<<
[Time: 8000 ms] DHT11 -> Temp: 29.8 *C | Hum: 64.0 % | Alarm: NORMAL (Nhiet do an toan)
```

---

### BÀI 8: ĐIỀU KHIỂN ĐỘ SÁNG LEDC PWM + NÚT BẤM + HEARTBEAT

#### Linh kiện & Sơ đồ nối dây
- **PWM LED**: Cực dương nối vào **GPIO 4** (qua trở 220Ω), cực âm nối **GND**.
- **Heartbeat LED**: Sử dụng LED onboard **GPIO 2**.
- **Button**: Nối **GPIO 18** xuống **GND** (`INPUT_PULLUP`).

#### Nguyên lý hoạt động
- Sử dụng bộ phát xung phần cứng **LEDC PWM** của ESP32:
  - Tần số: 5000 Hz (5 kHz).
  - Độ phân giải: 8-bit (Giá trị Duty cycle từ 0 đến 255).
- Mảng độ sáng 5 cấp độ:
  - `0%` -> Duty = 0
  - `25%` -> Duty = 64
  - `50%` -> Duty = 128
  - `75%` -> Duty = 191
  - `100%` -> Duty = 255
- Nút bấm có debounce bằng `millis()`: Mỗi lần nhấn hợp lệ, con trỏ tăng 1 nấc và quay vòng (0 -> 1 -> 2 -> 3 -> 4 -> 0).
- LED Heartbeat nhấp nháy chu kỳ 1.5s (750 ms sáng / 750 ms tắt) hoàn toàn độc lập.
- Tương thích chuẩn API mới của ESP32 Core v3 (`ledcAttach` / `ledcWrite`), đồng thời hỗ trợ core v2 cũ.

#### Kết quả mong đợi trên Serial Monitor
```text
==================================================
Lab 2 - Bai 8: PWM LED + Button + Heartbeat
PWM LED Pin   : GPIO 4 (8-bit resolution, 5kHz)
Levels        : 0% -> 25% -> 50% -> 75% -> 100%
Heartbeat Pin : GPIO 2 (Chu ky 1.5s)
Button Pin    : GPIO 18 (Debounce 50 ms)
==================================================
[Time: 2150 ms] Button Pressed -> Brightness Level: 25%  (Duty: 64/255)
[Time: 3820 ms] Button Pressed -> Brightness Level: 50%  (Duty: 128/255)
[Time: 5100 ms] Button Pressed -> Brightness Level: 75%  (Duty: 191/255)
[Time: 6540 ms] Button Pressed -> Brightness Level: 100% (Duty: 255/255)
[Time: 8200 ms] Button Pressed -> Brightness Level: 0%   (Duty: 0/255)
```

---

## 5. BẢNG SO SÁNH `millis()` VÀ `delay()`

| Tiêu chí | `delay(ms)` | `millis()` |
| :--- | :--- | :--- |
| **Bản chất hoạt động** | **Chặn (Blocking)**: CPU bị kẹt trong một vòng lặp rỗng đếm chu kỳ máy, không làm được bất kỳ tác vụ nào khác. | **Không chặn (Non-blocking)**: CPU chỉ đọc giá trị bộ đếm thời gian phần cứng và tiếp tục thực thi ngay các dòng lệnh kế tiếp. |
| **Khả năng đa nhiệm** | Không thể chạy nhiều tác vụ có chu kỳ khác nhau. | Chạy song song không giới hạn các tác vụ độc lập (LED nhấp nháy, đọc cảm biến, bấm nút, giao tiếp mạng...). |
| **Thời gian phản hồi nút nhấn** | Rất kém, nút nhấn bị "bỏ rơi" khi CPU đang bận delay. | Phản hồi tức thì trong micro-giây ở mọi vòng lặp `loop()`. |
| **Tiết kiệm năng lượng & Tối ưu** | Lãng phí tài nguyên tính toán của vi điều khiển. | Tối ưu hóa chu kỳ CPU, cho phép tích hợp các cơ chế ngủ hoặc máy trạng thái hiệu quả. |

