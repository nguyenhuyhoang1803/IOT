/**
 * ==============================================================================
 * LAB 1 - HOMEWORK 2: ĐỒNG HỒ BẤM GIỜ STOPWATCH TRÊN LCD 16x2
 * ==============================================================================
 * Mục tiêu:
 * - Xây dựng hệ thống đồng hồ bấm giờ thể thao chính xác sử dụng máy trạng thái (FSM).
 * - Định dạng hiển thị trên LCD 16x2: "MM:SS:ms" (ví dụ: 00:00:000, 00:05:231, 01:12:054).
 * - Trạng thái ban đầu: "00:00:000" ở trạng thái STOPPED.
 * 
 * Điều khiển bằng 2 nút bấm (Debounce bằng millis):
 * - Button A (GPIO 18):
 *   + Nếu đang STOPPED -> START (Bắt đầu chạy tiếp từ thời gian đã tích lũy).
 *   + Nếu đang RUNNING -> PAUSE (Tạm dừng và bảo lưu thời gian chính xác).
 *   + Nhấn tiếp -> RESUME (Tiếp tục chạy, không bị nhảy thời gian hay reset về 0).
 * - Button B (GPIO 19):
 *   + CHỈ KHI STOPPED  -> RESET về "00:00:000".
 *   + KHI ĐANG RUNNING -> Nút B bị vô hiệu hóa hoàn toàn, KHÔNG được phép reset.
 * 
 * Độ mượt & Hiệu năng:
 * - TUYỆT ĐỐI KHÔNG dùng delay() để tính thời gian.
 * - Sử dụng millis() và cộng dồn thời gian tích lũy (accumulatedTime) chuẩn xác từng mili-giây.
 * - Chu kỳ quét làm mới LCD: 50 ms/lần (vừa đảm bảo mượt mà 20 fps, vừa chống nghẽn I2C).
 * 
 * Sơ đồ chân (Pinout):
 * - Button A (Start/Pause) : GPIO 18 (INPUT_PULLUP)
 * - Button B (Reset)       : GPIO 19 (INPUT_PULLUP)
 * - LCD SDA                : GPIO 21
 * - LCD SCL                : GPIO 22 (Địa chỉ I2C: 0x27)
 * ==============================================================================
 */

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ==============================================================================
// CẤU HÌNH GPIO & PHẦN CỨNG
// ==============================================================================
constexpr int BUTTON_A_PIN = 18; // Nút A: Start / Pause
constexpr int BUTTON_B_PIN = 19; // Nút B: Reset (Chỉ khi đã dừng)

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ==============================================================================
// ĐỊNH NGHĨA TRẠNG THÁI & THỜI GIAN
// ==============================================================================
enum StopwatchState {
    STOPPED,
    RUNNING
};

StopwatchState currentState = STOPPED;

constexpr unsigned long DEBOUNCE_DELAY  = 50; // Chống rung phím 50 ms
constexpr unsigned long LCD_REFRESH_MS  = 50; // Tần số làm mới màn hình 50 ms (20 FPS)

// Biến tính toán thời gian bấm giờ
unsigned long startTime       = 0; // Mốc thời gian bắt đầu chạy của lượt hiện tại
unsigned long accumulatedTime = 0; // Thời gian đã tích lũy từ các lượt chạy trước (ms)
unsigned long lastLcdUpdate   = 0; // Bộ đếm thời gian cập nhật LCD

// Biến Debounce cho Button A
int lastReadingA = HIGH;
int stableStateA = HIGH;
unsigned long lastDebounceTimeA = 0;

// Biến Debounce cho Button B
int lastReadingB = HIGH;
int stableStateB = HIGH;
unsigned long lastDebounceTimeB = 0;

/**
 * Hàm hiển thị thời gian lên LCD theo định dạng chuẩn MM:SS:ms
 */
