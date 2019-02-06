#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

const int stepPin = 9;        //9 is a special pin. DON'T CHANGE!!!!
const int dirPin = 4;
const int plusButton = 0;     //0 is a special pin. DON'T CHANGE!!!!
const int minusButton = 1;    //1 is a special pin. DON'T CHANGE!!!!
const int horizon = 5;
const int addPointButton = 8;
const int removePointButton = 16;
const int confirmButton = 6;
const int backButton = 7;
const int homingSwitch = 10;

//---------------------------bitmaps-----------------------------------------

// 'camera', 20x17px
const unsigned char camera [] PROGMEM = {
  0x1c, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x40, 0x00, 0x80, 0xdc, 0x78, 0xc0, 0x40, 0x84, 0x80, 0x40,
  0xa4, 0x80, 0x40, 0x84, 0x80, 0x40, 0x78, 0x80, 0x40, 0x00, 0x80, 0x3e, 0x0f, 0x00, 0x01, 0xf0,
  0x00, 0x02, 0x08, 0x00, 0x02, 0x08, 0x00, 0xff, 0xff, 0xf0, 0x04, 0x04, 0x00, 0x08, 0x02, 0x00,
  0xf8, 0x03, 0xf0
};

// 'slider', 128x13px
const unsigned char slider [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01,
  0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x01,
  0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x01,
  0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01,
  0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x48,
  0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x08,
  0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x88,
  0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x58,
  0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x30,
  0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0
};

// 'home', 10x10px
const unsigned char homeIcon [] PROGMEM = {
  0x0c, 0x00, 0x1e, 0x00, 0x33, 0x00, 0x61, 0x80, 0xc0, 0xc0, 0x40, 0x80, 0x40, 0x80, 0x4c, 0x80,
  0x4c, 0x80, 0x7f, 0x80
};

// 'AUTOMATIC', 22x18px
const unsigned char autIcon [] PROGMEM = {
  0x00, 0xfc, 0x00, 0x03, 0x03, 0x00, 0x04, 0x00, 0x00, 0x08, 0x00, 0x00, 0x10, 0xfc, 0x00, 0x10,
  0x84, 0x00, 0x20, 0x84, 0x10, 0x21, 0x02, 0x38, 0x21, 0x02, 0x54, 0x21, 0x02, 0x10, 0xa9, 0xfe,
  0x10, 0x71, 0x02, 0x20, 0x21, 0x02, 0x20, 0x01, 0x02, 0x20, 0x00, 0x00, 0x40, 0x00, 0x00, 0x80,
  0x03, 0x87, 0x00, 0x00, 0x78, 0x00
};

// 'MANUAL', 22x18px
const unsigned char manIcon [] PROGMEM = {
  0x00, 0x01, 0x00, 0x00, 0x02, 0x80, 0x00, 0x05, 0x90, 0x00, 0x05, 0x28, 0x00, 0x00, 0xc8, 0x01,
  0x04, 0x10, 0x01, 0x8d, 0xf0, 0x01, 0x55, 0x00, 0x01, 0x24, 0x00, 0x01, 0x24, 0x00, 0x01, 0x04,
  0x00, 0x01, 0x04, 0x00, 0x0d, 0x04, 0x00, 0x19, 0x04, 0x00, 0x35, 0x04, 0x00, 0x6d, 0x04, 0x00,
  0x58, 0x00, 0x00, 0x70, 0x00, 0x00
};

//-------------------------------------------------------------------------------------------

bool isScreenChanged;

int sliderDirection = -1;
int sliderSpeed = 50;
const unsigned long maxSpeed = 75;
const unsigned long minSpeed = 5000;
const unsigned long ultraLowSpeed = 2000;
const long numberOfSteps = 62000;
int accCam = 10;
bool autMode;
bool isPlaying;
long currentStep;
int plusPressed;
int minusPressed;

//------menu---------
int itemSelected = 0;
unsigned int  cameraIconAt = 0;

