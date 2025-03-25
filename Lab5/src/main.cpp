#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi credentials
const char* ssid = "zietek";
const char* password = "piotrek12345";

// Create a web server on port 80
WebServer server(80);

const char* html = "<html><style> body{margin: 0; padding: 0;}</style><body><a href='/on'><button style='height: 50%; width: 100%; color: black; background-color: green; font-size: 500px; border: none;'>ON</button ></a> <a href='/off'><button style='height: 50%; width: 100%; color: black; background-color: red; font-size: 500px; border: none;'>OFF</button></a></body></html>";

// Handle root request
void handleRoot() {
  server.send(200, "text/html", html);
}

void handleOn(){
  server.send(200, "text/html", html);
  digitalWrite(2, HIGH);
}

void handleOff(){
  server.send(200, "text/html", html);
  digitalWrite(2, LOW);
}

// Handle 404 errors
void handleNotFound() {
  server.send(404, "text/html", "404: Not Found");
}

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // Define server routes
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.onNotFound(handleNotFound);

  // Start the server
  server.begin();
  Serial.println("HTTP Server started!");
  pinMode(2, OUTPUT);
}

void loop() {
  server.handleClient(); // Handle client requests
}