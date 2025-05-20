/* --------------------------------------------------------------
   Destiny Ellenwood
   UCFID: 5276398
   Application: 01 - Rev1
   Class: Real Time Systems - Su 2025
   AI Use: Commented inline, assisted with defining function uses to help me fill in parameters
---------------------------------------------------------------*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_4 // I'm using GPIO4 to simulate a satellite beacon light (instead of the default GPIO2)


// This task simulates the satellite's beacon LED blinking at 2 Hz (on/off every 250 ms)
void satellite_beacon_task(void *pvParameters) {
    bool beacon_on = false; // Keeps track of LED state (ON or OFF)
    TickType_t previous_tick = xTaskGetTickCount();  // Saves the tick count when the task last ran

    while (1) {
        TickType_t current_tick = xTaskGetTickCount();  // Current tick count when task runs
        uint32_t elapsed_ms = (current_tick - previous_tick) * portTICK_PERIOD_MS;  // Calculate elapsed time
        previous_tick = current_tick;  // Update the previous tick for next loop

        if (beacon_on) {
            gpio_set_level(LED_PIN, 1);  // Turn LED ON
            printf("Beacon ON | Time = %lu ms | Period = %lu ms\n",
       (unsigned long)(current_tick * portTICK_PERIOD_MS),
       (unsigned long)elapsed_ms);

        } else {
            gpio_set_level(LED_PIN, 0);  // Turn LED OFF
            printf("Beacon OFF | Time = %lu ms | Period = %lu ms\n",
       (unsigned long)(current_tick * portTICK_PERIOD_MS),
       (unsigned long)elapsed_ms);
        }

        beacon_on = !beacon_on;  // Flip the LED state for the next loop
        vTaskDelay(pdMS_TO_TICKS(250));  // Wait 250 ms → this gives a 2 Hz blink rate
    }

    // This will never run (task loops forever)
    vTaskDelete(NULL);
}

// This task handles the telemetry messages — logs system status every 10 seconds 
void telemetry_task(void *pvParameters) {
  int packet_id = 1;  // Increments each time we log telemetry
  while (1) {
    // Print out a simulated telemetry message with time in ms
    printf("Telemetry Packet %d: Status OK | Time = %lu ms\n", packet_id, xTaskGetTickCount() * portTICK_PERIOD_MS);
    packet_id++;   // Move to next packet
    vTaskDelay(pdMS_TO_TICKS(10000)); // Delay for 10 seconds (0.1 Hz task)
  }

  vTaskDelete(NULL); // Never reached — this task runs forever
}

// This is the main entry point where I configure the hardware and start the tasks
void app_main() {
    gpio_reset_pin(LED_PIN);  // Reset the pin (good practice)
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);  // Set the LED pin as output
    
    // Create the two FreeRTOS tasks with equal priority
    xTaskCreate(satellite_beacon_task, "Beacon Task", 2048, NULL, 1, NULL);
    xTaskCreate(telemetry_task, "Telemetry Task", 2048, NULL, 1, NULL);
}
