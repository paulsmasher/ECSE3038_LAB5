#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include<Arduino.h>
#include  "env.h"

const char* serverName = "ecse-three-led-api.onrender.com";
const char* ssid = "ARRIS-356B";
const char* password = "50A5DC7C356B";

const char* api_key = "Norman#3543";
const char* api_url = "https://ecse-three-led-api.onrender.com/api/state";

// Define the pins for the LEDs

const int led1_pin = 12;
const int led2_pin = 14;
const int led3_pin = 27;

void setup() {
  Serial.begin(9600);
  pinMode(led1_pin, OUTPUT);
  pinMode(led2_pin, OUTPUT);
  pinMode(led3_pin, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");

}

void loop() {
  HTTPClient http;
  http.begin(api_url);
  http.addHeader("X-API-Key", api_key);

  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    Serial.println(response);

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
    } else {
      bool led1_state = doc["led1"];
      bool led2_state = doc["led2"];
      bool led3_state = doc["led3"];
      digitalWrite(led1_pin, led1_state ? HIGH : LOW);
      digitalWrite(led2_pin, led2_state ? HIGH : LOW);
      digitalWrite(led3_pin, led3_state ? HIGH : LOW);
    }
  } else {
    Serial.println("HTTP request failed");
  }

  http.end();
  delay(1000);
}