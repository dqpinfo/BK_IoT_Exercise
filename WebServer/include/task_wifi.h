#ifndef __TASK_WIFI_H__
#define __TASK_WIFI_H__

#include <Arduino.h>
#include "global.h"

void startAP();
void connectToWiFi();

void task_wifi(void *pvParameters);

#endif