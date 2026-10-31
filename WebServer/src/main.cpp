#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "global.h"
#include "task_wifi.h"
#include <Adafruit_NeoPixel.h>
#include "DHT20.h"

#define LED_PIN   48
#define NEO_PIN   45
#define NEO_COUNT 1
#define SDA_PIN   11
#define SCL_PIN   12

DHT20 dht;

void internet_task(void *pvParameters)
{
  while (1)
  {
    // Bị chặn ở đây đến khi xSemaphoreGive() được gọi
    if (xSemaphoreTake(xBinarySemaphoreInternet, portMAX_DELAY) == pdTRUE)
    {
      // TODO: execute tasks need WiFi connection
    }
  }
}

void task_dht20(void *pvParameters)
{
  while (1)
  {
    dht.read();

    glob_temperature = dht.getTemperature();
    glob_humidity = dht.getHumidity();

    if (isnan(glob_temperature) || isnan(glob_humidity))
    {
      Serial.println("Read DHT20 sensor failed");
    }
    else
    {
      Serial.printf("Temperature: %.1f °C - Humidity: %.1f %%\n", glob_temperature, glob_humidity);
    }

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

  Wire.begin(SDA_PIN, SCL_PIN);
  dht.begin();

  delay(1000);  // Wait for sensor stable

  // Tạo semaphore trước khi task dùng
  xBinarySemaphoreInternet = xSemaphoreCreateBinary();
 
  // Khởi động các task
  xTaskCreate(task_wifi, "task wifi", 8192, NULL, 31, NULL);
  xTaskCreate(task_dht20, "task dht20", 4096, NULL, 30, NULL);
  xTaskCreate(task_LED, "task LED", 4096, NULL, 29, NULL);
}
 
void loop()
{
  // put your main code here, to run repeatedly:
}