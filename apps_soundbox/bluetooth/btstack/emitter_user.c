#include "bluetooth/avctp_user.h"
#include "aec/aec_api.h"
#include "common/app_cfg.h"
#include "dac/dac_api.h"
#include "common/msg.h"
#include "sdk_cfg.h"
#include "clock.h"
#include "rtc/rtc_api.h"
#include "play_sel/play_sel.h"
#include "uart_param.h"
#include "emitter_user.h"
#include "dac/ladc.h"

EMITTER_INFO_T emitter_info;

void set_emitter_role(u8 role)
{
	emitter_info.role = role;
	__set_emitter_role(role);
}

u8 get_emitter_role(void)
{
	return emitter_info.role;
}

void set_emitter_aux_media(u8 aux_en)
{
	if(aux_en) {
		if(emitter_info.a2dp_source_media == A2DP_SOURCE_AUX)
			return;
		emitter_info.a2dp_source_media = A2DP_SOURCE_AUX;
		emitter_aux_open();
		__emitter_send_media_toggle(aux_en);
	} else {
		if(emitter_info.a2dp_source_media == A2DP_SOURCE_AUX)
		{
			emitter_info.a2dp_source_media = A2DP_SOURCE_NULL;
			__emitter_send_media_toggle(aux_en);
			emitter_aux_close();
		}
	}
}

void set_emitter_decode_media(u8 dec_en)
{
	if(dec_en) {
		if(emitter_info.a2dp_source_media == A2DP_SOURCE_DEC)
			return;
		emitter_info.a2dp_source_media = A2DP_SOURCE_DEC;
		__emitter_send_media_toggle(dec_en);
	} else {
		emitter_info.a2dp_source_media = A2DP_SOURCE_NULL;
		__emitter_send_media_toggle(dec_en);
	}
}	

u8 src_task_priority()
{
	return TaskSrcPrio;
}

//#define SEARCH_BD_ADDR_LIMIITED
u8 bd_addr_filt[][6] = 
{
	{0x8E,0xA7,0xCA,0x0A,0x5E,0xC8},/*S10_H*/
	{0xA7,0xDD,0x05,0xDD,0x1F,0x00},/*ST-001*/
	{0xE9,0x73,0x13,0xC0,0x1F,0x00},/*HBS 730*/
	{0x38,0x7C,0x78,0x1C,0xFC,0x02},/*Bluetooth*/
};
u8 search_bd_addr_filt(u8 *addr)
{
	u8 i;
	puts("bd_addr:");
	put_buf(addr,6);
	for(i=0;i<(sizeof(bd_addr_filt)/sizeof(bd_addr_filt[0]));i++)
	{
		if(memcmp(addr,bd_addr_filt[i],6) == 0) {
			printf("bd_addr match:%d\n",i);
			return 0;
		}
	}
	puts("bd_addr not match\n");
	return 1;
}

//#define SEARCH_BD_NAME_LIMIITED
#if 0
u8 bd_name_filt[][32] = 
{
	"BT-SPEAKER",
	"CSC608"
	"QCY-QY19",	
	"Newmine",
	"HT1+",
	"S-35",
	"HUAWEI AM08",
	"AC69_Bluetooth",
	"BV200",
	"MNS",
	"Jam Heavy Metal",
	"Bluedio",
	"HR-686",
	"BT MUSIC",
	"BW-USB-DONGLE",
	"S530",
	"XPDQ7",
	"MICGEEK Q9S",
	"S10_H",
	"HBS-730",
	"SPORT-S9",
	"Q5",
	"IAEB25",
	"QY7",
	"MS-808",
	"LG HBS-730",
	"NG-BT07"
};
#else
u8 bd_name_filt[][32] = 
{
	"AC690x_SDK",	
};
#endif

u8 search_bd_name_filt(u8 *data,u8 len)
{
	char bd_name[32] = {0};
	u8 i,bd_name_len;
	
	if(len >(sizeof(bd_name))) {
		printf("bd_name_len error:%d\n",len);
		return 1;
	}

	memcpy(bd_name,data,len);
	bd_name_len = len;
	printf("inquiry_bd_name:%s,len:%d\n",bd_name,len);

	for(i=0;i<(sizeof(bd_name_filt)/sizeof(bd_name_filt[0]));i++)
	{
		if(memcmp(data,bd_name_filt[i],bd_name_len) == 0) {
			puts("\n*****find dev ok******\n");
			return 0;
		}
	}

	return 1;
}

//#define BD_INQUIRY_RESULT_CUSTOM
#ifdef BD_INQUIRY_RESULT_CUSTOM
u8 bd_inquiry_result_custom(char *bd_name,u8 name_len,u8 *addr)
{
	char bt_name[32] = {0};
	u8 len;

	if(name_len == 0) {
		puts("No_eir\n");
	}
	else {
		len	= name_len;
		if(len > 32)
			len = 31;
		/* display bd_name */
		memcpy(bt_name,bd_name,len);
		printf("inquiry_bd_name:%s,len:%d\n",bt_name,name_len);
	}

	/* display bd_addr */
	put_buf(addr,6);

    /* You can connect the specified bd_addr by below api      */
	//user_send_cmd_prepare(USER_CTRL_START_CONNEC_VIA_ADDR,6,addr);

	return 0;
}
#endif

/*
	Inquiry again will ignore the first few bd_addr according to SEARCH_BD_ADDR_FILT.
    Define SEARCH_BD_ADDR_FILT as 0 means all bd_addrs are valid by inquiried.
*/
#define SEARCH_BD_ADDR_FILT     0 ///<maximum:10

void emitter_init(u8 role)
{
	//emitter role shoule set firstly
	set_emitter_role(role);	

#ifdef SEARCH_BD_ADDR_LIMIITED
	search_bd_addr_filt_handle_register(search_bd_addr_filt);
#endif

#ifdef SEARCH_BD_NAME_LIMIITED
	search_bd_name_filt_handle_register(search_bd_name_filt);
#endif

#ifdef BD_INQUIRY_RESULT_CUSTOM
	bd_inquiry_result_custom_register(bd_inquiry_result_custom);
	__set_inquiry_again_flag(0);//0:inquiry complete	1:inquiry again when inquiry complete,unless dev match
#else
	__set_inquiry_again_flag(0);//0:inquiry complete	1:inquiry again when inquiry complete,unless dev match
#endif

	__set_bd_addr_filt(SEARCH_BD_ADDR_FILT);
}



