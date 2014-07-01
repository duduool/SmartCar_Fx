#include "gpio.h"
 
//===========================================================================================
//GPIO��ʼ��
//PTX:X�˿�
//===========================================================================================
void GPIO_Init(GPIO_InitTypeDef* GPIO_InitStruct)
{
	GPIO_Type *GPIOx = NULL;
	PORT_Type *PORTx = NULL;
	GPIOx = GPIO_InitStruct->GPIOx;
	//���˿�ʱ��
	switch((uint32_t)GPIOx)
	{
        case PTA_BASE:PORTx=PORTA;SIM->SCGC5|=SIM_SCGC5_PORTA_MASK;break;   //����PORTA��ʹ��ʱ�ӣ�������ǰ���ȿ���ʹ��ʱ�Ӳμ�k10�ֲ�268ҳ��
        case PTB_BASE:PORTx=PORTB;SIM->SCGC5|=SIM_SCGC5_PORTB_MASK;break;	//����PORTB��ʹ��ʱ��
        case PTC_BASE:PORTx=PORTC;SIM->SCGC5|=SIM_SCGC5_PORTC_MASK;break;	//����PORTC��ʹ��ʱ��
        case PTD_BASE:PORTx=PORTD;SIM->SCGC5|=SIM_SCGC5_PORTD_MASK;break;	//����PORTD��ʹ��ʱ��
        case PTE_BASE:PORTx=PORTE;SIM->SCGC5|=SIM_SCGC5_PORTE_MASK;break;	//����PORTE��ʹ��ʱ��
        default : break;
	} 
	//����ΪGPIOģʽ
	PORTx->PCR[GPIO_InitStruct->GPIO_Pin]&=~(PORT_PCR_MUX_MASK);    
	PORTx->PCR[GPIO_InitStruct->GPIO_Pin]|=PORT_PCR_MUX(1); 
	//ȷ�������뻹�����
	if((GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OOD) || (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OPP))
	{
		//����GPIOx�ڵĵ�GPIO_Pin����Ϊ���
		GPIOx->PDDR |= (1<<(GPIO_InitStruct->GPIO_Pin));	
		//��Ϊ�����ʱ�رո����ŵ����������蹦��
        PORTx->PCR[(GPIO_InitStruct->GPIO_Pin)]&=~(PORT_PCR_PE_MASK); 
		//�����ƽ����
		(Bit_SET == GPIO_InitStruct->GPIO_InitState)?(GPIOx->PDOR |= (1<<(GPIO_InitStruct->GPIO_Pin))):(GPIOx->PDOR &= ~(1<<(GPIO_InitStruct->GPIO_Pin)));
		//��©�����������
		if(GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OOD)
		{
            //ORTx->PCR[GPIO_InitStruct->GPIO_Pin]|= PORT_PCR_ODE_MASK;
		}
		else if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OPP)
		{
            //PORTx->PCR[GPIO_InitStruct->GPIO_Pin]&= ~PORT_PCR_ODE_MASK;
		}
	}
	//���������ģʽ
	else if ((GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IN_FLOATING) || (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD) || (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU))
	{
		//����GPIOx�ڵĵ�GPIO_Pin����Ϊ����
		GPIOx->PDDR &= ~(1<<(GPIO_InitStruct->GPIO_Pin));		
		if(GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IN_FLOATING)
		{
			//�ر�����������
			PORTx->PCR[GPIO_InitStruct->GPIO_Pin]&=~PORT_PCR_PE_MASK; 	//����������DISABLE
		}
		else if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD) //����
		{
			//������������
			PORTx->PCR[GPIO_InitStruct->GPIO_Pin]|= PORT_PCR_PE_MASK; 	//����������ʹ��
			PORTx->PCR[GPIO_InitStruct->GPIO_Pin]&= ~PORT_PCR_PS_MASK;
		}
		else if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU) //����
		{
			//������������
			PORTx->PCR[GPIO_InitStruct->GPIO_Pin]|= PORT_PCR_PE_MASK; 	//����������ʹ��
			PORTx->PCR[GPIO_InitStruct->GPIO_Pin]|= PORT_PCR_PS_MASK;
		}
	}
	//�����ж�ģʽ
	PORTx->PCR[GPIO_InitStruct->GPIO_Pin]&=~PORT_PCR_IRQC_MASK;
	PORTx->PCR[GPIO_InitStruct->GPIO_Pin]|=PORT_PCR_IRQC(GPIO_InitStruct->GPIO_IRQMode);//�ⲿ�жϴ������� 
}


//============================================================================================
// GPIO����жϱ�־
// GPIOx:x�˿�
//============================================================================================
ITStatus GPIO_GetITStates(GPIO_Type *GPIOx,uint16_t GPIO_Pin)
{
	PORT_Type *PORTx = NULL;
	//���˿�ʱ��
	switch((uint32_t)GPIOx)
	{
		case PTA_BASE:PORTx = PORTA;break;
        case PTB_BASE:PORTx = PORTB;break;
        case PTC_BASE:PORTx = PORTC;break;
        case PTD_BASE:PORTx = PORTD;break;
        case PTE_BASE:PORTx = PORTE;break;
        default : break;
	} 
	//���ر�־λ
	if(PORTx->ISFR & (1<<GPIO_Pin))
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}


