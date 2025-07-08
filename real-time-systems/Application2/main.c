/* --------------------------------------------------------------
   Destiny Ellenwood
   UCFID: 5276398
   Application: 02
   Class: Real Time Systems - Su 2025
   AI Use: Commented inline, assisted with defining functions, and used to help me fill in parameters
---------------------------------------------------------------*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "math.h"

#define LED_PIN GPIO_NUM_4 // I'm using GPIO4 to simulate a satellite beacon light (instead of the default GPIO2)
#define LDR_ADC_CHANNEL ADC1_CHANNEL_4  // GPIO32
#define LDR_PIN GPIO_NUM_32
#define VREF 3.3
#define RFIXED 10000.0 // 10kΩ resistor
#define GAMMA 0.7 // Given
#define RL 50.0 // Given
#define AVG_WINDOW 10
#define SENSOR_THRESHOLD 200  // I chose this threshold


//TODO9 - Blink Task
// This task simulates the satellite's beacon LED blinking at 1 Hz (on/off every 500 ms)
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
        vTaskDelay(pdMS_TO_TICKS(500));  // Wait 500 ms → this gives a 1 Hz blink rate
    }

    // This will never run (task loops forever)
    vTaskDelete(NULL);
}

//TODO10 - Print Task
// This task handles the telemetry messages — logs system status every 1 second 
void telemetry_task(void *pvParameters) {
  int packet_id = 1;  // Increments each time we log telemetry
  while (1) {
    // Print out a simulated telemetry message with time in ms
    printf("Telemetry Packet %d: Status OK | Time = %lu ms\n", packet_id, xTaskGetTickCount() * portTICK_PERIOD_MS);
    packet_id++;   // Move to next packet
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second 
  }

  vTaskDelete(NULL); // Never reached — this task runs forever
}

//TODO11- Sensor Reading Task
//This is the new high-priority task. It will periodically read the sensor, process the data (compute a moving average), and check for above/below threshold based on your specific context chosen above in order to log an alert. 
void solar_sensor_task (void *pvParameters) {

    // Before entering the loop, get the current tick count
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t periodTicks = pdMS_TO_TICKS(500); // 500 ms interval

    // Variables for raw sensor reading and computed values
    int raw;
    float Vmeasure = 0.0;   //TODO11b/e - Will hold computed Vmeasure from ADC raw value
    float Rmeasure = 0.0;   //TODO11c/f - Will hold computed Rmeasure via voltage divider
    float lux = 0.0;        //TODO11d/g - Final lux value from gamma equation

    //TODO11a - Define moving average window and initialize circular buffer
    float sum = 0;
    int luxreadings[AVG_WINDOW] = {0};
    int idx = 0;

    //TODO99 - Pre-fill buffer to stabilize average on startup
    for (int i = 0; i < AVG_WINDOW; ++i) {
        raw = adc1_get_raw(LDR_ADC_CHANNEL);
        Vmeasure = (raw / 4095.0) * VREF;                          //TODO11b
        Rmeasure = (Vmeasure / (VREF - Vmeasure)) * RFIXED;       //TODO11c
        lux = pow((RL * 1000.0 / Rmeasure), (1.0 / GAMMA));       //TODO11d
        luxreadings[i] = lux;
        sum += lux;
    }

    // BONUS EXPERIMENT: Uncomment this block to simulate starvation
    /*
    while (1) {
        for (volatile int i = 0; i < 100000000; i++); // Simulates 300+ ms of CPU time (busy loop)
        vTaskDelayUntil(&lastWakeTime, periodTicks);
    }
    */

    while (1) {
        // Read raw ADC value
        raw = adc1_get_raw(LDR_ADC_CHANNEL);

        // Recompute Vmeasure, Rmeasure, and lux every cycle
        Vmeasure = (raw / 4095.0) * VREF;                          //TODO11e
        Rmeasure = (Vmeasure / (VREF - Vmeasure)) * RFIXED;       //TODO11f
        lux = pow((RL * 1000.0 / Rmeasure), (1.0 / GAMMA));       //TODO11g

        // Debugging outputs for verification
    printf("[Sensor Debug] RAW ADC = %d\n", raw);
    printf("[Sensor Debug] Vmeasure = %.2f V | Rmeasure = %.2f Ω | LUX = %.2f\n", Vmeasure, Rmeasure, lux);


        // Update moving average circular buffer
        sum -= luxreadings[idx];
        luxreadings[idx] = lux;
        sum += lux;
        idx = (idx + 1) % AVG_WINDOW;
        int avg = sum / AVG_WINDOW;

        //TODO11h - Check average vs threshold and print eclipse warning if necessary
        if (avg < SENSOR_THRESHOLD) {
            printf("**Eclipse Warning!** Avg Lux = %d < threshold %d\n", avg, SENSOR_THRESHOLD);
        } else {
            //TODO11i - (Optional) Print normal average lux for debugging
            printf("Sensor Reading: Avg Lux = %d\n", avg);
        }

        //TODO11j - Print time info for analysis/logging
        printf("Sensor Time = %lu ms\n", xTaskGetTickCount() * portTICK_PERIOD_MS);

        //TODO11k - Use vTaskDelayUntil for consistent sampling period
        vTaskDelayUntil(&lastWakeTime, periodTicks);
    }

    vTaskDelete(NULL); // Not expected to run

}

// This is the main entry point where I configure the hardware and start the tasks
void app_main() {
    // Setting up the LED pin for beacon
    gpio_reset_pin(LED_PIN);  // Reset the pin (good practice)
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);  // Set the LED pin as output

    //TODO4 - Set up GPIO32 (LDR) for input. Ensures GPIO 32 is ready for ADC input
    gpio_reset_pin(LDR_PIN);
    gpio_set_direction(LDR_PIN,GPIO_MODE_INPUT);
    
    //TODO5 - Set resolution of the ADC to 12-bit resolution
    adc1_config_width(ADC_WIDTH_BIT_12);

    //TODO6 - Set attentuation to 11 dB for full 0-3.3V input range
    adc1_config_channel_atten(LDR_ADC_CHANNEL,ADC_ATTEN_DB_11);

    //TODO7 - Pinning tasks to core 1
    //TODO8 - Assigning task priorities
    xTaskCreatePinnedToCore(satellite_beacon_task, "Beacon Task", 2048, NULL, 1, NULL, 1); // Priority 1 (Low): Beacon LED is non-critical and can be delayed safely
    xTaskCreatePinnedToCore(telemetry_task, "Telemetry Task", 2048, NULL, 2, NULL, 1); // Priority 2 (Medium): Telemetry is important for status updates but not time-sensitive
    xTaskCreatePinnedToCore(solar_sensor_task, "Sensor", 4096, NULL, 3, NULL, 1); // Priority 3 (High): Must run reliably every 500ms to detect eclipse conditions in real-time
}
