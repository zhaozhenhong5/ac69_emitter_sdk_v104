/*
 *********************************************************************************************************
 *                                                AC51
 *                                      fs browser select
 *                                             CODE
 *
 *                          (c) Copyright 2015-2016, ZHUHAI JIELI
 *                                           All Rights Reserved
 *
 * File : *
 * By   : jamin.li
 * DATE : 2015-10-15 build this file
 *********************************************************************************************************
 */

#include "file_operate/file_bs_deal.h"
#include "uart.h"

#define FILE_BS_OPT_DBG
#ifdef  FILE_BS_OPT_DBG

    #define file_bs_puts     puts
    #define file_bs_printf   printf

#else
    #define file_bs_puts(...)
    #define file_bs_printf(...)

#endif

/*
 *********************************************************************************************************
 *
 * Description: ��ָ��������,����key(byte)��λ��
 *
 * Arguments  : buf - ָ��������ʼ��ַ, buf_len-����, key-������key,
 *              find_type -��������:0-��ǰ��������,1-�Ӻ���ǰ����
 *
 * Returns    : key��λ��
 *
 * Note:
 *********************************************************************************************************
 */
//����byte��λ��
int static find_byte_pos(u8 *buf, u16 buf_len, u8 key, u8 find_type)
{
    u8 *data = buf;

    if (0 == find_type)
    {
        //��ǰ������
        while (buf_len > 0)
        {
            if (*data == key)
            {
                return (int) (data - buf);
            }
            data++;
            buf_len--;
        }
    }
    else
    {
        //�Ӻ���ǰ��
        data += (buf_len - 1);
        while (buf_len > 0)
        {
            if (*data == key)
            {
                return (int) (data - buf);
            }
            data--;
            buf_len--;
        }
    }
    return -1;
}


/*
 *********************************************************************************************************
 *
 * Description: ת��·����ʽ
 *
 * Arguments  :
 *
 * Returns    : 0-Ϊ�ɹ�������ֵ ʧ��
 *
 * Note:
 *********************************************************************************************************
 */
int file_comm_change_file_path(char *dest,char *src)
{
    u8 start_flag = 0,space_flag,val;

    while(1)
    {
        val = *src;
        if(val == '\0')
        {
            *dest = '\0';
            break;
        }
        switch(val)
        {
            case '/':
                if(start_flag != 0)
                {
                    space_flag = 0;
                }
                start_flag++;
                *dest = val;
            break;

            case ' ':
                space_flag = 1;
            break;

            default:
                if(space_flag != 0)
                {
                    *dest = '.';
                    dest++;
                    space_flag = 0;
                }
                *dest = val;
            break;
        }

        dest++;
        src++;
    }

    return 0;
}

/*
 *********************************************************************************************************
 *
 * Description: ������fsȡ�ȵĳ���
 *
 * Arguments  : str,len
 *
 * Returns    : �ֽڳ���(������������)
 *
 * Note:    ����0xffffֵ���Զ����������
 *********************************************************************************************************
 */
int file_comm_long_name_fix(u8 *str,u16 len)
{
    u8 *src = str;
    while(len > 1)
    {
        if ((*str == 0xff) && (*(str + 1) == 0xff))
        {
            break;
        }

        if ((*str == 0x0) && (*(str + 1) == 0x0))
        {
            break;
        }

        str += 2;
        len-= 2;
    }
    *str = 0x00;
    *(str + 1) = 0x00;
    return (int) (str - src);
}

/*
 *********************************************************************************************************
 *
 * Description: ת��8+3������ʾ
 *
 * Arguments  : dest,src
 *
 * Returns    : ת�������ֵĳ���(������������)
 *
 * Note:      ת�����Զ����������
 *********************************************************************************************************
 */
int file_comm_display_83name(u8 *dest, u8 *src)
{
    u8 offset;

    for (offset = 8; offset > 0; offset--)
    {
        if (src[offset - 1] != 0x20)
        {
            break;
        }
    }

    memcpy(dest, src, offset);

    if(src[8] != 0x20)
    {
        dest[offset++] = '.';
        memcpy(dest + offset, src + 8, 3);
        offset += 3;
    }

    dest[offset++] =  '\0';

    return offset; //name 's len
}

/*
 *********************************************************************************************************
 *
 * Description: ��ȡ��ʾ���ļ��л��ļ�����
 *
 * Arguments  :tpath,disp_file_name,disp_dir_name
 *
 * Returns    :
 *
 * Note:      ת���� lfn_cnt Ϊ0Ϊ������lfn_cnt��Ϊ0 ���ǳ���
 *********************************************************************************************************
 */
