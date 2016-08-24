#include "pwm.h"


extern uint8_t  MaxPWM;

//��ʼ��PWM����
void PWM_Init() {
	PWM_OFF;
	TCCR1B = 1;		//T1����Ƶ
}


//����PWMռ�ձ�
void setPWM(uint8_t targetPWM) {
	uint8_t tmp = targetPWM;

	if(tmp_pwm > MaxPWM) 
	{
		tmp_pwm = MaxPWM;
//		LEDRED_ON;
	}

	analogWrite(9, tmp_pwm); 
	analogWrite(10, tmp_pwm); 
	analogWrite(5, tmp_pwm);
}


