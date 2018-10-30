
/*******************************************************************************************

Copyright (C), 2014 -2014, JIELI Tech. Co., Ltd.
File name:if_decoder_ctrl.h
Author:    Version:    Date: // ���ߡ��汾���������
������  1.0             2014-06-19   09:02:00����
Description:

Others:    // �������ݵ�˵��
Function List:


1. ....
History:


1. Date: Author: Modification:
2. ...

*************************************************/
#ifndef __if_decoder_ctrl_h
#define __if_decoder_ctrl_h






#include "typedef.h"
//#define DECODE_NORMAL  0x00
//#define DECODE_FF      0x01
//#define DECODE_FR      0x02
//#define DECODE_STOP    0x03

#define PLAY_MOD_NORMAL   0x00
#define PLAY_MOD_FF   0x01
#define PLAY_MOD_FB   0x02


//play control

//PLAY_NEXT
#define AUDIO_BK_EN

struct if_decoder_io{
    void *priv ;
	int (*input)(void *priv ,u32 addr , void *buf, int len,u8 type);
	/*
	priv -- ˽�нṹ�壬�ɳ�ʼ�������ṩ��
	addr -- �ļ�λ��
	buf  -- �����ַ
	len  -- ���볤�� 512 ��������
	type -- 0 --ͬ�������ȵ����ݶ������������ŷ��أ� ��1 -- �첽�����������ݶ������������ͷŻأ�

	*/
	int (*check_buf)(void *priv ,u32 addr , void *buf);
	void (*output)(void *priv  ,void *data, u32 len);
	u32 (*get_lslen)(void *priv);
	u32 (*store_rev_data)(void *priv,u32 addr ,int len);
};

typedef struct if_decoder_io IF_DECODER_IO;
typedef struct decoder_inf
{
	u16 sr ;            ///< sample rate
	u16 br ;            ///< bit rate
	u32 nch ;           ///<����
	u32 total_time;     ///<��ʱ��
}dec_inf_t ;


typedef struct __audio_decoder_ops {
	char *name ;                                                            ///< ����������
	u32 (*open)(void *work_buf, const struct if_decoder_io *decoder_io, u8 *bk_point_ptr);  ///<�򿪽�����

	u32 (*format_check)(void *work_buf);					///<��ʽ���

	u32 (*run)(void *work_buf, u32 type);					///<��ѭ��

	dec_inf_t* (*get_dec_inf)(void *work_buf) ;				///<��ȡ������Ϣ
	u32 (*get_playtime)(void *work_buf) ;					///<��ȡ����ʱ��
	u32 (*get_bp_inf)(void *work_buf);						///<��ȡ�ϵ���Ϣ;���ضϵ���Ϣ��ŵĵ�ַ

	//u32 (*need_workbuf_size)() ;							///<��ȡ�������������buffer
	u32 (*need_dcbuf_size)() ;		                        ///<��ȡ������Ҫ��buffer
	u32 (*need_rdbuf_size)();                               ///<��ȡ�������buf�Ķ��ļ�����buf�Ĵ�С
	u32 (*need_bpbuf_size)() ;				                ///<��ȡ����ϵ���Ϣ��Ҫ��buffer�ĳ���

	//void (*set_dcbuf)(void* ptr);			                ///<���ý���buffer
	//void (*set_bpbuf)(void *work_buf,void* ptr);			///<���öϵ㱣��buffer

	void (*set_step)(void *work_buf,u32 step);				///<���ÿ�����������
	void (*set_err_info)(void *work_buf,u32 cmd,u8 *ptr,u32 size);		///<���ý���Ĵ�������
	u32 (*dec_confing)(void *work_buf,u32 cmd,void*parm);
}audio_decoder_ops,decoder_ops_t;




#endif


