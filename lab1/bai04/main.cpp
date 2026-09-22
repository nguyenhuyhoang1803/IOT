/**
 * ==============================================================================
 * LAB 1 - BÀI 4: CẢNH BÁO NHIỆT ĐỘ QUÁ NGƯỠNG (DHT22 + ALARM LED)
 * ==============================================================================
 * Mục tiêu:
 * - Giao tiếp và đọc dữ liệu nhiệt độ, độ ẩm từ cảm biến DHT22 định kỳ mỗi 2000 ms.
 * - Kiểm tra ngưỡng nhiệt độ an toàn: Ngưỡng cảnh báo là 30.0°C.
 * - Nếu nhiệt độ > 30.0°C: LED cảnh báo nhấp nháy liên tục (Blink non-blocking).
 * - Nếu nhiệt độ <= 30.0°C: LED cảnh báo TẮT hoàn toàn.
 * - Xử lý an toàn khi cảm biến lỗi (NaN): Thông báo lỗi lên Serial và giữ LED tắt.
 * - Sử dụng millis() để điều khiển nhấp nháy LED, không block tiến trình đọc cảm biến.
 * 
 * Sơ đồ chân (Pinout):
 * - DHT22 Data : GPIO 23 (VCC nối 3.3V, GND nối GND)
 * - Alarm LED  : GPIO 2  (nối qua điện trở 220Ω xuống GND)
 * ==============================================================================
 */

#include <Arduino.h>
#include <DHT.h>

// ==============================================================================
// CẤU HÌNH GPIO & PHẦN CỨNG
// ==============================================================================
constexpr int DHT_PIN   = 23; // Chân Data cảm biến DHT22
constexpr int ALARM_LED = 2;  // Chân LED cảnh báo

#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

// ==============================================================================
// CẤU HÌNH THỜI GIAN & NGƯỠNG
// ==============================================================================
constexpr unsigned long DHT_READ_INTERVAL = 2000; // Đọc cảm biến mỗi 2 giây (2000 ms)
constexpr unsigned long BLINK_INTERVAL    = 250;  // Nhấp nháy LED: 250 ms ON / 250 ms OFF
constexpr float TEMP_THRESHOLD           = 30.0; // Ngưỡng nhiệt độ báo động (30.0 °C)

// ==============================================================================
// BIẾN THỜI GIAN & TRẠNG THÁI
// ==============================================================================
unsigned long previousMillisDht   = 0;
unsigned long previousMillisBlink = 0;

bool isAlarmActive = false; // Trạng thái cảnh báo: true = Quá nhiệt (>30°C), false = Bình thường
bool ledState      = false; // Trạng thái bật/tắt của LED khi đang nhấp nháy

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 1000);

    Serial.println("==================================================");
    Serial.println("Lab 1 - Bai 4: Canh bao nhiet do DHT22 + LED Blink");
    Serial.println("DHT22 Pin: GPIO 23 | Alarm LED: GPIO 2");
    Serial.println("Nguong bao dong: > 30.0 *C");
    Serial.println("==================================================");

    pinMode(ALARM_LED, OUTPUT);
    digitalWrite(ALARM_LED, LOW);

    // Khởi động cảm biến DHT
    dht.begin();
}

void loop() {
    unsigned long currentMillis = millis();

    // --------------------------------------------------------------------------
    // Task 1: Đọc cảm biến DHT22 mỗi 2000 ms và kiểm tra ngưỡng
    // --------------------------------------------------------------------------
    if (currentMillis - previousMillisDht >= DHT_READ_INTERVAL) {
        previousMillisDht = currentMillis;

        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        // Kiểm tra lỗi nếu không đọc được giá trị từ cảm biến
        if (isnan(temperature) || isnan(humidity)) {
            Serial.println("[ERROR] Khong the doc du lieu tu DHT22! LED OFF.");
            isAlarmActive = false;
            ledState = false;
            digitalWrite(ALARM_LED, LOW);
        } else {
            Serial.print("Temp: ");
            Serial.print(temperature, 1);
            Serial.print(" C | Humidity: ");
            Serial.print(humidity, 1);
            Serial.print(" % | ");

            // Đánh giá ngưỡng nhiệt độ
            if (temperature > TEMP_THRESHOLD) {
                isAlarmActive = true;
                Serial.println("WARNING (Nhiet do > 30*C - LED BLINK)");
            } else {
                isAlarmActive = false;
                Serial.println("NORMAL");
                // Tắt LED ngay lập tức khi về ngưỡng an toàn
                ledState = false;
                digitalWrite(ALARM_LED, LOW);
            }
        }
    }

    // --------------------------------------------------------------------------
    // Task 2: Điều khiển nhấp nháy LED cảnh báo (Non-blocking bằng millis)
    // --------------------------------------------------------------------------
    if (isAlarmActive) {
        if (currentMillis - previousMillisBlink >= BLINK_INTERVAL) {
            previousMillisBlink = currentMillis;
            ledState = !ledState;
            digitalWrite(ALARM_LED, ledState ? HIGH : LOW);
        }
    }
}

