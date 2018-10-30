#include "sdk_cfg.h"
#include "common/app_cfg.h"
#include "common/msg.h"
#include "common/includes.h"
#include "common/common.h"
#include "rtos/task_manage.h"
#include "play_sel/play_sel.h"
#include "sdmmc/sd_host_api.h"
#include "encode/dev_write_api.h"
#include "encode/mp2_encode_api.h"
#include "encode/encode.h"
#include "record.h"
#include "dac/dac_api.h"
#include "ui/ui_api.h"
#include "rec_key.h"
#include "sys_detect.h"
#include "rec_ui.h"
#include "key_drv/key.h"
#include "clock_api.h"
#include "clock.h"
/* #include "music_ui.h" */
/* #include "music.h" */
/* #include "linein.h" */

#define BT_REC_SR		8000L		//�����޸�
#define FM_REC_SR		32000L		//
#define AUX_REC_SR		48000L
#define MIX_REC_SR		48000L

extern void *malloc_fun(void *ptr,u32 len,char *info);
extern void free_fun(void **ptr);
extern volatile u8 new_lg_dev_let;

#if REC_EN

#define REC_FILE     1//1��¼MP3�ļ�       0��¼WAV�ļ�

const u16 enc_bitrate_h[]=
{
    ///>(44100), (48000), (32000) �����ʿ��������±�����
    32,48,56,64,80,96,112,128
};
const u16 enc_bitrate_l[]=
{
    ///>(44.1K/2,/4),  (48k/2,/4),  (32K/2,/4) �����ʿ��������±�����
    8,16,24,32,40,48,56,64,80,96,112,128
};

const char rec_folder_name[] = "/JL_REC";                   //¼���ļ���  //��֧��һ��Ŀ¼

#if REC_FILE
	const char rec_file_name[] =   "/JL_REC/FILE0000.MP3";      //MP3¼���ļ�������·����
#else
	const char rec_file_name[] =   "/JL_REC/FILE0000.WAV";      //ADPCM¼���ļ�������·����
#endif

u32 rec_fname_cnt;       //¼���ļ�������
u32 rec_total_number;    //¼���ļ�������

REC_CTL *g_rec_ctl;
RECORD_OP_API *rec_mic_api;

/* #if REC_BT_EN */
	/* s16 *g_bt_buf_ptr; */
/* #endif */

extern ENC_OPS *get_mpl3_ops();
extern ENC_OPS *get_mp2_ops();
extern ENC_OPS *get_enadpcm_ops();
/* extern ENC_OPS_t *get_mp2_opst(); */
/* extern ENC_OPS_t *get_enadpcm_opst(); */

ENC_OPS *(*enc_ops_list[MAX_FORMAT])(void) AT(.const_tab);
ENC_OPS *(*enc_ops_list[MAX_FORMAT])(void) =
{
    NULL,       //���

#if ENCODE_MP2_EN
//    get_mpl3_ops,
    get_mp2_ops,
#endif

#if ENCODE_ADPCM_EN
    get_enadpcm_ops,
#endif
};

/* ENC_OPS_t *(*enc_ops_list_t[MAX_FORMAT])(void) AT(.const_tab); */
/* ENC_OPS_t *(*enc_ops_list_t[MAX_FORMAT])(void) = */
/* { */
    /* NULL,       //��� */

/* #if ENCODE_MP2_EN */
    /* get_mp2_opst, */
/* #endif */

/* #if ENCODE_ADPCM_EN */
    /* get_enadpcm_opst, */
/* #endif */
/* }; */

