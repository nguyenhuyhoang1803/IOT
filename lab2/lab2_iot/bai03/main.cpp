/**
 * ==============================================================================
 * BÀI 3: HEARTBEAT LED + SERIAL UPTIME KHÔNG CHẶN
 * ==============================================================================
 * Mục tiêu:
 * - LED Heartbeat (nhịp tim hệ thống) nhấp nháy đều đặn với chu kỳ 1 giây (500 ms bật,
 *   500 ms tắt) để báo hiệu vi điều khiển vẫn đang sống và hoạt động bình thường.
 * - Cứ mỗi 3 giây (3000 ms), gửi thông điệp uptime lên Serial Monitor:
 *   `System running - uptime: X ms`
 * - Hai tác vụ có chu kỳ hoàn toàn khác nhau (1s vs 3s), tuyệt đối không dùng delay()
 *   và không tác vụ nào được cản trở tác vụ còn lại.
 * 
 * Sơ đồ chân (Pinout):
 * - Heartbeat LED: GPIO 2 (LED onboard của board ESP32 DevKit)
 * 
 * ==============================================================================
 */

#include <Arduino.h>

// Định nghĩa chân GPIO
constexpr int HEARTBEAT_LED_PIN = 2; // LED tích hợp sẵn trên board ESP32

// Định nghĩa các chu kỳ (ms)
constexpr unsigned long HEARTBEAT_INTERVAL = 500; // Nửa chu kỳ: 500 ms ON, 500 ms OFF -> Chu kỳ 1s
constexpr unsigned long SERIAL_INTERVAL = 3000;   // Gửi Serial định kỳ mỗi 3 giây

// Biến lưu mốc thời gian độc lập cho từng tác vụ
unsigned long previousMillisHeartbeat = 0;
unsigned long previousMillisSerial = 0;

// Trạng thái hiện tại của LED Heartbeat
bool heartbeatState = false;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 1000);

    Serial.println("==============================================");
    Serial.println("Lab 2 - Bai 3: Heartbeat LED + Serial Uptime");
    Serial.println("Heartbeat LED: GPIO 2 (Chu ky 1000 ms)");
    Serial.println("Serial log: Moi 3000 ms");
    Serial.println("==============================================");

    pinMode(HEARTBEAT_LED_PIN, OUTPUT);
    digitalWrite(HEARTBEAT_LED_PIN, LOW);
}

void loop() {
    unsigned long currentMillis = millis();

    // --------------------------------------------------------------------------
    // Task 1: Điều khiển nhịp tim LED (Chu kỳ 1s: 500ms sáng / 500ms tối)
    // --------------------------------------------------------------------------
    if (currentMillis - previousMillisHeartbeat >= HEARTBEAT_INTERVAL) {
        previousMillisHeartbeat = currentMillis;

        heartbeatState = !heartbeatState;
        digitalWrite(HEARTBEAT_LED_PIN, heartbeatState ? HIGH : LOW);
    }

    // --------------------------------------------------------------------------
    // Task 2: Gửi thông điệp Uptime lên Serial Monitor mỗi 3 giây
    // --------------------------------------------------------------------------
    if (currentMillis - previousMillisSerial >= SERIAL_INTERVAL) {
        previousMillisSerial = currentMillis;

        Serial.print("System running - uptime: ");
        Serial.print(currentMillis);
        Serial.println(" ms");
    }
}

