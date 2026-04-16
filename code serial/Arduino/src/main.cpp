// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "incremental.h"
#include "bouton.h"
#include "accelerometer.h"
#include "bargraph.h"
#include <ArduinoJson.h>

/*---------------------------------------------------
VARIABLE ET FONCTION SERIAL
---------------------------------------------------*/
#define BAUD 115200        // Frequence de transmission serielle
volatile bool shouldSend_ = false;  // Drapeau prêt à envoyer un message
volatile bool shouldRead_ = false;  // Drapeau prêt à lire un message
void sendMsg(); 
void readMsg();
void serialEvent();
/*---------------------------------------------------
VARIABLES LCD
---------------------------------------------------*/
const int rs = 3, en = 4 , d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/*---------------------------------------------------
VARIABLES ENCODER
---------------------------------------------------*/
#define ENCODER_PIN_A 19   // doit être une pin interruption
#define ENCODER_PIN_B 29

/*---------------------------------------------------
VARIABLES BOUTONS
---------------------------------------------------*/
#define PIN_BOUTON1 39
#define PIN_BOUTON2 37 //à changer
#define PIN_BOUTON3 35
#define PIN_BOUTON4 33

/*---------------------------------------------------
VARIABLES ACCELEROMETER
---------------------------------------------------*/
#define X_AXIS_PIN A2
#define Y_AXIS_PIN A3
#define Z_AXIS_PIN A4

int RawMinRest = 255;
int RawMaxRest = 405;

/*---------------------------------------------------
VARIABLES JOYSTICK
---------------------------------------------------*/
#define X_potPin A0
#define Y_potPin A1

/*---------------------------------------------------
VARIABLES BARGRAPH
---------------------------------------------------*/
// BARGRAPH PINS --> bargraph.cpp
int const nbDELmax = 10;

/*---------------------------------------------------
VARIABLES LED
---------------------------------------------------*/
#define LED_PIN 30 

/*---------------------------------------------------
MUON
---------------------------------------------------*/
#define SUB_RESET_PIN 5
#define SUB_READ_PIN 6

/*---------------------------------------------------
SETUP
---------------------------------------------------*/
void setup()
{
  // SETUP LCD
  Serial.begin(BAUD);
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


  // SETUP BARGRAPH
  setup_bargraph(nbDELmax);
  //SETUP DEL
  pinMode(LED_PIN, OUTPUT);

  //Muon com
  pinMode(SUB_READ_PIN, INPUT);
  pinMode(SUB_RESET_PIN, OUTPUT);

}

/*---------------------------------------------------
LOOP
---------------------------------------------------*/
void loop() {

  if(shouldRead_){
    readMsg();
    sendMsg();
  }

  delay(10);  // delais de 10 ms
}

/*---------------------------------------------------
FONCTION
---------------------------------------------------*/
void serialEvent() { shouldRead_ = true; }

void sendMsg(){

  //Exemple de JSON : {"ENCODER":-627,"BTN_1":0,"BTN_2":0,"BTN_3":0,"BTN_4":0,"X_mG":-307,"Y_mG":-734,"Z_mG":-1800,"pot_X":259,"pot_Y":257}
  StaticJsonDocument<500> dataJSON;

  // LOOP ENCODER
  dataJSON["ENCODER"] = read_encoder();

  // LOOP BOUTONS
  dataJSON["BTN_1"] = read_bouton(PIN_BOUTON1);
  dataJSON["BTN_2"] = read_bouton(PIN_BOUTON2);
  dataJSON["BTN_3"] = read_bouton(PIN_BOUTON3);
  dataJSON["BTN_4"] = read_bouton(PIN_BOUTON4);

  // LOOP ACCELEROMETER
  int xRaw = ReadAxis(X_AXIS_PIN);
  int yRaw = ReadAxis(Y_AXIS_PIN);
  int zRaw = ReadAxis(Z_AXIS_PIN);

  // Convert raw values to 'milli-Gs"
  dataJSON["X_mG"] = map(xRaw, RawMinRest, RawMaxRest, -1000, 1000);
  dataJSON["Y_mG"] = map(yRaw, RawMinRest, RawMaxRest, -1000, 1000);
  dataJSON["Z_mG"] = map(zRaw, RawMinRest, RawMaxRest, -1000, 1000) - 1000;

  // LOOP POTENTIOMETRE
  dataJSON["pot_X"] = analogRead(X_potPin);  // Read potentiometer
  dataJSON["pot_Y"] = analogRead(Y_potPin);  // Read potentiometer


  dataJSON["Muon"] = digitalRead(SUB_READ_PIN);
  digitalWrite(SUB_RESET_PIN, HIGH);
  delayMicroseconds(100);
  digitalWrite(SUB_RESET_PIN, LOW);
  dataJSON["time"] = millis();


  // Serialisation
  serializeJson(dataJSON, Serial);

  // Envoie
  Serial.println();
  shouldSend_ = false;


}

void readMsg(){
  // Lecture du message Json
  StaticJsonDocument<500> dataJSON;
  JsonVariant parse_msg;

  // Lecture sur le port Serial
  DeserializationError error = deserializeJson(dataJSON, Serial);
  shouldRead_ = false;

  // Si erreur dans le message
  if (error) {
    Serial.print("deserialize() failed: ");
    Serial.println(error.c_str());
    return;
  }
  
  // Analyse des éléments du message message
  if (!parse_msg.isNull()) 
    updateBargraph(nbDELmax, dataJSON["BARGRAPH"].as<int>());

    // Analyse des éléments du message message
  if (!parse_msg.isNull()) 
      digitalWrite(LED_PIN,dataJSON["LED"].as<bool>());

}

/*void readMuon(){
  int static timerMuon = 0;
  int muonCheck = analogRead(A0);

  if (muonCheck < 620 && cycleMuon > 10) {
    muonDetected = true;
    cycleMuon = 0;
  } 
  else
    cycleMuon += 1;

}*/