#ifndef __AVCTP_USER_H__
#define __AVCTP_USER_H__


#include "typedef.h"


///***ע�⣺���ļ���ö��������������أ���Ҫ�Ǹ��û��ṩ�������á��û������Լ����м����ֵ��*/
////----user (command) codes----////
typedef enum
{
/*
ʹ��user_send_cmd_prepare(USER_CMD_TYPE cmd,u16 param_len,u8 *param)��������
//����0��֧�ֲ���������ȷ������1��֧�֣�2�ǲ�������
Ҫ����������û����˵�����������param_len��0��param��NULL
����A��USER_CTRL_HFP_CALL_SET_VOLUME������Ҫ1��������ʹ�����ӣ�
u8 vol = 8;
user_send_cmd_prepare(USER_CTRL_HFP_CALL_SET_VOLUME,1, &vol);

����B��USER_CTRL_DIAL_NUMBER ����Ҫ�������ȴ�������param_len�Ǻ��볤�ȣ�param�ɴ���������ָ�룬
user_val->income_phone_num�Ѿ���ú���
user_send_cmd_prepare(USER_CTRL_DIAL_NUMBER,user_val->phone_num_len,user_val->income_phone_num);

*/
    //��ѯ��汾��
    USER_CTRL_BT_LIB_VERSION        ,   //ֱ�ӷ���һ��U32ֵ������汾�š�����ͻ���ѯ
    USER_CTRL_LMP_LIB_VERSION       ,   //ֱ�ӷ���һ��U32ֵ������汾�š�����ͻ���ѯ
    USER_CTRL_STACK_LIB_VERSION     ,   //ֱ�ӷ���һ��U32ֵ������汾�š�����ͻ���ѯ

    //hfp��·����
    //���ƴ�绰������ע�������Щ�ֻ��������б仯������Сû�仯��ͬ��Ҫ��������DAC����
     /*���绰���������йصĲ������ն�ִ�лص�����call_vol_change*/
    USER_CTRL_HFP_CALL_VOLUME_UP    ,   /*������1���ֻ�����ͬ����ʾ*/
    USER_CTRL_HFP_CALL_VOLUME_DOWN  ,    /*������1���ֻ�����ͬ����ʾ*/
    USER_CTRL_HFP_CALL_SET_VOLUME,   /*���ù̶�ֵ���ֻ�����ͬ����ʾ����Ҫ��1������ֵ*/
    USER_CTRL_HFP_CALL_GET_VOLUME,  /*��ȡ������Ĭ�ϴ�call_vol_change����*/

    //��������绰
    USER_CTRL_HFP_CALL_ANSWER       ,
    //�Ҷϵ绰
    USER_CTRL_HFP_CALL_HANGUP       ,
    //�ز���һ������绰
    USER_CTRL_HFP_CALL_LAST_NO      ,
    //��ȡ��ǰͨ���绰����
    USER_CTRL_HFP_CALL_CURRENT      ,
     //��ȡ��ǰ�ֻ������ں�ʱ��
    USER_CTRL_HFP_DATE_TIME      ,
    //���ʹ�绰ʱ���ź�ѡ��DTMF tones ,��һ������������֧��{0-9, *, #, A, B, C, D}
    USER_CTRL_HFP_DTMF_TONES      ,
    //���ӻ�Ͽ�SCO��esco,ѡ�����������Զ��ж�Ҫ�Ͽ���������sco
    USER_CTRL_SCO_LINK              ,
    //����SCO��esco
    USER_CTRL_CONN_SCO              ,
    //�Ͽ�sco��esco
    USER_CTRL_DISCONN_SCO           ,
    //���ݵ绰���벦��
    /**USER_CTRL_DIAL_NUMBER�����в���������Ҫ�������ȴ�������
    param_len�Ǻ��볤�ȣ�param�ɴ���������ָ��*/
    USER_CTRL_DIAL_NUMBER           ,
    //���͵���  /**Ҫ������HFP������*/
    USER_CTRL_SEND_BATTERY          ,
    //*����siri״̬*//*����ע��ص�������ȡ����ֵ*/
    USER_CTRL_HFP_GET_SIRI_STATUS,
	//*����siri*/
    USER_CTRL_HFP_GET_SIRI_OPEN,
	//*�ر�siri,һ��˵�껰�����Զ��ر���,���Ҫ��ǰ��ֹ�ɵ���*/
    USER_CTRL_HFP_GET_SIRI_CLOSE,
    //ͨ�������и�����ʾ�������
    /*
    ����
    char num = '1';
    user_send_cmd_prepare(USER_CTRL_SEND_DTMF_TONE,1,(u8 *)&num);
    */
    USER_CTRL_SEND_DTMF_TONE,

    //*��̨�ֻ�ͨ�������л�*/
    USER_CTRL_HFP_CHANGE_STATE,
    //*�Ҷϵ�ǰ�ֻ�ͨ����ת��������һ̨*/
    USER_CTRL_HFP_CHANGE_WORKING,
    //����ͨ������
    //Ӧ��
     USER_CTRL_HFP_THREE_WAY_ANSWER1,     //�Ҷϵ�ǰȥ����һ����δ���������ڱ���״̬�����ԣ�
     USER_CTRL_HFP_THREE_WAY_ANSWER2,     //������ǰȥ����, ������������ͨ�����л�
     USER_CTRL_HFP_THREE_WAY_ANSWER1X,
     USER_CTRL_HFP_THREE_WAY_ANSWER2X,
    //����
    USER_CTRL_HFP_THREE_WAY_REJECT,           //�ܾ���̨����
    USER_CTRL_HFP_BIA,

    //���ֿ��Ʋ���
    //���ֲ���
    USER_CTRL_AVCTP_OPID_PLAY       ,
    //������ͣ
    USER_CTRL_AVCTP_OPID_PAUSE      ,
    //����ֹͣ
    USER_CTRL_AVCTP_OPID_STOP       ,
    //������һ��
    USER_CTRL_AVCTP_OPID_NEXT       ,
    //������һ��
    USER_CTRL_AVCTP_OPID_PREV       ,
    //���ֿ��
    USER_CTRL_AVCTP_OPID_FORWARD          ,
    //���ֿ���
    USER_CTRL_AVCTP_OPID_REWIND       ,
    //����ѭ��ģʽ
    USER_CTRL_AVCTP_OPID_REPEAT_MODE   ,
    USER_CTRL_AVCTP_OPID_SHUFFLE_MODE,
    //ͬ�������ӿ�
    USER_CTRL_AVCTP_OPID_SEND_VOL,
    //��·��������
    //����,ʹ�õ���VM�ĵ�ַ��һ�㰴��������ʹ�øýӿ�
    USER_CTRL_START_CONNECTION      ,
    //ͨ����ַȥ���ӣ����֪����ַ��ȥ����ʹ�øýӿ�
    USER_CTRL_START_CONNEC_VIA_ADDR      ,
    //ͨ��ָ����ַ�ֶ��������õ�ַ�����һ���Ͽ��豸�ĵ�ַ
    USER_CTRL_START_CONNEC_VIA_ADDR_MANUALLY      ,
    //ͨ��ָ����ַ�ֶ���������,û�ж����ַ�����¿�ʼinquiry����
    USER_CTRL_START_CONNEC_VIA_ADDR_STEREO      ,
	//
	USER_CTRL_START_CONNECT_EMITTER,
    //�Ͽ����ӣ��Ͽ���ǰ������������
    USER_CTRL_DISCONNECTION_HCI      ,
    //�Ͽ���������
    USER_CTRL_DISCONNECTIO_STEREO_HCI      ,
	//ȡ�����������
    USER_DEL_PAGE_STEREO_HCI      ,
    //��ȡԶ������
    USER_CTRL_READ_REMOTE_NAME      ,
    //���ж������ģ������������Ӹ߼���Ƶ�������ֻ���Ҳ�Զ��������ӣ�һ�㰴��������ʹ�øýӿ�
    USER_CTRL_AUTO_CONN_A2DP         ,
    //���Ӹ߼���Ƶ���������һ���Ͽ��豸�ĵ�ַ
    USER_CTRL_CONN_A2DP              ,
    //�Ͽ��߼���Ƶ��ֻ�Ͽ��߼���Ƶ��·������е绰���ᱣ��
    USER_CTRL_DISCONN_A2DP           ,
	//
	USER_CTRL_AVDTP_START			 ,
    //�Ͽ�SDP��һ�㰴��������ʹ�øýӿ�
    USER_CTRL_DISCONN_SDP_MASTER     ,
    //�����ر�
    USER_CTRL_POWER_OFF              ,
    USER_CTRL_READ_SCAN_ENABLE       ,
    //�ر������ɷ���
    USER_CTRL_WRITE_SCAN_DISABLE     ,
    //�������ɷ���
    USER_CTRL_WRITE_SCAN_ENABLE     ,
    USER_CTRL_WRITE_SCAN_ENABLE_KEY   ,
    //�ر�����������
    USER_CTRL_WRITE_CONN_DISABLE     ,
    //������������
    USER_CTRL_WRITE_CONN_ENABLE     ,
     USER_CTRL_WRITE_CONN_ENABLE_KEY     ,
    //����������������Ҫ���������豸�����ܵ�����˵������ڲ������ƹ���
    USER_CTRL_SEARCH_DEVICE               ,
    ///����sniffģʽ��һ�㰴��������ʹ�øýӿ�
    USER_CTRL_SNIFF_IN,
    USER_CTRL_SNIFF_EXIT,
    ///*hid��������*/
    //��������
    USER_CTRL_HID_CONN               ,
    //ֻ��һ����������׿�ֻ�ʹ��
    USER_CTRL_HID_ANDROID            ,
    //ֻ��һ��������ƻ���Ͳ��ְ�׿�ֻ�����
    USER_CTRL_HID_IOS                ,
    //���������հ���
    USER_CTRL_HID_BOTH               ,
    //HID�Ͽ�
    USER_CTRL_HID_DISCONNECT         ,
	//Home Key,apply to IOS and Android
	USER_CTRL_HID_HOME				 ,
	//Return Key,only support Android
	USER_CTRL_HID_RETURN			 ,
	//LeftArrow Key
	USER_CTRL_HID_LEFTARROW			 ,
	//RightArrow Key
	USER_CTRL_HID_RIGHTARROW		 ,

    //AVCTP�Ͽ��������ֿ�����·��һ�㲻ʹ��
    USER_CTRL_AVCTP_DISCONNECT         ,
    //AVCTP���ӣ������ֿ�����·��һ�㲻ʹ��
    USER_CTRL_AVCTP_CONN                   ,

    ///�������ڷ�������
    /**USER_CTRL_SPP_SEND_DATA�����в������������ȴ�������
    param_len�����ݳ��ȣ�param��������ָ��
    ����0,��ʾ׼���ɹ�����PENDing����ŷ���
    3��ʾ��һ������û���꣬*/
    USER_CTRL_SPP_SEND_DATA         ,
    USER_CTRL_SPP_UPDATA_DATA         ,
    //serial port profile disconnect command
    USER_CTRL_SPP_DISCONNECT         ,

	USER_CTRL_INQUIRY_CANCEL		,


    //������������
    //ɾ�����µ�һ���豸����
    USER_CTRL_DEL_LAST_REMOTE_INFO   ,
    //ɾ�������豸����
    USER_CTRL_DEL_ALL_REMOTE_INFO    ,
     //back to the background
    USER_CTRL_BTSTACK_SUSPEND            ,

    USER_CTRL_LAST
}USER_CMD_TYPE;


