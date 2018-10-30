#include "sdk_cfg.h"
#include "ui/lcd/lcd128X64.h"
#include "ui/lcd/lcd_drv_interface.h"
#include "ui/lcd/lcd_drv_api.h"
#include "ui/lcd/lcd_spi.h"
#include "ui/ui_api.h"
#include "common/common.h"

#if LCD_128X64_EN

///LCD ��ʼ�������
const u8 lcd_init_table[] =
{
	LCD_SOFT_RST,
	LCD_BIAS_SET | 0x00,   //lcd bias setting
	LCD_ADC_SEL,           //0XA0 ADC
	LCD_COMMON_MODE,       //0xc8 common output mode
	(LCD_V0_RATIO | 0x03), //Resistor Ratio
	LCD_POWER_MODE,        //0x2f lcd power control
	LCD_START_LINE | 0,    //0x40
	LCD_ON,
	LCD_ELE_VOLUME,        //0X81 /electronic Volume
	0x22,                  //0x32//
	LCD_DISP_BW,
};

/*----------------------------------------------------------------------------*/
/**@brief    ������ʾ����ʼ����
   @param     x:������
   @param     y:������
   @return
   @note      void lcd_set(u8 y,u8 x)
*/
/*----------------------------------------------------------------------------*/
void lcd_128x64_set(u8 page,u8 column)
{
    lcd_sendcmd((u8)(LCD_COLUMN_HIGH | column>>4));
    lcd_sendcmd((u8)(LCD_COLUMN_LOW | (0x0f & column)));
    lcd_sendcmd((u8)(LCD_PAGE_ADDR | page));
}

/*----------------------------------------------------------------------------*/
/**@brief    ���öԱȶ�,lcd�Աȶȷ�Χ������0-63
   @param    c:�Աȶȵ�ֵ��lcd�Աȶȷ�Χ������0~60
   @return   void
   @note     void lcd_set_contrast(u8 c)
*/
/*----------------------------------------------------------------------------*/
void lcd_set_contrast(u8 c)
{
    if(c > 60)
	{
		c = 34;
	}
    lcd_sendcmd(0x81);
    lcd_sendcmd(c);    //lcd�Աȶȷ�Χ������27-43
}

/*----------------------------------------------------------------------------*/
/**@brief    LCD Ӳ����ʼ��
   @param    void
   @return   void
   @note     void lcd_hardware_init(void)
*/
/*----------------------------------------------------------------------------*/
void lcd_hardware_init(void)
{
    tu8 i;
    u8 flag;

    ui_puts("lcd_hardware_init\n");

    lcd_spi_init(); //LCD SPI init

    for(i = 0; i < sizeof(lcd_init_table); i++)
	{
		lcd_sendcmd(lcd_init_table[i]);
		delay(10);
	}

    lcd_set_contrast(35);
    lcd_clear();

    ui_puts("lcd_hardware_init OK\n");
}

#endif/*LCD_128X64_EN*/

