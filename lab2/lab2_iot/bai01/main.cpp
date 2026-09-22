/**
 * ==============================================================================
 * BÀI 1: HAI LED NHẤP NHÁY ĐỘC LẬP BẰNG MILLIS()
 * ==============================================================================
 * Mục tiêu:
 * - Điều khiển 2 LED nhấp nháy với chu kỳ khác nhau (500 ms và 1200 ms) hoàn toàn
 *   độc lập và đồng thời.
 * - Tuyệt đối KHÔNG dùng hàm delay(), áp dụng nguyên lý Time-Triggered Programming
 *   bằng cách theo dõi thời gian với millis().
 * 
 * Sơ đồ chân (Pinout):
 * - LED 1: GPIO 2 (Led tích hợp trên board hoặc LED ngoài nối trở 220Ω xuống GND)
 * - LED 2: GPIO 4 (LED ngoài nối qua trở 220Ω xuống GND)
 * 
 * Giải thích nguyên lý millis():
 * - `delay(ms)` làm vi điều khiển bị treo cứng (blocking), tiêu tốn hàng triệu chu
 *   kỳ lệnh để "chờ đợi", không thể làm việc khác.
 * - `millis()` trả về số mili-giây kể từ lúc ESP32 khởi động.
 * - Bằng cách ghi nhớ thời điểm thực hiện lần cuối (`previousMillis`) và so sánh
 *   với thời điểm hiện tại (`currentMillis`), ta chỉ đảo trạng thái LED khi đã trôi
 *   qua đủ khoảng thời gian quy định (`interval`), trong khi hàm loop() vẫn được chạy
 *   liên tục hàng triệu lần mỗi giây để xử lý các tác vụ khác.
 * ==============================================================================
 */

#include <Arduino.h>

// Định nghĩa chân GPIO dưới dạng hằng số để dễ quản lý và thay đổi
constexpr int LED1_PIN = 2; // LED 1 kết nối chân GPIO 2
constexpr int LED2_PIN = 4; // LED 2 kết nối chân GPIO 4

// Định nghĩa chu kỳ đổi trạng thái (nửa chu kỳ sáng/tối) tính bằng mili-giây
constexpr unsigned long INTERVAL_LED1 = 500;  // LED 1 đảo trạng thái mỗi 500 ms
constexpr unsigned long INTERVAL_LED2 = 1200; // LED 2 đảo trạng thái mỗi 1200 ms

// Các biến lưu mốc thời gian lần cuối mỗi LED đổi trạng thái (phải dùng unsigned long)
unsigned long previousMillisLed1 = 0;
unsigned long previousMillisLed2 = 0;

// Biến lưu trạng thái hiện tại của mỗi LED (HIGH hoặc LOW)
bool led1State = false;
bool led2State = false;

void setup() {
    // Khởi tạo cổng Serial để theo dõi hoạt động (115200 baud)
    Serial.begin(115200);
    while (!Serial && millis() < 1000); // Chờ Serial sẵn sàng tối đa 1s

    Serial.println("==============================================");
    Serial.println("Lab 2 - Bai 1: Hai LED nhap nhay doc lap");
    Serial.println("LED 1 (GPIO 2): chu ky toggle 500 ms");
    Serial.println("LED 2 (GPIO 4): chu ky toggle 1200 ms");
    Serial.println("==============================================");

    // Thiết lập các chân GPIO là OUTPUT
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);

    // Xuất trạng thái ban đầu (cả 2 LED đều tắt)
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
}

void loop() {
    // Lấy thời gian hiện tại ở đầu mỗi vòng lặp loop()
    unsigned long currentMillis = millis();

    // --------------------------------------------------------------------------
    // Task 1: Xử lý nhấp nháy LED 1
    // Kiểm tra an toàn tràn số (overflow-safe): currentMillis - previousMillis >= interval
    // --------------------------------------------------------------------------
    if (currentMillis - previousMillisLed1 >= INTERVAL_LED1) {
        // Cập nhật mốc thời gian đã xử lý
        previousMillisLed1 = currentMillis;

        // Đảo trạng thái LED 1
        led1State = !led1State;
        digitalWrite(LED1_PIN, led1State ? HIGH : LOW);

        Serial.print("[Time: ");
        Serial.print(currentMillis);
        Serial.print(" ms] LED 1 (GPIO 2) -> ");
        Serial.println(led1State ? "ON" : "OFF");
    }

    // --------------------------------------------------------------------------
    // Task 2: Xử lý nhấp nháy LED 2
    // Hoàn toàn độc lập với Task 1, không bị Task 1 cản trở hay làm trễ
    // --------------------------------------------------------------------------
    if (currentMillis - previousMillisLed2 >= INTERVAL_LED2) {
        // Cập nhật mốc thời gian đã xử lý
        previousMillisLed2 = currentMillis;

        // Đảo trạng thái LED 2
        led2State = !led2State;
        digitalWrite(LED2_PIN, led2State ? HIGH : LOW);

        Serial.print("[Time: ");
        Serial.print(currentMillis);
        Serial.print(" ms] LED 2 (GPIO 4) -> ");
        Serial.println(led2State ? "ON" : "OFF");
    }
}

