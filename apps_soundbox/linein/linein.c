#include "common/app_cfg.h"
#include "rtos/os_api.h"
#include "rtos/os_cfg.h"
#include "common/error.h"
#include "common/msg.h"
#include "rtos/task_manage.h"
#include "linein_key.h"
#include "linein.h"
#include "dac/dac_api.h"
#include "play_sel/play_sel.h"
#include "key_drv/key.h"
#include "ui/led/led.h"
#include "dac/dac_api.h"
#include "dev_linein.h"
#include "ui/ui_api.h"
#include "dac/ladc.h"
#include "linein_prompt.h"
#include "linein_ui.h"
#include "echo/echo_api.h"
#include "echo/echo_deal.h"
#include "record.h"
#include "encode/encode.h"
#include "key_drv/key_voice.h"
#include "dac/dac.h"
#include "dac/dac_api.h"
#include "rcsp/rcsp_interface.h"
#include "dac/eq.h"

extern void *malloc_fun(void *ptr, u32 len, char *info);
extern void free_fun(void **ptr);
extern u8 eq_mode;


RECORD_OP_API * aux_rec = NULL;
REVERB_API_STRUCT *aux_reverb = NULL;
#define get_mute_status   is_dac_mute


/*----------------------------------------------------------------------------*/
/**@brief  AUX DACͨ��ѡ�񣬿���
 @param  ��
 @return ��
 @note   void aux_dac_channel_on(void)
 */
/*----------------------------------------------------------------------------*/
void aux_dac_channel_on(void)
{
	if(LINEIN_CHANNEL == DAC_AMUX0) {
    	JL_PORTA->DIR |=  (BIT(1)|BIT(2));
    	JL_PORTA->DIE &= ~(BIT(1)|BIT(2));
	}
	else if(LINEIN_CHANNEL == DAC_AMUX1) {
    	JL_PORTA->DIR |=  (BIT(4)|BIT(3));
    	JL_PORTA->DIE &= ~(BIT(4)|BIT(3));
		JL_PORTA->PD  &= ~(BIT(3));//PA3 default pull_down 
	}
	else if(LINEIN_CHANNEL == DAC_AMUX2) {
    	JL_PORTB->DIR |=  (BIT(11)|BIT(12));
    	JL_PORTB->DIE &= ~(BIT(11)|BIT(12));
	} 

#if AUX_AD_ENABLE
    dac_channel_on(MUSIC_CHANNEL, FADE_ON);
	set_sys_vol(0,0,FADE_ON);
    dac_set_samplerate(48000,0);
    ladc_reg_init(ENC_LINE_LR_CHANNEL , LADC_SR48000);
	amux_channel_en(LINEIN_CHANNEL,1);
	os_time_dly(35);
	set_sys_vol(dac_ctl.sys_vol_l,dac_ctl.sys_vol_r,FADE_ON);
#else
    dac_channel_on(LINEIN_CHANNEL, FADE_ON);
    os_time_dly(20);//wait amux channel capacitance charge ok
	set_sys_vol(dac_ctl.sys_vol_l,dac_ctl.sys_vol_r,FADE_ON);
	#if ECHO_EN
	///����dac����ͨ��
	dac_trim_en(0);
	dac_digital_en(1);
	#else
	///�ر�dac����ͨ��
	dac_trim_en(1);
	dac_digital_en(0);
	#endif//end of ECHO_EN
#endif//end of AUX_AD_ENABLE
}

/*----------------------------------------------------------------------------*/
/**@brief  AUX DACͨ��ѡ�񣬹ر�
 @param  ��
 @return ��
 @note   void aux_dac_channel_off(void)
 */
/*----------------------------------------------------------------------------*/
void aux_dac_channel_off(void)
{
#if AUX_AD_ENABLE
	dac_channel_off(MUSIC_CHANNEL,FADE_ON);
	ladc_close(ENC_LINE_LR_CHANNEL);
	amux_channel_en(LINEIN_CHANNEL,0);
#else
    dac_channel_off(LINEIN_CHANNEL, FADE_ON);
	dac_trim_en(0);
	/* dac_mute(0, 1); */
#endif
}

void aux_info_init(void)
{
    aux_dac_channel_on();

    //#if (AUX_REC_EN==0)
    //    ladc_reg_init(ENC_DAC_CHANNEL,LADC_SR16000);   //��ȡ����Ҫ��ʼ�����ڲ���
    //#endif

	ui_open_aux(&aux_rec,sizeof(RECORD_OP_API **));
    /* ui_open_aux(0, 0); */
}

/*----------------------------------------------------------------------------*/
/**@brief  LINE IN ����
 @param  p��������������ָ��
 @return ��
 @note   static void linein_task(void *p)
 */
