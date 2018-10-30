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

#include "ui/lcd/lcd_ui_menu.h"
#include "ui/lcd/lcd_drv_api.h"
#include "ui/lcd/lcd_drv_interface.h"

#if  LCD_SUPPORT_MENU//LCD_128X64_EN

#define LCD_MENU_OPT_DBG
#ifdef  LCD_MENU_OPT_DBG

    #define lcd_menu_puts     puts
    #define lcd_menu_printf   printf

#else
    #define lcd_menu_puts(...)
    #define lcd_menu_printf(...)

#endif

#define RDEF_MAIN_TASK_NAME        "MainTask"
#define RDEF_RTC_TASK_NAME         "RTCTask"
#define RDEF_MUSIC_TASK_NAME       "MusicTask"
#define RDEF_RECORD_TASK_NAME      "RECTask"
#define RDEF_RADIO_TASK_NAME       "FMRadioTask"
#define RDEF_LINEIN_TASK_NAME      "LineinTask"
#define RDEF_UDISK_TASK_NAME       "USBdevTask"
#define RDEF_BTMSG_TASK_NAME       "btmsg"
#define RDEF_BTSTACK_TASK_NAME     "BtStackTask"
#define RDEF_IDLE_TASK_NAME        "IdleTask"
#define RDEF_LIGHT_TASK_NAME       "LightTask"



void start_radio_menu(u32 arg);
void start_main_menu(u32 arg);
void start_music_menu(u32 arg);
void start_bt_menu(u32 arg);
void start_linein_menu(u32 arg);
void start_rtc_menu(u32 arg);
void start_usb_menu(u32 arg);
void start_record_menu(u32 arg);
void start_setting_menu(u32 arg);
void start_btcall_menu(u32 arg);

int start_browser_submenu(int active);
int start_rec_ctl_submenu(int active);
int start_echo_ctl_submenu(int active);

int music_func_cbk_playmode_disp(int active);
int music_func_cbk_playmode_set(int active);
int music_func_cbk_eq_disp(int active);
int music_func_cbk_eq_set(int active);

int btmusic_func_cbk_eq_disp(int active);
int btmusic_func_cbk_eq_set(int active);

int rec_ctl_func_cbk_deal_m0(int active);
int rec_ctl_func_cbk_deal_m1(int active);

int echo_ctl_func_cbk_deal_m0(int active);

int main_func_cbk_deal(int active);
int music_func_cbk_deal(int active);
int radio_func_cbk_deal(int active);
int record_func_cbk_deal(int active);
int linein_func_cbk_deal(int active);
int rtc_func_cbk_deal(int active);
int usb_func_cbk_deal(int active);

int btcall_func_cbk_deal(int active);
int btcall_no_sco_func_cbk_deal(int active);
int btmusic_func_cbk_deal(int active);

int browser_func_cbk_deal(int active);
static int func_menuwait_sub_deal(u16 back_item_active);
static void func_slider_display(u8 slider_id,u16 active,u16 total);

int send_msg_to_curtask(int msg0,int msg1);

static const char menu_app_task_namestr[MENU_TASK_APP_MAX][16] =
{
    {   RDEF_MAIN_TASK_NAME},
    {   RDEF_RTC_TASK_NAME},
    {   RDEF_MUSIC_TASK_NAME},
    {   RDEF_RECORD_TASK_NAME},
    {   RDEF_RADIO_TASK_NAME},
    {   RDEF_LINEIN_TASK_NAME},
    {   RDEF_UDISK_TASK_NAME},
    //{RDEF_BTSTACK_TASK_NAME},
    {   RDEF_BTMSG_TASK_NAME},
};

item_cbk_f menu_item_cbk;
//menu_list_t menu_list_table[MENU_LAYER_MAX];
menu_list_t *menu_list_table;

u8 menu_list_layer;
u8 menu_task_app;
u8 menu_need_draw;

MUSIC_OP_API *menu_browser_mapi;
u8 *disp_items_buf;
FS_DIR_INFO *bs_items_buf;

#if 0
const char test_string[] =
{   "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"};

const char test_unicode1[] =
{
    0x4B,0x6D,0xD5,0x8B,0x55,0x00,0x4E,0x00,0x49,0x00,
    0x43,0x00,0x4F,0x00,0x44,0x00,0x45,0x00,0x3E,0x66,
    0x3A,0x79,0x0C,0xFF,0xDA,0x6E,0x4F,0x5C,0x3E,0x66,
    0x3A,0x79,0x00,0x5F,0xCB,0x59,0x00,0x00
};

const char test_unicode[] = {0x60,0x4F,0x7D,0x59,0x17,0x54,0x00,0x00};
#endif


/******** menu item list define ********/
//�˵����ͼ��λ��
static const GUI_POINT item_icon_point[MENU_BROWSER_SCREEN] =
{
    {   0,0},
    {   0,16},
    {   0,32},
    {   0,48},
};

//ѡ�в˵���ͻ������
static const _lcd_area_size_t item_set_backgroup[MENU_BROWSER_SCREEN] =
{
    //left,top,right,bottom

    {   16,0,119,1},
    {   16,2,119,3},
    {   16,4,119,5},
    {   16,6,119,7},
};

//�˵���ʾ�ռ�����
static const u8 item_DVcTxt_group[MENU_BROWSER_SCREEN] =
{   DVcTxt1_9,DVcTxt2_9,DVcTxt3_9,DVcTxt4_9};

//�������飬�����̣��߶�
static const u8 item_v_slider_group[3] =
{   DVcRzImg3_9,DVcRzImg4_9,DVcRzImg5_9};

//slider_block �ĸ���
static const u8 item_v_slider_block_cnt[2] =
{   64/8,48/8};

//slider ��ʾ��ʼλ��
static const GUI_POINT item_v_slider_block_point[2] =
{
    {   120,0},
    {   120,16},
};
/******** menu key msg define ********/

//�˵���ڻص�����
static const app_menulist_f app_menulist_func_table[MENU_TASK_APP_MAX] =
{
    start_main_menu,
    start_rtc_menu,
    start_music_menu,
    start_record_menu,
    start_radio_menu,
    start_linein_menu,
    start_usb_menu,
    start_bt_menu,
};

//��������Ӧ����Ϣ��
const u8 menu_adkey_msg_table[10] =
{
    UI_MSG_NULL, //0
    UI_MSG_NULL, //1
    UI_MSG_NULL, //2
    UI_MSG_MENULIST_KEYPREV,//3
    UI_MSG_MENULIST_KEYNEXT,//4
    UI_MSG_NULL, //5
    UI_MSG_NULL, //6
    UI_MSG_MENULIST_KEYEXIT,//7
    UI_MSG_MENULIST_KEYBACK,//8
    UI_MSG_MENULIST_KEYMODE,//9

};

//��������Ӧ����Ϣ��
const u8 menu_irkey_msg_table[10] =
{
    UI_MSG_NULL, //0
    UI_MSG_NULL, //1
    UI_MSG_NULL, //2
    UI_MSG_MENULIST_KEYMODE,//3
    UI_MSG_MENULIST_KEYPREV,//4
    UI_MSG_MENULIST_KEYNEXT,//5
    UI_MSG_MENULIST_KEYEXIT,//6
    UI_MSG_NULL, //7
    UI_MSG_NULL,//8
    UI_MSG_NULL,//9

};



/************************************************
 *    main menu display  deal  *
 *************************************************/

//ͼƬ��ʾ����
static const GUI_POINT main_icon_point[MENU_MAIN_SCREEN] =
{
    {   0+4,18+8},
    {   32+4,18+8},
    {   64+4,18+8},
    {   96+4,18+8},
};

//ͼƬ�б�λѡ�к�ѡ��
static const u8 main_app_img_group[][2] =
{
    {   DVcRzImg3_10,DVcRzImg11_10},//bt

    {   DVcRzImg4_10,DVcRzImg12_10},//music

#if FM_RADIO_EN
    {   DVcRzImg5_10,DVcRzImg13_10},//fm
#endif

    {   DVcRzImg6_10,DVcRzImg14_10},//line in

#if USB_PC_EN
    {   DVcRzImg7_10,DVcRzImg15_10},//pc
#endif

#if RTC_CLK_EN
    {   DVcRzImg8_10,DVcRzImg16_10},//rtc
#endif

#if REC_EN
    {   DVcRzImg9_10,DVcRzImg17_10},//record
#endif

//    {   DVcRzImg10_10,DVcRzImg18_10},//sys_set
};

//�����ַ���
static const char main_app_name_group[][16] =
{
    {   "   ����"}, //0

    {   "   ����"}, //1
#if FM_RADIO_EN
    {   "   ����"}, //2
#endif

    {   "  ��Ƶ����"}, //3

#if USB_PC_EN
    {   "  ��������"}, //4
#endif

#if RTC_CLK_EN
    {   "   ʱ��"}, //5
#endif

#if REC_EN
    {   "   ¼��"}, //6
#endif

//    {   "  ϵͳ����"}, //7
};

//ѡ�лص�����
static const item_cbk_f main_func_cbk_layer0[1] =
{
    (void*)main_func_cbk_deal
};

//ͼ���Ӧ��APPid
static const u8 main_menu_id_group[] =
{
    MENU_TASK_BTSTACK_APP, //0

    MENU_TASK_MUSIC_APP, //1

#if FM_RADIO_EN
    MENU_TASK_RADIO_APP, //2
#endif

    MENU_TASK_LINEIN_APP, //3

#if USB_PC_EN
    MENU_TASK_UDISK_APP, //4
#endif

#if RTC_CLK_EN
    MENU_TASK_RTC_APP, //5
#endif

#if REC_EN
    MENU_TASK_RECORD_APP, //6
#endif

    0xff, //7
};

/************************************************
 *    browser menu display  deal  *
 *************************************************/
//ѡ�лص�����
static const item_cbk_f bs_func_cbk_gp_layer0[1] =
{
    (void*)browser_func_cbk_deal
};

