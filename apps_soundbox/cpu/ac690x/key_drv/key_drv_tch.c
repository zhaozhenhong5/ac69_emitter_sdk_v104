/*******************************************************************************************
File Name: 	key_drv_tch.c

Version: 	1.00

Discription AC5100 Toutch Key API

Author:     bingquan Cai

Email:      bingquan_cai@zh-jieli.com

Date:		2013.05.09

Copyright (c) 2010 - 2012 JieLi. All Rights Reserved.
*******************************************************************************************/
#include "key_drv/key_drv_tch.h"
#include "includes.h"
#include "key_drv/key.h"
#include "cpu/timer_api.h"
#include "timer.h"
#include "irq_api.h"
#include "clock_api.h"


#if KEY_TCH_EN

///******ʹ��˵��*******///
//1.ͨ��ѡ��
//�޸� key_drv_tch.h�ļ���ĺ꼴�ɣ���ѡ������ͨ�� PA8��PA7������ʾ,ͨ��������������
//#define PITCH_ON_TCH_IO  (TCH_PA8|TCH_PA7)
//
//2.����ֵ����
//PA7 �� PA8 �ֱ��� 14ͨ����15ͨ�������Ǳ�ʾ�ŵ�0��key,��1��key
//�� tu8 get_tchkey_value(void)�������޸ĳ���������
//        case PA7_CHANNEL:
//            return 0;
//        case PA8_CHANNEL:
//            return 1;
//
//        case PB0_CHANNEL:
//        case PB1_CHANNEL:
//        case PB2_CHANNEL:
//        case PB3_CHANNEL:
//        case PB4_CHANNEL:
//        case PB10_CHANNEL:
//        case PB11_CHANNEL:
//        case PB12_CHANNEL:
//        case PA1_CHANNEL:
//        case PA2_CHANNEL:
//        case PA3_CHANNEL:
//        case PA4_CHANNEL:
//        case PA5_CHANNEL:
//        case PA6_CHANNEL:
//            return NO_KEY;
//        default:
//            return NO_KEY;


///CTMģ��
///*////////////////////////////////////////////////////////
///*////////////////////////////////////////////////////////

sCTM_KEY_VAR ctm_key_value;
sCTM_KEY_VAR *ctm_key_var;


static void ctm_key_var_init(sCTM_KEY_VAR *ptr)
{
    ctm_key_var = ptr;
}


tu8 get_tchkey_value(void)
{
    u8 key;
    u8 i;

    for (i = 0; i < TOUCH_KEY_CH_MAX; i++)
    {
        if (!(ctm_key_value.touch_key_state & (BIT(i))))
            break;
    }
    key = (i < TOUCH_KEY_CH_MAX) ? i : NO_KEY;

    //���Ϸ��ص���ͨ���ţ�����Ҫ��ͨ����ת���� key��(�ڼ�������)
    switch(key)
    {
        case PA7_CHANNEL:
            //puts("key 0\n");
            return 0;
        case PA8_CHANNEL:
            //puts("key 1\n");
            return 1;
        case PB4_CHANNEL:
             //puts("key 2\n");
            return 2;

        case PB0_CHANNEL:
        case PB1_CHANNEL:
        case PB2_CHANNEL:
        case PB3_CHANNEL:
        case PB10_CHANNEL:
        case PB11_CHANNEL:
        case PB12_CHANNEL:
        case PA1_CHANNEL:
        case PA2_CHANNEL:
        case PA3_CHANNEL:
        case PA4_CHANNEL:
        case PA5_CHANNEL:
        case PA6_CHANNEL:
            return NO_KEY;
        default:
            return NO_KEY;
    }
}