#if BT_REC_EN
extern RECORD_OP_API * rec_bt_api ;
cbuffer_t bt_adc_cb;
u8* bt_adc_cbuf;
#define BT_REC_BUF_LEN 1024 
void bt_rec_buf_init(void)
{
	u32 bt_adc_cbuf_len;
	enc_puts("bt_rec buf init\n");

	bt_adc_cbuf_len = BT_REC_BUF_LEN;
	bt_adc_cbuf= malloc(BT_REC_BUF_LEN);
	ASSERT(bt_adc_cbuf);
    cbuf_init(&bt_adc_cb, bt_adc_cbuf, bt_adc_cbuf_len);
}
void bt_rec_buf_release(void)
{
	enc_printf("temp = %x",bt_adc_cbuf);
	if(bt_adc_cbuf != NULL)
		free(bt_adc_cbuf);
	bt_adc_cbuf = NULL;
	cbuf_clear(&bt_adc_cb);
}
void bt_rec_buf_write(s16* buf,u32 len)
{
	if((rec_bt_api != NULL) && (rec_bt_api->rec_ctl != NULL)&&(rec_bt_api->rec_ctl->enable == ENC_STAR))
	{
		if(cbuf_is_write_able(&bt_adc_cb,len))
		{
			/* putchar('g'); */
			cbuf_write(&bt_adc_cb,buf,len);
		}
	}
}
/*----------------------------------------------------------------------------*/
/**@brief  ����¼������Դ��ȡ
   @param  ��
   @return ��
   @note   void bt_rec_get_data(s16 *buf,u32 len,)//¼����ȡpcm���ݺ���
*/
/*----------------------------------------------------------------------------*/
void bt_rec_get_data(s16 *buf, u32 len)//¼����ȡpcm���ݺ���
{

	u8 bt_rec_buf[0x40];//dac_buf_len
	s16 *dp = (s16 *)bt_rec_buf;
	u16 adc_buf[0x20];
	u32 rec_point;
	if((rec_bt_api != NULL) && (rec_bt_api->rec_ctl != NULL)&&(rec_bt_api->rec_ctl->enable == ENC_STAR))
	{
		if(cbuf_read(&bt_adc_cb, (u8*)adc_buf,len/2 )!= (len/2))
		{
			/* enc_printf("no_enough\n"); */
			memset((u8*)adc_buf,0x00,len);
		}
		else
		{
		}
		if(rec_bt_api->rec_ctl->enable == ENC_STAR)
		{
			for(rec_point = 0; rec_point < 0x20; rec_point++)
			{
				dp[rec_point] =(s16) (adc_buf[rec_point]+buf[rec_point*2]);//all left channel data
				if(dp[rec_point]>32767)
				{
					dp[rec_point]=32767;
				}
				else if(dp[rec_point]<-32768)
				{
					dp[rec_point]=-32768;
				}
			}
			rec_bt_api->rec_ctl->ladc.save_ladc_buf(&rec_bt_api->rec_ctl->ladc, dp, 0, 0x40);

			for(rec_point = 0; rec_point < 0x20; rec_point++)
			{
				/* dp[rec_point] = (buf[rec_point*2+1]);// + adc_buf[rec_point])/2;//all right channel data */
				dp[rec_point] =(s16) (adc_buf[rec_point]+buf[rec_point*2+1]);//all left channel data
				if(dp[rec_point]>32767)
				{
					dp[rec_point]=32767;
				}
				else if(dp[rec_point]<-32768)
				{
					dp[rec_point]=-32768;
				}
			}
			rec_bt_api->rec_ctl->ladc.save_ladc_buf(&rec_bt_api->rec_ctl->ladc, dp, 1, 0x40);
		}
	}
}
#endif

extern s32 get_dev_next(u8 drv, u8 dev_type);