/************************************************
 *    record menu display  deal  *
 *************************************************/
//ģʽ�ַ�����,��Ҫ���ı�˳���ɾ���ַ����������ַ�����ĩβ׷��
static const char echo_ctl_menu_string[][MENU_ITEM_STRLEN] =
{
    {   "  �������"}, //0

    {   "����"},       //1

    {   "�򿪻���"},   //2

    {   "�رջ���"},   //3
};

//�򿪻���
static const u8 echo_ctl_menu_strid_layer0_m0[] =
{   2,1};
//�ص�����
static const item_cbk_f echo_ctl_func_cbk_gp_layer0_m0[1] =
{   (void*)echo_ctl_func_cbk_deal_m0};

//�رջ���
static const u8 echo_ctl_menu_strid_layer0_m1[] =
{   3,1};
//�ص�����
static const item_cbk_f echo_ctl_func_cbk_gp_layer0_m1[1] =
{   (void*)echo_ctl_func_cbk_deal_m0};

static const u8 echo_ctl_menu_strid_layer0_nosupport[] =
{   1};

/************************************************
 *    record menu display  deal  *
 *************************************************/
//ģʽ�ַ�����,��Ҫ���ı�˳���ɾ���ַ����������ַ�����ĩβ׷��
static const char rec_ctl_menu_string[][MENU_ITEM_STRLEN] =
{
    {   "  ¼������"}, //0

    {   "����"}, //1

    {   "��ʼ¼��"}, //2

    {   "��ͣ¼��"}, //3

    {   "����¼��"}, //4

    {   "����¼��"}, //5
};

//Ĭ�����Ĳ˵�Ϊ���ز���
//ֹͣ¼���˵�
static const u8 rec_ctl_menu_strid_layer0_m0[] =
{   2,1};
//�ص�����
static const item_cbk_f rec_ctl_func_cbk_gp_layer0_m0[1] =
{   (void*)rec_ctl_func_cbk_deal_m0};

//��ʼ¼����˵�
static const u8 rec_ctl_menu_strid_layer0_m1[] =
{   3,5,1};
//�ص�����
static const item_cbk_f rec_ctl_func_cbk_gp_layer0_m1[1] =
{   (void*)rec_ctl_func_cbk_deal_m1};

//��ͣ¼����˵�
static const u8 rec_ctl_menu_strid_layer0_m2[] =
{   4,5,1};
//�ص�����
static const item_cbk_f rec_ctl_func_cbk_gp_layer0_m2[1] =
{   (void*)rec_ctl_func_cbk_deal_m1};

//�洢�豸�����ڲ˵�
static const u8 rec_ctl_menu_strid_layer0_m3[] =
{   1};
//�ص�����
//rec_ctl_func_cbk_gp_layer0_m0


/************************************************
 *    radio menu display  deal  *
 *************************************************/
//ģʽ�ַ�����,��Ҫ���ı�˳���ɾ���ַ����������ַ�����ĩβ׷��
static const char radio_menu_string[][MENU_ITEM_STRLEN] =
{
    {   "  ����ģʽ"}, //0

    {   "����"}, //1

    {   "ǫ̈̄����"}, //2

    {   "¼������"}, //3

    {   "�������"}, //4

};

//Ĭ�����Ĳ˵�Ϊ���ز���
//ֹͣ¼���˵�
static const u8 radio_menu_strid_layer0[] =
{   2,3,4,1};
//�ص�����,1��
static const item_cbk_f radio_func_cbk_gp_layer0[1] =
{   (void*)radio_func_cbk_deal};

/************************************************
 *    music menu display  deal  *
 *************************************************/
//ģʽ�ַ�����,��Ҫ���ı�˳���ɾ���ַ����������ַ�����ĩβ׷��
static const char music_menu_string[][MENU_ITEM_STRLEN] =
{
    {   "  ����ģʽ"}, //0

    {   "����"}, //1

    {   "ѭ��ģʽ"}, //2

    {   "EQģʽ"}, //3

    {   "����Ŀ¼"}, //4

    {   "ȫ��ѭ��"}, //5

    {   "Ŀ¼ѭ��"}, //6

    {   "����ѭ��"}, //7

    {   "�������"}, //8

    {   "��Ȼ"}, //9

    {   "ҡ��"}, //10

    {   "����"}, //11

    {   "�ŵ�"}, //12

    {   "��ʿ"}, //13

    {   "���"}, //14

    {   "ɾ���ļ�"}, //15

    {   "����U��"}, //16

    {   "���ſ���"}, //17

    {   "�������"}, //18

    {   "���豸ѭ��"}, //19
};

//���˵�
static const u8 music_menu_strid_layer0[] =
{   2,3,18,4,15,1};
static const u8 music_menu_strid_layer0_m1[] =
{   2,3,18,4,15,16,1};
static const u8 music_menu_strid_layer0_m2[] =
{   2,3,18,4,15,17,1};
//�ص�����
static const item_cbk_f music_func_cbk_gp_layer0[] =
{
    (void*)music_func_cbk_playmode_disp,
    (void*)music_func_cbk_eq_disp,
    (void*)start_echo_ctl_submenu,
    (void*)start_browser_submenu,
    (void*)music_func_cbk_deal,
    (void*)music_func_cbk_deal,
};

//PLAYMODE�˵�
static const u8 music_menu_strid_playmode[] =
{   5,19,7,8,6,1};
/* {   5,6,7,8,1}; */
//�ص�����
static const item_cbk_f music_func_cbk_gp_playmode[1] =
{   (void*)music_func_cbk_playmode_set};

//EQ�˵�
static const u8 music_menu_strid_eq[] =
{   9,10,11,12,13,14,1};
//�ص�����
static const item_cbk_f music_func_cbk_gp_eq[1] =
{   (void*)music_func_cbk_eq_set};

/************************************************
 *    bt menu display  deal  *
 *************************************************/
//ģʽ�ַ�����,��Ҫ���ı�˳���ɾ���ַ����������ַ�����ĩβ׷��
static const char bt_menu_string[][MENU_ITEM_STRLEN] =
{
    {   "  ����ģʽ"}, //0

    {   "����"}, //1

    {   "¼������"}, //2

    {   "EQģʽ"}, //3

    {   "��Ȼ"}, //4

    {   "ҡ��"}, //5

    {   "����"}, //6

    {   "�ŵ�"}, //7

    {   "��ʿ"}, //8

    {   "���"}, //9

    {   "�������"}, //10

};

//btmusic�˵�
static const u8 btmusic_menu_strid_layer0[] =
{   3,10,1};
//�ص�����
static const item_cbk_f btmusic_func_cbk_gp_layer0[1] =
{   (void*)btmusic_func_cbk_deal};

//EQ�˵�
static const u8 btmusic_menu_strid_eq[] =
{   4,5,6,7,8,9,1};
//�ص�����
static const item_cbk_f btmusic_func_cbk_gp_eq[1] =
{   (void*)btmusic_func_cbk_eq_set};

//btcall �˵�
static const u8 btcall_menu_strid_layer0[] =
{   2,1};

//btcall �˵�
static const u8 btcall_menu_no_sco_strid_layer0[] =
{   1};

//�ص�����
static const item_cbk_f btcall_func_cbk_gp_layer0[1] =
{   (void*)btcall_func_cbk_deal};

static const item_cbk_f btcall_no_sco_func_cbk_gp_layer0[1] =
{   (void*)btcall_no_sco_func_cbk_deal};

/************************************************
 *    linein menu display  deal  *
 *************************************************/
//ģʽ�ַ�����,��Ҫ���ı�˳���ɾ���ַ����������ַ�����ĩβ׷��
static const char linein_menu_string[][MENU_ITEM_STRLEN] =
{
    {   "  ��Ƶģʽ"}, //0

    {   "����"}, //1

    {   "¼������"}, //2

    {   "�������"}, //3

};

//ֹͣ¼���˵�
static const u8 linein_menu_strid_layer0[] =
{   2,3,1};
//�ص�����
static const item_cbk_f linein_func_cbk_gp_layer0[1] =
{   (void*)linein_func_cbk_deal};

/************************************************
 *    usb menu display  deal  *
 *************************************************/

/************************************************
 *    rtc menu display  deal  *
 *************************************************/
//ģʽ�ַ�����,��Ҫ���ı�˳���ɾ���ַ����������ַ�����ĩβ׷��
static const char rtc_menu_string[][MENU_ITEM_STRLEN] =
{
    {   "  ʱ��ģʽ"}, //0

    {   "����"}, //1

    {   "ʱ������"}, //2

    {   "��������"}, //2

};

//ֹͣ¼���˵�
static const u8 rtc_menu_strid_layer0[] =
{   2,3,1};
//�ص�����
static const item_cbk_f rtc_func_cbk_gp_layer0[1] =
{   (void*)rtc_func_cbk_deal};

/************************************************
 *    record menu display  deal  *
 *************************************************/
//ģʽ�ַ�����,��Ҫ���ı�˳���ɾ���ַ����������ַ�����ĩβ׷��
static const char record_menu_string[][MENU_ITEM_STRLEN] =
{
    {   "  ¼��ģʽ"}, //0

    {   "����"}, //1

    {   "¼������"}, //2

};

//ֹͣ¼���˵�
static const u8 record_menu_strid_layer0[] =
{   2,1};
//�ص�����
static const item_cbk_f record_func_cbk_gp_layer0[1] =
{   (void*)record_func_cbk_deal};

/************************************************
 *    systerm menu display  deal  *
 *************************************************/

/*
 *********************************************************************************************************
 *
 * Description: ȷ����ǰ���е�����ID
 *
 * Arguments  :
 *
 * Returns    : 0-Ϊ�ɹ�������ֵ ʧ��
 *
 * Note:
 *********************************************************************************************************
 */

