#include "analog.h"

/**
 * adapted to arduino micro board which using a ATmega32U4 chip
 *  > Aref=5V
 *    the default analog reference is of 5 volts (on 5V Arduino boards)
 */


/**
 * read volt on a channel
 * @param  channel channel number mapping follows
 *                 0~1:ADC0~ADC1, 2~3:N/A, 4~7:ADC4~ADC7
 * @return         ADC value in voltage, max: 5V
 */
float getADCVolt(uint8_t channel)
{
	uint8_t temp;

	temp  = ADMUX;   //store ADMUX, because ADC might using another pin
	ADMUX |= channel; //channel selection, ADMUX[4:0]: Analog Channel Selection Bits
	ADMUX |= (1 << 6); //set Aref=5V, ADMUX[7:6]Reference Selection Bits
	ADCSRB &= ~(1 << 6); //disable analog comparator multiplexer, ADCSRB[6]:ACME
	ADCSRA = 0xD3; // ADC enable, ADC start conversion, clear ADC interrupt flag, ADC Prescaler set to 8, ADCSRA=11010011b

	ADCSRA |= 0x10;	 //clear interrupt flag
	ADMUX |= channel; //might be commented
	ADCSRA |= 0x40; //start a ADC conversion
	while (((ADCSRA & 0x10) == 0)); //wait until conversion complete

	//after conversion
	ADMUX = temp; //restore original ADMUX value
	ADCSRA = 0;
	ADCSRB |= (1 << 6); //enable analog comparator multiplexer

	//ADCW = Vin * 1023 / Vref
	float volt = ((float)ADCW) * 5.0f / 1023; //ADCW is ADCH and ADCL combined, in 16bits
	return volt;
}

/**
 * return averaged current value in amps
 * @param  sampleTimes sample times
 * @return             averaged current value in amps
 */
float getAmps(const uint8_t sampleTimes)
{
	float amps = 0.0f;
	for (int i = 0; i < sampleTimes; ++i)
	{
		/**
		 * ACS712-x20A characteristics:
		 *  > full range: -20A ~ +20A
		 *  > sensitivity: 100mV/A
		 *  > typical value: output = 2.5V@current = 0A
		 *  				 output = 0.5V@current = -20A
		 *  				 output = 4.5V@current = +20A
		 */
		amps += getADCVolt(5); //channel 5 is current sensing channel
	}

	amps = amps / sampleTimes; //arithmic mean
	amps = (amps - 2.5f) / 0.1f;

	return amps;
}




