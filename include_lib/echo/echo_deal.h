#ifndef _ENCHO_DEAL_H_
#define _ENCHO_DEAL_H_

#include "typedef.h"
#include "string.h"
#include "mem/malloc.h"
#include "common/app_cfg.h"
#include "rtos/os_api.h"
#include "rtos/os_cfg.h"
#include "common/error.h"
#include "dac/ladc_api.h"
#include "dac/dac_api.h"
#include "reverb_api.h"
#include "aec/AnalogAGC.h"

#define ECHO_RUN_NAME   "ECHO_RUN"

///echo spec define src
#define ECHO_11K_SR    10500
#define ECHO_12K_SR    11424
#define ECHO_08K_SR    7616

#define ECHO_22K_SR    21000
#define ECHO_24K_SR    22848
#define ECHO_16K_SR    15232

#define ECHO_44K_SR    42000
#define ECHO_48K_SR    45696
#define ECHO_32K_SR    30464

///normal src
#define NOR_11K_SR    11025
#define NOR_12K_SR    12000 
#define NOR_08K_SR    8000 

#define NOR_22K_SR    22050 
#define NOR_24K_SR    24000 
#define NOR_16K_SR    16000 

#define NOR_44K_SR    44100 
#define NOR_48K_SR    48000 
#define NOR_32K_SR    32000 

#define REVERB_SR 	NOR_16K_SR


typedef struct _MIC_VAR_SET
{
	u8 use_magc;//agc_sw
	s8 max_gain;//agc�������
	s8 min_gain;//agc��С����
	s32 tms;//����֮����������ƶ���ms
}MIC_VAR_SET;

typedef struct _MIC_VAR
{
	int Aagc_Target;
	int Aagc_TimeConstant;
    short *buff;
	u8 use_magc;
	s8 cur_mic_gain;
	s8 last_mic_gain;
	s8 max_gain;
	s8 min_gain;
	s8 MAX_NDT_MIC_GAIN;
	s8 MIN_NDT_MIC_GAIN;
	Analog_AGC Aagc;
}MIC_VAR;

typedef struct _REVERB_API_STRUCT_
{
    REVERB_PARM_SET *reverb_parm_obj;  //����
	reverb_audio_info *audio_info;
    unsigned char *ptr;                   //����bufָ��
    REVERB_FUNC_API *func_api;            //����ָ��
    REVERB_IO *func_io;                   //��������ӿ�
    LADC_CTL *ladc_p;
	MIC_VAR  *mic_var;
}REVERB_API_STRUCT;


extern void reverb_stop(void *reverb_api_obj);
extern REVERB_FUNC_API *get_reverb_func_api(void);

extern void echo_set_vol(u8 vol);
extern void echo_reg_digital_vol_tab(void *tab, u8 len);

void reg_mic_var(MIC_VAR_SET* parm);
void reg_ef_reverb_parm2(EF_REVERB_PARM2 * parm);

#if ECHO_PARM
void set_reverb_parm(void *reverb_obj, u16 sr, u16 pre_delay_ms);
#else
void set_reverb_parm(void *reverb_obj, u16 deep_val, u16 vol_val);
#endif

void howlingsuppress_sw(u8 on);

#endif
