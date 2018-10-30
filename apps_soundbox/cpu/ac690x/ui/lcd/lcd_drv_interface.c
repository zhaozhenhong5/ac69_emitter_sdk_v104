#include "sdk_cfg.h"
#include "common/common.h"
#include "ui/lcd/lcd_drv_interface.h"
#include "ui/lcd/lcd_drv_api.h"
#include "ui/ui_api.h"
#include "ui/lcd/lcd_spi.h"
#include "uicon/dc_showres.h"

#if LCD_128X64_EN

//ȫ�ֱ���
volatile bool lcd_busy;         ///<LCDӲ��æ���
u8 lcd_updata_page_len;         ///���µ�PAGE��Ŀ
u8 lcd_currPage;                ///<��ǰPAGE


///>LCD �ӿں���
const UI_LCD_IO ui_lcd_io =
{
    .lcd_clear_area_callback            = lcd_clear_area,
    .lcd_clear_area_with_draw_callback  = lcd_clear_area_with_draw,
    .lcd_clear_area_rect_callback       = lcd_clear_area_rect,
    .lcd_TurnPixelReverse_Page_callback = lcd_TurnPixelReverse_Page,
    .lcd_TurnPixelReverse_Rect_callback = lcd_TurnPixelReverse_Rect,
};

///>LCD �ؼ��ӿ�
const LCD_INFO lcd_info =
{
    .ui_lcd_callback = (UI_LCD_IO *)&ui_lcd_io,
    .lcd_height = SCR_HEIGHT,
    .lcd_width = SCR_WIDTH,
    .lcd_buff = disp_buf,
};


/*----------------------------------------------------------------------------*/
/**@brief    ������ָ������ֻ��BUFF��������
   @param    start_page:��ʼ page
   @param    end_page:����page
   @return   void
   @note     void lcd_clear_area(u8 start_page, u8 end_page)
*/
/*----------------------------------------------------------------------------*/
void lcd_clear_area(u8 start_page, u8 end_page)
{
    for (; start_page < end_page; start_page++)
    {
        my_memset((disp_buf + start_page *LCDCOLUMN), 0x00, LCDCOLUMN);
    }
}

/*----------------------------------------------------------------------------*/
/**@brief    ������ָ������ֻ��BUFF���Ҹ��µ��Դ�
   @param    start_page:��ʼ page
   @param    end_page:����page
   @return   void
   @note     void lcd_clear_area_with_draw(u8 start_page, u8 end_page)
*/
/*----------------------------------------------------------------------------*/
void lcd_clear_area_with_draw(u8 start_page, u8 end_page)
{
    for (; start_page < end_page; start_page++)
    {
        my_memset((disp_buf + start_page *LCDCOLUMN), 0x00, LCDCOLUMN);
        lcd_draw(start_page, 1, disp_buf);
    }
}

/*----------------------------------------------------------------------------*/
/**@brief    ������ָ�������ָ����ȣ�ֻ��BUFF��������
   @param    start_page:��ʼ page
   @param    end_page:����page
   @param    x0:��ʼ��
   @param    x1:������
   @return   void
   @note     void lcd_clear_area_rect(u8 start_page, u8 end_page, u8 x0, u8 x1)
*/
/*----------------------------------------------------------------------------*/
void lcd_clear_area_rect(u8 start_page, u8 end_page, u8 x0, u8 x1)
{
    for (; start_page < end_page; start_page++)
    {
        my_memset((disp_buf + start_page *LCDCOLUMN + x0), 0x00, x1- x0);
    }
}

/*----------------------------------------------------------------------------*/
/**@brief    ҳ����ȡ��ʵ�ֺ���
   @param    startpage: ȡ��page
   @param    pagelen��  ȡ��page�ĸ���
   @return   ����ȡ�����
   @note     u32 lcd_TurnPixelReverse_Page(u8 startpage, u8 pagelen)
*/
/*----------------------------------------------------------------------------*/
u32 lcd_TurnPixelReverse_Page(u8 startpage, u8 pagelen)
{
    u8 hnum,l;

    if( (startpage > LCDPAGE) || (pagelen > LCDPAGE) )
        return FALSE;

    for(hnum = startpage; hnum < startpage + pagelen; hnum = hnum + 1)
    {
        for(l = 0; l < LCDCOLUMN; l++)
        {
            *(disp_buf + hnum * LCDCOLUMN + l) =  ~(*(disp_buf + hnum * LCDCOLUMN + l));
        }
    }
    return TRUE;
}

