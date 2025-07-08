/* --------------------------------------------------------------
   Destiny Ellenwood
   UCFID: 5276398
   Application: 04
   Class: Real Time Systems - Su 2025
   AI Use: Commented inline, assisted with defining functions, and used to help me fill in parameters
---------------------------------------------------------------*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "math.h"

#define ALERT_LED_PIN GPIO_NUM_15 // Alert led for radiation levels
#define LED_PIN GPIO_NUM_2 // I'm using GPIO2 to simulate a satellite beacon light (instead of the default GPIO2)
#define LDR_ADC_CHANNEL ADC1_CHANNEL_4  // GPIO32
#define LDR_PIN GPIO_NUM_32
#define VREF 3.3
#define RFIXED 10000.0 // 10kΩ resistor
#define GAMMA 0.7 // Given
#define RL 50.0 // Given
#define AVG_WINDOW 10
#define SENSOR_THRESHOLD 200  // I chose this threshold
#define BUFFER_SIZE 50
#define BUTTON_PIN GPIO_NUM_4 //TODOA3a - GPIO pin for pushbutton interrupt trigger

int sensor_buffer[BUFFER_SIZE];  //TODOA3b - Circular buffer for storing lux readings for logger task
int buffer_index = 0;            //TODOA3c - Index to keep track of current position in the buffer

SemaphoreHandle_t xButtonSem;   //TODOA3d - Binary semaphore signaled by ISR to wake logger task
SemaphoreHandle_t xLogMutex;    //TODOA3e - Mutex to protect shared access to sensor_buffer
SemaphoreHandle_t xSensorEventSem;  // Counting semaphore for sensor alerts



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
        } else {
            gpio_set_level(LED_PIN, 0);  // Turn LED OFF
        }

        beacon_on = !beacon_on;  // Flip the LED state for the next loop
        vTaskDelay(pdMS_TO_TICKS(1400));  // Wait 1400 ms → this gives a 1 Hz blink rate
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
    xSemaphoreTake(xLogMutex, portMAX_DELAY);
    printf("Telemetry Packet %d: Status OK | Time = %lu ms\n", packet_id, xTaskGetTickCount() * portTICK_PERIOD_MS);
    xSemaphoreGive(xLogMutex);
    
    packet_id++;   // Move to next packet
    vTaskDelay(pdMS_TO_TICKS(7000)); // Delay for 7 seconds 
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
        lux = pow(50 * 1e3 * pow(10, 0.7) / Rmeasure, (1 / 0.7));       //TODO11d - Corrected lux formula 
        luxreadings[i] = lux;
        sum += lux;
    }


    while (1) {
        // Read raw ADC value
        raw = adc1_get_raw(LDR_ADC_CHANNEL);

        // Recompute Vmeasure, Rmeasure, and lux every cycle
        Vmeasure = (raw / 4095.0) * VREF;                          //TODO11e
        Rmeasure = (Vmeasure / (VREF - Vmeasure)) * RFIXED;       //TODO11f
        lux = pow(50 * 1e3 * pow(10, GAMMA) / Rmeasure, (1.0 / GAMMA));  //TODO11g - Corrected lux formula      

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
             xSemaphoreGive(xSensorEventSem);  // Signal the Event Response Task
      
        } //else {
            //TODO11i - (Optional) Print normal average lux for debugging
            //printf("Sensor Reading: Avg Lux = %d\n", avg);
        //}

        xSemaphoreTake(xLogMutex, portMAX_DELAY);
        sensor_buffer[buffer_index] = avg;
        buffer_index = (buffer_index + 1) % BUFFER_SIZE;
        xSemaphoreGive(xLogMutex);


        //TODO11j - Print time info for analysis/logging
        printf("Sensor Time = %lu ms\n", xTaskGetTickCount() * portTICK_PERIOD_MS);

        //TODO11k - Use vTaskDelayUntil for consistent sampling period
        vTaskDelayUntil(&lastWakeTime, periodTicks);
    }

    vTaskDelete(NULL); // Not expected to run

}


void EventResponseTask(void *pvParameters) {
    static bool system_mode = false; // Keeps track of current system mode (used to simulate SAFE or NORMAL state)

    while (1) {
        // Check for sensor alert (from radiation/light sensor
        // This uses a counting semaphore so we can catch multiple threshold crossings.
        if (xSemaphoreTake(xSensorEventSem, pdMS_TO_TICKS(100)) == pdTRUE) {
            xSemaphoreTake(xLogMutex, portMAX_DELAY); // Use mutex to protect console output from other tasks
            printf("⚠️ RADIATION ALERT! Avg Lux below safe threshold!\n");
            xSemaphoreGive(xLogMutex);

            // Blink red LED to signal high-radiation warning (real-time reaction)
            gpio_set_level(ALERT_LED_PIN, 1);
            vTaskDelay(pdMS_TO_TICKS(500));
            gpio_set_level(ALERT_LED_PIN, 0);
        }

        // Check for button press using a binary semaphore (ground control command)
        // This part is blocking for 10ms, which is a small window to stay responsive
        if (xSemaphoreTake(xButtonSem, pdMS_TO_TICKS(10)) == pdTRUE) {
            // Toggle simulated system mode on button press
            system_mode = !system_mode;

            xSemaphoreTake(xLogMutex, portMAX_DELAY); // Mutex still needed for safe UART printing
            printf("📡 Ground Control Command Received. System mode toggled to: %s\n", 
                   system_mode ? "SAFE MODE" : "NORMAL MODE");

            // Simulated log compression task (original LoggerTask is merged here)
            // We summarize sensor buffer data for downlink on command
            int min = 4095, max = 0, sum = 0, count = BUFFER_SIZE;
            for (int i = 0; i < BUFFER_SIZE; i++) {
                int val = sensor_buffer[i];
                if (val < min) min = val;
                if (val > max) max = val;
                sum += val;
            }
            float avg = sum / (float)count;
            printf("Button pressed! Compressing log...\n");
            printf("LOG DUMP: N=%d, Min=%d, Max=%d, Avg=%.2f\n", count, min, max, avg);
            xSemaphoreGive(xLogMutex);

            // Blink green LED rapidly as an acknowledgment of the ground command
            for (int i = 0; i < 3; i++) {
                gpio_set_level(LED_PIN, 1);
                vTaskDelay(pdMS_TO_TICKS(100));
                gpio_set_level(LED_PIN, 0);
                vTaskDelay(pdMS_TO_TICKS(100));
            }
        }
    }

    vTaskDelete(NULL); // Never reached
}



void button_watch_task(void *pvParameters) {
    static int last_state = 1;  // Start assuming button is unpressed (logic HIGH due to pull-up)
    TickType_t last_debounce_time = 0;
    const TickType_t debounce_delay = pdMS_TO_TICKS(50);  // 50 ms debounce time

    while (1) {
        int current_state = gpio_get_level(BUTTON_PIN);  // Read the current button state

        // Check for a falling edge: means the button was just pressed (HIGH → LOW transition)
        if (current_state == 0 && last_state == 1) {
            TickType_t now = xTaskGetTickCount();
            // Only register the press if enough time has passed (debounce logic)
            if ((now - last_debounce_time) > debounce_delay) {
                last_debounce_time = now;

                // Give binary semaphore to notify EventResponseTask of button press
                xSemaphoreGive(xButtonSem); // This replaces ISR logic from App 3 with task-level polling
            }
        }

        last_state = current_state;  // Save the current state for next loop
        vTaskDelay(pdMS_TO_TICKS(10));  // Poll button every 10ms (lightweight and responsive enough)
    }
}



// This is the main entry point where I configure the hardware and start the tasks
void app_main() {

    // --- BUTTON SETUP (No ISR used in App 4) ---
    // Configuring GPIO4 as input with internal pull-up resistor (active-low button logic)
    // We're detecting falling edges using polling and debounce inside the button_watch_task
gpio_config_t io_conf = {
    .intr_type = GPIO_INTR_DISABLE,               //Disable interrupts (moved away from ISR-based design)
    .mode = GPIO_MODE_INPUT,                      //TODOA3o - Set button pin as input
    .pin_bit_mask = (1ULL << BUTTON_PIN),         //TODOA3p - Select GPIO4 for interrupt
    .pull_up_en = GPIO_PULLUP_ENABLE              //TODOA3q - Enable internal pull-up (no external resistor needed)
};
gpio_config(&io_conf);  //TODOA3r - Apply GPIO config



    xButtonSem = xSemaphoreCreateBinary();  //TODOA3u - Create binary semaphore for button press
    xLogMutex = xSemaphoreCreateMutex();  //TODOA3v - Create mutex for accessing sensor buffer
    xSensorEventSem = xSemaphoreCreateCounting(10, 0); // max count 10, starts at 0; 10 is the max number of events it can queue

    // Setting up the LED pin for alert
    gpio_reset_pin(ALERT_LED_PIN);
    gpio_set_direction(ALERT_LED_PIN, GPIO_MODE_OUTPUT);


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
    xTaskCreatePinnedToCore(EventResponseTask, "EventResponse", 4096, NULL, 5, NULL, 1);// Priority 5 (Critical): Responds to both sensor alerts and ground-control commands; must preempt lower tasks to handle real-time events like radiation warnings or emergency toggles
    xTaskCreatePinnedToCore(button_watch_task, "ButtonWatch", 2048, NULL, 4, NULL, 1); // Priority 4 (High): Monitors ground-control button with software debounce and signals button events; responsiveness is important to avoid missing presses

}