static int check_curtask_appid(void)
{
    u8 i;
    menu_task_app = 0;
    for( i = 0; i< MENU_TASK_APP_MAX;i++)
    {
        if(strcmp(menu_app_task_namestr[i],curr_task->name) == 0)
        {
            break;
        }
        menu_task_app++;
    }

    if(i == MENU_TASK_APP_MAX)
    {
        if(strcmp(RDEF_BTSTACK_TASK_NAME,curr_task->name) == 0)
        {
            menu_task_app = MENU_TASK_BTSTACK_APP;
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

/*
 *********************************************************************************************************
 *
 * Description: ������Ϣ�����߳�
 *
 * Arguments  :
 *
 * Returns    : 0-Ϊ�ɹ�������ֵ ʧ��
 *
 * Note:
 *********************************************************************************************************
 */
int send_msg_to_curtask(int msg0,int msg1)
{
    os_taskq_post((void*)&menu_app_task_namestr[menu_task_app], 2, msg0,msg1);
    os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
    return 0;
}

/*
 *********************************************************************************************************
 *
 * Description: �����б���ʾ��Ϣ
 *
 * Arguments  : buf--�������,disp_cnt--��ȡ��Ϣ������,,disp_group--��ǰ��ʾ��ҳ�ţ�1~��
 *              ,back_item_id--���ز˵�id
 *
 * Returns    : ��Ч��Ϣ����
 *
 * Note:
 *********************************************************************************************************
 */
u16 func_browser_reflash(FS_DIR_INFO *buf,u16 disp_cnt,u16 disp_group,u16 back_item_id)
{
    u16 real_cnt,disp_sn;
    FS_DIR_INFO *src_addr = buf;
    LONG_FILE_NAME * l_name_pt;
    real_cnt = 0;

    disp_sn = (disp_group * disp_cnt) + 1; //ȡ�ļ���Ϣ��1��ʼ

    memset(buf,0x00,sizeof(FS_DIR_INFO) * disp_cnt);

    if(ITEM_BACK_FIRST_FIRST_ID == back_item_id)
    {
        //�з��ز˵�
        if(0 == disp_group)
        {
            //��һҳ�棬��ȡһ����Ϣ
            strcpy(buf->lfn_buf.lfn,"..");
            buf->fn_type = BS_FNAME_TYPE_SHORT;
            buf->dir_type = BS_DIR_TYPE_FORLDER;
            disp_cnt--;
            buf++;
            disp_group = 0xffff;
        }
        else
        {
            //����ҳ����ż�һ
            disp_sn--;
        }
    }

    if(disp_cnt > 0)
    {
        real_cnt = (u16)file_bs_get_dir_info(menu_browser_mapi,buf,disp_sn,disp_cnt);//pos��1��ʼ
        lcd_menu_printf("get  parm:%d,%d;result = %d\n",disp_sn,disp_cnt,real_cnt);
        if(real_cnt > disp_cnt)
        {
            puts("*fs err\n");
            real_cnt = disp_cnt;
        }
    }

#if 1
    for(disp_sn = 0;disp_sn < real_cnt;disp_sn++)
    {
        l_name_pt = &buf[disp_sn].lfn_buf;

//        printf("dir%d:dir_type =%d, name_type =%d\n",disp_sn,buf[disp_sn].dir_type,buf[disp_sn].fn_type);
//        printf_buf(buf[disp_sn].lfn_buf.lfn,8);

        if(buf[disp_sn].fn_type == BS_FNAME_TYPE_SHORT)
        {
            lcd_menu_printf("bs(%d)is short:\n",disp_sn);
            file_comm_display_83name((void*)&l_name_pt->lfn[32],(void*)l_name_pt->lfn);
            strcpy(l_name_pt->lfn,&l_name_pt->lfn[32]);
        }
        else
        {
            lcd_menu_printf("bs(%d)is long:len = %d\n",disp_sn,l_name_pt->lfn_cnt);
            if(l_name_pt->lfn_cnt > 510)
            {
               l_name_pt->lfn_cnt = 510;
               puts("***get long name err!!!");
            }
//            printf_buf(l_name_pt->lfn,32);
            l_name_pt->lfn_cnt = file_comm_long_name_fix((void*)l_name_pt->lfn,l_name_pt->lfn_cnt);
            lcd_menu_printf("disp_len = %d\n",l_name_pt->lfn_cnt);
//            printf_buf(l_name_pt->lfn,l_name_pt->lfn_cnt);
            l_name_pt->lfn[l_name_pt->lfn_cnt] = 0;
            l_name_pt->lfn[l_name_pt->lfn_cnt+1] = 0;
        }

//        printf_buf(l_name_pt->lfn,32);
    }
#endif

    if(disp_group == 0xffff)
    {
        real_cnt++;
    }

    return real_cnt;
}
/*
 *********************************************************************************************************
 *
 * Description: ��������λ��ʾ
 *
 * Arguments  : slider_id - ������id,active - �˵�ѡ����,total-�˵�����
 *
 * Returns    :
 *
 * Note:
 *********************************************************************************************************
 */
static void func_slider_display(u8 slider_id,u16 active,u16 total)
{
    GUI_POINT block_pos;
    u8 block_cnt = item_v_slider_block_cnt[slider_id];
    u16 y_offset,n;

    memcpy(&block_pos,&item_v_slider_block_point[slider_id],sizeof(GUI_POINT));
    ui_pic(item_v_slider_group[slider_id],&block_pos);

    if(active == 0)
    {
        y_offset = 0;
    }
    else if(active == (total -1))
    {
        y_offset = (block_cnt - 1);
    }
    else
    {
        n = ((active*100)/total) * block_cnt;
        //n = ((active*100)*block_cnt)/total;
        y_offset = n/100;
        if((n%100)> 0)
        {
            y_offset++;
        }

        if(y_offset == 0)
        {
            y_offset++;
        }
        else if(y_offset >= (block_cnt - 1))
        {
            y_offset = (block_cnt - 2);
        }
        else
        {
            ;
        }
    }

    //    lcd_menu_printf("slider:%d,%d;%d,%d\n",active,total,y_offset,block_cnt);

    block_pos.y +=(y_offset<<3);//*8
    block_pos.x++;
    //    lcd_menu_printf("pos:%d,%d",block_pos.x,block_pos.y);
    ui_pic(item_v_slider_group[2],&block_pos);
}

/*
 *********************************************************************************************************
 *
 * Description: �ȴ����հ�������˵�����
 *
 * Arguments  : back_item_active-�˳��Ĳ˵�ѡ��id��0XFFFF --û�з��ز˵�
 *
 * Returns    : �������
 *
 * Note:�����뺯��
 *********************************************************************************************************
 */
int func_menuwait_sub_deal(u16 back_item_active)
{
    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer];
    u16 time_out_cnt = 0;
    int msg[3];
    int ret = MENU_OPT_BACK;

    wait_msg_start:

    //    lcd_menu_puts("mlist wait\n");

    memset(msg,0x00,sizeof(msg));
    ret = os_taskq_pend(50, ARRAY_SIZE(msg), msg);//500ms_reflash

    if(ret == OS_TIMEOUT)
    {
        //        lcd_menu_puts("mlist TO\n");
        time_out_cnt++;
        if(time_out_cnt >= MENU_WAIT_TIME_MAX)//add timeout for debug
        {
            //�ȴ���ʱ,��ʾ������
            lcd_menu_puts("mlist_TO backmain\n");
            os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
            return MENU_OPT_BACKMAIN;
        }

        if((menu_list_pt->item_cfg & ITEM_CFG_BIT_ACTIVE_SCROLL) != 0)
        {
            if(time_out_cnt >= MENU_TEXT_SCROLL_TIME)
            {
                // ���й����������������Ĺ���������������ʱ�˳�
                return MENU_OPT_ACTIVE_SCROLL;
            }
        }

        goto wait_msg_start;
    }

    lcd_menu_printf("mlist get_msg,%d,0x%x\n",menu_list_layer,msg[0]);

    menu_item_cbk = NULL;

    switch(msg[0])
    {
        //ѡ�м�
        case UI_MSG_MENULIST_KEYMODE:

        //�ȼ����û�з��ز˵�
        if(menu_list_pt->item_active == back_item_active)
        {
            //�з��ز˵���������һ��
            if(0 == menu_list_layer)
            {
                //��ʾ������
                os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
                return MENU_OPT_BACKMAIN;
            }
            else
            {
                return MENU_OPT_BACK;
            }
        }
        else
        {
            if(menu_list_pt->item_cbk_gp != NULL)
            {
                if((menu_list_pt->item_cfg & ITEM_CFG_BIT_CBKGP) != 0)
                {
                    //�ص�����
                    menu_item_cbk = (void*)(*((int*)menu_list_pt->item_cbk_gp + menu_list_pt->item_active));
                }
                else
                {
                    //ֻ��һ���ص�
                    menu_item_cbk = (void*)(*((int*)menu_list_pt->item_cbk_gp + 0));
                }

                //                lcd_menu_printf("item_cbk,0x%x\n",(int)menu_item_cbk);
                if(menu_item_cbk != NULL)
                {
                    return MENU_OPT_CALLCBK;
                    /*
                     menu_list_layer++;
                     ret = menu_item_cbk((void*)menu_list_pt->item_active);
                     menu_list_layer--;
                     if(ret == MENU_OPT_BACKMAIN)
                     {
                     return ret;
                     }
                     menu_need_draw = TRUE;
                     */
                }
                //�ص�Ϊ�գ�����Ӧ
            }

        }
        break;

        //�Ϸ�
        case UI_MSG_MENULIST_KEYPREV:
        if(menu_list_pt->item_active> 0)
        {
            menu_list_pt->item_active--;
            menu_need_draw = TRUE;
        }
        else
        {
            //ѭ���˵�
            menu_list_pt->item_active = menu_list_pt->item_total - 1;
            menu_need_draw = TRUE;
        }
        break;

        //�·�
        case UI_MSG_MENULIST_KEYNEXT:
        if(menu_list_pt->item_active < menu_list_pt->item_total - 1)
        {
            menu_list_pt->item_active++;
            menu_need_draw = TRUE;
        }
        else
        {
            //ѭ���˵�
            menu_list_pt->item_active = 0;
            menu_need_draw = TRUE;
        }
        break;

        //�����ϼ�
        case UI_MSG_MENULIST_KEYBACK:
        if(menu_list_layer> 0)
        {
            return MENU_OPT_BACK;
        }
        else
        {
            os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
            return MENU_OPT_BACKMAIN;
        }
        break;

        //�˳��˵�
        case UI_MSG_MENULIST_KEYEXIT:
        os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
        return MENU_OPT_BACKMAIN;
        break;

        case UI_MSG_REFLASH:
        case UI_MSG_OTHER:
        if((msg[1] >= MENU_200MS_REFRESH) && (msg[1] <= MENU_REFRESH))
        {
            //��ʱˢ�²��˳�
            break;
        }

        if((msg[1] == MENU_SET_EQ) || (msg[1] == MENU_SET_PLAY_MODE))
        {
            //���˽���ˢ��
            break;
        }

        case UI_MSG_DIS_MAIN:
        if(UI_var.bCurMenu != msg[1])
        {
            lcd_menu_printf("get ui-new exit,0x%x,0x%x\n",msg[0],msg[1]);
            os_taskq_post(UI_TASK_NAME, 3, msg[0], msg[1], msg[2]);
            return MENU_OPT_BACKMAIN;
        }
        break;

        default:
        break;
    }

    if(menu_need_draw == TRUE)
    {
        return MENU_OPT_WAITING;//ˢ�½��棬������wait
    }

    goto wait_msg_start;
}

/*
 *********************************************************************************************************
 *
 * Description: ͨ�ò˵���ʾ����
 *
 * Arguments  :
 *
 * Returns    : �������
 *
 * Note:�����뺯������֧��������ʾ�������ʾ����������
 *********************************************************************************************************
 */
int func_menulist_deal(void)
{
    menu_list_t *menu_list_pt;
    u16 cur_group,n,i,cnt_pre_screen,offset,sn_icon_a;
    int ret = MENU_OPT_BACK;
    _lcd_area_size_t *back_pt;

    menu_need_draw = TRUE;

    while (1)
    {
        menu_list_pt = &menu_list_table[menu_list_layer];
        if(menu_list_pt->item_active >= menu_list_pt->item_total)
        {
            menu_list_pt->item_active = 0;
        }

        if (menu_need_draw == TRUE)
        {
            lcd_clear();

            if(menu_list_pt->head_id == -1)
            {
                //�ޱ�����ʾ
                cnt_pre_screen = MENU_BROWSER_SCREEN;
                offset = 0;
            }
            else
            {
                //�б�����ʾ
                cnt_pre_screen = MENU_ITEMS_SCREEN;
                offset = 1;
                lcd_disp_text(menu_list_pt->str_table + (menu_list_pt->head_id * MENU_ITEM_STRLEN),item_DVcTxt_group[0]); // TITLE STRING

            }

            cur_group = menu_list_pt->item_active / cnt_pre_screen;
            n = cur_group * cnt_pre_screen;

            if(menu_list_pt->choise_sn != -1)
            {
                //��ѡ��˵�
                sn_icon_a = menu_list_pt->choise_sn;
            }
            else
            {
                //��ͨ�˵�
                sn_icon_a = menu_list_pt->item_active;
            }

            for (i = 0; i < cnt_pre_screen; i++,n++)
            {
                if (n >= menu_list_pt->item_total)
                {
                    break; //����������
                }

                //��ʾͼ��
                if(sn_icon_a == n)
                {
                    //ѡ��
                    ui_pic(menu_list_pt->icon_a, (GUI_POINT*)&item_icon_point[i+offset]);
                }
                else
                {
                    ui_pic(menu_list_pt->icon_b, (GUI_POINT*)&item_icon_point[i+offset]);
                }

                //��ʾ�˵�����
                lcd_disp_text(menu_list_pt->str_table + (menu_list_pt->item_id_gp[n] * MENU_ITEM_STRLEN),item_DVcTxt_group[i+offset]);
            }

            //��ʾ������
            func_slider_display(offset,menu_list_pt->item_active,menu_list_pt->item_total);

            //ѡ��ȡ����ʾ
            i = menu_list_pt->item_active % cnt_pre_screen;
            back_pt = (_lcd_area_size_t*)&item_set_backgroup[i+offset];
            lcd_TurnPixelReverse_Rect(back_pt->left,back_pt->top,back_pt->right,back_pt->bottom);

            draw_lcd_buf();
            menu_need_draw = FALSE;
        }//need draw

        ret = func_menuwait_sub_deal(menu_list_pt->item_total -1);

        if(ret == MENU_OPT_CALLCBK)
        {
            menu_list_layer++;
            ret = menu_item_cbk((void*)menu_list_pt->item_active);
            menu_list_layer--;
            if(ret == MENU_OPT_BACK)
            {
                //�ص�������������,�ϼ��˵������ȴ�ѡ��
                ret = MENU_OPT_WAITING;
            }
            menu_need_draw = TRUE;
        }

        if(ret != MENU_OPT_WAITING)
        {
            break;
        }
    }

    return ret;
}

/*
 *********************************************************************************************************
 *
 * Description: �ļ��������ʾ����
 *
 * Arguments  :
 *
 * Returns    :
 *
 * Note:��֧��������ʾ�������ʾ����������
 *********************************************************************************************************
 */
int func_menubrowser_deal(void)
{
    menu_list_t *menu_list_pt;
    u16 cur_group,disp_group,n,i,cnt_pre_screen,offset;
    u16 scroll_active_cnt = 0,tmp16;
    u8 flags[MENU_BROWSER_SCREEN];
    int ret = MENU_OPT_BACK;
    _lcd_area_size_t *back_pt;

    menu_need_draw = TRUE;
    disp_group = 0xffff;

    while (1)
    {
        menu_list_pt = &menu_list_table[menu_list_layer];

        menu_list_pt->item_cfg |= MENU_OPT_ACTIVE_SCROLL; //Ĭ�Ͽ��Թ���

        if(menu_list_pt->item_active >= menu_list_pt->item_total)
        {
            menu_list_pt->item_active = 0;
        }

//        printf("item_total = %d\n",menu_list_pt->item_total);

        if (menu_need_draw == TRUE)
        {
            lcd_clear();

            if(menu_list_pt->head_id == -1)
            {
                //�ޱ�����ʾ
                cnt_pre_screen = MENU_BROWSER_SCREEN;
                offset = 0;
            }
            else
            {
                //�б�����ʾ
                cnt_pre_screen = MENU_ITEMS_SCREEN;
                offset = 1;
                lcd_disp_text(menu_list_pt->str_table,item_DVcTxt_group[0]); // TITLE STRING
            }

            cur_group = menu_list_pt->item_active / cnt_pre_screen;
            n = cur_group * cnt_pre_screen;

            if(disp_group != cur_group)
            {
                //ˢ����ʾ�б�����
                disp_group = cur_group;
                func_browser_reflash(bs_items_buf,cnt_pre_screen,cur_group,ITEM_BACK_FIRST_FIRST_ID);
            }

            for (i = 0; i < cnt_pre_screen; i++,n++)
            {
                if (n >= menu_list_pt->item_total)
                {
                    break; //����������
                }

                //��ʾ�ļ��л��ļ�ͼ��
                if(bs_items_buf[i].dir_type == BS_DIR_TYPE_FORLDER)//���������ϼ�ͼƬ
                {
                    //�ļ���
                    tmp16 = menu_list_pt->icon_a;
                }
                else
                {
                    //�ļ�
                    tmp16= menu_list_pt->icon_b;
                }

                ui_pic(tmp16,(GUI_POINT*)&item_icon_point[i+offset]); //

                if(bs_items_buf[i].fn_type== BS_FNAME_TYPE_SHORT)
                {
                    flags[i] = (ANSI|SCROLL_RESET);
                }
                else
                {
                    flags[i] = (UNICODE_LE|SCROLL_RESET);
                }

                 //��ʾ�˵�����
                lcd_disp_text2((void*)bs_items_buf[i].lfn_buf.lfn,item_DVcTxt_group[i+offset],flags[i]);

            }

            i = menu_list_pt->item_active % cnt_pre_screen;

            //��ʾ������
            func_slider_display(offset,menu_list_pt->item_active,menu_list_pt->item_total);

            //ѡ��ȡ����ʾ
            back_pt = (_lcd_area_size_t *)&item_set_backgroup[i+offset];
            lcd_TurnPixelReverse_Rect(back_pt->left,back_pt->top,back_pt->right,back_pt->bottom);

            draw_lcd_buf();
            menu_need_draw = FALSE;
        }//need draw

        ret = func_menuwait_sub_deal(ITEM_BACK_FIRST_FIRST_ID);

        if(ret == MENU_OPT_CALLCBK)
        {
            menu_list_layer++;
            ret = menu_item_cbk((void*)menu_list_pt->item_active);
            menu_list_layer--;
            if(ret == MENU_OPT_BACK)
            {
                //�ص�������������,�ϼ��˵������ȴ�ѡ��
                ret = MENU_OPT_WAITING;
            }
            disp_group = 0xffff;
            menu_need_draw = TRUE;
        }

        if(ret == MENU_OPT_ACTIVE_SCROLL)
        {
            //����ѡ�������
            scroll_active_cnt++;
            if(scroll_active_cnt> MENU_TEXT_SCROLL_MAXCNT)
            {
                //�ȴ���ʱ,��ʾ������
                lcd_menu_puts("2mlist_TO backmain\n");
                os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
                return MENU_OPT_BACKMAIN;
            }
            //��ʾ�˵�����
            //lcd_menu_puts("active_scroll\n");
            i = menu_list_pt->item_active % cnt_pre_screen;
            back_pt = (_lcd_area_size_t *)&item_set_backgroup[i+offset];
            lcd_TurnPixelReverse_Rect(back_pt->left,back_pt->top,back_pt->right,back_pt->bottom);//��ȡ����ԭ����
            for(n = 0; n < MENU_TEXT_SCROLL_CHAR;n++)
            {
                lcd_disp_text2((void*)bs_items_buf[i].lfn_buf.lfn,item_DVcTxt_group[i+offset],flags[i]&(~SCROLL_RESET));
            }
            lcd_TurnPixelReverse_Rect(back_pt->left,back_pt->top,back_pt->right,back_pt->bottom);//�ı����ݺ���ȡ��
            draw_lcd_buf();

            ret = MENU_OPT_WAITING;
        }
        else
        {
            scroll_active_cnt = 0;
        }

        if(ret != MENU_OPT_WAITING)
        {
            break;
        }
    }

    return ret;
}

/*
 *********************************************************************************************************
 *
 * Description: mainӦ�����˵�����
 *
 * Arguments  :
 *
 * Returns    : �������
 *
 * Note:�����뺯��
 *********************************************************************************************************
 */
int func_menumain_deal(void)
{
    menu_list_t *menu_list_pt;
    u16 cur_group,total_group,n,i;
    int ret = MENU_OPT_BACK;

    menu_need_draw = TRUE;

    while (1)
    {
        menu_list_pt = &menu_list_table[menu_list_layer];

        if(menu_list_pt->item_active >= menu_list_pt->item_total)
        {
            menu_list_pt->item_active = 0;
        }

        total_group = menu_list_pt->item_total / MENU_MAIN_SCREEN;
        if((menu_list_pt->item_total % MENU_MAIN_SCREEN) != 0)
        {
            total_group++;
        }

        if (menu_need_draw == TRUE)
        {
            lcd_clear();
            cur_group = menu_list_pt->item_active / MENU_MAIN_SCREEN;
            n = cur_group * MENU_MAIN_SCREEN;

            //ѡ�б�����ʾ
            lcd_disp_text((void*)main_app_name_group[menu_list_pt->item_active],item_DVcTxt_group[0]); // TITLE STRING

            //��������ͼ��
            for (i = 0; i < MENU_MAIN_SCREEN; i++,n++)
            {
                if (n >= menu_list_pt->item_total)
                {
                    break; //����������
                }

                if(n != menu_list_pt->item_active)
                {
                    ui_pic(main_app_img_group[n][0], (GUI_POINT*)&main_icon_point[i]);
                }
                else
                {
                    //ѡ��
                    ui_pic(main_app_img_group[n][1], (GUI_POINT*)&main_icon_point[i]);
                }
            }

            //��ʾ���Ҽ�ͷ
            if(total_group> 1)
            {
                if(cur_group> 0)
                {
                    ui_pic(DVcRzImg1_10, 0);
                }
                if(cur_group < (total_group - 1))
                {
                    ui_pic(DVcRzImg2_10, 0);
                }
            }
            draw_lcd_buf();
            menu_need_draw = FALSE;
        }//need draw

        ret = func_menuwait_sub_deal(ITEM_BACK_NO_ITEM_ID);

        if(ret == MENU_OPT_CALLCBK)
        {
            menu_list_layer++;
            ret = menu_item_cbk((void*)menu_list_pt->item_active);
            menu_list_layer--;
            if(ret == MENU_OPT_BACK)
            {
                //�ص�������������,�ϼ��˵������ȴ�ѡ��
                ret = MENU_OPT_WAITING;
            }
            menu_need_draw = TRUE;
        }

        if(ret != MENU_OPT_WAITING)
        {
            break;
        }
    }

    return ret;
}

/*
 *********************************************************************************************************
 *
 * Description: lcd �˵����������
 *
 * Arguments  : arg - �������
 *
 * Returns    :
 *
 * Note:
 *********************************************************************************************************
 */
//
void enter_menulist_display(u32 arg)
{
    app_menulist_f menu_start_func;

    lcd_menu_printf("enter menulist arg = %x\n",arg);

    if(check_curtask_appid() != 0)
    {
        return;
    }

    menu_list_table = malloc(sizeof(menu_list_t)*MENU_LAYER_MAX);
    if(menu_list_table == NULL)
    {
        puts("***menu_list malloc fail\n");
        return;
    }

    //ע��˵�������
    menu_key_msg_register(UI_TASK_NAME,(void*)menu_adkey_msg_table,(void*)menu_irkey_msg_table);

    menu_list_layer = 0;
    menu_list_table[menu_list_layer].icon_a = DVcRzImg1_9;
    menu_list_table[menu_list_layer].icon_b = DVcRzImg2_9;

    if((arg&0xff) == UI_MENU_LIST_MAIN)
    {
        menu_task_app = MENU_TASK_MAIN_APP;
    }
    else if((arg&0xff) == UI_MENU_LIST_ITEM)
    {
        ;
    }
    else
    {
        ;
    }

    menu_start_func = app_menulist_func_table[menu_task_app];
    lcd_menu_printf("menu_start_func,0x%x\n",(int)menu_start_func);

    menu_start_func(arg);

    //ȡ������ע��
    menu_key_msg_unregister();

    free(menu_list_table);

}

/*
 *********************************************************************************************************
 *
 * Description: �ļ�����Ӳ˵���ʾ
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int start_browser_submenu(int active)
{
    int ret,tmp32;
    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer];
    MUSIC_DIS_VAR *music_var = (MUSIC_DIS_VAR*)UI_var.ui_buf_adr;

    bs_items_buf = malloc(sizeof(FS_DIR_INFO) * MENU_BROWSER_SCREEN);
    if(bs_items_buf == NULL)
    {
        puts("***bs malloc fail\n");
        return MENU_OPT_BACKMAIN;
    }

    menu_list_pt->head_id = -1;
    menu_list_pt->choise_sn = -1;
    menu_list_pt->item_active = 0;
    menu_list_pt->item_cfg = 0;

    menu_list_pt->icon_a = DVcRzImg6_9;
    menu_list_pt->icon_b = DVcRzImg7_9;

//    menu_list_pt->item_total = 20;
    menu_list_pt->item_cbk_gp = (void*)bs_func_cbk_gp_layer0;

//    menu_list_pt->bs_total_dir = 18;

    lcd_menu_puts("start_browser_disp\n");

    menu_browser_mapi =  ((MUSIC_DIS_VAR*)UI_var.ui_buf_adr)->mapi;

    if(menu_browser_mapi == NULL)
    {
        puts("bs no device\n");
        return MENU_OPT_BACKMAIN;
    }

    tmp32 = music_var->opt_state;

    if(tmp32 == MUSIC_OPT_BIT_PLAY)
    {
        //stop play
        os_taskq_post(RDEF_MUSIC_TASK_NAME, 1, MSG_MUSIC_PAUSE);
        os_time_dly(5);
    }

    menu_list_pt->item_total = file_bs_open_handle(menu_browser_mapi,NULL) + 1;//�������ز˵�

    ret = func_menubrowser_deal();

    file_bs_close_handle(menu_browser_mapi);

    free(bs_items_buf);


     if(tmp32 == MUSIC_OPT_BIT_PLAY)
    {
        //start play
        os_taskq_post(RDEF_MUSIC_TASK_NAME, 1, MSG_MUSIC_PLAY);
        os_time_dly(5);
    }


    return ret;
}

/*
 *********************************************************************************************************
 *
 * Description: ¼���Ӳ˵���ʾ
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int start_rec_ctl_submenu(int active)
{
    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer];

    menu_list_pt->head_id = 0;//-1;
    menu_list_pt->item_active = 0;
    menu_list_pt->item_cfg = 0;
    menu_list_pt->choise_sn = -1;

    menu_list_pt->icon_a = DVcRzImg1_9;
    menu_list_pt->icon_b = DVcRzImg2_9;

    menu_list_pt->str_table = (char*)rec_ctl_menu_string;

#if REC_EN

    if(file_operate_get_total_phydev() == 0)
    {
        UI_var.rec_opt_state = UI_REC_OPT_NODEV;
    }
	else
	{
		if(UI_var.rec_opt_state == UI_REC_OPT_NODEV)
		{
			UI_var.rec_opt_state = UI_REC_OPT_STOP;
		}
	}

#else

	UI_var.rec_opt_state = UI_REC_OPT_NODEV;

#endif

    if(UI_var.rec_opt_state == UI_REC_OPT_START)
    {
        menu_list_pt->item_id_gp = (u8 *)rec_ctl_menu_strid_layer0_m1;
        menu_list_pt->item_total = sizeof(rec_ctl_menu_strid_layer0_m1)/(sizeof(u8));
        menu_list_pt->item_cbk_gp = (void*)rec_ctl_func_cbk_gp_layer0_m1;
    }
    else if(UI_var.rec_opt_state == UI_REC_OPT_PAUSE)
    {
        menu_list_pt->item_id_gp = (u8 *)rec_ctl_menu_strid_layer0_m2;
        menu_list_pt->item_total = sizeof(rec_ctl_menu_strid_layer0_m2)/(sizeof(u8));
        menu_list_pt->item_cbk_gp = (void*)rec_ctl_func_cbk_gp_layer0_m2;
    }
    else if(UI_var.rec_opt_state == UI_REC_OPT_NODEV)
    {
        menu_list_pt->item_id_gp = (u8 *)rec_ctl_menu_strid_layer0_m3;
        menu_list_pt->item_total = sizeof(rec_ctl_menu_strid_layer0_m3)/(sizeof(u8));
        menu_list_pt->item_cbk_gp = (void*)rec_ctl_func_cbk_gp_layer0_m0;
    }
    else
    {
        menu_list_pt->item_id_gp = (u8 *)rec_ctl_menu_strid_layer0_m0;
        menu_list_pt->item_total = sizeof(rec_ctl_menu_strid_layer0_m0)/(sizeof(u8));
        menu_list_pt->item_cbk_gp = (void*)rec_ctl_func_cbk_gp_layer0_m0;
    }

    lcd_menu_puts("start_recctl_menu\n");

    return func_menulist_deal();

}

/*
 *********************************************************************************************************
 *
 * Description: main�˵����
 *
 * Arguments  : arg
 *
 * Returns    :
 *
 * Note:
 *********************************************************************************************************
 */
void start_main_menu(u32 arg)
{
    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer];

    menu_list_pt->head_id = 0;
    menu_list_pt->item_active = 0;
    menu_list_pt->item_cfg = 0;
    menu_list_pt->choise_sn = -1;

    menu_list_pt->item_total = sizeof(main_app_img_group)/(2);//ȡ������Ӧ��
    menu_list_pt->item_cbk_gp = (void*)main_func_cbk_layer0;

    lcd_menu_puts("start_main_menu\n");

    func_menumain_deal();
}

/*
 *********************************************************************************************************
 *
 * Description: ¼���˵����
 *
 * Arguments  : arg
 *
 * Returns    :
 *
 * Note:
 *********************************************************************************************************
 */
void start_radio_menu(u32 arg)
{
    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer];

    menu_list_pt->head_id = 0;//-1;
    menu_list_pt->item_active = 0;
    menu_list_pt->item_cfg = 0;
    menu_list_pt->choise_sn = -1;

    menu_list_pt->str_table = (char*)radio_menu_string;

    menu_list_pt->item_id_gp = (u8 *)radio_menu_strid_layer0;
    menu_list_pt->item_total = sizeof(radio_menu_strid_layer0)/(sizeof(u8));
    menu_list_pt->item_cbk_gp = (void*)radio_func_cbk_gp_layer0;

    lcd_menu_puts("start_radio_menu\n");

    func_menulist_deal();
}