////----�������ͻ�ʹ�õ�״̬----////
typedef enum
{
     /*������һЩ��ʱ������״̬����get_bt_newest_status()��ȡ*/
    BT_STATUS_POWER_ON   = 1   ,/*�ϵ�*/
    BT_STATUS_POWER_OFF  = 2   ,
    BT_STATUS_INIT_OK          ,/*��ʼ�����*/
    BT_STATUS_FIRST_CONNECTED        ,/*���ӳɹ�*/
    BT_STATUS_SECOND_CONNECTED        ,/*���ӳɹ�*/
    BT_STATUS_FIRST_DISCONNECT       ,/*�Ͽ�����*/
    BT_STATUS_SECOND_DISCONNECT        ,/*�Ͽ�����*/
    BT_STATUS_PHONE_INCOME     ,/*����*/
    BT_STATUS_PHONE_NUMBER     ,/*���绰����*/
    BT_STATUS_PHONE_OUT        ,/*����绰*/
    BT_STATUS_PHONE_ACTIVE     ,/*��ͨ�绰*/
    BT_STATUS_PHONE_HANGUP     ,/*�Ҷϵ绰*/
    BT_STATUS_BEGIN_AUTO_CON   ,/*�������*/
    BT_STATUS_MUSIC_SOUND_COME ,/*���м���auto mute�ж����ֲ��ſ�ʼ*/
    BT_STATUS_MUSIC_SOUND_GO   ,/*���м���auto mute�ж����ֲ�����ͣ*/
    BT_STATUS_RESUME           ,/*��̨��Ч���ֶ��л�����*/
    BT_STATUS_RESUME_BTSTACK   ,/*��̨��Ч����̨ʱ�����л�����*/
    BT_STATUS_SUSPEND          ,/*���������˳�����*/
    BT_STATUS_TONE_BY_FILE_NAME ,/*ֱ��ʹ���ļ���������ʾ��*/

	BT_STATUS_INQUIRY_START		,
	BT_STATUS_INQUIRY_COMPLETE	,

   /*������1��������״̬����get_stereo_bt_connect_status��ȡ*/
    BT_STATUS_STEREO_WAITING_CONN,/*page���Ӷ����У���û������*/

   /*������6��������״̬����get_bt_connect_status()��ȡ*/
    BT_STATUS_INITING          ,/*���ڳ�ʼ��*/
    BT_STATUS_WAITINT_CONN     ,/*�ȴ�����*/
    BT_STATUS_AUTO_CONNECTINT  ,/*���ڻ���*/
    BT_STATUS_CONNECTING       ,/*�����ӣ�û�е绰�������ڻ*/
    BT_STATUS_TAKEING_PHONE    ,/*���ڵ绰*/
    BT_STATUS_PLAYING_MUSIC    ,/*��������*/
}STATUS_FOR_USER;

