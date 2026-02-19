/*

*/

// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>

#define REFRESH_RATE 100
#define X_AXIS_PIN A2
#define Y_AXIS_PIN A3
#define Z_AXIS_PIN A4

int ReadAxis(int axisPin);
void getMove();

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

/*void getMove()
{
  // Convert raw values to 'milli-Gs"
  long firstXScaled = map(ReadAxis(X_AXIS_PIN), RawMinRest, RawMaxRest, -1000, 1000);
  long firstYScaled = map(ReadAxis(Y_AXIS_PIN), RawMinRest, RawMaxRest, -1000, 1000);
  long firstZScaled = map(ReadAxis(Z_AXIS_PIN), RawMinRest, RawMaxRest, -1000, 1000);

  delay(50);

  // Convert raw values to 'milli-Gs"
  long secondXScaled = map(ReadAxis(X_AXIS_PIN), RawMinRest, RawMaxRest, -1000, 1000);
  long secondYScaled = map(ReadAxis(Y_AXIS_PIN), RawMinRest, RawMaxRest, -1000, 1000);
  long secondZScaled = map(ReadAxis(Z_AXIS_PIN), RawMinRest, RawMaxRest, -1000, 1000);

  long differenceX = firstXScaled - secondXScaled;
  lcd.setCursor(0,1);
  lcd.print("        ");
  lcd.setCursor(0,1);
  lcd.print(differenceX);
  lcd.setCursor(0,0);
  lcd.print("        ");
  lcd.setCursor(0,0);
  if (abs(differenceX) > 200)
  {
    if (differenceX > 0)
    {      
      lcd.print("Avant");
      delay(250);
    }
    else
    {
      lcd.print("Arrie");
      delay(250);
    }
  }
  else
  {
    lcd.print("Arret");
  }
}*/

void getMove()
{

  int xRaw = ReadAxis(X_AXIS_PIN);
  int yRaw = ReadAxis(Y_AXIS_PIN);
  int zRaw = ReadAxis(Z_AXIS_PIN);

  // Convert raw values to 'milli-Gs"
  long xScaled = map(xRaw, RawMinRest, RawMaxRest, -1000, 1000);
  long yScaled = map(yRaw, RawMinRest, RawMaxRest, -1000, 1000);
  long zScaled = map(zRaw, RawMinRest, RawMaxRest, -1000, 1000) - 1000;


  lcd.setCursor(0,0);
  if (abs(xScaled) > 300)
  {
    if (xScaled > 0)
    {      
      lcd.print("Avant  ");
    }
    else if (xScaled < 0)
    {
      lcd.print("Arriere");
    }
  }
  else
  {
    xRestCpt += 1;     
    if (xRestCpt > 500) 
    {
      lcd.print("       ");
      xRestCpt = 0;
    }
  }

  lcd.setCursor(9,0);
  if (abs(yScaled) > 300)
  {
    if (yScaled > 0)
    {      
      lcd.print("Gauche");
    }
    else if (yScaled < 0)
    {
      lcd.print("Droite");
    }
  }
  else
  {
    yRestCpt += 1;     
    if (yRestCpt > 500) 
    {
      lcd.print("      ");
      yRestCpt = 0;
    }
  }

  lcd.setCursor(0,1);
  if (abs(zScaled) > 400)
  {
    if (zScaled > 0)
    {      
      lcd.print("Bas ");
    }
    else if (zScaled < 0)
    {
      lcd.print("Haut");
    }
  }
  else
  {
    zRestCpt += 1;     
    if (zRestCpt > 500) 
    {
      lcd.print("    ");
      zRestCpt = 0;
    }
  }
}