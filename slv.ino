
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX=10, TX=11
const int laserPin = 8;
bool laserActive = false;

void setup() {
  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin, LOW);
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  static unsigned long lastCheck = 0;
  static unsigned long lastBlink = 0;
  static bool laserState = false;

  // Check for messages every 100ms
  if (millis() - lastCheck >= 100) {
    lastCheck = millis();

    if (mySerial.available()) {
      delay(50);
      String msg = "";
      while (mySerial.available()) {
        msg += (char)mySerial.read();
      }
      Serial.println("Got: " + msg);

      if (msg == "WRONG PIN") {
        laserActive = true;
      } else if (msg == "UNLOCKED") {
        laserActive = false;
        digitalWrite(laserPin, LOW);
      } else if (msg == "LOCKED") {
        laserActive = false;
        digitalWrite(laserPin, LOW);
      } else if (msg == "CLEARED") {
        laserActive = false;
        digitalWrite(laserPin, LOW);
      }
    }
  }

  // Blink laser if active
  if (laserActive) {
    if (millis() - lastBlink >= 500) {
      lastBlink = millis();
      laserState = !laserState;
      digitalWrite(laserPin, laserState ? HIGH : LOW);
    }
    
  }
}