typedef enum
{
    BT_CALL_BATTERY_CHG = 0, //��ص����ı�
    BT_CALL_SIGNAL_CHG,      //�����źŸı�
    BT_CALL_INCOMING,   //�绰����
    BT_CALL_OUTGOING,   //�绰���
    BT_CALL_ACTIVE,     //��ͨ�绰
    BT_CALL_HANGUP,      //�绰�Ҷ�
    BT_CALL_ALERT,       //Զ��reach
	BT_CALL_VOL_CHANGED,
}BT_CALL_IND_STA;

typedef enum
{
    BT_MACRO_AEC_SD_PRIO = 1 ,  //���ȼ���
    BT_MACRO_SBC_PRIO       ,
    BT_MACRO_BTLMP_PRIO     ,
    BT_MACRO_BTSTACK_PRIO   ,
    BT_MACRO_RESOURCE_PRIO  ,
    BT_MACRO_BTESCO_PRIO    ,
    BT_MACRO_MAX_SYS_VOL_L  ,  //�����������
    BT_MACRO_MAX_SYS_VOL_R  ,
    BT_MACRO_SYSCFG_REMOTE_DB ,
    BT_MACRO_SYSCFG_REMOTE_DB_HID,
    BT_MACRO_MSG_LAST_WORKMOD ,
    BT_MACRO_SYS_EVENT_TASK_RUN_REQ,
    BT_MACRO_MSG_BT_STACK_STATUS_CHANGE ,
    BT_TRIM_MODE,
    BT_ESCO_FILTER_LEVEL,
	BT_MACRO_A2DP_AAC_EN,
	BT_MACRO_STEREO_SYSCFG_REMOTE_DB ,
	BT_SBC_ENCODE_PRIO		,
}BT_STACK_MACRO;




