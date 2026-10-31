#include <Arduino.h>
#include "DHT20.h"

#define LED_PIN 48
#define SDA_PIN 11
#define SCL_PIN 12

DHT20 dht;
float temperature = 0.0;
float humidity = 0.0;
char buffer[64];

void task_scan_I2C_bus(void *pvParameters)
{
  byte address, error;

  while (1)
  {
    for (address = 1; address < 127; address++)
    {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();

      if (error == 0)
      {
        Serial.print("\nI2C device found at: 0x");
        if (address < 16) Serial.print("0");
        Serial.println(address, HEX);
      }
      else
      {
        Serial.print(".");
      }
    }

    // Scan every 5 seconds
    vTaskDelay(5000);
  }
}

void task_read_DHT20_info(void *pvParameters)
{
  while (1)
  {
    dht.read();

    temperature = dht.getTemperature();
    humidity = dht.getHumidity();

    if (isnan(temperature) || isnan(humidity))
    {
      Serial.println("Read DHT20 sensor failed");
    }
    else
    {
      sprintf(buffer, "Temperature: %.1f °C - Humidity: %.1f %%", temperature, humidity);
      Serial.println(buffer);
    }

    vTaskDelay(3000);
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  Wire.begin(SDA_PIN, SCL_PIN);
  dht.begin();
  
  delay(1000);  // Wait for sensor stable

  //xTaskCreate(task_scan_I2C_bus, "Scan I2C Bus", 2048, NULL, 31, NULL); // Task scan I2C bus
  xTaskCreate(task_read_DHT20_info, "Read DHT20 Info", 4096, NULL, 10, NULL); // Task read DHT20 information
}

void loop()
{
  // put your main code here, to run repeatedly:
}