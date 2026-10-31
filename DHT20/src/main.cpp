#include <Arduino.h>
#include "DHT20.h"

#define LED_PIN 48
#define SDA_PIN 11
#define SCL_PIN 12

DHT20 dht;

void task_scan_I2C_bus(void *pvParameters)
{
  byte address, error;

  while (1)
  {
    //Serial.println("Scanning I2C bus...");

    for (address = 1; address < 127; address++)
    {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();

      if (error == 0)
      {
        Serial.print("I2C device found at: 0x");
        if (address < 16) Serial.print("0");
        Serial.println(address, HEX);
      }
      else
      {
        //Serial.print(error);
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
    double temperature = dht.getTemperature();
    double humidity = dht.getHumidity();

    Serial.print("Temperature: "); Serial.print(temperature); Serial.print(" °C");
    Serial.print(" - Humidity: "); Serial.print(humidity); Serial.println(" %");

    vTaskDelay(2000);
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  
  Wire.begin(SDA_PIN, SCL_PIN);
  dht.begin();

  //xTaskCreate(task_scan_I2C_bus, "Scan I2C Bus", 2048, NULL, 31, NULL);     // Task scan I2C bus
  xTaskCreate(task_read_DHT20_info, "Read DHT20 Info", 2048, NULL, 2, NULL);  // Task read DHT20 information
}

void loop()
{
  // put your main code here, to run repeatedly:
}