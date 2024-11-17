#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>  
#include <Adafruit_Sensor.h>
#include "DHT.h"

// OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// OLED reset pin
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT sensor setup
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11  // Change to DHT22 or DHT21 if using a different sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize DHT sensor
  dht.begin();

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for most displays
    Serial.println("SSD1306 allocation failed");
    for (;;); // Loop forever if OLED initialization fails
  }
  
  // Clear the display buffer
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void loop() {
  delay(2000); // Wait 2 seconds between sensor readings

  // Read temperature and humidity
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Check if readings are valid
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return; // Skip this loop iteration if sensor fails
  }

  // Print readings to Serial Monitor for debugging
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  // Clear display
  display.clearDisplay();

  // Display temperature
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(t);
  display.print((char)167); // Degree symbol
  display.print("C");

  // Display humidity
  display.setCursor(0, 16);
  display.print("Humidity: ");
  display.print(h);
  display.print(" %");

  // Update the display with new data
  display.display();
}
