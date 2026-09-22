/**
 * ==============================================================================
 * LAB 1 - BÀI 1: MÔ PHỎNG ĐÈN GIAO THÔNG (TRAFFIC LIGHT)
 * ==============================================================================
 * Mục tiêu:
 * - Cấu hình nhiều chân GPIO ở chế độ OUTPUT.
 * - Sử dụng hàm delay() để tạo chu kỳ thời gian theo yêu cầu bài thực hành cơ bản.
 * - Điều khiển 3 LED: Xanh (Green), Vàng (Yellow), Đỏ (Red).
 * 
 * Chu kỳ hoạt động:
 * - GREEN ON  : 3 giây (3000 ms), YELLOW & RED tắt.
 * - YELLOW ON : 1 giây (1000 ms), GREEN & RED tắt.
 * - RED ON    : 3 giây (3000 ms), GREEN & YELLOW tắt.
 * - Lặp lại liên tục.
 * 
 * Sơ đồ chân (Pinout):
 * - Green LED  : GPIO 27 (nối qua điện trở 220Ω xuống GND)
 * - Yellow LED : GPIO 26 (nối qua điện trở 220Ω xuống GND)
 * - Red LED    : GPIO 25 (nối qua điện trở 220Ω xuống GND)
 * ==============================================================================
 */

#include <Arduino.h>

// ==============================================================================
// CẤU HÌNH GPIO
// ==============================================================================
constexpr int PIN_GREEN  = 27; // Chân điều khiển LED Xanh
constexpr int PIN_YELLOW = 26; // Chân điều khiển LED Vàng
constexpr int PIN_RED    = 25; // Chân điều khiển LED Đỏ

// ==============================================================================
// CẤU HÌNH THỜI GIAN (mili-giây)
// ==============================================================================
constexpr unsigned long TIME_GREEN  = 3000; // 3 giây
constexpr unsigned long TIME_YELLOW = 1000; // 1 giây
constexpr unsigned long TIME_RED    = 3000; // 3 giây

/**
 * Hàm điều khiển trạng thái 3 đèn cùng một lúc
 */
void setTrafficLights(bool green, bool yellow, bool red) {
    digitalWrite(PIN_GREEN,  green  ? HIGH : LOW);
    digitalWrite(PIN_YELLOW, yellow ? HIGH : LOW);
    digitalWrite(PIN_RED,    red    ? HIGH : LOW);
}

void setup() {
    // Khởi tạo giao tiếp Serial với baud rate 115200
    Serial.begin(115200);
    while (!Serial && millis() < 1000);

    Serial.println("==================================================");
    Serial.println("Lab 1 - Bai 1: Mo phong Den Giao Thong (ESP32)");
    Serial.println("Green: GPIO 27 | Yellow: GPIO 26 | Red: GPIO 25");
    Serial.println("==================================================");

    // Cấu hình các chân LED là OUTPUT
    pinMode(PIN_GREEN,  OUTPUT);
    pinMode(PIN_YELLOW, OUTPUT);
    pinMode(PIN_RED,    OUTPUT);

    // Ban đầu tắt tất cả các đèn
    setTrafficLights(false, false, false);
}

void loop() {
    // 1. BẬT ĐÈN XANH (3 giây)
    Serial.println("GREEN - 3 seconds");
    setTrafficLights(true, false, false);
    delay(TIME_GREEN);

    // 2. BẬT ĐÈN VÀNG (1 giây)
    Serial.println("YELLOW - 1 second");
    setTrafficLights(false, true, false);
    delay(TIME_YELLOW);

    // 3. BẬT ĐÈN ĐỎ (3 giây)
    Serial.println("RED - 3 seconds");
    setTrafficLights(false, false, true);
    delay(TIME_RED);
}