/*----------------------------------------------------------------------------*/
/**@brief  ¼��ͨ����ʼ�����ļ���ʼ��
   @param  mode: ¼��ͨ��
   @return ��
   @note   RECORD_OP_API *rec_init(u8 mode)
*/
/*----------------------------------------------------------------------------*/
static RECORD_OP_API *rec_init(u8 mode)
{
	u8 emu_lg_dev = 0;//ö�ٵ��豸����0����һ��ö�٣�1���ڶ���ö�٣�����1����ö���˳���
    s16 ret;
    u32 rec_file_num;
	u32 dev_next=0;
    RECORD_OP_API *rec_op_api = NULL;

    rec_op_api = malloc_fun(rec_op_api, sizeof(RECORD_OP_API), 0);
    ASSERT(rec_op_api);

    rec_op_api->fop_api = enc_fop_api_init();   //�ļ�������Ϣ��ʼ��
    printf("-------enc_fop_api_init %x------\n", rec_op_api->fop_api);

    rec_op_api->fw_api = fw_api_init();     //��д�ӿڳ�ʼ��
    printf("------fw_api_init %x------\n", rec_op_api->fw_api);
    printf("------new_lg_dev_let%x------\n", new_lg_dev_let);
    dev_next = get_dev_next(new_lg_dev_let, DEV_TYPE_MASS);
    printf("------lg_dev_let%x------\n", dev_next);

_file_init:
    puts("------encode_fs_open-----\n");
    ret = encode_fs_open(rec_op_api, DEV_SEL_SPEC, new_lg_dev_let, (char *)rec_folder_name, FA_CREATE_NEW);  //��ʼ���ļ�ϵͳ
    printf("--encode_fs_open ret = %x  \n", ret);

    if(-FILE_OP_ERR_LGDEV_NO_FIND == ret)
    {
        printf("--DEV_SEL_FIRST--\n");
        ret = encode_fs_open(rec_op_api, DEV_SEL_FIRST, new_lg_dev_let, (char *)rec_folder_name, FA_CREATE_NEW);  //��ʼ���ļ�ϵͳ
    }

    if(ret == FR_EXIST)
    {
#if REC_FILE
        encode_get_fileinfo(rec_op_api,(char *)rec_folder_name,"MP3",&rec_file_num, &rec_total_number);
#else
        encode_get_fileinfo(rec_op_api,(char *)rec_folder_name,"WAV",&rec_file_num, &rec_total_number);
#endif
        printf("rec_total_number = %d\n",rec_total_number);
        printf("rec_file_num = %d\n",rec_file_num);
        rec_fname_cnt = rec_file_num+1;

        if((rec_fname_cnt >= 9999)&&(rec_total_number >= 9999))//�Գ�����ʹ���ļ�����
        {
            goto _err_exit;
        }
    }
    else if(ret != FR_OK)//��ʼ��ʧ�ܣ��ͷ���Դ
    {
		if((emu_lg_dev < 2)&&(dev_next!=0))
		{
			printf("rec_folder_creater fail,nex_dev\n");
			emu_lg_dev ++; 
			new_lg_dev_let = dev_next;
			goto _file_init;
		}
        printf("rec_folder_creater fail,ret = %d\n",ret);
        goto _err_exit;
    }
    else
    {
        printf("rec_folder_creater succ\n");
        rec_fname_cnt = 0;       //�ļ��в����ڣ��ļ�������
    }

    puts("----enc_run_creat----\n");
    enc_run_creat();        //����¼���߳�

    puts("----rec_enc_init----\n");
    if(mode == ENC_MIC_CHANNEL)
    {
#if REC_FILE
        rec_op_api->rec_ctl = init_enc(rec_op_api, mode, enc_bitrate_h[7], LADC_SR48000, MP2_FORMAT); //����¼����Դ������¼��
#else
        rec_op_api->rec_ctl = init_enc(rec_op_api, mode, 1024, LADC_SR48000, ADPCM_FORMAT); //����¼����Դ������¼��
#endif
    }
    else if(mode == ENC_FM_CHANNEL)
    {
        ///FM¼������32K������
#if REC_FILE
        rec_op_api->rec_ctl = init_enc(rec_op_api, mode, enc_bitrate_h[7], LADC_SR32000, MP2_FORMAT); //����¼����Դ������¼��
#else
        rec_op_api->rec_ctl = init_enc(rec_op_api, mode, 1024, LADC_SR32000, ADPCM_FORMAT); //����¼����Դ������¼��
#endif
    }
    else if(mode == ENC_LINE_LR_CHANNEL)
    {
        printf("--------------ENC_ENC_LINE_LR_CHANNEL\n");
#if REC_FILE
        rec_op_api->rec_ctl = init_enc(rec_op_api, mode, enc_bitrate_h[7], LADC_SR48000, MP2_FORMAT); //����¼����Դ������¼��
#else
        rec_op_api->rec_ctl = init_enc(rec_op_api, mode, 1024, LADC_SR48000, ADPCM_FORMAT); //����¼����Դ������¼��
#endif
    }
    else if(mode == ENC_BT_CHANNEL)
    {
        ///����ͨ��¼������8K������
#if REC_FILE
        rec_op_api->rec_ctl = init_enc(rec_op_api, mode, enc_bitrate_l[5], LADC_SR8000, MP2_FORMAT); //����¼����Դ������¼��
#else
        rec_op_api->rec_ctl = init_enc(rec_op_api, mode, 1024, LADC_SR8000, ADPCM_FORMAT); //����¼����Դ������¼��
#endif
    }
    g_rec_ctl = rec_op_api->rec_ctl;

    printf("----rec_op_api %x----\n",rec_op_api);
    return rec_op_api;

_err_exit:
    printf("----err_exit----\n");
    encode_fs_close(rec_op_api);
    printf("----encode_fs_close----\n");
    free_fop(rec_op_api);
    printf("----free_fop----\n");
    free(rec_op_api);
    printf("----return NULL;----\n");
    return NULL;
}

