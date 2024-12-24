# Violent-Jet-Fan-with-Oled-Display

Description:
This project involves an ESC (Electronic Speed Controller) control system that is designed to manage the throttle of a motor based on the input from a potentiometer. It also includes a temperature monitoring system using a DHT11 sensor (only own this sensor, so I just use what's available, you can use other sensor) to ensure safe operation. The system shuts down the ESC if the temperature exceeds 99°C for more than 1 second, and resumes operation after 15 seconds to prevent overheating. Additionally, it features power limitation, where high power is only allowed after a 1.5-second delay when the potentiometer is set above 50%.

Features:
ESC Control: Adjusts the throttle based on potentiometer input.
Temperature Monitoring: Continuously monitors the temperature using the DHT11 sensor.
Overheat Protection: If the temperature exceeds 99°C for more than 1 second, the system shuts down the ESC for 15 seconds to prevent overheating.
Power Limitation: High power is allowed only after a 1.5-second delay when the potentiometer is set above 50%.
OLED Display: Displays real-time information about throttle percentage, voltage, battery capacity, temperature, and system status.
Debouncing: Prevents rapid toggling of the shutdown state due to brief temperature fluctuations.
Components Used:
Arduino (or compatible microcontroller)
DHT11 Temperature and Humidity Sensor
Potentiometer
ESC (Electronic Speed Controller)
OLED Display (SSD1306)
Wiring Diagram:
DHT11 connected to pin D2 (Data)
Potentiometer connected to analog pin A0
Voltage Sensor connected to analog pin A1
ESC connected to pin D3 (PWM Output)
OLED Display connected via I2C interface
