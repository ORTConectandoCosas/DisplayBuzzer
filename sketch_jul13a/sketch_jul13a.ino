#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID "Guri's AirPort Express"
#define WIFI_PASSWORD "guriguri"
#define TOKEN "45SKm5WY4RQIsmqVE72P"
#define CLIENT_NAME "ESP8266 limpieza"

char* thingsboardServer = "demo.thingsboard.io";
char* requestTopic = "v1/devices/me/rpc/request/+";

LiquidCrystal_I2C lcd(0x27, 16, 1);
WiFiClient wifiClient;
PubSubClient client(wifiClient);

int switchPin = 14;       
int ledPin = 2;
int buzzerPin = 16;
              
int switchStatusLast = HIGH;  
int switchStatus;
int ledStatus = LOW;         

void setup() {
  Serial.begin(9600);
  initializeDisplay();
  initializePins();
  digitalWrite(buzzerPin, HIGH);
  client.setServer(thingsboardServer, 1883);
  client.setCallback(on_message);
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void noAlertState()
{
  digitalWrite(buzzerPin, HIGH);
  lcd.clear();
  lcd.noBacklight();
}

void alertState(){
  bool soundState = false;
  int messageLength = 36;
  lcd.on();
  lcd.backlight();
  lcd.home();
  lcd.print("                LIMPIEZA SOLICITADA!");
  while (!isButtonPressed()) {
    if (soundState) {
      digitalWrite(buzzerPin, LOW);
    } else {
      digitalWrite(buzzerPin, HIGH);
    }
    soundState = !soundState;
    lcd.scrollDisplayLeft();
    delay(350);
  }
  noAlertState();
}

bool isButtonPressed() {
  switchStatus = digitalRead(switchPin);
  if (switchStatus != switchStatusLast) {
      delay(50); 
      int switchStatus = digitalRead(switchPin); 
       if (switchStatus != switchStatusLast) {
        ledStatus = !ledStatus;
        digitalWrite(ledPin, ledStatus);
        switchStatus = switchStatusLast;
        return true;
     }
  }
  return false;
}

void on_message(const char* topic, byte* payload, unsigned int length) {
  Serial.print("llega");
//   StaticJsonDocument<256> doc;
//   deserializeJson(doc, payload);
//   String message = doc["message"];
   alertState();
//   Serial.print(message);
}

void reconnect() {
   while (!client.connected()) {
    initializeWifi();
    Serial.print("Connecting to ThingsBoard node ...");
    if (client.connect(CLIENT_NAME, TOKEN, NULL)) {
      Serial.println( "[DONE]" );
      client.subscribe(requestTopic);
    } else {
      Serial.print("[FAILED]");
      Serial.println(" retrying in 5 seconds..." );
      delay(5000);
    }
  }
}

void initializePins() {
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
}

void initializeWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void initializeDisplay() {
  Wire.begin(4, 5);
  lcd.init();
  lcd.noBacklight();
}
