#include <SoftwareSerial.h>

// Thresholds for fall detection
const float freeFallThreshold = 0.2;  // g-force indicating near zero gravity (adjust as needed)
const float impactThreshold = 2.0;    // g-force indicating strong impact (adjust as needed)

// Pins for GSM module
#define RX 10
#define TX 11

SoftwareSerial gsm(RX, TX);  // RX and TX for GSM module

void setup() {
  Serial.begin(115200);      // For debugging
  gsm.begin(9600);           // Communication with GSM module

  // Wait for GSM module to initialize
  Serial.println("Initializing GSM module...");
  delay(2000);

  // Configure GSM module
  sendGSMCommand("AT");                // Basic AT command
  sendGSMCommand("AT+CMGF=1");         // Set SMS mode to text
  sendGSMCommand("AT+CNMI=2,2,0,0,0"); // Configure to show SMS instantly

  Serial.println("GSM module ready.");
}

void loop() {
  // Read analog values from ADXL335
  int xRaw = analogRead(A0);  // Replace A0 with the pin connected to X output of ADXL335
  int yRaw = analogRead(A1);  // Replace A1 with the pin connected to Y output of ADXL335
  int zRaw = analogRead(A2);  // Replace A2 with the pin connected to Z output of ADXL335

  // Convert raw analog values to acceleration in g's
  float accelX = (xRaw - 512) * (3.3 / 1024) / 0.3;  // Adjust if using 5V or other reference voltage
  float accelY = (yRaw - 512) * (3.3 / 1024) / 0.3;
  float accelZ = (zRaw - 512) * (3.3 / 1024) / 0.3;

  // Calculate the total acceleration magnitude
  float totalAccel = sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ);

  // Print the accelerometer values for debugging
  Serial.print("Total Acceleration: ");
  Serial.print(totalAccel);
  Serial.println(" g");

  // Check for possible fall
  static bool inFreeFall = false;  // Track whether we're currently in a free-fall state

  if (totalAccel < freeFallThreshold) {
    // If below the free-fall threshold, set the inFreeFall flag
    inFreeFall = true;
  }

  if (inFreeFall && totalAccel > impactThreshold) {
    // If we've previously detected free-fall and now detect an impact, consider it a fall
    Serial.println("Fall detected!");
    sendSMS("+1234567890", "Fall detected! Please check immediately."); // Replace with your phone number

    // Reset the flag
    inFreeFall = false;
  }

  delay(500);  // Delay to make output more readable
}

// Function to send an SMS
void sendSMS(const char* phoneNumber, const char* message) {
  gsm.print("AT+CMGS=\"");
  gsm.print(phoneNumber);
  gsm.println("\"");
  delay(100);
  gsm.println(message);
  delay(100);
  gsm.write(26); // CTRL+Z to send the message
  delay(5000);   // Wait for the message to send
}

// Function to send a command to the GSM module
void sendGSMCommand(const char* command) {
  gsm.println(command);
  delay(1000); // Wait for the GSM module to respond
}
