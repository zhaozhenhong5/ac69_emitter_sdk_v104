#include "bluetooth/avctp_user.h"
#include "aec/aec_api.h"
#include "common/app_cfg.h"
#include "dac/dac_api.h"
#include "vm/vm_api.h"
#include "common/msg.h"
#include "sdk_cfg.h"
#include "clock.h"
#include "ui/led/led.h"
#include "key_drv/key_drv_ad.h"
#include "dac/dac.h"
#include "fcc_test.h"
#include "dev_manage/dev_ctl.h"
#include "dac/dac.h"
#include "dac/eq.h"
#include "rtc/rtc_api.h"
#include "play_sel/play_sel.h"
#include "rcsp/rcsp_interface.h"
#include "uart_param.h"

extern volatile u8 low_power_flag;

static void in_low_pwr_port_deal(u8 mode) sec(.poweroff_text);
static void in_low_pwr_port_deal(u8 mode)
{
	if(mode == 2)   //enter sleep mode
	{
		JL_PORTA->DIR = 0xffff;
		JL_PORTA->PU = 0;
		JL_PORTA->PD = 0;
		JL_PORTA->DIE = 0;

		/* JL_PORTA->DIR |= 0xffdf; */
		/* JL_PORTA->PU &= ~0xffdf; */
		/* JL_PORTA->PD &= ~0xffdf; */
		/* JL_PORTA->DIE &= ~0xffdf; */
	}
	else
	{
		JL_PORTA->DIR |= 0xffe7;
		JL_PORTA->PU &= ~0xffe7;
		JL_PORTA->PD &= ~0xffe7;
		JL_PORTA->DIE &= ~0xffe7;

		JL_PORTA->OUT &= ~BIT(4);  //close led
	}

    JL_PORTB->DIR = 0xffff;
    JL_PORTB->PU  = 0;
    JL_PORTB->PD  = 0;
    JL_PORTB->DIE = 0;

    JL_PORTC->DIR = 0xffff;
    JL_PORTC->PU  = 0;
    JL_PORTC->PD  = 0;
    JL_PORTC->DIE = 0;

	/*ע��spi flash�ļ����Ų��ܹ�*/
	JL_PORTD->DIR |= 0xfff0;
	JL_PORTD->PU  &= ~0xfff0;
	JL_PORTD->PD  &= ~0xfff0;
	JL_PORTD->DIE &= ~0xfff0;

    /////////////////usb///////////////////
    USB_DP_DIR(1); //DP����Ϊ����
    USB_DM_DIR(1); //DP����Ϊ����
    USB_DP_PD(0);
    USB_DM_PD(0);
    USB_DM_PU(0);
    USB_DP_PU(0);
    JL_USB->CON0 = 0;
}

static u32 sniff_time_cnt =0;
/*�������û��ӿڣ�Э��ջ�ص�����*/
extern void io_key_init(void);
extern void ad_key0_init_io();
extern u8 is_pwrtimer_en();
static void out_low_pwr_deal(u32 time_ms)
{
	/* printf("%d\n",time_ms); */

#if KEY_IO_EN
    io_key_init();
#endif

#if KEY_AD_EN
	ad_key0_init_io();
#endif


	if(is_pwrtimer_en())
	{
		return;
	}

	sniff_time_cnt += time_ms;
	/* printf("%d\n",sniff_time_cnt); */
	if(sniff_time_cnt>=3000)
	{
		sniff_time_cnt =0;
		B_LED_ON();
	}else{
		B_LED_OFF();
	}

   // printf("sniff_time_cnt:%d  time_ms:%d\n",sniff_time_cnt,time_ms);
}
static void bt_updata_run()
{
	puts("-------bt_updata_run\n");
    os_taskq_post_msg("MainTask",2,MSG_UPDATA,1);
}
static void low_pwr_deal(u8 mode,u32 timer_ms) sec(.poweroff_text);
static void low_pwr_deal(u8 mode,u32 timer_ms)
{
	if(mode)
    {
		in_low_pwr_port_deal(mode);
	}
	else
	{
		out_low_pwr_deal(timer_ms);
	}
}

