#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// Initialize GPS and GSM modules
SoftwareSerial gpsSerial(3, 4); // RX, TX for GPS module
SoftwareSerial gsmSerial(10, 11); // RX, TX for GSM module
TinyGPSPlus gps;

// OLED display settings
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels
#define OLED_RESET -1     // Reset pin (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Geofence boundaries (latitude and longitude)
float minLat = 16.8580;
float maxLat = 16.8595;
float minLon = 81.7682;
float maxLon = 81.7695;

// GSM-related variables
String phoneNumber = "+1234567890";  // Replace with the recipient's phone number
bool lastGeofenceStatus = false;     // Tracks the last geofence state (inside/outside)

// Function to send SMS using GSM module
void sendSMS(String message) {
  gsmSerial.println("AT"); // Check GSM module connection
  delay(1000);
  gsmSerial.println("AT+CMGF=1"); // Set SMS text mode
  delay(1000);
  gsmSerial.println("AT+CMGS=\"" + phoneNumber + "\""); // Send SMS command
  delay(1000);
  gsmSerial.print(message); // SMS content
  delay(1000);
  gsmSerial.write(26); // End SMS with CTRL+Z
  delay(1000);
}

// Setup function
void setup() {
  Serial.begin(9600);     // Serial monitor
  gpsSerial.begin(9600);  // GPS module
  gsmSerial.begin(9600);  // GSM module

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("GPS + GSM Geofencing");
  display.display();
  delay(2000);

  // GSM initialization
  gsmSerial.println("AT"); // Check GSM connection
  delay(1000);
  gsmSerial.println("AT+CMGF=1"); // Set SMS text mode
  delay(1000);
  Serial.println("GPS + GSM Geofencing System Started");
  display.clearDisplay();
}

// Function to check if inside the geofence
bool isInGeofence(float latitude, float longitude) {
  return (latitude >= minLat && latitude <= maxLat && longitude >= minLon && longitude <= maxLon);
}

// Main loop
void loop() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();

        // Print location to Serial Monitor
        Serial.print("Latitude: "); Serial.println(latitude, 6);
        Serial.print("Longitude: "); Serial.println(longitude, 6);

        // Display location on OLED
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Latitude: ");
        display.println(latitude, 6);
        display.print("Longitude: ");
        display.println(longitude, 6);

        // Check geofence status
        bool currentGeofenceStatus = isInGeofence(latitude, longitude);
        if (currentGeofenceStatus) {
          Serial.println("Inside Geofence");
          display.println("Status: Inside Geofence");

          // Send SMS if the status changed
          if (lastGeofenceStatus != currentGeofenceStatus) {
            sendSMS("Alert: Device is now inside the geofence.");
          }
        } else {
          Serial.println("Outside Geofence");
          display.println("Status: Outside Geofence");

          // Send SMS if the status changed
          if (lastGeofenceStatus != currentGeofenceStatus) {
            sendSMS("Alert: Device is now outside the geofence.");
          }
        }

        // Update geofence status
        lastGeofenceStatus = currentGeofenceStatus;

        // Update OLED display
        display.display();
        Serial.println();
      } else {
        // Handle invalid GPS location
        Serial.println("Waiting for valid GPS fix...");
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Waiting for GPS fix...");
        display.display();
      }
    }
  }
}
