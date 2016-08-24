#include "acmp.h"

extern volatile uint8_t Phase;
extern volatile uint8_t CountT2Overflow;
extern uint8_t  StromConvertCmd;
extern uint16_t commutationCount;
extern uint8_t  InterT2Cnt;


//analog comparator interrupt handler
SIGNAL(SIG_COMPARATOR)
{
	uint8_t sense = 0;
	static uint8_t rpmctcnt = 0, lastt2cnt = 0, currt2cnt = 0;

	do {
		if (IS_COMPARATOR_OUTPUT_H)
			sense = 1;
		else
			sense = 0;

		switch (Phase)
		{

		//current:A-VCC,B-GND => next:A-VCC,C-GND
		//next zero-pass event: B-rising edge(falling edge in comparator output)
		case 0:
			PWM_A_ON;
			if (sense)
			{
				MUTEX_NEG_C_ON;
				if (StromConvertCmd)
					NormalStromConvert();
				SET_COMPARATOR_FALLING_INT;
				SET_COMPARATOR_INPUT_B;
				Phase++;
				commutationCount++; rpmctcnt++;
			}
			else
			{
				MUTEX_NEG_B_ON;
			}
			break;

		//current:A-VCC,C-GND => next:B-VCC,C-GND
		//next zero-pass event: A-falling edge(rising edge in comparator output)
		case 1:
			MUTEX_NEG_C_ON;
			if (!sense)
			{
				PWM_B_ON;
				if (StromConvertCmd) NormalStromConvert();
				SET_COMPARATOR_INPUT_A;
				SET_COMPARATOR_RISING_INT;
				Phase++;
				commutationCount++; rpmctcnt++;
			}
			else
			{
				PWM_A_ON;
			}
			break;

		//current:B-VCC,C-GND => next:B-VCC,A-GND
		//next zero-pass event: C-rising edge(falling edge in comparator output)
		case 2:
			PWM_B_ON;
			if (sense)
			{
				MUTEX_NEG_A_ON;
				if (StromConvertCmd) NormalStromConvert();
				SET_COMPARATOR_INPUT_C;
				SET_COMPARATOR_FALLING_INT;
				Phase++;
				commutationCount++; rpmctcnt++;
			}
			else
			{
				MUTEX_NEG_C_ON;
			}
			break;

		//current:B-VCC,A-GND => next:C-VCC,A-GND
		//next zero-pass event: B-falling edge(rising edge in comparator output)
		case 3:
			MUTEX_NEG_A_ON;
			if (!sense)
			{
				PWM_C_ON;
				if (StromConvertCmd) NormalStromConvert();
				SET_COMPARATOR_INPUT_B;
				SET_COMPARATOR_RISING_INT;
				Phase++;
				commutationCount++; rpmctcnt++;
			}
			else
			{
				PWM_B_ON;
			}
			break;

		//current:C-VCC,A-GND => next:C-VCC,B-GND
		//next zero-pass event: A-rising edge(falling edge in comparator output)
		case 4:
			PWM_C_ON;
			if (sense)
			{
				MUTEX_NEG_B_ON;
				if (StromConvertCmd) NormalStromConvert();
				SET_COMPARATOR_INPUT_A;
				SET_COMPARATOR_FALLING_INT;
				Phase++;
				commutationCount++; rpmctcnt++;
			}
			else
			{
				MUTEX_NEG_A_ON;
			}
			break;

		//current:C-VCC,B-GND => next:A-VCC,B-GND
		//next zero-pass event: C-falling edge(rising edge in comparator output)
		case 5:
			MUTEX_NEG_B_ON;
			if (!sense)
			{
				PWM_A_ON;
				if (StromConvertCmd) NormalStromConvert();
				SET_COMPARATOR_INPUT_C;
				SET_COMPARATOR_RISING_INT;
				Phase = 0;
				commutationCount++; rpmctcnt++;
			}
			else
			{
				PWM_C_ON;
			}
			break;
		}


	} while ((IS_COMPARATOR_OUTPUT_L && sense) || (IS_COMPARATOR_OUTPUT_H && ! sense));

	if (rpmctcnt == 6)
	{
		rpmctcnt = 0;
		currt2cnt = CountT2Overflow;
		InterT2Cnt = currt2cnt - lastt2cnt;
		lastt2cnt = currt2cnt;
	}
}



/**
 * switch MOSFETs according to current phase
 */
void manualPhaseShift() {
	switch (Phase)
	{
	//current:A-VCC,B-GND => next:A-VCC,C-GND
	//next zero-pass event: B-rising edge(falling edge in comparator output)
	case 0:
		PWM_A_ON;
		MUTEX_NEG_B_ON;
		SET_COMPARATOR_INPUT_C;
		SET_COMPARATOR_RISING_INT;
		break;

	case 1:	//AC
		PWM_A_ON;
		MUTEX_NEG_C_ON;
		SET_COMPARATOR_INPUT_B;
		SET_COMPARATOR_FALLING_INT;
		break;

	case 2:	//BC
		PWM_B_ON;
		MUTEX_NEG_C_ON;
		SET_COMPARATOR_INPUT_A;
		SET_COMPARATOR_RISING_INT;
		break;

	case 3:	//BA
		PWM_B_ON;
		MUTEX_NEG_A_ON;
		SET_COMPARATOR_INPUT_C;
		SET_COMPARATOR_FALLING_INT;
		break;

	case 4:	//CA
		PWM_C_ON;
		MUTEX_NEG_A_ON;
		SET_COMPARATOR_INPUT_B;
		SET_COMPARATOR_RISING_INT;
		break;

	case 5:	//CB
		PWM_C_ON;
		MUTEX_NEG_B_ON;
		SET_COMPARATOR_INPUT_A;
		SET_COMPARATOR_FALLING_INT;
		break;
	}
}
