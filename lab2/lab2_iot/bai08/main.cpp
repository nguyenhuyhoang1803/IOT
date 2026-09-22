/**
 * ==============================================================================
 * BÀI 8: ĐIỀU KHIỂN ĐỘ SÁNG LED BẰNG PHẦN CỨNG LEDC PWM + NÚT BẤM + HEARTBEAT
 * ==============================================================================
 * Mục tiêu:
 * - Sử dụng khối phần cứng LEDC (LED Controller) của ESP32 để phát xung PWM điều
 *   khiển độ sáng LED với độ phân giải 8-bit (0 - 255).
 * - Mỗi lần nhấn nút bấm (GPIO 18) có debounce bằng millis(), độ sáng LED chuyển tuần tự:
 *   0% (0) -> 25% (64) -> 50% (128) -> 75% (191) -> 100% (255) -> 0% (0)...
 * - Đồng thời duy trì một LED Heartbeat (GPIO 2) độc lập nhấp nháy chu kỳ 1.5 giây
 *   (750 ms bật, 750 ms tắt) mà không gây gián đoạn hay bị gián đoạn bởi nút bấm.
 * 
 * Khả năng tương thích ESP32 Core (Arduino-ESP32 v2.x & v3.x):
 * - Ưu tiên API mới của Arduino-ESP32 v3.0+:
 *   + `ledcAttach(pin, frequency, resolution);`
 *   + `ledcWrite(pin, duty);`
 * - Cung cấp macro tiền xử lý tương thích tự động nếu môi trường biên dịch đang chạy
 *   ESP32 Core v2.x (sử dụng ledcSetup, ledcAttachPin).
 * 
 * Sơ đồ chân (Pinout):
 * - PWM LED      : GPIO 4  (nối trở 220Ω xuống GND)
 * - Heartbeat LED: GPIO 2  (LED onboard hoặc LED ngoài)
 * - Button       : GPIO 18 (chế độ INPUT_PULLUP, nối xuống GND)
 * ==============================================================================
 */

#include <Arduino.h>
#include <esp_arduino_version.h>

// ==============================================================================
// CẤU HÌNH GPIO
// ==============================================================================
constexpr int PWM_LED_PIN       = 4;  // Chân điều khiển LED độ sáng biến thiên bằng PWM
constexpr int HEARTBEAT_LED_PIN = 2;  // Chân LED nhịp tim hệ thống
constexpr int BUTTON_PIN        = 18; // Chân nút bấm (INPUT_PULLUP)

// ==============================================================================
// CẤU HÌNH LEDC PWM
// ==============================================================================
constexpr uint32_t PWM_FREQ       = 5000; // Tần số xung PWM 5 kHz (mượt, không nhấp nháy)
constexpr uint8_t  PWM_RESOLUTION = 8;    // Độ phân giải 8-bit: Duty cycle từ 0 đến 255
constexpr int      PWM_CHANNEL    = 0;    // Channel 0 (dùng cho core v2.x cũ)

// Mảng 5 mức độ sáng theo yêu cầu đề bài:
// 0% -> 0, 25% -> 64, 50% -> 128, 75% -> 191, 100% -> 255
constexpr uint8_t BRIGHTNESS_LEVELS[] = {0, 64, 128, 191, 255};
constexpr size_t TOTAL_LEVELS = sizeof(BRIGHTNESS_LEVELS) / sizeof(BRIGHTNESS_LEVELS[0]);

// Chỉ số mức độ sáng hiện tại (khởi đầu 0: tắt hoàn toàn)
size_t currentLevelIndex = 0;

// ==============================================================================
// CẤU HÌNH THỜI GIAN (ms)
// ==============================================================================
// Chu kỳ Heartbeat đề bài yêu cầu là 1.5 giây (1500 ms) -> Nửa chu kỳ = 750 ms
constexpr unsigned long HEARTBEAT_INTERVAL = 750;
constexpr unsigned long DEBOUNCE_DELAY     = 50;

// Biến lưu mốc thời gian
unsigned long previousMillisHeartbeat = 0;
unsigned long lastDebounceTime = 0;

// Biến trạng thái
bool heartbeatState = false;
int lastReading = HIGH;
int buttonState = HIGH;

