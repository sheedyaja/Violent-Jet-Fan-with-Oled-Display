#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// OLED Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define SSD1306_I2C_ADDRESS 0x3C  // Define the I2C address for the OLED display

// DHT11 Settings
#define DHTPIN 2      // DHT11 data pin connected to D2
#define DHTTYPE DHT11 // DHT11 sensor type
DHT dht(DHTPIN, DHTTYPE);

// Voltage Sensor Settings
#define VOLTAGE_PIN A1 // Voltage sensor output connected to A1
float voltage = 0.0;
float maxVoltage = 8.2; // Maximum battery voltage
float minVoltage = 6.6; // Minimum battery voltage (cutoff)

// Potentiometer Settings
#define POT_PIN A0 // Potentiometer connected to A0

// ESC Control Pin
#define ESC_PIN 9  // Define the ESC control pin (e.g., pin 9)

// Timer for High Power
unsigned long highPowerStart = 0;
bool highPowerAllowed = false;
unsigned long stopSignalStart = 0;
bool signalStopped = false;
unsigned long stopDuration = 5000; // 5 seconds stop duration
unsigned long shutdownStart = 0;
bool systemShutdown = false;
unsigned long shutdownDuration = 15000; // 15 seconds shutdown duration

// Temperature Thresholds
#define TEMP_THRESHOLD 90.0
#define TEMP_SHUTDOWN_THRESHOLD 99.0

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Initialize DHT Sensor
  dht.begin();

  // Initialize OLED
  if (!display.begin(SSD1306_I2C_ADDRESS, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();

  // Set ESC_PIN as output
  pinMode(ESC_PIN, OUTPUT);
}

void loop() {
  // Read potentiometer value
  int potValue = analogRead(POT_PIN);
  int powerPercent = map(potValue, 0, 1023, 0, 100);

  // Read voltage from sensor
  int sensorValue = analogRead(VOLTAGE_PIN);
  voltage = (sensorValue * maxVoltage) / 1023.0; // Scale the reading to the sensor's max voltage

  // Calculate battery capacity
  int batteryCapacity = map(voltage * 100, minVoltage * 100, maxVoltage * 100, 0, 100);
  if (batteryCapacity < 0) batteryCapacity = 0;
  if (batteryCapacity > 100) batteryCapacity = 100;

  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Check if temperature exceeds 99°C (System Shutdown)
  if (temperature >= TEMP_SHUTDOWN_THRESHOLD) {
    if (!systemShutdown) {
      shutdownStart = millis(); // Start the shutdown timer
      systemShutdown = true; // Set the system to shutdown state
    }

    if (millis() - shutdownStart < shutdownDuration) {
      // Display shutdown message and stop the ESC signal
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("System Shutdown!");
      display.setCursor(0, 20);
      display.print("Overheat detected");
      display.setCursor(0, 40);
      display.print("Temp: ");
      display.print(temperature);
      display.print(" C");
      display.display();

      // Stop the motor by setting the ESC signal to 0
      analogWrite(ESC_PIN, 0);
    } else {
      // After 15 seconds, resume normal operation
      systemShutdown = false;
    }
  }
  // Check if temperature exceeds 90°C (Signal Stop)
  else if (temperature >= TEMP_THRESHOLD) {
    // Stop the potentiometer signal for 5 seconds
    if (!signalStopped) {
      stopSignalStart = millis(); // Start the stop timer
      signalStopped = true; // Ensure the stop action is only executed once
    }

    if (millis() - stopSignalStart < stopDuration) {
      // Display warning and stop the signal to ESC
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Overheat! Temp: ");
      display.print(temperature);
      display.print(" C");
      display.setCursor(0, 20);
      display.print("ESC is OFF");
      display.display();

      // Stop the motor by setting the ESC signal to 0
      analogWrite(ESC_PIN, 0);
    } else {
      // Resume normal operation after 5 seconds
      signalStopped = false;
    }
  } else {
    // If temperature is below 90°C, send the potentiometer signal to ESC
    if (!signalStopped && !systemShutdown) {
      analogWrite(ESC_PIN, map(powerPercent, 0, 100, 0, 255));
    }
  }

  // Display data on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Sensor Readings:");
  display.setCursor(0, 15);
  display.print("Throttle: ");
  display.print(powerPercent);
  display.print("%");
  display.setCursor(0, 30);
  display.print("Voltage: ");
  display.print(voltage, 2);
  display.print(" V");
  display.setCursor(0, 45);
  display.print("Battery: ");
  display.print(batteryCapacity);
  display.print(" %");
  display.setCursor(0, 55);
  display.print("Temp: ");
  display.print(temperature, 1);
  display.print(" C");

  display.display();

  // Debugging in Serial Monitor
  Serial.print("Throttle: ");
  Serial.print(powerPercent);
  Serial.print("% | Voltage: ");
  Serial.print(voltage, 2);
  Serial.print(" V | Battery: ");
  Serial.print(batteryCapacity);
  Serial.print("% | Temp: ");
  Serial.print(temperature, 1);
  Serial.println(" C");

  delay(500); // Delay for stability
}
