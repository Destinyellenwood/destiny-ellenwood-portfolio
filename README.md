# Embedded & Real-Time Systems Projects

Welcome! This repository showcases a collection of hands-on **embedded systems** and **real-time operating systems (RTOS)** projects completed using the **MSP430FR6989** and **ESP32** microcontrollers. These projects demonstrate direct control of hardware through C programming and real-time task management using FreeRTOS.

> 🎓 Developed as part of coursework at the University of Central Florida (UCF), these labs and applications cover GPIO, ADC, UART, SPI, I2C, LCD control, RTOS multitasking, timers, and more.

---

## 🔧 Repository Structure

### 🟢 [`embedded-systems/`](./embedded-systems/)
Covers 9 labs using the MSP430FR6989 covering:

| Lab | Topic | Core Features |
|-----|-------|---------------|
| [Lab 1](./embedded-systems/Lab01_LED_Blink) | LED Blinking | GPIO, delay loops |
| [Lab 2](./embedded-systems/Lab02_Button_Interrupts) | Buttons | Polling & interrupts |
| [Lab 3](./embedded-systems/Lab03_Timer_LED_Control) | Timers | Continuous/up mode |
| [Lab 4](./embedded-systems/Lab04_Interrupts_and_LowPower) | Interrupts + LPM | ISRs, FSM, power modes |
| [Lab 5](./embedded-systems/Lab05_LCD_Display) | LCD UI | 5-digit LCD control, timer counter |
| [Lab 6](./embedded-systems/Lab06_UART_Communication) | UART | TX/RX, formatted messages |
| [Lab 7](./embedded-systems/Lab07_I2C_LightSensor) | I2C + OPT3001 | Sensor config, lux reading |
| [Lab 8](./embedded-systems/Lab08_ADC_Joystick) | ADC | Analog joystick interface |
| [Lab 9](./embedded-systems/Lab09_SPI_LCD_Display) | SPI + Graphics | LCD display with shapes and logo |

Each lab folder includes:
- 📄 `README.md` documentation
- 💾 `main.c` source code
- 🔁 UART/LCD/ADC/SPI integrations
- 🔋 Low-power optimization

---

### 🔴 [`real-time-systems/`](./real-time-systems/)
RTOS-based applications on the ESP32 using **FreeRTOS**, simulated on Wokwi.

| Project | Description |
|---------|-------------|
| `Application1_LED_Blinking` | Runs a periodic LED toggle task and UART telemetry task |
| `Application2_UART_Telemetry` | Adds UART logging using `vTaskDelayUntil()` |

These projects highlight:
- `xTaskCreate()`, `vTaskDelay()`, `vTaskDelayUntil()`
- Task prioritization and scheduling
- Cooperative multitasking with predictable timing
- GPIO + UART task separation

---

## 💡 Skills Demonstrated

- 🧠 **Microcontroller Configuration**
  - GPIO, ADC12_B, eUSCI UART/I2C/SPI modules
- 📟 **Peripheral Communication**
  - LCD display, light sensor (OPT3001), joystick
- 💬 **Serial Protocols**
  - UART (TX/RX), I2C, SPI (LCD graphics)
- 📈 **Real-Time Task Management**
  - FreeRTOS on ESP32 with Wokwi simulation
- ⚡ **Low Power Modes**
  - LPM3 with interrupts and event-driven wake-up
- 🎨 **User Interfaces**
  - LCD digit display, graphics library (grlib), live counters

---

## 🛠️ Tools & Technologies

- **Code Composer Studio** (MSP430 projects)
- **PlatformIO + VSCode** (ESP32 / FreeRTOS)
- **Wokwi** (ESP32 simulation)
- **TI grlib** (LCD graphics)
- **C programming** (no OS abstraction)

---

## 📷 Preview

<img src="./embedded-systems/Lab09_SPI_LCD_Display/assets/snoopy image.png" alt="LCD Graphics Demo" width="500"/>

---

## 👨‍💻 Author

**Destiny Ellenwood**  
📍 Computer Engineering | University of Central Florida  
🔗 [LinkedIn]([https://www.linkedin.com/in/destinyellenwood/]) 
---

## 📜 License

MIT License — free to use, learn from, and build upon.

