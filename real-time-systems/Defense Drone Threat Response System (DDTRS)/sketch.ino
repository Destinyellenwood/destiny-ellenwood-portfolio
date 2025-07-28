/* -------------------------------------------------------------------
   Destiny Ellenwood
   Application 6: Defense Drone Threat Response System (DDTRS)
   UCF - Real Time Systems - Su 2025
   Company Context: Raytheon Autonomous Systems Division
   Mission: Detect flash-based threats, respond autonomously, and allow
   real-time operator override via button and web-based HTTP interface.


   http://localhost:9080
   ------------------------------------------------------------------- */

#include <WiFi.h>
#include <WebServer.h>
#include <uri/UriBraces.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


#define ALERT_LED_PIN 15       // Red LED for threat alert
#define HEARTBEAT_LED_PIN 2    // Green LED for drone heartbeat
#define LDR_PIN 32             // Analog pin for LDR
#define BUTTON_PIN 4           // Pushbutton for operator override
#define VREF 3.3
#define RFIXED 10000.0
#define GAMMA 0.7
#define SENSOR_THRESHOLD 200   // Lux threshold to trigger alert

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

#define ISR_BUTTON_PIN 27  // Pushbutton for ISR system override trigger
volatile bool isr_triggered = false;



WebServer server(80);
SemaphoreHandle_t xSensorSem;
SemaphoreHandle_t xButtonSem;
SemaphoreHandle_t xLogMutex;
SemaphoreHandle_t xISROverrideSem;
int current_lux = 0;
bool drone_mode_safe = false;
bool threat_detected = false;
bool self_destruct_requested = false;


void IRAM_ATTR handle_isr_button() {
  isr_triggered = true;
}


