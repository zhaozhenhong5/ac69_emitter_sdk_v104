#ifndef __GUI_MSG_H__
#define __GUI_MSG_H__

#include "typedef.h"

//typedef void (*MsgCB)(void);

typedef struct
{
	u16 msg_id;			//�ؼ�id��  [15..12]:�ؼ�����
	void *p;			//���ڴ��ݲ���
//    MsgCB cb;			//��ʾ�ص�����
    u32 result;			//���ؽ��
}GMSG;

typedef struct
{
	GMSG *msg_pool;		//��Ϣ��
	u16 msg_read;		//��ָ��
	u16 msg_write;		//дָ��
	u16 msgcnt;			//��Ϣ�������
} GUI_MSG;

void gui_msg_init(u16 element);
void gui_flush_all_msg(void);
void gui_send_fifo_msg(u16 msg_id,u8 type,void *p);
void gui_send_lifo_msg(u16 msg_id,u8 type,void *p);
GMSG *gui_get_msg(void);

#endif
