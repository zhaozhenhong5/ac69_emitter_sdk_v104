/*********************************************************************************************
    *   Filename        : sdk_cfg.h

    *   Description     : Config for Sound Box Case

    *   Author          : Bingquan

    *   Email           : bingquan_cai@zh-jieli.com

    *   Last modifiled  : 2016-12-01 15:36

    *   Copyright:(c)JIELI  2011-2016  @ , All Rights Reserved.
*********************************************************************************************/

#ifndef _CONFIG_
#define _CONFIG_

#include "includes.h"


///�����ջ��С���ò���
#define VM_TASK_STACK_SIZE          (1024 * 1)
#define MAIN_TASK_STACK_SIZE        (1024 * 2) //(1024 * 1)����
#define MUSIC_TASK_STACK_SIZE       (1024 * 4)
#define MUSIC_PHY_TASK_STACK_SIZE   (1024 * 4)
#define TONE_TASK_STACK_SIZE        (1024 * 4)
#define TONE_PHY_TASK_STACK_SIZE    (1024 * 2)
#define UI_TASK_STACK_SIZE          (1024 * 2)

//���ö���
///<��������ʹ��,������ʱ�����Ų�sd��u.fm,ble,����ʾ,��̨��app.
#define    BT_STEREO                     0   
///����ӻ������Ƿ���뱻���ӻ��ǻ���״̬
#define    STEREO_SLAVE_WAIT_CON         1  
///���������Ƿ�һ��ػ�
#define    BT_STEREO_POWEROFF_TOGETHER   0 

/********************************************************************************/
/*
 *          --------����������
 */
////<����ϵͳ��ӡ���Թ���
 #define __DEBUG         

///<LEDָʾʹ��
#define LED_EN                  1       
///<��������
#define UART_UPDATA_EN          0      
///<�������
#define SYS_LVD_EN              1       

///<����K�豦
#define BT_KTV_EN				0
///<Х������
#define HOWLING_SUPPRESSION_EN	0

#define BT_EMITTER_EN			1

/********************************************************************************/

/********************************************************************************/
/*
 *           --------�ֻ�app���� ���� 
 */
///�����Ƿ�֧���ֻ�app���ܣ�  (1-֧��,0-��֧��)
#define SUPPORT_APP_RCSP_EN   0 
///�����Ƿ�֧��������Ӳ������ (1-֧��,0-��֧��)
#define RCSP_LIGHT_HW	      0	

/********************************************************************************/

/********************************************************************************/
/*
 *           --------��Դ������
 */
///   0:  no change   
#define    PWR_NO_CHANGE        0     
///   1:  LDOIN 5v -> VDDIO 3.3v -> DVDD 1.2v
#define    PWR_LDO33            1     
///   2:  LDOIN 5v -> LDO   1.5v -> DVDD 1.2v, support bluetooth
#define    PWR_LDO15            2     
///   3:  LDOIN 5v -> DCDC  1.5v -> DVDD 1.2v, support bluetooth
#define    PWR_DCDC15           3     

///��ѡ���ã�PWR_NO_CHANGE/PWR_LDO33/PWR_LDO15/PWR_DCDC15
#define PWR_MODE_SELECT         PWR_LDO15

///����Ƿ�ػ�
#define CHARGE_POWER_OFF 	    0               
///�����������Զ��ػ���ʱ��u16���ͣ�0��ʾ���Զ��ػ�
#define AUTO_SHUT_DOWN_TIME     0               

///<����˫������
#define KEY_DOUBLE_CLICK        0  
/********************************************************************************/

/********************************************************************************/
/*
 *           --------��Ч������
 */
///<EQģ�鿪��
#define EQ_EN			        0       
///<EQ uart online debug
#define EQ_UART_BDG	    		0       
///<dac�����ϲ�
#define DAC_SOUNDTRACK_COMPOUND 0       
///<�Զ�mute
#if BT_KTV_EN
	#define DAC_AUTO_MUTE_EN		0       
#else
	#define DAC_AUTO_MUTE_EN		1       
#endif
///<������
#define KEY_TONE_EN     	    1       
///<��0��ʾʹ��Ĭ������
#define SYS_DEFAULT_VOL         0
///<��������
#define VOCAL_REMOVER			0


/********************************************************************************/


/********************************************************************************/
/*
 *           --------DAC VCOMO ����
 */
///�Ƿ�ѡ��VCMOֱ�ƶ���
#define VCOMO_EN 	            0	
///��ѡ���ã�DAC_L_R_CHANNEL/DAC_L_CHANNEL/DAC_R_CHANNEL  
#define DAC_CHANNEL_SLECT       DAC_L_R_CHANNEL

/********************************************************************************/

/********************************************************************************/
/*
 *           --------����������
 */
 #if (BT_STEREO == 1)
	#define SDMMC0_EN           0
	#define SDMMC1_EN           0
	#define USB_DISK_EN         0
	#define USB_PC_EN           0
#else
    #define SDMMC0_EN           1
	#define SDMMC1_EN           1
	#define USB_DISK_EN         1
	#define USB_PC_EN           1
#endif 


/********************************************************************************/

/********************************************************************************/
/*
 *           --------����������
 */
#include "bluetooth/bluetooth_api.h"

#define NFC_EN          0  ///<NFC ENABLE

///��ѡ���ã�NORMAL_MODE/TEST_BQB_MODE/TEST_FCC_MODE/TEST_FRE_OFF_MODE/TEST_BOX_MODE
#define BT_MODE             NORMAL_MODE

#if (BT_MODE == TEST_FCC_MODE)
  #undef UART_UPDATA_EN
  #define UART_UPDATA_EN    0
#endif

///ģ������
#define BT_ANALOG_CFG       0
#define BT_XOSC             0//

