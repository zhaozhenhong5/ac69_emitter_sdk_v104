#ifndef __DEVICE_DRIVE_H__
#define __DEVICE_DRIVE_H__
/* #include "typedef.h" */
#include "common/errno-base.h"
#include "ioctl.h"
#ifndef _MANGO_OS_
#define _MANGO_OS_
#endif
#ifdef _MANGO_OS_
#include "rtos/os_api.h"
#else

#ifndef _WIN_DEBUG_
#define _WIN_DEBUG_

#endif

#ifdef _WIN_DEBUG_
#include <pthread.h>
typedef pthread_mutex_t OS_MUTEX;
typedef u32 OS_ERR;
#define OS_ERR_NONE 0

#else
typedef u32 OS_MUTEX,OS_ERR;
#define OS_ERR_NONE 0

#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

    struct dev_mutex
    {
        OS_MUTEX *write_mutex;
        OS_MUTEX *read_mutex;
    };
    typedef enum _dev_type
    {

        DEV_SDCRAD_0 = 0X10,
        DEV_SDCRAD_1,
        DEV_SDCRAD_2,

        DEV_UDISK_H0,
        DEV_UDISK_H1,
        DEV_UDISK_F0,

        DEV_NOR_FLASH,
        DEV_NAND_FLASH,

        DEV_USB_SLAVE,

        DEV_STORAGE=0x100,
        DEV_LOGIC_DISK=0x101,

        DEV_HID=0x200,
        DEV_NET,
        DEV_AUDIO,
        DEV_ISP,
    }dev_type;

//    struct partition_table
//    {
//        int partition_addr;
//        int partition_size;
//    };
    typedef struct DEV_IO
    {
        const char name[8];
        s32(*mount)(void *parm);
        s32(*unmount)(void);
        s32(*read)(u8*buf, u32 addr , u32 len);
        s32(*write)(u8*buf , u32 addr , u32 len);
        s32(*ioctrl)(void *parm , u32 cmd);
        s32(*power)(u32 mod);
        s32(*detect)(void); //�豸״̬���
        s32(*var_init)(void *var_p, void *parm);
        struct dev_mutex *mutex;
        dev_type device_type;
		const char *fs_type;

        //void *private_data;//�豸˽�����ԣ������ṩ����ӿڻ��ߴ洢һЩ�豸������
    } dev_io_t;



//�豸״̬��
    typedef enum dev_sta
    {
        DEV_OFFLINE  = 0, //--0    �豸�������л������ߡ�
        DEV_ONLINE = 1, //---1    �豸�������л������ߡ�
        DEV_HOLD = 2, //---2        ����ֵ��ʾ�豸״̬δ�ı䡣
        DEV_POWER_ON = 0x10,    //        0x10 �C ����
        DEV_POWER_OFF,        //        0x11 -�ػ�
        DEV_POWER_STANDBY, //    0x12 -����
        DEV_POWER_WAKEUP,    //    0x13- ����

    }DEV_STA;
//�豸������룺
    typedef enum dev_err
    {
        DEV_ERR_NONE = 0,
        DEV_ERR_NOT_MOUNT,
        DEV_ERR_OVER_CAPACITY,

        DEV_ERR_UNKNOW_CLASS,

        DEV_ERR_NOT_READY,//�豸�Ѿ����ߣ�����û��ʼ�����
        DEV_ERR_LUN,

        DEV_ERR_TIMEOUT,
        DEV_ERR_CMD_TIMEOUT,
        DEV_ERR_READ_TIMEOUT,//0x08
        DEV_ERR_WRITE_TIMEOUT,

        DEV_ERR_OFFLINE,//0x0a

        DEV_ERR_CRC,
        DEV_ERR_CMD_CRC,
        DEV_ERR_READ_CRC,
        DEV_ERR_WRITE_CRC,

        DEV_ERR_CONTROL_STALL,
        DEV_ERR_RXSTALL,//0x10
        DEV_ERR_TXSTALL,
        DEV_ERR_CONTROL,

        DEV_ERR_NOT_STORAGE,
        DEV_ERR_INVALID_PATH,
        DEV_ERR_INVALID_DATA,
        DEV_ERR_OUTOFMEMORY,
        DEV_ERR_HANDLE_FREE,
        DEV_ERR_INVALID_HANDLE,//24
        DEV_ERR_INVALID_BUF,
        DEV_ERR_INUSE,
        DEV_ERR_NO_READ,
        DEV_ERR_NO_WRITE,
        DEV_ERR_NO_IOCTL,
        DEV_ERR_NO_POWER,
        DEV_ERR_NOT_EXIST,
        DEV_ERR_UNKNOW,
        DEV_ERR_FULL,
    }DEV_ERR;


#define DEV_GENERAL_MAGIC    0xe0
    //ÿ���豸����֧�ֵ�����
#define DEV_GET_STATUS             _IOR(DEV_GENERAL_MAGIC,0xe0,u32)    //��ȡ�豸״̬�����ߡ����ߡ�power_on��power_off��standby������
    //������������豸��֧���Ƿ��� -ENOTTY
#define DEV_GET_BLOCK_SIZE      _IOR(DEV_GENERAL_MAGIC,0xe1,u32)    //��ȡ�洢�豸�Ŀ��С
#define DEV_GET_BLOCK_NUM       _IOR(DEV_GENERAL_MAGIC,0xe2,u32)    //��ȡ�洢�豸�Ŀ�����

#define DEV_SET_REMAIN          _IOW(DEV_GENERAL_MAGIC,0xe3,u32)    //��ȡ�洢�豸ʣ��sector

#ifdef __cplusplus

}
#endif


#endif