/*
 *********************************************************************************************************
 *
 * Description: �˵����
 *
 * Arguments  : arg
 *
 * Returns    :
 *
 * Note:
 *********************************************************************************************************
 */
void start_music_menu(u32 arg)
{
    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer];
    MUSIC_DIS_VAR *music_var;
    music_var = (MUSIC_DIS_VAR*)UI_var.ui_buf_adr;

    menu_list_pt->head_id = 0;//-1;
    menu_list_pt->item_active = 0;
    menu_list_pt->item_cfg = ITEM_CFG_BIT_CBKGP;
    menu_list_pt->choise_sn = -1;

    menu_list_pt->str_table = (char*)music_menu_string;

    if(file_operate_get_total_phydev()> 1)
    {
        if(music_var->ui_curr_device != USB_DEVICE)
        {
            menu_list_pt->item_id_gp = (u8 *)music_menu_strid_layer0_m1;
            menu_list_pt->item_total = sizeof(music_menu_strid_layer0_m1)/(sizeof(u8));
        }
        else
        {
            menu_list_pt->item_id_gp = (u8 *)music_menu_strid_layer0_m2;
            menu_list_pt->item_total = sizeof(music_menu_strid_layer0_m2)/(sizeof(u8));
        }
    }
    else
    {
        menu_list_pt->item_id_gp = (u8 *)music_menu_strid_layer0;
        menu_list_pt->item_total = sizeof(music_menu_strid_layer0)/(sizeof(u8));
    }

    menu_list_pt->item_cbk_gp = (void*)music_func_cbk_gp_layer0;

    lcd_menu_puts("start_music_menu\n");

    func_menulist_deal();
}

