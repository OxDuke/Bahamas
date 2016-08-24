#ifndef ANALOG_H
#define ANALOG_H

#include <avr/io.h> 
#include "port.h"

/**
 * mapping of pins on Arduino Micro which carrys ATmega32U4
 * all pins that could be used as negative input for 
 * analog comparator is listed bellow:
 *
 *   <1> |  <2> |<3> |   <4>  | <5>
 * ------+------+----+--------+------
 *   PE6 | AIN0 | D7 | MIDDLE | postive input
 *   PF0 | ADC0 | A5 | NULL_A | negative input
 *   PF1 | ADC1 | A4 | NULL_B | negative input
 *   PF2 | ADC4 | A3 | NULL_C | negative input
 *   PF3 | ADC5 | A2 | ACS712 | current sense input
 *   PF4 | ADC6 | A1 |        |
 *   PF5 | ADC7 | A0 |        |
 *  
 * <1> pin name on the Atmega32U4 chip
 * <2> ADC name
 * <3> pin name on the Arduino Micro board
 * <4> connection with the zero-pass detect curcuit
 * <5> attribute
 *
 */

float getAmps(uint8_t sampleTimes); 

float getADCVolt(uint8_t channel);


#endif /* ANALOG_H */


