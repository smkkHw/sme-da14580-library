/*
  userButton.ino

  Example to sohws how the 2 userbutton works on Smarteverything

  Created: 6/17/2015 10:32:11 PM

   Author: Mik

*/

#include <Arduino.h>

char led;
bool bounce = false;
long referenceTime;


// the setup function runs once when you press reset or power the board
void setup() {
  Serial1.begin(115200);
  SerialUSB.begin(115200);
  ledYellowOneLight(LOW);
    ledYellowTwoLight(LOW);

  // LED & User Button are already initialized by the SME core.
  // it is not required to do here
}

// the loop function runs over and over again forever
void loop() {
  char data;

  while (Serial1.available()) {

    data = Serial1.read();
    SerialUSB.println(data);

    switch (data) {

      case '1':
        ledLight(true);
        break;

      case '0':
        ledLight(false);
        break;

      case 't':
      case 'T':
        bounce = false;
        break;
      default:
        led = data;
        break;

    }
  }

  if (isButtonOnePressed() && !bounce) {
    bounce = true;
    Serial1.print("B1 ");  
    ledYellowOneLight(HIGH);
    referenceTime = millis();
  } else if (isButtonTwoPressed() && !bounce) {
    bounce = true;
    Serial1.print("B2 ");
    ledYellowTwoLight(HIGH);
    referenceTime = millis();
  }

  if ((millis() - referenceTime) > 2000) {
    referenceTime = millis();
    bounce = false;
      ledYellowOneLight(LOW);
    ledYellowTwoLight(LOW);
  }
}




void ledLight(boolean _light) {
  char light = HIGH;

  if (!_light)
    light = LOW;

  switch (led) {

    case 'G':
    case 'g':
      ledGreenLight(light);
      break;

    case 'R':
    case 'r':
      ledRedLight(light);
      break;

    case 'B':
    case 'b':
      ledBlueLight(light);
      break;
  }

}
