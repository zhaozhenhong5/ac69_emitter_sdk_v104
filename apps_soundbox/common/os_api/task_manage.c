#include "sdk_cfg.h"
#include "rtos/task_manage.h"
#include "rtos/os_api.h"
#include "common/msg.h"
#include "linein/linein.h"
#include "usb_device/usb_device.h"
#include "fm/fm_radio.h"
#include "music/music.h"
#include "file_operate/file_op.h"
#include "dev_pc.h"
#include "linein/dev_linein.h"
#include "idle/idle.h"
#include "common/common.h"
#include "echo/echo.h"
#include "vm/vm.h"
#include "rcsp/rcsp_interface.h"
#include "emitter_user.h"

#if REC_EN
#include "encode/record.h"
#include "encode/encode.h"
#endif

struct task_info *curr_task; ///<��ǰ����ǰ̨���е�����ָ��
struct task_info *last_task_info;

extern const struct task_info rtc_task_info;
int check_return_false(void);
extern TASK_REGISTER(btstack_task_info);
extern TASK_REGISTER(bt_hid_task_info);
extern volatile u8 music_task_runing;

///�����л�
const struct task_info *task_list[] AT(.task_info);
const struct task_info *task_list[] =
{
    &btstack_task_info,

    #if BT_HID_INDEPENDENT_MODE
        &bt_hid_task_info,
    #endif

    &music_task_info,

#if FM_RADIO_EN
    &fm_radio_task_info,
#endif
    //
    &linein_task_info,
    //
#if USB_PC_EN
    &pc_task_info,
#endif
    //
#if RTC_CLK_EN
    &rtc_task_info, ///<RTC
#endif
    //
#if REC_EN
    &rec_task_info, ///<REC
#endif

#if ECHO_TASK_EN
    &echo_task_info, ///<ECHO
#endif
    //

#if RCSP_LIGHT_HW    
    &light_task_info,
#endif

    &idle_task_info,
};

const struct task_dev_info music_task_2_dev =
{
    .name = "MusicTask",
    .dev_check = file_operate_get_total_phydev,
};

const struct task_dev_info record_task_2_dev =
{
    .name = "RECTask",
    .dev_check = file_operate_get_total_phydev,
};

#if AUX_DETECT_EN
const struct task_dev_info linein_task_2_dev =
{
    .name = "LineinTask",
    .dev_check = aux_is_online,
};
#endif/*AUX_DETECT_EN*/


u32 app_usb_slave_online_status_null(void)
{
   return 0;
}



#if USB_PC_EN
const struct task_dev_info pc_task_2_dev =
{
    .name = "USBdevTask",
    .dev_check = app_usb_slave_online_status,
};
#endif/*USB_PC_EN*/

const struct task_dev_info *task_connect_dev[] AT(.task_info);
const struct task_dev_info *task_connect_dev[] =
{
    &music_task_2_dev,

#if AUX_DETECT_EN
    &linein_task_2_dev,
#endif/*AUX_DETECT_EN*/

#if USB_PC_EN
    &pc_task_2_dev,
#endif/*USB_PC_EN*/

#if REC_EN
    &record_task_2_dev,
#endif/*REC_EN*/

};
const int task_connect_dev_valsize = sizeof(task_connect_dev);

/*----------------------------------------------------------------------------*/
/**@brief  �����������֣���ȡ����ָ��
 @param  name����������
 @return ָ�����������ֵ�ָ�����NULL
 @note
 */
/*----------------------------------------------------------------------------*/
static u32 __get_task(const char *name)
{
    u32 max_cnt;
    u32 task_cnt;

    max_cnt = sizeof(task_list) / sizeof(int);

    for (task_cnt = 0; task_cnt < max_cnt; task_cnt++)
    {
        if (0 == strcmp(task_list[task_cnt]->name, name))
        {
            printf("get_task_cnt = %d\n", task_cnt);
            return task_cnt;
        }
    }
    return -1;
}

