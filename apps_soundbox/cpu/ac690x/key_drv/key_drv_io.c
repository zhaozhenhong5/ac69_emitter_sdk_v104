#include "includes.h"
#include "key_drv/key.h"
#include "key_drv/key_drv_io.h"


#if KEY_IO_EN

/*----------------------------------------------------------------------------*/
/**@brief   io������ʼ��
   @param   void
   @param   void
   @return  void
   @note    void io_key_init(void)
*/
/*----------------------------------------------------------------------------*/
void io_key_init(void)
{
    KEY_INIT();
}

/*----------------------------------------------------------------------------*/
/**@brief   ��ȡIO������ƽֵ
   @param   void
   @param   void
   @return  key_num:io������
   @note    tu8 get_iokey_value(void)
*/
/*----------------------------------------------------------------------------*/
tu8 get_iokey_value(void)
{
    //key_puts("get_iokey_value\n");
    tu8 key_num = NO_KEY;

    if(IS_KEY0_DOWN()){
        key_puts(" KEY0 ");
        return 0;
    }
    if(IS_KEY1_DOWN()){
        key_puts(" KEY1 ");
        return 1;
    }
    if(IS_KEY2_DOWN()){
        key_puts(" KEY2 ");
        return 2;
    }
    if(IS_KEY3_DOWN()){
		key_puts(" KEY3 ");
        return 3;
    }

//    KEY1_OUT_L();
//    if(IS_KEY2_DOWN())//
//    {
//        PORTA_OUT |= BIT(13);
//        return 4;
//    }
    return key_num;
}

#endif/*KEY_IO_EN*/
