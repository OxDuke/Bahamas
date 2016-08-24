#ifndef __UART_H__
#define __UART_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include "CONFIG.h"
#include "port.h"
#include "pwm.h"
#include "BLMC.h"


#define BAUD_RATE	57600

#define MAX_RECV_SIZE 	50
#define MAX_SEND_SIZE	50

void Uart_Init();

void uart_putchar(uint8_t c);

void RxComdAction();

void uart_sendbuffer(uint8_t len);

void CalCrc(uint8_t len);

#endif

