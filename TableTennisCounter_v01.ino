#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#define BUTTON_P1 2
#define BUTTON_P2 3

#define DISPLAY_ADDRESS 0x70

int p1Win[30][4] = { { 0, 0, 0, 0 }, { 48, 0, 0, 0 }, { 51, 0, 0, 0 }, { 243, 56, 0, 0 }, { 243, 56, 6, 0 }, { 243, 56, 6, 0 }, { 243, 56, 6, 0 }, { 243, 56, 6, 0 }, { 195, 56, 6, 0 }, { 192, 56, 6, 0 }, { 0, 56, 6, 0 }, { 48, 56, 6, 0 }, { 2096, 8, 6, 0 }, { 10294, 8, 6, 0 }, { 10294, 0, 0, 0 }, { 10294, 513, 0, 0 }, { 10294, 4617, 0, 0 }, { 10294, 4617, 48, 0 }, { 10294, 4617, 8496, 0 }, { 10294, 4617, 8502, 0 }, { 10294, 4617, 8502, 33 }, { 10294, 4617, 8502, 225 }, { 10294, 4617, 8502, 237 }, { 10294, 4617, 8502, 237 }, { 10294, 4617, 8502, 237 }, { 10294, 4617, 8502, 237 }, { 10294, 4617, 8502, 237 }, { 10294, 4617, 8502, 237 }, { 10294, 4617, 8502, 237 }, { 10294, 4617, 8502, 237 } };
int p1Winlen = 30;
int p2Win[30][4] = { { 0, 0, 0, 0 }, { 192, 0, 0, 0 }, { 195, 0, 0, 0 }, { 243, 0, 0, 0 }, { 243, 8, 0, 0 }, { 243, 56, 0, 0 }, { 243, 56, 24, 0 }, { 243, 56, 218, 0 }, { 243, 56, 219, 0 }, { 240, 56, 219, 0 }, { 240, 24, 218, 0 }, { 208, 24, 216, 0 }, { 16, 24, 152, 0 }, { 16, 8, 8, 0 }, { 0, 0, 8, 0 }, { 0, 0, 0, 0 }, { 48, 0, 0, 0 }, { 10288, 0, 0, 0 }, { 10294, 0, 0, 0 }, { 10294, 4608, 0, 0 }, { 10294, 4617, 0, 0 }, { 10294, 4617, 48, 0 }, { 10294, 4617, 8496, 0 }, { 10294, 4617, 8502, 0 }, { 10294, 4617, 8502, 33 }, { 10294, 4617, 8502, 225 }, { 10294, 4617, 8502, 237 }, { 10294, 4617, 8502, 237 }, { 10294, 4617, 8502, 237 }, { 10294, 4617, 8502, 237 } };
int p2Winlen = 30;

Adafruit_AlphaNum4 disp = Adafruit_AlphaNum4();

bool game = true;
bool end = false;

String state;

bool b1Down = false;
bool b2Down = false;

int count1;
int count2;
int pcount1 = 0;
int pcount2 = 0;
int totalCount = count1 + count2;

long waitTime = 1500;
long time1 = 0;
long time2 = 0;

int aufschlag = 0;

// player1 = 0 player2 = 1
int beginner = 0;
int winner;

bool blinkOn = false;
bool blinkState = false;
long previousMillis = 0;
long timeoutMillis;

bool ignorePoint[2] = {false, false};

void setup() {
  disp.begin(DISPLAY_ADDRESS);
  disp.setBrightness(10);
  disp.clear();
  disp.writeDisplay();

  state = "idle";
  pinMode(BUTTON_P1, INPUT_PULLUP);
  pinMode(BUTTON_P2, INPUT_PULLUP);
  count1 = 0;
  count2 = 0;

  time1 = 0;
  time2 = 0;
  Serial.begin(9600);
}

long current_millis = 0;
long interval = 500;