///<������̨
#if (SUPPORT_APP_RCSP_EN == 1)
#define BT_BACKMODE         1             
#else
#define BT_BACKMODE         0             
#endif
///dependency
#if (BT_BACKMODE == 0)
    ///<HID���յĶ���ģʽֻ֧�ַǺ�̨
    #define BT_HID_INDEPENDENT_MODE  0    
#endif

///<���籨��
#define BT_PHONE_NUMBER     1          


/*
 *           --------�����͹������� 
 */
///��ѡ���ã�SNIFF_EN/SNIFF_TOW_CONN_ENTER_POWERDOWN_EN
#define SNIFF_MODE_CONF     0

///��ѡ���ã�BT_POWER_DOWN_EN/BT_POWER_OFF_EN
#define BT_LOW_POWER_MODE   0

#define BT_OSC              0
#define RTC_OSCH            1
#define RTC_OSCL            2

///��ѡ���ã�BT_OSC/RTC_OSCH/RTC_OSCL
#define LOWPOWER_OSC_TYPE   BT_OSC

///��ѡ���ã�32768L//24000000L
#define LOWPOWER_OSC_HZ     24000000L



///��ѡ���ã�BT_BREDR_EN/BT_BLE_EN/(BT_BREDR_EN|BT_BLE_EN)
#if (BT_STEREO == 1)
	#define BLE_BREDR_MODE      (BT_BREDR_EN)
#else
    #define BLE_BREDR_MODE      (BT_BREDR_EN)
#endif 


/********************************************************************************/

/********************************************************************************/
/*
 *           --------оƬ��װ���� 
 */
///RTCVDD��û�а����Ҫ��1,Ŀǰ��Ӧ��װоƬAC6905
#define RTCVDD_TYPE              0          
#define BTAVDD_TYPE              0          

/********************************************************************************/


/********************************************************************************/
/*
 *           --------MUSIC MACRO
 */
//SMP�����ļ�֧��
#define MUSIC_DECRYPT_EN 		0	
//AB�ϵ�֧��
#define MUSIC_AB_RPT_EN 		0	

///<MP3
#define DEC_TYPE_MP3_ENABLE     1
///<SBC
#define DEC_TYPE_SBC_ENABLE     1
///<AAC
#define DEC_TYPE_AAC_ENABLE		0

///<3K_code_space
#define DEC_TYPE_WAV_ENABLE     1
///<5K_code_space
#define DEC_TYPE_FLAC_ENABLE    0
///<8K_code_space
#define DEC_TYPE_APE_ENABLE     0
///<30K_code_space
#define DEC_TYPE_WMA_ENABLE     0
///<30K_code_space
#define DEC_TYPE_F1A_ENABLE     0

/********************************************************************************/


/********************************************************************************/
/*
 *           --------FM MACRO
 */
///<��׼SDK
#if (BT_STEREO == 1)
	#define FM_RADIO_EN         0 
#else
    #define FM_RADIO_EN         1
#endif 
      
///dependency
#if (FM_RADIO_EN == 1)
    ///<RDA5807FM
    #define RDA5807                 0       
    ///<BK1080FM
    #define BK1080                  0       
    ///<QN8035FM
    #define QN8035                  0       
    ///<оƬ�ڲ�FM
    #define FM_INSIDE               1       
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------RTC MACRO
 */
///<��׼SDK RTCʱ��ģʽ
#define RTC_CLK_EN          0       
///dependency
#if (RTC_CLK_EN == 1)
    ///<RTC����ģʽ
    #define RTC_ALM_EN          1       
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------ECHO MACRO
 *			�������죬����ر�	DAC_AUTO_MUTE_EN
 */
///dependency
#if (BT_BACKMODE == 1)
    ///<��֧����������̨��������
    #define ECHO_EN             0       
    ///<����ģʽ ��׼SDK
    #define ECHO_TASK_EN        0       
#else
	#if BT_KTV_EN
    	///<���칦�� KTV_SDK
    	#define ECHO_EN             1
	#else
    	///<���칦�� ��׼SDK
    	#define ECHO_EN             0
	#endif

    ///<����ģʽ ��׼SDK
    #define ECHO_TASK_EN        0       
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------REC MACRO
 */
///dependency
#if (BT_BACKMODE == 1)
    ///<��֧����������̨����¼��
    #define REC_EN             0    
#else
    ///<��׼SDK¼������
    #define REC_EN             0    
#endif

#if (REC_EN == 1)
    ///<����¼��ʹ��
	#define BT_REC_EN		1       
    ///<MIC¼��ʹ��
	#define MIC_REC_EN		1       
    ///<FM¼��ʹ��
	#define FM_REC_EN		1       
    ///<AUX¼��ʹ��
	#define AUX_REC_EN		1       
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------UI MACRO
 */
///<UI_��ʾ
#if (RCSP_LIGHT_HW == 1)
#define UI_ENABLE                0     
#else
#define UI_ENABLE                0     
#endif
///dependency
#if (UI_ENABLE == 1)
    #define LCD_128X64_EN        1      ///<lcd ֧��
    #define LED_7_EN             0      ///<led ֧��
#else
    ///����֧��
    #define LCD_128X64_EN        0
    #define LED_7_EN             0
#endif 
#if ((LCD_128X64_EN == 1) && (LED_7_EN == 1))
#error  "UI driver support only one"
#endif

#if (LCD_128X64_EN == 1)
    ///�˵���ʾ
    #define LCD_SUPPORT_MENU     1       
    ///LRC�����ʾ
    #define LRC_LYRICS_EN        1       
#else
    #define LCD_SUPPORT_MENU     0
    #define LRC_LYRICS_EN        0
#endif

/********************************************************************************/

#endif