static void ctm_irq(u8 channel)
{
    u16 temp_u16_0, temp_u16_1,ctm_res_value;
    s16 temp_s16_0, temp_s16_1;
    s32 temp_s32_0;
//..............................................................................................
//ȡ����ֵ/ͨ���ж�
//..............................................................................................

    ctm_res_value = CTM_RES;

//    if(channel==15)
//    {
//       put_u16hex(ctm_res_value);
//    }
    //put_u8hex(channel);

    if(ctm_key_var->touch_init_cnt[channel])
    {
        ctm_key_var->touch_init_cnt[channel]--;
//		touch_cnt_buf[channel] = rvalue << FLT0CFG;
//		touch_release_buf[channel] = (long)(rvalue) << FLT1CFG0;
        ctm_key_var->touch_cnt_buf[channel] = (u32)ctm_res_value<<ctm_key_var->FLT0CFG;
        ctm_key_var->touch_release_buf[channel] = (u32)ctm_res_value<<ctm_key_var->FLT1CFG0;
    }

//..............................................................................................
//��ǰ����ֵȥ�����˲���
//..............................................................................................
    temp_u16_0 = ctm_key_var->touch_cnt_buf[channel];
    temp_u16_1 = temp_u16_0;
    temp_u16_1 -= (temp_u16_1 >> ctm_key_var->FLT0CFG);
    temp_u16_1 += ctm_res_value;//temp_u16_1 += rvalue;
    ctm_key_var->touch_cnt_buf[channel] = temp_u16_1;
    temp_u16_0 += temp_u16_1;
    temp_u16_0 >>= (ctm_key_var->FLT0CFG + 1);


//..............................................................................................
//��ͨ�������ͷż���ֵ�˲���
//..............................................................................................
    temp_s32_0 = ctm_key_var->touch_release_buf[channel];
    temp_u16_1 = temp_s32_0 >> ctm_key_var->FLT1CFG0;	//����˲���֮��İ����ͷ�ֵ
    temp_s16_0 = temp_u16_0 - temp_u16_1;	//��úͱ��μ��ֵ�Ĳ�ֵ�����°���Ϊ��ֵ���ͷŰ���Ϊ��ֵ
    temp_s16_1 = temp_s16_0;

//    	if(channel == 0)
//    	{
//    		printf("ch%d: %d  %d", (short)ch, temp_u16_0, temp_s16_1);
//    	}

    if(ctm_key_var->touch_key_state & BIT(channel))		//�����ͨ������Ŀǰ�Ǵ����ͷ�״̬
    {
        if(temp_s16_1 >= 0)		//��ǰ����ֵ���ڵ�ֵͨ���Ŵ���������
        {
            if(temp_s16_1 < (ctm_key_var->FLT1CFG2 >> 3))
            {
                temp_s16_1 <<= 3;	//�Ŵ���������
            }
            else
            {
                temp_s16_1 = ctm_key_var->FLT1CFG2;	//���ͣ���ֹĳЩ�ϴ����ƫ��´���
            }
        }
        else if(temp_s16_1 >= ctm_key_var->FLT1CFG1)	//��ǰ����ֵС�ڵ�ֵͨ���࣬������������
        {
        }
        else				//��ǰ����ֵС�ڵ�ֵͨ�ܶ࣬��С��������� (�з����������Զ���չ����λ???)
        {
            temp_s16_1 >>= 3;
        }
    }
    else			//�����ͨ������Ŀǰ�Ǵ��ڰ���״̬, ���������ͷż���ֵ
    {
        if(temp_s16_1 <= ctm_key_var->RELEASECFG1)
        {
            temp_s16_1 >>= 3;		//��С���������
        }
        else
        {
            temp_s16_1 = 0;
        }
    }

    temp_s32_0 += (s32)temp_s16_1;
    ctm_key_var->touch_release_buf[channel] = temp_s32_0;

//..............................................................................................
//�����������ͷż��
//..............................................................................................
    if(temp_s16_0 <= ctm_key_var->PRESSCFG)				//��������
    {
//        printf("P=%d\n",(0 - temp_s16_0));
        ctm_key_var->touch_key_state &= ~BIT(channel);
    }
    else if(temp_s16_0 >= ctm_key_var->RELEASECFG0)		//�����ͷ�
    {
        ctm_key_var->touch_key_state |= BIT(channel);
    }
}