void loop() {

  blink();

  current_millis = millis();
  totalCount = count1 + count2;

  if (current_millis - timeoutMillis > 600000) {
    state = "idle";
    timeoutMillis = current_millis;
  }

  if (digitalRead(BUTTON_P1) == LOW) {
    timeoutMillis = current_millis;
    if (!b1Down) {
      b1Down = true;
    }
  }
  if (digitalRead(BUTTON_P1) == HIGH) {
    
    if (b1Down && !ignorePoint[0]) {
      if (state == "game") {
        count1 += addPoint();
        aufschlag = getAufschlag();
      }
    }
    time1 = current_millis;
    ignorePoint[0] = false;
    b1Down = false;
  }

  if (current_millis - time1 > waitTime) {
    start(0);
  }

  if (digitalRead(BUTTON_P2) == LOW) {
    timeoutMillis = current_millis;
    if (!b2Down) {
      b2Down = true;
    }
  }
  if (digitalRead(BUTTON_P2) == HIGH) {
    
    if (b2Down && !ignorePoint[1]) {
      if (state == "game") {
        count2 += addPoint();
        aufschlag = getAufschlag();
      }
    }
    time2 = current_millis;
    ignorePoint[1] = false;
    b2Down = false;
  }

  if (current_millis - time2 > waitTime) {
    start(1);
  }

  //Serial.println(current_millis - time2);
  if (b1Down && b2Down) {
    count2 = pcount2;
    count1 = pcount1;
    ignorePoint[0] = true;
    ignorePoint[1] = true;
    aufschlag = getAufschlag();
  }



  if (state == "game") {
    displayCount(count1, 0, aufschlag == 0);
    displayCount(count2, 2, aufschlag == 1);

    disp.writeDisplay();

    if (((count1 < 11 && count2 < 11) || abs(count1 - count2) < 2)) {

    } else {
      win();
    }
    // display the text

  } else if (state == "idle") {
    idle();
  } else if (state == "end") {

    int p = int((millis() * 0.01)) % 60;
    if (p < 30) {
      if (winner == 0) {
        for (int j = 0; j < 4; j++) {
          disp.writeDigitRaw(j, p1Win[p][j]);
        }
      } else {
        for (int j = 0; j < 4; j++) {
          disp.writeDigitRaw(j, p2Win[p][j]);
        }
      }

    } else {
      displayCount(count1, 0, aufschlag == 0);
      displayCount(count2, 2, aufschlag == 1);
    }
    disp.writeDisplay();
  }

  
  /*
  Serial.print(state);
  Serial.print(";");
  Serial.print(count1);
  Serial.print(";");
  Serial.print(count2);
  Serial.println();
  */
  //Serial.println("Sate:" + state) + ",p1:" + count1 + "p2:" +count2);
}

void win() {
  if (count1 > count2) {
    winner = 0;
  }

  else {
    winner = 1;
  }
  //end = true;
  state = "end";
  blinkOn = false;
}

int getAufschlag() {
  totalCount = count1 + count2;
  if (totalCount < 20) {
    if (totalCount % 4 == 0 || totalCount % 4 == 1) {
      return beginner;
    } else {
      return abs(beginner - 1);
    }
  } else {
    if (totalCount % 2 == 0) {
      return beginner;
    } else {
      return abs(beginner - 1);
    }
  }
}

int addPoint() {
  if ((count1 < 11 && count2 < 11) || abs(count1 - count2) < 2) {
    pcount1 = count1;
    pcount2 = count2;
    return 1;
  } else {
    win();
    return 0;
  }
}

void start(int b) {
  count1 = 0;
  count2 = 0;
  beginner = b;
  aufschlag = getAufschlag();
  blinkOn = true;
  state = "game";
  ignorePoint[b] = true;
  //Serial.println("Start");
  
}

void displayCount(int c, int start, bool turn) {
  for (int i = 1; i >= 0; i--) {
    if (String(c).length() > 1) {
      disp.writeDigitAscii(i + start, String(c).charAt(i));
    } else {
      disp.writeDigitAscii(start + 1, String(c).charAt(i));
      disp.writeDigitAscii(start, ' ');
    }
  }
  if (turn && blinkState) {
    disp.writeDigitAscii(start, ' ');
    disp.writeDigitAscii(start + 1, ' ');
  }
}

void blink() {
  current_millis = millis();
  if (blinkOn) {
    if (current_millis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = current_millis;

      // if the LED is off turn it on and vice-versa:
      if (blinkState == false) {
        blinkState = true;
      } else {
        blinkState = false;
      }
    }
  } else {
    blinkState = false;
  }
}

int idelAnimation[14][4] = { { 8, 8, 24, 9 }, { 8, 8, 25, 8 }, { 8, 1032, 24, 8 }, { 8, 2056, 24, 8 }, { 16392, 8, 24, 8 }, { 8200, 8, 24, 8 }, { 264, 8, 24, 8 }, { 9, 8, 24, 8 }, { 8, 9, 24, 8 }, { 8, 8, 280, 8 }, { 8, 8, 8216, 8 }, { 8, 8, 16408, 8 }, { 8, 8, 24, 2056 }, { 8, 8, 24, 1032 } };
int idelAnimationlen = 14;

void idle() {
  int p = int((millis() * 0.01)) % idelAnimationlen;
  for (int j = 0; j < 4; j++) {
    disp.writeDigitRaw(j, idelAnimation[p][j]);
  }

  disp.writeDisplay();
}