static void start_rec(RECORD_OP_API ** rec_api)
{
    RECORD_OP_API * rec_op_api;
    s16 ret;
    u16 err_cnt;
    u32 tmp;
    char rec_file_name_temp[sizeof(rec_file_name)];

    if(!rec_api)
        return;

    memcpy(rec_file_name_temp, rec_file_name, sizeof(rec_file_name));

    rec_op_api = *rec_api;
    err_cnt = 0;    //err_reset

_get_file_name:
    if(rec_fname_cnt > 9999)
    {
        rec_fname_cnt = 0;
    }

    tmp = rec_fname_cnt/1000;
    rec_file_name_temp[12] = (char)(tmp+'0');

    tmp = rec_fname_cnt%1000/100;
    rec_file_name_temp[13] = (char)(tmp+'0');

    tmp = rec_fname_cnt%100/10;
    rec_file_name_temp[14] = (char)(tmp+'0');

    tmp = rec_fname_cnt%10;
    rec_file_name_temp[15] = (char)(tmp+'0');

    printf("--encode_file_open %s--\n",rec_file_name_temp);
    ret = encode_file_open(rec_op_api,rec_file_name_temp,FA_CREATE_NEW|FA_WRITE);  //��ʼ���ļ�
    if(FR_OK == ret)//�ļ������ɹ�
    {
        rec_op_api->rec_ctl->curr_device = (u8)file_operate_ctl(FOP_GET_PHYDEV_INFO,rec_op_api->fop_api,0,0);
//        music_ui.ui_curr_device = (u32)file_operate_ctl(FOP_GET_PHYDEV_INFO,rec_op_api->fop_api,0,0);
//        printf("--start_rec-- device %d--- \n",music_ui.ui_curr_device);
        rec_op_api->rec_ctl->file_info.file_del_size = 1; ///0:���Զ�ɾ���ļ� , ��0:ɾ���ļ� ɾ���ɹ��Զ���0
        os_taskq_post_msg(ENC_RUN_TASK_NAME, 1, rec_op_api); //����¼��
        while(rec_op_api->rec_ctl->enable != ENC_STAR)//�ȴ�¼�������߳�
        {
            os_time_dly(1);
        }
        UI_menu(MENU_REFRESH);
    }
    else if(FR_EXIST == ret)////�ļ��Ѿ�����
    {
        printf("--%s__FR_EXIST--\n", rec_file_name_temp);

        err_cnt++;
        if(err_cnt >= 9999)
        {
            printf("file_open err 9999\n");
            exit_rec_api(rec_api); //ֹͣ¼�����ͷ���Դ
        }

        rec_fname_cnt++;
        goto _get_file_name;
    }
    else//��������
    {
        printf("file_open ret :%x\n",ret);
        exit_rec_api(rec_api); //ֹͣ¼�����ͷ���Դ
    }
}


