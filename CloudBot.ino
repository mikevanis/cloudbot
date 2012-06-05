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

EasyStepper x(2, 3, 5, 4, 6);
EasyStepper y(7, 8, 10, 9, 11);

void setup() {
  x.setMicrostepping(8);
  y.setMicrostepping(8);
  x.sleep(true);
  y.sleep(true);
  x.setMaxSpeed(30);
  x.step(100);
  y.step(100);
}

void loop() {
  x.update();
  y.update();
}
