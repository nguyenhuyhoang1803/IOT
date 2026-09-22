/**
 * ==============================================================================
 * BÀI 2: CHỐNG DỘI NÚT NHẤN (BUTTON DEBOUNCE) BẰNG MILLIS()
 * ==============================================================================
 * Mục tiêu:
 * - Khử hiện tượng dội cơ khí (contact bounce) của nút nhấn bằng thuật toán phần
 *   mềm sử dụng millis(), tuyệt đối KHÔNG dùng delay(50).
 * - Mỗi lần nhấn nút hợp lệ sẽ đảo trạng thái (toggle) của 2 LED.
 * - Chỉ toggle 1 lần duy nhất cho mỗi lần nhấn, giữ nút không bị toggle liên tục.
 * 
 * Sơ đồ chân (Pinout):
 * - Button: GPIO 18 (sử dụng chế độ INPUT_PULLUP, 1 chân nối GPIO 18, chân kia nối GND)
 * - LED 1: GPIO 4 (nối qua điện trở 220Ω xuống GND)
 * - LED 2: GPIO 5 (nối qua điện trở 220Ω xuống GND)
 * 
 * Giải thích INPUT_PULLUP:
 * - Khi bật điện trở kéo lên nội (internal pull-up):
 *   + Khi thả nút (Released): Chân GPIO được kéo lên 3.3V -> digitalRead() = HIGH (1).
 *   + Khi nhấn nút (Pressed) : Nút nối chân GPIO về GND -> digitalRead() = LOW (0).
 * 
 * Nguyên lý phân biệt 3 trạng thái trong debounce:
 * 1. `reading`: Giá trị đọc tức thời tại thời điểm hiện tại từ digitalRead().
 * 2. `lastReading`: Giá trị đọc tức thời ở vòng lặp loop() liền trước để nhận biết
 *    khi nào chân có sự thay đổi (nhiễu hoặc bắt đầu chuyển mức).
 * 3. `buttonState`: Trạng thái ổn định sau khi đã lọc dội xong (tín hiệu giữ nguyên
 *    không đổi liên tục qua khoảng thời gian DEBOUNCE_DELAY).
 * ==============================================================================
 */

#include <Arduino.h>

// Định nghĩa chân GPIO
constexpr int BUTTON_PIN = 18; // Chân đọc nút nhấn (chế độ INPUT_PULLUP)
constexpr int LED1_PIN = 4;    // LED 1
constexpr int LED2_PIN = 5;    // LED 2

// Thời gian trễ lọc dội (40 - 50 ms)
constexpr unsigned long DEBOUNCE_DELAY = 50;

// Trạng thái đọc tức thời ở vòng lặp trước (ban đầu kéo lên HIGH)
int lastReading = HIGH;

// Trạng thái nút nhấn đã được xác nhận ổn định
int buttonState = HIGH;

// Mốc thời gian ghi nhận lần đổi trạng thái đọc tức thời gần nhất
unsigned long lastDebounceTime = 0;

// Trạng thái bật/tắt hiện tại của 2 LED
bool ledState = false;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 1000);

    Serial.println("==============================================");
    Serial.println("Lab 2 - Bai 2: Button Debounce bang millis()");
    Serial.println("Button: GPIO 18 (INPUT_PULLUP)");
    Serial.println("LEDs: GPIO 4 & GPIO 5");
    Serial.println("Quy uoc: Nhan = LOW, Tha = HIGH");
    Serial.println("==============================================");

    // Cấu hình nút nhấn với điện trở kéo lên nội bộ
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Cấu hình các chân LED là OUTPUT
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);

    // Ban đầu tắt 2 LED
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
}

void loop() {
    unsigned long currentMillis = millis();

    // 1. Đọc giá trị tức thời của chân nút nhấn
    int reading = digitalRead(BUTTON_PIN);

    // 2. Nếu giá trị đọc thay đổi so với lần đọc trước (có thể là cạnh sườn hoặc nhiễu nảy)
    if (reading != lastReading) {
        // Reset bộ đếm thời gian debounce
        lastDebounceTime = currentMillis;
    }

    // 3. Nếu tín hiệu đã giữ nguyên trạng thái trong khoảng thời gian >= DEBOUNCE_DELAY
    if ((currentMillis - lastDebounceTime) >= DEBOUNCE_DELAY) {
        // Tín hiệu đã ổn định, kiểm tra xem trạng thái ổn định mới có khác trạng thái cũ không
        if (reading != buttonState) {
            buttonState = reading; // Cập nhật trạng thái ổn định

            // 4. Phát hiện sự kiện cạnh xuống (Falling Edge: từ HIGH chuyển sang LOW)
            // Tương ứng với hành động người dùng NHẤN nút
            if (buttonState == LOW) {
                // Đảo trạng thái 2 LED
                ledState = !ledState;
                digitalWrite(LED1_PIN, ledState ? HIGH : LOW);
                digitalWrite(LED2_PIN, ledState ? HIGH : LOW);

                Serial.print("[Time: ");
                Serial.print(currentMillis);
                Serial.print(" ms] Valid Button Press Detected! LEDs -> ");
                Serial.println(ledState ? "ON" : "OFF");
            } else {
                // Hành động người dùng THẢ nút (LOW -> HIGH)
                Serial.print("[Time: ");
                Serial.print(currentMillis);
                Serial.println(" ms] Button Released.");
            }
        }
    }

    // 5. Lưu giá trị đọc tức thời để so sánh ở vòng lặp loop() tiếp theo
    lastReading = reading;
}

