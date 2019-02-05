/* This program currently doesn't use the graphical interface. 
The points are added in the setup() and SetEachTime() calculates each step 
and pushes it trough serial port. You can also uncomment the makeStep() and
it will move the motor but it'll also wait between each step so it might
take a while
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

const int stepPin = 3;
const int dirPin = 4;
const int plusButton = 12;
const int minusButton = 11;
const int horizon = 5;
const int addPointButton = 6;
const int removePointButton = 7;
const int confirmButton = 8;
const int backButton = 9;

const unsigned long numberOfSteps = 31000;
//int motorSpeed = 1500;
int pointX[10]; //these are positions of the points on the OLED screen
int pointY[10];
// 10 is an arbitrary number and in hindsight, I would go for more

unsigned int hours = 0;
unsigned int minutes = 0;
unsigned long seconds = 20;

void setup() {
  Serial.begin(57600);
  //display setup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //you might have to adjust the address based on the oled you have
  display.clearDisplay();
  SetTopText("READY");
  display.drawLine(0, 8, 128, 8, WHITE);
  display.display();
  //
  ResetPoints();
  AddPoint(10, 15); //10,35
  AddPoint(25, 40);
  DeletePoint(3);
  //AddPoint(70,55);
  ClearTopText();
  SetTopText(GetNumberOfPoints());
  DrawPoints();
  DrawLines();
  display.display();
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  SetEachTime();
}

void loop() {
  //SetEachTime();
}

void SerialPoints() { //send coordinates of points trough serail port
  Serial.print(F("X: "));
  for (int i = 0; i < 10 ; i++) {
    Serial.print(pointX[i]);
    Serial.print(F(" | "));
  }
  Serial.print(F("          Y: "));
  for (int i = 0; i < 10 ; i++) {
    Serial.print(pointY[i]);
    Serial.print(F(" | "));
  }
  Serial.println();
}

void DrawPoints() {
  if (GetNumberOfPoints() == 0) return;
  for (int i = 0; i <= GetNumberOfPoints() - 1; i++) {
    display.drawCircle(pointX[i], pointY[i], 2, WHITE);
  }
}

void DrawLines() {
  if (GetNumberOfPoints() <= 1) return;
  for (int i = 0; i <= GetNumberOfPoints() - 2; i++) {
    display.drawLine(pointX[i], pointY[i], pointX[i + 1], pointY[i + 1], WHITE);
  }
}

void ClearGraph() {
  display.fillRect(0, 9, 128, 64, BLACK);
}

void SetTopText(int shift, long line) {
  String myString = String(line);
  display.setTextColor(WHITE);
  display.setCursor(shift, 0);
  display.println(myString);
}

void SetTopText(int shift, String line) {
  display.setTextColor(WHITE);
  display.setCursor(shift, 0);
  display.println(line);
}

void SetTopText(String line) {
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(line);
}

void SetTopText(int line) {
  String myString = String(line);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(myString);
}

void ClearTopText() {
  display.fillRect(0, 0, 128, 8, BLACK);
}

void ClearTopText(int start, int finish) {
  display.fillRect(start, 0, finish, 8, BLACK);
}

void ResetPoints() { //clears all points and creates only straight line trouh the middle
  pointX[0] = 0;   //0
  pointY[0] = 36;  //28
  pointX[1] = 127; //127
  pointY[1] = 36;  //28
  for (int i = 2; i < 10; i++) {
    pointX[i] = -1;
    pointY[i] = -1;
  }
}

int GetNumberOfPoints() {
  int points = 0;
  int counter = 0;
  while (1) {
    if (pointX[counter] != -1) points++;
    else break;
    counter++;
  }
  return points;
}

void AddPoint(int x, int y) {
  if (GetNumberOfPoints() >= 9) {
    ClearTopText();
    SetTopText(F("Can't add more points"));
    return;
  }
  //First we need to find between which one it'll fit
  int pos = 0;
  while (1) {
    if (pointX[pos] < x) {
      if (pointX[pos] == -1) break;
      pos++;
    }
    else break;
  }

  //Check if point exist on this spot already
  if (pointX[pos] == x) {
    ClearTopText();
    SetTopText(F("Point already exist"));
    return;
  }

  MakeRoomForPoint(pos);
  pointX[pos] = x;
  pointY[pos] = y;
}

void MakeRoomForPoint(int index) {
  if (GetNumberOfPoints() >= 10) {
    ClearTopText();
    SetTopText(F("Can't add more points"));
    return;
  }

  for (int i = 9; i >= index ; i--) {
    if (pointX[i] != -1) {
      pointX[i + 1] = pointX[i];
      pointY[i + 1] = pointY[i];
    }
  }
}

void DeletePoint(int index) {
  if (index < 0 || index > 10) return;
  //
  for (int i = index + 1; i < 10 ; i++) {
    pointX[i - 1] = pointX[i];
    pointY[i - 1] = pointY[i];
  }
  pointX[9] = -1;
  pointY[9] = -1;
}

//-----------------------------------  UI  ----------------------------------------------

void DrawCursor(int x, int y) {
  display.drawLine(x - 2, y, x + 2, y, WHITE);
  display.drawLine(x, y - 2, x, y + 2, WHITE);
}

void PointsMenu() { // graphical interface - drawing points. currently unused
  /*  int Xpos = 64;
    int Ypos = 33;
    unsigned long timer = millis();
    int consecutivePresses = 0;
    while(1){
      ClearGraph();
      ClearTopText();
      SetTopText(9-GetNumberOfPoints());
      SetTopText(10,F("Points left"));

      if(digitalRead(plusButton)){
        if(millis() - timer < 100) consecutivePresses++;
        else consecutivePresses = 0;
        if(consecutivePresses > 30) consecutivePresses = 30;
        timer = millis();
        if(digitalRead(horizon)){
          Xpos+= consecutivePresses/5 + 1;
          if(Xpos > 127) Xpos = 127;
        }
        else{
          Ypos-= consecutivePresses/5 + 1;
          if(Ypos < 10) Ypos = 10;        }
      }
      if(digitalRead(minusButton)){
        if(millis() - timer < 100) consecutivePresses++;
        else consecutivePresses = 0;
        if(consecutivePresses > 30) consecutivePresses = 30;
        timer = millis();
        if(digitalRead(horizon)){
          Xpos-=consecutivePresses/5 + 1;
          if(Xpos < 0) Xpos = 0;
        }
        else{
          Ypos+=consecutivePresses/5 + 1;
          if(Ypos > 63) Ypos = 63;
        }
      }
      if(digitalRead(addPointButton)){
        AddPoint(Xpos,Ypos);
      }
      if(digitalRead(removePointButton)){
        display.fillRect(Xpos-5,9,10,55,WHITE);
        for(int i=0; i<GetNumberOfPoints();i++){
          if(pointX[i]>Xpos-5 && pointX[i]<Xpos+5) DeletePoint(i);
        }
      }
      if(digitalRead(confirmButton)){
        TimeMenu();
      }
      //displaying

      DrawCursor(Xpos,Ypos);
      DrawPoints();
      DrawLines();
      display.display();
      //SerialPoints();
    }*/
}

