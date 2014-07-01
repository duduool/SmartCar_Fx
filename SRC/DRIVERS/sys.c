#include "sys.h"
CPUInfoType_t CPUInfo; // struct to record important CPU information

extern uint32_t SystemCoreClock;

void GetCPUInfo(void)
{
	//CPU ����
	switch((SIM->SDID & SIM_SDID_FAMID_MASK)>>SIM_SDID_FAMID_SHIFT) 
	{  
    	case 0x0: CPUInfo.FamilyType = 0; break; //KL0x Family(low end)
    	case 0x1: CPUInfo.FamilyType = 1; break; //KL1x Family(basic)
		case 0x2: CPUInfo.FamilyType = 2; break; //KL2x Family(USB)
        case 0x3: CPUInfo.FamilyType = 3; break; //KL3x Family(segment LCD)
		case 0x4: CPUInfo.FamilyType = 4; break; //KL4x Familt(USB and segment LCD)
		default: CPUInfo.FamilyType = 255;break;
	}
	//Pin Package
	switch((SIM->SDID & SIM_SDID_PINID_MASK)>>SIM_SDID_PINID_SHIFT) 
	{  
		
		case 0x00: CPUInfo.PinCnt = 16;  break;
		case 0x01: CPUInfo.PinCnt = 24;  break;
		case 0x02: CPUInfo.PinCnt = 32;  break;
		case 0x03: CPUInfo.PinCnt =  0;  break;	
		case 0x04: CPUInfo.PinCnt = 48;  break;	
		case 0x05: CPUInfo.PinCnt = 64;  break;	
		case 0x06: CPUInfo.PinCnt = 80;  break;	
		case 0x07: CPUInfo.PinCnt =  0;  break;	
		case 0x08: CPUInfo.PinCnt =100;  break;	
		case 0x09: CPUInfo.PinCnt =  0;  break;	
		case 0x0A: CPUInfo.PinCnt =  0;  break;	
		case 0x0B: CPUInfo.PinCnt =  0;  break;	
		case 0x0C: CPUInfo.PinCnt =  0;  break;	
		case 0x0D: CPUInfo.PinCnt =  0;  break;	
		case 0x0E: CPUInfo.PinCnt =  0;  break;	
		case 0x0F: CPUInfo.PinCnt =  0;  break;	
        default:  CPUInfo.PinCnt = 0;    break;	
	}  
   //SiliconRevID
    switch((SIM->SDID & SIM_SDID_REVID_MASK)>>SIM_SDID_REVID_SHIFT) 
    { 
			case 0x0: CPUInfo.SiliconRev = 10;break;
			case 0x1: CPUInfo.SiliconRev = 11;break;
			case 0x2: CPUInfo.SiliconRev = 12;break;
			default:  CPUInfo.SiliconRev = 0; break;
    }	
	//PFlash ��С
	switch((SIM->FCFG1 & SIM_FCFG1_PFSIZE_MASK)>>SIM_FCFG1_PFSIZE_SHIFT)
	{
		case 0x01: CPUInfo.PFlashSize = 8*1024;  break;
		case 0x02: CPUInfo.PFlashSize = 16*1024; break;
		case 0x03: CPUInfo.PFlashSize = 32*1024; break;
		case 0x04: CPUInfo.PFlashSize = 0*1024;  break;
		case 0x05: CPUInfo.PFlashSize = 64*1024; break;
		case 0x06: CPUInfo.PFlashSize = 0*1024;  break;
		case 0x07: CPUInfo.PFlashSize = 128*1024;break;
		case 0x08: CPUInfo.PFlashSize = 0*1024;  break;
		case 0x09: CPUInfo.PFlashSize = 256*1024;break;
		case 0x0A: CPUInfo.PFlashSize = 0*1024;  break;
		case 0x0B: CPUInfo.PFlashSize = 0*1024;  break;
		case 0x0C: CPUInfo.PFlashSize = 0*1024;  break;
		case 0x0D: CPUInfo.PFlashSize = 0*1024;  break;	
		case 0x0E: CPUInfo.PFlashSize = 0*1024;  break;	
		case 0x0F: CPUInfo.PFlashSize = 256*1024;break;	
	 	default:  CPUInfo.PFlashSize = 0*1024;   break; 		
	}
	//SRAM��С
	switch((SIM->SDID & SIM_SDID_SRAMSIZE_MASK)>>SIM_SDID_SRAMSIZE_SHIFT)
	{
		case 0x00: CPUInfo.RAMSize = 512;     break;
		case 0x01: CPUInfo.RAMSize = 1*1024;  break;
		case 0x02: CPUInfo.RAMSize = 2*1024;  break;
		case 0x03: CPUInfo.RAMSize = 4*1024;  break;
		case 0x04: CPUInfo.RAMSize = 8*1024;  break;
		case 0x05: CPUInfo.RAMSize = 16*1024; break;
		case 0x06: CPUInfo.RAMSize = 32*1024; break;
		case 0x07: CPUInfo.RAMSize = 64*1024; break;
		default:break;
	}
	//ʱ������
	CPUInfo.CoreClock = SystemCoreClock;
	CPUInfo.BusClock=(SystemCoreClock/(((SIM->CLKDIV1&SIM_CLKDIV1_OUTDIV1_MASK)>>SIM_CLKDIV1_OUTDIV1_SHIFT)+1));     
	CPUInfo.FlashClock = (SystemCoreClock/(((SIM->CLKDIV1&SIM_CLKDIV1_OUTDIV4_MASK)>>SIM_CLKDIV1_OUTDIV4_SHIFT)+1));   
	CPUInfo.FlexBusClock = 0;
	CPUInfo.BusClock = (SystemCoreClock/(((SIM->CLKDIV1&SIM_CLKDIV1_OUTDIV4_MASK)>>SIM_CLKDIV1_OUTDIV4_SHIFT)+1));  
	
}


