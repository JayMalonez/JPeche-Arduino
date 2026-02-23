#include <Arduino.h>
#include <LiquidCrystal.h>
#include "bouton.h"

#define PIN_BOUTON1 29
#define PIN_BOUTON2 30 //à changer
#define PIN_BOUTON3 31
#define PIN_BOUTON4 32
const int rs = 3, en = 4 , d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {

  setup_bouton(PIN_BOUTON1);
  setup_bouton(PIN_BOUTON2);
  setup_bouton(PIN_BOUTON3);
  setup_bouton(PIN_BOUTON4);

  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pret...");
}

void loop()
{
  lcd.setCursor(0, 0);
  if (read_bouton(PIN_BOUTON1) == HIGH)
  {
    lcd.print("off");
  }
  else
  {
    lcd.print("on");
  }
  delay(50);

}