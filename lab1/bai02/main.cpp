/**
 * ==============================================================================
 * LAB 1 - BÀI 2: BUTTON ĐIỀU KHIỂN LED TRỰC TIẾP
 * ==============================================================================
 * Mục tiêu:
 * - Đọc tín hiệu số Digital Input từ nút bấm bằng digitalRead().
 * - Sử dụng điện trở kéo lên nội bộ INPUT_PULLUP của ESP32.
 * - Quy ước hoạt động INPUT_PULLUP:
 *   + Nút NHẤN (giữ) -> chân nối xuống GND -> đọc giá trị LOW (0).
 *   + Nút THẢ       -> điện trở kéo lên 3.3V -> đọc giá trị HIGH (1).
 * 
 * Logic điều khiển:
 * - Nhấn giữ nút  -> LED BẬT (HIGH).
 * - Thả nút ra    -> LED TẮT (LOW).
 * - Chỉ in thông báo lên Serial khi trạng thái thay đổi để tránh tràn log.
 * 
 * Sơ đồ chân (Pinout):
 * - Button  : GPIO 18 (1 chân nối GPIO 18, 1 chân nối GND)
 * - LED     : GPIO 2  (nối qua điện trở 220Ω xuống GND)
 * ==============================================================================
 */

#include <Arduino.h>

// ==============================================================================
// CẤU HÌNH GPIO
// ==============================================================================
constexpr int BUTTON_PIN = 18; // Chân nút bấm (INPUT_PULLUP)
constexpr int LED_PIN    = 2;  // Chân điều khiển LED

// Biến lưu trạng thái trước đó để phát hiện sự thay đổi trạng thái
int lastButtonState = HIGH;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 1000);

    Serial.println("==================================================");
    Serial.println("Lab 1 - Bai 2: Button dieu khien LED truc tiep");
    Serial.println("Button: GPIO 18 (INPUT_PULLUP) | LED: GPIO 2");
    Serial.println("Giu nut -> LED ON | Tha nut -> LED OFF");
    Serial.println("==================================================");

    // Cấu hình chân
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);

    // Trạng thái ban đầu: LED tắt
    digitalWrite(LED_PIN, LOW);
}

void loop() {
    // Đọc trạng thái tức thời của nút bấm
    int currentButtonState = digitalRead(BUTTON_PIN);

    // Kiểm tra xem trạng thái nút bấm có thay đổi so với vòng lặp trước không
    if (currentButtonState != lastButtonState) {
        lastButtonState = currentButtonState;

        if (currentButtonState == LOW) {
            // Nút đang được nhấn giữ
            digitalWrite(LED_PIN, HIGH);
            Serial.println("Button PRESSED -> LED ON");
        } else {
            // Nút vừa được thả ra
            digitalWrite(LED_PIN, LOW);
            Serial.println("Button RELEASED -> LED OFF");
        }
    }
}

