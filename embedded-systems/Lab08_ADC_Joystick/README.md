# Lab 08 – Analog-to-Digital Conversion with Joystick Input (MSP430FR6989)

## Overview
This lab demonstrates how to use the **ADC12_B** (successive approximation register-type) module on the MSP430FR6989 to sample analog voltage signals from a joystick’s X and Y axes. The digitized values are transmitted to a UART terminal at 9600 baud. Two programs were developed:

- **8.1**: Read and transmit only the X-axis ADC value.
- **8.2**: Read both X and Y values in sequence using the ADC’s multi-channel configuration.

---

## Key Concepts
- Successive Approximation ADC (SAR-type) operation
- Sample-and-hold time calculation using worst-case R-C values
- ADC12_B register configuration:
  - Resolution selection (12-bit)
  - Channel selection (`A10` for X, `A4` for Y)
  - Voltage references (VR+ = AVCC, VR− = AVSS)
  - Sequenced channel conversion using `ADC12CONSEQ_1`
- UART transmission of formatted ADC results
- Real-time ADC-to-UART loop for sensor monitoring

---

## Subprojects

| Subfolder | Description |
|-----------|-------------|
| [8.1_ADC_X_Only](./8.1_ADC_X_Only/) | Reads and transmits X-axis joystick data (A10 / P9.2) |
| [8.2_ADC_XY_Readings](./8.2_ADC_XY_Readings/) | Reads and transmits both X (A10) and Y (A4) joystick data using channel sequencing |

---

## Hardware
- **Joystick X-Axis**: A10 / P9.2  
- **Joystick Y-Axis**: A4 / P8.7  
- **UART TX**: P3.4  
- **UART RX**: P3.5  
- **Red LED**: P1.0 (blinks between readings)

---

## UART Output Example
Joystick X-Axis ADC Value: 2017
Joystick Y-Axis ADC Value: 1982

---

## Skills Demonstrated
- ADC initialization and sampling control
- Channel multiplexing and end-of-sequence detection
- UART driver functions for character and integer output
- Delay-based sampling loop for real-time user feedback

---

This lab showcases low-level sensor integration and data handling — a crucial skill for embedded systems engineers working with analog inputs and microcontroller peripherals.
