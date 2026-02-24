#include <Arduino.h>
#include "bargraph.h"

int const bargraphPins[] = {23, 25, 28, 24, 22, 17, 16, 15, 14, 2};

void setup_bargraph(int nbDELmax){
    for (int i = 0; i < nbDELmax; i++) {
        pinMode(bargraphPins[i], OUTPUT);
    }
}

void updateBargraph(int nbDELmax, int niveau) {
  for (int i = 0; i < nbDELmax; i++) {
    if (i < niveau) {
      digitalWrite(bargraphPins[i], HIGH);
    } else {
      digitalWrite(bargraphPins[i], LOW);
    }
  }
}