/*----------------------------------------------------------------------------*/
/**@brief  ���� 0
 @param
 @return
 @note   for debug
 */
/*----------------------------------------------------------------------------*/
int check_return_false(void)
{
    return 0;
}
/*----------------------------------------------------------------------------*/
/**@brief  ����������ʧ�ܵ�����
 @param  : task_cnt--��Ҫ�л�������id
 @return  ȷ�����е�����id
 @note  �����ǰ����������ʧ�ܻ��Զ��л�����һ������
 */
/*----------------------------------------------------------------------------*/
static u32 skip_task_without_dev(u32 *task_cnt)
{
    u8 i;
    u32 max_cnt;
    u32 dev_cnt;

    max_cnt = (sizeof(task_list) / sizeof(int)) - 1;
    dev_cnt = sizeof(task_connect_dev) / sizeof(int);
    for (i = 0; i < dev_cnt; i++)
    {
        if (!strcmp(task_list[*task_cnt]->name, task_connect_dev[i]->name))
        {
            if (!task_connect_dev[i]->dev_check())
            {
                (*task_cnt)++;
                if (*task_cnt >= max_cnt)
                {
                    *task_cnt = 0;
                }
                if (skip_task_without_dev(task_cnt))
                    return TRUE;
                else
                    return FALSE;
            }
            else
                return TRUE;
        }
    }

    if (i == dev_cnt)
        return TRUE;

    return TRUE;
}

/*----------------------------------------------------------------------------*/
/**@brief  ����ִ��
 @param  task_cnt:     ����idle
 @param  priv:     ����������Ĳ���
 @param  cur_mode���л���ʽ
 @return  �ɹ����ߴ�����Ϣ
 @note
 */
/*----------------------------------------------------------------------------*/
static RUN_TASK_STATUS run_task(u32 task_cnt, void *priv, TASK_SWITCH_MODE cur_mode)
{
    u32 max_cnt;

    max_cnt = (sizeof(task_list) / sizeof(int)) - 1;
    if (cur_mode == SWITCH_SPEC_TASK)
    {
        if (task_list[task_cnt] == curr_task) ///<ָ�����е������뵱ǰ�������е�����һ��
            return RUN_TASK_ERR_HAS_RPT;

    }
    else if (cur_mode == SWITCH_NEXT_TASK)
    {
        task_cnt++;
        if (task_cnt >= max_cnt)
        {
            task_cnt = 0;
        }
    }
    else if (cur_mode == SWITCH_PRE_TASK)
    {
        task_cnt--;
        if (task_cnt == 0)
        {
            task_cnt = max_cnt - 1;
        }
    }
    else if (cur_mode == SWITCH_LAST_TASK)
    {

    }

    skip_task_without_dev(&task_cnt); //�����豸�����ߵ�����
    printf("*****Next_Task_Name2:%s\n", task_list[task_cnt]->name);

    task_list[task_cnt]->init((void *) (task_list[task_cnt]->name));
    curr_task = (struct task_info *) task_list[task_cnt];

    return RUN_TASK_SUCC;
}

/*----------------------------------------------------------------------------*/
/**@brief  �����л�
 @param  task_name: ���������
 @param  priv:     ����������Ĳ���
 @param  cur_mode���л���ʽ
 @return  �ɹ����ߴ�����Ϣ
 @note
 */
