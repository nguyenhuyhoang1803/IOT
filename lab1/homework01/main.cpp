/**
 * ==============================================================================
 * LAB 1 - HOMEWORK 1: TRẠM THỜI TIẾT MINI (MINI WEATHER STATION / THERMOSTAT)
 * ==============================================================================
 * Mục tiêu:
 * - Tích hợp toàn diện: DHT22 + LCD 16x2 I2C + Button + LED.
 * - Trạng thái bình thường:
 *   + Dòng 1: "Temp: xx.x C"
 *   + Dòng 2: "Hum:  xx.x %"
 * - Khi nhấn Button (có lọc dội Debounce):
 *   + Chuyển đổi hiển thị nhiệt độ từ Celsius sang Fahrenheit: F = (C * 1.8) + 32.
 *   + Dòng 1: "Temp: xx.x F" trong ĐÚNG 5 GIÂY.
 *   + Sau 5 giây, tự động quay trở về hiển thị độ C (°C).
 * - QUAN TRỌNG: TUYỆT ĐỐI KHÔNG dùng delay(5000), phải dùng millis().
 * - Trong suốt 5 giây hiển thị độ F, cảm biến DHT22 vẫn đọc định kỳ và LED vẫn phản hồi tức thì.
 * - Điều kiện điều khiển LED độ ẩm:
 *   + Độ ẩm > 80.0%  -> LED BẬT (HIGH).
 *   + Độ ẩm <= 80.0% -> LED TẮT (LOW).
 * 
 * Sơ đồ chân (Pinout):
 * - DHT22 Data : GPIO 23
 * - LCD SDA    : GPIO 21
 * - LCD SCL    : GPIO 22 (Địa chỉ I2C: 0x27)
 * - Button     : GPIO 18 (INPUT_PULLUP, nối xuống GND)
 * - Humidity LED: GPIO 2 (nối qua trở 220Ω xuống GND)
 * ==============================================================================
 */

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ==============================================================================
// CẤU HÌNH GPIO & PHẦN CỨNG
// ==============================================================================
constexpr int DHT_PIN      = 23; // Chân cảm biến DHT22
constexpr int BUTTON_PIN   = 18; // Chân nút bấm chuyển đổi độ C/F
constexpr int HUM_LED_PIN  = 2;  // Chân LED cảnh báo độ ẩm cao (>80%)

#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ==============================================================================
// CẤU HÌNH THỜI GIAN & NGƯỠNG
// ==============================================================================
constexpr unsigned long DHT_INTERVAL      = 2000; // Đọc DHT22 mỗi 2 giây
constexpr unsigned long FAHRENHEIT_TIME   = 5000; // Thời gian hiển thị độ F: 5 giây
constexpr unsigned long DEBOUNCE_DELAY    = 50;   // Chống rung nút bấm: 50 ms
constexpr float         HUMIDITY_THRESHOLD = 80.0; // Ngưỡng độ ẩm cảnh báo (> 80%)

// ==============================================================================
// BIẾN QUẢN LÝ THỜI GIAN & TRẠNG THÁI
// ==============================================================================
unsigned long previousMillisDht = 0;
unsigned long fahrenheitStartTime = 0;
bool isFahrenheitMode = false;

// Biến cho nút bấm debounce
int lastButtonReading = HIGH;
int stableButtonState = HIGH;
unsigned long lastDebounceTime = 0;

// Dữ liệu cảm biến lưu trữ
float currentTempC = 0.0;
float currentHum   = 0.0;
bool isSensorDataValid = false;

/**
 * Hàm cập nhật giao diện LCD 16x2
 */
