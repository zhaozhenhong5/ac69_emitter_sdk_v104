#include "sdk_cfg.h"
#include "dev_manage/dev_ctl.h"
#include "dev_manage/drv_manage.h"
#include "sdmmc/sd_host_api.h"
#include "dev_manage/mango_dev_cache.h"
#include "common/app_cfg.h"
#include "file_operate/logic_dev_list.h"
#include "linein/linein.h"
#include "usb_device/dev_pc.h"
#include "sys_detect.h"
#include "linein/dev_linein.h"
#include "irq_api.h"
#include "crc_api.h"
#include "update.h"

#define UDISK_READ_PAGE    512

extern tbool syd_test(void *p_hdev);
extern const struct DEV_IO * dev_reg_usb(void *parm);
//extern OS_EVENT  sys_clk_chg_sem;

DEV_HANDLE volatile sd0;        ///>�豸���
DEV_HANDLE volatile sd1;        ///>�豸���
DEV_HANDLE volatile usb;        ///>�豸���
DEV_HANDLE volatile cache;      ///>�豸���

#if SDMMC0_EN
///sd0 io��⺯��
u8 sd0_io_check(void)
{
    return TRUE;  //��⵽
    return FALSE; //û��⵽
}
#endif


#if SDMMC1_EN
///sd1 io��⺯��
u8 sd1_io_check(void)
{
    return TRUE;  //��⵽
    return FALSE; //û��⵽
}
#endif


static void dev_reg_ctrl(void)
{

    u32 dev_parm=0;

#if SDMMC0_EN
    sSD_API_SET sd0_api_set;
    memset(&sd0_api_set, 0x0, sizeof(sSD_API_SET));
    sd0_api_set.controller_io = SD0_IO_A;        //SD0_IO_A��SD0������B���ڣ�SD0_IO_B��SD0������B����
    sd0_api_set.online_check_way = SD_CLK_CHECK; //CMD��ⷽʽ��SD_CMD_CHECK��CLK��ⷽʽ��SD_CLK_CHECK��IO��ⷽʽ��SD_IO_CHECK
    sd0_api_set.max_data_baud = 5;               //���ݴ��䲨����(0Ϊ����ٶ�)
    sd0_api_set.hc_mode = SD_1WIRE_MODE;         //1��ģʽ��SD_1WIRE_MODE��4��ģʽ��SD_4WIRE_MODE������ģʽ��SD_HI_SPEED
                                                    //--(SD_1WIRE_MODE|SD_HI_SPEED ��ʾ1�߸���ģʽ)
    if(SD_IO_CHECK == sd0_api_set.online_check_way)
    {
        sd0_api_set.io_det_func = sd0_io_check;  //����io��⺯��
    }
	DEVICE_REG(sd0,&sd0_api_set);                ///<ע��sd0��ϵͳ
    sd0 = dev_open("sd0", 0);
#endif

#if SDMMC1_EN
    sSD_API_SET sd1_api_set;
    memset(&sd1_api_set, 0x0, sizeof(sSD_API_SET));

	sd1_api_set.controller_io = SD1_IO_B;        //SD1_IO_A��SD1������B���ڣ�SD1_IO_B��SD1������B����
	sd1_api_set.online_check_way = SD_CLK_CHECK; //CMD��ⷽʽ��SD_CMD_CHECK��CLK��ⷽʽ��SD_CLK_CHECK��IO��ⷽʽ��SD_IO_CHECK
    sd1_api_set.max_data_baud = 5;               //���ݴ��䲨����(0Ϊ����ٶ�)
    sd1_api_set.hc_mode = SD_1WIRE_MODE;         //1��ģʽ��SD_1WIRE_MODE��4��ģʽ��SD_4WIRE_MODE������ģʽ��SD_HI_SPEED
                                                    //--(SD_1WIRE_MODE|SD_HI_SPEED ��ʾ1�߸���ģʽ)
    if(SD_IO_CHECK == sd1_api_set.online_check_way)
    {
        sd1_api_set.io_det_func = sd1_io_check;  //����io��⺯��
    }
	DEVICE_REG(sd1,&sd1_api_set);                ///<ע��sd1��ϵͳ
    sd1 = dev_open("sd1", 0);
#endif

#if USB_DISK_EN
    dev_parm = UDISK_READ_PAGE;
    DEVICE_REG(usb, &dev_parm); ///<ע��usb��ϵͳ
    usb = dev_open("usb", 0);
#endif

    DEVICE_REG(cache, NULL);
    cache = dev_open("cache", 0);
}

u32 dev_detect_fun(u32 info)
{
#if SD_DADA_MULT
	sd_data_multiplex();
#endif
	
#if AUX_DETECT_EN
    aux_check_api(info); //linein���
#endif

#if(BT_MODE==NORMAL_MODE)
#if USB_PC_EN
    pc_check_api(info);  //PC���
#endif
#endif

    return 0;
}

extern u32 lg_dev_list_init();
void dev_ctrl_sys_init(void *parm)
{
    ///�߼��豸�����ʼ��
    lg_dev_list_init();

    ///�����豸�����ʼ��
    phy_dev_list_init();

    ///ע���豸(�����豸���ڴ˴�ע��)
	dev_reg_ctrl();

    ///�����豸�������߳�
    dev_init(TaskDevCheckPrio,TaskDevMountPrio,(void*)dev_detect_fun);

//    os_mutex_create(&sys_clk_chg_sem);

}

u32 dev_updata_cfg(void)
{
    JL_AUDIO->DAC_CON = 0;
    irq_global_disable();
    irq_clear_all_ie();
    clr_PINR_ctl();
	led_update_start();
    return 1; //����λ����
}