/*
 *********************************************************************************************************
 *
 * Description: �˵����
 *
 * Arguments  : arg
 *
 * Returns    :
 *
 * Note:
 *********************************************************************************************************
 */
void start_btcall_menu(u32 arg)
{
    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer];

    menu_list_pt->head_id = 0;//-1;
    menu_list_pt->item_active = 0;
    menu_list_pt->item_cfg = 0;
    menu_list_pt->choise_sn = -1;

    menu_list_pt->str_table = (char*)bt_menu_string;


	extern bool get_sco_connect_status();
	if(get_sco_connect_status())
	{
		menu_list_pt->item_id_gp = (u8 *)btcall_menu_strid_layer0;
		menu_list_pt->item_total = sizeof(btcall_menu_strid_layer0)/(sizeof(u8));
		menu_list_pt->item_total = sizeof(btcall_menu_strid_layer0)/(sizeof(u8));
		menu_list_pt->item_cbk_gp = (void*)btcall_func_cbk_gp_layer0;
	}
	else
	{
		menu_list_pt->item_id_gp = (u8 *)btcall_menu_no_sco_strid_layer0;
		menu_list_pt->item_total = sizeof(btcall_menu_no_sco_strid_layer0)/(sizeof(u8));
		menu_list_pt->item_total = sizeof(btcall_menu_no_sco_strid_layer0)/(sizeof(u8));
		menu_list_pt->item_cbk_gp = (void*)btcall_no_sco_func_cbk_gp_layer0;
	}

    lcd_menu_puts("start_btcall_menu\n");

    func_menulist_deal();
}

