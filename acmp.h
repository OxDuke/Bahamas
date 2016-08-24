#ifndef __ACMP_H__
#define __ACMP_H__


#include <avr/io.h> 
#include <avr/interrupt.h>
#include "CONFIG.h"
#include "port.h"
#include "pwm.h"
#include "analog.h"

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

#define SET_COMPARATOR_INPUT_A ADMUX |= 0x00;	//set comparator negative input to NULL_A
#define SET_COMPARATOR_INPUT_B ADMUX |= 0x01;	//set comparator negative input to NULL_B
#define SET_COMPARATOR_INPUT_C ADMUX |= 0x04;	//set comparator negative input to NULL_C


#define IS_COMPARATOR_INTERRUPT_SET	 ((ACSR & 0x10))	//is comparator interrupt flag bit set
#define IS_COMPARATOR_OUTPUT_L		 (!(ACSR & 0x20))	//is comparator output low
#define IS_COMPARATOR_OUTPUT_H		 ((ACSR & 0x20))	//is comparator output high

//#define ENABLE_SENSE_INT 	{ ACSR |= 0x0A;  }	//enable comparator interrupt(falling edge interrupt)
#define ENABLE_COMPARATOR_INTERRUPT 	{ ACSR |= 0x08;  }	//enable comparator interrupt
#define DISABLE_COMPARATOR_INTERRUPT	{ ACSR &= ~0x08; }	//disable comparator interrupt

#define SET_COMPARATOR_FALLING_INT	{ ACSR &= ~0x01; }	//set comparator falling edge interrupt
#define SET_COMPARATOR_RISING_INT	{ ACSR |= 0x03;	 }	//set comparator rising edge interrupt
#define SET_COMPARATOR_TOGGLE_INT	{ ACSR &= ~0x03; }	//set comparator toggle interrupt

#define ENABLE_COMPARATOR_MUTIPLEXER { ADCSRB |= (1 << 6); }	//enable comparator multiplexer



void manualPhaseShift();


#endif

