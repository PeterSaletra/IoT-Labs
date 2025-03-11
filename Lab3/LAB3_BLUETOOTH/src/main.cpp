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
  // Check for messsage from serial monitor.
  if (Serial.available()) {
    // Write message to paired Bluetooth device.
    buffer = Serial.read();
    Serial.println(buffer);
    SerialBT.write((uint8_t*)&buffer, 1);
  }
  
  // Check for message from paired Bluetooth device.
  if (SerialBT.available()) {
    // Write message to serial monitor.
    buffer = SerialBT.read();

    if(buffer == '1'){
      digitalWrite(LED_BUILTIN, LOW);
    } else if(buffer == '0'){
      digitalWrite(LED_BUILTIN, HIGH);
    }
    
    Serial.println(buffer);
    SerialBT.write((uint8_t*)&buffer, 1);
  }


  delay(20);
}