void TimeMenu() { // graphical interface - setting time. currently unused
  /*if(GetNumberOfPoints() < 2){  //in case only one or none points are set
    ClearTopText();
    SetTopText(F("Adding points..."));
    display.display();
    delay(750);
    ClearTopText();
    SetTopText(F("Added automatic points"));
    ClearGraph();
    AddPoint(0,36);
    AddPoint(127,36);
    DrawPoints();
    DrawLines();
    }

    unsigned long timer = millis();
    int consecutivePresses = 0;
    DrawPoints();
    DrawLines();
    while(1){
    if(digitalRead(plusButton)){
      if(millis() - timer < 100) consecutivePresses++;
      else consecutivePresses = 0;
      if(consecutivePresses > 40) consecutivePresses = 40;
      timer = millis();
      //
      if(seconds < 60){
        seconds++;
        if(seconds > 86400) seconds = 86400;
        minutes =0;
      }
      else{
        seconds+=60+(consecutivePresses/5)*60;
        if(seconds > 86400) seconds = 86400;
        minutes = seconds/60;
        hours = minutes/60;
      }
    }

    if(digitalRead(minusButton)){
      if(millis() - timer < 100) consecutivePresses++;
      else consecutivePresses = 0;
      if(consecutivePresses > 40) consecutivePresses = 40;
      timer = millis();
      //
      if(seconds < 60){
        seconds--;
        if(seconds < 2) seconds = 2;
        minutes =0;
      }
      else{
        seconds-=60+(consecutivePresses/5)*60;
        if(seconds < 3) seconds = 3;
        minutes = seconds/60;
        hours = minutes/60;
      }
    }

    if(digitalRead(backButton))return;
    if(digitalRead(confirmButton)){
      while(digitalRead(confirmButton)){}
      delay(5);
      SetEachTime();
    }
    //
    ClearTopText();
    byte mShift=0;
    byte hShift=0;
    byte sShift=0;
    if(hours%60 < 10){
      SetTopText(F("0"));
      hShift = 7;
    }
    if(minutes%60 < 10){
      SetTopText(28,F("0"));
      mShift = 7;
    }
    if (seconds%60 < 10){
      SetTopText(56,F("0"));
      sShift = 7;
    }
    SetTopText(hShift,hours%60);
    SetTopText(14,F("h:"));
    SetTopText(28+mShift,minutes%60);
    SetTopText(42,F("m:"));
    SetTopText(56+sShift,seconds%60);
    SetTopText(70,F("s"));
    display.display();
    }*/
}