typedef enum
{
    BT_MUSIC_STATUS_IDLE = 0,
    BT_MUSIC_STATUS_STARTING,
    BT_MUSIC_STATUS_SUSPENDING,
}BT_MUSIC_STATE;   //����״̬


enum
{
	BT_CURRENT_CONN_PHONE=1,//��ǰֻ���ֻ�������
	BT_CURRENT_CONN_STEREO_MASTER,
	BT_CURRENT_CONN_STEREO_SALVE,
	BT_CURRENT_CONN_STEREO_MASTER_PHONE,//��ǰ���ӵ����ֻ������Ҷ�����Ѿ�������
		
};

#define BT_PROMPT_EN     //����ʱ�䰴���ļ��Ų��ļ�

#define    SPP_CH       0x01
#define    HFP_CH       0x02
#define    A2DP_CH      0x04    //media
#define    AVCTP_CH     0x08
#define    HID_CH       0x10
#define    AVDTP_CH     0x20

#define BD_ROLE_SLAVE	0x0
#define BD_ROLE_HOST	0x1

typedef void (*bt_simple_fun_handle)(void);

extern u32 user_send_cmd_prepare(USER_CMD_TYPE cmd,u16 param_len,u8 *param);

/*
u8 get_curr_channel_state();  ��  channel  �ж�����
������ȡ��ǰ��·������״̬�����������ж�����Щ��·��������
*/
extern u8 get_curr_channel_state();
/*
u8 get_last_call_type();
���ڻ�ȡǰһ��ʹ������ͨ��ʱ���绹�Ǵ��
*/
extern u8 get_last_call_type();

