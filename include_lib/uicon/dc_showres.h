#ifndef __DC_SHOWRES_H__
#define __DC_SHOWRES_H__

#include "typedef.h"
#include "imb_module.h"

#ifdef WIN32
#pragma pack(push)
#pragma pack(1)
#endif

typedef struct
{
	u8 magic[4]; //'R', 'U', '2', 0x19
	u16 counts;  //��Դ�ĸ���,��ʱ����
	u32 lang;	//�ַ�������������,1 bit��ʾһ������,��22������
	u16 sindex;  //�ַ�����ʼ����
} __attribute__((packed))  RES_HEAD_T; //10�ֽ�

/*��Դ����������Ŀ¼������ݽṹ*/
typedef struct
{
	u32 dwOffset; //��Դ�����������ƫ��
	u16 wCount; 	 //��Դ��������
	u8 bItemType; //'P'--PIC Table,'S'--String Table,'X' -- XML File
	u8 bPanelType;
} __attribute__((packed))  RES_ENTRY_T; //8�ֽ�

enum
{
    LCD_TYPE = 0x0,
    TFT_TYPE,
};

enum
{
	RGB_1BIT,
	RGB_8BIT,
	RGB_16BIT,
	RGB_24BIT,
	YUV420,
	JPEG,
};

typedef struct
{
	u16 nNum;		//��ɫ�������ɫ��
	u32 dwOffset;	//��ɫ�����������ļ���ƫ��
	u32 dwLength;	//��ɫ�����ݴ�С
} __attribute__((packed))  RES_PAL_T;  //6�ֽ�

typedef struct
{
	u8 wType; 		//ͼƬ���ͣ�������ʾ
	u16 wWidth; 		//ͼƬ���(������Ϊ��λ)
	u16 wHeight; 	//ͼƬ�߶�(������Ϊ��λ)
	u32 dwOffset; 	//���������ļ���ƫ��,4 bytes
	u32 dwLength; 	//ͼƬ���ݴ�С,���4G,4 bytes
} __attribute__((packed))  RES_PIC_T; //13�ֽ�

typedef struct
{
	u16 wWidth; 		//�ַ����ܿ��(������Ϊ��λ)
	u16 wHeight; 	//�ַ����߶�(������Ϊ��λ)
	u32 dwOffset; 	//���������ļ���ƫ��,4 bytes
	u32 dwLength; 	//�ַ������ݴ�С,���4G��4 bytes
} __attribute__((packed)) RES_STR_T; //12�ֽ�

#ifdef WIN32
#pragma pack(pop)
#endif

typedef enum
{
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT,
}__attribute__((packed)) ALIGN_FLAG;

typedef struct _UI_LCD_IO
{
    void  (*lcd_clear_area_callback)(u8 start_page, u8 end_page);
    void  (*lcd_clear_area_with_draw_callback)(u8 start_page, u8 end_page);
    void  (*lcd_clear_area_rect_callback)(u8 start_page, u8 end_page, u8 x0, u8 x1);
    tbool (*lcd_TurnPixelReverse_Page_callback)(u8 startpage, u8 pagelen);
    tbool (*lcd_TurnPixelReverse_Rect_callback)(u8 left, u8 top, u8 right, u8 bottom);
}UI_LCD_IO;


typedef struct _LCD_INFO
{
    UI_LCD_IO *ui_lcd_callback;///LCD�ӿ�
    u16 lcd_width;  ///lcd���Ŀ��
    u16 lcd_height; ///lcd���ĸ߶�
    u8 *lcd_buff;    ///��ʾbuff
}LCD_INFO;
extern LCD_INFO *lcd_parm;

enum
{
    SD_SEEK_SET = 0x01,
    SD_SEEK_CUR = 0x02
};

extern u8 g_PanelType;

// void dc_lcd_pram_init(u16 width, u16 height, u8 *buf);
bool dc_init(void);
void dc_set_screen_scale(u8 x_scale,u8 y_scale);
bool dc_drawpic(u8 layer_index,u16 id,u16 x,u16 y,u8 osd_color);
bool dc_drawstring(u8 layer_index,u16 id,u16 x,u16 y,u16 limit_width,u8 align_mode,u8 step);
void dc_set_color(u8 forecolor,u8 backcolor);
void dc_get_color(OSDCOLOR *color);
u16 dc_getpicheight(u16 id);
u16 dc_getpicwidth(u16 id);
bool dc_osdpalette(u8 osdlayer,u32 *pal);
u16 dc_getstringwidth(u16 id);
u16 dc_getstringheight(u16 id);
//void dc_clr_rect(u16 x, u16 y, u16 width, u16 height);
// tbool dc_TurnPixelReverse_Rect(u8 left, u8 top, u8 right, u8 bottom);
// tbool dc_TurnPixelReverse_Page(u8 startpage, u8 pagelen);
// void dc_clear_area_rect(u8 start_page,u8 end_page,u8 x0,u8 x1);

#endif
