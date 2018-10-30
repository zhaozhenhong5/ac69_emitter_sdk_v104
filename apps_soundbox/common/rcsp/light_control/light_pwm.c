#include "rcsp/light_control/light_pwm.h"
#include "cpu/clock_api.h"

#if SUPPORT_APP_RCSP_EN

/*
*************************************************************
*Description: PWMռ�ձ��������
              1��ѡ��OUT_CH0����OUT_CH1��ʱ������IO�����ó���
              ���������������������
              2��ѡ��IO�������ʱ��ֻ��IO�������PWM

*Arguments  : out_ch:OUT_CHѡ��  0-ѡ��OUT_CH0  1-ѡ��OUT_CH1
                                 2-ѡ��IO
              prescale:Ԥ��Ƶϵ��(0:Ԥ��Ƶ1    1:Ԥ��Ƶ4
                                  2:Ԥ��Ƶ16   3:Ԥ��Ƶ64)
              duty:ռ�ձ�(0-USE_DENOMINATOR)/USE_DENOMINATOR
              fre:Ƶ��
*Returns    : 0:�ɹ�     ����:����
*Note       :
*By         : minzong
*************************************************************
*/
AC51_PWM_ERR PWM_output_cfg(OUT_CH_SEL out_ch,u8 prescale,u8 duty,u32 fre)
{
    u32 sys_clk = OSC_Hz;//clock_get_lsb_freq();
    u8  div = 0;

//    P_FUNCTION
    /* printf("par: out_ch=  %d, prescale= %d, duty= %d, fre= %d, sys_clk= %d\n",out_ch,prescale,duty,fre,sys_clk); */
//    printf("hsb= %d, 0x%x\n",clock_get_sys_freq(),SYS_DIV);

    if(prescale == 0)
        div = 1;
    else if(prescale == 1)
        div = 4;
    else if(prescale == 2)
        div = 16;
    else if(prescale == 3)
        div = 64;
    else
        return PWM_ERR_PRESCALE;

    if(duty > USE_DENOMINATOR)
    {
        return PWM_ERR_DUTY;
    }

//    puts("--do--\n");
    if(out_ch == OUT_CH0_SEL)//blue
    {
        //PWM1,blue,PB9
//        puts("-blue-\n");
        JL_IOMAP->CON1 &= ~(0x0700);
        JL_IOMAP->CON1 |= 0x0300;//pwm1

        JL_TIMER1->CON = 0x0000;
        JL_TIMER1->PRD = sys_clk/(div*fre);
        JL_TIMER1->PWM = (JL_TIMER1->PRD*duty)/USE_DENOMINATOR;
        JL_TIMER1->CON = (prescale<<4) | (1<<8)| (1<<0) |(2<<2); //
    }
    else if(out_ch == OUT_CH1_SEL)//
    {
        //PWM3,green-PB8,white-PB7
//        puts("-green&white-\n");
        JL_IOMAP->CON1 &= ~(0x3800);
        JL_IOMAP->CON1 |= 0x1800;//pwm3

        JL_TIMER3->CON = 0x0000;
        JL_TIMER3->PRD = sys_clk/(div*fre);
        JL_TIMER3->PWM = (JL_TIMER3->PRD*duty)/USE_DENOMINATOR;
        JL_TIMER3->CON =  (prescale<<4) | (1<<8)| (1<<0) | (2<<2); //
    }
    else if(out_ch == OUT_ASSIGN_PORT)//
    {
        //PWM2,red,PB3
//        puts("-red-\n");
        JL_TIMER2->CON = 0x0000;
        JL_TIMER2->PRD = sys_clk/(div*fre);
        JL_TIMER2->PWM = (JL_TIMER2->PRD*duty)/USE_DENOMINATOR;
        JL_TIMER2->CON =  (prescale<<4) | (1<<8)| (1<<0) | (2<<2); //

/*
		JL_TIMER2->CON = 0x0000;
        JL_TIMER2->PRD = sys_clk/(div*fre);
        JL_TIMER2->PWM = (JL_TIMER2->PRD*duty)/USE_DENOMINATOR;
        JL_TIMER2->CON =  (prescale<<4) | (1<<8)| (1<<0) | (2<<2); //
*/   

	}
    else
    {
        return PWM_ERR_SELEC_CH;
    }
    return PWM_ERR_NONE;
}


#endif

