** Student: Destiny Ellenwood **
** UCF ID: 5276398 **
** Course: EEE4775 – Real-Time Systems **
** Semester: Summer 2025 **
** AI Use: Checked for spelling errors and assisted with understanding the questions that were asked. **

** This application simulates a real-time onboard system for a small satellite or space probe. The blinking LED represents a beacon or heartbeat indicator, confirming that the satellite is operational. **
** The solar sensor task mimics a subsystem monitoring light intensity to detect eclipse events or solar panel exposure, which is critical for managing power in orbit. Periodic telemetry messages simulate status updates sent to ground control. **
** When the system receives an external trigger (e.g., button press), it compresses and transmits a summary of logged sensor data, as if responding to a ground command. All tasks are carefully prioritized to reflect real-world satellite constraints, ensuring power-critical functions preempt visual indicators and logging operations. **
** This application demonstrates core real-time principles within a space mission context. **

** 1. Signal Discipline: **
The binary semaphore synchronizes the button task with the system by letting the event response task know a button press occurred. When the button is pressed(falling edge), the task gives the semaphore, and the EventResponseTask takes it to perform the system mode toggle and log compression. 
Since it’s binary, it doesn’t queue up multiple presses, it just flips to “given” once. 
If you press the button multiple times quickly, some presses might get missed if the semaphore hasn’t been taken yet. If i had used a counting semaphore instead, I could queue multiple presses and handle each one, but that’s not what we needed here since we only want one response per press.

** 2. Event Flood Handling ** 
When I rotated the potentiometer quickly past the lux threshold, I saw multiple alerts printed saying "⚠️ RADIATION ALERT! Avg Lux below safe threshold!". That's because the counting semaphore took account each event that crosssed the threshold. 
I set it to where the counting semaphore let me queue up to 10 alerts so that none were missed. If I had used a binary semaphore instead, only the first event would trigger a response, 
and the rest would be ignored until the task took the semaphore. That would be a problem for things like repeated alerts/events, especially in this case where my space system needs to have a regulated radiation level, so those other alerts being lost would break my real-time alert behavior.

** 3. Protecting Shared Output **
In my space system, the telemetry messages, radiation alerts, and log compressions are all printed to the console. I designed it kind of like how a satellite downlinks different system statuses to mission control. When I removed the mutex for testing, 
I started seeing jumbled or overlapping messages, especially when a radiation alert and telemetry packet happened around the same time.
It showed me how important it is to guard shared output. Even though these tasks don’t run exactly at the same time, the RTOS can context switch in the middle of a printf(), which makes logs messy and hard to read. 
Without the mutex, multiple tasks tried writing to the UART at once, which in a real satellite could mean corrupting critical mission logs or missing a hazard warning, like my radiation levels which is dangerous for both hardware and mission success.

 ** 4. Scheduling and Preemption **
The task priorities definitely affected responsiveness. For example, my EventResponseTask is priority 5, so when a radiation alert is triggered, it immediately preempts the telemetry_task or the satellite_beacon_task. When the lux sensor dropped below the threshold, 
the EventResponseTask ran right away and blinked the red LED, even if the satellite_beacon_task or telemetry_task was already running. That’s exactly what I expected.
During those alert periods, the heartbeat from my satellite_beacon_task (green LED blink) slowed down, which makes sense since it’s the lowest priority and the higher-priority tasks like sensor monitoring or event handling were taking over the CPU.


 ** 5. Timing and Responsiveness **
Most of my tasks use vTaskDelay, which works fine for non-time-critical operations like telemetry and blinking. But for my solar_sensor_task, I switched to vTaskDelayUntil to maintain a consistent 500ms sampling period regardless of how long the task takes.
This gave me predictable timing, especially when doing math like the moving average for the radiation levels. Polling rate absolutely affects detection. If I delayed too long between readings, I could miss quick dips in light. 
So for tasks tied to real-time thresholds, vTaskDelayUntil is better for deterministic timing.
 
 ** 6. Theme Integration ** 

I chose the space systems theme. The green LED is my satellite’s heartbeat, the light sensor tracks radiation levels (lux as a stand-in), and the red LED signals radiation exposure. The pushbutton represents a ground control command, like toggling safe mode. 
Synchronization matters here because if radiation warnings or commands are delayed or skipped, it could mean serious system failure. 
The mutex and semaphores made sure each event was handled safely and quickly, ensuring it maintained real-time behavior.




