/*******************************************************************************************
File Name: 	SD_Host_API.h

Version: 	1.00

Discription SD Host �ӿں���

Author: 	Caibingquan

Email:  	change.tsai@gmail.com

Date:		2013.01.02

Copyright (c) 2010 - 2012 JieLi. All Rights Reserved.
*******************************************************************************************/
#ifndef _SD0_Host_API_H_
#define _SD0_Host_API_H_

#include "typedef.h"
#include "dev_manage/device_driver.h"

/**---------------------�ⲿ����----------------------**/
/**---------------------------------------------------**/
extern const struct DEV_IO * dev_reg_sd0(void *parm);
extern const struct DEV_IO * dev_reg_sd1(void *parm);


//----------------------------------
// IO CONTROL CMD
//----------------------------------
enum
{
	SDMMC_GET_VAR_ADDR = 1,
	SDMMC_GET_CTRLER_STATUS,	//��ȡSD��������æ״̬
	SDMMC_GET_CTRLER_FLAG,
	SDMMC_GET_CMD_LOCK,			//����CMD���Ƿ�����
	SDMMC_GET_BUSY,				//��ȡSD BUSY״̬
	SDMMC_MEM_FREE,				//�ͷ�SD����ı����ռ�
	SDMMC_CLOSE_CONTROLLER,		//suspend SD�����ر�SD������(����ռ��IO��)
	SDMMC_RELEASE_CONTROLLER,	//���´�SD��������ռ����ӦIO��
};

/*SD����ģʽ����*/
typedef enum
{
    SD_4WIRE_MODE = 0,
    SD_1WIRE_MODE = BIT(0),
    SD_HI_SPEED = BIT(1),
}SD_NWRITE_SPEED_MODE;

/*SD��������ӦIO����*/
typedef enum
{
    SD0_IO_A = 1, //sd0������A����
    SD0_IO_B,     //sd0������B����

    SD1_IO_A,     //sd1������A����
    SD1_IO_B,     //sd1������B����
}SD_CONTROLLER_IO;

/*SD���߼�ⷽʽ*/
typedef enum
{
    SD_CMD_CHECK = 1, //SD��������
    SD_CLK_CHECK,     //SD��CLK���
    SD_IO_CHECK,      //SD��IO���
}SD_CHECK_WAY;

typedef struct _SD_API_SET
{
    u8  controller_io;     //<������IO����
    u8  online_check_way;  //<sd�����߼�ⷽʽ��cmd��clk
    u8  max_data_baud;     //<���ݴ����������
    u8  hc_mode;           //<sd������ģʽ�͸���ģʽ����
    u8  (*io_det_func)(void); //<io�ڼ�⺯��
    u8  rw_way;               //��д����ѡ��Ĭ��ѡ�񡰵ȴ���д�꺯����
}sSD_API_SET __attribute__((aligned(4)));


/**---------------------enum��----------------------**/
/**---------------------------------------------------**/
/*sd����ʧ���б�*/
enum
{
    SD_SUCC = 0x0,
    SD_ERR_NOT_MOUNTED,             ///<�豸δ����
    SD_ERR_OVER_CAPACITY,           ///<������������Χ
    SD_ERR_UNKNOWN_CLASS,           ///<�豸δ֪ Class
    SD_ERR_DEV_BUSY,                ///<��������æ

    SD_ERR_READ = 0x10,             ///<������
    SD_ERR_READ_MULTIPLE_BLOCK,

    SD_ERR_WRITE = 0x20,            ///<д����
    SD_ERR_WRITE_MULTIPLE_BLOCK,

    SD_ERR_TIMEOUT = 0x30,          ///<���䳬ʱ
    SD_ERR_TIMEOUT_COMMAND,         ///<�����Ӧ��ʱ
    SD_ERR_TIMEOUT_DATA,            ///<���ݴ��䳬ʱ
    SD_ERR_TIMEOUT_READ,            ///<���䳬ʱ �����ݽ׶�
    SD_ERR_TIMEOUT_WRITE,           ///<���䳬ʱ д���ݽ׶�

    SD_ERR_OFFLINE = 0x40,          ///<�豸������
    SD_ERR_OFFLINE_READ,
    SD_ERR_OFFLINE_WRITE,

    SD_ERR_CRC16 = 0x50,
    SD_ERR_CRC16_COMMAND,
    SD_ERR_CRC16_DATA,
    SD_ERR_CRC16_READ,
    SD_ERR_CRC16_WRITE,

    SD_STOP_ERR = 0x60,            ///<��ͣ
    SD_STOP_ERR_READ,
    SD_STOP_ERR_WRITE,
    SD_IDENTIFICATION_RETRY_ERR,   ///<������Ч ���ƴ���׶�
};

#endif/*_SD0_Host_API_H_*/
