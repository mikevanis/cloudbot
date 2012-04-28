/*
@todo:
 Smoothing for steppers
 Microstepping -completed
 Multithreading for stepping
 
 Motor speed is instructed in increments of 500 microseconds.
 For example: 
 1 is 500
 2 is 1000
 3 is 1500
 4 is 2000
 5 is 2500
 
 
 */

#define DIR1 2
#define STEP1 3
#define MS11 5
#define MS21 4
#define SLEEP1 6
#define DIR2 7
#define STEP2 8
#define MS12 10
#define MS22 9
#define SLEEP2 11

// Steps to execute, remaining, & direction
int prevSteps[2];
int stepsRemaining[2];
boolean antiClockwise = false;
int mtrSpeed[2];
int maxSpeed = 5;
boolean micro1 = false;
boolean micro2 = false;

// Variables used for timing
unsigned long previous_timer;
byte timer;  // In increments of 500 microseconds

void setup() {
  Serial.begin(9600);

  pinMode(DIR1, OUTPUT);
  pinMode(STEP1, OUTPUT);
  pinMode(MS11, OUTPUT);
  pinMode(MS21, OUTPUT);
  pinMode(SLEEP1, OUTPUT);

  pinMode(DIR2, OUTPUT);
  pinMode(STEP2, OUTPUT);
  pinMode(MS12, OUTPUT);
  pinMode(MS22, OUTPUT);
  pinMode(SLEEP2, OUTPUT);

  digitalWrite(DIR1, HIGH);
  digitalWrite(MS11, MS1_MODE(1));
  digitalWrite(MS21, MS2_MODE(1));
  digitalWrite(SLEEP1, HIGH);

  digitalWrite(DIR2, LOW);
  digitalWrite(MS12, MS1_MODE(1));
  digitalWrite(MS22, MS2_MODE(1));
  digitalWrite(SLEEP2, HIGH);

  maxSpeed = 10;
  mtrSpeed[0] = maxSpeed;
  mtrSpeed[1] = maxSpeed;

  stepTo(200, 200);
}

void loop() {
  // ------- EVERY 500 MICROSECONDS -------
  if((unsigned long)(micros() - previous_timer) >= 500) {
    previous_timer = micros();

    // Smoothing];
    for(int i=0; i<2; i++) {
      if(stepsRemaining[i] != prevSteps[i]) {
        if(stepsRemaining[i] > 50) {
          mtrSpeed[i] = maxSpeed;
        }
        else {
          mtrSpeed[i] = -stepsRemaining[i] + 60;
        }
      }
      prevSteps[i] = stepsRemaining[i];
    }

    // Run motors
    if(stepsRemaining[0] > 0) {
      // ------- MOTOR 1 -------
      if((timer+1) % mtrSpeed[0] == 0) {
        digitalWrite(STEP1, LOW);
        digitalWrite(STEP1, HIGH);
        stepsRemaining[0]--;
      }
    }

    if(stepsRemaining[1] > 0) {
      // ------- MOTOR 2 -------
      if((timer+1) % mtrSpeed[1] == 0) {
        digitalWrite(STEP2, LOW);
        digitalWrite(STEP2, HIGH);
        stepsRemaining[1]--;
      }
    }

    timer = (timer + 1) % 100;
  }
}

int MS1_MODE(int stepMode) {
  switch(stepMode) {
  case 1:
    stepMode = 0;
    Serial.println("Full steps...");
    break;
  case 2:
    stepMode = 1;
    Serial.println("Half steps...");
    break;
  case 4:
    stepMode = 0;
    Serial.println("Quarter steps...");
    break;
  case 8:
    stepMode = 1;
    Serial.println("Eighth steps...");
    break;
  }
  return stepMode;
}

int MS2_MODE(int stepMode) {
  switch(stepMode) {
  case 1:
    stepMode = 0;
    break;
  case 2:
    stepMode = 0;
    break;
  case 4:
    stepMode = 1;
    break;
  case 8:
    stepMode = 1;
    break;
  }
  return stepMode;
}

void demo() {
  int modeType = 1;

  while(modeType<=8) {
    digitalWrite(DIR2, HIGH);
    digitalWrite(MS12, MS1_MODE(modeType));
    digitalWrite(MS22, MS2_MODE(modeType));
    digitalWrite(SLEEP2, HIGH);

    int i=0;
    while(i<(200)) {
      digitalWrite(STEP2, LOW);
      digitalWrite(STEP2, HIGH);
      delayMicroseconds(6000);

      i++;
    }
    modeType *= 2;
    delay(500);
  }
  digitalWrite(SLEEP2, LOW);
  Serial.println("Sleeping... ");
  delay(1000);
  Serial.print("z ");
  delay(1000);
  Serial.print("z ");
  delay(1000);
  Serial.print("z ");
  digitalWrite(SLEEP2, HIGH);
  Serial.println("Awake!");
  delay(2000);
}

void stepTo(int steps1, int steps2) {
  if(steps1 < 0) {
    digitalWrite(DIR1, HIGH);
    stepsRemaining[0] = steps1 * -1;
  }
  else {
    digitalWrite(DIR1, LOW);
    stepsRemaining[0] = steps1;
  }
  if(steps2 < 0) {
    digitalWrite(DIR2, HIGH);
    stepsRemaining[1] = steps2 * -1;
  }
  else {
    digitalWrite(DIR2, LOW);
    stepsRemaining[1] = steps2;
  }
  Serial.println(stepsRemaining[0]);
  Serial.println(stepsRemaining[1]);
}