///---sdp service record profile- �û�ѡ��֧��Э��--///
#define USER_SUPPORT_PROFILE_SPP    1
#define USER_SUPPORT_PROFILE_HFP    1
#define USER_SUPPORT_PROFILE_A2DP   1
#define USER_SUPPORT_PROFILE_AVCTP  1
#define USER_SUPPORT_PROFILE_HID    0
static void bt_profile_select_init(void)
{
    u8 support_profile;
    support_profile = SPP_CH | HFP_CH | A2DP_CH | AVCTP_CH | HID_CH | AVDTP_CH;
#if (USER_SUPPORT_PROFILE_HFP==0)
    support_profile &= ~HFP_CH;
#endif
#if (USER_SUPPORT_PROFILE_AVCTP==0)
    support_profile &= ~AVCTP_CH;
#endif
#if (USER_SUPPORT_PROFILE_A2DP==0)
    support_profile &= ~A2DP_CH;
    support_profile &= ~AVCTP_CH;
    support_profile &= ~AVDTP_CH;
#endif
#if (USER_SUPPORT_PROFILE_SPP==0)
    support_profile &= ~SPP_CH;
#endif
#if (USER_SUPPORT_PROFILE_HID== 0)
    support_profile &= ~HID_CH;
#endif

    bt_cfg_default_init(support_profile);/*�ⲿ����֧��ʲôЭ��*/

#if BT_HID_INDEPENDENT_MODE
    set_hid_independent_info();//Ҫ�� bt_cfg_default_init()���ú�
#endif

}

//��������������������ӻ�����Ϣ��vm
static void bt_save_stereo_info(u8 stereo_type)
{
	u8 stereo_info;
    stereo_info = stereo_type;
    vm_write_api(VM_BT_STEREO_INFO,&stereo_info );
	printf("------stereo_info=0x%x\n",stereo_info );
}

///���������ӳɹ�֮��ͬ��ϵͳ�������ӻ�
static void stereo_sys_vol_sync()
{
     stereo_host_cmd_handle(MSG_VOL_STEREO,dac_ctl.sys_vol_l);
#if EQ_EN
     os_taskq_post_msg("btmsg", 1, MSG_BT_SYNC_STEREO_EQ);
#endif
}

///���������
static void stereo_deal_cmd(u8 msg,u8 value)
{
    if(msg < MSG_MAIN_MAX)
    {
         os_taskq_post_msg("MainTask", 2, msg,value);
    }
    else if(keymsg_task_name)
    {
        os_taskq_post_msg(keymsg_task_name, 2, msg,value);
    }
}


#if(BT_MODE == NORMAL_MODE)
	static const char host_name[]="AC69-Classic";
#else
	static const char host_name[]="AC69-RAM";
#endif
static const char hid_name[]="AC69_HID";
extern void __bt_set_background_conn_type_flag(u8 flag);

/*
	PIN CODE�б���������ʹ��pincode��Ե�ʱ�򣬷����������
	���б�һ��һ��������ԡ��б��ԱԽ�࣬����ʱ��Խ����
*/
char pin_code_list[10][4] =
{
	{'0','0','0','0'},
	{'1','2','3','4'},
	{'8','8','8','8'},
	{'1','3','1','4'},
	{'4','3','2','1'},
	{'1','1','1','1'},
	{'2','2','2','2'},
	{'3','3','3','3'},
	{'4','4','4','4'},
	{'5','5','5','5'}
};

