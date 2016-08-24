#ifndef __PORT_H__
#define __PORT_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include "CONFIG.h"
#include "timer0.h"

/**
 * port mapping table for Arduino Micro
 *
 * <1> | <2> | <3> | <4>  |    <5>   |<6>
 * ----+-----+-----+------+----------+---------
 *  A+ | PB5 | D9  | OC1A |
 *  B+ | PB6 | D10 | OC1B |
 *  C+ | PC6 | D5  | 0C3A |
 *  A- | PB4 | D8  |      |
 *  B- | PB7 | D11 |      |
 *  C- | PD6 | D12 |      |
 *
 * <1> name
 * <2> pin name on the Atmega32U4 chip
 * <3> pin name on the Ardu Microboard
 * <4> PWM register used
 *
 */

#ifdef USE_AVR_mega32
#define LEDGRN_ON	{ /*PORTA &= ~0x10*/; }		//绿灯亮
#define LEDGRN_OFF	{ /*PORTA |=  0x10*/; }		//绿灯暗
#define LEDRED_ON	{ /*PORTA &= ~0x08*/; }		//红灯亮
#define LEDRED_OFF	{ /*PORTA |=  0x08*/; }		//红灯暗

#define POS_ABC_OFF		{ analogWrite(9, 0); analogWrite(10, 0); analogWrite(5, 0); }	//场效应管A+,B+,C+全关
#define NEG_ABC_OFF		{ digitalWrite(8, LOW); digitalWrite(11, LOW); digitalWrite(12, LOW); } //场效应管A-,B-,C-全关
#define FETS_OFF		{ NEG_ABC_OFF; POS_ABC_OFF; }	//所有6个场效应管全关

#define POS_A_ON		{ /*PORTD |= 0x80*/; }	//A+开
#define POS_B_ON		{ /*PORTD |= 0x10*/; }	//B+开	
#define POS_C_ON		{ /*PORTD |= 0x20*/; }	//C+开

#define NEG_A_ON		{ digitalWrite(8, HIGH); }	//A-开
#define NEG_B_ON		{ digitalWrite(11, HIGH); }	//B-开
#define NEG_C_ON		{ digitalWrite(12, HIGH); }	//C-开

#define MUTEX_NEG_A_ON	{ digitalWrite(11, LOW); digitalWrite(12, LOW); NEG_A_ON; }	//互斥型A-开
#define MUTEX_NEG_B_ON	{ digitalWrite(8, LOW); digitalWrite(12, LOW); NEG_B_ON; }	//互斥型B-开
#define MUTEX_NEG_C_ON	{ digitalWrite(8, LOW); digitalWrite(11, LOW); NEG_C_ON; }	//互斥型C-开

#endif



void Port_Init();

void LedRedBlink(uint8_t anz);


#endif
