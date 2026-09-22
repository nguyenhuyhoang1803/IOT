/**
 * ==============================================================================
 * LAB 1 - HOMEWORK 3: MENU ĐƠN GIẢN CHUYỂN TRANG TRÊN LCD 16x2 (SIMPLE LCD MENU)
 * ==============================================================================
 * Mục tiêu:
 * - Xây dựng hệ thống menu 3 trang trên LCD 16x2 điều hướng bằng 1 nút bấm (Button).
 * - Sử dụng cấu trúc switch-case và biến đếm `page` để quản lý giao diện.
 * - Logic chuyển trang bắt buộc:
 *   + page++;
 *   + if (page > 3) { page = 1; }
 *   + switch(page) để vẽ nội dung tương ứng.
 * 
 * Nội dung các trang (Page):
 * - Page 1:
 *     Dòng 1: "Nhiet do:"
 *     Dòng 2: "xx.x C"
 * - Page 2:
 *     Dòng 1: "Do am:"
 *     Dòng 2: "xx.x %"
 * - Page 3:
 *     Dòng 1: "Trang thai:"
 *     Dòng 2: "OK"
 * 
 * Cơ chế hoạt động:
 * - Nút bấm (GPIO 18, INPUT_PULLUP) có lọc dội (Debounce bằng millis) để đảm bảo
 *   mỗi lần nhấn chỉ chuyển đúng một trang.
 * - Cảm biến DHT22 (GPIO 23) được đọc định kỳ mỗi 2000 ms, KHÔNG đọc liên tục
 *   trong từng vòng loop.
 * - Màn hình LCD chỉ được vẽ lại khi chuyển trang hoặc khi dữ liệu cảm biến thay đổi
 *   để giữ giao diện tĩnh mượt mà, không flicker.
 * 
 * Sơ đồ chân (Pinout):
 * - DHT22 Data : GPIO 23
 * - LCD SDA    : GPIO 21
 * - LCD SCL    : GPIO 22 (Địa chỉ I2C: 0x27)
 * - Button     : GPIO 18 (INPUT_PULLUP, nối xuống GND)
 * ==============================================================================
 */

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ==============================================================================
// CẤU HÌNH GPIO & PHẦN CỨNG
// ==============================================================================
constexpr int DHT_PIN    = 23; // Chân cảm biến DHT22
constexpr int BUTTON_PIN = 18; // Chân nút bấm chuyển trang

#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ==============================================================================
// CẤU HÌNH THỜI GIAN & BIẾN MENU
// ==============================================================================
constexpr unsigned long DHT_INTERVAL   = 2000; // Đọc DHT22 mỗi 2 giây
constexpr unsigned long DEBOUNCE_DELAY = 50;   // Chống rung phím 50 ms

// Biến quản lý trang hiện tại (1 -> 2 -> 3 -> 1...)
int page = 1;

// Biến lưu mốc thời gian
unsigned long previousMillisDht = 0;
unsigned long lastDebounceTime  = 0;

// Biến Debounce cho nút bấm
int lastButtonReading = HIGH;
int stableButtonState = HIGH;

// Dữ liệu cảm biến
float temperature = 0.0;
float humidity    = 0.0;
bool isDataReady  = false;

/**
 * Hàm hiển thị nội dung LCD theo trang tương ứng
 */
void renderPage() {
    lcd.clear();

    switch (page) {
        case 1:
            // --- TRANG 1: NHIỆT ĐỘ ---
            lcd.setCursor(0, 0);
            lcd.print("Nhiet do:");
            lcd.setCursor(0, 1);
            if (isDataReady) {
                lcd.print(temperature, 1);
                lcd.print(" C");
            } else {
                lcd.print("Dang doc...");
            }
            break;

        case 2:
            // --- TRANG 2: ĐỘ ẨM ---
            lcd.setCursor(0, 0);
            lcd.print("Do am:");
            lcd.setCursor(0, 1);
            if (isDataReady) {
                lcd.print(humidity, 1);
                lcd.print(" %");
            } else {
                lcd.print("Dang doc...");
            }
            break;

        case 3:
            // --- TRANG 3: TRẠNG THÁI HỆ THỐNG ---
            lcd.setCursor(0, 0);
            lcd.print("Trang thai:");
            lcd.setCursor(0, 1);
            lcd.print("OK");
            break;

        default:
            page = 1;
            renderPage();
            break;
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 1000);

    Serial.println("==================================================");
    Serial.println("Lab 1 - HW 3: Simple LCD Menu (3 Pages)");
    Serial.println("Button GPIO 18: Nhan de chuyen trang (1 -> 2 -> 3 -> 1)");
    Serial.println("==================================================");

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    lcd.init();
    lcd.backlight();
    dht.begin();

    // Vẽ giao diện Trang 1 lần đầu
    renderPage();
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

        if (!isnan(t) && !isnan(h)) {
            temperature = t;
            humidity = h;
            isDataReady = true;

            // Nếu đang ở Page 1 hoặc Page 2 thì cập nhật số liệu mới lên màn hình
            if (page == 1 || page == 2) {
                renderPage();
            }

            Serial.print("[Sensor Update] Temp: ");
            Serial.print(temperature, 1);
            Serial.print(" C | Hum: ");
            Serial.print(humidity, 1);
            Serial.print(" % | Page: ");
            Serial.println(page);
        } else {
            Serial.println("[DHT22 ERROR] Loi doc cam bien!");
        }
    }

    // --------------------------------------------------------------------------
    // Task 2: Đọc nút bấm chuyển trang có Debounce bằng millis
    // --------------------------------------------------------------------------
    int reading = digitalRead(BUTTON_PIN);
    if (reading != lastButtonReading) {
        lastDebounceTime = currentMillis;
    }

    if ((currentMillis - lastDebounceTime) >= DEBOUNCE_DELAY) {
        if (reading != stableButtonState) {
            stableButtonState = reading;

            // Khi phát hiện nút vừa được nhấn (cạnh xuống LOW)
            if (stableButtonState == LOW) {
                // Tăng số trang và quay vòng tuần tự 1 -> 2 -> 3 -> 1
                page++;
                if (page > 3) {
                    page = 1;
                }

                Serial.print("[Button Pressed] Chuyen sang Page ");
                Serial.println(page);

                // Cập nhật giao diện trang mới
                renderPage();
            }
        }
    }
    lastButtonReading = reading;
}

