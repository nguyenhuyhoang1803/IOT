/**
 * ==============================================================================
 * LAB 1 - BÀI 3: CÔNG TẮC THÔNG MINH (TOGGLE + DEBOUNCE BẰNG MILLIS)
 * ==============================================================================
 * Mục tiêu:
 * - Nâng cấp từ Bài 2: Biến nút bấm nhấn nhả thành công tắc bật/tắt (Toggle Switch).
 * - Lọc dội phím (Debounce) bằng millis(), TUYỆT ĐỐI KHÔNG dùng delay(50).
 * - Phân biệt giữa trạng thái đọc tức thời (reading) và trạng thái ổn định (stable).
 * - Chỉ đảo trạng thái LED đúng một lần khi có sự kiện cạnh xuống (nút vừa được nhấn).
 * - Giữ nút bấm KHÔNG làm LED chớp tắt liên tục.
 * 
 * Logic hoạt động:
 * - Ban đầu: LED TẮT.
 * - Nhấn nút lần 1 -> LED BẬT.
 * - Nhấn nút lần 2 -> LED TẮT.
 * - Lặp lại tuần tự.
 * 
 * Sơ đồ chân (Pinout):
 * - Button : GPIO 18 (INPUT_PULLUP, nối xuống GND)
 * - LED    : GPIO 2  (nối qua điện trở 220Ω xuống GND)
 * ==============================================================================
 */

#include <Arduino.h>

// ==============================================================================
// CẤU HÌNH GPIO & THỜI GIAN
// ==============================================================================
constexpr int BUTTON_PIN = 18; // Chân nút bấm (INPUT_PULLUP)
constexpr int LED_PIN    = 2;  // Chân điều khiển LED

constexpr unsigned long DEBOUNCE_DELAY = 50; // Thời gian chống rung phím (50 ms)

// ==============================================================================
// CÁC BIẾN QUẢN LÝ TRẠNG THÁI & THỜI GIAN
// ==============================================================================
bool ledState = false;              // Trạng thái hiện tại của LED: false = Tắt, true = Bật
int lastButtonReading = HIGH;       // Giá trị đọc thô tức thời ở vòng lặp trước
int stableButtonState = HIGH;       // Trạng thái ổn định của nút sau khi đã lọc dội
unsigned long lastDebounceTime = 0; // Mốc thời gian xuất hiện biến động tín hiệu gần nhất

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 1000);

    Serial.println("==================================================");
    Serial.println("Lab 1 - Bai 3: Cong tac thong minh (Toggle + Debounce)");
    Serial.println("Button: GPIO 18 (INPUT_PULLUP) | LED: GPIO 2");
    Serial.println("Nhan 1 lan -> Toggle LED (Khong dung delay)");
    Serial.println("==================================================");

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);

    // Khởi tạo LED tắt ban đầu
    digitalWrite(LED_PIN, LOW);
}

void loop() {
    // 1. Đọc giá trị tức thời của chân nút bấm
    int currentReading = digitalRead(BUTTON_PIN);

    // 2. Nếu có sự thay đổi giữa lần đọc này và lần trước (có nhiễu hoặc mới bấm)
    if (currentReading != lastButtonReading) {
        lastDebounceTime = millis(); // Đặt lại bộ đếm thời gian chống rung
    }

    // 3. Nếu tín hiệu giữ nguyên trạng thái lâu hơn thời gian debounce (50ms)
    if ((millis() - lastDebounceTime) >= DEBOUNCE_DELAY) {
        // Nếu trạng thái ổn định đã thực sự thay đổi
        if (currentReading != stableButtonState) {
            stableButtonState = currentReading;

            // Kiểm tra sự kiện cạnh xuống (vừa mới nhấn nút: HIGH -> LOW)
            if (stableButtonState == LOW) {
                // Đảo trạng thái LED
                ledState = !ledState;
                digitalWrite(LED_PIN, ledState ? HIGH : LOW);

                // Gửi thông báo lên Serial
                if (ledState) {
                    Serial.println("Button pressed -> LED ON");
                } else {
                    Serial.println("Button pressed -> LED OFF");
                }
            }
        }
    }

    // 4. Lưu lại giá trị đọc cho vòng lặp kế tiếp
    lastButtonReading = currentReading;
}

