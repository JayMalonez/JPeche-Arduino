/*

*/

// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "accelerometer.h"

#define REFRESH_RATE 100
#define X_AXIS_PIN A2
#define Y_AXIS_PIN A3
#define Z_AXIS_PIN A4

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 3, en = 4 , d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
unsigned long startTime = 0;
int RawMinRest = 255;
int RawMaxRest = 405;
long somme = 0;
long cptSomme = 0;
const int sampleSize = 10;

int xRestCpt = 0;
int yRestCpt = 0;
int zRestCpt = 0;

void setup() {

  Serial.begin(115200); // open the serial port at 9600 bps:
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  startTime = millis();
}

void loop() {

  /*//Read raw values
  int xRaw = ReadAxis(X_AXIS_PIN);
  int yRaw = ReadAxis(Y_AXIS_PIN);
  int zRaw = ReadAxis(Z_AXIS_PIN);

  // Convert raw values to 'milli-Gs"
  long xScaled = map(xRaw, RawMinRest, RawMaxRest, -1000, 1000);
  long yScaled = map(yRaw, RawMinRest, RawMaxRest, -1000, 1000);
  long zScaled = map(zRaw, RawMinRest, RawMaxRest, -1000, 1000);

  // re-scale to fractional Gs
  float xAccel = xScaled / 1000.0;
  float yAccel = yScaled / 1000.0;
  float zAccel = zScaled / 1000.0;


  lcd.setCursor(0,0);
  lcd.print("X ");
  lcd.print(xAccel, 1);
  lcd.setCursor(9,0);
  lcd.print("Y ");
  lcd.print(yAccel, 1);
  lcd.setCursor(0,1);
  lcd.print("Z ");
  lcd.print(zAccel, 1);*/

  getMove();

}

