
Destiny Ellenwood
Application 6: Defense Drone Threat Response System (DDTRS)
UCF - Real Time Systems - Su 2025
Company Context: Raytheon Autonomous Systems Division
Mission: Detect flash-based threats, respond autonomously, and allow
real-time operator override via button and web-based HTTP interface.
---

## Company Synopsis: ##
Raytheon is a leading defense contractor that builds advanced autonomous systems for national security, including drones and missile defense systems. 
In this project, I built a real-time prototype for a Defense Drone Threat Response System (DDTRS), which reacts to light-based threats like muzzle flashes or laser targeting. 
The system processes sensor data in real time and gives both autonomous and manual override capabilities through a pushbutton and web interface. 
Real-time constraints are critical because delays in detection or response could compromise the drone’s mission or safety.

---
## Task Table: ##

sensor_task: Runs every 500 ms (Hard). If this misses its deadline, the system might fail to detect a threat on time.

response_task: Runs with semaphores (within 10–100 ms window) (Hard). Missing this means no alert or mode change in time.

button_watch_task: Polls every 10 ms (Hard). If missed, the operator’s toggle might not register during a safety event.

wifi_server_task: Polled around every 20 ms (Soft). Failure just delays web control or feedback.

heartbeat_task: Runs every 1000 ms (Soft). This is just for visual heartbeat – failure doesn't impact mission logic.

isr_override(): Triggered by falling edge interrupt (Hard). If this fails, an emergency override won't be captured.

---

## Scheduler Fit: ## 
All hard tasks have higher priority than soft tasks. response_task is priority 4, sensor_task is 3, and button_watch_task is 3. Soft tasks like heartbeat_task are priority 1.
A timestamp from the serial monitor shows that sensor reading and response log happen within the same 500 ms window. That confirms I'm meeting the deadline. For example:
[Sensor] Lux = 185 followed directly by ⚠️ THREAT DETECTED! Lux = 185.

---

 ## Race-Proofing: ##
Shared resources like current_lux and all Serial output are protected with xLogMutex to avoid race conditions.
Example: in sensor_task, I do:

xSemaphoreTake(xLogMutex, portMAX_DELAY);
Serial.printf("[Sensor] Lux = %d\n", current_lux);
xSemaphoreGive(xLogMutex);
Without the mutex, if response_task or the web server were logging at the same time, output could get corrupted.

---

## Worst-Case Spike: ##
I simulated continuous flashing light (lux values < threshold) and rapid button presses. Even when the sensor triggered constantly, response_task was able to handle all semaphores without delay.
There was about 100 ms margin before response_task would start to fall behind. That shows the system can still meet deadlines under high load.

---

## Design Trade-Off: ##
I chose not to implement a dynamic or real-time updating HTML interface with AJAX to avoid unpredictable memory usage and processing delays.
In a defense setting like Raytheon’s, timing and reliability are more important than fancy UI. A simple static page with command links ensures fast, deterministic interaction.

---

## AI Use Disclosure: ##
I used ChatGPT (OpenAI) to brainstorm task structure, help implement the ISR logic, write the HTML for the web server, and verify deadline requirements.
I fully reviewed and customized all code. Any AI-assisted sections were tested for timing and correctness. I documented AI use in comments and here for full transparency.

---
## Concurrency Diagram ##


+-------------------------+            +--------------------------+
|     heartbeat_task      |            |      sensor_task         |
|   (1000ms, Soft)        |            |   (500ms, Hard)          |
+-------------------------+            +--------------------------+
                                                 |
                                                 v
                                          [xSensorSem Semaphore]
                                                 |
                                                 v
+-------------------------+            +--------------------------+
|    response_task        | <----------|  button_watch_task       |
|   (Event-driven, Hard)  |            |   (10ms poll, Hard)      |
|                         |<-----------| [xButtonSem Semaphore]   |
|                         |<----------- ISR_BUTTON_PIN (ISR) ----+
|                         |<-----------| [xISROverrideSem]        |
+-------------------------+            +--------------------------+
           |
           v
[self_destruct_requested flag]
           |
           v
+-------------------------+
|   self_destruct_task    |
|  (50ms poll, Soft)      |
+-------------------------+


        +--------------------------+
        |    wifi_server_task      |
        |   (20ms poll, Soft)      |
        +--------------------------+
           |                |
           v                v
 [drone_mode_safe]   [self_destruct_requested]
           |                |
           v                v
    Affects UI / State   Triggers countdown


+--------------------------+
|   isr_response_task      |
|   (Hard, event-driven)   |
+--------------------------+
         ^
         |
     ISR_BUTTON_PIN
---

Legend: DDTRS

Hard RT = Hard Real-Time task (must meet strict deadlines for safety or mission-critical actions)

Soft RT = Soft Real-Time task (timing preferred but not critical; delays are tolerable)

Boxes = FreeRTOS tasks (each box shows task name, period, and type)

Arrows (→) = Direction of signaling via:

Semaphores (e.g., xSensorSem, xButtonSem, xISROverrideSem)

Shared flags (e.g., self_destruct_requested, drone_mode_safe)

ISR_BUTTON_PIN = External interrupt line tied to hardware button (invokes ISR handler)