void PinMuxConfig(uint8_t GPIOIndex, uint8_t PinIndex, uint8_t MuxIndex)
{
	switch(GPIOIndex)
	{
		case 0:
			SIM->SCGC5|=SIM_SCGC5_PORTA_MASK;
			PORTA->PCR[PinIndex]&= ~(PORT_PCR_MUX_MASK);    
			PORTA->PCR[PinIndex]|= PORT_PCR_MUX(MuxIndex); 
			break;
		case 1:
			SIM->SCGC5|=SIM_SCGC5_PORTB_MASK;
			PORTB->PCR[PinIndex]&= ~(PORT_PCR_MUX_MASK);    
			PORTB->PCR[PinIndex]|= PORT_PCR_MUX(MuxIndex); 
			break;
		case 2:
			SIM->SCGC5|=SIM_SCGC5_PORTC_MASK;
			PORTC->PCR[PinIndex]&= ~(PORT_PCR_MUX_MASK);    
			PORTC->PCR[PinIndex]|= PORT_PCR_MUX(MuxIndex); 
			break;
		case 3:
			SIM->SCGC5|=SIM_SCGC5_PORTD_MASK;
			PORTD->PCR[PinIndex]&= ~(PORT_PCR_MUX_MASK);    
			PORTD->PCR[PinIndex]|= PORT_PCR_MUX(MuxIndex); 
			break;
		case 4:
			SIM->SCGC5|=SIM_SCGC5_PORTE_MASK;
			PORTE->PCR[PinIndex]&= ~(PORT_PCR_MUX_MASK);    
			PORTE->PCR[PinIndex]|= PORT_PCR_MUX(MuxIndex); 
			break;
		default:
			break;
	}
}


