#include "adc.h"

static uint8_t ADC_Cal(ADC_Type *ADCx);

void ADC_Init(ADC_InitTypeDef* ADC_InitStruct)
{
	uint8_t i;
	
	ADC_Type *ADCx = ADC0;
	PeripheralMapTypeDef *pADC_Map = (PeripheralMapTypeDef*)&(ADC_InitStruct->ADCxMap);
	
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;    //��ʱ����
	ADCx->CFG1 &= ~(ADC_CFG1_MODE_MASK);  //����AD����
	ADCx->CFG1 |= ADC_CFG1_MODE(ADC_InitStruct->ADC_Precision);
	ADCx->CFG1 &= ~ADC_CFG1_ADICLK_MASK;  //����ʱ��Ϊadʱ��Դ
	ADCx->CFG1 |=  ADC_CFG1_ADICLK(0); 
	ADCx->CFG1 &= ~ADC_CFG1_ADLSMP_MASK;  //����ʱ��
	ADCx->CFG1 &= ~ADC_CFG1_ADIV_MASK;    //����ʱ�ӷ�Ƶ
	ADCx->CFG1 |= ADC_CFG1_ADIV(3); 

	if(pADC_Map->m_PinCntIndex > 1)       // ��ͨ�������ͨ��
	{
		ADCx->SC1[pADC_Map->m_SpecDefine1] |= ADC_SC1_DIFF_MASK; 
	}
	else
	{
		ADCx->SC1[pADC_Map->m_SpecDefine1] &= ~ADC_SC1_DIFF_MASK; 
	}
	//Calieration
	i = ADC_Cal(ADCx);
	//����Դ
	(ADC_TRIGGER_HW == ADC_InitStruct->ADC_TriggerSelect)?(ADCx->SC2 |= ADC_SC2_ADTRG_MASK):(ADCx->SC2 &= ~ADC_SC2_ADTRG_MASK);
	
	for(i=0;i<pADC_Map->m_PinCntIndex;i++)
	{
		PinMuxConfig(pADC_Map->m_PortIndex,pADC_Map->m_PinBaseIndex+i,pADC_Map->m_MuxIndex);
	}
}

void ADC_ITConfig(ADC_Type* ADCx,uint8_t ADC_Mux, uint16_t ADC_IT, FunctionalState NewState)
{
	switch(ADC_IT)
	{
		case ADC_IT_AI:
			(ENABLE == NewState)?(ADCx->SC1[ADC_Mux] |= ADC_SC1_AIEN_MASK):(ADCx->SC1[ADC_Mux] &= ~ADC_SC1_AIEN_MASK);
			break;
		default:break;
	}
}

void ADC_DMACmd(ADC_Type* ADCx, uint16_t ADC_DMAReq, FunctionalState NewState)
{
	switch(ADC_DMAReq)
	{
		case ADC_DMAReq_COCO:
			(NewState == ENABLE)?(ADCx->SC2 |= ADC_SC2_DMAEN_MASK):(ADCx->SC2 &= ~ADC_SC2_DMAEN_MASK);
			break;
		default:break;
	}
}

uint32_t ADC_GetConversionValue(uint32_t ADCxMap)
{
	ADC_Type *ADCx = ADC0;
	PeripheralMapTypeDef *pADC_Map = (PeripheralMapTypeDef*)&(ADCxMap);
	

	ADCx->SC1[pADC_Map->m_SpecDefine1] &= ~(ADC_SC1_ADCH_MASK);	
	ADCx->SC1[pADC_Map->m_SpecDefine1] |= pADC_Map->m_ChlIndex;

	while((ADCx->SC1[pADC_Map->m_SpecDefine1] & ADC_SC1_COCO_MASK) == 0);  //�ȴ�ת�����
	//���ؽ��	
	return ADCx->R[pADC_Map->m_SpecDefine1];
}


ITStatus ADC_GetITStatus(ADC_Type* ADCx, uint8_t ADC_Mux, uint16_t ADC_IT)
{
	ITStatus retval;
	switch(ADC_IT)
	{
		case ADC_IT_AI:
			(ADCx->SC1[ADC_Mux] & ADC_SC1_COCO_MASK)?(retval = SET):(retval = RESET);
			break;
		default:break;
	}
	return retval;
}


//======================================================================
//����жϱ�־
//��ڣ��˿ڣ�ADCx ͨ����ADCx:ADC_Mux 
//���أ���
//
//======================================================================
void ADC_ClearITPendingBit(ADC_Type* ADCx, uint8_t ADC_Mux, uint16_t ADC_IT)
{
	uint8_t i = 0;
	i = i;
	switch(ADC_IT)
	{
		case ADC_IT_AI:
		  i = ADCx->R[ADC_Mux];
			break;
		default:break;
	}
}


//ADC У׼
static uint8_t ADC_Cal(ADC_Type *ADCx)
{
    uint16_t cal_var;
    ADCx->SC2 &= ~ADC_SC2_ADTRG_MASK; //�������
	ADCx->SC3 &= ~ADC_SC3_ADCO_MASK;  
	//32����ƽ��
	ADCx->SC3 &= ~ADC_SC3_AVGS_MASK;
	ADCx->SC3 |=  ADC_SC3_AVGS(3);
	//ʹ��Ӳ��ƽ��
	ADCx->SC3 |= ADC_SC3_AVGE_MASK;
	//��ʼУ׼
    ADCx->SC3 |= ADC_SC3_CAL_MASK ; 
	while((ADCx->SC1[A] & ADC_SC1_COCO_MASK) == 0);
	if((ADCx->SC3 & ADC_SC3_CALF_MASK) == ADC_SC3_CALF_MASK)
	{
		return 1;
	}
   // ����У׼
   cal_var = 0x00;
   cal_var =  ADCx->CLP0; 
   cal_var += ADCx->CLP1; 
   cal_var += ADCx->CLP2; 
   cal_var += ADCx->CLP3; 
   cal_var += ADCx->CLP4; 
   cal_var += ADCx->CLPS; 
   cal_var = cal_var/2;
   cal_var |= 0x8000; //����MSB
   ADCx->PG =  ADC_PG_PG(cal_var);
   // ����У׼
   cal_var = 0x00;
   cal_var =  ADCx->CLM0; 
   cal_var += ADCx->CLM1; 
   cal_var += ADCx->CLM2; 
   cal_var += ADCx->CLM3; 
   cal_var += ADCx->CLM4; 
   cal_var += ADCx->CLMS; 
   cal_var = cal_var/2;
   cal_var |= 0x8000; //����msb
	ADCx->MG = ADC_MG_MG(cal_var); 
   ADCx->SC3 &= ~ADC_SC3_CAL_MASK;
   return 0;
}
