/*
* Name & Email: Valerie Wong
* Discussion Section: 021
* Assignment: Lab 5 Exercise 2
* Exercise Description: A common real-world use of stepper motors is to control heavy duty locks, for example, the type of lock that might be integrated into a safe. 
*                       You will need to create a sequence of inputs that the lock will recieve from the joystick to open the "safe". 
*                       You need to choose 4 individual inputs to be your passcode, an example passcode would be up-up-left-right.

* I acknowledge all content contained herein, excluding template 
  or example code, is my own original work.

* Demo Link: https://youtu.be/v20bnqR7ORw 
*/
int i = 8;
int c = 0;
int cnt = 0;
int passSize = 0; // reading size
int passcodeSize = 0; // passcode size
int k = 0;
int tmp;
int passcode[4] = {1, 1, 4, 2};
int reading[4] = {0, 0, 0 , 0};
bool locked = true;
bool newPass = false;
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
int j_x;
int j_y;
int j_btn;
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

void readJoystick() {
  j_x = analogRead(JS_X);
  j_y = analogRead(JS_Y);
  j_btn = digitalRead(JS_BTN);
}

bool checkInput() {
  bool isInput = false;
  readJoystick();

  if (j_x > 800 || j_x < 200 || j_y > 800 || j_y < 200) {
    isInput = true;
  }

  return isInput;
}

int readInput() {
  int input;
  readJoystick();

  if (j_y > 900) {
    reading[passSize] = 1;
    input = 1;
  }
  else if (j_x > 900) {
    reading[passSize] = 2; 
    input = 2;
  }
  else if (j_y < 200) {
    reading[passSize] = 3;
    input = 3;
  }
  else if (j_x < 100) {
    reading[passSize] = 4;
    input = 4;
  }

  return input;
}

