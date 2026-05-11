 /* Library: RemoteUpload
 * Description: Effortless Local and Remote (Google Drive) OTA updates for ESP32 and ESP8266
 * @author Idlan Zafran Mohd Zaidie
 * Date: 2026
 * Note: You are free to use this code, but please leave this header intact.
 * @license MIT
 */

#include "RemoteUpload.h"

RemoteUpload::RemoteUpload() {}

void RemoteUpload::begin(const char* hostname, const char* password, const char* driveLink) {
  // --- 1. SET UP LOCAL OTA (PUSH) ---
  ArduinoOTA.setHostname(hostname);

  if (password != nullptr && strlen(password) > 0) {
    ArduinoOTA.setPassword(password);
  }

  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
    Serial.println("Local OTA: Start updating " + type);
  });
  ArduinoOTA.onEnd([]() { Serial.println("\nLocal OTA: End"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Local OTA Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Local OTA Error[%u]: ", error);
  });

  ArduinoOTA.begin();
  
  // --- 2. SET UP REMOTE OTA (PULL) ---
  if (driveLink != nullptr && strlen(driveLink) > 0) {
    _updateUrl = extractDriveId(driveLink);
    Serial.println("Remote OTA configured with Direct URL: " + _updateUrl);
  }

  Serial.println("OTA Ready. IP address: " + WiFi.localIP().toString());
}

void RemoteUpload::handle() {
  ArduinoOTA.handle(); // Listens for local IDE uploads
}

void RemoteUpload::checkUpdate() {
  if (_updateUrl.isEmpty()) {
    Serial.println("OTA Error: No Google Drive link was provided in begin()");
    return;
  }

  Serial.println("\n--- Starting OTA Check ---");
  
  // Check memory before we do anything heavy
  Serial.printf("Free RAM before HTTPS: %d bytes\n", ESP.getFreeHeap());
  
  #if defined(ESP8266)
    Serial.printf("Heap Fragmentation: %d%%\n", ESP.getHeapFragmentation());
  #endif

  WiFiClientSecure client;
  client.setInsecure(); 
  
  OTA_UPDATER_OBJ.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  
  // Check memory right before the massive download begins
  Serial.printf("Free RAM right before download: %d bytes\n", ESP.getFreeHeap());

  t_httpUpdate_return ret = OTA_UPDATER_OBJ.update(client, _updateUrl);
  
  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("Update Failed. Error (%d): %s\n", OTA_UPDATER_OBJ.getLastError(), OTA_UPDATER_OBJ.getLastErrorString().c_str());
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("No new updates found.");
      break;
    case HTTP_UPDATE_OK:
      Serial.println("Update successful! Rebooting now...");
      break;
  }
}

String RemoteUpload::extractDriveId(String rawUrl) {
  int startIndex = rawUrl.indexOf("/d/");
  if (startIndex == -1) {
    Serial.println("OTA Warning: Link does not look like a standard Google Drive URL.");
    return rawUrl; // Return as-is, just in case they used a custom direct link
  }
  startIndex += 3;

  int endIndex = rawUrl.indexOf("/", startIndex);
  if (endIndex == -1) endIndex = rawUrl.indexOf("?", startIndex);
  if (endIndex == -1) endIndex = rawUrl.length();

  String fileId = rawUrl.substring(startIndex, endIndex);
  return "https://drive.google.com/uc?export=download&id=" + fileId;
}

void RemoteUpload::handleSerial(const char* triggerWord) {
  // Check if there is any data coming in over the USB cable
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Clean up invisible characters like \r or \n
    
    // Use equalsIgnoreCase so "UPDATE", "Update", and "update" all work
    if (input.equalsIgnoreCase(triggerWord)) {
      Serial.println("\n[RemoteOTA] Trigger word received via Serial!");
      checkUpdate(); 
    }
  }
}

RemoteUpload ota;