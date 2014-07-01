#include "pid.h"

/* global variable */
PID PIDSteer;
PIDx sPIDx;

/* PID���������ʼ�� */
void PID_Init(PID *pid, const float desired, const float kp,
		const float ki, const float kd)
{
	pid->error = 0;
	pid->prevError = 0;
	pid->integ = 0;
	pid->deriv = 0;
	pid->desired = desired;
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;

	pid->iLimit = DEFAULT_LIMIT;
}

/* ����PID������� */
float Update_PID(PID *pid, const float measured)
{
	float output;

	pid->error = pid->desired - measured;       
	pid->integ += pid->error;                   
    pid->deriv = (pid->error - pid->prevError); 
    
	if(pid->integ > pid->iLimit) {              
		pid->integ = pid->iLimit;
	} else if (pid->integ < - pid->iLimit) {
		pid->integ = -pid->iLimit;
	}
	
	output = pid->kp * pid->error + pid->ki * pid->integ + pid->kd * pid->deriv; 
	pid->prevError = pid->error;

	return output;
}

/* ������PID���� */
int32_t Steer_Update(int8_t measured)
{
	return Update_PID(&PIDSteer, measured);
}

/* PIDx������ʼ�� */
void PIDx_Init(void)
{
    sPIDx.Un_1 = 0;
    sPIDx.En_1 = 0;
    sPIDx.En_2 = 0;
    sPIDx.Kp   = 4;
    sPIDx.Ki   = 0;
    sPIDx.Kd   = 1;
}

/* ���������Ƶ���� */
float PIDx_Value(PIDx *pp, int setpoint, int detectpoint)
{
    int En;     // ����ƫ��
    float Un;   // ���
    
    En = setpoint - detectpoint;    // ƫ��
    Un = pp->Un_1 + pp->Kp * (En - pp->En_1)
                  + pp->Ki * En
                  + pp->Kd * (En - 2*pp->En_1 + pp->En_2);
    pp->En_2 = pp->En_1;
    pp->En_1 = En;
    pp->Un_1 = Un;
    
    return Un; 
}
