#include <HX711.h>
#include <SoftwareSerial.h>

// Pin configuration
#define DOUT  3  // HX711 data output pin
#define CLK  2   // HX711 clock pin
#define GSM_TX 7 // TX pin of GSM module
#define GSM_RX 8 // RX pin of GSM module
#define LOW_GAS_THRESHOLD 2000  // Set threshold for low gas weight in grams

HX711 scale;
SoftwareSerial gsm(GSM_TX, GSM_RX);

// Variables
float weight = 0;

void setup() {
  Serial.begin(9600);
  gsm.begin(9600);

  // Initialize HX711
  scale.begin(DOUT, CLK);
  scale.set_scale();      // Set the scale calibration factor
  scale.tare();           // Reset the scale to 0

  // GSM Module initialization
  sendSMS("System initialized. Gas monitoring started.");
  Serial.println("System Initialized.");
}

void loop() {
  // Read weight from the load cell
  weight = scale.get_units(10); // Get the weight in grams

  // Display weight
  Serial.print("Gas Weight: ");
  Serial.print(weight);
  Serial.println(" g");

  // Check if the weight is below the threshold
  if (weight < LOW_GAS_THRESHOLD) {
    Serial.println("Low gas detected! Sending SMS for refill...");
    sendSMS("Low gas alert: Please book a refill.");
    delay(10000);  // Wait for 10 seconds before sending another alert
  }
  
  delay(1000); // Refresh every second
}

// Function to send SMS using GSM module
void sendSMS(String message) {
  gsm.println("AT+CMGF=1");    // Set GSM module to text mode
  delay(1000);

  gsm.println("AT+CMGS=\"+91XXXXXXXXXX\"");  // Replace XXXXXXXXXX with gas agency number
  delay(1000);

  gsm.println(message);  // Message text
  delay(1000);

  gsm.write(26);  // ASCII code for Ctrl+Z (end of message)
  delay(1000);

  Serial.println("SMS sent.");
}