void setup() {
  //Serial.begin(57600);
  //display setup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  //interrupts();
  pinMode(plusButton, INPUT);
  pinMode(minusButton, INPUT);
  attachInterrupt(digitalPinToInterrupt(plusButton), plus, FALLING);
  attachInterrupt(digitalPinToInterrupt(minusButton), minus, FALLING);

  cameraIconAt = map(currentStep, 0, numberOfSteps, 92, 4);
  drawBaseHM();
  changeDirectionHM();
  ChangeModeHM();
  changePlayPauseHM();
  display.display();
}

void loop() {
  handMode();
}

void handMode() {
  while (1) {
    isScreenChanged = false;
    if (digitalRead(plusButton) == LOW) itemShift(1);
    if (digitalRead(minusButton) == LOW) itemShift(-1);

    if (digitalRead(confirmButton)) {
      switch (itemSelected) {
        case 0:
          debounceButton(confirmButton);
          changeDirectionHM();
          break;
        case 1:
          debounceButton(confirmButton);
          findHome();
          break;
        case 3:
          debounceButton(confirmButton);
          ChangeModeHM();
          break;
        case 4:
          debounceButton(confirmButton);
          adjustSpeedHM();
          break;
        case 5:
          debounceButton(confirmButton);
          changePlayPauseHM();
          break;
      }
    }

    if (updateCamera()) isScreenChanged = true;
    checkEndstops();

    if (isScreenChanged) {
      display.display();
    }
  }
}

void automaticMode() {
  //display.clearDisplay();
  //drawBaseAut();
  //changePlayPauseAut();
  //display.display();
  while (1) {
    isScreenChanged = false;
    if (digitalRead(plusButton) == LOW) itemShift(1);
    if (digitalRead(minusButton) == LOW) itemShift(-1);

    if (digitalRead(confirmButton)) {
      switch (itemSelected) {
        case 0:
          debounceButton(confirmButton);
          changeDirectionHM();
          break;
        case 1:
          debounceButton(confirmButton);
          findHome();
          break;
        case 3:
          debounceButton(confirmButton);
          ChangeModeHM();
          break;
        case 4:
          debounceButton(confirmButton);
          adjustSpeedAut();
          break;
        case 5:
          debounceButton(confirmButton);
          changePlayPauseAut();
          break;
      }
    }

    if (updateCamera()) isScreenChanged = true;
    checkEndstops();

    if (isScreenChanged) {
      display.display();
    }
  }
}

void itemShift(int indexModifier) {
  if ((indexModifier == 1 || indexModifier == -1) == 0) return;
  if (itemSelected + indexModifier > 5) return;
  if (itemSelected + indexModifier < 0) return;

  int x[6] = {0, 0, 0, 22, 54, 104};
  const int y[6] = {4, 18, 30, 30, 30, 30};
  int w[6] = {3, 3, 3, 17, 34, 22};
  const int h[6] = {10, 10, 3, 3, 3, 3};
  const int animSpeed[6] = {12, 14, 14, 16, 24, 20};
  if (autMode) {
    x[4] = 51;
    x[5] = 74;
    w[4] = 12;
    w[5] = 54;
  }
  int currentX = x[itemSelected];
  int currentY = y[itemSelected];
  int currentW = w[itemSelected];
  int currentH = h[itemSelected];
  //
  int counter = 0;
  display.drawRect(currentX, currentY, currentW, currentH, BLACK);
  display.drawRect(x[5], y[5], w[5], h[5], BLACK);
  display.drawRect(x[0], y[0], w[0], h[0], BLACK);
  itemSelected += indexModifier;
  while (currentX != x[itemSelected] || currentY != y[itemSelected] || currentW != w[itemSelected] || currentH != h[itemSelected]) {
    if (currentX != x[itemSelected]) {
      if (currentX < x[itemSelected])currentX++;
      else currentX--;
    }
    if (currentY != y[itemSelected]) {
      if (currentY < y[itemSelected])currentY++;
      else currentY--;
    }
    if (currentW != w[itemSelected]) {
      if (currentW < w[itemSelected])currentW++;
      else currentW--;
    }
    if (currentH != h[itemSelected]) {
      if (currentH < h[itemSelected])currentH++;
      else currentH--;
    }
    display.drawRect(currentX, currentY, currentW, currentH, WHITE);
    counter++;

    //animation speed compenstaion
    if (indexModifier > 0) {
      if (counter == animSpeed[itemSelected - 1]) {
        updateCamera();
        display.display();
        counter = 0;
      }
    } else {
      if (counter == animSpeed[itemSelected]) {
        updateCamera();
        display.display();
        counter = 0;
      }
    }
    display.drawRect(currentX, currentY, currentW, currentH, BLACK);
    digitalWrite(13, HIGH);
    //delay(100);
  }
  digitalWrite(13, LOW);
  if (itemSelected == 2) {
    itemShift(indexModifier);
    display.drawRect(x[2], y[2], w[2], h[2], BLACK);
  }
  display.drawRect(currentX, currentY, currentW, currentH, WHITE);
  display.drawRect(x[2], y[2], w[2], h[2], BLACK);
  updateCamera();
  display.display();
}

