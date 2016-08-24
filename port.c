#include "port.h"



//初始化各IO端口
void Port_Init() {

	//++++++++++++++++++++mega32+++++++++++++++++++++++
	#ifdef USE_AVR_mega32
		DDRA  = 0x18;
		PORTA = 0;

		DDRB  = 0x13;
		PORTB = 0xE0;
		
		DDRC  = 0;
		PORTC = 3;

		DDRD  = 0xBA;
		PORTD = 0x40;
	#endif


} 

//红LED闪烁指定次数
void LedRedBlink(uint8_t anz) {
	sei();
	while(anz--) {
		LEDRED_ON;
		Delay_ms(300);
		LEDRED_OFF;
		Delay_ms(300);
	}
	Delay_ms(1000);
}

