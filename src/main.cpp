#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include<Arduino.h>
#include  "env.h"
#include  "secrets.h"


// Define the pins for the LEDs

const int led1_pin = 12;
const int led2_pin = 14;
const int led3_pin = 27;

bool states[8][3] = {
  {false, false, false},
  {false, false, true},
  {false, true, false},
  {false, true, true},
  {true, false, false},
  {true, false, true},
  {true, true, false},
  {false, false, false }
};

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
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

}

void loop() {
  // Define the JSON payload
  DynamicJsonDocument payload(200);

  for (int i = 0; i < 8; i++) {
    payload["light_switch_1"] = states[i][0];
    payload["light_switch_2"] = states[i][1];
    payload["light_switch_3"] = states[i][2];

    // Serialize the payload
    String payload_str;
    serializeJson(payload, payload_str);

    HTTPClient http;
    http.begin(api_url);
    http.addHeader("X-API-Key", api_key);
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.PUT(payload_str);
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Request successful");
    } else {
      Serial.println("HTTP request failed");
    }

    http.end();
    delay(2000);
  }
}