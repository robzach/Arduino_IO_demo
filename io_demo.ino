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
  // TO DO: debounce these
  if (digitalRead(INPUTMODEPIN) == LOW){
    inputMode++;
    inputMode %= 4; // wrap back to 0 if at 4
  }
  if (digitalRead(OUTPUTMODEPIN) == LOW){
    outputMode++;
    outputMode %= 4; // wrap back to 0 if at 4
  }

  // read selected input into the master input variable
  static int inVal;
  switch (inputMode){
    case 0: // potentiometer input
      inVal = potVal;
      inputName = "potentiometer";
      break;
    case 1: // accelerometer input
      inVal = accelVal;
      inputName = "accelerometer";
      break;
    case 2: // photoresistor input
      inVal = photoVal;
      inputName = "photoresistor";
      break;
    case 3: // IR proximity sensor input
    default:
      inVal = IRVal;
      inputName = "IR prox.";
      break;
  }

  // calculate output values for each device
  int outVal = map(inVal, 0, 1023, 0, 255);

  // drive selected output with appropriate value
  switch (outputMode){
    case 0: // LED output
      noTone(SPEAKERPIN); // turn off speaker (if on)
      analogWrite(LEDPIN, map(inVal, 0, 1023, 0, 255));
      outputName = "LED";
      break;
    case 1: // speaker output
      tone(SPEAKERPIN, map(inVal, 0, 1023, 150, 3000));
      outputName = "speaker";
      break;
    case 2: // vibration output
      noTone(SPEAKERPIN);
      analogWrite(VIBRATIONPIN, map(inVal, 0, 1023, 0, 255));
      outputName = "vibration";
      break;
    case 3: // servo output
    default:
      noTone(SPEAKERPIN);
      gaugeMotor.write(map(inVal, 0, 1023, 10, 170));
      outputName = "servo";
      break;
  }
}
