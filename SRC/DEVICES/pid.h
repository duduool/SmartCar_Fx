#ifndef _PID_H_
#define _PID_H_

#include <stdint.h>

#define DEFAULT_LIMIT 20.0
#define PID_STEER_KP  2.0
#define PID_STEER_KI  0.0
#define PID_STEER_KD  300.0

typedef struct {
  float desired;      // ����Ҫ�ﵽ��ֵ
  float error;        // ���
  float prevError;    // ��һ�ε����
  float integ;        // ���ֺ�
  float deriv;        // ΢��
  float kp;           // ����ϵ��
  float ki;           // ����ϵ��
  float kd;           // ΢��ϵ��
  float iLimit;       // ������
} PID;

typedef struct {
    float Kp;         // ��������
    float Ki;         // ���ֳ���
    float Kd;         // ΢�ֳ���
    float Un_1;       // Ŀ��ֵ
    float En_1;       // ��һ�ε����
    float En_2;       // ���ϴ˵����
} PIDx;

/* APIs */
int32_t Steer_Update(int8_t error);
float   Update_PID(PID *pid, const float measured);
void    PID_Init(PID *pid, const float desired, const float kp, const float ki, const float kd);
void    PIDx_Init(void);
float   PIDx_Value(PIDx *pp, int setpoint, int detectpoint);

#endif /*_PID_H_*/
