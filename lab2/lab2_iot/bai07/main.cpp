/**
 * ==============================================================================
 * BÀI 7: CẢNH BÁO NHIỆT ĐỘ DHT11 + CÒI BUZZER NGẮT QUÃNG NON-BLOCKING
 * ==============================================================================
 * Mục tiêu:
 * - Đọc nhiệt độ và độ ẩm từ cảm biến DHT11 mỗi 2000 ms.
 * - Ngưỡng cảnh báo: 30.0°C.
 * - Nếu nhiệt độ > 30.0°C:
 *   Buzzer kêu ngắt quãng (ON 200 ms, OFF 200 ms, lặp lại liên tục).
 *   Cờ alarm = true.
 * - Nếu nhiệt độ <= 30.0°C:
 *   Buzzer TẮT hoàn toàn (LOW).
 *   Cờ alarm = false.
 * - Tuyệt đối KHÔNG sử dụng delay().
 * - Task 1 (đọc DHT11) và Task 2 (điều khiển Buzzer) chạy độc lập bằng millis().
 * - Kiểm tra an toàn: Nếu cảm biến trả về NaN, in thông báo lỗi, giữ hệ thống an
 *   toàn và tắt còi báo động.
 * 
 * Sơ đồ chân (Pinout):
 * - DHT11 Data : GPIO 23
 * - Buzzer (+) : GPIO 15 (Active Buzzer, cực âm nối GND)
 * - Alarm LED  : GPIO 2  (LED chỉ báo trực quan)
 * ==============================================================================
 */

#include <Arduino.h>
#include <DHT.h>

// ==============================================================================
// 1. CẤU HÌNH PHẦN CỨNG & HẰNG SỐ
// ==============================================================================
constexpr int DHT_PIN = 23;                       // Chân Data cảm biến DHT11
constexpr int BUZZER_PIN = 15;                    // Chân tín hiệu còi Buzzer
constexpr int ALARM_LED = 2;                      // LED báo động (GPIO 2 tích hợp trên ESP32)

constexpr float TEMPERATURE_THRESHOLD = 30.0;    // Ngưỡng nhiệt độ cảnh báo (30°C)

constexpr unsigned long DHT_INTERVAL = 2000;      // Chu kỳ đọc DHT11: 2000 ms
constexpr unsigned long BUZZER_INTERVAL = 200;    // Chu kỳ bật/tắt Buzzer: 200 ms

// Lưu ý Wokwi: Wokwi dùng DHT22 mô phỏng thay thế cho DHT11 (giao thức tương đương)
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

// ==============================================================================
// 2. BIẾN THỜI GIAN ĐỘC LẬP & TRẠNG THÁI
// ==============================================================================
unsigned long previousMillisDht = 0;              // Timer độc lập cho Task đọc DHT11
unsigned long previousMillisBuzzer = 0;           // Timer độc lập cho Task điều khiển Buzzer

bool isAlarmActive = false;                      // Cờ báo động: true = vượt ngưỡng (>30°C), false = an toàn
bool buzzerState = false;                         // Trạng thái ON/OFF hiện tại của Buzzer khi đang kêu ngắt quãng

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 1000);

    Serial.println("==================================================");
    Serial.println("Lab 2 - Bai 7: Canh bao nhiet do DHT11 + Buzzer");
    Serial.println("DHT11 Data Pin : GPIO 23 (Doc moi 2000 ms)");
    Serial.println("Buzzer Pin     : GPIO 15 (Bip ON/OFF moi 200 ms khi > 30 C)");
    Serial.println("Nguong bao dong: > 30.0 C");
    Serial.println("==================================================");

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(ALARM_LED, OUTPUT);

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(ALARM_LED, LOW);

    dht.begin();
}

void loop() {
    unsigned long currentMillis = millis();

    // --------------------------------------------------------------------------
    // TASK 1: Đọc cảm biến DHT11 mỗi 2000 ms và cập nhật trạng thái báo động
    // --------------------------------------------------------------------------
    if (currentMillis - previousMillisDht >= DHT_INTERVAL) {
        previousMillisDht = currentMillis;

        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        // Xử lý an toàn: Nếu cảm biến lỗi (NaN), tắt còi và báo lỗi lên Serial
        if (isnan(temperature) || isnan(humidity)) {
            Serial.print("[Time: ");
            Serial.print(currentMillis);
            Serial.println(" ms] [ERROR] DHT11 read failed! Buzzer OFF.");

            isAlarmActive = false;
            buzzerState = false;
            digitalWrite(BUZZER_PIN, LOW);
            digitalWrite(ALARM_LED, LOW);
        } else {
            Serial.print("[Time: ");
            Serial.print(currentMillis);
            Serial.print(" ms] DHT11 -> Temp: ");
            Serial.print(temperature, 1);
            Serial.print(" C | Hum: ");
            Serial.print(humidity, 1);
            Serial.print(" % | Alarm: ");

            if (temperature > TEMPERATURE_THRESHOLD) {
                isAlarmActive = true;
                Serial.println("DANGER");
            } else {
                isAlarmActive = false;
                Serial.println("NORMAL");
                // Khi nhiệt độ an toàn (<= 30°C), tắt còi và LED ngay lập tức
                buzzerState = false;
                digitalWrite(BUZZER_PIN, LOW);
                digitalWrite(ALARM_LED, LOW);
            }
        }
    }

    // --------------------------------------------------------------------------
    // TASK 2: Điều khiển Buzzer bíp ngắt quãng non-blocking mỗi 200 ms
    // --------------------------------------------------------------------------
    if (isAlarmActive) {
        // Khi isAlarmActive == true: đảo trạng thái buzzer mỗi 200 ms
        if (currentMillis - previousMillisBuzzer >= BUZZER_INTERVAL) {
            previousMillisBuzzer = currentMillis;

            buzzerState = !buzzerState;
            digitalWrite(BUZZER_PIN, buzzerState ? HIGH : LOW);
            digitalWrite(ALARM_LED, buzzerState ? HIGH : LOW);
        }
    } else {
        // Khi isAlarmActive == false: đảm bảo buzzer và LED luôn tắt
        if (buzzerState) {
            buzzerState = false;
            digitalWrite(BUZZER_PIN, LOW);
            digitalWrite(ALARM_LED, LOW);
        }
    }
}
