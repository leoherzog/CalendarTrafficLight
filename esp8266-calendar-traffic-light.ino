#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

int redPin = 14;
int yellowPin = 12;
int greenPin = 13;

void setup() {

  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  digitalWrite(redPin, HIGH);
  
  Serial.begin(115200);
  WiFi.begin("ssid", "psk");
  
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (digitalRead(yellowPin) == HIGH) {
      digitalWrite(yellowPin, LOW);
    } else {
      digitalWrite(yellowPin, HIGH);
    }
  }
  
  digitalWrite(yellowPin, HIGH);

  Serial.println("Connected:");
  Serial.println(WiFi.localIP());
  
  digitalWrite(greenPin, HIGH);
  
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {
    
    WiFiClientSecure client;
    client.setInsecure();
    
    HTTPClient https;
    https.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    
    if (https.begin(client, "https://script.google.com/macros/s/apps-script-published-web-app-url/exec")) {
      
      int responseCode = https.GET();
      
      if (responseCode > 0) {
        
        String status = https.getString();
        Serial.println("HTTP " + String(responseCode) + ": " + status);
        
        if (status == "g") {
          digitalWrite(redPin, LOW);
          digitalWrite(yellowPin, LOW);
          digitalWrite(greenPin, HIGH);
        } else if (status == "y") {
          digitalWrite(redPin, LOW);
          digitalWrite(yellowPin, HIGH);
          digitalWrite(greenPin, LOW);
        } else if (status == "r") {
          digitalWrite(redPin, HIGH);
          digitalWrite(yellowPin, LOW);
          digitalWrite(greenPin, LOW);
        } else {
          digitalWrite(redPin, LOW);
          digitalWrite(yellowPin, LOW);
          digitalWrite(greenPin, LOW);
        }
        
      } else {
        Serial.println("HTTP " + String(responseCode));
        ESP.restart();
      }
      
      https.end();
      
    } else {
      Serial.printf("Problem connecting. Restarting...");
      ESP.restart();
    }
    
  }
  
  delay(30000);
    
}