/*
 *********************************************************************************************************
 *
 * Description: �˵����
 *
 * Arguments  : arg
 *
 * Returns    :
 *
 * Note:
 *********************************************************************************************************
 */
void start_btmusic_menu(u32 arg)
{
    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer];

    menu_list_pt->head_id = 0;//-1;
    menu_list_pt->item_active = 0;
    menu_list_pt->item_cfg = 0;
    menu_list_pt->choise_sn = -1;

    menu_list_pt->str_table = (char*)bt_menu_string;

    menu_list_pt->item_id_gp = (u8 *)btmusic_menu_strid_layer0;
    menu_list_pt->item_total = sizeof(btmusic_menu_strid_layer0)/(sizeof(u8));
    menu_list_pt->item_cbk_gp = (void*)btmusic_func_cbk_gp_layer0;

    lcd_menu_puts("start_btcall_menu\n");

    func_menulist_deal();
}

/*
 *********************************************************************************************************
 *
 * Description: �˵����
 *
 * Arguments  : arg
 *
 * Returns    :
 *
 * Note:
 *********************************************************************************************************
 */
void start_bt_menu(u32 arg)
{
    if((arg>>8) != 0)
    {
        start_btcall_menu(arg);
    }
    else
    {
        start_btmusic_menu(arg);
    }
}

/*
 *********************************************************************************************************
 *
 * Description: �˵����
 *
 * Arguments  : arg
 *
 * Returns    :
 *
 * Note:
 *********************************************************************************************************
 */
void start_linein_menu(u32 arg)
{
    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer];

    menu_list_pt->head_id = 0;//-1;
    menu_list_pt->item_active = 0;
    menu_list_pt->item_cfg = 0;
    menu_list_pt->choise_sn = -1;

    menu_list_pt->str_table = (char*)linein_menu_string;

    menu_list_pt->item_id_gp = (u8 *)linein_menu_strid_layer0;
    menu_list_pt->item_total = sizeof(linein_menu_strid_layer0)/(sizeof(u8));
    menu_list_pt->item_cbk_gp = (void*)linein_func_cbk_gp_layer0;

    lcd_menu_puts("start_line_menu\n");

    func_menulist_deal();
}

/*
 *********************************************************************************************************
 *
 * Description: �˵����
 *
 * Arguments  : arg
 *
 * Returns    :
 *
 * Note:
 *********************************************************************************************************
 */
void start_record_menu(u32 arg)
{
    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer];

    menu_list_pt->head_id = 0;//-1;
    menu_list_pt->item_active = 0;
    menu_list_pt->item_cfg = 0;
    menu_list_pt->choise_sn = -1;

    menu_list_pt->str_table = (char*)record_menu_string;

    menu_list_pt->item_id_gp = (u8 *)record_menu_strid_layer0;
    menu_list_pt->item_total = sizeof(record_menu_strid_layer0)/(sizeof(u8));
    menu_list_pt->item_cbk_gp = (void*)record_func_cbk_gp_layer0;

    lcd_menu_puts("start_lrecord_menu\n");

    func_menulist_deal();
}

/*
 *********************************************************************************************************
 *
 * Description: �˵����
 *
 * Arguments  : arg
 *
 * Returns    :
 *
 * Note:
 *********************************************************************************************************
 */
void start_usb_menu(u32 arg)
{
    lcd_menu_puts("start_usb_menu\n");
}

/*
 *********************************************************************************************************
 *
 * Description: �˵����
 *
 * Arguments  : arg
 *
 * Returns    :
 *
 * Note:
 *********************************************************************************************************
 */
void start_rtc_menu(u32 arg)
{
    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer];

    menu_list_pt->head_id = 0;//-1;
    menu_list_pt->item_active = 0;
    menu_list_pt->item_cfg = 0;
    menu_list_pt->choise_sn = -1;

    menu_list_pt->str_table = (char*)rtc_menu_string;

    menu_list_pt->item_id_gp = (u8 *)rtc_menu_strid_layer0;
    menu_list_pt->item_total = sizeof(rtc_menu_strid_layer0)/(sizeof(u8));
    menu_list_pt->item_cbk_gp = (void*)rtc_func_cbk_gp_layer0;

    lcd_menu_puts("start_rtc_menu\n");

    func_menulist_deal();
}



