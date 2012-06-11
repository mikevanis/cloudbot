/*
@todo:
 Smoothing for steppers
 Microstepping -completed
 Multithreading for stepping -completed
 
 Motor speed is instructed in increments of 500 microseconds.
 For example: 
 1 is 500
 2 is 1000
 3 is 1500
 4 is 2000
 5 is 2500
 
 
 */

#include <EasyStepper.h>

EasyStepper x(2, 3, 5, 4, 6, 0);
EasyStepper y(7, 8, 10, 9, 11, 1);

char command[5];

int currentPos;

void setup() {
  Serial.begin(115200);
  x.setMicrostepping(8);
  y.setMicrostepping(1);
  x.sleep(true);
  y.sleep(false);
  x.setMaxSpeed(3);
  y.setMaxSpeed(5);
  zero();
  x.setPosition(50);
  x.stepTo(0);
}

void loop() {
  if(x.isMoving()==0) {
    if(Serial.available() > 0) {
      int pos = Serial.parseInt();
      x.stepTo(pos);
      Serial.print("pos = ");
      Serial.println(pos);
      Serial.flush();
    }
  }
  else {
    Serial.flush();
  }
  
  x.update();
}

void zero() {
  x.update();
  int magneticAvg = 0;
  for(int i=0; i<60; i++) {
    magneticAvg += x.readEndStop();
  }
  //Serial.println(magneticAvg);
  if(magneticAvg != 0) {
    x.step(1);
    zero();
  }
}

void stepTo(int pos) {
  int steps = (currentPos*-1) + pos;
  x.step(steps);
}
