#include <Arduino.h>
#include "power.h"


int pin = 12;
volatile int state = LOW;

void blink() {
    state = !state;
    ledGreenLight(state);
}

void setup() {
    ledGreenLight(LOW);
    //attachInterrupt(pin, blink, RISING);
    system_set_sleepmode(SYSTEM_SLEEPMODE_STANDBY);
    
    	/* Goes in STANDBY sleep  */
    	system_sleep();
}

void loop() {
    ledGreenLight(state);
}    