static void bt_function_select_init()
{
    /*������������ѡ������*/
#if(BT_MODE==NORMAL_MODE)
    u8 debug_addr[6] = {0x88, 0x67, 0x66, 0x89, 0x33, 0x11};
	__set_use_default_value(0);
#else
    u8 debug_addr[6] = {0x11, 0x22, 0x33, 0x33, 0x22, 0x11};
	__set_use_default_value(1);
#endif
    __set_host_name(host_name,sizeof(host_name));
    __set_hid_name(hid_name,sizeof(hid_name));
    __set_pin_code((char*)pin_code_list,(sizeof(pin_code_list)/sizeof(pin_code_list[0])));
    __set_bt_mac_addr(debug_addr);

    bt_profile_select_init();

    __bt_set_sniff(SNIFF_MODE_CONF,SNIFF_CNT_TIME);/*���ý���sniff�ǽ���poweroff����powerdown�����ÿ��ж���s����sniff*/

    __bt_set_update_battery_time(10000); /*���õ�����ʾ���͸��µ�����ʱ�䣬Ϊ0��ʾ�رյ�����ʾ���ܣ���λ���룬u32, ����С��5000ms*/
    __bt_set_a2dp_auto_play_flag(0); /*�߼���Ƶ��������Զ����Ÿ���ʹ��, 0��ʹ�ܣ�1ʹ��*/
    __set_simple_pair_flag(1);       /*�ṩ�ӿ��ⲿ������Է�ʽ,1ʹ�ܼ�����ԡ�0ʹ��pin code, ��ʹ�������ļ���ֵ*/
#if BT_STEREO
    __set_user_ctrl_conn_num(2);     /*�û�����֧�����ӵĸ�����1 �� 2*/
    __set_auto_conn_device_num(2);   /*�ýӿ����������ϵ������Ҫ���������豸�ĸ�����0��ʾ�ϵ粻����������20��Ч��ֱ������һ��*/
    __set_stereo_device_indicate("MO");/*���ö���������ʶ,support�����ַ���ʶ��inquiryʱ����,��������Ӧ�ı�ʶ����������*/
    __set_searche_timeout_value(12000); /*��������������ʱ�䡣ms��λ������������*/
    __set_connect_stereo_timeout_value(0); /*���û������䳬ʱʱ�䣬0Ϊ����ʱ��ֱ�������϶���Ϊֹ*/
    __set_start_connet_stereo(1);          /*���ÿ����Ƿ��������*/
    __set_soundtrack_compound(1);          /*����soundtrack,����������������ʹ��*/
    __set_indicate_name_cmp_en(0);      /*������ͬ�������ֲ�������ʹ��*/
#else
	__set_user_ctrl_conn_num(1);     /*�û�����֧�����ӵĸ�����1 �� 2*/
    __set_auto_conn_device_num(1);   /*�ýӿ����������ϵ������Ҫ���������豸�ĸ�����0��ʾ�ϵ粻����������20��Ч��ֱ������һ��*/
#endif
    __set_page_timeout_value(5000); /*��������ʱ�䳤������,��ʹ�øú���ע��ʹ�ã�ms��λ,u16*/
	__set_sbc_cap_bitpool(53);//38 
#if BT_HID_INDEPENDENT_MODE
    __set_hid_auto_disconn_en(0);    /*�Ƿ��Զ��Ͽ��ֻ���HID����,1��Ͽ���0�Ǳ���HID����*/
#else
    __set_hid_auto_disconn_en(1);    /*�Ƿ��Զ��Ͽ��ֻ���HID����,1��Ͽ���0�Ǳ���HID����*/
#endif
    __set_test_masetr_enable(1);     /*�ӿ������ǲ��Ժ�ģʽ��������SDK*/
    __set_super_timeout_value(8000); /*����ʱ��ʱ�������á�ms��λ����������Ч*/
    __set_connect_scan_timeout_value(0);/*//���ÿ��������ɷ��ֿ����ӵ�ʱ�䣬Ϊ��ʡ�磬����һ���Զ��رտ����ӵ�ʱ�䡣ms��λ��Ϊ 0 ʱ��ʹ���Զ��ر�*/
    //__set_connect_scan_with_key(0);     /*������һ̨֮�����ӵڶ�̨�Ƿ�ͨ����������connect_scan*/
	__bt_set_music_back_play_flag(1);
    __set_music_break_in_flag(1);  /* ��Ƶ���Ͽ��أ�0�������ϣ�1������*/
    __set_hfp_switch(1);             /*ͨ�����Ͽ��أ�0�������ϣ�1������*/
#if BT_STEREO
	__bt_set_background_conn_type_flag(0);
#else
	__bt_set_background_conn_type_flag(1);
#endif
	//__change_hci_class_type(BD_CLASS_WEARABLE_HEADSET);/*�ı������豸���ͣ����Ըı����ӳɹ���ʾͼ��*/
}

 /*
    ������һЩ��Ĺ��Ӻ���ע�ᣬһ�����ڸı������
    Ϊ�˼��ݲ�ͬ�ͻ��Բ�ͬ�������������й��Ӻ�����ע�Ὣʹ��Ĭ�ϲ�����
    �������һЩ�ص������ӿڣ��û��������������ã���
    �ص���������Э��ջ��ȡ��Ҫֵ���߼�ʱ�����Ľӿ�
 */
