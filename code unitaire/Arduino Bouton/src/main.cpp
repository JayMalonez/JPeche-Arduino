#include <Arduino.h>
#include <LiquidCrystal.h>

const int PIN = 29;
const int rs = 3, en = 4 , d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {

  pinMode(PIN, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pret...");
}

void Bouton_on_off()
{
  lcd.setCursor(0, 0);
  if (digitalRead(PIN) == HIGH)
  {
    lcd.print("offe");
  }
  else
  {
    lcd.print("onne");
  }
}

void loop()
{
  Bouton_on_off();
  delay(50);
  
}