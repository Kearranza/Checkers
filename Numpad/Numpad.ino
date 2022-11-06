#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
const int ledPin = 9;
const int ledPin2 = 10;

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  analogWrite(ledPin, 5);
  Serial.begin(9600);
}
  
void loop(){
  char key = keypad.getKey();
  
  if (key){
    Serial.write(key);
    Serial.println(key);
    digitalWrite(ledPin2,HIGH);
    delay(100);
    digitalWrite(ledPin2,LOW);

  }
  if (Serial.available()){
    char led_specifier = Serial.read();
    int led_brightness = Serial.parseInt();
    analogWrite(ledPin, led_brightness);
    delay(200);
    analogWrite(ledPin,0);
  }
}
