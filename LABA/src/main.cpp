#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Ping.h>
#include <BluetoothSerial.h>

const char* ssid = "";
const char* password = "";

const char* mqtt_server = "test.mosquitto.org";
WiFiClient espClient;
PubSubClient client(espClient);

BluetoothSerial SerialBT;

const int ledPin = 2;

bool isWet = false;
int sensorValue = 0;

#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    SerialBT.println("Connecting to WiFi...");
  }
}

void reconnect() {
  while (!client.connected()) {
    client.connect("zietekdroid");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == '1') {
    SerialBT.println("1");
  } else {
    SerialBT.println("0");
  }

}

void setup() {
  // Serial.begin(115200);
  SerialBT.begin("ESP32");
  pinMode(ledPin, OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.subscribe("xddd");
  if (Ping.ping("8.8.8.8")) {
    SerialBT.println("Ping OK");
  } else {
    SerialBT.println("Ping FAILED");
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  sensorValue = random(0, 101);
  isWet = sensorValue > 50;

  String stan = isWet ? "mokro" : "sucho";

  digitalWrite(ledPin, HIGH);
  delay(isWet ? 100 : 500);
  digitalWrite(ledPin, LOW);
  delay(isWet ? 100 : 500);


  if(stan == "mokro") {
    snprintf (msg, MSG_BUFFER_SIZE, "{\"Stan\": \"%s\", \"Wartosc\": %d}", stan, sensorValue);
    client.publish("zieteksaletramokro", msg);
  } else {
    snprintf (msg, MSG_BUFFER_SIZE, "{\"Stan\": \"%s\", \"Wartosc\": %d}", stan, sensorValue);
    client.publish("zieteksaletrasucho", msg);
  }

  SerialBT.println("Status: " + stan + " | Wilgotność: " + String(sensorValue));

  delay(2000);
}