/*�����ȼ��仯*/
#define POWER_FLOOR_LEVEL   32
static int bt_get_battery_value()
{   //ֻ֧��1-9ֱ�ӵ�ֵ��9���ȼ�
    u16 battery_level;
    if(get_battery_level() <= POWER_FLOOR_LEVEL)
        battery_level = 1;
    else
        battery_level = get_battery_level() - POWER_FLOOR_LEVEL;
    return battery_level;
}

/*�绰ʱ�ı�������״ֵ̬��ע���ǵ绰���������ӿ�ʹ�ú���з���*/
static void call_vol_change(int vol)
{
	//iphone(0-15level)
    printf("vol %d",vol);
}
/*�ýӿڶ�ȡ����ʱ��Ӹûص��������أ�Ĭ�������Ͼ���һ�λص���
Ҫʹ��Զ���豸���ֵĿ���copy����*/
static void bt_read_remote_name(u8 *name)
{
    puts("\n remote name : ");
    puts((char *)name);
    puts("\n");
}

/*����һ����־������Ҫ������Ӳ�����*/
static void bt_is_auto_conn(u8 auto_conn_flag)
{   //���ֻ����ɹ����Ǳ����ɹ�
    if(auto_conn_flag)
    {
        //�����ɹ�
    }else{
        //�����ɹ�
    }
}

extern int spp_mutex_init(void);
extern int spp_mutex_del(void);
static void spp_data_deal(u8 packet_type, u16 channel, u8 *packet, u16 size)
{
    //��Ϊ�޸���user_send_cmd_prepare�ӿڷ��ʹ������ݻ�peng�ŷ���ŷ��ء�
    //���Ǹûص�ָ��������Э��ջѭ�����õģ�������user_send_cmd_prepare�ӿڷ�����
    //Ҫ�����ÿ���ֱ��int spp_data_try_send(const u8* buf, u16 len)�ӿ�
    switch(packet_type)
    {
        case 1:
             //���ӳɹ�
             puts("spp connect\n");
    		 spp_mutex_init();
             break;
        case 2:
             //���ӶϿ�
             puts("spp disconnect\n");
			 spp_mutex_del();
             break;
        case 7://DATA
            ///puts("SP ");
		if(packet[0]=='9')
		{
		    //os_taskq_post_msg(keymsg_task_name, 1,MSG_BT_SPP_UPDATA);
		}
#if EQ_EN
            if(packet[0]=='E'&&packet[1]=='Q')
             {
                    update_ef_info(packet+4,size-4,packet[2]);
                    break;
             }
#endif // EQ_EN
             break;
    }
}

#if SUPPORT_APP_RCSP_EN
extern void ble_enter_mutex(void);
extern void ble_exit_mutex(void);
static void spp_rcsp_data_deal(u8 packet_type, u16 channel, u8 *packet, u16 size)
{
    //��Ϊ�޸���user_send_cmd_prepare�ӿڷ��ʹ������ݻ�peng�ŷ���ŷ��ء�
    //���Ǹûص�ָ��������Э��ջѭ�����õģ�������user_send_cmd_prepare�ӿڷ�����
    //Ҫ�����ÿ���ֱ��int spp_data_try_send(const u8* buf, u16 len)�ӿ�
    switch(packet_type)
    {
        case 1:
             //���ӳɹ�
             puts("spp connect\n");
    		 spp_mutex_init();
#if (BLE_BREDR_MODE&BT_BLE_EN)
 			 ble_enter_mutex();
#endif
 			 rcsp_event_com_start(RCSP_APP_TYPE_SPP);
             rcsp_register_comsend(bt_spp_send_data);
             break;
        
		case 2:
             //���ӶϿ�
             puts("spp disconnect\n");
             rcsp_event_com_stop();
#if (BLE_BREDR_MODE&BT_BLE_EN)
             ble_exit_mutex();
#endif
			 spp_mutex_del();
             break;
        
		case 7://DATA
            ///puts("SP ");
            rcsp_comdata_recieve(packet,size);
             break;
    }
}
#endif

