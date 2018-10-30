/*--------------------------------------------------------------------------*/
/**@file    UI_common.c
   @brief   UI ��ʾ��������
   @details
   @author  bingquan Cai
   @date    2012-8-30
   @note    AC319N
*/
/*----------------------------------------------------------------------------*/
#include "sdk_cfg.h"
#include "ui/ui_common.h"
#include "ui/ui_api.h"

#if UI_ENABLE

const u8 asc_number[] = "0123456789";    ///<0~9��ASCII���

u8 bcd_number[5];

/*----------------------------------------------------------------------------*/
/**@brief  ��ȡһ��һλʮ���Ƶ����ĸ�λ
   @param  i:�����һ��һλʮ���Ƶ���
   @return ��
   @note   void itoa1(u8 i)
*/
/*----------------------------------------------------------------------------*/
void itoa1(u8 i)
{
    bcd_number[0] = asc_number[i % 10];
    bcd_number[1] = 0;
}

/*----------------------------------------------------------------------------*/
/**@brief  ��ȡһ��2λʮ���Ƶ����ĸ���λ
   @param  i:�����һ��2λʮ���Ƶ���
   @return ��
   @note   void itoa2(u8 i)
*/
/*----------------------------------------------------------------------------*/
void itoa2(u8 i)
{
    bcd_number[0] = asc_number[(i % 100) / 10];
    bcd_number[1] = asc_number[(i % 10)];
    bcd_number[2] = 0;
}
/*----------------------------------------------------------------------------*/
/**@brief  ��ȡһ��3λʮ�������ĸ���λ
   @param  i:�����һ��3λʮ������
   @return ��
   @note   void itoa3(u8 i)
*/
/*----------------------------------------------------------------------------*/
void itoa3(u16 i)
{
    bcd_number[0] = asc_number[(i % 1000) / 100];
    bcd_number[1] = asc_number[(i % 100) / 10];
    bcd_number[2] = asc_number[i % 10];
    bcd_number[3] = 0;
}
/*----------------------------------------------------------------------------*/
/**@brief  ��ȡһ��4λʮ���Ƶ����ĸ���λ
   @param  i:�����һ��4λʮ���Ƶ���
   @return ��
   @note   void itoa4(u8 i)
*/
/*----------------------------------------------------------------------------*/
void itoa4(u16 i)
{
    bcd_number[0] = asc_number[(i % 10000) / 1000];
    bcd_number[1] = asc_number[(i % 1000) / 100];
    bcd_number[2] = asc_number[(i % 100) / 10];
    bcd_number[3] = asc_number[i % 10];
    bcd_number[4] = 0;
}

void itoa2_api(u8 i,u8 * buf)
{
    itoa2(i);
    buf[0] = bcd_number[0];
    buf[1] = bcd_number[1];
}

void itoa4_api(u16 i,u8 * buf)
{
    itoa4(i);
    buf[0] = bcd_number[0];
    buf[1] = bcd_number[1];
    buf[2] = bcd_number[2];
    buf[3] = bcd_number[3];
}
#endif
