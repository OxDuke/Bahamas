#include "uart.h"

uint8_t RxBuffer[MAX_RECV_SIZE];
uint8_t TxBuffer[MAX_SEND_SIZE];
static uint8_t rx_ptr = 0;		//��ǰ���ջ�����ָ��
static uint8_t tx_ptr = 0;		//��ǰ���ͻ�����ָ��
static uint8_t tx_len = 0;		//���ͱ��ĳ���

extern uint8_t  PWM;
extern uint8_t  PwmSet;
extern uint8_t  PwmSetValue;
extern uint8_t  MotorAddress;
extern uint16_t SIO_Timeout;



//�����жϷ������
SIGNAL(SIG_UART_RECV) {
	uint8_t rdat = UDR;

	if(rx_ptr < MAX_RECV_SIZE)		
		RxBuffer[rx_ptr++] = rdat;
	SIO_Timeout = 4;
}

//�����жϷ������
SIGNAL(SIG_UART_TRANS) {
	if(tx_ptr < tx_len) {
		UDR = TxBuffer[tx_ptr++];
	} else {
		tx_ptr = 0;	tx_len = 0;
	}
}


//��ʼ������
void Uart_Init() {
	UCSRA |= (1<<U2X);					//���ٷ���
	UCSRB |= (1<<TXEN)  | (1<<RXEN);	//����ʹ��
	UCSRB |= (1<<TXCIE) | (1<<RXCIE);	//�����жϡ������ж�ʹ��
	UCSRC |= (1<<UCSZ1) | (1<<UCSZ0);	//8λ����λ

	UBRRH = 0;
	UBRRL = (SYSCLK/((uint32_t)BAUD_RATE * 8) - 1);	//����������
}

//����һ���ֽ�
void uart_putchar(uint8_t c) {
	UDR = c;
}

//����һ�α���
void uart_sendbuffer(uint8_t len) {
	tx_len = len;
	tx_ptr = 0;
	UDR = TxBuffer[tx_ptr++];
	CalCrc(len);
}

//����CRCУ����
void CalCrc(uint8_t len) {
	uint8_t i;
	uint16_t crcsum = 0;

	if(len <= 2) return ;
	for(i = 0; i < len - 2; i++)
		crcsum += TxBuffer[i];
	
	TxBuffer[len - 2] = crcsum / 0x100;
	TxBuffer[len - 1] = crcsum % 0x100;
}

//���CRCУ����
uint8_t CheckCrcOk() {
	uint8_t  i;
	uint16_t crcsum = 0;

	if(rx_ptr <= 2) return 0;

	for(i = 0; i < rx_ptr-2; i++)
		crcsum += RxBuffer[i];

	if(crcsum%0x100 == RxBuffer[rx_ptr-1] && crcsum/0x100 == RxBuffer[rx_ptr-2]) 
		return 1;

	return 0;
}

void RxComdAction() {
	if(MotorAddress == RxBuffer[0]) {
		if(CheckCrcOk()) {
			uint16_t cdat = (uint16_t)RxBuffer[2]*0x100+ RxBuffer[3];
			switch(RxBuffer[1]) {
				//��LED����
				case 1:
					if(cdat == 1) LEDRED_ON;
					if(cdat == 0) LEDRED_OFF;
					break;

				//��LED����
				case 2:
					if(cdat == 1) LEDGRN_ON;
					if(cdat == 0) LEDGRN_OFF;
					break;	

				//A+�ܽţ��������ã�
				case 3:
					if(cdat == 1) POS_A_ON;
					if(cdat == 2) PWM_A_ON;
					break;

				//B+�ܽţ��������ã�
				case 4:
					if(cdat == 1) POS_B_ON;
					if(cdat == 2) PWM_B_ON;
					break;

				//C+�ܽţ��������ã�
				case 5:
					if(cdat == 1) POS_C_ON;
					if(cdat == 2) PWM_C_ON;
					break;

				//A-�ܽţ��������ã�
				case 6:
					switch(cdat) {
						case 0:	break;
						case 1: NEG_A_ON;	   	break;
						case 2: MUTEX_NEG_A_ON;	break;
					}
					break;

				//B-�ܽţ��������ã�
				case 7:
					switch(cdat) {
						case 0: break;
						case 1: NEG_B_ON;		break;
						case 2: MUTEX_NEG_B_ON;	break;
					}
					break;

				//C-�ܽţ��������ã�
				case 8:
					switch(cdat) {
						case 0: break;
						case 1: NEG_C_ON;		break;
						case 2: MUTEX_NEG_C_ON; break;
					}
					break;

				//FETS���������ã�
				case 9:
					switch(cdat) {
						case 0: FETS_OFF; 	  		break;
						case 1: POS_ABC_OFF;  		break;
						case 2: NEG_ABC_OFF;  		break;
						case 3: PWM_OFF;	  		break;
						case 4: ALLPIN_AND_PWM_OFF;	break;
					}
					break;

				//PWM���ã��������ã�
				case 10:
					SetPWM((uint8_t)cdat);
					break;

				//�������������������ã�
				case 11:
					MotorStartUp(cdat);
					ALLPIN_AND_PWM_OFF;
					PWM = 0; SetPWM(PWM);									
					break;
	
				//�������PWM��������
				case 12:
					PwmSet = 1;
					PwmSetValue = cdat;
					break;

				//����
				default:
					break;
			} //end of Switch
		} //end of CrcCheck
	} //end of RxBuffer[0]
	
	//������ջ�����ָ��
	rx_ptr = 0;
}

