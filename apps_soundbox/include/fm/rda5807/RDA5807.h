#ifndef _RDA_5807_H_
#define _RDA_5807_H_

#include "sdk_cfg.h"



void rda5807_init(void);
bool rda5807_set_fre(u16 fre);
void rda5807_powerdown(void);
void rda5807_mute(u8 dir);
bool rda5807_read_id(void);
void rda5807_setch(u8 db);

//extern u8 rda5807_dat[12];
//extern u8 read_dat[10];
//extern u8 fm_type;

#define RDA5807_WR_ADDRESS  0x20       ///<RDA5807 д��ַ
#define RDA5807_RD_ADDRESS  0x21       ///<RDA5807 ����ַ

#define RDA5807_RSSI  0x4

#define rda5807_stc() (read_dat[0] & (1 << 6))       ///<��Ƶ����
#define rda5807_true() (read_dat[2] & (1 << 0))      ///<�Ƿ���̨
#define rda5807_rssi() ((read_dat[2] >> 1))          ///<�����ź�ǿ��


#endif
