#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <TimeLib.h>  // Time library for time functions

// OLED display settings
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels
#define OLED_RESET     4  // Reset pin (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Gas sensor pin and threshold
int gasSensorPin = A0;
int gasThreshold = 260;

// GSM module setup (using software serial)
SoftwareSerial gsmSerial(7, 8); // RX, TX (connected to GSM module)

bool gasAlertSent = false; // Flag to avoid multiple SMS alerts for the same event

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  
  // Initialize gas sensor pins
  pinMode(gasSensorPin, INPUT);
  pinMode(2, OUTPUT);  // Output pin for gas leakage warning
  pinMode(3, OUTPUT);  // Output pin for safe status

  // Initialize GSM module
  gsmSerial.begin(9600);
  sendATCommand("AT");  // Check GSM module communication
  sendATCommand("AT+CMGF=1");  // Set SMS text mode

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3C for OLED
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Don't proceed, loop forever
  }

  // Set an initial time (for example, from user input)
  setTime(12, 0, 0, 1, 1, 2024);  // Set time to 12:00:00 on 1st Jan 2024

  // Clear display buffer and show initial message
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();
  delay(2000);
}

void sendATCommand(String command) {
  gsmSerial.println(command);
  delay(100);
  while (gsmSerial.available()) {
    Serial.write(gsmSerial.read());
  }
}

void sendSMS(String message) {
  gsmSerial.println("AT+CMGS=\"+91XXXXXXXXXX\"");  // Replace with the recipient's number
  delay(100);
  gsmSerial.print(message);
  delay(100);
  gsmSerial.write(26);  // Send Ctrl+Z to end the message
  delay(5000);  // Wait for the message to be sent
}

void displayTime() {
  // Display the time in the top-right corner
  display.setCursor(80, 0);
  if (hour() < 10) display.print('0');
  display.print(hour());
  display.print(':');
  if (minute() < 10) display.print('0');
  display.print(minute());
  display.print(':');
  if (second() < 10) display.print('0');
  display.println(second());
}

void loop() {
  // Check gas sensor readings
  int gasLevel = analogRead(gasSensorPin);
  display.clearDisplay();
  display.setTextSize(1);

  // Display the time in the corner
  displayTime();

  // Gas level message
  display.setCursor(0, 10);
  display.print("Gas Level: ");
  display.println(gasLevel);

  // Check if gas leakage detected
  if (gasLevel >= gasThreshold) {
    display.setCursor(0, 20);
    display.println("WARNING: GAS LEAKAGE");
    digitalWrite(2, LOW);  // Activate warning outputs
    digitalWrite(3, HIGH);

    // Send SMS alert if not already sent
    if (!gasAlertSent) {
      sendSMS("ALERT: High gas level detected! Take immediate action.");
      gasAlertSent = true;  // Set flag to true after sending SMS
    }
  } else {
    display.setCursor(0, 20);
    display.println("All is Well");
    digitalWrite(2, HIGH);  // Deactivate warning outputs
    digitalWrite(3, LOW);
    gasAlertSent = false;  // Reset flag when gas level returns to normal
  }

  // Update OLED display with gas sensor and time
  display.display();
  delay(500);  // Short delay for smoother display updates
}
