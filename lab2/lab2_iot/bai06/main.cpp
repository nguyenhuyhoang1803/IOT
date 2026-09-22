/**
 * ==============================================================================
 * BÀI 6: ĐÈN GIAO THÔNG SỬ DỤNG MÁY TRẠNG THÁI HỮU HẠN (FINITE STATE MACHINE)
 * ==============================================================================
 * Mục tiêu:
 * - Mô phỏng hệ thống đèn tín hiệu giao thông: ĐỎ -> XANH -> VÀNG -> ĐỎ.
 * - Thời gian từng pha:
 *   + ĐỎ (RED)    : 5000 ms (5 giây)
 *   + XANH (GREEN): 4000 ms (4 giây)
 *   + VÀNG (YELLOW): 2000 ms (2 giây)
 * - Sử dụng kiến trúc Máy trạng thái hữu hạn (FSM) với enum và switch-case kết hợp
 *   với millis() định thời, tuyệt đối KHÔNG dùng delay().
 * - Tại mỗi trạng thái chỉ có duy nhất một đèn tương ứng được sáng, các đèn khác tắt.
 * - Tạo các hàm chuyên trách: `setTrafficState()`, `updateTrafficLight()`.
 * 
 * Sơ đồ chân (Pinout):
 * - RED LED   : GPIO 25 (qua điện trở 220Ω xuống GND)
 * - YELLOW LED: GPIO 26 (qua điện trở 220Ω xuống GND)
 * - GREEN LED : GPIO 27 (qua điện trở 220Ω xuống GND)
 * ==============================================================================
 */

#include <Arduino.h>

// ==============================================================================
// CẤU HÌNH GPIO (Định nghĩa hằng số ở đầu file)
// ==============================================================================
constexpr int RED_LED_PIN    = 25; // Đèn Đỏ
constexpr int YELLOW_LED_PIN = 26; // Đèn Vàng
constexpr int GREEN_LED_PIN  = 27; // Đèn Xanh

// ==============================================================================
// ĐỊNH NGHĨA MÁY TRẠNG THÁI (FINITE STATE MACHINE)
// ==============================================================================
enum TrafficState {
    STATE_RED,
    STATE_GREEN,
    STATE_YELLOW
};

// Thời gian duy trì từng trạng thái (ms)
constexpr unsigned long DURATION_RED    = 5000; // 5 giây
constexpr unsigned long DURATION_GREEN  = 4000; // 4 giây
constexpr unsigned long DURATION_YELLOW = 2000; // 2 giây

// Biến lưu trạng thái hiện tại và mốc thời gian bắt đầu trạng thái đó
TrafficState currentState = STATE_RED;
unsigned long stateStartTime = 0;

// ==============================================================================
// CÁC HÀM XỬ LÝ TRẠNG THÁI
// ==============================================================================

/**
 * Hàm thiết lập phần cứng đèn theo trạng thái FSM:
 * Bật duy nhất đèn tương ứng và tắt hoàn toàn 2 đèn còn lại.
 */
void applyLedOutputs(TrafficState state) {
    switch (state) {
        case STATE_RED:
            digitalWrite(RED_LED_PIN, HIGH);
            digitalWrite(YELLOW_LED_PIN, LOW);
            digitalWrite(GREEN_LED_PIN, LOW);
            break;

        case STATE_GREEN:
            digitalWrite(RED_LED_PIN, LOW);
            digitalWrite(YELLOW_LED_PIN, LOW);
            digitalWrite(GREEN_LED_PIN, HIGH);
            break;

        case STATE_YELLOW:
            digitalWrite(RED_LED_PIN, LOW);
            digitalWrite(YELLOW_LED_PIN, HIGH);
            digitalWrite(GREEN_LED_PIN, LOW);
            break;
    }
}

/**
 * Hàm chuyển trạng thái (State Transition):
 * Cập nhật trạng thái mới, ghi lại mốc thời gian bắt đầu và in log chẩn đoán.
 */
void setTrafficState(TrafficState newState, unsigned long currentMillis) {
    currentState = newState;
    stateStartTime = currentMillis;
    applyLedOutputs(currentState);

    Serial.print("[Time: ");
    Serial.print(currentMillis);
    Serial.print(" ms] >>> State Changed -> ");

    switch (currentState) {
        case STATE_RED:
            Serial.println("RED (Duration: 5000 ms)");
            break;
        case STATE_GREEN:
            Serial.println("GREEN (Duration: 4000 ms)");
            break;
        case STATE_YELLOW:
            Serial.println("YELLOW (Duration: 2000 ms)");
            break;
    }
}

/**
 * Hàm cập nhật logic máy trạng thái:
 * Được gọi liên tục trong loop(), kiểm tra xem trạng thái hiện tại đã hết thời
 * gian quy định hay chưa để kích hoạt chuyển sang trạng thái kế tiếp.
 */
void updateTrafficLight(unsigned long currentMillis) {
    switch (currentState) {
        case STATE_RED:
            // Thứ tự chuyển: ĐỎ -> XANH sau 5000 ms
            if (currentMillis - stateStartTime >= DURATION_RED) {
                setTrafficState(STATE_GREEN, currentMillis);
            }
            break;

        case STATE_GREEN:
            // Thứ tự chuyển: XANH -> VÀNG sau 4000 ms
            if (currentMillis - stateStartTime >= DURATION_GREEN) {
                setTrafficState(STATE_YELLOW, currentMillis);
            }
            break;

        case STATE_YELLOW:
            // Thứ tự chuyển: VÀNG -> ĐỎ sau 2000 ms
            if (currentMillis - stateStartTime >= DURATION_YELLOW) {
                setTrafficState(STATE_RED, currentMillis);
            }
            break;
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 1000);

    Serial.println("==================================================");
    Serial.println("Lab 2 - Bai 6: Den giao thong State Machine");
    Serial.println("Chu trinh: RED (5s) -> GREEN (4s) -> YELLOW (2s)");
    Serial.println("GPIO: RED=25, YELLOW=26, GREEN=27");
    Serial.println("==================================================");

    // Cấu hình các chân điều khiển LED
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);

    // Khởi tạo trạng thái ban đầu là ĐÈN ĐỎ
    setTrafficState(STATE_RED, millis());
}

void loop() {
    unsigned long currentMillis = millis();

    // Cập nhật máy trạng thái liên tục không chặn
    updateTrafficLight(currentMillis);
}

