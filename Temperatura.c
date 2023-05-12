#include "DHTesp.h"
#include <LiquidCrystal_I2C.h>
#include "ThingSpeak.h"
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <ThingESP.h>

#define I2C_ADDR 0x27
#define LCD_COLUMNS 16
#define LCD_LINES 2
const int DHT_PIN = 15;
DHTesp dhtSensor;

WiFiClient client;

const char* ssid = "Wokwi-GUEST";
const char* pass = "";


unsigned long myChannelNumber = 2;
const char* server = "api.thingspeak.com";
const char* myWriteApiKey = "F4407RL6BRFXHMXS";

unsigned long lastTime = 0;
unsigned long timerDelay = 30000;


int tempC;
int umidadeT;

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);


void setup() {
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  dhtSensor.getPin();
  delay(10);
  lcd.init();
  lcd.backlight();
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);

WiFi.begin(ssid, pass);
while(WiFi.status() != WL_CONNECTED){
  delay(100);
  Serial.println(".");
}
Serial.println("....");
Serial.println("Wifi Conected");

WiFi.mode(WIFI_STA);

ThingSpeak.begin(client);
}



void loop() {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  tempC = dhtSensor.getTemperature();
  umidadeT = dhtSensor.getHumidity();

  if (data.temperature > 35) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
  if (data.humidity > 70) {
    digitalWrite(14, HIGH);
  } else {
    digitalWrite(14, LOW);
  }
  Serial.println("Temp:" + String(data.temperature, 1) + "C");
  Serial.println("Umidade:" + String(data.humidity, 1) + "%");
  Serial.println("------");
  lcd.setCursor(0, 0);
  lcd.print("Temperatura: " + String(data.temperature, 1) + "\xDF" + "C  ");
  lcd.setCursor(0, 1);
  lcd.print("Umidade: " + String(data.humidity, 1) + "%  ");


  ThingSpeak.setField(1, tempC);
  ThingSpeak.setField(2,umidadeT);

  int x = ThingSpeak.writeFields(myChannelNumber,myWriteApiKey);

}