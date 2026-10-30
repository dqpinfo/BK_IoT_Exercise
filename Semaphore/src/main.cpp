#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define BUTTON_PIN  GPIO_NUM_0  // BOOT button (active-low)
#define LED_PIN     GPIO_NUM_48 // Single LED on GPIO48

// Global semaphore handle
SemaphoreHandle_t xBinarySemaphore = NULL;

void task_monitor_button(void *pvParameters)
{
  while (1)
  {
    // Button pressed (active-low)
    if (gpio_get_level(BUTTON_PIN) == 0)
    {
      vTaskDelay(pdMS_TO_TICKS(50)); // Simple debounce

      if (gpio_get_level(BUTTON_PIN) == 0)
      {
        // Give the semaphore to blink LED task
        if (xSemaphoreGive(xBinarySemaphore) == pdTRUE)
          Serial.println("Give -> OK");
        else
          Serial.println("Give ignored (semaphore full)");
        
        // Wait for release: 1 press = 1 Give
        while (gpio_get_level(BUTTON_PIN) == 0)
          vTaskDelay(pdMS_TO_TICKS(10));
      }
    }

    // Polling every 10 ms
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void task_blink_led(void *pvParameters)
{
  while (1)
  {
    // Blocked here (0% CPU) until monitor button task gives
    if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE)
    {
      Serial.println("Take -> OK");

      // Blink LED 3 times
      for (int i = 0; i < 3; i++)
      {
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(200));
        gpio_set_level(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(200));
      }
    }
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  // Created empty (0), task Take has to wait
  xBinarySemaphore = xSemaphoreCreateBinary();

  if (xBinarySemaphore == NULL)
  {
    Serial.println("Create semaphore failed (out of heap)");
    return;
  }
  
  // Stack size is in BYTES on the ESP32 port
  xTaskCreate(task_monitor_button, "Monitor Button", 4096, NULL, 31, NULL);
  xTaskCreate(task_blink_led, "Blink LED", 4096, NULL, 1, NULL);
}

void loop()
{
  // put your main code here, to run repeatedly:
}