/*----------------------------------------------------------------------------*/
/**@brief  RECORD ��Ϣ��������
   @param  rec_api_p��record ����ָ��
   @param  msg��record ��Ϣ
   @return ��
   @note   void rec_msg_deal(RECORD_OP_API **rec_api_p, int *msg)
*/
/*----------------------------------------------------------------------------*/
void rec_msg_deal(RECORD_OP_API **rec_api_p, int *msg)
{
    switch(msg[0])
    {
    case MSG_REC_START:
        puts("MSG_REC_START\n");
        if(*rec_api_p == NULL)
        {
            printf("curr_task_name %s\n", curr_task->name);
			
			if(unlikely(0))
				;
#if MIC_REC_EN
            else if(compare_task_name(REC_TASK_NAME))
                (*rec_api_p) = rec_init(ENC_MIC_CHANNEL);
#endif

#if FM_REC_EN
            else if(compare_task_name(FM_TASK_NAME))
			{
                (*rec_api_p) = rec_init(ENC_FM_CHANNEL);
			}
#endif

#if AUX_REC_EN
            else if(compare_task_name(LINEIN_TASK_NAME))
                (*rec_api_p) = rec_init(ENC_LINE_LR_CHANNEL);
#endif

#if BT_REC_EN
            else if(compare_task_name("BtStackTask"))
			{
				/* bt_rec_buf_init(); */
                (*rec_api_p) = rec_init(ENC_BT_CHANNEL);
			}
#endif

            else
                (*rec_api_p) = NULL;//err
        }

        if(*rec_api_p)
        {
            if((*rec_api_p)->rec_ctl->enable == ENC_STOP)
            {
                puts("start_rec\n");
#if BT_REC_EN
				if(compare_task_name("BtStackTask"))
				{
					bt_rec_buf_init();
				}
				else
#endif
#if FM_REC_EN
					if(compare_task_name(FM_TASK_NAME))
					{
						set_sys_freq(FM_REC_Hz);
					}
#endif

                SET_UI_REC_OPT(UI_REC_OPT_START);
                start_rec(rec_api_p); //����¼��
            }
        }
        UI_menu(MENU_REFRESH);
        break;


    case MSG_REC_STOP:
        puts("--REC_MSG_REC_STOP\n");
        if(*rec_api_p)
        {
#if 0	//ֹͣ¼�������ͷ�¼����Դ
			stop_rec(*rec_api_p); 
            (*rec_api_p)->rec_ctl->lost_frame.black_lost_frame = 0;
            (*rec_api_p)->rec_ctl->lost_frame.front_lost_frame = 0;
#else	//ֹͣ¼���ͷ�����¼����Դ
			exit_rec_deal(rec_api_p); //ֹͣ¼�����ͷ���Դ
#endif

            rec_fname_cnt++;//¼���ļ������
        }
//            UI_menu(MENU_MAIN);
        SET_UI_REC_OPT(UI_REC_OPT_STOP);
        UI_menu(MENU_REFRESH);
        break;

    case MSG_REC_PP:
        if(*rec_api_p)
        {
            if((*rec_api_p)->rec_ctl->enable == ENC_STAR)
            {
                (*rec_api_p)->rec_ctl->enable = ENC_PAUS;
                puts("enc_pause\n");
                SET_UI_REC_OPT(UI_REC_OPT_PAUSE);
            }
            else if((*rec_api_p)->rec_ctl->enable == ENC_PAUS)
            {
                (*rec_api_p)->rec_ctl->enable = ENC_STAR;
                puts("enc_star\n");
                SET_UI_REC_OPT(UI_REC_OPT_START);
            }
            else
            {
                puts("--MSG_ENCODE_PP_err\n");
            }
        }
        UI_menu(MENU_REFRESH);
        break;

    case MSG_ENCODE_ERR:
		enc_puts("--REC_MSG_ENCODE_ERR\n");
		exit_rec_api(rec_api_p); //ֹͣ¼�����ͷ���Դ
        SET_UI_REC_OPT(UI_REC_OPT_ERR);
        UI_menu(MENU_REFRESH);
        break;

    case SYS_EVENT_DEV_OFFLINE:
        enc_puts("--enc_SYS_EVENT_DEV_OFFLINE--\n");
        if(*rec_api_p)
        {
            enc_puts("DEV_OFFLINE-00\n");
            if((*rec_api_p)->fop_api->cur_lgdev_info->lg_hdl->phydev_item == (void *)msg[1])
            {
                enc_puts("DEV_OFFLINE-01\n");

                /* if((*rec_api_p)->rec_ctl->enable != ENC_STOP) */
                /* { */
                    /* (*rec_api_p)->rec_ctl->enable = ENC_STOP; */
                    /* SET_UI_REC_OPT(UI_REC_OPT_ERR); */
                /* } */

                /* UI_menu(MENU_REFRESH); */

			    SET_UI_REC_OPT(UI_REC_OPT_ERR);
				UI_menu(MENU_REFRESH);
                exit_rec_api(rec_api_p); //ֹͣ¼�����ͷ���Դ
#if BT_REC_EN
				if(compare_task_name("BtStackTask"))
				{
					bt_rec_buf_release();
				}
#endif
            }
            else
            {
                ///<�ǽ����豸��ɾ�����߼��豸
                enc_puts("DEV_OFFLINE-02\n");
                break;
            }
        }
        else
        {
            UI_menu(MENU_REFRESH);
        }

        break;

    default:
        break;
    }
}

