/**
 * ==============================================================================
 * BÀI 4: ĐỌC CẢM BIẾN DHT11 + NÚT NHẤN KHÔNG CHẶN (NON-BLOCKING)
 * ==============================================================================
 * Mục tiêu:
 * - Đọc cảm biến DHT11 mỗi 2000 ms bằng cơ chế time-triggered (millis()).
 * - In nhiệt độ, độ ẩm và thời gian millis() lên Serial Monitor.
 * - Kiểm tra lỗi cảm biến trả về NaN bằng isnan() và xử lý an toàn.
 * - Trong suốt 2000 ms chờ cảm biến, vòng lặp loop() vẫn chạy liên tục để đọc nút
 *   nhấn với thuật toán debounce của Bài 2. Nút nhấn phản hồi tức thì mà không bị
 *   delay bởi cảm biến.
 * 
 * Sơ đồ chân (Pinout):
 * - DHT11 Data : GPIO 23 (Kèm trở kéo lên 4.7kΩ - 10kΩ nếu dùng module trần;
 *                         nếu dùng module DHT11 có sẵn 3 chân VCC, GND, DATA thì
 *                         nối trực tiếp DATA vào GPIO 23).
 * - Button     : GPIO 18 (Chế độ INPUT_PULLUP, nối nút xuống GND).
 * - Indicator  : GPIO 2  (LED chỉ thị trạng thái nút bấm).
 * ==============================================================================
 */

#include <Arduino.h>
#include <DHT.h>

// Định nghĩa chân GPIO
constexpr int DHT_PIN = 23;        // Chân dữ liệu DHT11
constexpr int BUTTON_PIN = 18;     // Chân nút nhấn (INPUT_PULLUP)
constexpr int INDICATOR_LED = 2;   // LED phản hồi nút nhấn

// Cấu hình loại cảm biến
#define DHT_TYPE DHT11

// Khởi tạo đối tượng cảm biến DHT
DHT dht(DHT_PIN, DHT_TYPE);

// Khoảng thời gian đọc cảm biến và chống dội nút nhấn
constexpr unsigned long SENSOR_INTERVAL = 2000; // Đọc DHT11 mỗi 2 giây
constexpr unsigned long DEBOUNCE_DELAY = 50;    // 50 ms lọc dội

// Biến thời gian
unsigned long previousMillisSensor = 0;
unsigned long lastDebounceTime = 0;

// Biến trạng thái nút bấm
int lastReading = HIGH;
int buttonState = HIGH;
bool indicatorState = false;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 1000);

    Serial.println("==================================================");
    Serial.println("Lab 2 - Bai 4: DHT11 + Button Non-blocking");
    Serial.println("DHT11 Data Pin: GPIO 23 (Chu ky doc: 2000 ms)");
    Serial.println("Button Pin    : GPIO 18 (INPUT_PULLUP, phan hoi tuc thi)");
    Serial.println("LED Indicator : GPIO 2");
    Serial.println("==================================================");

    // Cấu hình GPIO
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(INDICATOR_LED, OUTPUT);
    digitalWrite(INDICATOR_LED, LOW);

    // Khởi động cảm biến DHT
    dht.begin();
}

void loop() {
    unsigned long currentMillis = millis();

    // --------------------------------------------------------------------------
    // Task 1: Xử lý nút bấm phản hồi tức thời (Event-driven với Debounce)
    // Tác vụ này chạy ở mọi vòng lặp loop(), độ trễ phản hồi tính bằng micro-giây
    // --------------------------------------------------------------------------
    int reading = digitalRead(BUTTON_PIN);
    if (reading != lastReading) {
        lastDebounceTime = currentMillis;
    }

    if ((currentMillis - lastDebounceTime) >= DEBOUNCE_DELAY) {
        if (reading != buttonState) {
            buttonState = reading;
            if (buttonState == LOW) { // Nhấn nút
                indicatorState = !indicatorState;
                digitalWrite(INDICATOR_LED, indicatorState ? HIGH : LOW);

                Serial.print("[Time: ");
                Serial.print(currentMillis);
                Serial.print(" ms] Button pressed! Indicator LED -> ");
                Serial.println(indicatorState ? "ON" : "OFF");
            }
        }
    }
    lastReading = reading;

    // --------------------------------------------------------------------------
    // Task 2: Đọc cảm biến DHT11 định kỳ mỗi 2000 ms (Time-triggered)
    // --------------------------------------------------------------------------
    if (currentMillis - previousMillisSensor >= SENSOR_INTERVAL) {
        previousMillisSensor = currentMillis;

        // Đọc giá trị độ ẩm và nhiệt độ từ cảm biến
        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature();

        // Kiểm tra xem dữ liệu đọc có hợp lệ không (isnan: Is Not a Number)
        if (isnan(humidity) || isnan(temperature)) {
            Serial.print("[Time: ");
            Serial.print(currentMillis);
            Serial.println(" ms] [DHT11 ERROR] Khong the doc du lieu tu cam bien DHT11! Kiem tra day noi.");
        } else {
            Serial.print("[Time: ");
            Serial.print(currentMillis);
            Serial.print(" ms] DHT11 -> Nhiet do: ");
            Serial.print(temperature, 1);
            Serial.print(" *C | Do am: ");
            Serial.print(humidity, 1);
            Serial.println(" %");
        }
    }
}

