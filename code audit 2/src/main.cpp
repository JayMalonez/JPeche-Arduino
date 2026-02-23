// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "incremental.h"
#include "bouton.h"
#include "accelerometer.h"

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
#define PIN_BOUTON2 30 //à changer
#define PIN_BOUTON3 31
#define PIN_BOUTON4 32

/*---------------------------------------------------
VARIABLES ACCELEROMETER
---------------------------------------------------*/
#define REFRESH_RATE 100
#define X_AXIS_PIN A2
#define Y_AXIS_PIN A3
#define Z_AXIS_PIN A4

unsigned long startTime = 0;
int RawMinRest = 255;
int RawMaxRest = 405;
long somme = 0;
long cptSomme = 0;
const int sampleSize = 10;

int xRestCpt = 0;
int yRestCpt = 0;
int zRestCpt = 0;

/*---------------------------------------------------
SETUP
---------------------------------------------------*/
void setup()
{
  // SETUP LCD
  Serial.begin(9600);
  lcd.begin(16, 2);

  // SETUP ENCODER
  if(setup_encoder(pinA, pinB) == -1)
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
    lcd.print("Encodeur")
    lcd.setCursor(0,1);
    lcd.print("                ");  // Clear
    lcd.setCursor(0,1);
    lcd.print(value);
    lastValue_encoder = value_encoder;
  }

  int value_bouton1 = read_bouton(PIN_BOUTON1);
  int value_bouton2 = read_bouton(PIN_BOUTON2);
  int value_bouton3 = read_bouton(PIN_BOUTON3);
  int value_bouton4 = read_bouton(PIN_BOUTON4);

  // LOOP BOUTONS
  if(value_bouton1 == LOW || value_bouton2 == LOW || value_bouton3 == LOW || value_bouton4 == LOW)
  {
    lcd.setCursor(0,0);
    lcd.print("Bouton(s) ON   ");
    lcd.setCursor(0,1);
    lcd.print("                ");  // Clear line 2

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
  else
  {
    lcd.setCursor(0,0);
    lcd.print("                ");  // Clear line 1
    lcd.setCursor(0,1);
    lcd.print("                ");  // Clear line 2
  }
}