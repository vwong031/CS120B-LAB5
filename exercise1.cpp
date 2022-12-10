/*
* Name & Email: Valerie Wong
* Discussion Section: 021
* Assignment: Lab 5 Exercise 1
* Exercise Description: This exercise will use the joystick to control the direction and speed of the stepper motor. 
*                       The 4 LED's will indicate what input the microcontroller received from the joystick.

* I acknowledge all content contained herein, excluding template 
  or example code, is my own original work.

* Demo Link: https://youtu.be/8WN_l4S2JQ0
*/
int i = 0;
int c = 0;
int cnt = 0;
bool isCounter;
int IN1 = 10;
int IN2 = 11;
int IN3 = 12;
int IN4 = 13;
int UP = 3;
int DOWN = 4;
int LEFT = 2;
int RIGHT = 5;
int JS_X = A0;
int JS_Y = A1;
int JS_BTN = 7;
int sig[4] = {IN1, IN2, IN3, IN4};
int steps[8][4] { //codes for the motor to turn properly
  {0, 0, 0, 1},
  {0, 0, 1, 1},
  {0, 0, 1, 0},
  {0, 1, 1, 0},
  {0, 1, 0, 0},
  {1, 1, 0, 0},
  {1, 0, 0, 0},
  {1, 0, 0, 1},
};

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);

} task;

int delay_gcd;
const unsigned short tasksNum = 2;
task tasks[tasksNum];

void reduceSpeed(bool isCounter) {
  if (cnt == 2) {
    if (isCounter) {
      ++i;
    }
    else {
      --i;
    }
    cnt = 0;
  }
}

void counterclockwise() {
  if (c == 4) {
    c = 0;
    i++;
  }
      
  if (i == 8) {
    i = 0;
  }
}

void clockwise() {
  if (c == 4) {
    c = 0;
    i--;
  }
      
  if (i == 0) {
    i = 8;
  }
}

void rotateMotor() {
  for (c; c < 4; c++) {
    if (steps[i][c] == 1) {
      digitalWrite(sig[c], HIGH);
    }
    else {
      digitalWrite(sig[c], LOW);
    }
  }
}

enum SM1_States { SM1_INIT, COUNTERCLOCKWISE, CLOCKWISE, REDUCE_SPEED_COUNTER, REDUCE_SPEED_CLOCK, WAIT };
int SM1_Tick(int state1) {
  switch (state1) { // State transitions
    case SM1_INIT:
      state1 = COUNTERCLOCKWISE;
      break;
      
    case COUNTERCLOCKWISE:
      counterclockwise();
      
      if (analogRead(JS_X) < 200) {
        state1 = CLOCKWISE;
      }
      else if (analogRead(JS_Y) < 200) {
        state1 = REDUCE_SPEED_COUNTER;
      }
      else if (analogRead(JS_Y) > 700) {
        state1 = COUNTERCLOCKWISE;
      }
      else if (!digitalRead(JS_BTN)) {
        state1 = WAIT;
      }
      break;

    case CLOCKWISE:
      clockwise();

      if (analogRead(JS_X) > 700) {
        state1 = COUNTERCLOCKWISE;
      }
      else if (analogRead(JS_Y) < 200) {
        state1 = REDUCE_SPEED_CLOCK;
      }
      else if (analogRead(JS_Y) > 700) {
        state1 = CLOCKWISE;
      }
      else if (!digitalRead(JS_BTN)) {
        state1 = WAIT;
      }
      break;

    case REDUCE_SPEED_COUNTER:
      isCounter = true;
      if (c == 4) {
        c = 0;
        
        reduceSpeed(isCounter);
        ++cnt;
      }
      
      if (i == 8) {
        i = 0;
      }
      
      if (analogRead(JS_X) < 200) {
        state1 = CLOCKWISE;
      }
      else if (analogRead(JS_X) > 700) {
        state1 = COUNTERCLOCKWISE;
      }
      else if (analogRead(JS_Y) > 700) {
        state1 = COUNTERCLOCKWISE;
      }
      else if (!digitalRead(JS_BTN)) {
        state1 = WAIT;
      }
      break;

    case REDUCE_SPEED_CLOCK:
      isCounter = false;
      if (c == 4) {
        c = 0;
        
        reduceSpeed(isCounter);
        ++cnt;
      }
      
      if (i == 0) {
        i = 8;
      }
      
      if (analogRead(JS_X) < 200) {
        state1 = CLOCKWISE;
      }
      else if (analogRead(JS_X) > 700) {
        state1 = COUNTERCLOCKWISE;
      }
      else if (analogRead(JS_Y) > 700) {
        state1 = CLOCKWISE;
      }
      else if (!digitalRead(JS_BTN)) {
        state1 = WAIT;
      }
      break;

    case WAIT:
      if (digitalRead(JS_BTN)) {
        state1 = COUNTERCLOCKWISE;
      }
      
      break;

  }
  switch (state1) { // State Action
    case SM1_INIT:
      break;
    case COUNTERCLOCKWISE:
      rotateMotor();
      break;

    case CLOCKWISE:
      rotateMotor();
      break;

    case REDUCE_SPEED_COUNTER:
      rotateMotor();
      break;

    case REDUCE_SPEED_CLOCK:
      rotateMotor();
      break;

    case WAIT:
      break;
  }

  return state1;
}

