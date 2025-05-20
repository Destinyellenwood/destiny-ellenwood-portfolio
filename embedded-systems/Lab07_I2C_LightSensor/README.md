# Lab 07 – I2C Communication with OPT3001 Light Sensor (MSP430FR6989)

## Overview
This lab demonstrates how to interface with the **OPT3001 ambient light sensor** over the I2C bus using the MSP430FR6989. The lab consists of two parts:
- Reading static data (Manufacturer and Device IDs)
- Configuring the sensor and retrieving real-time lux measurements

All output is sent to a terminal using UART, allowing for easy testing and verification of sensor communication.

---

## Key Concepts
- I2C master-mode communication using `eUSCI_B1`
- Multi-byte register reads and writes using start/restart/stop conditions
- Register mapping for sensor data access
- UART output for debugging and visualization
- Lux data conversion from raw I2C readings
- Use of SMCLK for I2C clock generation at 125 kHz

---

## Subprojects

| Subfolder | Description |
|-----------|-------------|
| [7.1_Read_Sensor_IDs](./7.1_Read_Sensor_IDs/) | Reads Manufacturer ID (0x7E) and Device ID (0x7F) from OPT3001 |
| [7.2_Read_Light_Data](./7.2_Read_Light_Data/) | Configures the sensor and continuously reads lux measurements |

---

## Hardware
- **I2C Sensor**: OPT3001 (on BoosterPack)
- **I2C Pins**: P4.1 (SDA), P4.0 (SCL)
- **UART Pins**: P3.4 (TX), P3.5 (RX)
- **Red LED**: P1.0 (used for heartbeat/blink feedback)

---

## Skills Demonstrated
- Multi-protocol integration (I2C + UART)
- Register-level I2C programming
- Real-time data acquisition and formatting
- Use of helper functions for 16-bit word transfer
- Working with sensor datasheets and addressing

---

## Example UART Output

OPT3001 Light Sensor Test
Manufacturer ID: 0x5449
Device ID: 0x3001

Reading #1: Lux = 105
Reading #2: Lux = 119


---

This lab represents a full example of real-world embedded communication — setting up I2C peripherals, reading sensor registers, and outputting human-readable values.

