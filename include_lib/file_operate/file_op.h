#ifndef __FILE_OP_H__
#define __FILE_OP_H__

#include "file_op_err.h"
#include "logic_dev_list.h"
//------file op cmd------------------
typedef enum
{
    FOP_INIT=0,
    FOP_FIND_UPDATE_FILE=1,
    FOP_GET_TOTALFILE_NUM,
    FOP_OPEN_FILE,
    FOP_OPEN_FILE_BYPATH,
    FOP_OPEN_FILE_BYSCLUCT,
    FOP_READ_BPINFO,
    FOP_SAVE_BPINFO,
	FOP_CLEAR_BPINFO,
	FOP_READ_FLACBPINFO,
    FOP_SAVE_FLACBPINFO,
    FOP_SAVE_LASTOP_DEV,
    FOP_FIND_SPEC_LGDEV,
    FOP_ALLFILE_ERR_LGDEV,
    FOP_DEV_STATUS,
    FOP_CLOSE_LOGDEV,
    FOP_GET_PHYDEV_INFO,
    FOP_GET_FOLDER_INFO,
    FOP_GET_FILE_NAME,
    FOP_MAX,
}ENUM_FILE_CMD;

//------play mode------------------
typedef enum
{
    REPEAT_ALL = FOP_MAX+1,                   ///<ȫ��ѭ��
    REPEAT_ONE_LGDEV,                       ///<�����߼��豸ѭ��
    REPEAT_ONE,                             ///<����ѭ��
    REPEAT_RANDOM,                          ///<���豸�������
    REPEAT_FOLDER,                          ///<�ļ���ѭ��
    
//    REPEAT_ONE_PHDEV,                       ///<���������豸ѭ��
    MAX_PLAY_MODE,
}ENUM_PLAY_MODE;

//-----file sel mode----------------
typedef enum							//�����ļ��ķ�ʽ����
{
    PLAY_NEXT_FILE = MAX_PLAY_MODE+1,       ///<������һ��
    PLAY_AUTO_NEXT,                         ///<������һ�ף����Ž���ʱ���Զ���һ�׿���
    PLAY_FIRST_FILE,  		                ///<�ӵ�һ���ļ�����
    PLAY_BREAK_POINT,                       ///<�Ӽ����ļ���λ�ÿ�ʼ����
    PLAY_LAST_FILE,				            ///<�����һ���ļ�����
    PLAY_PREV_FILE,                         ///<������һ��
    PLAY_SPEC_FILE,                         ///<����ָ���ļ����
    PLAY_FILE_BYPATH   ,                    ///<��������ļ�����·��(���ļ���ʱ���򿪴�·���µĵ�һ���ļ�)
    PLAY_NEXT_FOLDER,                       ///<��һ���ļ���
    PLAY_PRE_FOLDER,                        ///<��һ���ļ���
	PLAY_AB_REPEAT,							///AB�ϵ�
    MAX_FILE_SEL_MODE,
} ENUM_FILE_SELECT_MODE;

//-----dev sel mode-----------------
typedef enum
{
    DEV_SEL_CUR = MAX_FILE_SEL_MODE+1,   ///<��ǰ��豸
    DEV_SEL_NEXT,                        ///<��һ���豸
    DEV_SEL_PREV,                        ///<��һ���豸
    DEV_SEL_FIRST,                       ///<��һ���豸
    DEV_SEL_LAST,                        ///<���һ���豸
    DEV_SEL_SPEC,                        ///<ָ���豸
    MAX_DEV_MODE, //0x26
}ENUM_DEV_SEL_MODE;

typedef struct _FILE_OPERATE_INIT_
{
    ENUM_DEV_SEL_MODE      cur_dev_mode;        ///<�豸ѡ��ģʽ
    ENUM_PLAY_MODE         cur_play_mode;       ///<�ļ�����ģʽ
    ENUM_FILE_SELECT_MODE  cur_sel_mode;        ///<�ļ�ѡ��ģʽ
    u32                    dev_let;             ///<ָ���豸
    u32 *                  filenum;             ///<�ļ����
    u8 *                   filepath;            ///<�ļ�·��
    u32                    filesclust;          ///<�ļ���
}FILE_OPERATE_INIT;

typedef struct _FILE_OPERATE_
{
    FILE_OPERATE_INIT *fop_init;        ///<�ļ������������ļ�����
    lg_dev_info *cur_lgdev_info;        ///<��ǰ�ļ�����������Ӧ���߼��豸��Ϣ
}FILE_OPERATE;

u32 file_operate_get_total_phydev();
u32 file_operate_ctl(u32 cmd,void *parm,void *input,void *output);
#endif
