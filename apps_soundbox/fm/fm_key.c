#include "sdk_cfg.h"
#include "fm_key.h"
#include "common/msg.h"

#if FM_RADIO_EN
/*******************************************************************
                            AD������
*******************************************************************/
#if LCD_SUPPORT_MENU

#define ADKEY_FM_SHORT		\
                        /*00*/    MSG_FM_PP,\
                        /*01*/    MSG_FM_NEXT_STATION,\
                        /*02*/    MSG_FM_PREV_STATION,\
                        /*03*/    MSG_FM_NEXT_STEP,\
                        /*04*/    MSG_FM_PREV_STEP,\
                        /*05*/    NO_MSG,\
                        /*06*/    MSG_ENTER_SLEEP_MODE,\
                        /*07*/    NO_MSG,\
                        /*08*/    MSG_ENTER_MENUMAIN,\
                        /*09*/    MSG_ENTER_MENULIST,
#else

#define ADKEY_FM_SHORT		\
                        /*00*/    MSG_FM_PP,\
                        /*01*/    MSG_FM_NEXT_STATION,\
                        /*02*/    MSG_FM_PREV_STATION,\
                        /*03*/    MSG_FM_NEXT_STEP,\
                        /*04*/    MSG_FM_PREV_STEP,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*09*/    MSG_FM_SCAN_ALL_INIT,\
                        /*07*/    MSG_ECHO_START,\
                        /*08*/    MSG_REC_START,
                        
#endif //#if LCD_SUPPORT_MENU


#define ADKEY_FM_LONG		\
                        /*00*/    MSG_CHANGE_WORKMODE,\
                        /*01*/    MSG_VOL_UP,\
                        /*02*/    MSG_VOL_DOWN,\
                        /*03*/    MSG_FM_SCAN_ALL_UP,\
                        /*04*/    MSG_FM_SCAN_ALL_DOWN,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    MSG_ECHO_STOP,\
                        /*08*/    MSG_REC_STOP,

#define ADKEY_FM_HOLD		\
                        /*00*/    NO_MSG,\
                        /*01*/    MSG_VOL_UP,\
                        /*02*/    MSG_VOL_DOWN,\
                        /*03*/    NO_MSG,\
                        /*04*/    NO_MSG,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    NO_MSG,\
                        /*08*/    NO_MSG,\
                        /*09*/    NO_MSG,

#define ADKEY_FM_LONG_UP	\
                        /*00*/    NO_MSG,\
                        /*01*/    NO_MSG,\
                        /*02*/    NO_MSG,\
                        /*03*/    NO_MSG,\
                        /*04*/    NO_MSG,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    NO_MSG,\
                        /*08*/    NO_MSG,\
                        /*09*/    NO_MSG,

const u8 fm_ad_table[4][10] =
{
    /*�̰�*/	    {ADKEY_FM_SHORT},
    /*����*/		{ADKEY_FM_LONG},
    /*����*/		{ADKEY_FM_HOLD},
    /*����̧��*/	{ADKEY_FM_LONG_UP},
};

/*******************************************************************
                            I/O������
*******************************************************************/
#define IOKEY_FM_SHORT		\
                        /*00*/    MSG_FM_SCAN_ALL_INIT,\
                        /*01*/    MSG_FM_NEXT_STATION,\
                        /*02*/    MSG_FM_PREV_STATION,\
                        /*03*/    NO_MSG,\
                        /*04*/    NO_MSG,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    NO_MSG,\
                        /*08*/    NO_MSG,\
                        /*09*/    NO_MSG,

#define IOKEY_FM_LONG		\
                        /*00*/    MSG_CHANGE_WORKMODE,\
                        /*01*/    MSG_VOL_UP,\
                        /*02*/    MSG_VOL_DOWN,\
                        /*03*/    NO_MSG,\
                        /*04*/    NO_MSG,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    NO_MSG,\
                        /*08*/    NO_MSG,\
                        /*09*/    NO_MSG,


#define IOKEY_FM_HOLD		\
                        /*00*/    NO_MSG,\
                        /*01*/    MSG_VOL_UP,\
                        /*02*/    MSG_VOL_DOWN,\
                        /*03*/    NO_MSG,\
                        /*04*/    NO_MSG,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    NO_MSG,\
                        /*08*/    NO_MSG,\
                        /*09*/    NO_MSG,

#define IOKEY_FM_LONG_UP	\
                        /*00*/    NO_MSG,\
                        /*01*/    NO_MSG,\
                        /*02*/    NO_MSG,\
                        /*03*/    NO_MSG,\
                        /*04*/    NO_MSG,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    NO_MSG,\
                        /*08*/    NO_MSG,\
                        /*09*/    NO_MSG,

const u8 fm_io_table[4][10] =
{
    /*�̰�*/	    {IOKEY_FM_SHORT},
    /*����*/		{IOKEY_FM_LONG},
    /*����*/		{IOKEY_FM_HOLD},
    /*����̧��*/	{IOKEY_FM_LONG_UP},
};

/*******************************************************************
                            IR������
*******************************************************************/
#define IRFF00_FM_KEY_SHORT			\
                                /*00*/    MSG_POWER_OFF,\
							    /*01*/    MSG_CHANGE_WORKMODE,\
								/*02*/    MSG_MUTE,\
								/*03*/    MSG_FM_PP,\
								/*04*/    MSG_FM_PREV_STEP,\
								/*05*/    MSG_FM_NEXT_STEP,\
								/*06*/    MSG_FM_SCAN_ALL_INIT,\
								/*07*/    MSG_VOL_DOWN,\
								/*08*/    MSG_VOL_UP,\
								/*09*/    MSG_0,\
                                /*10*/    NO_MSG,\
								/*11*/    NO_MSG,\
								/*12*/    MSG_1,\
								/*13*/    MSG_2,\
								/*14*/    MSG_3,\
								/*15*/    MSG_4,\
								/*16*/    MSG_5,\
								/*17*/    MSG_6,\
								/*18*/    MSG_7,\
								/*19*/    MSG_8,\
								/*20*/    MSG_9


