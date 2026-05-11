 /* Library: RemoteUpload
 * Description: Effortless Local and Remote (Google Drive) OTA updates for ESP32 and ESP8266
 * @author Idlan Zafran Mohd Zaidie
 * Date: 2026
 * Note: You are free to use this code, but please leave this header intact.
 * @license MIT
 */

#include <Arduino.h>
#include <WiFi.h> // Use <ESP8266WiFi.h> if using an ESP8266
#include "RemoteUpload.h"

// ==========================================
// USER CONFIGURATION: Replace with your data
// ==========================================
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* otaPassword = "YOUR_OTA_PASSWORD"; // Password for local IDE updates
const char* myDriveLink = "https://drive.google.com/file/d/YOUR_FILE_ID/view?usp=sharing";

// 1. Define the pin your button is connected to
const int BUTTON_PIN = 4; // GPIO 4

void setup() {
  Serial.begin(115200);
  
  // 2. Configure the pin to use the ESP's internal pull-up resistor
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  // Initialize both OTA methods
  ota.begin("my-smart-device", otaPassword, myDriveLink);
  
  Serial.println("\n--- DEVICE RUNNING: VERSION 1.0 ---");
  Serial.println("Press the physical button to fetch new code from Drive!");
}

void loop() {
  // Keeps local IDE updates working
  ota.handle();       
  
  // Keeps Serial Monitor updates working (you can leave this or delete it)
  ota.handleSerial("update"); 
  
  // 3. Check if the physical button is pressed (LOW = Pressed)
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("\n[Hardware] Button pressed! Initiating Drive download...");
    
    // A tiny delay to "debounce" the button (prevents electrical static 
    // from triggering it twice in a row)
    delay(50); 
    
    // Trigger the actual update!
    ota.checkUpdate(); 
  }
  
  // The rest of your normal project code goes here
}