#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "T2.36 (3)de'KOSTel";
const char* password = "godbless";
const char* mqtt_server = "192.168.3.111";
const int mqtt_port = 1883;

const int LDR_PIN = 34;
const int LED_PIN = 13;
const int THRESHOLD = 2000;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  client.setServer(mqtt_server, mqtt_port);
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT");
    } else {
      delay(2000);
    }
  }
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);
  Serial.println(ldrValue);

  // Menentukan status LED dan mengirimkan sebagai integer
  int status = (ldrValue < THRESHOLD) ? 0 : 1;
  digitalWrite(LED_PIN, (status == 0) ? LOW : HIGH);

  // Mengirim status LED sebagai integer
  client.publish("ldr/status", String(status).c_str());

  // Mengirim nilai LDR sebagai integer (dalam bentuk string untuk MQTT)
  client.publish("ldr/value", String(ldrValue).c_str());

  client.loop();
  delay(1000);
}
