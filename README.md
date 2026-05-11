# RemoteUpload 🛜

**By [Idlan Zafran Mohd Zaidie](https://github.com/IdlanZafran)**

[![Follow](https://img.shields.io/github/followers/IdlanZafran?label=Follow%20Me&style=social)](https://github.com/IdlanZafran)

[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/IdlanZafran/RemoteUpload)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

_If you like this project, don’t forget to:_
- _⭐ Star this repository_
- _👤 Follow my GitHub for future updates_

# Overview

**RemoteUpload** is a lightweight, cross-platform (ESP32 & ESP8266) library for Arduino and PlatformIO that simplifies Over-The-Air (OTA) updates. 

It handles two things simultaneously:
1. **Local OTA:** Allows you to upload code directly from the Arduino IDE or PlatformIO over your local WiFi.
2. **Remote OTA via Google Drive:** Automatically downloads and flashes `.bin` firmware files hosted on Google Drive.

## Features
- 🚀 **Dual Compatibility:** Works out-of-the-box on both ESP8266 and ESP32.
- 🔗 **Google Drive URL Parsing:** Just paste the standard Google Drive "Share" link. The library automatically extracts the file ID and formats the direct download link.
- ⌨️ **Serial Trigger:** Built-in `handleSerial()` function lets you trigger remote updates by simply typing a word (e.g., "update") into the Serial Monitor.

## Installation

### PlatformIO
Add the following to your `platformio.ini` file:

```ini
lib_deps =
  yourusername/RemoteUpload@^1.0.0
```

### Arduino IDE
1. Download this repository as a .zip file.
2. Go to Sketch > Include Library > Add .ZIP Library...
3. Select the downloaded .zip file.

#### Preparing your Google Drive Firmware
1. In the Arduino IDE, go to Sketch > Export compiled Binary (or use pio run -t target in PlatformIO).
2. Upload the .bin file to Google Drive.
3. Right-click the file in Google Drive, click Share, and set General Access to "Anyone with the link".
3. Copy the link.