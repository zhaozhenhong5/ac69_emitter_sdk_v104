#ifndef _CLOCK_INTERFACE_H_
#define _CLOCK_INTERFACE_H_

#include "typedef.h"

//��ͷ�ļ�Ϊapps �����ӿ��ļ�����������driver.h

#define RTC_OSC_Hz     32768L
#define RC_OSC_Hz      250000L
#define OSC_Hz         24000000L
#define SPI_MAX_CLK	   100000000L	//<-SPI���֧��Ƶ��

#define RFI_Hz		   80000000L	//<-�̶�Ƶ��
#define APC_Hz		   96000000L	//<-�������������ɵ���
#define LSB_Hz         80000000L

typedef enum
{
    ///ԭ��ʱ��Դ��ϵͳʱ��Դ
    SYS_CLOCK_INPUT_RTOSC_32K1P = 0, //RTOSC ����(32K)
    SYS_CLOCK_INPUT_RTOSC_32K2P,     //RTOSC ˫��(32K)
    SYS_CLOCK_INPUT_RTOSC_24M2P,     //RTOSC ˫��(12-26M)
    SYS_CLOCK_INPUT_BT_OSC,          //BTOSC ˫��(12-26M)
    SYS_CLOCK_INPUT_RC,
	SYS_CLOCK_INPUT_PAT,

    ///����ʱ��Դ��ϵͳʱ��Դ
    SYS_CLOCK_INPUT_PLL_32K1P,
    SYS_CLOCK_INPUT_PLL_32K2P,
    SYS_CLOCK_INPUT_PLL_24M2P,
    SYS_CLOCK_INPUT_PLL_BT_OSC,
    SYS_CLOCK_INPUT_PLL_PAT,
}SYS_CLOCK_INPUT;

typedef enum
{
    PB0_CLOCK_OUTPUT = 0,
    PB0_CLOCK_OUT_LSB,
    PB0_CLOCK_OUT_BT_OSC,
    PB0_CLOCK_OUT_RTOSC_H,

    PB0_CLOCK_OUT_RC = 4,
    PB0_CLOCK_OUT_HSB,
    PB0_CLOCK_OUT_RTOSC_L,
    PB0_CLOCK_OUT_PLL_SYS,
}PB0_CLK_OUT;

typedef enum
{
    PB9_CLOCK_OUTPUT = 0,
    PB9_CLOCK_OUT_FM_LO_D2,			//<-����Ƶģ�����
    PB9_CLOCK_OUT_PLL_RFI,
    PB9_CLOCK_OUT_PLL_192M,

    PB9_CLOCK_OUT_BT_LO_D32 = 4,	//<-����Ƶģ�����
    PB9_CLOCK_OUT_WL,
    PB9_CLOCK_OUT_APC,
    PB9_CLOCK_OUT_RCCL,
}PB9_CLK_OUT;


#define SYS_CLOCK_IN    SYS_CLOCK_INPUT_PLL_BT_OSC



#endif