//=================================================================================================
// ���� ����ϵͳ��Ƶ
// ��ڣ� ʱ��Դѡ��
//			  ClockSource_IRC    : �ڲ�ʱ��Դ
//			  ClockSource_EX50M  : �ⲿ50M����
//	  	 CoreClock�ں�Ƶ��
//		      CoreClock_48M  : CoreClock = 48M
//		      CoreClock_64M  : CoreClock = 64M
//			  CoreClock_72M  : CoreClock = 72M
//			  CoreClock_96M  : CoreClock = 96M
//			  CoreClock_100M : CoreClock = 100M
//            CoreClock_11M  : CoreClock = 110M
//            CoreClock_120M : CoreClock = 120M
//���������ã��������ԣ�����ʱ�Ӳ��ɳ���40M��40M��8���ڱع�����
//=================================================================================================
void SystemClockSetup(uint8_t ClockOption,uint16_t CoreClock)
{
	//====ʹ���ڲ�ʱ��Դ====//
	if(ClockOption == ClockSource_IRC)
	{
		SIM->CLKDIV1 = (uint32_t)0x00020000UL; 
		MCG->C1 |= (MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK);
		MCG->C2 = 0;
		MCG->C4 &= ~MCG_C4_DRST_DRS_MASK;
		switch(CoreClock)
		{
			case CoreClock_48M:
				MCG->C4 |= (MCG_C4_DMX32_MASK|MCG_C4_DRST_DRS(1));
				SIM->CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV4(1));
				SystemCoreClock = 48000000;
				break;				
			case CoreClock_24M:	
				MCG->C4 |= (MCG_C4_DMX32_MASK|MCG_C4_DRST_DRS(0));
				SIM->CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV4(0));
				SystemCoreClock = 24000000;
				break;		
		}
		OSC0->CR = (uint8_t)0x80U;
		MCG->C5 = (uint8_t)0x00U;
		MCG->C6 = (uint8_t)0x00U;
		while((MCG->S & MCG_S_IREFST_MASK) == 0x00U) {};/* ȷ��FLL�ο�ʱ�����ڲ�ʱ��. */
		while((MCG->S & 0x0CU) != 0x00U) {};    /* �ȴ�FLL��ѡ�� */
	}
	
	//====ʹ���ⲿ50Mʱ��Դ====//
	if(ClockOption == ClockSource_EX50M)
	{
		SIM->SCGC5 |= (uint32_t)0x0200UL; /*  ���� PORTA ʱ���� */   
 
		SIM->CLKDIV1 = (uint32_t)0x10010000UL; /*  �ں�/ϵͳ2��Ƶ��Flash������2��Ƶ */
		
		/* PORTA->PCR18: ISF=0,MUX=0 */
		PORTA->PCR[18] &= (uint32_t)~0x01000700UL;
		
		/* PORTA->PCR19: ISF=0,MUX=0 */
		PORTA->PCR[19] &= (uint32_t)~0x01000700UL;
		
		/* Switch to FBE Mode */
		/* OSC0->CR: ERCLKEN=1,??=0,EREFSTEN=0,??=0,SC2P=1,SC4P=0,SC8P=0,SC16P=1 */	
		OSC0->CR = (uint8_t)0x89U; //�ⲿ�ο�ʹ�ܣ�
		
		/* MCG->C2: LOCRE0=0,??=0,RANGE0=2,HGO0=0,EREFS0=1,LP=0,IRCS=0 */
		MCG->C2 = (uint8_t)0x34U; //ѡ��ʹ���ⲿ�ο�Դ��ʹ��8-32Mhz����
		
		/* MCG->C1: CLKS=2,FRDIV=3,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
		MCG->C1 = (uint8_t)0xBAU; //FLL�ⲿ�ο�,1536��Ƶ
		
		/* MCG->C4: DMX32=0,DRST_DRS=0 */
		MCG->C4 &= (uint8_t)~(uint8_t)0xE0U;
		
		/* MCG->C5: ??=0,PLLCLKEN0=0,PLLSTEN0=0,PRDIV0=1 */
		MCG->C5 = MCG_C5_PRDIV0(24);// PLL�ⲿ�ο�,25��Ƶ	���Ĵ����Լ�1������0Ϊ1��Ƶ��1Ϊ2��Ƶ��2Ϊ3��Ƶ��nΪn+1��Ƶ��
		
        switch(CoreClock)
		{
			case CoreClock_48M:
				MCG->C6 = MCG_C6_VDIV0(0); //2M*24 = 48M   �˼Ĵ����Լ�24������0��Ϊ24��Ƶ
				SIM->CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV4(1)); //Core=48M/1  Bus=48M/1/2   n+1����Ƶ
				SystemCoreClock = 48000000;
				break;				
			case CoreClock_24M:	
				MCG->C6 = MCG_C6_VDIV0(0); //2M*24 = 48M   �˼Ĵ����Լ�24������0��Ϊ24��Ƶ
				SIM->CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(1) | SIM_CLKDIV1_OUTDIV4(0)); //Core=48M/2  Bus=48M/2/1    n+1����Ƶ
				SystemCoreClock = 24000000;
				break;		
			case CoreClock_96M:	
				MCG->C6 = MCG_C6_VDIV0(24); //2M*48 = 96M   �˼Ĵ����Լ�24������0��Ϊ24��Ƶ
				SIM->CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV4(2)); //Core=96M/1  Bus=96M/1/3=32M    n+1����Ƶ
				SystemCoreClock = 96000000;
				break;	
			case CoreClock_80M:	
				MCG->C6 = MCG_C6_VDIV0(16); //2M*40 = 80M   �˼Ĵ����Լ�24������0��Ϊ24��Ƶ
				SIM->CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV4(2)); //Core=80M/1  Bus=80M/1/3=26.667m    n+1����Ƶ
				SystemCoreClock = 80000000;
				break;	
			case CoreClock_64M:	
				MCG->C6 = MCG_C6_VDIV0(8); //2M*32 = 64M   �˼Ĵ����Լ�24������0��Ϊ24��Ƶ
				SIM->CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV4(1)); //Core=64M/1  Bus=64M/1/2=32M    n+1����Ƶ
				SystemCoreClock = 64000000;
				break;
			case CoreClock_100M:	
				MCG->C6 = MCG_C6_VDIV0(26); //2M*50 = 100M   �˼Ĵ����Լ�24������0��Ϊ24��Ƶ
				SIM->CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV4(2)); //Core=100M/1  Bus=100M/1/3=33.33M    n+1����Ƶ
				SystemCoreClock = 100000000;
				break;
			case CoreClock_110M:	
				MCG->C6 = MCG_C6_VDIV0(31); //2M*55 = 110M   �˼Ĵ����Լ�24������0��Ϊ24��Ƶ
				SIM->CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV4(2)); //Core=110M/1  Bus=110M/1/3=36.67M    n+1����Ƶ
				SystemCoreClock = 110000000;
				break;
			case CoreClock_120M:	
				MCG->C5 = MCG_C5_PRDIV0(19); //20��Ƶ2.5M
				MCG->C6 = MCG_C6_VDIV0(24); //2.5M*48 = 120M   �˼Ĵ����Լ�24������0��Ϊ24��Ƶ
				SIM->CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV4(3)); //Core=120M/1  Bus=120M/1/4=30M    n+1����Ƶ
				SystemCoreClock = 120000000;
				break;
		}

				
		while((MCG->S & MCG_S_IREFST_MASK) != 0x00U) {}; /* Check that the source of the FLL reference clock is the external reference clock. */
		while((MCG->S & 0x0CU) != 0x08U) {};    /* Wait until external reference clock is selected as MCG output */
		/* Switch to PBE Mode */
		/* MCG->C6: LOLIE0=0,PLLS=1,CME0=0,VDIV0=0 */
		MCG->C6 |= (uint8_t)0x40U;
		while((MCG->S & 0x0CU) != 0x08U) {};    /* Wait until external reference clock is selected as MCG output */
		while((MCG->S & MCG_S_LOCK0_MASK) == 0x00U) {}; /* Wait until locked */
		/* Switch to PEE Mode */
		/* MCG->C1: CLKS=0,FRDIV=3,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
		MCG->C1 = (uint8_t)0x1AU;
		while((MCG->S & 0x0CU) != 0x0CU) {};    /* Wait until output of the PLL is selected */				
	}

	GetCPUInfo();//��ȡCPU��Ϣ ���¼����������Ƶ��
}