//�����ж�
void ctm_irq_isr(void)
{
    u8 channel;
    CTM_CPND(1);
    channel = CTM_CH();
    ctm_irq(channel);
}

IRQ_REGISTER(IRQ_CTM_IDX,ctm_irq_isr);



/** \brief
 *
 * \param  ch ѡ�е�channel
 * \return ��
 *
 */
void tch_key_init(void)
{
	u16 ch = PITCH_ON_TCH_IO;

    ctm_key_var_init(&ctm_key_value);

    my_memset((u8 *)&ctm_key_value, 0x0, sizeof(sCTM_KEY_VAR));
    /*����������������*/
    ctm_key_value.FLT0CFG = 1;
    ctm_key_value.FLT1CFG0 = 7;
    ctm_key_value.FLT1CFG1 = -80;
    ctm_key_value.FLT1CFG2 = (-(-10)) << 7; //1280

    ///���������ȵ���Ҫ����
    ctm_key_value.PRESSCFG = -50;    //����������
    ctm_key_value.RELEASECFG0 = -10; //�ͷ�������0
    ctm_key_value.RELEASECFG1 = -100;//�ͷ�������1

    my_memset((u8 *)&(ctm_key_value.touch_init_cnt[0]), 0x10, TOUCH_KEY_CH_MAX);//����16��

    ctm_key_value.touch_key_state = 0xffff; //<����Ĭ���ͷ�

    //PB������ th0~th4  PB0~PB4 5��ͨ�� th5~th7 PB10~PB12 3��ͨ��
    JL_PORTB->DIR |=   (ch&0x001f)|((ch&0x00e0)<<5);
    JL_PORTB->PU  &= ~((ch&0x001f)|((ch&0x00e0)<<5));
    JL_PORTB->PD  &= ~((ch&0x001f)|((ch&0x00e0)<<5));
    JL_PORTB->DIE &= ~((ch&0x001f)|((ch&0x00e0)<<5));

    //PA������  th8~th15 PA1~PA8
    JL_PORTA->DIR |=   (ch&0xff00)>>7;
    JL_PORTA->PU  &= ~((ch&0xff00)>>7);
    JL_PORTA->PD  &= ~((ch&0xff00)>>7);
    JL_PORTA->DIE &= ~((ch&0xff00)>>7);

    CTM_CH_EN(ch);//�򿪶�Ӧͨ��


    /*��������ѡ�񣬸��ݵ������ã�����Ƶ�ʱ�����1MHz*/
    CTM_ISEL(5);


    //һ����˵������ 2.5ms �� 1.25ms
    //��ÿ��ͨ���Ĳ������ʱ��ֱ�Ϊ����ʹ�õ�ͨ��ҲҪռ��ʱ�䣩
    //2.5*16=40ms  1��25��
    //1.25*16=20ms 1��50��
    //���������Ӧ�Ƽ�ʹ��2.5ms ����Ҫ������Ӧ���Ƽ�1.25ms

    //��PLL8M��ʱ��Դ 8M 8000000
    //բ��ʱ�� = GTSELѡ������ʱ������ * PSEL��Ƶ����N * ��CTM_PRD+1��
    //*բ��ʱ�� = 1/8000000 * (155 + 1) * 128*/


    CTM_GTSEL(3);//ѡ��PLL8Mʱ�� 0011

    //1.25ms
    //CTM_PRD(77);
    //CTM_PSEL(0x7);

    //2.5ms
    CTM_PRD(155);
    CTM_PSEL(0x7);

    CTM_EN(1);   //ģ��ʹ��
    CTM_CPND(1); //��pending
    CTM_IE(1);   //�ж�ʹ��

    printf("CTM_CON0 : %04x\n", CTM_CON0);
    printf("CTM_CON1 : %04x\n", CTM_CON1);

    //ע�ᴥ���ж�
    IRQ_REQUEST(IRQ_CTM_IDX,ctm_irq_isr,3);

}

///CTMģ��
///*////////////////////////////////////////////////////////
///*////////////////////////////////////////////////////////
#endif
