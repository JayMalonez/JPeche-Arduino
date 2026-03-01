/*
 * incremental.h
 * 
 * Serge Caron, feb 2026
 * 
 * Decode cheap rotary encoder. Contrary to a quadrature encoder, some rotary encoders always send 2 pulses per detend (1 falling edge and 1 rising edge per channel)
 * before going back to an idle state.
 * In such case, all you have to do is interrupt on the falling edge of one channel. When it occurs, the state of the second channel tells you the direction of rotation.
 * Original code is from there:
 * https://arduino.stackexchange.com/questions/61785/cant-you-easily-use-quadrature-incremental-rotary-encoders-at-2x-or-4x-their
 *
 * ArduinoMega digital pin usable for interrupt: 2, 3, 18, 19, 20, 21
 */

#ifndef INCREMENTAL_H
#define INCREMENTAL_H

void encode();  // The interrupt. Leave it alone
int setup_encoder(int intPinA, int PinB);   // intPinA must be an interrupt pin. Return -1 if interrupt could not be setup.
int read_encoder();
void write_encoder(int value);

#endif


/* endoffile */