/*----------------------------------------------------------------------------*/
RUN_TASK_STATUS task_switch(char *task_name, void *priv, TASK_SWITCH_MODE cur_mode)
{
    RUN_TASK_STATUS status;
    struct task_info *task;
    u32 task_cnt;

    //    GET_SYS_MEM_STAT();

    if (task_name != NULL)
    {
        task_cnt = __get_task(task_name);
        if (-1 == task_cnt)
        {
            puts("t_sw_1\n");
            return RUN_TASK_ERR_NO_TAG;
        }
        task = (struct task_info *) task_list[task_cnt];
        if (task == curr_task)
        {
            puts("t_sw_2\n");
            return RUN_TASK_ERR_HAS_RPT;
        }
    }
    else
    {
        if (curr_task != NULL)
        {
            task_cnt = __get_task(curr_task->name);
            if (-1 == task_cnt)
            {
                puts("t_sw_3\n");
                return RUN_TASK_ERR_NO_TAG;
            }
        }
        else
        {
            ///<��ǰû����������Ҳûָ����Ҫ���е�����
            return RUN_TASK_ERR_NO_TAG;
        }

    }
    puts("t_sw_4\n");

#if REC_EN
    //stop_rec(rec_op_api); //����¼��
#endif

    if (curr_task != NULL)
    {
        curr_task->exit(); //�˳���Ӧ����
        puts("\n\ncurr_task exit!!!\n\n");
        GET_SYS_MEM_STAT();
    }

    if (SWITCH_LAST_TASK == cur_mode)
    {
        puts("last mode\n");
        task_cnt = __get_task(last_task_info->name);
    }
    else
    {
        last_task_info = curr_task;
    }
    ///dac_mute(0,1);    //Ҫ��ȫ�˳�֮ǰ��ģʽ�ٽ�mute����Ȼ���ϸ�ģʽ��β��
    status = run_task(task_cnt, priv, cur_mode);

	vm_check_all(1);

#if SUPPORT_APP_RCSP_EN
	rcsp_app_start(curr_task->name);
#endif

	return status;
}

/*----------------------------------------------------------------------------*/
/**@brief 	create a task 
 @param  	task_name
 @return	-1,create task fail
 			 0,create task success
 @note
 */
/*----------------------------------------------------------------------------*/
s8 task_create(char *task_name)
{
	u32 task_cnt;
	if(strcmp(MUSIC_TASK_NAME,task_name) == 0)
	{
		if(file_operate_get_total_phydev() == 0) {
			return -1;
		}
		else {

		}
	}

	if(task_name)
	{
		task_cnt = __get_task(task_name);
		if(task_cnt == -1) {
			return -1;
		}
    	task_list[task_cnt]->init((void *) (task_list[task_cnt]->name));
		return 0;
	}
	return -1;
}

/*----------------------------------------------------------------------------*/
/**@brief  ɾ��һ������
 @param  task_name��ɾ�����������
 @return
 @note
 */
/*----------------------------------------------------------------------------*/
void os_task_delete(char *task_name)
{
    if (os_task_del_req(task_name) != OS_TASK_NOT_EXIST)
    {
        os_taskq_post_event(task_name, 1, SYS_EVENT_DEL_TASK);
        do
        {
            OSTimeDly(1);
        }
        while (os_task_del_req(task_name) != OS_TASK_NOT_EXIST);
        printf("delete task succ =%s \n", task_name);
		if((strcmp(MUSIC_TASK_NAME,task_name) == 0) || (strcmp(PC_TASK_NAME,task_name) == 0))
		{
			music_task_runing = 0;
			set_emitter_decode_media(0);
		}
    }
}

/*----------------------------------------------------------------------------*/
/**@brief  ����������Ϊ��ɾ��״̬
 @return
 @note   �˺����ɱ�ɾ���������Լ��������ã����ͷ���ռ�õ���Դ�󣬵��ô�
 ����������˺����󣬲��ٷ��أ�����������������
 */
/*----------------------------------------------------------------------------*/
void os_task_ready_delete()
{
    os_task_del_res(OS_TASK_SELF);
}

/*----------------------------------------------------------------------------*/
/**@brief  �Ƚ��Ƿ�Ϊ��ǰ���е�����
 @param    task_name: �Ƚ����������
 @return   1-yes��0-no
 @note
 */
/*----------------------------------------------------------------------------*/
u32 compare_task_name(char *task_name)
{
    if (task_name != NULL)
    {
        if(0 == strcmp(task_name,curr_task->name))
        {
            puts("is curr_task\n");
            return 1;
        }
    }
    puts("not curr_task\n");
    return 0;
}
