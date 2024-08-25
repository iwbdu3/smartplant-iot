#include <ESP8266WiFi.h>
#include <MQTT.h>

#define pinPompa 13

const char ssid[] = "patsepat";
const char pass[] = "12345678";

WiFiClient net;
MQTTClient client;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting ke server");
  while (!client.connect("startlink1")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("startlink/#", 1);
}

void messageReceived(String &topic, String &data) {
  Serial.println("incoming: " + topic + " - " + data);

  if(topic == "startlink/pompa"){
    if(data == "on"){
      digitalWrite(pinPompa, 1);
    } else if(data == "off"){
      digitalWrite(pinPompa, 0);
    }
  }
}

void setup() {
  pinMode(pinPompa, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  client.begin("broker.emqx.io", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }
}
