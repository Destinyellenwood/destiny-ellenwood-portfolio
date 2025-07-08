** Student: Destiny Ellenwood **
** UCF ID: 5276398 **
** Course: EEE4775 – Real-Time Systems **
** Semester: Summer 2025 **
** AI Use: Checked for spelling errors and assisted with understanding the questions that were asked. **


** 1. Task Timing and Jitter **

For the sensor task, using vTaskDelayUntil, the sensor readings and alert messages consistently printed every 500m, showing a stable and regular period. 
In contrast, over time the LED blink and telemetry messages showed small timing drifts, sometimes off by a few milliseconds. This is expected becase vTaskDelayUntil uses an absolute tick count
as a reference, meaning it always wakes up at fixed intervals. While as vTaskDelay just waits a relative amount of time after the task finishes, meaning if the task was delayed for some reason
(like the sensor task running), that delay gets added to the next cycle. This is what creates a drift, which is noticable over longer durations.
So while drift is acceptable for non-critical tasks like status LEDs or periodic console prints, it's not acceptable for precise sensor sampling, which is why we used vTaskDelayUntil.

** 2. Priority-Based Preemption ** 

FreeRTOS using premptive scheduling, meaning a higher-priority task can interrupt a lower-priority one the moment it becomes ready. In my application, the sensor task has the highest priority at 3, so if it's time
for it to run while the telemetry task (which is at priority 2) is mid-print, the scheduler immediately preempts the print task and runs the sensor code instead.

An example of this is if tick = 5000 ms and the telemetry task is running, but the sensor's 500 ms period has just elapsed, FreeRTOS will switch from the telemetry task to the sensor task to meet its timing.
This is consistent with real-time deadlines, where high-priority periodic tasks are given CPU preference. After the sensor finishes, the telemetry task resumes right where it left off.
Even though I couldn't visually catch this happening in my print log, I know this is the expected scheduler behavior based on how the priority system is designed.

** 3. Effect of Task Execution Time **

 In my application, each task runs quickly and blocks efficiently, so there's no problem. But if the sensor-task took 300 ms to compute each reading, that would eat up a huge chunk of CPU time. If this time ever exceeded its 500 ms period, the task
 would miss its deadline, leading to potential threshold warnings being skipped and starvation of the LED and telemetry tasks. 
 This aligns with the theory in Harder Ch.3 and 6 where a system becomes unscheduable when CPU utilization exceeds 100%, especially if a high-priority task consumes too much time.

 Some options that a system designer could consider if the high-priority task started starving the lower tasks or missin its schedule is optimizing the sensor algorithm to reduce execution time,
 offload heavy computation to a second core since the ESP32 has two, lower the priority of the sensor task and use a hardware interrupt, or increase the period if less frequent sampling is acceptable.

 ** 4. vTaskDelay vs vTaskDelayUntil **

 We used vTaskDelayUntil fpr the sensor task instead of using vTaskDelay because it ensures a consistent periodic execution, regardless of how long the task takes to run. It calculates the next wake time from the original start avoiding drift. If we had used vTaskDelay(500 ms), then 
 small delays would accumulate each cycle, causing noticable drift over time. For the LED task, its okay to use vTaskDelay since a jitter isn't noticable when blinking a light.
 The same applies for the telemetry task, printing doesn't require hard timing either, they're both soft timing taks. 

 ** 5. Thematic Integration Reflection **

 In my Space Systems theme, my application includes tasks for a subsystem for a satellite or space probe.

 The sensor task (High Priority) monitors solar exposure to detect eclipses. If the satellite goes into Earth's shadow, this task must alert the system to switch to battery mode or conserve power. That's why its highest priority since power management is mission-critical.

 The telemetry task (Medium Priority) sends back system status packets to mission control.  Important for diagnostics, bbut its not catastrophic if a message is a few millaseconds late.

 The LED beacon tasj (Low Priority) acts as a visual indicator or health heartbeat for engineers on the ground. Helpful for debugging or external monitoring but okay if slighly delayed.

 ** 6. Bonus ** 

 For the experiment that causes starvation, I added a long for-loop inside the sensor task that runs for a significant amount of time before delaying again. This loop represents a situation where the sensor task is doing a lot of work, like complex calculations or filtering,
 and it doesn't give the CPU time to run the lower-priority tasks like the telemetry or LED blink.

 In RTOS terms and from the textbook, the sensor task stayed in the "running" state the whole time, while the lower-priority tasks were stuck in the "ready" state but never got scheduled. That's because RTOS will always run the higher priority "ready" task. It showed how important it is 
 for high-priority tasks to use vTaskDelayUntil or block themselves so other tasks can run.

 After seeing this behavior, I commented out the loop and went back to the regular sensor task code so that the system runs like it should.