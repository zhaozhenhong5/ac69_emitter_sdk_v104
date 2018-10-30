/*
*********************************************************************************************************
*                                             BC51
*
*                                             CODE
*
*                          (c) Copyright 2015-2016, ZHUHAI JIELI
*                                           All Rights Reserved
*
* File : *
* By   : jamin.li
* DATE : 11/11/2015 build this file
*********************************************************************************************************
*/
#ifndef _SRC_H_
#define _SRC_H_

#include "rtos/os_api.h"
#include "hw_cpu.h"
#include "cpu.h"
#include "cbuf/circular_buf.h"
#include "common/printf.h"
#include "mem/malloc.h"

#define SRC_MAX_CHANNEL 2

///DAC_BUF_LEN
#define SRC_INPUT_BUF_LEN  (128)
#define SRC_OUTPUT_BUF_LEN (128)

#define SRC_CBUFF_SIZE         (SRC_INPUT_BUF_LEN*12)   ///1.5K,�������ݻ���ش�С
#define SRC_START_CHANGE_LEN   (SRC_INPUT_BUF_LEN*4)    ///512,����ת���Ļ�������ݳ���
#define SRC_FLTB_CACHE_SIZE    (SRC_MAX_CHANNEL*48)     ///Ӳ������,ÿ��ͨ��������Ҫ48bytes,4�ֽڶ���

#define SRC_PMALLOC_SIZE       (SRC_INPUT_BUF_LEN + SRC_OUTPUT_BUF_LEN + SRC_FLTB_CACHE_SIZE)///����ram����ռ�

typedef struct
{
    u8 nchannel;    ///һ��ת����ͨ��������(1 ~ SRC_MAX_CHANNEL)
    u8 reserver[3];
    u16 in_rate;    ///���������
    u16 out_rate;   ///���������
    u16 in_chinc;   ///���뷽��,��ͨ��ת��ʱ��ÿͨ�����ݵĵ�ַ����
    u16 in_spinc;   ///���뷽��,ͬһͨ����һ�������ǰһ���ݵĵ�ַ����
    u16 out_chinc;  ///�������,��ͨ��ת��ʱ��ÿͨ�����ݵĵ�ַ����
    u16 out_spinc;  ///�������,ͬһͨ����һ�������ǰһ���ݵĵ�ַ����
    void (*output_cbk)(u8* ,u16 ); ///ת������������ص����жϵ���
} src_param_t;

typedef struct
{
    u8 src_read_empty;   ///����CBUFF���ձ��
    u8 src_channel_cnt;  ///ת����ͨ������
    u8 reserver[2];
    void (*src_output_cbk)(u8* ,u16 );///ת������������ص����жϵ���
    cbuffer_t *src_cb ;   ///CBUFF����
    u8  *src_c_buf_addr;  ///����д��CBUFF
    u16 *src_input_buff;  ///ת����������buffer
    u16 *src_output_buff; ///ת���������buffer
    u32 *src_fltb_cache;  ///ÿ��ͨ��������Ҫ48bytes
}src_var_t;



struct src_driver {

    int (*src_init)(src_param_t *parm);
    void (*src_exit)();
    u32 (*src_write_data)(u8 *buf,u16 len,u8 full_check);
    void (*src_clear_data)(void);
	void (*src_config)(src_param_t *param);
};


// #define REGISTER_SRC_DRIVER(drv) \
	// const struct src_driver *__src_drv \
			// __attribute__((section(".text"))) = &drv
#define REGISTER_SRC_DRIVER(drv) \
	const struct src_driver *__src_drv \
			sec(.text) = &drv

extern const struct src_driver *__src_drv;
int src_init_api(src_param_t *parm);
u32 src_write_data_api(u8 *buf , u16 len,u8 full_check);
void src_clear_data_api(void);
void src_param_cfg(src_param_t *param);
void src_exit_api();
#endif
