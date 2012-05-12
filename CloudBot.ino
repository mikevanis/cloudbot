#define DIR1 2
#define STEP1 3
#define MS11 5
#define MS21 4
#define SLEEP1 6

int numOfSteps;
int stepsRemaining;
boolean antiClockwise = false;
boolean microStepping = false;
int minSpeed;
int maxSpeed;
int currentSpeed;

unsigned long previous_timer;
byte timer;

void setup() {
  Serial.begin(9600);

  pinMode(DIR1, OUTPUT);
  pinMode(STEP1, OUTPUT);
  pinMode(MS11, OUTPUT);
  pinMode(MS21, OUTPUT);
  pinMode(SLEEP1, OUTPUT);

  digitalWrite(DIR1, HIGH);
  digitalWrite(MS11, MS1_MODE(1));
  digitalWrite(MS21, MS2_MODE(1));
  digitalWrite(SLEEP1, HIGH);

  maxSpeed = 10;
  minSpeed = 60;

  currentSpeed = maxSpeed;

  stepTo(500);
}

void loop() {
  // 500usec WRAPAROUND
  if((unsigned long)(micros() - previous_timer) >= 500) {
    previous_timer = micros();

    // Run motor
    if(stepsRemaining > 0) {

      // Is it time to microstep yet?
      if(stepsRemaining == numOfSteps/2 && !microstepping) {
        stepsRemaining *= 8;
        digitalWrite(MS11, MS1_MODE(8));
        digitalWrite(MS21, MS2_MODE(8));
        microstepping = true;
      }
      else if(stepsRemaining > numOfSteps/2) {
        if((timer+1) % maxSpeed == 0) {
          digitalWrite(STEP1, LOW);
          digitalWrite(STEP1, HIGH);
          stepsRemaining--;
        }
      }
      else {
        if((timer+1) % (currentSpeed/8) == 0) {
          digitalWrite(STEP1, LOW);
          digitalWrite(STEP1, HIGH);
          stepsRemaining--;
          currentSpeed = map(stepsRemaining, numOfSteps/2, 1, maxSpeed, minSpeed);
          Serial.println(currentSpeed);
        }
      }
    }
    else {
      digitalWrite(SLEEP1, LOW);
      microstepping = false;
      digitalWrite(MS11, MS1_MODE(8));
      digitalWrite(MS21, MS2_MODE(8));
    }
    timer = (timer+1) % 100;
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

void stepTo(int amt) {
  digitalWrite(SLEEP1, HIGH);
  if(amt < 0) {
    digitalWrite(DIR1, HIGH);
    stepsRemaining = amt * -1;
    numOfSteps = stepsRemaining;
  }
  else {
    digitalWrite(DIR1, LOW);
    stepsRemaining = amt;
    numOfSteps = stepsRemaining;
  }
}



