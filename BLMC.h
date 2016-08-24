#ifndef __BLMC_H__
#define __BLMC_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include "CONFIG.h"
#include "port.h"
#include "pwm.h"
#include "analog.h"
#include "timer0.h"
#include "acmp.h"



void MotorTest();

uint8_t motorRampUp(uint8_t pm);



#endif

 
