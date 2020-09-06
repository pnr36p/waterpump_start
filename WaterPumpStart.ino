#include <Servo.h>
#include <EEPROM.h>

const int startupDelay = 5000; // startup delay, ms

const int buttonPin = 2;       // the number of the pushbutton pin, that can cycle the max speed between 50%, 75% and 100%
const int ESCPin =  12;        // the number of the ESC control pin
const int LEDPin =  13;        // the number of the LED control pin
const int minSpeed =  1000;    // minimum speed of ESC (default 543)
const int maxSpeed =  2000;    // maximum speed of ESC (default 2400)
const byte maxSpeed100 =  180; // maximum speed of ESC at 100% (default 180 degrees)
const byte maxSpeed75  =  100; // maximum speed of ESC at 75% (default 135 degrees)
const byte maxSpeed50  =  40;  // maximum speed of ESC at 50% (default 90 degrees) 
const int eeAddress = 0;       // EEPROM address to store max speed percentage

// Most ESC work fine with these values

// WARNING: In some cases you need to change frequency of the PWM signal of Servo.h library, find this string:

// #define REFRESH_INTERVAL    20000

// and replace it with this:

// #define REFRESH_INTERVAL    16667

// That will change frequency from 50Hz to 60Hz


Servo myservo;  // create servo object to control a servo
byte pos = 0;    // variable to store the servo position
byte memSpeed = 0; // variable to read the EEPROM value

void setup() {
  EEPROM.get(eeAddress, memSpeed); // Reading the EEPROM maximum speed value
  if ((memSpeed != maxSpeed75) and (memSpeed != maxSpeed50))   {  // Checking if the value is right (for example, first launch)
    memSpeed = maxSpeed100; }
    
  pinMode(ESCPin, OUTPUT);    // attach ESCPin to ESC
  pinMode(LEDPin, OUTPUT);    // attach LEDPin to LED
  pinMode(buttonPin, INPUT_PULLUP);  // attach buttonPin to button
  myservo.attach(ESCPin, minSpeed, maxSpeed);  // attaches the servo on ESCPin to the servo object with min/max speed
  myservo.write(0);           // tell ESC to go stop

  delay(startupDelay);        // wait for ESC init
  digitalWrite(LEDPin, HIGH); // begin startup


  for (pos = 0; pos <= memSpeed - 1; pos += 1) { // goes from 0 degrees to 180 degrees
                                                                            // in steps of 1 degree
    myservo.write(pos);                                            // tell ESC to go change speed using variable 'pos'
    delay(25);                                                          // waits 15ms for the ESC to reach the speed
  }
  digitalWrite(LEDPin, LOW);                                      // end of startup
}

void loop() {
    myservo.write(pos);                                             // tell ESC to go change speed using variable 'pos'
    
    // Changing top speed
    int buttonValue = digitalRead(buttonPin);               //Reading buttonPin
    if (buttonValue == LOW){                   
      // If button pressed, change the speed
      while (digitalRead(buttonPin) == LOW) {               // Wait for button depress
          digitalWrite(LEDPin, HIGH);
          delay(50);
          digitalWrite(LEDPin, LOW);
          } 
      switch (pos) {
        case maxSpeed100:             // If speed is 100% then change it to 75%
          pos = maxSpeed75;
          EEPROM.put(eeAddress, pos); // Write it to the EEPROM
          break;
        case maxSpeed75:              // If speed is 75% then change it to 50%
          pos = maxSpeed50;
          EEPROM.put(eeAddress, pos); // Write it to the EEPROM
          break;
        case maxSpeed50:              // If speed is 50% then change it to 100% smoothly
          for (pos = maxSpeed50; pos <= maxSpeed100 - 1; pos += 1) { // goes from maxSpeed50 to maxSpeed100
                                                                                                          // in steps of 1 degree
               myservo.write(pos);                                                               // tell ESC to go change speed using variable 'pos'
               delay(25);                                                                           // waits 15ms for the ESC to reach the speed
          }
          pos = maxSpeed100;
          EEPROM.put(eeAddress, pos); // Write it to the EEPROM
          break;
      }
   }
    delay(100);                         // waits 100ms
}