enum SM2_States { SM2_INIT, RIGHT_LED, LEFT_LED, UP_LED, DOWN_LED, ALL_LEDS};
int SM2_Tick(int state2) {
  switch (state2) { // State transitions
    case SM2_INIT:
      //State Transition
      state2 = RIGHT_LED;
      break;

    case RIGHT_LED:
      if (analogRead(JS_X) < 200) {
        state2 = LEFT_LED;
      }
      else if (analogRead(JS_Y) > 700) {
        state2 = UP_LED;
      }
      else if (analogRead(JS_Y) < 200) {
        state2 = DOWN_LED;
      }
      else if (!digitalRead(JS_BTN)) {
        state2 = ALL_LEDS;
      }
      break;

    case LEFT_LED:
      if (analogRead(JS_X) > 700) {
        state2 = RIGHT_LED;
      }
      else if (analogRead(JS_Y) > 700) {
        state2 = UP_LED;
      }
      else if (analogRead(JS_Y) < 200) {
        state2 = DOWN_LED;
      }
      else if (!digitalRead(JS_BTN)) {
        state2 = ALL_LEDS;
      }
      break;

    case UP_LED:
      if (analogRead(JS_X) > 700) {
        state2 = RIGHT_LED;
      }
      else if (analogRead(JS_X) < 200) {
        state2 = LEFT_LED;
      }
      else if (analogRead(JS_Y) < 200) {
        state2 = DOWN_LED;
      }
      else if (!digitalRead(JS_BTN)) {
        state2 = ALL_LEDS;
      }
      break;

    case DOWN_LED:
      if (analogRead(JS_X) > 700) {
        state2 = RIGHT_LED;
      }
      else if (analogRead(JS_X) < 200) {
        state2 = LEFT_LED;
      }
      else if (analogRead(JS_Y) > 700) {
        state2 = UP_LED;
      }
      else if (!digitalRead(JS_BTN)) {
        state2 = ALL_LEDS;
      }
      break;

    case ALL_LEDS:
      if (digitalRead(JS_BTN)) {
        state2 = RIGHT_LED;
      }
      break;

  }
  switch (state2) { // State Action
    case SM2_INIT:
      break;
      
    case RIGHT_LED:
      digitalWrite(UP, LOW);
      digitalWrite(RIGHT, HIGH);
      digitalWrite(DOWN, LOW);
      digitalWrite(LEFT, LOW);
      
      break;

    case LEFT_LED:
      digitalWrite(UP, LOW);
      digitalWrite(RIGHT, LOW);
      digitalWrite(DOWN, LOW);
      digitalWrite(LEFT, HIGH);
      
      break;

    case UP_LED:
      digitalWrite(UP, HIGH);
      digitalWrite(RIGHT, LOW);
      digitalWrite(DOWN, LOW);
      digitalWrite(LEFT, LOW);
      break;

    case DOWN_LED:
      digitalWrite(UP, LOW);
      digitalWrite(RIGHT, LOW);
      digitalWrite(DOWN, HIGH);
      digitalWrite(LEFT, LOW);
      break;

    case ALL_LEDS:
      digitalWrite(UP, HIGH);
      digitalWrite(RIGHT, HIGH);
      digitalWrite(DOWN, HIGH);
      digitalWrite(LEFT, HIGH);
      break;
  }

  return state2;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(UP, OUTPUT);
  pinMode(DOWN, OUTPUT);
  pinMode(LEFT, OUTPUT);
  pinMode(RIGHT, OUTPUT);
  pinMode(JS_BTN, INPUT_PULLUP);
  Serial.begin(9600);


  unsigned char i = 0;
  tasks[i].state = SM1_INIT;
  tasks[i].period = 1;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  i++;
  tasks[i].state = SM2_INIT;
  tasks[i].period = 50;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;

  delay_gcd = 1; // GCD
}

void loop() {
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = millis(); // Last time this task was ran
    }
  }
}
