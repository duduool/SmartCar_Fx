//======================================================================
//�������-MKL25Z128-����
//�汾��V1.0
//ʵ��ƽ̨��MKL25Z128ϵͳ��
//�Ա��꣺http://landzo.taobao.com/
//�޸ģ�2013.12.15
//ʹ��˵�������ļ����������Ƿ�˼�������ܳ��������ʹ�õ�����CCD�ɼ�������
//ʱ���źŲ����Ķ˿ڳ�ʼ����ADC�˿ڳ�ʼ����ʹ��ǰ����ccd.h�ļ��޸�����
//Ҫ�Ķ˿ڲ���,���õ�ʱ��ֻҪ��CCDPar��ַ��CCDInit���ɣ�Ȼ��ʹ��CCD_GetImage()
//������������
//======================================================================
#include "ccd.h"
#include "delay.h"
#include "adc.h"
#include "uart.h"
#include "stdio.h"

extern uint8_t THRESHOLD;
extern uint8_t IMAGE[COL];       //��������CCD��AD����ֵ

//�ڴ��޸Ķ˿ں�           
CCD_ConfigType  CCDParSet={ADC0,ADC_PRECISION_8BIT,SI_GPIO,SI_Pin,CK_GPIO,CK_Pin}; 
//�ɼ��˿�,�ɼ����ȣ�SI�źŶ˿ڣ�SI�˿����źţ�CLK�źŶ˿ڣ�CLK�˿����ź�

//======================================================================
//����CCD�ɼ���ʼ��
//��ڲ�����AD�ɼ��˿ڣ�ADCx
//          ���ȣ�Pre
//����ֵ  ����
//�������ܣ�ʱ���źŲ����Ķ˿ڳ�ʼ����ADC�˿ڳ�ʼ��
//======================================================================
void CCDInit(CCD_ConfigType *CCDPar)
{
	GPIO_InitTypeDef GPIO_InitStruct_SI;
	GPIO_InitTypeDef GPIO_InitStruct_CK;
	ADC_InitTypeDef CCDInitStruct;         //����CCD�ɼ���ʼ���ṹ��
	
	//ADC�˿ڳ�ʼ��	
	CCDInitStruct.ADCxMap = (uint32_t)CCDPar->ADCx;          //�˿�
	CCDInitStruct.ADC_Precision = CCDPar->Pre;               //����
	
	//ʱ�������ʼ��
	GPIO_InitStruct_SI.GPIO_Pin = SI_Pin;
	GPIO_InitStruct_SI.GPIO_InitState = Bit_RESET;
	GPIO_InitStruct_SI.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStruct_SI.GPIO_Mode = GPIO_Mode_OPP;
	GPIO_InitStruct_SI.GPIOx = CCDPar->SIGPIO;

	GPIO_InitStruct_CK.GPIO_Pin = CCDPar->CKPin;
	GPIO_InitStruct_CK.GPIO_InitState = Bit_RESET;
	GPIO_InitStruct_CK.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStruct_CK.GPIO_Mode = GPIO_Mode_OPP;
	GPIO_InitStruct_CK.GPIOx = CCDPar->CKGPIO;
	
	ADC_Init(&CCDInitStruct);              //��ַӳ��
	GPIO_Init(&GPIO_InitStruct_SI);
	GPIO_Init(&GPIO_InitStruct_CK);
}

//======================================================================
//�� �� ����     CCD_GetImage()                      
//��ڲ�����     ��             
//����ֵ  ��     ��
//�������ܣ�     CCD�����Լ�����                                                  
//======================================================================
void CCD_GetImage(uint32_t ADCx) 
{
	unsigned char i;
    
    SI_1;  //SI�˿���1
    CK_1;  //CK�˿���1
    SI_0;  //SI�˿���0
	
		
    for (i = 0; i < COL; i++) {
    //��ʱ������CCD�еĵ��������۵ĵ������ͬʱ���������Ƿ�ﵽ����״̬�����Բɼ����ߵ�״̬
    //����ʱ������ɼ����ڻ�䳤��������ʹ����������CCD��λ��������ʵ��ͼ��Ч��������ʱ. 
       CK_0;
                                    
       DelayUs(450);                 
                     
       IMAGE[i] =ADC_GetConversionValue(ADCx); 
        
       CK_1;
       DelayUs(450); 
    }
   
    CK_0;
	DelayUs(20); 
}

//����ͨ��
void SendImage(void)
{
	unsigned char i;
	char buf[5];
	    for (i = 0; i < COL; i++) {
//           if(IMAGE[i] == 1) 
//              UART_SendData((UART0_Type *)UART0,' ');
//           else
//              UART_SendData((UART0_Type *)UART0,'*');

					if(IMAGE[i] > THRESHOLD)
					sprintf(buf, " ");
				else
					sprintf(buf, "%02X ", IMAGE[i]);
				//sprintf(buf, "%02X ", IMAGE[i]);
				UART_printf(buf);
			//UART_SendData((UART0_Type *)UART0,'a' + (IMAGE[i] >> 4));
       }
      UART_printf("\n");
}

uint8_t getThreshold(void)
{
	unsigned char i;
	uint8_t min = IMAGE[0];
	uint8_t max = IMAGE[0];
	for (i = 1; i < COL; i++) {
		if(IMAGE[i] > max)
			max = IMAGE[i];
		else
		{
			if(IMAGE[i] < min)
				min = IMAGE[i];
		}
	}
	UART_printf("\n max=%d  min=%d\n",max,min);
	return (max + min)/2;
}
/*************************************************************************
*  �������ƣ�SendHex
*  ����˵�����ɼ���������
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2012-10-20
*  ��    ע��
*************************************************************************/
void SendHex(unsigned char hex) 
{
  unsigned char temp;
  temp = hex >> 4;
  if(temp < 10) {
    UART_SendData(UART0, temp + '0');
  } else {
    UART_SendData(UART0, temp - 10 + 'A');
  }
  temp = hex & 0x0F;
  if(temp < 10) {
    UART_SendData(UART0, temp + '0');
  } else {
   UART_SendData(UART0, temp - 10 + 'A');
  }
}

/*************************************************************************
*  �������ƣ�SendImageData
*  ����˵����
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2012-10-20
*  ��    ע��
*************************************************************************/
void SendImageData(unsigned char * ImageData) 
{
    unsigned char i;
    unsigned char crc = 0;

    /* Send Data */
    UART_SendData(UART0, '*');
    UART_SendData(UART0, 'L');
    UART_SendData(UART0, 'D');

    SendHex(0);
    SendHex(0);
    SendHex(0);
    SendHex(0);

    for(i=0; i<128; i++) {
      SendHex(*ImageData++);
    }

    SendHex(crc);
    UART_SendData(UART0,'#');
}