////=================================================================================================
// ����һ���жϵ����ȼ�
// ��ڣ�IRQn �жϺ�
//			 PriorityGroup ���ȼ����� 
//		     NVIC_PriorityGroup_0
//			 NVIC_PriorityGroup_1
//		     NVIC_PriorityGroup_2
//			 NVIC_PriorityGroup_3
//		     NVIC_PriorityGroup_4
//		 	 PreemptPriority  ��ռ���ȼ�
//			 SubPriority      �����ȼ�(��Ӧ���ȼ�)
// ��������жϵ���ռ���ȼ�����Ӧ���ȼ���һ���Ļ� �������ж��ȷ�������ִ��
//�����ȼ�����ռ���ȼ��ǿ��Դ�ϵ����ȼ�����ռ���ȼ��жϵ�
//����ռ���ȼ���ͬ���ж� �����ȼ�����Ӧ���ȼ������Դ�ϵ����ȼ�����Ӧ���ȼ� 
////=================================================================================================
void NVIC_Init(IRQn_Type IRQn,uint32_t PriorityGroup,uint32_t PreemptPriority,uint32_t SubPriority)
{
    //NVIC_SetPriorityGrouping(PriorityGroup);
	//NVIC_SetPriority(IRQn,NVIC_EncodePriority(PriorityGroup,PreemptPriority,SubPriority));
}


//=================================================================================================
//��λ
//=================================================================================================
void SystemSoftReset(void)
{   
	//SCB->AIRCR =0X05FA0000|(uint32_t)0x04;	 
	NVIC_SystemReset();
} 


//=================================================================================================
// ʹ�����ж�
// ����  CMSIS Cortex-M Core Function Access �� ���� ��.core_cmFunc.h
//=================================================================================================
void EnableInterrupts(void)
{
	__enable_irq();
}


//=================================================================================================
// �ر����ж�
// ����  CMSIS Cortex-M Core Function Access �� ���� ��.core_cmFunc.h
//=================================================================================================
void DisableInterrupts(void)
{
	__disable_irq();
}


//=================================================================================================
// �����ж���������ʼλ��
//=================================================================================================
void SetVectorTable(uint32_t VectorOffset)
{
	SCB->VTOR = VectorOffset;  //�����ж�������ƫ�Ƶ�ַ
}

//=================================================================================================
//��ù̼���汾
//���أ��̼���汾
//=================================================================================================
uint16_t GetFWVersion(void)
{
	return(LZVERSION);
}
