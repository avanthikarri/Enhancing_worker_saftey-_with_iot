#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Reset pin is not used in I2C mode
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

TinyGPSPlus gps;
SoftwareSerial gpsSerial(4, 3); // RX, TX (connect to TX, RX of GPS module)

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);

  // Initialize OLED
  if (!display.begin(SSD1306_I2C_ADDRESS, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();

  Serial.println(F("Waiting for GPS signal..."));
}

void loop() {
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);

    if (gps.time.isUpdated()) { // Check if GPS time is updated
      int hour = gps.time.hour();
      int minute = gps.time.minute();
      int second = gps.time.second();

      // Display time
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(10, 25); // Adjust position
      display.printf("%02d:%02d:%02d", hour, minute, second);
      display.display();
    }
  }
}
