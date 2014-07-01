#include "adc.h"
#include "led.h"
#include "pit.h"
#include "isr.h"
#include "pwm.h"
#include "ccd.h"
#include "cntl.h"
#include "uart.h"
#include "delay.h"

/* Core Clock = 100MHZ, Bus Clock = 100/3MHZ
   PWM = 100HZ, MOTOR = 20KHZ    
*/
extern uint8_t IMAGE[COL];  
extern uint8_t Mid[6];
extern uint16_t duty[];

int main(void)
{
    uint8_t i = 0, mid;
    i = i;
    DisableInterrupts();
        
    /* ѡ���ⲿ50M���񣬲����ں˿ɳ�Ƶ��100M */
    SystemClockSetup(ClockSource_EX50M, CoreClock_100M);
    /* ��ʼ�����Դ��� */
    UART_PortInit(UART0_RX_PD06_TX_PD07, 9600);
    /* ��ʱ������ʼ�� */
    DelayInit();
    /* �����ʼ�� */
    Steer_Init();
    /* �����ʼ�� */
    Motor_Init();
    /* LED PTA14��ʼ�� */
    LEDInit(PTA, 14);
    /* ��ʼ��CCD */
    CCDInit(&CCDParSet);
    /* ��ӡCPUʱ����Ϣ */
    //DisplayCPUInfo();
    /* ��ֹ2S������ */
    //DelayMs(2000);
    
    EnableInterrupts();
    
    /* �������ڶ�ʱ�жϣ�ÿ��1ms����һ���ж� */
    //Config_PIT(PIT0,80);
        
    while(1) {
        #if 1
        /* Sampling CCD data */
        //for (i = 0; i < 6; i++) {
            CCD_GetImage((uint32_t)ADC0_SE8A_PB0);
            Mid_Filter();
         Binarization();
            SendImage();
            mid = Get_MidLine();
        //    Mid[i] = Get_MidLine();
        //}
			  
        /* ������� */
        //Steer_PIDx(Average());
        Steer_Out(duty[ABS(mid - 64)]);
        #else
        /* Tesing steer */
        for (i = 0; i < 128; i++) {
            Steer_Out(duty[i]);
            DelayMs(500);
        }
        #endif
    }
}
