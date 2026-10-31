#include <Arduino.h>
#include "global.h"
#include "task_wifi.h"
#include <Adafruit_NeoPixel.h>

#define LED_PIN   48
#define NEO_PIN   45
#define NEO_COUNT 1

void internet_task(void *pvParameters)
{
  while (1)
  {
    // Bị chặn ở đây đến khi xSemaphoreGive() được gọi
    if (xSemaphoreTake(xBinarySemaphoreInternet, portMAX_DELAY) == pdTRUE)
    {
      Serial.println("Connected to internet");
      // TODO
    }
  }
}

void task_dht20(void *pvParameters)
{
  while (1)
  {
    glob_temperature = (float)random(10, 99);
    glob_humidity = (float)random(10, 99);

    //Serial.print(glob_temperature);
    //Serial.print(" °C Humidity: ");
    //Serial.print(glob_humidity);
    //Serial.println(" %");
    
    vTaskDelay(3000);
  }
}

void task_LED(void *pvParameters)
{
  pinMode(LED_PIN, OUTPUT);

  Adafruit_NeoPixel strip(NEO_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
  strip.begin();
  strip.clear();  // Set all pixels to off to start
  strip.show();

  while (1)
  {
    if (glob_singleLEDState)
    {
      digitalWrite(LED_PIN, HIGH);  // Turn ON LED
    }
    else
    {
      digitalWrite(LED_PIN, LOW);   // Turn OFF LED
    }

    if (glob_neoLEDState)
    {
      strip.setPixelColor(0, strip.Color(255, 0, 0)); // Set pixel 0 to red
      strip.show();                                   // Update the strip
    }
    else
    {
      strip.setPixelColor(0, strip.Color(0, 0, 0));   // Set the pixel to off
      strip.show();                                   // Update the strip
    }
    
    vTaskDelay(100);
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  // 1) Tạo semaphore trước khi task dùng
  xBinarySemaphoreInternet = xSemaphoreCreateBinary();
 
  // 2) Khởi động các task
  xTaskCreate(task_wifi, "task wifi", 8192, NULL, 31, NULL);
  xTaskCreate(task_dht20, "task dht20", 4096, NULL, 30, NULL);
  xTaskCreate(task_LED, "task LED", 4096, NULL, 29, NULL);
}
 
void loop()
{
  // put your main code here, to run repeatedly:
}