int newPasscode() {
  int input;
  readJoystick();

  if (j_y > 900) {
    passcode[passcodeSize] = 1;
    input = 1;
  }
  else if (j_x > 900) {
    passcode[passcodeSize] = 2; 
    input = 2;
  }
  else if (j_y < 200) {
    passcode[passcodeSize] = 3;
    input = 3;
  }
  else if (j_x < 200 && j_btn) {
    passcode[passcodeSize] = 4;
    input = 4;
  }

  return input;
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

void ledsON() {
  digitalWrite(UP, HIGH);
  digitalWrite(RIGHT, HIGH);
  digitalWrite(DOWN, HIGH);
  digitalWrite(LEFT, HIGH);

}

void ledsOFF() {
  digitalWrite(UP, LOW);
  digitalWrite(RIGHT, LOW);
  digitalWrite(DOWN, LOW);
  digitalWrite(LEFT, LOW);
}

bool comparePasscodes() {
  bool isSame = true;
  for (int i = 0; i < 4; ++i) {
    if (reading[i] != passcode[i]) {
      isSame = false; 
      break;
    }
  }

  return isSame;
}

enum SM1_States { SM1_INIT, LOCK, UNLOCK };
int SM1_Tick(int state1) {
  switch (state1) { // State transitions
    case SM1_INIT:
      state1 = LOCK;
      break;

    case LOCK:
      if (!locked) {
        state1 = UNLOCK;
      }
  
      if (k >= 0) {
        if (c == 4) {
          c = 0;
          i--;
        }
      
        if (i <= 0) {
          i = 8;
          --k;
        }
      }

      if (k == 0) {
        ledsOFF();
      }
      break;
      
    case UNLOCK:
      if (locked) {
        state1 = LOCK;
      }
      
      if (k <= 256) {
        if (c == 4) {
          c = 0;
          i++;
        }
      
        if (i >= 8) {
          i = 0;
          ++k;
        }
      }
      
      break;

  }
  switch (state1) { // State Action
    case SM1_INIT:
      break;

    case LOCK:
      rotateMotor();
      break;

    case UNLOCK:
      rotateMotor();
      break;
  }

  return state1;
}

enum SM2_States { SM2_INIT, WAIT, INPUT_PASS, NEW_PASS, UP_LED, RIGHT_LED, DOWN_LED, LEFT_LED, CORRECT, INCORRECT };
int SM2_Tick(int state2) {
  switch (state2) { // State transitions
    case SM2_INIT:
      //State Transition
      state2 = WAIT;
      break;

    case WAIT:
      readJoystick();
      
      if (!j_btn && passSize < 4 && !locked) {
        ledsOFF();
        newPass = true;   
        passcodeSize = 0;
      }
      
      if (checkInput() && !newPass) {
        state2 = INPUT_PASS;
      }
      else if (checkInput() && newPass) {
        state2 = NEW_PASS;
      }
      break;

    case INPUT_PASS:
      readJoystick();
      tmp = readInput();
      
      if ((passSize < 4) && (j_btn == 1)) {
        if (tmp == 1) {
          ++passSize;
          state2 = UP_LED; 
        }
        else if (tmp == 2) {
          ++passSize;
           state2 = RIGHT_LED; 
        }
        else if (tmp == 3) {
          ++passSize;
          state2 = DOWN_LED; 
        }
        else if (tmp == 4) {
          ++passSize;
          state2 = LEFT_LED; 
        }
      }
      else {
        if (comparePasscodes() && passSize >= 4) {
          locked = !locked;
          passSize = 0;
          state2 = CORRECT;
        }
        else {
          passSize = 0;
          state2 = INCORRECT;
        }
      }

      if (passSize == 4) {
        state2 = INPUT_PASS;
      }
      break;

    case NEW_PASS:
      tmp = newPasscode();
      
      if (passcodeSize < 4) {
        if (tmp == 1) {
          ++passcodeSize;
          state2 = UP_LED; 
        }
        else if (tmp == 2) {
          ++passcodeSize;
          state2 = RIGHT_LED; 
        }
        else if (tmp == 3) {
          ++passcodeSize;
          state2 = DOWN_LED; 
        }
        else if (tmp == 4) {
          ++passcodeSize;
          state2 = LEFT_LED; 
        }
        
        if (passcodeSize == 4) {
          newPass = false;
          state2 = CORRECT;
        }
      }
      break;
      
    case UP_LED:
      readJoystick();
      
      if (cnt < 10 && locked) {
        state2 = UP_LED;
      }
      else if (cnt < 10 && !locked && !newPass) {
        ledsON();
      }
      else if (cnt < 10 && !locked && newPass) {
        state2 = UP_LED;
      }
      else if (cnt >= 10 && locked) {
        ledsOFF();
        state2 = WAIT;
        cnt = 0;
      }
      else if (cnt >= 10 & !locked & newPass) {
        ledsOFF();
        state2 = WAIT;
        cnt = 0;
      }
      else if (cnt >= 10 && !locked) {
        state2 = WAIT;
        cnt = 0;
      }
      break;

    case RIGHT_LED:
      if (cnt < 10 && locked) {
        state2 = RIGHT_LED;
      }
      else if (cnt < 10 && !locked && !newPass) {
        ledsON();
      }
      else if (cnt < 10 && !locked && newPass) {
        state2 = RIGHT_LED;
      }
      else if (cnt >= 10 && locked) {
        ledsOFF();
        state2 = WAIT;
        cnt = 0;
      }
      else if (cnt >= 10 & !locked & newPass) {
        ledsOFF();
        state2 = WAIT;
        cnt = 0;
      }
      else if (cnt >= 10 && !locked) {
        state2 = WAIT;
        cnt = 0;
      }
      break;

    case DOWN_LED:
      if (cnt < 10 && locked) {
        state2 = DOWN_LED;
      }
      else if (cnt < 10 && !locked && !newPass) {
        ledsON();
        state2 = WAIT;
      }
      else if (cnt < 10 && !locked && newPass) {
        state2 = DOWN_LED;
      }
      else if (cnt >= 10 && locked) {
        ledsOFF();
        state2 = WAIT;
        cnt = 0;
      }
      else if (cnt >= 10 & !locked & newPass) {
        ledsOFF();
        state2 = WAIT;
        cnt = 0;
      }
      else if (cnt >= 10 && !locked) {
        state2 = WAIT;
        cnt = 0;
      }
      break;

    case LEFT_LED:
      if (cnt < 10 && locked) {
        state2 = LEFT_LED;
      }
      else if (cnt < 10 && !locked && !newPass) {
        ledsON();
        state2 = WAIT;
      }
      else if (cnt < 10 && !locked && newPass) {
        state2 = LEFT_LED;
      }
      else if (cnt >= 10 && locked) {
        ledsOFF();
        state2 = WAIT;
        cnt = 0;
      }
      else if (cnt >= 10 & !locked & newPass) {
        ledsOFF();
        state2 = WAIT;
        cnt = 0;
      }
      else if (cnt >= 10 && !locked) {
        state2 = WAIT;
        cnt = 0;
      }
      break;

    case CORRECT:
      if (cnt > 10) {
        state2 = WAIT;
      }
      break;

    case INCORRECT:
      if (cnt < 60) {
        state2 = INCORRECT;
      }
      else {
        state2 = WAIT;
      }
      break;

  }
  switch (state2) { // State Action
    case SM2_INIT:
      break;

    case WAIT:
      break;

    case INPUT_PASS:
      break;

    case NEW_PASS:
      ++cnt;
      break;

    case UP_LED:
      if (locked || newPass) {
        digitalWrite(UP, HIGH);
        digitalWrite(RIGHT, LOW);
        digitalWrite(DOWN, LOW);
        digitalWrite(LEFT, LOW);
      }
      else {
        ledsON();
      }
      ++cnt;
      break;

    case RIGHT_LED:
      if (locked || newPass) {
        digitalWrite(UP, LOW);
        digitalWrite(RIGHT, HIGH);
        digitalWrite(DOWN, LOW);
        digitalWrite(LEFT, LOW);
      }
      else {
        ledsON();
      }
      ++cnt;
      break;

    case DOWN_LED:
      if (locked || newPass) {
        digitalWrite(UP, LOW);
        digitalWrite(RIGHT, LOW);
        digitalWrite(DOWN, HIGH);
        digitalWrite(LEFT, LOW);
      }
      else {
        ledsON();
      }
      ++cnt;
      break;

    case LEFT_LED:
      if (locked || newPass) {
        digitalWrite(UP, LOW);
        digitalWrite(RIGHT, LOW);
        digitalWrite(DOWN, LOW);
        digitalWrite(LEFT, HIGH);
      }
      else {
        ledsON();
      }
      ++cnt;
      break;

    case CORRECT:
      if (!locked) {
        ledsON();  
      }
      ++cnt;
      break;

    case INCORRECT:
      if (!locked) {
        ledsON();
      }
      else {
        if (cnt % 20 == 0 || cnt % 20 == 1 || cnt % 20 == 2) {
          ledsON();
        }
        else {
          ledsOFF();
        } 
      }
      ++cnt;
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
