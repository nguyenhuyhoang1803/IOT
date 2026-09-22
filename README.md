# IOT - ESP32 Labs (PlatformIO + Wokwi Simulator)

Repository chứa toàn bộ mã nguồn và cấu hình mô phỏng Wokwi cho môn học **Internet of Things (IoT)** sử dụng vi điều khiển **ESP32** và **Arduino Framework**.

Tất cả các bài thực hành đều được cấu hình chạy trực tiếp bằng **Wokwi Simulator trong VS Code** mà không cần board phần cứng thật.

---

## Danh Sách Các Bài Lab

### 1. [Lab 1: Lập Trình Cơ Bản ESP32](lab1/README.md)
* **Bài 1**: Mô phỏng Đèn Giao Thông (GPIO 27, 26, 25 - delay).
* **Bài 2**: Button điều khiển LED trực tiếp (GPIO 18, 2 - INPUT_PULLUP).
* **Bài 3**: Công tắc thông minh Toggle Switch + Debounce bằng millis() (GPIO 18, 2).
* **Bài 4**: Cảnh báo nhiệt độ quá ngưỡng DHT22 (GPIO 23, 2 - LED Blink non-blocking).
* **Bài 5**: Hiển thị thời gian Uptime trên LCD 16x2 I2C (GPIO 21, 22 - Anti-flicker).
* **Homework 1**: Trạm thời tiết mini (DHT22, LCD, Button chuyển đổi độ F trong đúng 5s, LED cảnh báo độ ẩm > 80%).
* **Homework 2**: Đồng hồ bấm giờ Stopwatch MM:SS:ms (LCD 16x2, Button A Start/Pause, Button B Reset).
* **Homework 3**: Simple LCD Menu 3 trang chuyển đổi tuần tự (LCD, DHT22, Button - switch-case).

### 2. [Lab 2: Lập Trình Non-Blocking & State Machine](lab2/lab2_iot/README.md)
* **Bài 1**: Hai LED nhấp nháy độc lập bằng millis() (500 ms & 1200 ms).
* **Bài 2**: Button Debounce bằng millis() đảo trạng thái 2 LED.
* **Bài 3**: Heartbeat LED (1s) + Serial Uptime định kỳ (3s).
* **Bài 4**: Đọc cảm biến không chặn (2s) + Button phản hồi tức thì.
* **Bài 5**: Ba tác vụ song song (LED 300ms, Button toggle LED, đo loop count per second).
* **Bài 6**: Mô phỏng Đèn Giao Thông bằng máy trạng thái hữu hạn (FSM enum & switch-case).
* **Bài 7**: Cảnh báo nhiệt độ DHT11/DHT22 + Còi Buzzer bíp ngắt quãng non-blocking (200ms ON / 200ms OFF).
* **Bài 8**: Điều khiển độ sáng LED 5 mức bằng phần cứng LEDC PWM (8-bit) + Nút bấm + Heartbeat.

---

## Công Nghệ & Phần Cứng
* **Vi điều khiển**: ESP32 DevKit V1 (`esp32dev`)
* **Môi trường phát triển**: VS Code + PlatformIO Core
* **Framework**: Arduino Framework (C++)
* **Mô phỏng**: Wokwi Simulator extension for VS Code