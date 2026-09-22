/**
 * ==============================================================================
 * BÀI 5: BA TÁC VỤ SONG SONG TRONG LOOP() (TIME-TRIGGERED & EVENT-DRIVEN)
 * ==============================================================================
 * Mục tiêu:
 * - Chạy đồng thời 3 tác vụ logic trong cùng một hàm loop() mà không sử dụng delay():
 *   + Task 1 (Time-triggered): LED 1 đảo trạng thái mỗi 300 ms.
 *   + Task 2 (Event-driven): Đọc nút bấm có debounce, khi nhấn thì toggle LED 2.
 *   + Task 3 (Time-triggered & Profiling): Đếm số vòng lặp loop() thực thi được trong
 *     mỗi 1 giây và gửi lên Serial: `Loop count per second: ...`, sau đó reset bộ đếm.
 * - Thể hiện rõ tư duy lập trình nhúng non-blocking: Do không có hàm delay nào chặn,
 *   ESP32 có thể lặp hàm loop() hàng chục nghìn đến hàng trăm nghìn lần mỗi giây,
 *   cho phép phản hồi sự kiện gần như tức thời.
 * 
 * Khai báo GPIO:
 * - Vì đề bài không chỉ định cố định chân GPIO cho bài này, các chân được định nghĩa
 *   rõ ràng bằng hằng số ở đầu file để người dùng dễ dàng cấu hình lại nếu muốn:
 *   + LED 1: GPIO 2  (Mặc định LED tích hợp trên board)
 *   + LED 2: GPIO 4  (LED ngoài qua điện trở 220Ω xuống GND)
 *   + Button: GPIO 18 (Nút nhấn chế độ INPUT_PULLUP)
 * ==============================================================================
 */

#include <Arduino.h>

// ==============================================================================
// CẤU HÌNH GPIO (Tùy chỉnh linh hoạt theo phần cứng thực tế)
// ==============================================================================
constexpr int LED1_PIN = 2;   // Chân điều khiển LED 1 (Nhấp nháy 300 ms)
constexpr int LED2_PIN = 4;   // Chân điều khiển LED 2 (Đảo trạng thái khi bấm nút)
constexpr int BUTTON_PIN = 18; // Chân đọc nút nhấn (Chế độ INPUT_PULLUP)

// ==============================================================================
// CẤU HÌNH THỜI GIAN (ms)
// ==============================================================================
constexpr unsigned long INTERVAL_TASK1 = 300;   // LED 1 toggle mỗi 300 ms
constexpr unsigned long INTERVAL_TASK3 = 1000;  // Task 3 báo cáo loop count mỗi 1000 ms (1s)
constexpr unsigned long DEBOUNCE_DELAY = 50;    // Thời gian lọc dội nút nhấn 50 ms

// ==============================================================================
// CÁC BIẾN QUẢN LÝ THỜI GIAN & TRẠNG THÁI
// ==============================================================================
// Biến lưu mốc thời gian lần cuối của từng task
unsigned long previousMillisTask1 = 0;
unsigned long previousMillisTask3 = 0;
unsigned long lastDebounceTime = 0;

// Trạng thái các thiết bị
bool led1State = false;
bool led2State = false;

// Trạng thái nút bấm
int lastReading = HIGH;
int buttonState = HIGH;

// Biến đếm số chu kỳ vòng lặp loop() trong 1 giây
unsigned long loopCounter = 0;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 1000);

    Serial.println("==========================================================");
    Serial.println("Lab 2 - Bai 5: Ba tac vu song song (Time-triggered & Event-driven)");
    Serial.println("Task 1: LED 1 (GPIO 2) nhap nhay moi 300 ms");
    Serial.println("Task 2: Button (GPIO 18) debounce dao trang thai LED 2 (GPIO 4)");
    Serial.println("Task 3: Do tan so loop() - bao cao so vong lap moi giay");
    Serial.println("==========================================================");

    // Cấu hình chân vào/ra
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
}

void loop() {
    // Luôn tăng biến đếm số lần chạy hàm loop()
    loopCounter++;

    unsigned long currentMillis = millis();

    // --------------------------------------------------------------------------
    // TASK 1 (Time-triggered): Nhấp nháy LED 1 mỗi 300 ms
    // --------------------------------------------------------------------------
    if (currentMillis - previousMillisTask1 >= INTERVAL_TASK1) {
        previousMillisTask1 = currentMillis;

        led1State = !led1State;
        digitalWrite(LED1_PIN, led1State ? HIGH : LOW);
    }

    // --------------------------------------------------------------------------
    // TASK 2 (Event-driven): Xử lý nút nhấn có debounce, khi nhấn thì toggle LED 2
    // --------------------------------------------------------------------------
    int reading = digitalRead(BUTTON_PIN);
    if (reading != lastReading) {
        lastDebounceTime = currentMillis;
    }

    if ((currentMillis - lastDebounceTime) >= DEBOUNCE_DELAY) {
        if (reading != buttonState) {
            buttonState = reading;
            if (buttonState == LOW) { // Nhấn nút (Falling Edge)
                led2State = !led2State;
                digitalWrite(LED2_PIN, led2State ? HIGH : LOW);

                Serial.print("[Time: ");
                Serial.print(currentMillis);
                Serial.print(" ms] [Task 2] Button Pressed -> LED 2 toggled to ");
                Serial.println(led2State ? "ON" : "OFF");
            }
        }
    }
    lastReading = reading;

    // --------------------------------------------------------------------------
    // TASK 3 (Time-triggered): Đo số vòng loop() thực thi trong 1 giây
    // --------------------------------------------------------------------------
    if (currentMillis - previousMillisTask3 >= INTERVAL_TASK3) {
        previousMillisTask3 = currentMillis;

        Serial.print("[Time: ");
        Serial.print(currentMillis);
        Serial.print(" ms] [Task 3] Loop count per second: ");
        Serial.println(loopCounter);

        // Đặt lại biến đếm cho chu kỳ 1 giây tiếp theo
        loopCounter = 0;
    }
}

