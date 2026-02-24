#include <Arduino.h>
#include <LiquidCrystal.h>
#include "accelerometer.h"

extern LiquidCrystal lcd;

int RawMinRest = 255;
int RawMaxRest = 405;
const int sampleSize = 10;

int xRestCpt = 0;
int yRestCpt = 0;
int zRestCpt = 0;

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

void get_move(int X_AXIS, int Y_AXIS, int Z_AXIS)
{

  int xRaw = ReadAxis(X_AXIS);
  int yRaw = ReadAxis(Y_AXIS);
  int zRaw = ReadAxis(Z_AXIS);

  // Convert raw values to 'milli-Gs"
  long xScaled = map(xRaw, RawMinRest, RawMaxRest, -1000, 1000);
  long yScaled = map(yRaw, RawMinRest, RawMaxRest, -1000, 1000);
  long zScaled = map(zRaw, RawMinRest, RawMaxRest, -1000, 1000) - 1000;


  lcd.setCursor(0,0);
  if (abs(xScaled) > 300)
  {
    if (xScaled > 0)
    {      
      lcd.print("Avant   ");
      xRestCpt = 0; 
    }
    else if (xScaled < 0)
    {
      lcd.print("Arriere ");
      xRestCpt = 0;
    }
  }
  else
  {
    xRestCpt += 1;     
    if (xRestCpt > 500) 
    {
      lcd.print("        ");
      xRestCpt = 0;
    }
  }

  lcd.setCursor(9,0);
  if (abs(yScaled) > 300)
  {
    if (yScaled > 0)
    {      
      lcd.print("Gauche");
      yRestCpt = 0;
    }
    else if (yScaled < 0)
    {
      lcd.print("Droite");
      yRestCpt = 0;
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
      zRestCpt = 0;
    }
    else if (zScaled < 0)
    {
      lcd.print("Haut");
      zRestCpt = 0;
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