// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "incremental.h"

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 3, en = 4 , d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Encoder pins
const int pinA = 19;   // doit être une pin interruption
const int pinB = 27;

int lastValue = 9999;  // pour éviter rafraîchissement inutile

void setup()
{
  lcd.begin(16, 2);
  lcd.print("Encodeur:");

  //setup de l'encodeur
  if(setup_encoder(pinA, pinB) == -1)
  {
    lcd.setCursor(0,1);
    lcd.print("Erreur ENCOD!");
    while(1); // stop
  }
}

void loop()
{
  // lecture de l'encodeur à chaque loop
  int value = read_encoder();

  // utilisation de l'encodeur
  if(value != lastValue)
  {
    lcd.setCursor(0,1);
    lcd.print("                ");  // Clear
    lcd.setCursor(0,1);
    lcd.print(value);
    lastValue = value;
  }
}