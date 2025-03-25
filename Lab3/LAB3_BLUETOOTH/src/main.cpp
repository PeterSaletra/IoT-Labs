// source:
// https://dronebotworkshop.com/esp32-bluetooth/
//

#include <Arduino.h>
#include <BluetoothSerial.h>  // Include BluetoothSerial library.
 
// Name of Bluetooth client.
String device_name = "SALETROID";
boolean isOn = false;
// Check if Bluetooth is available.
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled!
#endif
 
// Check if Serial Port Profile (SPP) is available.
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled!
#endif

#ifndef LED_BUILTIN
    #define LED_BUILTIN 1
#endif

// Create a BluetoothSerial object.
BluetoothSerial SerialBT;
char buffer;

void setup() {

  // Start Serial Monitor.
  Serial.begin(115200);
  Serial.printf("The device \"%s\" is started and can be paired with Bluetooth.\n", device_name.c_str());
  pinMode(LED_BUILTIN, OUTPUT);

  // Start Bluetooth Serial.
  SerialBT.begin(device_name);
  //SerialBT.deleteAllBondedDevices();  // Uncomment the next line to delete previously paired devices. Must be called directly after bluetooth begin.
 
}
 
void loop() {
  // Check for message from serial monitor.
  
  // Scan for available Bluetooth devices.
  BTScanResults* results = SerialBT.discover();
  int numDevices = results->getCount();
  if (numDevices > 0) {
    Serial.printf("Found %d devices:\n", numDevices);
    for (int i = 0; i < numDevices; i++) {
      BTAdvertisedDevice* device = results->getDevice(i);
      Serial.printf("Device %d: %s [%s]\n", i + 1, device->getName().c_str(), device->getAddress().toString().c_str());
    }
  } else {
    Serial.println("No devices found.");
  }
  delay(5000); // Wait for 5 seconds before scanning again
}