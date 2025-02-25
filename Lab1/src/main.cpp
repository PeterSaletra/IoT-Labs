#include <Arduino.h>
#include <LiquidCrystal.h>
#include <DHT.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const int button1 = 1;
const int button2 = 2;
const int button3 = 3;
const int button0 = 0;


void setup() {
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button0, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Hello World!");

  lcd.begin(16, 2);
  lcd.print("Hello World!");
}

void loop() {

  float temperature = analogRead(A1);
  temperature = ((temperature*5.0)/ 1024.0) * 100.0;
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("\n");

  if (digitalRead(button1) == LOW) {
    lcd.clear();
    lcd.print("Button 1 pressed");
    Serial.println("Button 1 pressed");
  }else if (digitalRead(button2) == LOW) {
    lcd.clear();
    lcd.print("Button 2 pressed");
    Serial.println("Button 2 pressed");
  }else if (digitalRead(button3) == LOW) {
    lcd.clear();
    lcd.print("Button 3 pressed");
    Serial.println("Button 3 pressed");
  }else if (digitalRead(button0) == LOW) {
    lcd.clear();
    lcd.print("Button 0 pressed");
    Serial.println("Button 0 pressed");
  }else {
    lcd.clear();
    lcd.print("Nothing pressed");
    Serial.println("Nothing pressed");
  }
  delay(1000);
}