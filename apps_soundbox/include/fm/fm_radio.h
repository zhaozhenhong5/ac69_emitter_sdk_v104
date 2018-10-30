#ifndef FM_RADIO_H
#define FM_RADIO_H

#include "ui/ui_api.h"
#include "encode/encode.h"
// #include "common/includes.h"

// #define FM_DEBUG
#ifdef FM_DEBUG
#define fm_printf printf
#define fm_puts puts
#else
#define fm_printf(...)
#define fm_puts(...)
#endif


#define FM_AUTO_STEREO  0      ///>�Զ��������л�

typedef enum
{
    FM_UNACTIVE=0,
    FM_ACTIVE,
    FM_SCAN_STOP,
    FM_SCAN_BUSY,
    FM_SCAN_PREV,
    FM_SCAN_NEXT,
    FM_SCAN_ALL,
}FM_STA;

typedef struct _FM_MODE_VAR_
{
    RECORD_OP_API ** fm_rec_op;
    FM_STA scan_mode;       ///��ǰFM��״̬
    u16 wFreq;              ///<��ǰƵ��
    u16 wFreChannel; 	    ///<��ǰƵ��
    u16 wLastwTotalChannel;
    u16 wTotalChannel;      ///<��̨��
    u16 bAddr;	            ///<���ߵ�FM������ģ��ָ��
    u8 fm_mute;         ///��ǰFM��״̬��1: mute/ 0:play
    //FM_INFO * fm_info;
} FM_MODE_VAR;

typedef struct _FM_INFO_
{
    u8 dat[32];             ///FM��Ϣ����buf
} FM_INFO;

FM_MODE_VAR *fm_mode_var;   ///<FM״̬�ṹ��
FM_INFO *fm_info;           ///<FM��̨��Ϣ

extern const struct task_info fm_radio_task_info;


void fm_arg_close(void);
void fm_arg_open(void);
void fm_radio_powerdown(void);
u8 fm_radio_init(void);
/*----------------------------------------------------------------------------*/
/**@brief   FMģ���ʼ������
   @param   nrs����Χ:2-7
   @param   nc1����Χ:50-100
   @param   nc2����Χ:30-80
   @param   ncc����Χ:20-60
            ����4������Ϊ��̨�źŷ�ֵ���ѵ���̨��ֵ������趨��nrsֵ��
            ����ͬʱ��С��nc1��nc2��ncc��3��ֵ��Ϊ��̨������Ϊ��̨
   @param   fmif_g:fm�ź�ģ�����棨db������Ӧ�±�FMIF_G
   @return   void
   @note   void set_fm_scan(u8 nrs, u8 nc1, u8 nc2, u8 ncc,u8 fmif_g);
*/
/*----------------------------------------------------------------------------*/
void set_fm_scan(u8 nrs, u8 nc1, u8 nc2, u8 ncc,u8 fmif_g);

/*
FMIF_G	db
00000	0
00001	0
00010	0
00011	0
00100	0
00101	43
00110	42
00111	41
01000	40
01001	39
01010	38
01011	37
01100	36
01101	35
01110	34
01111	33
10000	32
10001	31
10010	30
10011	29
10100	28
10101	27
10110	25
10111	24
11000	22.5
11001	21
11010	20
11011	18
11100	15.5
11101	13
11110	9
11111	6.8
*/

#endif