void drawBaseHM() {
  display.drawLine(16, 4, 16, 28, WHITE);
  display.drawLine(45, 4, 45, 28, WHITE);
  display.drawLine(96, 4, 96, 28, WHITE);
  display.drawLine(2, 16, 12, 16, WHITE);

  display.drawBitmap(0, 51,  slider, 128, 13, 1);
  display.drawBitmap(4, 18,  homeIcon, 10, 10, 1);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(55, 0);
  display.print("Speed:");

  display.setTextSize(2);
  display.setCursor(55, 10);
  display.print(sliderSpeed);
  display.print("%");

  display.drawRect(0, 4, 3, 10, WHITE);
  display.drawBitmap(cameraIconAt, 40, camera, 20, 17, WHITE);
  isScreenChanged = true;
}

void drawBaseAut() {
  display.drawLine(16, 4, 16, 28, WHITE);
  display.drawLine(45, 4, 45, 28, WHITE);
  display.drawLine(68, 4, 68, 28, WHITE);
  display.drawLine(2, 16, 12, 16, WHITE);

  display.drawBitmap(0, 51,  slider, 128, 13, 1);
  display.drawBitmap(4, 18,  homeIcon, 10, 10, 1);

  display.fillRect(52, 5, 9, 20, WHITE);
  display.fillRect(52, 5, 9, 20 - accCam, BLACK);
  display.fillTriangle(52, 25, 52, 5, 55, 25, BLACK);
  display.fillTriangle(61, 25, 61, 5, 58, 25, BLACK);

  display.drawRect(0, 4, 3, 10, WHITE);
  display.drawBitmap(cameraIconAt, 40, camera, 20, 17, WHITE);
  isScreenChanged = true;
}


void adjustSpeedHM() {
  display.fillRect(48, 29, 46, 4, WHITE);
  display.display();
  display.setTextSize(2);
  display.setCursor(47, 10);
  display.print(sliderSpeed);
  display.print("%");
  while (1) {
    bool updateScr = false;
    if (updateCamera()) updateScr = true;
    bool hitEnd = false;
    if (checkEndstops()) hitEnd = true;
    display.fillRect(47, 10, 47, 19, BLACK);
    if (digitalRead(plusButton) == LOW) {
      sliderSpeed++;
      if (sliderSpeed >= 100) sliderSpeed = 100;
      motorSpeed(sliderSpeed);
      updateScr = true;
    }

    if (digitalRead(minusButton) == LOW) {
      sliderSpeed--;
      if (sliderSpeed <= 1) sliderSpeed = 1;
      motorSpeed(sliderSpeed);
      updateScr = true;
    }

    int centerOfText;
    if (sliderSpeed == 100) centerOfText = 47;
    if (sliderSpeed < 100 && sliderSpeed >= 10) centerOfText = 55;
    if (sliderSpeed < 10) centerOfText = 59;

    if (digitalRead(backButton) || hitEnd) {
      display.fillRect(48, 29, 46, 4, BLACK);
      display.setTextSize(2);
      display.setCursor(centerOfText, 10);
      display.print(sliderSpeed);
      display.print("%");
      isScreenChanged = true;
      display.drawRect(54, 30, 34, 3, WHITE);
      return;
    }

    if (updateScr) {
      display.setTextSize(2);
      display.setCursor(centerOfText, 10);
      display.print(sliderSpeed);
      display.print("%");
      display.display();
    }
  }
  isScreenChanged = true;
  motorSpeed(sliderSpeed);
}

