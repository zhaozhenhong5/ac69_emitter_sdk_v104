#include "lyrics_api.h"
#include "uicon/lyrics.h"
#include "fat/tff.h"
#include "fat/fs_io.h"
#include "ui/lcd/lcd_drv_interface.h"
#include "uicon/ui.h"

#if LRC_LYRICS_EN

extern u32 lg_dev_open_file_bysclust(void *parm,u32 *filenum,u32 spc_sclust);
extern u32 lg_dev_get_file_indir(void *parm, void *ext_name);

/*----------------------------------------------------------------------------*/
/**@brief ���÷������ٶ�
   @param lrc_len--��ǰ��ʳ��ȣ�time_gap-����һ����ʵļ����
   @param *roll_speed - �������ٶ� 500ms unit
   @return
   @note
 */
/*----------------------------------------------------------------------------*/
void lrc_roll_speed_ctrl(u8 lrc_len, u32 time_gap, u8 *roll_speed)
{
    ///��ҳ�����ٶȿ���
    ///printf("speed = %d,%d",lrc_len,time_gap);
    ///DVcTxt1_11 ��ʾ����Ϊ32 bytes
    if(lrc_len > (LRC_DISPLAY_TEXT_LEN*2))
    {
        *roll_speed = ((time_gap+2)/3)<<1;
    }
    else if(lrc_len > LRC_DISPLAY_TEXT_LEN)
    {
        *roll_speed = time_gap;
    }
    else
    {
        *roll_speed = 250; ///never load new page
    }

}

/*----------------------------------------------------------------------------*/
/**@brief  �����ʾ����
   @param
   @return
   @note
 */
/*----------------------------------------------------------------------------*/
void clr_lrc_disp_buff(void)
{
    lcd_clear_area_rect(4, 8, 0, 128);
}


/*----------------------------------------------------------------------------*/
/**@brief ���ģ���ʼ��
   @param
   @return 0--�ɹ�����0 ʧ��
   @note
 */
/*----------------------------------------------------------------------------*/
int lrc_init(void)
{
    LRC_CFG t_lrc_cfg;
    t_lrc_cfg.once_read_len = ONCE_READ_LENGTH;
    t_lrc_cfg.once_disp_len = ONCE_DIS_LENGTH;
    t_lrc_cfg.label_temp_buf_len = LABEL_TEMP_BUF_LEN;
    t_lrc_cfg.roll_speed_ctrl_cb = lrc_roll_speed_ctrl;
    t_lrc_cfg.clr_lrc_disp_cb = NULL;//clr_lrc_disp_buff;
    t_lrc_cfg.lrc_text_id = LRC_DISPLAY_TEXT_ID;
    t_lrc_cfg.read_next_lrc_flag = 0;

    return lrc_param_init(&t_lrc_cfg);
}

/*----------------------------------------------------------------------------*/
/**@brief ���ģ���˳�
   @param
   @return
   @note

 */
/*----------------------------------------------------------------------------*/
void lrc_exit(void)
{
    lrc_destroy();
}

/*----------------------------------------------------------------------------*/
/**@brief ������ʣ�����
   @param
   @return 0--�ɹ�����0 ʧ��
   @note
 */
/*----------------------------------------------------------------------------*/
int lrc_find(MUSIC_OP_API *m_op_api)
{
    u32 find_lrc_flag;
    void *lrc_file_hdl;

    find_lrc_flag = lg_dev_get_file_indir(m_op_api->fop_api->cur_lgdev_info,"LRC");//root

    if(0 == find_lrc_flag)
    {
        puts("\ncan't find lrc------\n");
        return -1;
    }

    lrc_file_hdl = m_op_api->fop_api->cur_lgdev_info->lg_hdl->lrc_fhdl;

    if(FALSE == lrc_analysis(lrc_file_hdl))
    {
        printf("analazy err \n");
        return -1;
    }
    return 0;
}

#endif
