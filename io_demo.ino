/*
 * I/O demonstration machine
 * 
 * Push a button to select between inputs:
 *  * potentiometer
 *  * accelerometer (ADXL335 analog 3-axis)
 *  * photoresistor
 *  * IR proximity sensor (LTH-1550-01)
 *  
 * Push a different button to select between outputs:
 *  * hobby servomotor position
 *  * LED brightness
 *  * speaker pitch
 *  * vibration motor intensity
 *  
 * The selected input will drive the selected output continuously.
 *  
 * Additionally, data about the selected input and outputs is written 
 * regularly to an LCD screen and serial monitor.
 * 
 * Robert Zacharias, rzachari@andrew.cmu.edu
 * Feb. 16, 2019
 * released to the public domain by the author
 */

 // inputs
 const int POTPIN = A0;
 const int ACCELPIN = A1;
 const int PHOTOPIN = A2;
 const int IRPIN = A3;
 const int INPUTMODEPIN = 2;
 const int OUTPUTMODEPIN = 4;

 // outputs
 const int LEDPIN = 6         // must be a PWM pin to use analogWrite
 const int SERVOPIN = 8;
 const int SPEAKERPIN = 10;
 const int VIBRATIONPIN = 11; // must be a PWM pin to use analogWrite
 
