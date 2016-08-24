#ifndef __PWM_H__
#define __PWM_H__

#include <avr/io.h>
#include "CONFIG.h"
#include "port.h"


#define MAX_PWM		255

#ifdef USE_AVR_mega32

#define PWM_C_ON	{ /*TCCR1A = 0x81; TCCR2 = 0x41; PORTD &= ~0x90*/; }	//T2时钟源不分频
#define PWM_B_ON	{ /*TCCR1A = 0x21; TCCR2 = 0x41; PORTD &= ~0xA0*/; } 	//T2时钟源不分频
#define PWM_A_ON	{ /*TCCR1A = 0x01; TCCR2 = 0x61; PORTD &= ~0x30*/; }	//T2时钟源不分频
//OCR引脚全都和PWM脱开,作为普通GPIO引脚输出低电平
#define PWM_OFF		{ digitalWrite(9, LOW); digitalWrite(10, LOW); digitalWrite(5, LOW); }
#define ALLPIN_AND_PWM_OFF	{ digitalWrite(8, LOW); digitalWrite(11, LOW); digitalWrite(12, LOW); PWM_OFF; }

#endif





void PWM_Init();

void setPWM(uint8_t m);

#endif

