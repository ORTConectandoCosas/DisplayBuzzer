#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID "Guri's AirPort Express"
#define WIFI_PASSWORD "guriguri"
#define TOKEN "45SKm5WY4RQIsmqVE72P"
#define CLIENT_ID "eb4989b0-c606-11ea-bf35-41d006a14e9a"

char* thingsboardServer = "demo.thingsboard.io";
char* requestTopic = "v1/devices/me/rpc/request/+";
char* telemetryTopic = "v1/devices/me/telemetry";

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
  initializeWifi();
  client.setServer(thingsboardServer, 1883);
  client.setCallback(on_message);
  initializePins();
  digitalWrite(buzzerPin, HIGH);
  alertState();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void noAlertState()
{
//  String payload = "{";
//    payload += "\"alertStatus\":"; 
//    payload += false; 
//    payload += "}";
//  char attributes[100];
//  payload.toCharArray(attributes, 100);
//  if (client.publish(telemetryTopic, attributes) == true) {
//    Serial.println("Publicado ok");
//  }
//  Serial.println(attributes);
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
  char json[length + 1];
  strncpy (json, (char*) payload, length);
  json[length] = '\0';
  Serial.print("Message: ");
  Serial.println(json);
}

void reconnect() {
   while (!client.connected()) {
    initializeWifi();
    if (client.connect(CLIENT_ID, TOKEN, NULL)) {
      client.subscribe(requestTopic);
      client.subscribe(telemetryTopic);
    } else {
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
  }
}

void initializeDisplay() {
  Wire.begin(4, 5);
  lcd.init();
  lcd.noBacklight();
}
