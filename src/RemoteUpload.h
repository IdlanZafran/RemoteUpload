/* Library: RemoteUpload
 * Description: Effortless Local and Remote (Google Drive) OTA updates for ESP32 and ESP8266
 * @author Idlan Zafran Mohd Zaidie
 * Date: 2026
 * Note: You are free to use this code, but please leave this header intact.
 * @license MIT
 */

#ifndef REMOTE_UPLOAD_H
#define REMOTE_UPLOAD_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// --- SMART BOARD DETECTION ---
#if defined(ESP32)
  #include <WiFi.h>
  #include <ESPmDNS.h>
  #include <HTTPUpdate.h>
  #define OTA_UPDATER_OBJ httpUpdate 
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <ESP8266httpUpdate.h>
  #define OTA_UPDATER_OBJ ESPhttpUpdate
#else
  #error "This library only supports ESP32 and ESP8266!"
#endif

class RemoteUpload {
  public:
    RemoteUpload();
    void begin(const char* hostname = "esp-device", const char* password = nullptr, const char* driveLink = nullptr);
    void handle();
    void checkUpdate();

    void handleSerial(const char* triggerWord = "update");

  private:
    String _updateUrl;
    String extractDriveId(String rawUrl); 
};

extern RemoteUpload ota;

#endif