void displayTime(unsigned long totalMs) {
    unsigned int minutes = (totalMs / 60000) % 60;
    unsigned int seconds = (totalMs / 1000) % 60;
    unsigned int millisec = totalMs % 1000;

    char buffer[17];
    // Định dạng: 2 chữ số phút, 2 chữ số giây, 3 chữ số mili-giây
    snprintf(buffer, sizeof(buffer), "%02u:%02u:%03u", minutes, seconds, millisec);

    lcd.setCursor(3, 1);
    lcd.print(buffer);
}

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 1000);

    Serial.println("==================================================");
    Serial.println("Lab 1 - HW 2: Stopwatch MM:SS:ms tren LCD 16x2");
    Serial.println("Button A (GPIO 18): Start / Pause");
    Serial.println("Button B (GPIO 19): Reset (chi khi STOPPED)");
    Serial.println("==================================================");

    pinMode(BUTTON_A_PIN, INPUT_PULLUP);
    pinMode(BUTTON_B_PIN, INPUT_PULLUP);

    lcd.init();
    lcd.backlight();

    // Dòng 1: Tiêu đề và trạng thái
    lcd.setCursor(0, 0);
    lcd.print("STOPWATCH: STOP ");

    // Dòng 2: Hiển thị 00:00:000 ban đầu
    displayTime(0);
}

void loop() {
    unsigned long currentMillis = millis();

    // --------------------------------------------------------------------------
    // Task 1: Xử lý nút bấm Button A (Start / Pause) có Debounce
    // --------------------------------------------------------------------------
    int readingA = digitalRead(BUTTON_A_PIN);
    if (readingA != lastReadingA) {
        lastDebounceTimeA = currentMillis;
    }

    if ((currentMillis - lastDebounceTimeA) >= DEBOUNCE_DELAY) {
        if (readingA != stableStateA) {
            stableStateA = readingA;

            if (stableStateA == LOW) { // Nhấn nút A
                if (currentState == STOPPED) {
                    // Chuyển từ DỪNG sang CHẠY (Start hoặc Resume)
                    currentState = RUNNING;
                    startTime = currentMillis; // Ghi nhận mốc bắt đầu của chặng này
                    lcd.setCursor(11, 0);
                    lcd.print("RUN ");
                    Serial.println("[Button A] Stopwatch: START / RESUME");
                } else {
                    // Chuyển từ CHẠY sang DỪNG (Pause)
                    currentState = STOPPED;
                    accumulatedTime += (currentMillis - startTime); // Cộng dồn thời gian vừa chạy
                    lcd.setCursor(11, 0);
                    lcd.print("PAUSE");
                    Serial.print("[Button A] Stopwatch: PAUSE tai ");
                    Serial.print(accumulatedTime);
                    Serial.println(" ms");
                }
            }
        }
    }
    lastReadingA = readingA;

    // --------------------------------------------------------------------------
    // Task 2: Xử lý nút bấm Button B (Reset) có Debounce
    // --------------------------------------------------------------------------
    int readingB = digitalRead(BUTTON_B_PIN);
    if (readingB != lastReadingB) {
        lastDebounceTimeB = currentMillis;
    }

    if ((currentMillis - lastDebounceTimeB) >= DEBOUNCE_DELAY) {
        if (readingB != stableStateB) {
            stableStateB = readingB;

            if (stableStateB == LOW) { // Nhấn nút B
                // CHỈ CHO PHÉP RESET KHI ĐANG STOPPED
                if (currentState == STOPPED) {
                    accumulatedTime = 0;
                    lcd.setCursor(11, 0);
                    lcd.print("RESET");
                    displayTime(0);
                    Serial.println("[Button B] Stopwatch: RESET ve 00:00:000");
                } else {
                    Serial.println("[Button B] Tu choi Reset vi Stopwatch dang RUNNING!");
                }
            }
        }
    }
    lastReadingB = readingB;

    // --------------------------------------------------------------------------
    // Task 3: Cập nhật màn hình LCD theo chu kỳ 50 ms
    // --------------------------------------------------------------------------
    if (currentMillis - lastLcdUpdate >= LCD_REFRESH_MS) {
        lastLcdUpdate = currentMillis;

        if (currentState == RUNNING) {
            // Tính tổng thời gian = Thời gian tích lũy trước đó + thời gian của chặng hiện tại
            unsigned long totalElapsed = accumulatedTime + (currentMillis - startTime);
            displayTime(totalElapsed);
        }
    }
}