void updateLcd() {
    if (!isSensorDataValid) {
        lcd.setCursor(0, 0);
        lcd.print("Dang doc DHT22..");
        lcd.setCursor(0, 1);
        lcd.print("Xin cho trong giay");
        return;
    }

    // --- DÒNG 1: NHIỆT ĐỘ (CELSIUS HOẶC FAHRENHEIT) ---
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    if (isFahrenheitMode) {
        float tempF = (currentTempC * 1.8) + 32.0;
        lcd.print(tempF, 1);
        lcd.print(" F   "); // Thêm khoảng trắng đệm xóa ký tự cũ
    } else {
        lcd.print(currentTempC, 1);
        lcd.print(" C   ");
    }

    // --- DÒNG 2: ĐỘ ẨM ---
    lcd.setCursor(0, 1);
    lcd.print("Hum:  ");
    lcd.print(currentHum, 1);
    lcd.print(" %   ");
}

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 1000);

    Serial.println("==================================================");
    Serial.println("Lab 1 - HW 1: Mini Weather Station (DHT22 + LCD)");
    Serial.println("Button: Chuyen sang do F trong dung 5 giay");
    Serial.println("LED Pin 2: Sang khi do am > 80%");
    Serial.println("==================================================");

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(HUM_LED_PIN, OUTPUT);
    digitalWrite(HUM_LED_PIN, LOW);

    // Khởi động LCD và DHT22
    lcd.init();
    lcd.backlight();
    dht.begin();

    updateLcd();
}

void loop() {
    unsigned long currentMillis = millis();

    // --------------------------------------------------------------------------
    // Task 1: Đọc cảm biến DHT22 định kỳ mỗi 2000 ms
    // --------------------------------------------------------------------------
    if (currentMillis - previousMillisDht >= DHT_INTERVAL) {
        previousMillisDht = currentMillis;

        float t = dht.readTemperature();
        float h = dht.readHumidity();

        if (isnan(t) || isnan(h)) {
            Serial.println("[DHT22 ERROR] Khong doc duoc cam bien!");
            isSensorDataValid = false;
        } else {
            currentTempC = t;
            currentHum = h;
            isSensorDataValid = true;

            // Điều khiển LED cảnh báo độ ẩm
            if (currentHum > HUMIDITY_THRESHOLD) {
                digitalWrite(HUM_LED_PIN, HIGH);
            } else {
                digitalWrite(HUM_LED_PIN, LOW);
            }

            // In thông tin chẩn đoán ra Serial
            Serial.print("[Sensor] Temp: ");
            Serial.print(currentTempC, 1);
            Serial.print(" C | Hum: ");
            Serial.print(currentHum, 1);
            Serial.print(" % | Mode: ");
            Serial.println(isFahrenheitMode ? "FAHRENHEIT (5s)" : "CELSIUS");

            // Làm mới LCD khi có dữ liệu mới
            updateLcd();
        }
    }

    // --------------------------------------------------------------------------
    // Task 2: Đọc nút bấm chuyển đổi đơn vị độ F (Debounce bằng millis)
    // --------------------------------------------------------------------------
    int reading = digitalRead(BUTTON_PIN);
    if (reading != lastButtonReading) {
        lastDebounceTime = currentMillis;
    }

    if ((currentMillis - lastDebounceTime) >= DEBOUNCE_DELAY) {
        if (reading != stableButtonState) {
            stableButtonState = reading;

            // Nhận diện nút vừa được nhấn (cạnh xuống LOW)
            if (stableButtonState == LOW) {
                isFahrenheitMode = true;
                fahrenheitStartTime = currentMillis; // Bắt đầu đếm thời gian 5 giây
                Serial.println("[Button] Kich hoat hien thi do F trong 5 giay!");
                updateLcd();
            }
        }
    }
    lastButtonReading = reading;

    // --------------------------------------------------------------------------
    // Task 3: Tự động hoàn trả về độ C sau 5 giây (Non-blocking)
    // --------------------------------------------------------------------------
    if (isFahrenheitMode) {
        if (currentMillis - fahrenheitStartTime >= FAHRENHEIT_TIME) {
            isFahrenheitMode = false;
            Serial.println("[Timer] Het 5 giay, tu dong quay lai do C.");
            updateLcd();
        }
    }
}