//================================================================================================
// GPIO����жϱ�־
// GPIOx:x�˿�
//================================================================================================
void GPIO_ClearITPendingBit(GPIO_Type *GPIOx,uint16_t GPIO_Pin)
{
	PORT_Type *PORTx = NULL;
	//���˿�ʱ��
	switch((uint32_t)GPIOx)
	{
		case PTA_BASE:PORTx=PORTA;SIM->SCGC5|=SIM_SCGC5_PORTA_MASK;break;   //����PORTA��ʹ��ʱ�ӣ�������ǰ���ȿ���ʹ��ʱ�Ӳμ�k10�ֲ�268ҳ��
        case PTB_BASE:PORTx=PORTB;SIM->SCGC5|=SIM_SCGC5_PORTB_MASK;break;	//����PORTB��ʹ��ʱ��
        case PTC_BASE:PORTx=PORTC;SIM->SCGC5|=SIM_SCGC5_PORTC_MASK;break;	//����PORTC��ʹ��ʱ��
        case PTD_BASE:PORTx=PORTD;SIM->SCGC5|=SIM_SCGC5_PORTD_MASK;break;	//����PORTD��ʹ��ʱ��
        case PTE_BASE:PORTx=PORTE;SIM->SCGC5|=SIM_SCGC5_PORTE_MASK;break;	//����PORTE��ʹ��ʱ��
        default : break;
	} 
	PORTx->ISFR |= (1<<GPIO_Pin);
}


//================================================================================================
// ��ʼ���ṹ�� ����Ĭ�ϲ���
//GPIO_InitStruct: ��ʼ���ṹ
//================================================================================================
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)
{
	GPIO_InitStruct->GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IN_FLOATING;
}


//==================================================================================================
// ��һ��IO Pin����Ϊ0 �� 1
//GPIOx:x�˿�
//==================================================================================================
void GPIO_WriteBit(GPIO_Type *GPIOx,uint16_t GPIO_Pin,BitAction BitVal)
{	
  if (BitVal != Bit_RESET)
  {
    GPIOx->PSOR |= (1<<GPIO_Pin);
  }
  else
  {
    GPIOx->PCOR |= (1<<GPIO_Pin);
  }
}

//==================================================================================================
// ��һ��IO Pin����Ϊ1
//GPIOx:x�˿�
//==================================================================================================
void GPIO_SetBits(GPIO_Type* GPIOx, uint16_t GPIO_Pin)
{
	GPIOx->PSOR |= (1<<GPIO_Pin);
}


//==================================================================================================
// ��һ��IO Pin����Ϊ0
//GPIOx:x�˿�
//==================================================================================================
void GPIO_ResetBits(GPIO_Type* GPIOx, uint16_t GPIO_Pin)
{
	GPIOx->PCOR |= (1<<GPIO_Pin);
}


//==================================================================================================
// ��תһ��IO Pin�ĵ�ƽ
// GPIOx:x�˿�
//==================================================================================================
void GPIO_ToggleBit(GPIO_Type *GPIOx,uint16_t GPIO_Pin)
{
	GPIOx->PTOR |= (1<<GPIO_Pin);	//�����ŵĵ�ƽ���з�ת
}

//==================================================================================================
// д��˿�IO��ƽ ���
//GPIOx:x�˿�
//==================================================================================================
void GPIO_Write(GPIO_Type *GPIOx,uint32_t PortVal)
{
	GPIOx->PDOR = PortVal;
}

//==================================================================================================
// ��ȡһ���Ѿ�����Ϊ�����IO Pin�� ��ƽ
//GPIOx:X�˿�
//==================================================================================================
uint8_t GPIO_ReadOutputDataBit(GPIO_Type* GPIOx, uint16_t GPIO_Pin)
{
	uint8_t bitstatus = 0x00;
	
  if(((GPIOx->PDOR >> GPIO_Pin) & 1 ) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}


//==================================================================================================
// ��ȡһ���Ѿ�����Ϊ�����IO�� ��ƽ
//GPIOx:X�˿�
//==================================================================================================
uint32_t GPIO_ReadOutputData(GPIO_Type* GPIOx)
{
  return ((uint16_t)GPIOx->PDOR);
}


//==================================================================================================
// ��ȡһ��IO�˿ڵ�ĳһ��PIN�ĵ�ƽ
// GPIOx:X�˿�
//==================================================================================================
uint8_t GPIO_ReadInputDataBit(GPIO_Type* GPIOx, uint16_t GPIO_Pin)
{	 
	uint8_t bitstatus = 0x00;
	
  if (((GPIOx->PDIR >> GPIO_Pin)& 0x01) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}


//==================================================================================================
// ��ȡһ��IO�˿ڵ������ƽ
//GPIOx:X�˿�
//==================================================================================================
uint32_t GPIO_ReadInputData(GPIO_Type *GPIOx)
{
	 return(GPIOx->PDIR);
}