void adjustSpeedAut() {
  display.fillRect(49, 29, 16, 4, WHITE);
  display.display();

  while (1) {
    bool updateScr = false;
    if (updateCamera()) updateScr = true;
    display.fillRect(51, 4, 12, 22, BLACK);
    if (digitalRead(plusButton) == LOW) {
      debounceButton(plusButton);
      accCam++;
      if (accCam >= 20) accCam = 20;
      updateScr = true;
    }

    if (digitalRead(minusButton) == LOW) {
      debounceButton(minusButton);
      accCam--;
      if (accCam <= 1) accCam = 1;
      updateScr = true;
    }

    if (digitalRead(backButton)) {
      debounceButton(backButton);
      display.fillRect(49, 29, 16, 4, BLACK);
      display.fillRect(51, 4, 12, 22, BLACK);
      display.fillRect(52, 5, 9, 20, WHITE);
      display.fillRect(52, 5, 9, 20 - accCam, BLACK);
      display.fillTriangle(52, 25, 52, 5, 55, 25, BLACK);
      display.fillTriangle(61, 25, 61, 5, 58, 25, BLACK);
      isScreenChanged = true;
      display.drawRect(51, 30, 12, 3, WHITE);
      return;
    }

    if (updateScr) {
      display.fillRect(52, 5, 9, 20, WHITE);
      display.fillRect(52, 5, 9, 20 - accCam, BLACK);
      display.fillTriangle(52, 25, 52, 5, 55, 25, BLACK);
      display.fillTriangle(61, 25, 61, 5, 58, 25, BLACK);
      display.display();
    }
  }
  isScreenChanged = true;
  motorSpeed(sliderSpeed);
}

void changeDirectionHM() {
  if (sliderDirection == 1)sliderDirection = -1;
  else if (sliderDirection == -1)sliderDirection = 1;
  isScreenChanged = true;
  //clearing screen
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(4, 2);
  display.write(27);
  display.setCursor(4, 2);
  display.write(26);
  //
  if (sliderDirection == 1) {
    digitalWrite(dirPin, LOW);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(4, 2);
    display.write(27);
  }  else {
    digitalWrite(dirPin, HIGH);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(4, 2);
    display.write(26);
  }
}

