#ifndef _EMITTER_USER_H_
#define _EMITTER_USER_H_


#include "sdk_cfg.h"

#define A2DP_SOURCE_NULL       0x00
#define A2DP_SOURCE_AUX        0x01
#define A2DP_SOURCE_DEC        0x02
#define A2DP_SOURCE_TEST       0x03

typedef struct _EMITTER_INFO
{
	volatile u8 role;
	u8 a2dp_source_media;
}EMITTER_INFO_T;
EMITTER_INFO_T emitter_info;

void set_emitter_role(u8 role);
u8 get_emitter_role(void);
void emitter_init(u8 role);

void set_emitter_aux_media(u8 aux_en);
void set_emitter_decode_media(u8 dec_en);

//extern api from lib
extern u8 a2dp_send_media_to_sbc_encoder(u32* pbuf, u32 len,u16 rate,u8 ch) ;//ADC 中断调用
extern void __emitter_send_media_toggle(u8 toggle);
extern void search_bd_name_filt_handle_register(u8 (*handle)(u8 *data,u8 len)) ;
extern void search_bd_addr_filt_handle_register(u8 (*handle)(u8 *addr)) ;
extern void bd_inquiry_result_custom_register(u8 (handle)(char *bd_name,u8 name_len,u8 *addr));

#endif
