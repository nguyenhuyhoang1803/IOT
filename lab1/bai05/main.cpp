/**
 * ==============================================================================
 * LAB 1 - BÀI 5: HIỂN THỊ THỜI GIAN UPTIME TRÊN LCD 16x2 I2C
 * ==============================================================================
 * Mục tiêu:
 * - Giao tiếp với màn hình LCD 16x2 qua giao thức I2C (SDA: GPIO 21, SCL: GPIO 22).
 * - Sử dụng địa chỉ I2C chuẩn 0x27 (tương thích Wokwi và module thực tế).
 * - Hiển thị thời gian hệ thống hoạt động (Uptime) theo giây được tính từ millis() / 1000.
 * 
 * Quy cách hiển thị:
 * - Dòng 1: "Thoi gian chay:"
 * - Dòng 2: "xxx giay" (ví dụ "5 giay", "10 giay", "100 giay")
 * 
 * Chống giật (Flicker) & Xử lý ký tự dư:
 * - TUYỆT ĐỐI KHÔNG dùng delay(1000) để tính thời gian.
 * - Chỉ ghi lại nội dung ra LCD khi số giây thực sự thay đổi (currentSeconds != lastSeconds).
 * - Chèn thêm các ký tự khoảng trắng đệm ở cuối dòng 2 để xóa sạch các ký tự dư thừa
 *   khi chuyển từ số có nhiều chữ số về số có ít chữ số (ví dụ: "100 giay" về "9 giay").
 * 
 * Sơ đồ chân (Pinout):
 * - LCD SDA : GPIO 21
 * - LCD SCL : GPIO 22
 * - LCD VCC : 5V (hoặc 3.3V)
 * - LCD GND : GND
 * ==============================================================================
 */

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ==============================================================================
// CẤU HÌNH LCD I2C
// ==============================================================================
// Địa chỉ I2C: 0x27, 16 cột x 2 dòng
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Biến lưu số giây ở lần cập nhật trước đó
unsigned long lastSeconds = 0xFFFFFFFF; // Khởi tạo giá trị khác biệt để vẽ ngay lần đầu

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 1000);

    Serial.println("==================================================");
    Serial.println("Lab 1 - Bai 5: Hien thi Uptime tren LCD 16x2 I2C");
    Serial.println("I2C Pins: SDA = GPIO 21, SCL = GPIO 22 (Dia chi: 0x27)");
    Serial.println("==================================================");

    // Khởi tạo màn hình LCD
    lcd.init();
    lcd.backlight();

    // In tiêu đề cố định ở Dòng 1 một lần duy nhất trong setup()
    lcd.setCursor(0, 0);
    lcd.print("Thoi gian chay:");
}

void loop() {
    // Tính toán số giây hoạt động từ millis()
    unsigned long currentSeconds = millis() / 1000;

    // Chỉ cập nhật LCD khi số giây thay đổi để loại bỏ hoàn toàn hiện tượng nhấp nháy (flicker)
    if (currentSeconds != lastSeconds) {
        lastSeconds = currentSeconds;

        // Đưa con trỏ về đầu Dòng 2
        lcd.setCursor(0, 1);

        // In số giây và kèm khoảng trắng đệm phía sau để xóa sạch các ký tự cũ dư thừa
        lcd.print(currentSeconds);
        lcd.print(" giay        ");

        // In đồng thời lên Serial Monitor để quan sát
        Serial.print("[Uptime] ");
        Serial.print(currentSeconds);
        Serial.println(" giay");
    }
}