#define IRFF00_FM_KEY_LONG			\
								/*00*/    NO_MSG,\
                                /*01*/    NO_MSG,\
								/*02*/    NO_MSG,\
								/*03*/    MSG_FM_SCAN_ALL_INIT,\
								/*04*/    MSG_FM_PREV_STATION,\
								/*05*/    MSG_FM_NEXT_STATION,\
								/*06*/    NO_MSG,\
								/*07*/    MSG_ENTER_MENULIST,\
								/*08*/    MSG_ENTER_MENUMAIN,\
								/*09*/    NO_MSG,\
                                /*10*/    NO_MSG,\
								/*11*/    NO_MSG,\
								/*12*/    NO_MSG,\
								/*13*/    NO_MSG,\
								/*14*/    NO_MSG,\
								/*15*/    NO_MSG,\
								/*16*/    NO_MSG,\
								/*17*/    NO_MSG,\
								/*18*/    NO_MSG,\
								/*19*/    NO_MSG,\
								/*20*/    NO_MSG

#define IRFF00_FM_KEY_HOLD			\
								/*00*/    NO_MSG,\
                                /*01*/    NO_MSG,\
								/*02*/    NO_MSG,\
								/*03*/    NO_MSG,\
								/*04*/    NO_MSG,\
								/*05*/    NO_MSG,\
								/*06*/    NO_MSG,\
								/*07*/    MSG_VOL_DOWN,\
								/*08*/    MSG_VOL_UP,\
								/*09*/    NO_MSG,\
                                /*10*/    NO_MSG,\
								/*11*/    NO_MSG,\
								/*12*/    NO_MSG,\
								/*13*/    NO_MSG,\
								/*14*/    NO_MSG,\
								/*15*/    NO_MSG,\
								/*16*/    NO_MSG,\
								/*17*/    NO_MSG,\
								/*18*/    NO_MSG,\
								/*19*/    NO_MSG,\
								/*20*/    NO_MSG


#define IRFF00_FM_KEY_LONG_UP 		\
								/*00*/    NO_MSG,\
                                /*01*/    NO_MSG,\
								/*02*/    NO_MSG,\
								/*03*/    NO_MSG,\
								/*04*/    NO_MSG,\
								/*05*/    NO_MSG,\
								/*06*/    NO_MSG,\
								/*07*/    NO_MSG,\
								/*08*/    NO_MSG,\
								/*09*/    NO_MSG,\
								/*10*/    NO_MSG,\
								/*11*/    NO_MSG,\
								/*12*/    NO_MSG,\
								/*13*/    NO_MSG,\
                                /*14*/    NO_MSG,\
								/*15*/    NO_MSG,\
								/*16*/    NO_MSG,\
								/*17*/    NO_MSG,\
								/*18*/    NO_MSG,\
								/*19*/    NO_MSG,\
								/*20*/    NO_MSG
const u8 fm_ir_table[4][21] =
{
    /*�̰�*/	    {IRFF00_FM_KEY_SHORT},
    /*����*/		{IRFF00_FM_KEY_LONG},
    /*����*/		{IRFF00_FM_KEY_HOLD},
    /*����̧��*/	{IRFF00_FM_KEY_LONG_UP},
};


/*******************************************************************
                            touchkey������
*******************************************************************/
#define TOUCHKEY_FM_SHORT		\
                        /*00*/    NO_MSG,\
                        /*01*/    NO_MSG,\
                        /*02*/    NO_MSG,\
                        /*03*/    NO_MSG,\
                        /*04*/    NO_MSG,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    NO_MSG,\
                        /*08*/    NO_MSG,\
                        /*09*/    NO_MSG,

#define TOUCHKEY_FM_LONG		\
                        /*00*/    NO_MSG,\
                        /*01*/    NO_MSG,\
                        /*02*/    NO_MSG,\
                        /*03*/    NO_MSG,\
                        /*04*/    NO_MSG,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    NO_MSG,\
                        /*08*/    NO_MSG,\
                        /*09*/    NO_MSG,


#define TOUCHKEY_FM_HOLD		\
                        /*00*/    NO_MSG,\
                        /*01*/    NO_MSG,\
                        /*02*/    NO_MSG,\
                        /*03*/    NO_MSG,\
                        /*04*/    NO_MSG,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    NO_MSG,\
                        /*08*/    NO_MSG,\
                        /*09*/    NO_MSG,

#define TOUCHKEY_FM_LONG_UP	\
                        /*00*/    NO_MSG,\
                        /*01*/    NO_MSG,\
                        /*02*/    NO_MSG,\
                        /*03*/    NO_MSG,\
                        /*04*/    NO_MSG,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    NO_MSG,\
                        /*08*/    NO_MSG,\
                        /*09*/    NO_MSG,

const u8 fm_touch_table[4][10] =
{
    /*�̰�*/	    {TOUCHKEY_FM_SHORT},
    /*����*/		{TOUCHKEY_FM_LONG},
    /*����*/		{TOUCHKEY_FM_HOLD},
    /*����̧��*/	{TOUCHKEY_FM_LONG_UP},
};
#endif