/*set_device_volume,get_dac_vol�Ǹ߼���Ƶͬ���ֻ�������صģ�ע����绰ʱ�������
�û���ʹ��*/
static void set_device_volume(int volume )
{
	u8 dac_max_vol = get_max_sys_vol(1);
    dac_ctl.sys_vol_l  = volume * dac_max_vol / 0x7F;
    dac_ctl.sys_vol_r = dac_ctl.sys_vol_l  ;
    if(is_dac_mute()) {
   		dac_mute(0,1);
    }
#if BT_STEREO
	if(!is_check_stereo_slave())
	{
		stereo_host_cmd_handle(MSG_VOL_STEREO,dac_ctl.sys_vol_r);
			
	}
#endif
    set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_ON);
	printf("vol_sync,dac = %d\n",dac_ctl.sys_vol_l);
}

static int get_dac_vol()
{
	int dac_max_vol = get_max_sys_vol(1);
	int phone_vol = dac_ctl.sys_vol_l * 0x7F / dac_max_vol;
	printf("vol_sync,phone = %d\n",phone_vol);
    /*The value 0x0 corresponds to 0%. The value 0x7F corresponds to 100%.*/
    return phone_vol;
}

//phone_talking_end,save phone_vol and recover dac_vol
static void phone_sync_sys_dac_vol(u8 vol,u8 phone_vol)
{
   dac_ctl.sys_vol_l = vol;
   dac_ctl.sys_vol_r = vol;
   dac_ctl.phone_vol = phone_vol;
   vm_write_api(VM_PHONE_VOL,&dac_ctl.phone_vol);
   printf("phone_vol:%d ,sys_vol:%d\n",dac_ctl.phone_vol,vol);
}

//phone_talking_start,set phone_vol with vm_phone_vol
#define PHONE_VOL_REMEMBER
static u8 phone_sync_phone_vol(u8 phone_vol_max)
{
	u8 phone_vol = 0;
    //vm_read_api(VM_PHONE_VOL,&phone_vol);
  	printf("phone_vol:%d,max_vol:%d\n",dac_ctl.phone_vol,phone_vol_max);
	if(dac_ctl.phone_vol < 10)
		dac_ctl.phone_vol = phone_vol_max;

#ifndef	PHONE_VOL_REMEMBER
	dac_ctl.phone_vol = phone_vol_max;
#endif

   	dac_ctl.sys_vol_l = dac_ctl.phone_vol;
   	dac_ctl.sys_vol_r = dac_ctl.phone_vol;
	return dac_ctl.phone_vol;
}

extern u8 get_key_invalid_flag(void);

/*����Ƿ���Խ���͹���*/
/*1 : ���Խ�lowpower   0: �����Խ�lowpower*/
static u32 check_enter_lowpower(void)
{


	//�Ǿ���ģʽ����lowpowerģʽ
    if(!is_auto_mute())
	{
		return 0;
	}

	if(play_sel_busy()) {
		//puts("play_sel_busy\n");
		return 0;
	}

	//playing key_voice,don't enter lowpower
	if(dac_ctl.sniff_check) {
		return 0;
	}

	//close dac module
	if(dac_ctl.toggle) {
		puts("---off----\n");
		//dac_off_control();
		os_taskq_post(MAIN_TASK_NAME,1,MSG_DAC_OFF);
		return 0;
	}

#if SYS_LVD_EN
	if(low_power_flag)
		return 0;
#endif

	return 1;
}

/*Ĭ��û��ע�ᣬ�ڶ�ȡ�������ļ������á��û���Ҫ�Ե�ַ�����⴦��Ŀ��Բ������ӽ����޸�*/
static void bt_addr_deal(char *addr)
{
    /*��ַָ�룬��ַֻ��6��byte��ע��Խ����ʻ�������, ����*/
    puts("bt_addr_deal\n");
    addr[0] = 0x1A;
    addr[1] = 0x2A;
    addr[2] = 0x3A;
//    addr[3] = 0x1A;
//    addr[4] = 0x2A;
//    addr[5] = 0x3A;
}

static u8 bt_aec_priority()
{
	return (u8)TaskBtAecPrio;
}

