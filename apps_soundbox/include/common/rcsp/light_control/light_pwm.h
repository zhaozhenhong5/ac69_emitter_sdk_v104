#ifndef _LIGHT_PWM_H_
#define _LIGHT_PWM_H_

#include "light_def.h"

//����ռ�ձȵ�ʱ����������ĸ��ֵ,����0xFF����ĸ��ʱ��,����ռ�ձ�ʱΪx/0xFF
#define USE_DENOMINATOR  0xFF//100

//ö�ٽṹ��
typedef enum _AC51_PWM_ERR_
{
    PWM_ERR_NONE,
    PWM_ERR_PRESCALE,
    PWM_ERR_SELEC_CH,
    PWM_ERR_DUTY,
}AC51_PWM_ERR;

typedef enum _OUT_CH_SEL_
{
    OUT_CH0_SEL = 0,
    OUT_CH1_SEL,
    OUT_ASSIGN_PORT,
}OUT_CH_SEL;

typedef enum _MOTOR_PWM_CH_SEL_
{
    MOTORPWM1_L = 0,
    MOTORPWM1_H,
    MOTORPWM2_L,
    MOTORPWM2_H,
    MOTORPWM3_L,
    MOTORPWM3_H,
    MOTORPWMMAX,
}MOTOR_PWM_CH_SEL;

//���⺯������
AC51_PWM_ERR PWM8_output(OUT_CH_SEL out_ch,u8 prescale,u8 duty);
AC51_PWM_ERR PWM2_output(OUT_CH_SEL out_ch,u8 prescale,u8 duty,u32 fre);
AC51_PWM_ERR PWM3_output(OUT_CH_SEL out_ch,u8 prescale,u8 duty,u32 fre);
AC51_PWM_ERR MOTORPWM_output(MOTOR_PWM_CH_SEL channel,u8 prescale,u16 duty,u32 fre);
AC51_PWM_ERR PWM_output_cfg(OUT_CH_SEL out_ch,u8 prescale,u8 duty,u32 fre);

#endif