void file_comm_change_display_name(char *tpath,LONG_FILE_NAME *disp_file_name,LONG_FILE_NAME *disp_dir_name)
{
    u16 len;
    int pos;

     //ȡ�ļ�������ʾ

    if(disp_file_name != NULL)
    {
        if(disp_file_name->lfn_cnt != 0)
        {
            //long name
            file_bs_puts("file long name  \n");
            //printf_buf(disp_file_name->lfn, 16);
            disp_file_name->lfn_cnt= file_comm_long_name_fix((void*)disp_file_name->lfn,disp_file_name->lfn_cnt );//���ӽ�����
        }
        else
        {
            //short name
            file_bs_puts("file short name\n");
            len = strlen((void *)tpath); //���ӽ�����
            pos = find_byte_pos((void *)tpath,len,0x2f,1);//
            if(pos == -1)
            {
                strcpy(disp_file_name->lfn,"----");
            }
            else
            {
                memcpy((void*)&disp_file_name->lfn[32],tpath + pos + 1,11);
                file_comm_display_83name((void*)disp_file_name->lfn,(void*)&disp_file_name->lfn[32]);
            }

            disp_file_name->lfn_cnt = 0;
        }

        //printf_buf(disp_file_name->lfn, 32);

    }


    //ȡ�ļ���������ʾ
    if(disp_dir_name != NULL)
    {
        if(disp_dir_name->lfn_cnt != 0)
        {
            //long name
            file_bs_puts("folder long name \n");
            //printf_buf(disp_dir_name->lfn, 16);
            disp_dir_name->lfn_cnt= file_comm_long_name_fix((void*)&disp_dir_name->lfn,disp_dir_name->lfn_cnt);//���ӽ�����
        }
        else
        {
            //short name
            file_bs_puts("folder short name \n");
            len = strlen((void *)tpath); //���ӽ�����
            pos = find_byte_pos((void *)tpath,len,0x2f,1);//

            if(pos != -1)
            {
                pos = find_byte_pos((void *)tpath,pos,0x2f,1);//
            }

            if(pos == -1)
            {
                strcpy(disp_dir_name->lfn,"ROOT");
            }
            else
            {
                memcpy(&disp_dir_name->lfn[32],tpath + pos + 1,11);
                file_comm_display_83name((void*)disp_dir_name->lfn,(void*)&disp_dir_name->lfn[32]);
            }
            disp_dir_name->lfn_cnt = 0;
        }

        //printf_buf(disp_dir_name->lfn, 32);
    }
}
/*
 *********************************************************************************************************
 *
 * Description: ���ļ������
 *
 * Arguments  : m_op_api
 *
 * Returns    : dir������
 *
 * Note:
 *********************************************************************************************************
 */
//���ظ�Ŀ¼dir����
extern void ui_ext_setting(const char *str);
extern u32 lg_dev_open_dir(void *parm,void * dir_info, void *file_type);
u32 file_bs_open_handle(MUSIC_OP_API *m_op_api,u8 *ext_name)
{
    u32 total_dir;

    if(m_op_api == NULL)
   {
        puts("*open hdl is null\n");
        return 0;
   }

    file_bs_puts("open bs hdl\n");
    file_bs_printf("bs_mapi:0x%x\n",(int)m_op_api);

    if(ext_name != NULL)
    {
        ui_ext_setting((void*)ext_name);
    }
    else
    {
        ui_ext_setting("WAVWMAMP3FLA");
    }


    //ui_ext_setting("MP3");

    total_dir = lg_dev_open_dir(m_op_api->fop_api->cur_lgdev_info,NULL,0);
    if(m_op_api->fop_api->cur_lgdev_info->lg_hdl->ui_fhdl == 0)
    {
        file_bs_puts("open bs fail\n");
        return 0;
    }
    file_bs_printf("open bs hdl_out,root_dir = 0x%x\n",total_dir);
    return total_dir;
}

/*
 *********************************************************************************************************
 *
 * Description: �ر��ļ������
 *
 * Arguments  : m_op_api
 *
 * Returns    :
 *
 * Note:
 *********************************************************************************************************
 */
void file_bs_close_handle(MUSIC_OP_API *m_op_api)
{
    file_bs_puts("close bs hdl\n");
    fs_file_close(m_op_api->fop_api->cur_lgdev_info->lg_hdl->fs_hdl,&m_op_api->fop_api->cur_lgdev_info->lg_hdl->ui_fhdl);
    file_bs_puts("close bs hdl_out\n");
}

/*
 *********************************************************************************************************
 *
 * Description: �����ļ���Ŀ¼
 *
 * Arguments  : m_op_api,dir_info
 *
 * Returns    : dir������
 *
 * Note:
 *********************************************************************************************************
 */
u32 file_bs_entern_dir(MUSIC_OP_API *m_op_api,FS_DIR_INFO *dir_info)
{
    u32 total_dir;
    file_bs_printf("bs_enter dir\n");
    total_dir = lg_dev_open_dir(m_op_api->fop_api->cur_lgdev_info,dir_info,0);//
    file_bs_printf("bs_enter_out,t = 0x%x\n",total_dir);
    return total_dir;
}

/*
 *********************************************************************************************************
 *
 * Description: �����ϴ�Ŀ¼
 *
 * Arguments  : m_op_api
 *
 * Returns    : dir������
 *
 * Note:
 *********************************************************************************************************
 */
 extern u32 lg_dev_exit_dir(void *parm);
u32 file_bs_exit_dir(MUSIC_OP_API *m_op_api)
{
    u32 total_dir;
    file_bs_printf("bs_exit dir\n");
    total_dir =  lg_dev_exit_dir(m_op_api->fop_api->cur_lgdev_info);//
    file_bs_printf("bs_exit_out,total = 0x%x\n",total_dir);
    return total_dir;
}

/*
 *********************************************************************************************************
 *
 * Description: ��ȡָ��λ�õ�dir��Ϣ
 *
 * Arguments  : m_op_api,buf--�������buf,start_sn -��ȡ����ʵλ�ã���1��ʼ��,get_cnt--��ȡdir�ĸ���
 *
 * Returns    : ������ȡ����dir����
 *
 * Note:
 *********************************************************************************************************
 */
 extern u32 lg_dev_get_dir(void *parm,u32 start_num, u32 total_num, void *buf_info);
u32 file_bs_get_dir_info(MUSIC_OP_API *m_op_api,FS_DIR_INFO *buf,u16 start_sn,u16 get_cnt)
{
    u32 real_dir;
    file_bs_printf("bs_get dir_info\n");
    real_dir =  lg_dev_get_dir(m_op_api->fop_api->cur_lgdev_info,start_sn,get_cnt,buf);
    file_bs_printf("bs_get_out,t = 0x%x\n",real_dir);
    return real_dir;
}