//�ú�����Ҫ����Ϊ��һЩ���ֵ���ܻᱻ�û����޸�Ӱ�쵽������Ҫͨ���ӿڻ�ȡ��
///*�ر�ע�ⷵ��ֵ�ķ�Χ*///
#define BT_TRIM_ALWAYS 1
#define BT_TRIM_ONCE   0
static u8 bt_get_macro_value(BT_STACK_MACRO type)
{
    //be careful :   MAXRO return value in the function must between 1-254;
    switch(type)
    {
        case BT_MACRO_AEC_SD_PRIO:
            return TaskBtAecSDPrio;
        case BT_MACRO_SBC_PRIO:
            return TaskSbcPrio;
        case BT_MACRO_BTLMP_PRIO:
            return TaskBtLmpPrio;
        case BT_MACRO_BTSTACK_PRIO:
            return TaskBtStackPrio;
		case BT_MACRO_BTESCO_PRIO:
			return TaskBtEscoPrio;
        case BT_MACRO_RESOURCE_PRIO:
            return TaskResoursrPrio;
        case BT_MACRO_MAX_SYS_VOL_L:
            return MAX_SYS_VOL_L;
        case BT_MACRO_MAX_SYS_VOL_R:
            return MAX_SYS_VOL_R;
        case BT_MACRO_SYSCFG_REMOTE_DB:
            return SYSCFG_REMOTE_DB;
        case BT_MACRO_SYSCFG_REMOTE_DB_HID:
            return SYSCFG_REMOTE_DB_HID;
        case BT_MACRO_MSG_LAST_WORKMOD:
            return MSG_LAST_WORKMOD;
        case BT_MACRO_SYS_EVENT_TASK_RUN_REQ:
            return SYS_EVENT_TASK_RUN_REQ;
        case BT_MACRO_MSG_BT_STACK_STATUS_CHANGE:
            return MSG_BT_STACK_STATUS_CHANGE;
        case BT_TRIM_MODE:
#if(BT_MODE == NORMAL_MODE)
            return BT_TRIM_ONCE;//BT_TRIM_ALWAYS
#else 
			return BT_TRIM_ALWAYS;
#endif
		case BT_ESCO_FILTER_LEVEL:
			return 0;
		case BT_MACRO_A2DP_AAC_EN:
			return 0;
        case BT_MACRO_STEREO_SYSCFG_REMOTE_DB:
            return SYSCFG_REMOTE_DB_STEREO;

		case BT_SBC_ENCODE_PRIO:
			return TaskSbcEncodePrio;
        default:
            printf("check the lib,0x%x\n",type);
            return 0xff;
    }
}

static void low_pwr_dac_on_control()
{
	os_taskq_post(MAIN_TASK_NAME,1,MSG_DAC_ON);
}

static void low_pwr_dac_off_control()
{
	os_taskq_post(MAIN_TASK_NAME,1,MSG_DAC_OFF);
}

u32 pwr_timer_in(void)
{
	/* putchar('I'); */
	return 1;
}

u8 led_flag = 0;
u32 pwr_timer_out(void)
{
	/* putchar('O'); */
#if KEY_IO_EN
    io_key_init();
#endif


	if(led_flag)
	{
		R_LED_ON();
		B_LED_OFF();
	}
	else
	{
		R_LED_OFF();
		B_LED_ON();
	}
	led_flag = !led_flag;
	return 1;
}

