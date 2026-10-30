#include "task_wifi.h"
#include <WiFi.h>
#include "web_server.h"

#define BOOT_BUTTON_PIN GPIO_NUM_0 // BOOT button (active-low)

unsigned long lastConnectWifiTime = 0;

// AP mode
void startAP()
{
    isSTDMode = false;
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid.c_str(), password.c_str());

    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
}

// STA mode
void connectToWiFi()
{
    Serial.print("Connecting to: ");
    Serial.print(wifi_ssid.c_str());
    Serial.print(" Password: ");
    //Serial.println(wifi_password.c_str());

    isWifiConnected = false;
    lastConnectWifiTime = millis();

    isSTDMode = true;
    WiFi.mode(WIFI_STA);

    if (wifi_password.isEmpty())
    {
        WiFi.begin(wifi_ssid.c_str());
    }
    else
    {
        WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
    }
}

void task_wifi(void *pvParameters)
{
    pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);

    while (1)
    {
        handleClient();

        // Switch to AP Mode when boot button pressed
        if (digitalRead(BOOT_BUTTON_PIN) == LOW)
        {
            vTaskDelay(100); // Simple debounce

            if (digitalRead(BOOT_BUTTON_PIN) == LOW)
            {
                if (isSTDMode)
                {
                    startAP();
                    setupServer();
                }
            }
        }

        // STA Mode
        if (isSTDMode)
        {
            if (WiFi.status() == WL_CONNECTED)
            {
                if (!isWifiConnected)
                {
                    Serial.print("STA IP address: ");
                    Serial.println(WiFi.localIP());

                    setupServer();
                    isWifiConnected = true;
                }
                
                xSemaphoreGive(xBinarySemaphoreInternet);
            }
            else if (millis() - lastConnectWifiTime > 5000)
            {
                // Reconnect to wifi every 5 seconds
                connectToWiFi();
            }
        }

        vTaskDelay(20); // Avoid watchdog reset
    }
}