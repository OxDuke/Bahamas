
//status
#define STATUS_STOPPED 0
#define STATUS_NEED_TO_RAMP_UP 1
#define STATUS_RAMPING_UP 2
#define STATUS_RUNNING 3
#define STATUS_NEED_TO_STOP 4

uint8_t statusFlag = STATUS_STOPPED;

uint8_t PWMValue = 0;

volatile uint8_t currentPhase = 0, lastPhase = 0;

void setup()
{
	Serial.begin(115200);

	pinsInit();

	// set status to STATUS_NEED_TO_STOP
	statusFlag = STATUS_NEED_TO_STOP;
	PWMValue = 0;
	currentPhase = 0; lastPhase = 0;

	ENABLE_COMPARATOR_MUTIPLEXER;


}
void loop()
{
	;
	//<1>. get PWM value from master
	PWMValue = getCommandPWMValue();

	//<2>. according to PWMValue, ramp up or stay running or stay stopped
	if (PWMValue < 5)
	{
		if (statusFlag != STATUS_STOPPED)
			statusFlag = STATUS_NEED_TO_STOP;
	}
	else
	{
		if (statusFlag == STATUS_STOPPED)
			statusFlag = STATUS_NEED_TO_RAMP_UP;
	}

	//<3>. do things according to statusFlag, this is a status machine
	switch (statusFlag)
	{

	case STATUS_STOPPED:
		setPWM(0);
		//need add: stopped signal
		break;

	case STATUS_NEED_TO_RAMP_UP:
		//ramp up motor with PWM =5
		uint8_t result;
		result = motorRampUp(10);
		if (result == TRUE) // ramp up a success
		{
			statusFlag = STATUS_RAMPING_UP;

			currentPhase--;
			SET_COMPARATOR_TOGGLE_INT;
			ENABLE_COMPARATOR_INTERRUPT; //let comparator take control

			statusFlag = STATUS_RUNNING;

			//current amp check, prevent stuck
			if (getAmps(20) > 15)
			{
//				raiseEmergency(EMERGENCY_AMP_TOO_HIGH);
				statusFlag = statusFlag = STATUS_NEED_TO_STOP;
			}
		}
		else //ramp up a failure
		{
			setPWM(0);
			statusFlag = STATUS_NEED_TO_STOP;
			//need add: ramp up failure signal
		}
		break;

	case STATUS_RAMPING_UP:
		break;

	case STATUS_RUNNING:
		setPWM(PWMValue);

		if (getAmps(4) > 15)
		{
//			raiseEmergency(EMERGENCY_AMP_TOO_HIGH);
			statusFlag = statusFlag = STATUS_NEED_TO_STOP;
		}

		//need add: calculate RPMs
		//need add: running signal
		break;

	case STATUS_NEED_TO_STOP:
		setPWM(0);
		DISABLE_COMPARATOR_INTERRUPT;
		delay(50);
		statusFlag = STATUS_STOPPED;
		break;
	}

}

void pinsInit()
{

	//postive MOSFET control pins, with PWM
	pinMode(9, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(5, OUTPUT);
	analogWrite(9, 0);
	analogWrite(10, 0);
	analogWrite(5, 0);

	//negative MOSFET control pins, without PWM
	pinMode(8, OUTPUT);
	pinMode(11, OUTPUT);
	pinMode(12, OUTPUT);
	digitalWrite(8, LOW);
	digitalWrite(11, LOW);
	digitalWrite(12, LOW);

	//analog comparator pins
	pinMode(7, INPUT); //postive input
	pinMode(A5, INPUT); //negative input
	pinMode(A4, INPUT); //negative input
	pinMode(A3, INPUT); //negative input

	//current sensing input
	pinMode(A2, INPUT); //A2 pin, using ADC5 as current sensing input
}

void setPWM(uint8_t targetPWM)
{
	uint8_t tmp = targetPWM;

	if (tmp_pwm > MaxPWM)
	{
		tmp_pwm = MaxPWM;
//		LEDRED_ON;
	}

	analogWrite(9, tmp_pwm);
	analogWrite(10, tmp_pwm);
	analogWrite(5, tmp_pwm);
}


void raiseEmergency(uint8_t emergencyCode)
{
	;
}
void motorEmergencyStop()
{
	;
}