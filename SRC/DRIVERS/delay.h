
#ifndef __DELAY_H__
#define __DELAY_H__


#include "sys.h"

void DelayInit(void); //��ʱ��ʱ����ʼ��
void DelayUs(uint32_t us);      //��ʱus
void DelayMs(uint32_t ms);      //��ʱms
void SamplingDelay(void);
		 
#endif