extern void get_esco_st(u8 sw);
/*
�������õ���һϵ�п����ò������ã�ͨ��handle����ʽ��
�����û��Ͳ��ÿ��ǵ������ú����ľ���λ�á���Ҫ�ı����ע�ᣬ����Ҫ������Ĭ�ϲ���
*/
static void bredr_handle_register()
{
    register_edr_init_handle();
	aec_get_macro_handle_register( bt_aec_priority);    /*must set*/
    set_bt_info_callback(bt_function_select_init);/*�൱��Ĭ�����ã�Ҫ��Э��ջ��ʼ��ǰ����*/
    get_battery_value_register(bt_get_battery_value);   /*������ʾ��ȡ�����Ľӿ�*/
    call_vol_change_handle_register(call_vol_change); /*ͨ�������е����������з����Ļص�����*/
    read_remote_name_handle_register(bt_read_remote_name);  /*���ӳɹ����ȡ�ֻ����ִӸú�������*/
    discon_complete_handle_register(bt_discon_complete_handle);/*�Ͽ����������ϻ���õĺ��������ͻ�������Ϣ*/

#if (BT_MODE != NORMAL_MODE)
	fcc_test_handle_register(fcc_uart_init,test_fcc_default_info);/*fcc���Ժ���ע��*/
#endif
#if SUPPORT_APP_RCSP_EN
	spp_data_deal_handle_register(spp_rcsp_data_deal);    /*�򵥴���*/
#else
	spp_data_deal_handle_register(spp_data_deal);    /*�򵥴���*/
#endif

	hook_phone_number_register(hook_hfp_incoming_phone_number);   /*��ȡ���͵��绰���뷵��*/
    phone_sync_sys_vol_handle_register(phone_sync_sys_dac_vol,phone_sync_phone_vol );  /*ͨ��ʱͬ��ϵͳ����*/
    //music_vol_change_handle_register(set_device_volume, get_dac_vol );  /*ͬ���ֻ���������*/
    //bt_addr_spec_deal_handle_register(bt_addr_deal);   /*���Ը��Ի����õ�ַ*/
	bt_auto_conn_handle_register(bt_is_auto_conn);
    bt_low_pwr_handle_register(low_pwr_deal,low_pwr_dac_on_control,low_pwr_dac_off_control);
    bt_updata_run_handle_register(bt_updata_run);/*������������*/
    bt_set_low_power(LOWPOWER_OSC_TYPE,LOWPOWER_OSC_HZ,BT_LOW_POWER_MODE,1,SYS_Hz/1000000L,check_enter_lowpower,NULL); /*�����͹�������*/
	bt_chip_io_type_setting(RTCVDD_TYPE , BTAVDD_TYPE);
	bt_set_noconnect_lowpower_fun(1,pwr_timer_in,pwr_timer_out);/*����û�����ӵ�ʱ���Ƿ����͹���ģʽ*/
	esco_handle_register(get_esco_st);
#if BT_STEREO	
	bt_stereo_register(bt_save_stereo_info,stereo_sys_vol_sync,stereo_deal_cmd);  /*����ӿ�*/
#else
	bt_stereo_register(NULL,NULL,NULL);  /*����ӿ�*/
#endif
}
static void ble_handle_register()
{
    /* ble_init_config(); */
    register_ble_init_handle();
}
/*�������û��ӿڣ�Э��ջ�ص�����*/
extern void bt_osc_internal_cfg(u8 sel_l,u8 sel_r);
extern void app_server_init(void);
#if BT_STEREO
static u8 rx_mem[10*1024] sec(.bt_classic_data);
static u8 tx_mem[5*1024] sec(.bt_classic_data);
#else
static u8 rx_mem[5*1024] sec(.bt_classic_data);
static u8 tx_mem[3*1024] sec(.bt_classic_data);
#endif
void bt_app_cfg()
{
    u8 poweroff = 0;
    poweroff = bt_power_is_poweroff_post();
    cfg_test_box_and_work_mode(NON_TEST,BT_MODE);
	cfg_ble_work_mode(BT_MODE);
#if (BT_MODE == TEST_BQB_MODE)
	ble_RF_test_uart_para_init(UART2_HARDWARE_NAME,57600,UART_TXPC4_RXPC5);
#endif
	cfg_bt_pll_para(OSC_Hz,SYS_Hz,BT_ANALOG_CFG,BT_XOSC);
 	cfg_bredr_mem(rx_mem,sizeof(rx_mem),tx_mem,sizeof(tx_mem));
	/*internal capacitor range:0x00~0x1F*/
	bt_osc_internal_cfg(0x11,0x11);

    bt_get_macro_handle_register(bt_get_macro_value); /*must set*/

    __set_ble_bredr_mode(BLE_BREDR_MODE);    /*bt enble BT_BLE_EN|BT_BREDR_EN */

#if BT_STEREO
    __set_stereo_mode(1);
#else
    __set_stereo_mode(0);
#endif // BT_STEREO

	set_bt_config_check_callback(bt_info_init);   //    bt_info_init();[>��ȡflash�������ļ�<]
#if (BLE_BREDR_MODE&BT_BREDR_EN)
    bredr_handle_register();
#endif   //SUPPORT BREDR
#if (BLE_BREDR_MODE&BT_BLE_EN)
    ble_handle_register();
	app_server_init();
#endif   //SUPPORT BLE
}