/*----------------------------------------------------------------------------*/
static void linein_task(void *p)
{
    int msg[6];
    aux_reverb = NULL;
    tbool psel_enable;
	u8 line_do_mute_flag = 0;

    aux_puts("\n************************LINEIN TASK********************\n");
    malloc_stats();
    led_fre_set(15,0);

    psel_enable = tone_play_by_name(LINEIN_TASK_NAME,1, BPF_LINEIN_MP3); //line in��ʾ������

#if (AUX_AD_ENABLE && EQ_EN)
	eq_enable();
    eq_mode = get_eq_default_mode();
#endif

    while (1)
    {
        memset(msg, 0x00, sizeof(msg));
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);

        if(psel_enable)
		{
            switch(msg[0])
            {
            case SYS_EVENT_PLAY_SEL_END:
			case SYS_EVENT_DEL_TASK: 				//����ɾ��music����
                puts("\n----play_sel_stop------\n");
                play_sel_stop();
                psel_enable = 0;
                break;
            default:
                msg[0] = NO_MSG;
                break;
            }
		}

        if(msg[0] != MSG_HALF_SECOND)
        {
            printf("\nlinein_msg= 0x%x\n",msg[0]);
        }

        clear_wdt();
#if SUPPORT_APP_RCSP_EN
		rcsp_linein_task_msg_deal_before(msg);
#endif
		switch (msg[0])
        {
            case SYS_EVENT_DEL_TASK:
	            if (os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
	            {
	                //				    UI_menu(MENU_WAIT);
	                aux_puts("AUX_SYS_EVENT_DEL_TASK\n");
	                ui_close_aux();

	                //                  exit_rec_api(&rec_linin_api); //ֹͣ¼�����ͷ���Դ
	                echo_exit_api((void **)&aux_reverb);
                	exit_rec_api(&aux_rec); //ֹͣ¼�����ͷ���Դ
	                //				    disable_ladc(ENC_DAC_CHANNEL);
	                aux_dac_channel_off();
	                if(line_do_mute_flag)
					{
						puts("line unmute\n");
						line_do_mute_flag = 0;
						dac_mute(0, 1);
					}
					os_task_del_res_name(OS_TASK_SELF);
	            }
	            break;

            case SYS_EVENT_PLAY_SEL_END: //��ʾ������
	            aux_puts("AUX_SYS_EVENT_PLAY_SEL_END\n");
	            if(linein_prompt_break)
	            {
	                linein_prompt_break = 0;
	                break;
	            }
	            aux_puts("AUX_play_tone_end\n");
            case MSG_AUX_INIT:
	            aux_puts("MSG_AUX_INIT\n");
	            aux_info_init();
	            break;

            case MSG_AUX_MUTE:

	            if (get_mute_status())
	            {
	                aux_puts("MSG_AUX_UNMUTE\n");
	                dac_mute(0, 1);
                    /* send_key_voice(500); */
	                led_fre_set(15,0);
				}
	            else
	            {
	                aux_puts("MSG_AUX_MUTE\n");
	                os_time_dly(30);
	                dac_mute(1, 1);
	                led_fre_set(0,0);
	            }
				line_do_mute_flag = 1; 
                UI_menu(MENU_REFRESH);
            	break;

#if AUX_AD_ENABLE
			case MSG_MUSIC_EQ:
				eq_mode++;
				if(eq_mode > 5)
					eq_mode = 0;
				printf("Aux_EQ:%d\n",eq_mode);
				eq_mode_sw(eq_mode);
				break;
#endif
			case MSG_PROMPT_PLAY:
            case MSG_LOW_POWER:
	            puts("aux_low_power\n");
				aux_dac_channel_off();
	            linein_prompt_play(BPF_LOW_POWER_MP3);
	            break;

#if LCD_SUPPORT_MENU
            case MSG_ENTER_MENULIST:
	            UI_menu_arg(MENU_LIST_DISPLAY,UI_MENU_LIST_ITEM);
	            break;
#endif

            case MSG_HALF_SECOND:
	            //aux_puts(" Aux_H ");
#if AUX_REC_EN
	            if(updata_enc_time(aux_rec))
	            {
	                UI_menu(MENU_HALF_SEC_REFRESH);
	            }
#endif

	            break;

            default:
				if(msg[0] == MSG_ECHO_START)//start echo
				{
                	exit_rec_api(&aux_rec); //stop rec
				}
				else if (msg[0] == MSG_REC_START)//start rec
				{
	                echo_exit_api((void **)&aux_reverb);//stop echo
				}
				echo_msg_deal_api((void **)&aux_reverb, msg);
	            rec_msg_deal_api(&aux_rec, msg);
	            break;
        }

#if SUPPORT_APP_RCSP_EN
		rcsp_linein_task_msg_deal_after(msg);
#endif
    }
}

/*----------------------------------------------------------------------------*/
/**@brief  LINE IN ���񴴽�
 @param  priv��������������ָ��
 @return ��
 @note   static void linein_task_init(void *priv)
 */
/*----------------------------------------------------------------------------*/
static void linein_task_init(void *priv)
{
    u32 err;
    err = os_task_create(linein_task, (void*) 0, TaskLineinPrio, 10
#if OS_TIME_SLICE_EN > 0
            ,1
#endif
            , LINEIN_TASK_NAME);

    if (OS_NO_ERR == err)
    {
        key_msg_register(LINEIN_TASK_NAME, linein_ad_table, linein_io_table, linein_ir_table, NULL);
    }
}

/*----------------------------------------------------------------------------*/
/**@brief  LINE IN ����ɾ���˳�
 @param  ��
 @return ��
 @note   static void linein_task_exit(void)
 */
/*----------------------------------------------------------------------------*/
static void linein_task_exit(void)
{
    if (os_task_del_req(LINEIN_TASK_NAME) != OS_TASK_NOT_EXIST)
    {
        os_taskq_post_event(LINEIN_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
        do
        {
            OSTimeDly(1);
        } while (os_task_del_req(LINEIN_TASK_NAME) != OS_TASK_NOT_EXIST);
        aux_puts("del_linein_task: succ\n");
    }
}

/*----------------------------------------------------------------------------*/
/**@brief  LINE IN ������Ϣ
 @note   const struct task_info linein_task_info
 */
/*----------------------------------------------------------------------------*/
TASK_REGISTER(linein_task_info) =
{
    .name = LINEIN_TASK_NAME,
    .init = linein_task_init,
    .exit = linein_task_exit,
};
