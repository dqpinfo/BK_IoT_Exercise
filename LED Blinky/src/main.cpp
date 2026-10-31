#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define INTERVAL  2000
#define LED_PIN   48
#define NEO_PIN   45
#define NEO_COUNT 1

void led_blinky(void *pvParameters)
{
  pinMode(LED_PIN, OUTPUT); // Initialize LED pin

  while(1)
  {
    digitalWrite(LED_PIN, HIGH);  // Turn ON LED
    Serial.println("Single LED => ON");
    vTaskDelay(INTERVAL);

    digitalWrite(LED_PIN, LOW);   // Turn OFF LED
    Serial.println("Single LED => OFF");
    vTaskDelay(INTERVAL);
  }
}

void neo_blinky(void *pvParameters)
{
    Adafruit_NeoPixel strip(NEO_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    
    strip.begin();
    strip.clear();  // Set all pixels to off to start
    strip.show();

    while(1)
    {                          
        strip.setPixelColor(0, strip.Color(255, 0, 0)); // Set pixel 0 to red
        strip.show();                                   // Update the strip
        Serial.println("NEO LED => ON");
        vTaskDelay(INTERVAL);

        strip.setPixelColor(0, strip.Color(0, 0, 0));   // Set the pixel to off
        strip.show();                                   // Update the strip
        Serial.println("NEO LED => OFF");
        vTaskDelay(INTERVAL);
    }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  xTaskCreate(led_blinky, "LED Control", 2048, NULL, 31, NULL); // Task control single LED
  xTaskCreate(neo_blinky, "NEO Control", 2048, NULL, 1, NULL);  // Task control LED NEO
}

void loop()
{
  // put your main code here, to run repeatedly:
}