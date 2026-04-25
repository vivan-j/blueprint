#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Keypad.h>
#include <Servo.h>

SoftwareSerial mySerial(11, 12); // RX=11, TX=12
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo gate;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String realpass = "1002";
String guess = "";
int counter = 0;
bool unlocked = false;
bool lastUnlocked = false;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Enter PIN:");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  gate.attach(10);
  gate.write(180);
  delay(500);
  gate.detach();
}

void loop() {
  char key = keypad.getKey();
  if (!unlocked) {
    if (key) {
      if (key == '#') {
        counter = 0;
        guess = "";
        mySerial.print("CLEARED");
        delay(10);
        lcd.setCursor(0, 1);
        lcd.print("                ");
        Serial.println("Cleared");

      } else if (counter < 4) {
        counter++;
        guess += String(key);
        String masked = "";
        for (int i = 0; i < counter; i++) masked += '*';
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(masked);
        Serial.println("Entered so far: " + guess);

        if (counter == 4) {
          if (guess == realpass) {
            unlocked = true;
            mySerial.print("UNLOCKED");
            delay(10);
            lcd.setCursor(0, 0);
            lcd.print("** UNLOCKED! ** ");
            lcd.setCursor(0, 1);
            lcd.print("                ");
            Serial.println("Unlocked");
          } else {
            mySerial.print("WRONG PIN");
            delay(10);
            lcd.setCursor(0, 0);
            lcd.print("  Wrong PIN!    ");
            lcd.setCursor(0, 1);
            lcd.print("  Try again...  ");
            delay(2000);
            lcd.setCursor(0, 0);
            lcd.print("Enter PIN:      ");
            lcd.setCursor(0, 1);
            lcd.print("                ");
            Serial.println("Incorrect password");
          }
          counter = 0;
          guess = "";
        }
      }
    }

  } else {
    if (key && key == '*') {
      unlocked = false;
      mySerial.print("LOCKED");
      delay(10);
      lcd.setCursor(0, 0);
      lcd.print("Enter PIN:      ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      Serial.println("Locked");
    }
  }


  if (unlocked != lastUnlocked) {
    if (unlocked) {
      gate.attach(10);
      gate.write(90);
      delay(1000);
      gate.detach();
    } else {
      gate.attach(10);
      gate.write(180);
      delay(1000);
      gate.detach();
    }
    lastUnlocked = unlocked;
  }
}
