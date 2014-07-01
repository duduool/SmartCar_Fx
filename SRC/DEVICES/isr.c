#include "isr.h"
#include "adc.h"
#include "ccd.h"
#include "pit.h"
#include "led.h"
#include "pwm.h"
#include "pid.h"
#include "cntl.h"
#include "delay.h"

//======================================================================
//PIT�ж����ú���
//��ڣ�PITͨ����PITx  ��ʱ������timePIT
//���أ���
//======================================================================
void Config_PIT(uint8_t PITx , uint32_t timePIT)
{
	PIT_InitTypeDef PITInit;                  //����pit���ýṹ
	PITInit.PITx=PITx;                        //pitͨ��
	PITInit.PIT_Interval=timePIT;             //pitʱ����
	PIT_Init(&PITInit);                       //��������
	PIT_ITConfig( PITx,PIT_IT_TIF,ENABLE );   //�����ж�
    /* ���²����ڳ�ʼ����������У����������жϿ������������ */
	NVIC_EnableIRQ(PIT_IRQn);                 //�������ȹ���
	PIT_Start(PITx);                          //����PIT
}

/* ����С�� */
static void TwinkleLED_Callback(void)
{
    ITStatus state;
 	state = PIT_GetITStatus(PIT0, PIT_IT_TIF);
    
    if(state == SET) {
		TwinkleLed(PTA,14);
 	}
}

//======================================================================
//��ʱ�ж��¼�����
//��ڣ���
//���أ���
//======================================================================
void PIT_IRQHandler(void)
{
    /* С�Ƴ��������������ж� */
    TwinkleLED_Callback();
    PIT_ClearITPendingBit(PIT0, PIT_IT_TIF);
}

//======================================================================
//�͹��Ķ�ʱ���ж��¼�����
//��ڣ���
//���أ���
//======================================================================
void LPTimer_IRQHandler(void)
{
    LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;
}