void ChangeModeHM() {
  autMode = !autMode;
  isScreenChanged = true;
  display.fillRect(20, 5, 22, 18, BLACK);
  if (!autMode) {
    display.drawBitmap(20, 5,  manIcon, 22, 18, 1);
    display.display();
    int refreshrate = 4; //4
    int counter = 0;
    int pace = 0;
    while (1) {
      display.fillRect(94 + counter, 8, 4, 14, WHITE);
      display.fillRect(102 + counter, 8, 4, 14, WHITE);
      display.drawLine(68 + (counter * 1.75), 4, 68 + (counter * 1.75), 28, WHITE);     //dividing line
      if (pace == refreshrate) {
        pace = 0;
        updateCamera();
        display.display();
      }
      if (counter == 16) break;
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(55, 0);
      display.print("Speed:");
      int centerOfText;
      if (sliderSpeed == 100) centerOfText = 47;
      if (sliderSpeed < 100 && sliderSpeed >= 10) centerOfText = 55;
      if (sliderSpeed < 10) centerOfText = 59;
      display.setTextSize(2);
      display.setCursor(centerOfText, 10);
      display.print(sliderSpeed);
      display.print("%");
      display.drawLine(68 + (counter * 1.75), 4, 68 + (counter * 1.75), 28, BLACK);   //clearing
      display.fillRect(69 + (counter * 1.75), 0, 30, 29, BLACK);
      display.fillRect((68 + (counter * 1.75)) - (16 - counter), 0, (16 - counter), 29, BLACK);
      display.fillRect(65 + (counter * 1.75), 0, 6, 4, BLACK);
      display.fillRect(94 + counter, 8, 4, 14, BLACK);
      display.fillRect(102 + counter, 8, 4, 14, BLACK);
      counter++;
      pace++;
    }
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(55, 0);
    display.print("Speed:");
    int centerOfText;
    if (sliderSpeed == 100) centerOfText = 47;
    if (sliderSpeed < 100 && sliderSpeed >= 10) centerOfText = 55;
    if (sliderSpeed < 10) centerOfText = 59;
    display.setTextSize(2);
    display.setCursor(centerOfText, 10);
    display.print(sliderSpeed);
    display.print("%");
    display.display();
    handMode();
  } else {
    if (!isPlaying) changePlayPauseHM();
    display.drawBitmap(20, 5,  autIcon, 22, 18, 1);
    display.display();
    int refreshrate = 4;
    int counter = 0;
    int pace = 0;
    while (1) {
      display.fillRect(110 - counter, 8, 4, 14, WHITE);                                 //pause icon
      display.fillRect(118 - counter, 8, 4, 14, WHITE);                                 //pause icon
      display.drawLine(96 - (counter * 1.75), 4, 96 - (counter * 1.75), 28, WHITE);     //dividing line
      if (pace == refreshrate) {
        pace = 0;
        updateCamera();
        display.display();
      }
      if (counter == 16) break;
      display.drawLine(96 - (counter * 1.75), 4, 96 - (counter * 1.75), 28, BLACK);     //clearing dividing line
      display.fillRect(97 - (counter * 1.75), 0, refreshrate + 3, 29, BLACK);           //clearing right of the line
      display.fillRect((95 - (counter * 1.75)) - 15, 0, 15, 29, BLACK);                 //clearing left of the line
      display.fillRect(110 - counter, 8, 4, 14, BLACK);                                 //cleaing pause icon
      display.fillRect(118 - counter, 8, 4, 14, BLACK);                                 //clearing pause icon
      counter++;
      pace++;
    }
    display.fillRect(46, 0, 15, 32, BLACK);
    int risingMeter = 0;
    int renderTime = map(accCam, 1, 20, 2, 7);
    while (risingMeter != accCam + 1) {                                                 //just a fancy animation below
      display.fillRect(52, 5, 9, 20, WHITE);                                            //next4 line, drawing acceleration
      display.fillRect(52, 5, 9, 20 - risingMeter, BLACK);
      display.fillTriangle(52, 25, 52, 5, 55, 25, BLACK);
      display.fillTriangle(61, 25, 61, 5, 58, 25, BLACK);
      if (risingMeter % renderTime == 0) {
        display.display();
      }
      risingMeter++;
    }
    display.display();                                                                  //end of fancy animation
    automaticMode();
  }
}

void changePlayPauseHM() {
  isPlaying = !isPlaying;
  isScreenChanged = true;
  display.fillRect(110, 8, 12, 15, BLACK);
  if (isPlaying) {
    display.fillRect(110, 8, 4, 14, WHITE);
    display.fillRect(118, 8, 4, 14, WHITE);
    motorStop();
  } else {
    display.fillTriangle(110, 8, 110, 22, 120, 15, WHITE);
    motorStart(sliderSpeed);
  }
}