/*----------------------------------------------------------------------------*/
/**@brief    ����ȡ��ʵ�ֺ���
   @param    left:   ȡ���������߽���
   @param    top��   ȡ������Ķ�������
   @param    right�� ȡ��������ұ߽���
   @param    bottom��ȡ������ĵײ�����
   @param    left��rightȡֵ��Χ��0--(LCDCOLUMN-1)  ;  top��bottomȡֵ��Χ��0--(LCDPAGE-1)
   @return   ����ȡ�����
   @note     u32 lcd_TurnPixelReverse_Rect(u8 left, u8 top, u8 right, u8 bottom)
*/
/*----------------------------------------------------------------------------*/
u32 lcd_TurnPixelReverse_Rect(u8 left, u8 top, u8 right, u8 bottom)
{
    u8 hnum,l;

    if( (left >= LCDCOLUMN) || (top >= LCDPAGE) || (left >= right) || (top >= bottom) )
        return FALSE;

    for(hnum = top; hnum < bottom + 1; hnum = hnum + 1)
    {
        for(l = left; l < right; l++)
        {
            *(disp_buf + hnum * LCDCOLUMN + l) = ~(*(disp_buf + hnum * LCDCOLUMN + l));
        }
    }

    return TRUE;
}


void dc_clear_screen_area(u32 x, u32 y, u32 width, u32 height)
{
    u32 i;
    u32 h, w;

    if(x >= SCR_WIDTH || y >= SCR_HEIGHT)
    {
        return;
    }

    h = ((y + height) > SCR_HEIGHT) ? (SCR_HEIGHT - y) : height;
    w = ((x + width) > SCR_WIDTH) ? (SCR_WIDTH - x) : width;

    for(i=0; i<h; i++)
    {
        memset((u8*)&disp_buf[(y + i) * 128 + x], 0x00, w);
    }

    //lcd_draw_screen(0, dc_screen_height, dc_displaybuf);
}


void lcd_set(u8 page, u8 column)
{
    lcd_128x64_set(page, column);
}

/*----------------------------------------------------------------------------*/
/**@brief   lcd spi��ʼ��
   @param   void
   @return  void
   @note    void lcd_spi_init(void)
*/
/*----------------------------------------------------------------------------*/
void lcd_sendcmd(u8 cmd)
{
    SPI1_init(0, 0x10);
    LCD_A0_L();
    delay(10);
    LCD_CS_L();
    delay(10);
    SPI1_WriteByte(cmd);
    LCD_CS_H();
}

/*----------------------------------------------------------------------------*/
/**@brief   ����DMA��LCD
   @param   ��
   @return  0��ҪDMA��ҳ��� 1��ҪDMA��ҳ��û��
   @note    u32 lcd_write_dma(void)
*/
/*----------------------------------------------------------------------------*/
u32 lcd_write_dma(u8 * pbuf)
{
    SPI1_init(0, 0x10);
    LCD_CS_H();
    if (lcd_updata_page_len == 0)
    {

        lcd_busy = 0;
        return 0;
    }
    lcd_busy = 1;
    lcd_set(lcd_currPage, 0);
    LCD_A0_H();
    delay(10);
    LCD_CS_L();

    SPI1_DmaWrite(pbuf + lcd_currPage*LCDCOLUMN, LCDCOLUMN);
//    for(i = 0;i<LCDCOLUMN;i++)
//        SPI1_WriteByte(pbuf[lcd_currPage*LCDCOLUMN+i]);

    lcd_currPage++;
    lcd_updata_page_len--;

    return 1 ;
}

/*----------------------------------------------------------------------------*/
/**@brief   �����Դ�����ݵ�LCD
   @param   start_page �����¿�ʼ����ʼPage
   @param   page_len �����µ�Page��
   @return
   @note    void draw_lcd(u8 start_page, u8 page_len)
*/
/*----------------------------------------------------------------------------*/
void lcd_draw(u8 start_page, u8 page_len, u8 * pbuf)
{
    //flush_dcache(pbuf, LCDPAGE*LCDCOLUMN);//liwenhong
    while (lcd_busy);
    lcd_currPage = start_page;
    lcd_updata_page_len = page_len;
    //printf_buf(pbuf,128*8);
    while(lcd_write_dma(pbuf));
//    printf("over");
}


/*----------------------------------------------------------------------------*/
/**@brief   lcd spi��ʼ��
   @param   void
   @return  void
   @note    void lcd_spi_init(void)
*/
/*----------------------------------------------------------------------------*/
void lcd_spi_init(void)
{
    ui_puts("lcd_spi_init\n");

    SPI1_init(0, 0x10);//����ģʽ��������ϵ��Ϊ16

    LCD_PORT_OUT();
    LCD_PORT_OUT_H();

    SPI1_init(0, 0x10);//����ģʽ��������ϵ��Ϊ16

    ui_puts("lcd_spi_init end\n");
    LCD_RES_L();
    delay(10);
    LCD_RES_H();
    delay(10);
}

#else

LCD_INFO *lcd_parm;
void lcd_draw(u8 start_page, u8 page_len, u8 * pbuf)
{
}

#endif/*LCD_128x64_EN*/

