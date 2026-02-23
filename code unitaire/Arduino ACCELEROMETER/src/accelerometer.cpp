#include <Arduino.h>
#include "accelerometer.h"

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