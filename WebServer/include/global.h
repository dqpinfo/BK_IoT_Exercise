#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>

// Trạng thái mạng dùng chung giữa các task
extern bool isSTDMode;
extern bool isWifiConnected;
extern SemaphoreHandle_t xBinarySemaphoreInternet;

// Thông tin AP riêng của ESP32
extern String ssid;
extern String password;

// Thông tin router từ trang /settings
extern String wifi_ssid;
extern String wifi_password;

// Giá trị cảm biến (do sensor task ghi)
extern float glob_temperature;
extern float glob_humidity;

// Trạng thái đèn LED trên board
extern bool glob_singleLEDState;
extern bool glob_neoLEDState;

#endif