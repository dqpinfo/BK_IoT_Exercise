#include "global.h"

bool isSTDMode = true;
bool isWifiConnected = false;
SemaphoreHandle_t xBinarySemaphoreInternet = NULL;

String ssid = "ESP32-ABC";
String password = "12345678";

String wifi_ssid = "dump wifi";
String wifi_password = "dump password";

float glob_temperature = 0;
float glob_humidity = 0;

bool glob_singleLEDState = false;
bool glob_neoLEDState = false;