void SetEachTime() {
  //scaling the graph to appropriate size
  long scaledPointX[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  long scaledPointY[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  for (int i = 0; i < GetNumberOfPoints(); i++) {
    scaledPointX[i] = map(pointX[i], 0, 128, 0, numberOfSteps);
    scaledPointY[i] = map(pointY[i], 10, 64, numberOfSteps * 0.078125, numberOfSteps / 2);
  }
  // assigning how much time each line in the graph gets by calculating volume under the graph
  unsigned long volume[10];
  float portion[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  long combinedVolume = 0;
  //int numberOfLines = 0;
  for (int i = 0; i < GetNumberOfPoints() - 1; i++) {
    int x = pointX[i + 1] - pointX[i];
    int y = 64 - ((pointY[i] + pointY[i + 1]) / 2);
    volume[i] = x * y;                            // calculating volume under curve to asssign time
    combinedVolume += volume[i];
    //numberOfLines++;
  }
  for (int i = 0; i < GetNumberOfPoints(); i++) {
    float percentage = (float(volume[i]) / float(combinedVolume)) * 100;
    portion[i] = float(seconds) / 100 * percentage; //actual time for each line
  }
  bool straightLine = true; //They are all straight duh, horizontal line would probably be better
  int curveIndex = 0;       // This is just counter of how many non horizontal lines are in the graph
  for (int i = 0; i < GetNumberOfPoints() - 1; i++) {
    if (pointY[i] != pointY[i + 1]) {
      //straightLine = false;
      //break;
    }
    curveIndex++;
  }
  //This is the part where I'm bad at math and it doesn't work
  //Each line is defined with linear function y=ax+b
  //I don't know how much deeper I should go because it doesn't work so...
  
  for (int currentLine = 0; currentLine < (GetNumberOfPoints()-1); currentLine++) {
    //find 2 points
    unsigned long pointAy = 0;
    unsigned long pointAd = 0;
    unsigned long pointBy = 0;
    unsigned long pointBd = 0;
    //
    unsigned long midPointX = ((scaledPointX[currentLine] + scaledPointX[currentLine + 1]) / 2)-scaledPointX[currentLine];
    unsigned long midPointY = ((scaledPointY[currentLine] + scaledPointY[currentLine + 1]) / 2);
    if (GetNumberOfPoints() > 2) {
      //point A calculations
      pointAy = midPointY / 2;
      pointAd = (portion[currentLine] * 0.25) / (float(midPointX)) * 1000000;
      //point B calculations
      pointBy = ((scaledPointY[currentLine + 1] - scaledPointY[currentLine])+ midPointY) / 2;
      pointBd = (portion[currentLine] * 0.75) / ((float(scaledPointX[currentLine + 1])-float(scaledPointX[currentLine])) - float(midPointX)) * 1000000;

      Serial.print(scaledPointX[currentLine]);
      Serial.print(F(" | "));
      Serial.print(scaledPointY[currentLine]);
      Serial.print(F(" | "));
      Serial.print(scaledPointX[currentLine+1]);
      Serial.print(F(" | "));
      Serial.print(scaledPointY[currentLine+1]);
      Serial.print(F(" | mid point X: "));
      Serial.print(midPointX);
      Serial.print(F(" | mid point Y: "));
      Serial.println(midPointY);

      //y=mx+b This way we define the scale for speed using the points above
      float m = (float(scaledPointY[currentLine+1]) - float(scaledPointY[currentLine])) / (float(scaledPointX[currentLine+1])-float(scaledPointX[currentLine]));
      long b = scaledPointY[currentLine];
      b = scaledPointY[currentLine];
      Serial.print(F(" | m: "));
      Serial.print(m, 4);
      Serial.print(F(" | b: "));
      Serial.println(b);

      //
      //MoveOneLine(m,b,scaledPointX[0],scaledPointY[0],scaledPointX[1],scaledPointY[1]);
      //-----------------------------------------------------------------------------------
      unsigned long scalingPointX = (scaledPointX[currentLine+1]-scaledPointX[currentLine] ) / 2;
      float scalingB = 100 - (m * scalingPointX);
      float ratio = (portion[currentLine] * 1000 * 1000) / ((float(scaledPointX[currentLine+1]) - float(scaledPointX[currentLine])) * 100);

      Serial.print(F(" | scalingPointX: "));
      Serial.print(scalingPointX);
      Serial.print(F(" | seconds "));
      Serial.println(seconds);
      Serial.print(F(" | portion "));
      Serial.println(portion[currentLine]);
      Serial.print(F(" | scalingB: "));
      Serial.print(scalingB);

      int numberOfSteps = scaledPointX[currentLine+1] - scaledPointX[currentLine];
      float combinedTimes = 0;
      float t0 = (float(b) * float(numberOfSteps) / 1000000.0);
      Serial.print(F(" | t0: "));
      Serial.println(t0, 6);
      ratio =(((portion[currentLine] * float(b)) / t0) / float(b));
      ratio = 1;

      Serial.print(F(" | ratio "));
      Serial.println(ratio, 6);
      Serial.print(F(" | number of steps: "));
      Serial.println(numberOfSteps);
      for (int i = -numberOfSteps / 2; i <= numberOfSteps / 2; i++ ) {
        float portionOfTime = (m * i + b) * (ratio);
        //MakeStep(portionOfTime);
        Serial.print(F(" | "));
        Serial.print(portionOfTime);
        combinedTimes += portionOfTime;
      }
      Serial.println();
      Serial.print("All times combined are(us):");
      Serial.println(combinedTimes);
      Serial.println(F("-------------------------------------"));
    }
  }
}

void MoveOneLine(float m, long b, long x0, long y0, long x1, long y1) {
  /*
    float localM = (float(y1)-float(y0))/(float(x1)-float(x0));
    //long localB = y0 - (localM*x0);
    Serial.println();
    Serial.print(F(" | local m: "));
    Serial.print(localM);
    //Serial.print(F(" | local b: "));
    //Serial.println(localB);
    unsigned long counter = x0;
    while(counter <= x1){
      long y = localM*counter+localB;
      //long Delay = m*y+b;
      long Delay = (float(counter)-float(b))/float(m);
      Serial.print(Delay);
      Serial.print(F(" | "));
      //MakeStep(Delay);
      counter++;
    }
    Serial.println();
    Serial.print(F(" | combined: "));
    Serial.print(counter);*/
}

//----------------------------- Movement -------------------------------------

void LinearMovement(unsigned long Delay) {
  /*
    ClearTopText();
    SetTopText(F("Linear"));
    display.display();
    delay(500);
    ClearTopText();
    SetTopText(F("delay:"));
    SetTopText(42,Delay);
    display.display();
    delay(500);*/
}

void MakeStep(unsigned long Delay) {
  unsigned long actualyDelay = Delay / 2;
  if (actualyDelay > 15000) {
    actualyDelay = actualyDelay / 1000;
    digitalWrite(stepPin, HIGH);
    delay(actualyDelay);
    digitalWrite(stepPin, LOW);
    delay(actualyDelay);
  }
  else {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(actualyDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(actualyDelay);
  }
}
