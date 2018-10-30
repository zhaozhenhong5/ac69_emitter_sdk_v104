/*
 *********************************************************************************************************
 *                                                AC46
 *                                      lcd ui menu display
 *                                             CODE
 *
 *                          (c) Copyright 2015-2016, ZHUHAI JIELI
 *                                           All Rights Reserved
 *
 * File : *
 * By   : jamin.li
 * DATE : 2015-10-08 build this file
 *********************************************************************************************************
 */


#ifndef _LCD_UI_MENU_H_
#define _LCD_UI_MENU_H_

#include "sdk_cfg.h"
#include "dac/dac_api.h"
#include "ui/lcd/lcd_disp.h"
#include "uicon/ui_con.h"
#include "uicon/ui.h"
#include "ui/ui_api.h"
#include "dec/decoder_phy.h"
#include "music.h"
#include "music_ui.h"
#include "ui/ui_common.h"
// #include "uicon/ui_con1.h"
#include "fm/fm_radio.h"
#include "timer.h"
#include "key_drv/key.h"
#include "file_operate/file_op.h"
#include "dac/dac.h"
#include "rtc_setting.h"
#include "bluetooth/bt_ui.h"
#include "common/msg.h"
#include "rtos/task_manage.h"

#if REC_EN
#include "encode/encode.h"
#endif

#include "file_operate/file_bs_deal.h"

enum
{
    MENU_TASK_MAIN_APP = 0,
    MENU_TASK_RTC_APP,
    MENU_TASK_MUSIC_APP,
    MENU_TASK_RECORD_APP,
    MENU_TASK_RADIO_APP,
    MENU_TASK_LINEIN_APP,
    MENU_TASK_UDISK_APP,
    MENU_TASK_BTSTACK_APP,
    MENU_TASK_APP_MAX,
};


enum
{
    MENU_OPT_NOERR,
    MENU_OPT_HOTKEY,
    MENU_OPT_BACK,
    MENU_OPT_BACKMAIN, //����������
    MENU_OPT_WAITING,   //�����ȴ�����
    MENU_OPT_CALLCBK,   // �ص�����
    MENU_OPT_ACTIVE_SCROLL, // ѡ�������
};

typedef int (*item_cbk_f)(void *arg);
typedef void (*app_menulist_f)(u32 );

//#pragma pack (1) /*ָ����1�ֽڶ���*/

typedef struct
{
    u8    icon_a;       //ѡ��ͼ��
    u8    icon_b;       //δѡ��ͼ��
    s8    choise_sn;    //��ѡ�˵���ѡ����
    u8    reserver;
    s16   head_id;      //�˵�ͷ��ʾ�ַ���id,-1 Ϊû�б���
    u16   item_total;   //�˵�������
    u16   item_active;  //��˵����
    u16   item_cfg;     //���ò˵�������ûص���ʽ���Ƿ��в˵������
    u16   bs_total_dir; //���Ŀ¼�µ��ļ��и���
    u16   reserver2;     //δ��
    u8   *item_id_gp;      //�˵����ַ���ID��
    const char *str_table; //�ַ�����
    void *item_cbk_gp;     //�˵�ѡ�лص�������
} menu_list_t;

//#pragma pack () /*ȡ��ָ�����룬�ָ�ȱʡ����*/

//item_cfg bit_map
#define ITEM_CFG_BIT_CBKGP           (1<<0) // �ص�Ϊ���ݱ��
#define ITEM_CFG_BIT_ACTIVE_SCROLL   (1<<1) // �˵�֧��ѡ�й���

//�˵�������IDֵ
#define ITEM_BACK_FIRST_FIRST_ID     0
#define ITEM_BACK_NO_ITEM_ID         0xffff

#define MENU_LAYER_MAX      (MAX_DEEPTH+1)     //�˵���������
#define MENU_ITEM_MAXLEN    14    //�˵�����ʾ����󳤶�
#define MENU_ITEM_STRLEN     16    //�˵������ַ���ĳ��ȣ�����������
#define MENU_HEAD_STRLEN    16    //�˵�ͷ���ַ���ĳ��ȣ�����������

#define MENU_ITEMS_SCREEN        3     //ÿ���Ĳ˵������
#define MENU_MAIN_SCREEN         4     //ÿ����ͼ�����
#define MENU_BROWSER_SCREEN  4     //ÿ�����ļ������

#define MENU_BROWSER_STRLEN  128  //ÿ����Ϣ��ʾ�ĳ���

#define MENU_TEXT_SCROLL_TIME                  1  //�������ʱ�䣬��λ:500MS
#define MENU_TEXT_SCROLL_CHAR                 1  //ÿ�ι������ַ�����
#define MENU_TEXT_SCROLL_MAXCNT            (64)  //������������������Ϊ��ʱ�˳�

#define MENU_WAIT_TIME_MAX                 (MENU_TEXT_SCROLL_TIME * MENU_TEXT_SCROLL_MAXCNT) //��λ:500MS

extern void enter_menulist_display(u32 arg);

#endif/*_LCD_UI_MENU_H_*/
