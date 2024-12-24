# ESC Control System with Temperature Monitoring and Power Limitation

## Description

This project involves an **ESC (Electronic Speed Controller)** control system that manages the throttle of a motor based on potentiometer input. It also includes a **temperature monitoring system** using the **DHT11 sensor** to ensure safe operation. The system has built-in safety features to protect against overheating and power limitations:

- **Overheat Protection**: If the temperature exceeds 99°C for more than 1 second, the system shuts down the ESC for 15 seconds to prevent damage. After the cooldown period, the system resumes operation.
- **Power Limitation**: High power is only allowed after a 1.5-second delay when the potentiometer is set above 50%.
- **OLED Display**: Displays real-time information about throttle percentage, voltage, battery capacity, temperature, and system status.

### Features:
- **ESC Control**: Adjusts the throttle based on potentiometer input.
- **Temperature Monitoring**: Monitors temperature continuously with the DHT11 sensor.
- **Overheat Protection**: Automatically shuts down the ESC if the temperature exceeds 99°C for more than 1 second, with a 15-second cooldown.
- **Power Limitation**: Limits power to 50% unless the potentiometer is held above 50% for 1.5 seconds.
- **OLED Display**: Displays throttle percentage, voltage, battery capacity, and temperature.
- **Debouncing**: Prevents rapid toggling of shutdown states due to brief temperature fluctuations.

---

## Components Used:
- **Arduino (or compatible microcontroller)**
- **DHT11 Temperature and Humidity Sensor**: This sensor is used because it was the only one available at the time. (Yes, I’m too lazy to purchase a better one!)
- **Potentiometer**
- **ESC (Electronic Speed Controller)**
- **OLED Display (SSD1306)**
- **2S High Ampere 30A Battery**: Provides power to the system and the connected motor.
- **130k BLDC Fan**: A 130,000 RPM brushless DC fan used for cooling and airflow.

---

## Wiring Diagram:
- **DHT11**: Connected to pin `D2` (Data)
- **Potentiometer**: Connected to analog pin `A0`
- **Voltage Sensor**: Connected to analog pin `A1`
- **ESC**: Connected to pin `D3` (PWM Output)
- **OLED Display**: Connected via I2C interface
- **2S 30A Battery**: Powers the system, connected to the ESC and motor
- **130k BLDC Fan**: Connected to ESC for motor control

## How to Use:
1. Power on the system using the 2S 30A battery.
2. Adjust the potentiometer to control the throttle.
3. The system will continuously monitor the temperature and display relevant information on the OLED screen.
4. If the temperature exceeds **99°C** for more than **1 second**, the system will shut down the ESC and display a warning message: 
   - **"System Shutdown: Overheat Detected, Temp is 99°C"**.
5. After **15 seconds**, the system will automatically resume operation.
6. Throttle control will be limited to **50%** unless the potentiometer is held above **50%** for at least **1.5 seconds**.
7. The system will display **Throttle**, **Voltage**, **Battery Capacity**, and **Temperature** on the OLED screen in real-time.
8. If the temperature fluctuates around the thresholds (**90°C** or **99°C**), the system will debounce and avoid repeated shutdowns or restarts.