void changePlayPauseAut() {
  isPlaying = true;
  //isScreenChanged = true;
  display.fillRect(70, 0, 60, 29, BLACK); //clear the screen
  display.fillTriangle(94, 8, 94, 22, 104, 15, WHITE);  //draw play graphics below
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(75, 5);
  display.write(27);
  display.setCursor(109, 5);
  display.write(26);
  display.fillRect(72, 29, 58, 4, WHITE);
  display.display();
  long actualSpeed = ultraLowSpeed;
  long desiredSpeed = ultraLowSpeed;
  minusPressed = 0;
  plusPressed = 0;
  unsigned long startTime = millis();
  //motorStart(1);
  //motorDirectSpeed(actualSpeed);
  long lossOfSpeed = (ultraLowSpeed - maxSpeed) * 0.007;
  bool isMotorRunning = false;
  while (1) {

    if (plusPressed > 0 && sliderDirection == -1) {
      if ((accCam * 15) > desiredSpeed) desiredSpeed = maxSpeed;
      else desiredSpeed -= accCam * 15 * plusPressed;
      //+scaledSpeed = constrain(scaledSpeed, ultraLowSpeed, maxSpeed);
      if (desiredSpeed < maxSpeed) desiredSpeed = maxSpeed;
      plusPressed = 0;
    }
    if (minusPressed > 0 && sliderDirection == -1) {
      desiredSpeed += accCam * 15 * minusPressed;
      if (desiredSpeed >= ultraLowSpeed) desiredSpeed = ultraLowSpeed;
      minusPressed = 0;
    }
    if (plusPressed > 0 && sliderDirection == 1) {
      desiredSpeed += accCam * 15 * plusPressed;
      if (desiredSpeed >= ultraLowSpeed) desiredSpeed = ultraLowSpeed;
      plusPressed = 0;
    }
    if (minusPressed > 0 && sliderDirection == 1) {
      if ((accCam * 15) > desiredSpeed) desiredSpeed = maxSpeed;
      else desiredSpeed -= accCam * 15 * minusPressed;
      //+scaledSpeed = constrain(scaledSpeed, ultraLowSpeed, maxSpeed);
      if (desiredSpeed < maxSpeed) desiredSpeed = maxSpeed;
      minusPressed = 0;
    }

    if (checkEndstopsAut()){
      motorStop();
      desiredSpeed = ultraLowSpeed;
      actualSpeed = ultraLowSpeed;
    }
    
    if (millis() - startTime > 15) {                                //update speed just every 15ms
      startTime = millis();
      long speedGain = (desiredSpeed - actualSpeed) / 10;
      actualSpeed += speedGain;
      if (actualSpeed >= ultraLowSpeed) {                            //slowed to a crawl and must stop
        motorStop();
        isMotorRunning = false;
        while (1) {                                                 //changing direction
          if (digitalRead(plusButton) == 0) {
            if (sliderDirection == -1) break;
            else {
              changeDirectionHM();
              display.display();
              break;
            }
          }
          if (digitalRead(minusButton) == 0) {
            if (sliderDirection == -1) {
              changeDirectionHM();
              display.display();
              break;
            }
            else break;
          }
          if (digitalRead(backButton) == 0) break;
        }
      }
      if (!isMotorRunning && actualSpeed < ultraLowSpeed) {        //starting from stop
        motorStart(1);
        motorDirectSpeed(actualSpeed);
        isMotorRunning = true;
      }
      if (actualSpeed < ultraLowSpeed)desiredSpeed += lossOfSpeed ;      //natural loss of speed
      motorDirectSpeed(actualSpeed);
    }
    if (updateCamera()) display.display();          //update screen if icon of camera moves

    if (digitalRead(backButton)) {                  //back button return to main menu and changes graphcs to pause icon
      debounceButton(backButton);
      motorStop();
      display.fillRect(70, 0, 60, 29, BLACK);
      display.fillRect(72, 29, 58, 4, BLACK);
      display.drawRect(74, 30, 54, 3, WHITE);
      display.fillRect(94, 8, 4, 14, WHITE);
      display.fillRect(102, 8, 4, 14, WHITE);
      isScreenChanged = true;
      isPlaying = false;
      return;
    }

  }
  /*if (isPlaying) {
    display.fillRect(94, 8, 4, 14, WHITE);
    display.fillRect(102, 8, 4, 14, WHITE);
    } else {
    display.fillTriangle(94, 8, 94, 22, 104, 15, WHITE);
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(75, 5);
    display.write(27);
    display.setCursor(109, 5);
    display.write(26);
    }*/
  isPlaying = false;
}