// ----------------------------- Tasks ---------------------------------
void heartbeat_task(void *pvParameters) {  // Soft Real-Time Task: Heartbeat (1Hz)
  bool on = false;
  while (1) {
    digitalWrite(HEARTBEAT_LED_PIN, on);
    on = !on;
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void sensor_task(void *pvParameters) {  // Hard Real-Time Task: Sensor sampling + threat detection
  TickType_t lastWake = xTaskGetTickCount();
  while (1) {
    int raw = analogRead(LDR_PIN);
    float V = (raw / 4095.0) * VREF;
    float R = (V / (VREF - V)) * RFIXED;
    float lux = pow(50 * 1e3 * pow(10, GAMMA) / R, (1 / GAMMA));
    current_lux = (int)lux;

    if (lux < SENSOR_THRESHOLD) {
      xSemaphoreGive(xSensorSem);
    }

    xSemaphoreTake(xLogMutex, portMAX_DELAY);
    Serial.printf("[Sensor] Lux = %d\n", current_lux);
    xSemaphoreGive(xLogMutex);

    vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(500));
  }
}

void response_task(void *pvParameters) {  // Hard Real-Time Task: Respond to threats or override
  while (1) {
    // Threat detection from sensor
    if (xSemaphoreTake(xSensorSem, pdMS_TO_TICKS(100)) == pdTRUE) {
      threat_detected = true;
      xSemaphoreTake(xLogMutex, portMAX_DELAY);
      Serial.printf("⚠️ THREAT DETECTED! Lux = %d\n", current_lux);
      xSemaphoreGive(xLogMutex);

      digitalWrite(ALERT_LED_PIN, HIGH);
      vTaskDelay(pdMS_TO_TICKS(300));
      digitalWrite(ALERT_LED_PIN, LOW);
    }

    // Manual toggle via button (S1)
    if (xSemaphoreTake(xButtonSem, pdMS_TO_TICKS(10)) == pdTRUE) {
      drone_mode_safe = !drone_mode_safe;
      xSemaphoreTake(xLogMutex, portMAX_DELAY);
      Serial.printf("🚨 Operator Toggle: MODE = %s\n", drone_mode_safe ? "SAFE" : "NORMAL");
      xSemaphoreGive(xLogMutex);
    }

    // Emergency ISR override via BTN2
    if (xSemaphoreTake(xISROverrideSem, pdMS_TO_TICKS(10)) == pdTRUE) {
      drone_mode_safe = true;  // Force to SAFE mode
      xSemaphoreTake(xLogMutex, portMAX_DELAY);
      Serial.println("🚨 [ISR] Emergency override triggered via ISR button!");
      Serial.println("🔒 Switching to SAFE mode");
      xSemaphoreGive(xLogMutex);

      // Optional visual feedback
      digitalWrite(HEARTBEAT_LED_PIN, HIGH);
      digitalWrite(ALERT_LED_PIN, HIGH);
      vTaskDelay(pdMS_TO_TICKS(300));
      digitalWrite(HEARTBEAT_LED_PIN, LOW);
      digitalWrite(ALERT_LED_PIN, LOW);
    }
  }
}


void button_watch_task(void *pvParameters) {  // Hard Real-Time Task: Monitor pushbutton input
  int last = 1;
  TickType_t debounce = 0;
  while (1) {
    int state = digitalRead(BUTTON_PIN);
    if (state == LOW && last == HIGH) {
      TickType_t now = xTaskGetTickCount();
      if ((now - debounce) > pdMS_TO_TICKS(50)) {
        debounce = now;
        xSemaphoreGive(xButtonSem);
      }
    }
    last = state;
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void sendHtml() {  // Web HTML page response (Raytheon-themed + animated buttons) - Chatgpt assisted in modifying : https://chatgpt.com/share/68869e5e-17e8-8000-9355-177c78a31449
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <style>
      body {
        background-color: #1a1a1a;
        color: #ffffff;
        font-family: Arial, sans-serif;
        text-align: center;
        padding: 30px;
      }

      h2 {
        color: #d6001c;  /* Raytheon Red */
      }

      p {
        font-size: 18px;
        margin: 10px 0;
      }

      a {
        display: inline-block;
        margin: 12px;
        padding: 12px 24px;
        background-color: #d6001c;
        color: #ffffff;
        text-decoration: none;
        border-radius: 6px;
        font-weight: bold;
        transition: background-color 0.3s, box-shadow 0.3s;
        animation: pulse 2s infinite;
      }

      a:hover {
        background-color: #a00016;
        box-shadow: 0 0 12px #d6001c;
      }

      @keyframes pulse {
        0% { box-shadow: 0 0 0px #d6001c; }
        50% { box-shadow: 0 0 12px #d6001c; }
        100% { box-shadow: 0 0 0px #d6001c; }
      }
    </style>
  </head>
  <body>
    <h2>Defense Drone Interface</h2>
    <p>Lux: LUX_VAL</p>
    <p>Mode: MODE_VAL</p>
    <p><a href='/toggle/1'>Return to Base</a></p>
    <p><a href='/toggle/2'>Self-Destruct</a></p>
  </body>
  </html>)rawliteral";

  html.replace("LUX_VAL", String(current_lux));
  html.replace("MODE_VAL", drone_mode_safe ? "SAFE" : "NORMAL");
  server.send(200, "text/html", html);
}





void wifi_server_task(void *pvParameters) {  // Soft Real-Time Task: HTTP Web Interface
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(pdMS_TO_TICKS(250));
  }

  xSemaphoreTake(xLogMutex, portMAX_DELAY);
  Serial.printf("WiFi connected. IP: %s\n", WiFi.localIP().toString().c_str());
  xSemaphoreGive(xLogMutex);

  server.on("/", sendHtml);
  server.on(UriBraces("/toggle/{}"), []() {
    int action = server.pathArg(0).toInt();
    
    xSemaphoreTake(xLogMutex, portMAX_DELAY);
    if (action == 1) {
      drone_mode_safe = true;
      Serial.println("[HTTP] Operator toggled Return to Base");
    } else if (action == 2) {
      drone_mode_safe = false;
      Serial.println("[HTTP] Operator triggered Self-Destruct");
      self_destruct_requested = true;
    }
    xSemaphoreGive(xLogMutex);

    sendHtml();
  });

  server.begin();
  while (1) {
    server.handleClient();
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void self_destruct_task(void *pvParameters) { // Soft Real-Time Task - Responds to wifi server task
  while (1) {
    if (self_destruct_requested) {
      xSemaphoreTake(xLogMutex, portMAX_DELAY);
      Serial.println(">>> Drone self-destructing in:");
      xSemaphoreGive(xLogMutex);

      for (int i = 3; i >= 1; i--) {
        xSemaphoreTake(xLogMutex, portMAX_DELAY);
        Serial.printf("    %d...\n", i);
        xSemaphoreGive(xLogMutex);

        digitalWrite(ALERT_LED_PIN, HIGH);
        vTaskDelay(pdMS_TO_TICKS(250));
        digitalWrite(ALERT_LED_PIN, LOW);
        vTaskDelay(pdMS_TO_TICKS(750));
      }

      xSemaphoreTake(xLogMutex, portMAX_DELAY);
      Serial.println("💥 SYSTEM TERMINATED 💥");
      xSemaphoreGive(xLogMutex);

      self_destruct_requested = false;
    }

    vTaskDelay(pdMS_TO_TICKS(50));  // Poll every 50ms
  }
}


void isr_response_task(void *pvParameters) {  // Hard Real-Time Task: ISR-triggered response - Chatgpt assisted : https://chatgpt.com/share/68869e5e-17e8-8000-9355-177c78a31449
  while (1) {
    if (isr_triggered) {
      isr_triggered = false;

      xSemaphoreTake(xLogMutex, portMAX_DELAY);
      Serial.println("[ISR] Mission Override Triggered from ISR Button!");
      xSemaphoreGive(xLogMutex);

      
      digitalWrite(ALERT_LED_PIN, HIGH);
      vTaskDelay(pdMS_TO_TICKS(200));
      digitalWrite(ALERT_LED_PIN, LOW);
    }

    vTaskDelay(pdMS_TO_TICKS(10));  // Polling interval
  }
}

void IRAM_ATTR isr_override() { // Chatgpt assisted : https://chatgpt.com/share/68869e5e-17e8-8000-9355-177c78a31449
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(xISROverrideSem, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void setup() {
  Serial.begin(115200);

  Serial.println("========================================");
  Serial.println("Defense Drone Threat Response System");
  Serial.println("Autonomous Mode Booting...");
  Serial.println("Press the BLUE button to toggle SAFE MODE.");
  Serial.println("Once in SAFE MODE, mission operations will begin.");
  Serial.println("========================================");

  pinMode(ALERT_LED_PIN, OUTPUT);
  pinMode(HEARTBEAT_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(ISR_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ISR_BUTTON_PIN), isr_override, FALLING);

   // Wait here until operator presses the button to toggle SAFE MODE
  while (!drone_mode_safe) {
    if (digitalRead(BUTTON_PIN) == LOW) {
      delay(50); // debounce
      if (digitalRead(BUTTON_PIN) == LOW) {
        drone_mode_safe = true;
        Serial.println(" SAFE MODE ENGAGED! Initializing system tasks...");
        break;
      }
    }
    delay(10);
  }

  xISROverrideSem = xSemaphoreCreateBinary();
  xSensorSem = xSemaphoreCreateCounting(10, 0);
  xButtonSem = xSemaphoreCreateBinary();
  xLogMutex = xSemaphoreCreateMutex();

  xTaskCreate(heartbeat_task, "Heartbeat", 2048, NULL, 1, NULL);
  xTaskCreate(sensor_task, "Sensor", 4096, NULL, 3, NULL);
  xTaskCreate(response_task, "Response", 4096, NULL, 4, NULL);
  xTaskCreate(button_watch_task, "Button", 2048, NULL, 3, NULL);
  xTaskCreate(wifi_server_task, "WebServer", 4096, NULL, 2, NULL);
  xTaskCreate(self_destruct_task, "SelfDestruct", 2048, NULL, 2, NULL);

}

void loop() {
  // Required in Arduino even if not used
}
