#include <Arduino.h>
#include "bouton.h"

void setup_bouton(int pin)
{
    pinMode(pin, INPUT);
}

int read_bouton(int pin)
{
    return digitalRead(pin);
}