bool updateCamera() {
  if (map(currentStep, 0, numberOfSteps, 92, 4) != cameraIconAt) {
    display.fillRect(cameraIconAt, 40, 20, 17, BLACK);            //delete original
    display.fillRect(4, 54, 108, 2, WHITE);                        //draw rod
    cameraIconAt = map(currentStep, 0, numberOfSteps, 92, 4);
    if (cameraIconAt <= 4) cameraIconAt = 4;
    if (cameraIconAt >= 92) cameraIconAt = 92;
    display.drawBitmap(cameraIconAt, 40, camera, 20, 17, WHITE);
    isScreenChanged = true;
    return true;
  }
  return false;
}

void plus() {
  plusPressed++;
}

void minus() {
  minusPressed++;
}

bool checkEndstops() {
  if (currentStep > numberOfSteps) {
    changePlayPauseHM();
    currentStep = numberOfSteps;
    changeDirectionHM();
    return true;
  }
  if (currentStep < 0) {
    changePlayPauseHM();
    currentStep = 0;
    changeDirectionHM();
    return true;
  }
  return false;
}

bool checkEndstopsAut() {
  if (currentStep > numberOfSteps) {
    currentStep = numberOfSteps;
    return true;
  }
  if (currentStep < 0) {
    currentStep = 0;
    return true;
  }
  return false;
}

void smartDelay(unsigned long duration) {
  /*unsigned long startingTime = millis();
    while ((millis() - startingTime) < duration){}*/
  delay(duration);    //this makes it just a regular delay. I wasnt sure if interrupt service routine would run on regular delay. That's why I made smartDelay which isn't actually necessary.
}

void debounceButton(int buttonInput) {
  if (buttonInput == 0 || buttonInput == 1) {
    smartDelay(3);
    while (digitalRead(buttonInput) == 0) {}
    smartDelay(3);
    return;
  }
  smartDelay(3);
  while (digitalRead(buttonInput)) {}
  smartDelay(3);
}

//----------------------------- Movement -------------------------------------

void findHome() {
  bool dirChanged = false;
  if (sliderDirection != -1) {
    changeDirectionHM();
    dirChanged = true;
  }
  int originalSpeed = sliderSpeed;
  sliderSpeed = 80;
  motorStart(sliderSpeed);
  while (digitalRead(homingSwitch) != 1) {}
  motorStop();
  currentStep = 0;
  sliderSpeed = originalSpeed;
  if (dirChanged) changeDirectionHM();
}

void motorStart(int startValue) {
  TCCR1A = 0;               //reset values
  TCCR1B = 0;               //reset values
  TCNT1  = 0;               //initiliaze counter

  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS11) | (1 << CS10);    // 64 prescaler
  TCCR1A |= (1 << COM1A0);  //toggle pin 9 on compare match
  TIMSK1 |= (1 << OCIE1A);   //enable compare interrupt
  motorSpeed(startValue);
  //interrupts();
}

void motorStop() {
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;
  //noInterrupts();
}

ISR (TIMER1_COMPA_vect) {
  currentStep += sliderDirection;
}

void motorSpeed(int Mspeed) {
  unsigned long scaledValue = map(Mspeed, 1, 100, minSpeed, maxSpeed);
  TCNT1  = 0;               //initiliaze counter
  OCR1A = scaledValue / 4;
}

void motorDirectSpeed(int Mspeed) {
  TCNT1  = 0;               //initiliaze counter
  OCR1A = Mspeed / 4;
}
