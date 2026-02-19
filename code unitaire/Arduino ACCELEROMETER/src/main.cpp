/*

*/

// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>

#define Z_AXIS_PIN A0
#define REFRESH_RATE 100
#define X_AXIS_PIN A2
#define Y_AXIS_PIN A3
#define Z_AXIS_PIN A4

int ReadAxis(int axisPin);

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 3, en = 4 , d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
unsigned long startTime = 0;
int RawMinRest = 260;
int RawMaxRest = 410;
long somme = 0;
long cptSomme = 0;
const int sampleSize = 10;

void setup() {

  Serial.begin(115200); // open the serial port at 9600 bps:
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  startTime = millis();
}

void loop() {

  //Read raw values
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

  Serial.print("X, Y, Z  :: ");
  Serial.print(xRaw);
  Serial.print(", ");
  Serial.print(yRaw);
  Serial.print(", ");
  Serial.print(zRaw);
  Serial.print(" :: ");
  Serial.print(xAccel );
  Serial.print("G, ");
  Serial.print(yAccel );
  Serial.print("G, ");
  Serial.print(zAccel);
  Serial.println("G");
}

// Take samples and return the average
int ReadAxis(int axisPin) {
  long reading = 0;
  analogRead(axisPin);
  delay(1);
  for (int i = 0; i < sampleSize; i++) {
    reading += analogRead(axisPin);
  }
  return reading / sampleSize;
}