/*
u8 get_call_status(); ��BT_CALL_IND_STA ö�ٵ�ֵ�ж�
���ڻ�ȡ��ǰ�����绰��״̬
*/
extern u8 get_call_status();
extern void user_cmd_ctrl_init();

extern void bt_info_init(void);
extern void bt_test_info_init(void);
extern void hook_bt_trim_backup(s16 i_dc,s16 q_dc);
extern u8 hook_bt_trim_check_crc(void);

/*���Ի���������*/
extern void bt_cfg_default_init(u8 support);
/*�û��������õ�ַ��6��byte*/
extern void __set_bt_mac_addr(u8* addr);

extern void __set_use_default_value(u8 mode);
/*�û����ö���hid��name,�32���ַ�*/
extern void __set_hid_name(const char *name,u8 len);
/*�û���������name,�32���ַ�*/
extern void __set_host_name(const char *name,u8 len);
/*�û���������pin code*/
extern void __set_pin_code(char *code,u8 pin_code_max);
/*�ýӿ����������ϵ������Ҫ���������豸�ĸ�����*/
extern void __set_auto_conn_device_num(u8 num);

/*���ö���������ʶ��inquiryʱ����,��������Ӧ�ı�ʶ����������*/
void __set_stereo_device_indicate(const char *indicate);


/*//�����ĳ�ʱ���á�ms��λ�����Ƕ��ֻ������������û���õ�*/
extern void __set_super_timeout_value(u32 time );
/*�ӿ������ǲ��Ժ�ģʽ��������SDK*/
extern void __set_test_masetr_enable(u8 en);
/*�ṩ�ӿ������Ƿ��Զ��Ͽ��ֻ���HID����*/
extern int __set_hid_auto_disconn_en(u8 flag);
/*�ⲿ����֧��ʲôЭ��*/
extern void bt_cfg_default_init(u8 support);
/*hid��Ϊ����ģʽʱҪ����*/
extern void set_hid_independent_info();

/*�ṩ��̬���������Զ����ŵĽӿڡ�
  �ýӿ�ʹ�õ�ʱ�����Ҫ��Ҫ��A2DP����֮ǰ����
   */
extern void __bt_set_a2dp_auto_play_flag(u8 flag);

/*�ṩ�����ӿ������Ƿ���Ҫ��̨���ַ���*/
extern void __bt_set_music_back_play_flag(u8 flag);

/*�ṩ�ӿ��ⲿ������Է�ʽ*/
extern void __set_simple_pair_flag(u8 flag);

extern bool get_remote_test_flag();
/*��ȡ��ǰ�Ƿ��Ƕ�����HIDģʽ*/
extern u8 __get_hid_independent_flag();
/*//����page�ĳ�ʱ���á�ms��λ*/
extern void __set_page_timeout_value(u16 time);
extern void __set_soundtrack_compound(u8 enable);
extern void __set_connect_stereo_timeout_value(u16 time);
/*//���ÿ��������ɷ��ֿ����ӵ�ʱ�䣬Ϊ��ʡ�磬����һ���Զ��رտ����ӵ�ʱ�䡣ms��λ��
    Ϊ 0 ʱ��ʹ���Զ��ر�*/
