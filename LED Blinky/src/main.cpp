#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define NEO_PIN 45
#define LED_COUNT 1 

void led_blinky(void *pvParameters)
{
  pinMode(GPIO_NUM_48, OUTPUT); // Initialize LED pin

  while(1)
  {
    digitalWrite(GPIO_NUM_48, HIGH);  // Turn ON LED
    vTaskDelay(1000);
    digitalWrite(GPIO_NUM_48, LOW);   // Turn OFF LED
    vTaskDelay(1000);
  }
}

void neo_blinky(void *pvParameters)
{
    Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    
    strip.begin();
    strip.clear();  // Set all pixels to off to start
    strip.show();

    while(1)
    {                          
        strip.setPixelColor(0, strip.Color(255, 0, 0)); // Set pixel 0 to red
        strip.show();                                   // Update the strip
        vTaskDelay(1000);
        strip.setPixelColor(0, strip.Color(0, 0, 0));   // Set the pixel to off
        strip.show();                                   // Update the strip
        vTaskDelay(1000);
    }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  xTaskCreate(led_blinky, "LED Control", 2048, NULL, 2, NULL);  // Task control single LED
  xTaskCreate(neo_blinky, "NEO Control", 2048, NULL, 2, NULL);  // Task control LED NEO
}

void loop() {
  // put your main code here, to run repeatedly:
}