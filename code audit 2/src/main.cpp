// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "incremental.h"
#include "bouton.h"
#include "accelerometer.h"
#include "bargraph.h"

/*---------------------------------------------------
VARIABLES LCD
---------------------------------------------------*/
const int rs = 3, en = 4 , d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/*---------------------------------------------------
VARIABLES ENCODER
---------------------------------------------------*/
#define ENCODER_PIN_A 19   // doit être une pin interruption
#define ENCODER_PIN_B 27

int lastValue_encoder = 9999;

/*---------------------------------------------------
VARIABLES BOUTONS
---------------------------------------------------*/
#define PIN_BOUTON1 29
#define PIN_BOUTON2 31 //à changer
#define PIN_BOUTON3 33
#define PIN_BOUTON4 35

/*---------------------------------------------------
VARIABLES ACCELEROMETER
---------------------------------------------------*/
#define REFRESH_RATE 100
#define X_AXIS_PIN A2
#define Y_AXIS_PIN A3
#define Z_AXIS_PIN A4

unsigned long startTime = 0;

long somme = 0;
long cptSomme = 0;

/*---------------------------------------------------
VARIABLES POTENTIOMETRES
---------------------------------------------------*/
#define potPin A1

int lastValue_pot = 9999;

/*---------------------------------------------------
VARIABLES BARGRAPH
---------------------------------------------------*/
// BARGRAPH PINS --> bargraph.cpp
int const nbDELmax = 10;

/*---------------------------------------------------
SETUP
---------------------------------------------------*/
void setup()
{
  // SETUP LCD
  Serial.begin(9600);
  lcd.begin(16, 2);

  // SETUP ENCODER
  if(setup_encoder(ENCODER_PIN_A, ENCODER_PIN_B) == -1)
  {
    lcd.print("Erreur ENCOD!");
    while(1); // stop
  }

  // SETUP BOUTONS
  setup_bouton(PIN_BOUTON1);
  setup_bouton(PIN_BOUTON2);
  setup_bouton(PIN_BOUTON3);
  setup_bouton(PIN_BOUTON4);

  // SETUP ACCELEROMETER
  startTime = millis();

  // SETUP BARGRAPH
  setup_bargraph(nbDELmax);

}

/*---------------------------------------------------
LOOP
---------------------------------------------------*/
void loop()
{
  // LOOP ENCODER
  int value_encoder = read_encoder();

  if(value_encoder != lastValue_encoder)
  {
    lcd.setCursor(0,0);
    lcd.print("Encodeur        ");
    lcd.setCursor(12,0);
    lcd.print(value_encoder);
    lastValue_encoder = value_encoder;
  }

  int value_bouton1 = read_bouton(PIN_BOUTON1);
  int value_bouton2 = read_bouton(PIN_BOUTON2);
  int value_bouton3 = read_bouton(PIN_BOUTON3);
  int value_bouton4 = read_bouton(PIN_BOUTON4);

  // LOOP BOUTONS
  if((value_bouton1 == LOW || value_bouton2 == LOW || value_bouton3 == LOW || value_bouton4 == LOW) && value_encoder <= 100)
  {
    lcd.setCursor(0,0);
    lcd.print("Bouton(s) ON   ");

    if(value_bouton1 == LOW){
      lcd.setCursor(0,1);
      lcd.print("1");
    }else{
      lcd.setCursor(0,1);
      lcd.print(" ");
    }

    if(value_bouton2 == LOW){
      lcd.setCursor(2,1);
      lcd.print("2");
    }else{
      lcd.setCursor(2,1);
      lcd.print(" ");
    }

    if(value_bouton3 == LOW){
      lcd.setCursor(4,1);
      lcd.print("3");
    }else{
      lcd.setCursor(4,1);
      lcd.print(" ");
    }

    if(value_bouton4 == LOW){
      lcd.setCursor(6,1);
      lcd.print("4");
    }else{
      lcd.setCursor(6,1);
      lcd.print(" ");
    }
  }
  else if(value_encoder <= 100)
  {
    lcd.setCursor(0,1);
    lcd.print("                ");  // Clear
  }

  // LOOP ACCELEROMETER
  if(value_encoder > 100 && value_encoder <= 200){
    get_move(X_AXIS_PIN, Y_AXIS_PIN, Z_AXIS_PIN);
  }

  // LOOP POTENTIOMETRE
  int value_pot = analogRead(potPin);  // Read potentiometer

  if(value_encoder > 200 && value_pot != lastValue_pot && (value_pot < lastValue_pot-5 || value_pot > lastValue_pot+5)){
    lcd.setCursor(0,0);
    lcd.print("Potentiometre   ");

    lcd.setCursor(0,1);
    lcd.print("                ");  // Clear
    lcd.setCursor(0, 1);
    lcd.print(value_pot);   // Print new value
    lastValue_pot = value_pot;
  }

  // LOOP BARGRAPH
  int niveau = map(value_pot, 0, 1023, 0, nbDELmax);
  updateBargraph(nbDELmax, niveau);
}