/*
 *********************************************************************************************************
 *
 * Description: �����Ӳ˵���ʾ
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int start_echo_ctl_submenu(int active)
{
    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer];

    menu_list_pt->head_id = 0;//-1;
    menu_list_pt->item_active = 0;
    menu_list_pt->item_cfg = 0;
    menu_list_pt->choise_sn = -1;

    menu_list_pt->icon_a = DVcRzImg1_9;
    menu_list_pt->icon_b = DVcRzImg2_9;

    menu_list_pt->str_table = (char*)echo_ctl_menu_string;

#if ECHO_EN

    if((UI_var.echo_pt != NULL)&&(UI_var.echo_pt[0] != 0))
    {
        menu_list_pt->item_id_gp = (u8 *)echo_ctl_menu_strid_layer0_m1;
        menu_list_pt->item_total = sizeof(echo_ctl_menu_strid_layer0_m1)/(sizeof(u8));
        menu_list_pt->item_cbk_gp = (void*)echo_ctl_func_cbk_gp_layer0_m1;
    }
    else
    {
        menu_list_pt->item_id_gp = (u8 *)echo_ctl_menu_strid_layer0_m0;
        menu_list_pt->item_total = sizeof(echo_ctl_menu_strid_layer0_m0)/(sizeof(u8));
        menu_list_pt->item_cbk_gp = (void*)echo_ctl_func_cbk_gp_layer0_m0;
    }

#else
    menu_list_pt->item_id_gp = (u8 *)echo_ctl_menu_strid_layer0_nosupport;
    menu_list_pt->item_total = sizeof(echo_ctl_menu_strid_layer0_nosupport)/(sizeof(u8));
    menu_list_pt->item_cbk_gp = (void*)echo_ctl_func_cbk_gp_layer0_m0;

#endif

    lcd_menu_puts("start_echo-ctl_menu\n");

    return func_menulist_deal();

}


extern const int task_connect_dev_valsize;
extern const struct task_dev_info *task_connect_dev[];

static u32 menu_fw_dev_is_exist(const char *check_task_name)
{
#if 1
    u8 i;
    u32 dev_cnt;

    dev_cnt = task_connect_dev_valsize / sizeof(int);
    //    rcsp_printf("is-dev: %d \n",dev_cnt);

    for (i = 0; i < dev_cnt; i++)
    {
        if (0 == strcmp((void*) check_task_name, (void*) (task_connect_dev[i]->name)))
        {
            if (0 == task_connect_dev[i]->dev_check())
            {
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
    }
#endif
    return TRUE;
}



static int check_taskmode_is_offline(char *next_task_name)
{

    lcd_menu_printf("menu:switch task, %s\n",next_task_name);

    if(0 == strcmp(next_task_name,RDEF_MUSIC_TASK_NAME))
    {
        if(FALSE == menu_fw_dev_is_exist(next_task_name))
        {
            lcd_menu_puts("menu:sw music fail,dev is not exist\n");
            return 1;
        }
    }

    else if(0 == strcmp(next_task_name,RDEF_LINEIN_TASK_NAME))
    {
        if(FALSE == menu_fw_dev_is_exist(next_task_name))
        {
            lcd_menu_puts("menu:sw linein fail,dev is not exist\n");
            return 2;
        }
    }

    else if(0 == strcmp(next_task_name,RDEF_UDISK_TASK_NAME))
    {
        if(FALSE == menu_fw_dev_is_exist(next_task_name))
        {
            lcd_menu_puts("menu:sw udisk fail,dev is not exist\n");
            return 3;
        }
    }

    else if(0 == strcmp(next_task_name,RDEF_RECORD_TASK_NAME))
    {
        if(FALSE == menu_fw_dev_is_exist(next_task_name))
        {
            lcd_menu_puts("menu:sw record fail,dev is not exist\n");
            return 4;
        }
    }


    return 0;
}



/*
 *********************************************************************************************************
 *
 * Description: main�˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int main_func_cbk_deal(int active)
{
    u8 app_id = main_menu_id_group[active];
    int ret = MENU_OPT_BACK;
    char *switch_task_name;

    if(app_id < MENU_TASK_APP_MAX)
    {
        if(app_id == MENU_TASK_BTSTACK_APP)
        {
            switch_task_name = RDEF_BTSTACK_TASK_NAME;
        }
        else
        {
            switch_task_name = (char *)&menu_app_task_namestr[app_id];
        }
        lcd_menu_printf("main-cbk:%s\n",switch_task_name);

        if(strcmp(switch_task_name,curr_task->name) != 0)
        {
            if(check_taskmode_is_offline(switch_task_name))
			{
    			os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, MENU_DEV_ERR);
			}
			else
			{
				os_taskq_post(RDEF_MAIN_TASK_NAME, 2, SYS_EVENT_TASK_RUN_REQ,switch_task_name);
            	os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, MENU_WAIT);
			}
        }
        else
       {
            os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
            return MENU_OPT_BACKMAIN;
       }
    }
    else
    {
        lcd_menu_puts("sys_set\n");
        //lcd_menu_puts("sys_exit\n");
        //        os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
        return ret;
    }

    ret = MENU_OPT_BACKMAIN;
    return ret;
}

/*
 *********************************************************************************************************
 *
 * Description: �˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int radio_func_cbk_deal(int active)
{
    int ret = MENU_OPT_BACK;
    int msg = MSG_NULL;

    lcd_menu_printf("rec0_deal,%d\n",active);

    switch(active)
    {
        case 0: //item0
        msg = MSG_FM_SCAN_ALL_INIT;
        break;

        case 1://item1
        ret = start_rec_ctl_submenu(active);
        break;

        case 2:
        ret = start_echo_ctl_submenu(active);
        break;

        default:
        break;
    }

    if(msg != MSG_NULL)
    {
        os_taskq_post((void*)&menu_app_task_namestr[menu_task_app], 2, msg,UI_POST_MSG_FLAG);
        os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
        ret = MENU_OPT_BACKMAIN;
    }

    return ret;

}

/*
 *********************************************************************************************************
 *
 * Description: �˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int btmusic_func_cbk_deal(int active)
{
    int ret = MENU_OPT_BACK;
    int msg = MSG_NULL;

    lcd_menu_printf("btmusic_deal,%d\n",active);

    switch(active)
    {
        case 0: //item0
        ret = btmusic_func_cbk_eq_disp(active);
        break;

        case 1: //item1
        ret = start_echo_ctl_submenu(active);
        break;

        default:
        break;
    }

    if(msg != MSG_NULL)
    {
        os_taskq_post((void*)&menu_app_task_namestr[menu_task_app], 2, msg,UI_POST_MSG_FLAG);
        os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
        ret = MENU_OPT_BACKMAIN;
    }
    return ret;

}

/*
 *********************************************************************************************************
 *
 * Description: �˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int btcall_func_cbk_deal(int active)
{
    int ret = MENU_OPT_BACK;
    int msg = MSG_NULL;

    lcd_menu_printf("btcal_deal,%d\n",active);

    switch(active)
    {
        case 0: //item0
        ret = start_rec_ctl_submenu(active);
        break;

        default:
        break;
    }

    if(msg != MSG_NULL)
    {
        os_taskq_post((void*)&menu_app_task_namestr[menu_task_app], 2, msg,UI_POST_MSG_FLAG);
        os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
        ret = MENU_OPT_BACKMAIN;
    }

    return ret;

}

/*
 *********************************************************************************************************
 *
 * Description: �˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int btcall_no_sco_func_cbk_deal(int active)
{
    int ret = MENU_OPT_BACK;
    int msg = MSG_NULL;

    lcd_menu_printf("btcal_deal,%d\n",active);

    switch(active)
    {
        default:
        break;
    }

    if(msg != MSG_NULL)
    {
        os_taskq_post((void*)&menu_app_task_namestr[menu_task_app], 2, msg,UI_POST_MSG_FLAG);
        os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
        ret = MENU_OPT_BACKMAIN;
    }

    return ret;

}

/*
 *********************************************************************************************************
 *
 * Description: �˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int linein_func_cbk_deal(int active)
{
    int ret = MENU_OPT_BACK;
    int msg = MSG_NULL;

    lcd_menu_printf("linein_deal,%d\n",active);

    switch(active)
    {
        case 0: //item0
        ret = start_rec_ctl_submenu(active);
        break;

        case 1: //item1
        ret = start_echo_ctl_submenu(active);
        break;

        default:
        break;
    }

    if(msg != MSG_NULL)
    {
        os_taskq_post((void*)&menu_app_task_namestr[menu_task_app], 2, msg,UI_POST_MSG_FLAG);
        os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
        ret = MENU_OPT_BACKMAIN;
    }

    return ret;

}

/*
 *********************************************************************************************************
 *
 * Description: �˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int record_func_cbk_deal(int active)
{
    int ret = MENU_OPT_BACK;
    int msg = MSG_NULL;

    lcd_menu_printf("record_deal,%d\n",active);

    switch(active)
    {
        case 0: //item0
        ret = start_rec_ctl_submenu(active);
        break;

        default:
        break;
    }

    if(msg != MSG_NULL)
    {
        os_taskq_post((void*)&menu_app_task_namestr[menu_task_app], 2, msg,UI_POST_MSG_FLAG);
        os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
        ret = MENU_OPT_BACKMAIN;
    }

    return ret;

}

/*
 *********************************************************************************************************
 *
 * Description: �˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int rtc_func_cbk_deal(int active)
{
    int ret = MENU_OPT_BACK;
    int msg = MSG_NULL;

    lcd_menu_printf("linein_deal,%d\n",active);

    switch(active)
    {
        case 0: //item0
        msg = MSG_RTC_SETTING;
        break;

        case 1://item1
        msg = MSG_ALM_SETTING;
        break;
        default:
        break;
    }

    if(msg != MSG_NULL)
    {
        os_taskq_post((void*)&menu_app_task_namestr[menu_task_app], 2, msg,UI_POST_MSG_FLAG);
        os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
        ret = MENU_OPT_BACKMAIN;
    }

    return ret;

}

/*
 *********************************************************************************************************
 *
 * Description: �˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int rec_ctl_func_cbk_deal_m0(int active)
{
    int ret = MENU_OPT_BACK;
    int msg = MSG_NULL;

    lcd_menu_printf("rec0_deal,%d\n",active);

    switch(active)
    {
        case 0: //item0
        msg = MSG_REC_START;
        break;

        default:
        break;
    }

    if(msg != MSG_NULL)
    {
        os_taskq_post((void*)&menu_app_task_namestr[menu_task_app], 2, msg,UI_POST_MSG_FLAG);
        os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
        ret = MENU_OPT_BACKMAIN;
    }

    return ret;

}

/*
 *********************************************************************************************************
 *
 * Description: �˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int rec_ctl_func_cbk_deal_m1(int active)
{
    int ret = MENU_OPT_BACK;
    int msg = MSG_NULL;

    lcd_menu_printf("rec1_deal,%d\n",active);

    switch(active)
    {
        case 0://item0
        msg = MSG_REC_PP;
        break;

        case 1://item1
        msg = MSG_REC_STOP;
        break;

        default:
        break;
    }

    if(msg != MSG_NULL)
    {
        os_taskq_post((void*)&menu_app_task_namestr[menu_task_app], 2, msg,UI_POST_MSG_FLAG);
        os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
        ret = MENU_OPT_BACKMAIN;
    }

    return ret;

}

/*
 *********************************************************************************************************
 *
 * Description: �˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int browser_func_cbk_deal(int active)
{
    int ret = MENU_OPT_BACK;
    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer - 1];
    FS_DIR_INFO *dir_info = bs_items_buf + (active % MENU_BROWSER_SCREEN);

    lcd_menu_printf("bs-cbk:%d\n",active);

    if((menu_list_layer < MENU_LAYER_MAX) &&
            (dir_info->dir_type ==  BS_DIR_TYPE_FORLDER))//
    {
        menu_list_pt = &menu_list_table[menu_list_layer];
        menu_list_pt->head_id = -1;
        menu_list_pt->item_active = 0;
        menu_list_pt->item_cfg = 0;

        menu_list_pt->icon_a = DVcRzImg6_9;
        menu_list_pt->icon_b = DVcRzImg7_9;

        menu_list_pt->item_total =file_bs_entern_dir(menu_browser_mapi, dir_info) + 1;//�������ز˵�
        menu_list_pt->item_cbk_gp = (void*)bs_func_cbk_gp_layer0;

        lcd_menu_printf("start_bs_sub,%d\n",menu_list_layer);

        ret = func_menubrowser_deal();

        file_bs_exit_dir(menu_browser_mapi);

    }
    else if(dir_info->dir_type ==  BS_DIR_TYPE_FILE)
    {
        lcd_menu_printf("sclust:0x%x,app = %d\n",dir_info->sclust,menu_task_app);
        os_taskq_post((void*)&menu_app_task_namestr[menu_task_app], 2, MSG_MUSIC_SPC_FILE,dir_info->sclust);
        os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
        ret = MENU_OPT_BACKMAIN;
		os_time_dly(2);
    }
    else
    {
        puts("***bs layer max\n");
    }

    return ret;
}

/*
 *********************************************************************************************************
 *
 * Description: �˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int music_func_cbk_deal(int active)
{
    int ret = MENU_OPT_BACK;
    int msg = MSG_NULL;
    int screen_main = UI_var.bMainMenu;

    lcd_menu_printf("music_deal,%d\n",active);

    switch(active)
    {
        case 0: //item0
        case 1: //item1
        case 2: //item2
        case 3: //item2
        break;

        case 4: //item4
        msg = MSG_MUSIC_DEL_FILE;
        break;

        case 5: //item5
        msg = MSG_MUSIC_U_SD;
        screen_main = MENU_WAIT;
        break;

        default:
        break;
    }

    if(msg != MSG_NULL)
    {
        lcd_menu_puts("music post msg\n");
        os_taskq_post((void*)&menu_app_task_namestr[menu_task_app], 2, msg,UI_POST_MSG_FLAG);
        os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, screen_main);
        ret = MENU_OPT_BACKMAIN;
    }

    return ret;

}

/*
 *********************************************************************************************************
 *
 * Description: �˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int music_func_cbk_playmode_disp(int active)
{
    int ret = MENU_OPT_BACK;
    MUSIC_DIS_VAR *music_var;
    music_var = (MUSIC_DIS_VAR*)UI_var.ui_buf_adr;

    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer];

    menu_list_pt->head_id = 0;//-1;
    menu_list_pt->item_active = 0;
    menu_list_pt->item_cfg = 0;

    menu_list_pt->str_table = (char*)music_menu_string;

    menu_list_pt->icon_a = DVcRzImg8_9;
    menu_list_pt->icon_b = DVcRzImg9_9;

    menu_list_pt->item_id_gp = (u8 *)music_menu_strid_playmode;
    menu_list_pt->item_total = sizeof(music_menu_strid_playmode)/(sizeof(u8));
    menu_list_pt->item_cbk_gp = (void*)music_func_cbk_gp_playmode;

    menu_list_pt->choise_sn = *(music_var->play_mode) - REPEAT_ALL;
    ret = func_menulist_deal();

    return ret;
}

/*
 *********************************************************************************************************
 *
 * Description: �˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int music_func_cbk_playmode_set(int active)
{
    int ret = MENU_OPT_BACK;
    MUSIC_DIS_VAR *music_var;
    music_var = (MUSIC_DIS_VAR*)UI_var.ui_buf_adr;
    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer-1];

    lcd_menu_puts("music_playmode_set\n");
    os_taskq_post((void*)&menu_app_task_namestr[menu_task_app], 2, MSG_MUSIC_RPT,(active+REPEAT_ALL)|(1<<16));
    os_time_dly(5); //�����̣߳������߳����㹻ʱ����Ӧ��Ϣ

    menu_list_pt->choise_sn = *(music_var->play_mode) - REPEAT_ALL;//����������

    return ret;
}

/*
 *********************************************************************************************************
 *
 * Description: �˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int music_func_cbk_eq_disp(int active)
{
    int ret = MENU_OPT_BACK;
    MUSIC_DIS_VAR *music_var;
    music_var = (MUSIC_DIS_VAR*)UI_var.ui_buf_adr;

    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer];

    menu_list_pt->head_id = 0;//-1;
    menu_list_pt->item_active = 0;
    menu_list_pt->item_cfg = 0;

    menu_list_pt->str_table = (char*)music_menu_string;

    menu_list_pt->icon_a = DVcRzImg8_9;
    menu_list_pt->icon_b = DVcRzImg9_9;

    menu_list_pt->item_id_gp = (u8 *)music_menu_strid_eq;
    menu_list_pt->item_total = sizeof(music_menu_strid_eq)/(sizeof(u8));
    menu_list_pt->item_cbk_gp = (void*)music_func_cbk_gp_eq;

    menu_list_pt->choise_sn = *(music_var->eq_mode);
    ret = func_menulist_deal();

    return ret;
}

/*
 *********************************************************************************************************
 *
 * Description: �˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int music_func_cbk_eq_set(int active)
{
    int ret = MENU_OPT_BACK;
    MUSIC_DIS_VAR *music_var;
    music_var = (MUSIC_DIS_VAR*)UI_var.ui_buf_adr;
    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer-1];

    lcd_menu_puts("music_eq_set\n");
    os_taskq_post((void*)&menu_app_task_namestr[menu_task_app], 2, MSG_MUSIC_EQ,active|(1<<16));
    os_time_dly(5);//�����̣߳������߳����㹻ʱ����Ӧ��Ϣ

    menu_list_pt->choise_sn = *(music_var->eq_mode);//����������

    return ret;
}

/*
 *********************************************************************************************************
 *
 * Description: �˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int btmusic_func_cbk_eq_disp(int active)
{
    int ret = MENU_OPT_BACK;
#if 1
    BT_DIS_VAR * bt_var;
    bt_var = (BT_DIS_VAR *)UI_var.ui_buf_adr;

    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer];

    menu_list_pt->head_id = 0;//-1;
    menu_list_pt->item_active = 0;
    menu_list_pt->item_cfg = 0;

    menu_list_pt->str_table = (char*)bt_menu_string;

    menu_list_pt->icon_a = DVcRzImg8_9;
    menu_list_pt->icon_b = DVcRzImg9_9;

    menu_list_pt->item_id_gp = (u8 *)btmusic_menu_strid_eq;
    menu_list_pt->item_total = sizeof(btmusic_menu_strid_eq)/(sizeof(u8));
    menu_list_pt->item_cbk_gp = (void*)btmusic_func_cbk_gp_eq;

    lcd_menu_puts("bt eq disp\n");
    menu_list_pt->choise_sn = *(bt_var->bt_eq_mode);
    ret = func_menulist_deal();
#endif
    return ret;
}

/*
 *********************************************************************************************************
 *
 * Description:
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:
 *********************************************************************************************************
 */
