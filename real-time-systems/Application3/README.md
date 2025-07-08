** Student: Destiny Ellenwood **
** UCF ID: 5276398 **
** Course: EEE4775 – Real-Time Systems **
** Semester: Summer 2025 **
** AI Use: Checked for spelling errors and assisted with understanding the questions that were asked. **

** This application simulates a real-time onboard system for a small satellite or space probe. The blinking LED represents a beacon or heartbeat indicator, confirming that the satellite is operational. **
** The solar sensor task mimics a subsystem monitoring light intensity to detect eclipse events or solar panel exposure, which is critical for managing power in orbit. Periodic telemetry messages simulate status updates sent to ground control. **
** When the system receives an external trigger (e.g., button press), it compresses and transmits a summary of logged sensor data, as if responding to a ground command. All tasks are carefully prioritized to reflect real-world satellite constraints, ensuring power-critical functions preempt visual indicators and logging operations. **
** This application demonstrates core real-time principles within a space mission context. **

** 1. Polling vs. Interrupt **

Using an ISR with a semaphore is more efficient and responsive than polling because it avoids constantly checking the button in a tight loop, which wastes CPU cycles and increases task latency. 
With polling, the logger task would need to run repeatedly just to check if the button was pressed, delaying more important work. Our design instead uses an interrupt to signal exactly when the button is pressed, immediately waking the logger task with no wasted effort. 
This eliminates the delay between button press and log dump and reduces power usage and CPU load, which is especially useful in embedded systems.

** 2. ISR Design ** 

We use the special FromISR versions of FreeRTOS APIs in ISRs because regular APIs like xSemaphoreTake are blocking calls and not safe to use in an interrupt context. 
Blocking inside an ISR would freeze the system since ISRs aren't meant to be paused or delayed. The FromISR APIs are non-blocking and specifically designed to safely interact with the scheduler from interrupt context. 
If we used the normal versions, we could cause undefined behavior, break the scheduler, or deadlock the system.


** 3. Real-Time Behavior **

 If the sensor task is running when the button is pressed, the ISR fires immediately and gives the semaphore using xSemaphoreGiveFromISR. 
 The flag xHigherPriorityTaskWoken is checked to see if a higher-priority task (like the logger) was unblocked by this. If so, portYIELD_FROM_ISR triggers a context switch after the ISR finishes. 
 However, because the sensor task has a priority of 3 and the logger task is priority 4, the logger will preempt the sensor task only if the sensor task enters a blocking state (like during its vTaskDelayUntil). 
 If the sensor task is actively running and not yielding, the logger has to wait its turn. This shows how real-time responsiveness still respects task boundaries and preemption rules.

 ** 4. Core Affinity **

 Pinning all tasks to Core 1 ensures consistent behavior and timing. If tasks weren't pinned, the ISR (which always runs on Core 0) could unpause a task that the scheduler places on Core 1, introducing timing variations or race conditions depending on which core picks up the task first. 
 This could make it harder to predict when the logger task actually runs after a button press. By using core affinity, we kept everything on one core, making it easier to understand the sequence of events and ensuring more deterministic scheduling, which is important in real-time systems.

 ** 5. Light Sensor Logging **

 To safely share data between the sensor task and the logger task, I used a mutex (xLogMutex) to protect access to the shared buffer. This prevents the two tasks from accessing the buffer at the same time, which could cause data corruption. 
 Without the mutex, if the logger reads the buffer while the sensor is in the middle of writing to it, we might get incomplete or invalid values. If the logger task runs at a higher priority and preempts the sensor task mid-write, the mutex blocks it until the sensor is done. 
 In more complex systems, a better solution might be a double-buffering approach to avoid delays.

 ** 6. Task Priorities ** 

 If we accidentally gave the logger task a lower priority (e.g., 1) and the blink task a higher priority (e.g., 3), the logger wouldn’t run right away when the button is pressed. Instead, lower-priority tasks like the logger would be delayed until the higher-priority blink and telemetry tasks are done. 
 This could delay the log dump, which defeats the purpose of fast reaction to button input. This ties into preemptive scheduling and shows a form of priority inversion, where a less important task (like the LED blink) prevents a more urgent one (like logging) from running. The correct priority order ensures responsiveness.

** 7. Resource Usage ** 
We defer the log compression work to a task instead of handling it in the ISR because long operations in ISRs can cause problems. First, long ISRs can delay other interrupts from firing, increasing system latency. Second, they can interfere with the scheduler tick, which might prevent other time-sensitive tasks from being scheduled properly. 
In this lab, keeping the ISR short allowed us to maintain responsiveness in the sensor and telemetry tasks, while still reacting quickly to the button press by delegating the heavy lifting to the logger task.

** 8. Chapter Connections ** 

In Mastering the FreeRTOS Kernel Section 7.4 "Binary Semaphores Used for Synchronization", it says:

“The interrupt safe version of the Binary Semaphore API can be used to unblock a task each time a particular interrupt occurs, effectively synchronizing the task with the interrupt... This allows the majority of the interrupt event processing to be implemented within the synchronized task, with only a very fast and short portion remaining directly in the ISR.”

That’s exactly how I implemented my button-activated logger. Instead of doing the compression in the ISR, I used xSemaphoreGiveFromISR() to signal the logger task and used portYIELD_FROM_ISR() to let the scheduler run it if needed. This kept my ISR short and let the logger handle the heavier work.

The section also explains that the deferred task can be given a higher priority so it “pre-empts the other tasks in the system.” I set my LoggerTask to priority 4 for this reason, so it can run as soon as the ISR fires. Overall, Section 7.4 guided how I structured the ISR-to-task communication and priority system in my app.






