/*
   I/O demonstration machine

   Push a button to select between inputs:
     potentiometer
     accelerometer (ADXL335 analog 3-axis)
     photoresistor
     IR proximity sensor (LTH-1550-01)

   Push a different button to select between outputs:
     hobby servomotor position
     LED brightness
     speaker pitch
     vibration motor intensity

   The selected input will drive the selected output continuously.

   Additionally, data about the selected input and outputs is written
   regularly to an LCD screen and serial monitor.

   Robert Zacharias, rzachari@andrew.cmu.edu
   Feb. 16, 2019
   released to the public domain by the author
*/

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// inputs
const int POTPIN = A0;
const int ACCELPIN = A1;
const int PHOTOPIN = A2;
const int IRPIN = A3;
const int INPUTMODEPIN = 2;
const int OUTPUTMODEPIN = 4;

// outputs
const int LEDPIN = 6;         // must be a PWM pin to use analogWrite
const int SERVOPIN = 8;
const int SPEAKERPIN = 10;
const int VIBRATIONPIN = 11; // must be a PWM pin to use analogWrite

// variables for timing
unsigned long timer;
unsigned long WAIT = 500; // 500 milliseconds = 1/2 second

 /* Create an LCD display object called "screen" with I2C address 0x27
    which is 16 columns wide and 2 rows tall. You can use any name you'd like. */
LiquidCrystal_I2C screen(0x3f, 16, 2);

Servo gaugeMotor;

void setup() {
  // set up pin modes
  pinMode(POTPIN, INPUT);
  pinMode(ACCELPIN, INPUT);
  pinMode(PHOTOPIN, INPUT);
  pinMode(IRPIN, INPUT);
  pinMode(INPUTMODEPIN, INPUT_PULLUP); // use INPUT_PULLUP for easier wiring
  pinMode(OUTPUTMODEPIN, INPUT_PULLUP);
  pinMode(LEDPIN, OUTPUT);
  pinMode(SPEAKERPIN, OUTPUT);
  pinMode(VIBRATIONPIN, OUTPUT);

  gaugeMotor.attach(SERVOPIN);

  screen.init();
  screen.backlight();

  Serial.begin(9600);
}

void loop() {
  // mode variables (static type so they don't reset every loop)
  static int inputMode, outputMode;

  // message variables
  static String inputName = "";
  static String outputName = "";

  // read all sensors
  int potVal = analogRead(POTPIN);
  int accelVal = analogRead(ACCELPIN);
  int photoVal = analogRead(PHOTOPIN);
  int IRVal = analogRead(IRPIN);
  // all these input values will be in range [0, 1023]

  // increment modes as per button presses
  if (digitalRead(INPUTMODEPIN) == LOW) {
    delay(5); // very simple debounce
    inputMode++;
    inputMode %= 4; // wrap back to 0 if at 4
  }
  if (digitalRead(OUTPUTMODEPIN) == LOW) {
    delay(5); // very simple debounce
    outputMode++;
    outputMode %= 4; // wrap back to 0 if at 4
  }

  // read selected input into the master input variable
  // scaled 0 to 1000
  static int inVal;
  switch (inputMode) {
    case 0: // potentiometer input
      inVal = map(potVal, 0, 1023, 0, 1000);
      inputName = "pot'meter";
      break;
    case 1: // accelerometer input
      // the device only really runs in a small range normally
      inVal = map(accelVal, 250, 450, 0, 1000);
      inputName = "accel'meter";
      break;
    case 2: // photoresistor input
      inVal = map(photoVal, 0, 1023, 0, 1000);
      inputName = "photores.";
      break;
    case 3: // IR proximity sensor input
    default:
      inVal = map(IRVal, 0, 1023, 0, 1000);
      inputName = "IR prox.";
      break;
  }

  // variable to hold calculated output value
  int outVal;

  // drive selected output with appropriate value
  switch (outputMode) {
    case 0: // LED output
      noTone(SPEAKERPIN); // turn off speaker (if on)
      analogWrite(VIBRATIONPIN, 0); // turn off motor (if on)
      outVal = map(inVal, 0, 1000, 0, 255);
      analogWrite(LEDPIN, outVal);
      outputName = "LED";
      break;
    case 1: // speaker output
      analogWrite(VIBRATIONPIN, 0); // turn off motor (if on)
      outVal = map(inVal, 0, 1000, 150, 3000);
      tone(SPEAKERPIN, outVal);
      outputName = "speaker";
      break;
    case 2: // vibration output
      noTone(SPEAKERPIN);
      outVal = map(inVal, 0, 1000, 0, 255);
      analogWrite(VIBRATIONPIN, outVal);
      outputName = "vibration";
      break;
    case 3: // servo output
    default:
      noTone(SPEAKERPIN);
      analogWrite(VIBRATIONPIN, 0); // turn off motor (if on)
      outVal = map(inVal, 0, 1000, 10, 170);
      gaugeMotor.write(outVal);
      outputName = "servo";
      break;
  }

  // report serial and LCD feedback every WAIT milliseconds
  if (millis() - timer >= WAIT) {
    Serial.println((String)"input: " + inputName + ", value = " + inVal + \
                   ";\t output: " + outputName + ", value = " + outVal);
    screen.clear();
    screen.home();
    screen.print(inputName + " " + inVal);
    screen.setCursor(0,1);
    screen.print(outputName + " " + outVal);
    
    timer = millis(); // reset the timer to the current time
  }

}
