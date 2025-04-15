#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Ping.h>
#include <BluetoothSerial.h>

const char* mqtt_server = "test.mosquitto.org";
WiFiClient espClient;
PubSubClient client(espClient);
BluetoothSerial SerialBT;

void reconnect() {
  while (!client.connected()) {
    client.connect("zietekdroid");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  
  SerialBT.print("Message arrived [");
  SerialBT.print(topic);
  SerialBT.print("] ");
  for (int i = 0; i < length; i++) {
    SerialBT.print((char)payload[i]);
  }
  SerialBT.println();

  if (topic == "GRZANIE") {
    digitalWrite(2, HIGH);
    delay(1000);
    digitalWrite(2, LOW);
  } else {
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
  }

}

void setup() {
  pinMode(2, OUTPUT);
  SerialBT.begin("ESP32"); //Bluetooth device name
  WiFi.mode(WIFI_STA);
  WiFi.begin("net-g", "E3d2y1t0a-");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    SerialBT.println("Connecting to WiFi...");
  }

  SerialBT.println("Connected to WiFi");
  SerialBT.println(WiFi.localIP());


  if(Ping.ping("8.8.8.8")){
    SerialBT.println("Ping successful");
  }

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.connect("zietekdroid");
  client.subscribe("GRZANIE");
  client.subscribe("CHLODZENIE");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}