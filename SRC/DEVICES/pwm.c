#include "pwm.h"

//======================================================================
// ��ڲ�����GPIO:PTA:A�˿� PTB:B�˿� PTC:C�˿� PTD:D�˿� PTE:E�˿�
//			 Pin���˿ڵ�N����	 	
// ���ز�������
// ʵ��PWM���ǰ��IO��ʼ��
//======================================================================
void PWMPortInit(GPIO_Type *GPIO, uint16_t Pin){
	GPIO_InitTypeDef GPIO_InitStruct1;
	GPIO_InitStruct1.GPIO_Pin = Pin;
	GPIO_InitStruct1.GPIO_InitState = Bit_RESET;
	GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_OPP;
	GPIO_InitStruct1.GPIOx = GPIO;
	GPIO_Init(&GPIO_InitStruct1);
}

//======================================================================
// ��ڲ�����Pin ��PWM������� 
//           Div ��ʱ��Դ��Ƶ��
//           modValue��ģֵ�����ʼ����modValue��أ�Duty/modValue=ʵ��ռ�ձ�
//          �����С������PWM�����Ƶ��f=f(bus)/Div/modValue�����Ϊ65535
// ���ز�������
// ʵ�ֳ�ʼ��PWM
//========================================================================
void PWMInit(uint8_t Pin, uint8_t Div, uint16_t modValue)
{    
	TPM_Type *pstTPMModule;  
	uint8_t  TPMCh;
	//ѡ��TPMʱ��Դ
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);//MCGFLL �� MCGPLL/2
    // ����PWM�˿�   
    switch (Pin)
    {
        case PTB18:
            //PWMPortInit(PTB,18);
            SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
            PORTB->PCR[18] = PORT_PCR_MUX(0x3) | PORT_PCR_DSE_MASK;
            pstTPMModule = TPM2; 
            TPMCh = CH0;
            break;
            
        case PTB19:
            //PWMPortInit(PTB,19);
            SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
            PORTB->PCR[19] = PORT_PCR_MUX(0x3) | PORT_PCR_DSE_MASK; 
            pstTPMModule = TPM2;
            TPMCh = CH1;
            break;
        
        case PTE21:
            //PWMPortInit(PTE,21);
            SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
            PORTE->PCR[21] = PORT_PCR_MUX(0x3) | PORT_PCR_DSE_MASK; 
            pstTPMModule = TPM1;
            TPMCh = CH1;
            break;
     
        default:;
    }
    
// 2. ʱ��ʹ��:
    
    if (pstTPMModule == TPM0) {
        SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
    } else if (pstTPMModule == TPM1) {
        SIM->SCGC6 |=SIM_SCGC6_TPM1_MASK;
    } else if (pstTPMModule == TPM2) {
        SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
    }
    
// 3. ���� PWM:
    
    Div &= 0x07; //ȡ����λ����ֹ���������7�ķ�Ƶ��
	pstTPMModule->SC |= //TPM_SC_CPWMS_MASK |   // CPWMS = 1 ���Ķ���PWMģʽ    
                        (TPM_SC_CMOD(1) |       // CLKS=1:System clock(it's Bus clock here
                        TPM_SC_PS(Div));        // f[ftm]=f[Bus]/(2^Div)
		
	pstTPMModule->MOD = modValue;               // ��������PWMƵ�ʣ���ֵԽ��ռ�ձȿɵ�����Խ�ߣ�
                                                //����PWMƵ�ʵĸߵ͸����Ч�ʣ�����������ܣ��ĵ�������أ�����ϸ����

	TPM_CnSC_REG(pstTPMModule,TPMCh) = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; 
                                                //������ߺ������      
}

//======================================================================
// ��ڲ�����Pin ��PWM������� 
//           Duty��ռ�ձȣ����ʼ����modValue��أ�Duty/modValue=ʵ��ռ�ձ�                                   
// ���ز�������
// ʵ�ָ���PWMռ�ձ�
//========================================================================
void PWMOutput(uint8_t Pin, uint16_t Duty)
{
    TPM_Type *pstTPMModule;
    uint8_t  TPMCh;
       
    switch (Pin)
    {  
        case PTB18:
            pstTPMModule = TPM2; 
            TPMCh = CH0;
            break;
            
        case PTB19:
            pstTPMModule = TPM2;
            TPMCh = CH1;
            break;
        
        case PTE21:
            pstTPMModule = TPM1;
            TPMCh = CH1;
            break;

        default:break;
    }
     
   TPM_CnV_REG(pstTPMModule,TPMCh) = Duty;
}

/* ��ʼ����ʼ�ٶ� */
static uint16_t StartSpeed_Init(void)
{
    uint16_t speed = 0;
    
    GPIO_InitTypeDef GPIO_InitStruct1;
    
    GPIO_InitStruct1.GPIO_Pin = 0;
    GPIO_InitStruct1.GPIO_InitState = Bit_RESET;
    GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
    GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct1.GPIOx = PTE;
    GPIO_Init(&GPIO_InitStruct1);
    
    GPIO_InitStruct1.GPIO_Pin = 1;
    GPIO_Init(&GPIO_InitStruct1);
    
    GPIO_InitStruct1.GPIO_Pin = 2;
    GPIO_Init(&GPIO_InitStruct1);
    
    GPIO_InitStruct1.GPIO_Pin = 3;
    GPIO_Init(&GPIO_InitStruct1);
    
    if ((uint8_t)1 == GPIO_ReadInputDataBit(PTE, 0)) {
        speed = MAX_SPEED;
    } else if ((uint8_t)1 == GPIO_ReadInputDataBit(PTE, 1)) {
        speed = MID_SPEED;
    } else if ((uint8_t)1 == GPIO_ReadInputDataBit(PTE, 2)) {
        speed = MIN_SPEED;
    } else if ((uint8_t)1 == GPIO_ReadInputDataBit(PTE, 3)) {
        speed = DEF_SPEED;
    } else {
        speed = 0;
    }
    
    return speed;
}


/**********************************************************************/
/* �����ʼ�� */
void Steer_Init(void)
{
    PWMInit(PTE21, 5, 15625); // ���Ƶ�ʴ��100HZ
    PWMOutput(PTE21, DIR_MID);
}

/* ������ */
void Steer_Out(uint16_t Duty)
{   
    if (Duty < DIR_LEFT)  Duty = DIR_LEFT;
    if (Duty > DIR_RIGHT) Duty = DIR_RIGHT;
    
    PWMOutput(PTE21, Duty);
}

/* �����ʼ�� */
void Motor_Init(void)
{
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
    PORTC->PCR[2] &= ~(PORT_PCR_MUX_MASK);
    PORTC->PCR[2] |= PORT_PCR_MUX(1);
    PTC->PDDR |= (1 << 2);
    PTC->PDOR |= (1 << 2);
    
    PWMInit(PTB19, 5, 100); // ���Ƶ�ʴ��20KHZ
    PWMOutput(PTB19, StartSpeed_Init());
}

/* �����ת��С��ǰ�� */
void Motor_Forward(uint16_t Duty) 
{
    PWMOutput(PTB19, Duty);
}

/* �����ת��С������ */
void Motor_Backward(uint16_t Duty) 
{
    PWMOutput(PTB18, Duty);
}
