/* --------------------------------------------------------------
   Destiny Ellenwood
   UCFID: 5276398
   Application: 01 - Rev1
   Class: Real Time Systems - Su 2025
---------------------------------------------------------------*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_2 // LED as satellite status beacon


// Task to simulate beacon LED blinking at 2 Hz
void beacon_task(void *pvParameters) {
    bool led_on = false;
    while (1) {
        if (led_on) {
            gpio_set_level(LED_PIN, 1);  // Turn LED ON
            printf("Beacon LED ON at time = %lu ms\n", xTaskGetTickCount() * portTICK_PERIOD_MS);
        } else {
            gpio_set_level(LED_PIN, 0);  // Turn LED OFF
            printf("Beacon LED OFF at time = %lu ms\n", xTaskGetTickCount() * portTICK_PERIOD_MS);
        }

        led_on = !led_on;  // Toggle LED state
        vTaskDelay(pdMS_TO_TICKS(250));  // Wait 250 ms
    }

    // Task runs forever, so this is not reached
    vTaskDelete(NULL);
}

// Task to 
void telemetry_task(void *pvParameters) {
  int packet_id = 1;
  while (1) {
    printf("Telemetry Packet %d: Status OK | Time = %lu ms\n", packet_id, xTaskGetTickCount() * portTICK_PERIOD_MS);
    packet_id++;
    vTaskDelay(pdMS_TO_TICKS(10000)); // Wait 10 seconds
  }

  vTaskDelete(NULL);
}

void app_main() {
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    
    xTaskCreate(beacon_task, "Beacon Task", 2048, NULL, 1, NULL);
    xTaskCreate(telemetry_task, "Telemetry Task", 2048, NULL, 1, NULL);
}

