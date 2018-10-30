#ifndef __TASK_MANAGE__
#define __TASK_MANAGE__

#include "typedef.h"

typedef enum
{
    RUN_TASK_SUCC = 0,          ///<��������ɹ�
    RUN_TASK_ERR_NO_TAG,        ///<û��ָ������ʲô����
    RUN_TASK_ERR_HAS_RPT,       ///<ָ���������Ѿ���������״̬
}RUN_TASK_STATUS;


typedef enum
{
    SWITCH_SPEC_TASK =0,        ///<ָ��task
    SWITCH_NEXT_TASK ,          ///<��һtask
    SWITCH_PRE_TASK,            ///<��һtask
    SWITCH_LAST_TASK,            ///<ǰһ��һtask
}TASK_SWITCH_MODE;

struct task_info
{
	const char *name;
	void (*init)(void *priv);
	void (*exit)();
};

struct task_dev_info
{
	const char *name;
	u32 (*dev_check)(void);
};

// #define  TASK_REGISTER(task) \
	// const struct task_info task __attribute__((section(".task_info")))
#define  TASK_REGISTER(task) \
	const struct task_info task sec(.task_info)


extern struct task_info *curr_task;

RUN_TASK_STATUS task_switch(char *task_name,void *priv,TASK_SWITCH_MODE cur_mode);
u32 compare_task_name(char *task_name);
int msg_mask_off_in_bt();
s8 task_create(char *task_name);
void os_task_delete(char *task_name);

#endif