extern void __set_connect_scan_timeout_value(u32 time );
/*���õ�����ʾ���͸��µ�����ʱ�䣬Ϊ0��ʾ�رյ�����ʾ����*/
void __bt_set_update_battery_time(u32 time);
/*���û���������֧�����ӵĸ�������Ҫ���ڿ��ƿ��ƿɷ��ֿ����Ӻͻ�������*/
extern void __set_user_ctrl_conn_num(u8 num);
/*�����˼����豸*/
extern u8 get_total_connect_dev(void);
/*�Ƿ�֧�ֵ�����ʾ����*/
extern void __bt_set_display_battery_enable(u8 flag);
/*�ṩ�ӿ��ⲿ����Ҫ����hfp��Ҫ����ͨ��*/
extern void __set_disable_sco_flag(u8 flag);
/*��ѯ�Ƿ��������ֻ���ͨ����Ƶ��·*/
extern bool get_sco_connect_status();
/*�߼���Ƶ���Ͽ���*/
extern void __set_music_break_in_flag(u8 flag);
/*ͨ�����Ͽ��أ�0�������ϣ�1������*/
extern void __set_hfp_switch(u8 switch_en);
/*�����ڲ�����0-16*/
extern void __set_xosc_value(u8 value);
extern void __set_esco_packet_type(u8 type);  /*esco packet  */
/*��������������ʱ�䡣ms��λ������inquiry������*/
extern void __set_searche_timeout_value(u16 time);
extern void __set_sbc_cap_bitpool(u16 sbc_cap_bitpoola);

/*������ͬ�������ֲ��������ʱ��*/
extern void __set_indicate_name_cmp_en(u8 stereo_indicate_name_en);

/*���ÿ����Ƿ��������*/
extern void __set_start_connet_stereo(u8 start_connect_stereo);
/*��Щ��ѡ�ӿ�����ʵ�ָ��Ի��������̣��ص�����ע�ᣬ�ǵó�������Ӵ*/
extern void get_battery_value_register(int (*handle)(void));    /*��������ʱ��ȡ�����ȼ��Ľӿ�ע��*/
extern void linkkey_missing_handle_register(void (*handle)());     /*���������ֻ�ɾ���������Ϣ��ע��ӿ�*/
extern void call_vol_change_handle_register(void (*handle)(int vol));   /*�ֻ�ͨ��ʱ���������Ļص������ӿ�ע�ắ��*/
extern void music_vol_change_handle_register(void (*handle)(int vol),int (*handle2)(void));  /*�ֻ�����������ģʽ������ͬ��*/
extern void phone_sync_sys_vol_handle_register(void (*handle)(u8 vol,u8 phone_vol),u8 (*phone_vol_hdl)(u8 phone_vol_max));
extern void read_remote_name_handle_register(void (*handle)(u8 *addr));   /*��ȡ�����ֺ�Ļص������ӿ�ע�ắ��*/
extern void hook_phone_number_register(void (*handle)(char* number, u16 length));  /*��ȡ���绰����Ļص�����*/
extern void spp_data_deal_handle_register(void (*handler)(u8 packet_type, u16 channel, u8 *packet, u16 size)); /*֧�ִ��ڹ��ܵ����ݴ���ӿ�*/
extern void discon_complete_handle_register(void (*handle)(u8 *addr , int reason));/*�Ͽ����������ϻ���õĺ��������ͻ�������Ϣ*/
extern void bt_updata_run_handle_register(void (*handle)());

