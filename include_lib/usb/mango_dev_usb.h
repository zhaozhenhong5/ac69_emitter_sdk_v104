#ifndef _MANGO_DEV_USB_H_
#define _MANGO_DEV_USB_H_

#include "typedef.h"
#include "dev_manage/device_driver.h"

/**---------------------enum��----------------------**/
/**---------------------------------------------------**/
/*ö��ʧ���б�*/
enum
{
    USB_SUCC = 0x0,
    USB_ERR_NOT_MOUNTED,            ///<�豸δö��
    USB_ERR_OVER_CAPACITY,          ///<������������Χ
    USB_ERR_UNKNOW_CLASS,                ///<�豸��MassStorage��
    USB_ERR_UNIT_NOT_READY,         ///<����߼���Ԫδ׼��
    USB_ERR_LUN,
    USB_ERR_BULK_RPT30,
    USB_ERR_BULK_RECEIVE,

    USB_ERR_READ = 0x10,            ///<������

    USB_ERR_WRITE = 0x20,           ///<д����
    USB_ERR_WRITE_SECTOR_SIZE,


    USB_ERR_TIMEOUT = 0x30,         ///<���䳬ʱ
    USB_ERR_TIMEOUT_CONTROL,        ///<���䳬ʱ ���ƴ���׶�
    USB_ERR_TIMEOUT_BULK_SEND,      ///<���䳬ʱ ��������׶�
    USB_ERR_TIMEOUT_BULK_RECEIVE,   ///<���䳬ʱ ��������׶�

    USB_ERR_OFFLINE = 0x40,         ///<�豸������
    USB_ERR_OFFLINE_CONTROL,
    USB_ERR_OFFLINE_BULK_SEND,
    USB_ERR_OFFLINE_BULK_RECEIVE,

    USB_ERR_STALL = 0x50,           ///<������Ч
    USB_ERR_STALL_CONTROL,          ///<������Ч ���ƴ���׶�
    USB_ERR_STALL_BULK_SEND,        ///<������Ч ��������׶�
    USB_ERR_STALL_BULK_RECEIVE,

    USB_ERR3 = 0x60,                ///<������Ч
    USB_ERR3_CONTROL,               ///<������Ч ���ƴ���׶�
};


/*****************************
        Typedef
*****************************/
typedef struct _USB_DETECT
{
    volatile u8  bDevOnline;    //<�豸���߱�־	[0]:USB Host [1]:USB Slave
    u8  bOnlineCnt;             //<USB ���߼�����
    u8  bLastData;              //<USB �������Ѽ���ֵ
    u8  bCurData;               //<USB ��������ǰ����ֵ
    u32 last_sta;               //<USB ��һ������״̬
}sUSB_DETECT;

typedef struct _USB_HOST_VAR
{
    /*-----�豸��������-------*/
    u32 dwDevCapacity;          //<�豸����
    u32 dwDevCurLBA;            //<��ǰ�豸�����߼���ַ
    u16 wDevTimeOutMax;		    //<�豸������ʱ����
    volatile u16 wDevTimeOut;   //<�豸������ʱ����
    u8  bDevOperation;          //<�豸����״̬
    u8  bDevStatus;			    //<�豸״̬
    volatile u8 bDevBusy;	    //<�豸��æ���
    u8  bDevError;              //<�豸��������

    /*-----�豸��������-------*/
    u8  bDevSectorPerLBA;       //<�豸�߼���ַSector��С Logical block sector size (SectorPerLBA * 512)
    u8  bDevCurOffset;          //<�豸�����߼���ַƫ�� Logical Block Sector offset
    u8  bDevTransFail;          //<Bulk-Only ��������־
    u8  bClass;                 //<[0]:MSD [1]:Audio [2]:HID
}sUSB_HOST_VAR;

typedef struct _USB_MSD_CLASS_INFO
{
    u16 wMaxPacketSize;
    u8  bInterfaceNumber;
    u8  bDevAddr;		        //<�豸��ַ
    u8  bDevEpIn;               //<Bulk Only ���������
    u8  bDevEpOut;              //<Bulk Only ���������
}sUSB_MSD_CLASS_INFO;

typedef struct _USB_CONTROL_TRANSFER
{
    u8 bTokens;		    //<���ư�����
    u8 bDatalen;	    //<���ݽ׶����贫����ֽ���
    u8 bMaxPktSize;     //<�豸����������ݰ�����
    u8 bEpNum;          //<��ʹ�õĶ˵������������˵�0��

    u8 *pBuf;           //<����EP0 FIFO buffer
}sUSB_CONTROL_TRANSFER;

typedef struct _USBH_REQUEST_BYTE	//<USB ����
{
    u8 bmRequestType;	            //<[7]:���䷽�� [6:5]:���� [4:0]:���շ�
    u8 bRequset;
    u8 wValueL;
    u8 wValueH;
    u8 wIndexL;
    u8 wIndexH;
    u8 wLengthL;
    u8 wLengthH;
}sUSBH_REQUEST_BYTE;

typedef union _USB_CONTROL
{
    sUSBH_REQUEST_BYTE SETUP_pkt; //
    u8  aStdReq[8];             //<Standard Request FIFO
}sUSB_CONTROL;

typedef struct _USB_BULK_ONLY_TRANSFER
{
    //u8 *pBuf;        //<Data ����buffer
    u8 bMaxLUN;         //<
    u8 bCBWLUN;         //<
}sUSB_BULK_ONLY_TRANSFER;

/*****************************
        Typedef Mix
*****************************/
typedef struct _USB_VAR
{
    sUSB_DETECT             detect;
    sUSB_HOST_VAR           host_var;
    sUSB_MSD_CLASS_INFO     msd_class_info;
    sUSB_CONTROL            control_setup;
    sUSB_CONTROL_TRANSFER   control_trans;
    sUSB_BULK_ONLY_TRANSFER bulk_only_trans;
}sUSB_VAR;

typedef struct __HUSB_REG_VAR
{
    dev_io_t husb_io;
    OS_MUTEX husb_rwmutex;
}HUSB_REG_VAR;


extern const struct DEV_IO * dev_reg_usb(void *parm);

#endif //_MANGO_DEV_USB_H_