/**
 * Hàm cập nhật Duty Cycle cho chân PWM phù hợp theo phiên bản ESP32 Core
 */
void setPwmDuty(uint8_t duty) {
#if defined(ESP_ARDUINO_VERSION) && (ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0))
    // API mới trên Arduino-ESP32 v3.0+
    ledcWrite(PWM_LED_PIN, duty);
#else
    // API cũ trên Arduino-ESP32 v2.x
    ledcWrite(PWM_CHANNEL, duty);
#endif
}

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 1000);

    Serial.println("==================================================");
    Serial.println("Lab 2 - Bai 8: PWM LED + Button + Heartbeat");
    Serial.println("PWM LED Pin   : GPIO 4 (8-bit resolution, 5kHz)");
    Serial.println("Levels        : 0% -> 25% -> 50% -> 75% -> 100%");
    Serial.println("Heartbeat Pin : GPIO 2 (Chu ky 1.5s)");
    Serial.println("Button Pin    : GPIO 18 (Debounce 50 ms)");
    Serial.println("==================================================");

    // Cấu hình chân Heartbeat và Nút bấm
    pinMode(HEARTBEAT_LED_PIN, OUTPUT);
    digitalWrite(HEARTBEAT_LED_PIN, LOW);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Khởi tạo phần cứng PWM LEDC theo đúng chuẩn phiên bản
#if defined(ESP_ARDUINO_VERSION) && (ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0))
    // Cú pháp mới v3.x: gán thẳng chân GPIO, tần số và độ phân giải
    ledcAttach(PWM_LED_PIN, PWM_FREQ, PWM_RESOLUTION);
#else
    // Cú pháp cũ v2.x: cấu hình channel rồi gán channel vào chân GPIO
    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(PWM_LED_PIN, PWM_CHANNEL);
#endif

    // Thiết lập độ sáng ban đầu là 0%
    setPwmDuty(BRIGHTNESS_LEVELS[currentLevelIndex]);
}

void loop() {
    unsigned long currentMillis = millis();

    // --------------------------------------------------------------------------
    // Task 1: Heartbeat LED độc lập (Chu kỳ 1.5s: 750 ms bật, 750 ms tắt)
    // --------------------------------------------------------------------------
    if (currentMillis - previousMillisHeartbeat >= HEARTBEAT_INTERVAL) {
        previousMillisHeartbeat = currentMillis;

        heartbeatState = !heartbeatState;
        digitalWrite(HEARTBEAT_LED_PIN, heartbeatState ? HIGH : LOW);
    }

    // --------------------------------------------------------------------------
    // Task 2: Đọc nút bấm có lọc dội (Debounce) và thay đổi độ sáng PWM
    // --------------------------------------------------------------------------
    int reading = digitalRead(BUTTON_PIN);
    if (reading != lastReading) {
        lastDebounceTime = currentMillis;
    }

    if ((currentMillis - lastDebounceTime) >= DEBOUNCE_DELAY) {
        if (reading != buttonState) {
            buttonState = reading;

            // Nhận diện sự kiện cạnh xuống (Nhấn nút)
            if (buttonState == LOW) {
                // Tăng chỉ số mức độ sáng và quay vòng (0 -> 1 -> 2 -> 3 -> 4 -> 0...)
                currentLevelIndex = (currentLevelIndex + 1) % TOTAL_LEVELS;
                uint8_t duty = BRIGHTNESS_LEVELS[currentLevelIndex];

                // Ghi giá trị Duty Cycle mới ra chân PWM
                setPwmDuty(duty);

                // In thông tin chẩn đoán lên Serial
                Serial.print("[Time: ");
                Serial.print(currentMillis);
                Serial.print(" ms] Button Pressed -> Brightness Level: ");
                switch (currentLevelIndex) {
                    case 0: Serial.print("0%   (Duty: "); break;
                    case 1: Serial.print("25%  (Duty: "); break;
                    case 2: Serial.print("50%  (Duty: "); break;
                    case 3: Serial.print("75%  (Duty: "); break;
                    case 4: Serial.print("100% (Duty: "); break;
                }
                Serial.print(duty);
                Serial.println("/255)");
            }
        }
    }
    lastReading = reading;
}