extern void bt_auto_conn_handle_register(void  (*handle)(u8));
extern void bt_low_pwr_handle_register(void (*handle)(u8 mode,u32 timer_ms),void (*dac_on_deal)(),void (*dac_off_deal)());
extern void bt_stereo_register(void (*bt_save_stereo_info_handle)(u8),void (*stereo_sys_vol_sync_handle)(void),void (*stereo_deal_cmd_handle)(u8 msg,u8 value));
/*����ͻ�һ���ò�����*/
extern u8 get_bt_newest_status(void);
extern void update_bt_current_status(u8 *addr, u8 new_status,u8 conn_status);
extern void hook_user_deal_hfp_status(u8 *addr, u32 sta, int param);
extern u8 get_bt_connect_status(void);
extern u8 get_bt_current_conn_type();
extern u8 get_stereo_bt_connect_status(void);
extern u8 get_bt_prev_status(void);
extern void update_bt_prev_status(u8 status);
extern void updata_profile_channels_status(u8 *addr , int state, int channel);
extern u8 __bt_get_macro_value(BT_STACK_MACRO type);
extern void hook_bt_trim_backup_register(void (*handle)(s16 i_dc,s16 q_dc));
extern void bt_discon_complete_handle(u8 *addr , int reason);
extern void hook_hfp_incoming_phone_number(char* number, u16 length);
extern void cfg_bt_power_osc_init(u8 osc, u32 osc_hz,u8 rtc_2pin_32k,u8 set_PR1);
extern void cfg_test_box_and_work_mode(u8 test_box,u8 work_mode);
extern void cfg_bt_pll_para(u32 osc,u32 sys,u8 low_power,u8 xosc);
extern void cfg_bredr_mem(u8* rx,u32 rx_len,u8 * tx, u32 tx_len);
extern void __bt_set_sniff(u8 sniff_mode_config,u8 sniff_cnt_time);
extern u8 check_is_poweroff_up();
extern tbool is_1t2_connection(void);
extern bool check_in_sniff_mode();
extern u8 get_current_search_index();
extern u8 app_get_is_poweroff_up();
extern bool is_connect_timeout_open();
extern void set_change_workmode_flag();
extern void set_bt_info_callback(void (*fun)(void));
extern void user_prote_bt_process(u8 mode);
extern void __set_connect_scan_with_key(u8 flag);
extern void __set_ble_bredr_mode(u8 flag);
extern void register_edr_init_handle(void);
extern void register_ble_init_handle(void);
extern void bt_get_macro_handle_register(u8 (*handle)(BT_STACK_MACRO type));
extern void fcc_test_handle_register(void (*handle)(void), void (*handle2)());
extern u8 bt_power_is_poweroff_post(void);
extern bool get_suspend_flag(void);
extern u8 is_bt_conn_hfp_hangup(u8 *addr);
extern tbool is_sniff_mode();
extern void bt_set_low_power(u8 type,u32 osc_hz,u8 config,u8 is_use_PR,u8 delay_us,u32 (*lowpower_fun)(void),void (*fun)(void));
extern void bt_set_noconnect_lowpower_fun(u8 pwr_timer_en,u32 (*in)(void),u32 (*out)(void));
extern void bt_power_max(u8 max_power);
extern void bt_chip_io_type_setting(u8 rtcvdd, u8 btavdd);
extern void esco_handle_register(void (*handle)(u8 sw));
extern bool check_bt_app_updata_run();
extern bool get_resume_flag();
extern void stereo_host_cmd_handle(int cmd,int value);
extern void stereo_slave_cmd_handle(int cmd,int value);
extern void clear_a2dP_sink_addr(u8 *addr);
extern void ble_RF_test_uart_para_init(char *uart_name,u32 baud,u8 io);
extern void cfg_ble_work_mode(u8 work_mode);
extern void stereo_clear_current_search_index();
extern void set_stereo_role_switch_fail(u8 err_code);
extern u8 get_stereo_role_switch_fail();
extern bool is_check_stereo_slave();
extern bool conn_is_have_stereo();
extern void __set_stereo_mode(u8 mode);
extern void __set_emitter_role(u8 role);
extern void __set_bd_addr_filt(u8 filt_num);
extern void __set_inquiry_again_flag(u8 flag);
extern void __set_emitter_auto_conn(u8 en);
extern void after_auto_connection_stereo(u8 set_auto_con,u8 enble);
#define BD_CLASS_WEARABLE_HEADSET	0x240404/*ios10.2 display headset icon*/
#define BD_CLASS_HANDS_FREE			0x240408/*ios10.2 display bluetooth icon*/
#define BD_CLASS_MICROPHONE			0x240410
#define BD_CLASS_LOUDSPEAKER		0x240414
#define BD_CLASS_HEADPHONES			0x240418
#define BD_CLASS_CAR_AUDIO			0x240420
#define BD_CLASS_HIFI_AUDIO			0x240428
extern void __change_hci_class_type(u32 class);
extern void calling_del_page();
#endif