int btmusic_func_cbk_eq_set(int active)
{
    int ret = MENU_OPT_BACK;
 #if 1
    BT_DIS_VAR * bt_var;
    bt_var = (BT_DIS_VAR *)UI_var.ui_buf_adr;
    menu_list_t *menu_list_pt = &menu_list_table[menu_list_layer-1];

    lcd_menu_puts("bt_eq_set\n");
    os_taskq_post((void*)&menu_app_task_namestr[menu_task_app], 2, MSG_BT_MUSIC_EQ,active|(1<<16));
    os_time_dly(5);//�����̣߳������߳����㹻ʱ����Ӧ��Ϣ

    menu_list_pt->choise_sn = *(bt_var->bt_eq_mode);//����������
    lcd_menu_puts("bt_eq_set exit\n");
#endif
    return ret;
}



/*
 *********************************************************************************************************
 *
 * Description: echo�˵��ص�����
 *
 * Arguments  : �ϼ��˵�ѡ����
 *
 * Returns    : �������
 *
 * Note:    ��pp�������߳�
 *********************************************************************************************************
 */
int echo_ctl_func_cbk_deal_m0(int active)
{
    int ret = MENU_OPT_BACK;
    int msg = MSG_NULL;

    lcd_menu_printf("echo0_deal,%d\n",active);

    switch(active)
    {
        case 0: //item0
	   if( UI_var.echo_pt[0] != 0)
	   {
	   	msg = MSG_ECHO_STOP;
	   }
	   else
	   {
	   	msg = MSG_ECHO_START;
	   }
	 /* msg = MSG_ECHO_PP; */
        break;

        default:
        break;
    }

    if(msg != MSG_NULL)
    {
        os_taskq_post((void*)&menu_app_task_namestr[menu_task_app], 2, msg,UI_POST_MSG_FLAG);
        os_taskq_post(UI_TASK_NAME, 2, UI_MSG_REFLASH, UI_var.bMainMenu);
        ret = MENU_OPT_BACKMAIN;
    }

    return ret;

}


#endif/**/

