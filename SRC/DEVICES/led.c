#include "led.h"

//======================================================================
// ��ڲ�����GPIO:PTA:A�˿� PTB:B�˿� PTC:C�˿� PTD:D�˿� PTE:E�˿�
//			 Pin������	 	
// ���ز�������
// ʵ��LED��˸ǰIO�ڵĳ�ʼ��
//======================================================================
void LEDInit(GPIO_Type *GPIO, uint8_t Pin){
	GPIO_InitTypeDef GPIO_InitStruct1;
	GPIO_InitStruct1.GPIO_Pin = Pin;
	GPIO_InitStruct1.GPIO_InitState = Bit_SET;
	GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_OPP;
	GPIO_InitStruct1.GPIOx = GPIO;
	GPIO_Init(&GPIO_InitStruct1);
}

//======================================================================
// ��ڲ�����GPIO:PTA:A�˿� PTB:B�˿� PTC:C�˿� PTD:D�˿� PTE:E�˿�
//			 Pin������	 	
// ���ز�������
// ʵ��LED��˸
//========================================================================
void TwinkleLed(GPIO_Type *GPIO,uint8_t Pin){
	GPIO_ToggleBit(GPIO,Pin);
}