void exit_rec_deal(void *p)
{
	exit_rec(p);

#if BT_REC_EN
	if(compare_task_name("BtStackTask"))
	{
		bt_rec_buf_release();
	}
#endif

	set_sys_freq(SYS_Hz);
    SET_UI_REC_OPT(UI_REC_OPT_STOP);
}

/*----------------------------------------------------------------------------*/
/**@brief  RECORD ��������
   @param  p��������������ָ��
   @return ��
   @note   static void rec_task(void *p)
*/
/*----------------------------------------------------------------------------*/
static void rec_task(void *p)
{
    int msg[3];
    u32 tone_flag = 0;

    puts("\n************************RECORD TASK********************\n");

    tone_play_by_name(REC_TASK_NAME, 1,BPF_REC_MP3); //RECORDģʽ��ʾ������

    while(1)
    {
        memset(msg, 0x00, sizeof(msg));
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);
	    if(MSG_HALF_SECOND != msg[0]){printf("rec_task get_msg, 0x%x\n", msg[0]);}
        clear_wdt();
        switch(msg[0])
        {
        case SYS_EVENT_DEL_TASK:
            enc_puts("RECORD_SYS_EVENT_DEL_TASK\n");
            if (os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
            {
//                UI_menu(MENU_WAIT);
                ui_close_rec();
                exit_rec_deal(&rec_mic_api); //ֹͣ¼�����ͷ���Դ
                play_sel_stop();   //�ر���ʾ��
                dac_channel_off(DAC_DIGITAL_AVOL, FADE_ON);

                os_task_del_res_name(OS_TASK_SELF);
            }
            break;

        case SYS_EVENT_PLAY_SEL_END: //��ʾ������
            enc_puts("RECORD_SYS_EVENT_PLAY_SEL_END\n");
            play_sel_stop(); //�ر���ʾ��
            tone_flag = 1;

        case MSG_REC_INIT:
            enc_puts("MSG_REC_INIT\n");
            dac_channel_on(DAC_DIGITAL_AVOL, FADE_ON);
            ui_open_rec(&rec_mic_api, sizeof(RECORD_OP_API**));
            break;

#if LCD_SUPPORT_MENU
        case MSG_ENTER_MENULIST:
            UI_menu_arg(MENU_LIST_DISPLAY,UI_MENU_LIST_ITEM);
            break;
#endif

        case MSG_HALF_SECOND:
            if(updata_enc_time(rec_mic_api))
            {
                UI_menu(MENU_HALF_SEC_REFRESH);
            }
            break;

        default:
            if(tone_flag)
                rec_msg_deal(&rec_mic_api, msg); //record ����
            break;
        }
    }
}

/*----------------------------------------------------------------------------*/
/**@brief  ENCODE���񴴽�
   @param  priv��������������ָ��
   @return ��
   @note   static void encode_task_init(void *priv)
*/
/*----------------------------------------------------------------------------*/
static void rec_task_init(void *priv)
{
    u32 err;

    //��ʼ��ENCODE APP����
    err = os_task_create_ext(rec_task,
                             (void*)0,
                             TaskEncodePrio,
                             30
#if OS_TIME_SLICE_EN > 0
                             ,1
#endif
                             ,REC_TASK_NAME
                             ,1024*4
                            );

    //����ӳ��
    if(OS_NO_ERR == err)
    {
        key_msg_register(REC_TASK_NAME, adkey_msg_rec_table, iokey_msg_rec_table, irff00_msg_rec_table, NULL);
    }
}


/*----------------------------------------------------------------------------*/
/**@brief  ENCODE ����ɾ���˳�
   @param  ��
   @return ��
   @note   static void encode_task_exit(void)
*/
/*----------------------------------------------------------------------------*/
static void rec_task_exit(void)
{
    if (os_task_del_req(REC_TASK_NAME) != OS_TASK_NOT_EXIST)
    {
        os_taskq_post_event(REC_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
        do
        {
            OSTimeDly(1);
        }
        while(os_task_del_req(REC_TASK_NAME) != OS_TASK_NOT_EXIST);
        enc_puts("del_encode_task: succ\n");
    }
}


/*----------------------------------------------------------------------------*/
/**@brief  ENCODE ������Ϣ
   @note   const struct task_info rec_task_info
*/
/*----------------------------------------------------------------------------*/
TASK_REGISTER(rec_task_info) =
{
    .name = REC_TASK_NAME,
    .init = rec_task_init,
    .exit = rec_task_exit,
};
#endif
