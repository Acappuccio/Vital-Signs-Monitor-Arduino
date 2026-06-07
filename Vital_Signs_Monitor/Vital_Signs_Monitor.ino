#include <LiquidCrystal.h>
#include <DHT.h>
#include <PulseSensorPlayground.h>

// LCD 
LiquidCrystal lcd(11,10,5,4,3,2);

// DHT setup
#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Pulse Sensor setup 
const int PulseWire = A0;
const int LED = LED_BUILTIN;
int Threshold = 545;

PulseSensorPlayground pulseSensor;

// Variables stored 
int bpm = 0;
float temperature;
float humidity;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2); 
  dht.begin();

  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED);
  pulseSensor.setThreshold(Threshold);

  if (pulseSensor.begin()) {
    Serial.println("Pulse sensor ready!");
    lcd.print("Sensor Ready");
    delay(2000);
    lcd.clear();
  }
}
void loop() {
  // PULSE SENSOR 
  if (pulseSensor.sawStartOfBeat()) {
    bpm = pulseSensor.getBeatsPerMinute();
  }
  // DHT SENSOR
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  // ERROR CHECK 
  if (isnan(humidity) || isnan(temperature)) {
    lcd.setCursor(0,0);
    lcd.print("Sensor Error   ");
    lcd.setCursor(0,1);
    lcd.print("Check DHT      ");
    delay(1000);
    return;
  }
  // LCD DISPLAY 
  lcd.setCursor(0,0);
  lcd.print("BPM:");
  lcd.print(bpm);
  lcd.print(" T:");
  lcd.print(temperature);
  lcd.print("C ");
  lcd.setCursor(0,1);
  lcd.print("H:");
  lcd.print(humidity);
  lcd.print("%      ");
  // SERIAL 
  Serial.print("BPM: ");
  Serial.print(bpm);
  Serial.print(" Temp: ");
  Serial.print(temperature);
  Serial.print(" Hum: ");
  Serial.println(humidity);

  delay(500);
}