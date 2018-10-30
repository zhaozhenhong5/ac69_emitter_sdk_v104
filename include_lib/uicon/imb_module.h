#ifndef __IMB_MODULE_H__
#define __IMB_MODULE_H__

#include "typedef.h"
//#include "dsc_ram.h"

struct imb_isr_work{
	void * param;
	void (*handler)(void * param);
};

// #define REGISTER_IMB_WORK(hdl, p)  const struct imb_isr_work imb_isr_##hdl \
                                        // __attribute__((section(".imb_isr_work"))) = {\
                                        // .param = (void *)p,\
                                        // .handler=hdl\
                                    // }
#define REGISTER_IMB_WORK(hdl, p)  const struct imb_isr_work imb_isr_##hdl \
                                        sec(.imb_isr_work) = {\
										.param = (void *)p,\
										.handler=hdl\
									}

#define SDRAM_ADDR(addr)  ((u32)(0x2000000+addr))
#define SET_REG(val,pos)  ((u32)val << pos)

typedef enum
{
	LAYER_IMAGE0,			//ͼƬ��0(û��͸����)
	LAYER_IMAGE1,			//ͼƬ��1
	LAYER_OSD0_WIN0,		//OSD��0 ����0
	LAYER_OSD0_WIN1,		//OSD��0 ����1
	LAYER_OSD0_WIN2,		//OSD��0 ����2
	LAYER_OSD1_WIN0,		//OSD��1 ����0
	LAYER_OSD1_WIN1,		//OSD��1 ����1
	LAYER_OSD1_WIN2,		//OSD��1 ����2
	LAYER_IMAGE1_MASK,  //MASK��(ͼƬ��1)
}LAYER_TYPE;//ͼ����
//˵����
//1.osd��һ��������,ÿ��osd������������ڣ������������ڴ�ֱ�����ϲ��ܽ���,��ͬ���osd�ڴ�ֱ��������໥����
//2.����ͼƬ��0û��͸���ȣ��������㶼��͸����
//3.��ͼ�㻹��Ҫ��ѭ�����ԭ��
//  ͼƬ��x,y,width,height����Ҫ4����
//  OSD��x,y,width,height����Ҫ2����


//������ɫ
typedef struct
{
	u8 forecolor;
	u8 backcolor;
}OSDCOLOR;

typedef struct
{
	u16 x;
	u16 y;
}TEXTPOS;
extern TEXTPOS text_pos;

typedef struct
{
	u16 x;
	u16 y;
	u16 width;
	u16 height;
}GUI_RECT;

typedef struct
{
	u16 x;
	u16 y;
}GUI_POINT;


#define OSD0            0x00
#define OSD1            0x01
#define OSD_ALL         0x02
#define OSD1_B2         0x03   //2bitģʽ��ֻ��OSD1��Ч�����ڿ��16 bytes����
#define WINDOW0         0x00
#define WINDOW1         0x10
#define WINDOW2         0x20

//<��ʾ�豸>
enum
{
    DISP_RGB,
    DISP_MCU,
    DISP_HDMI,
};

//<�ӿڲ���>
#define MODE_RGB_SYNC   0x00//���Դ� ����֡  ��HSYNC VSYNC�ź�
#define MODE_RGB_DE     0x01//���Դ� ����֡  ��DE�ź�
#define MODE_MCU        0x02//���Դ� ��֡
#define MODE_HDMI       0x03//HDMI��ʾ
#define MODE_AVOUT      0x04//AV OUT��ʾ
#define MODE_AVOUT1     0x05

#define PORT_1BIT       0x00//AV OUT
#define PORT_8BITS      0x01
#define PORT_16BITS     0x03
#define PORT_18BITS     0x07
#define PORT_24BITS     0x0f

//<�����ʽ>
#define FORMAT_RGB888   0x01
#define FORMAT_YUV888   0x02
#define FORMAT_RGB565   0x03
#define FORMAT_YUV422   0x04
#define FORMAT_ITU656   0x05

enum
{
    CFG_OSD_ADDR,  //����OSD��ɫ����ʼ��ַ(����OSD0��OSD1)
    CFG_OSD0_DAT,  //����OSD0��ɫ������
    CFG_OSD1_DAT,  //����OSD1��ɫ������
    CFG_ALL_DAT,   //ͬʱ����OSD0��OSD1��ɫ������
};

typedef struct
{
    u16 h_total_width;  //ˮƽʱ������
    u16 h_sync_width;   //ˮƽͬ��ʱ��
    u16 h_act_start;    //ˮƽ��ʼʱ��
    u16 h_act_width;    //ˮƽ���
    u16 v_total_height; //��ֱʱ������
    u16 v_sync_height;  //��ֱͬ��ʱ��
    u16 v_act_start;    //��ֱ��ʼʱ��
    u16 v_act_start1;   //��ֱ��ʼʱ��(����ģʽ)
    u16 v_act_width;    //��ֱ���
}L0_PARA_CFG;


//OSDͼ�����
typedef struct _OSDWIN
{
	u16 x;
	u16 y;
	u16 width;
	u16 height;
	u8 *buf;
    u8 layer;
	u8 alpha;
	u8 enable;
}OSDWIN;

typedef struct _OSDLAYER
{
	u32 *palette;
	u16 item;
	OSDWIN win[3];
}OSDLAYER;

//IMAGEͼ�����
typedef struct
{
    u8 layer;
    u8 enable;
    u8 alpha;       //ͼ��1��Ч
    u8 mask_en;     //ͼ��1��Ч
    u8 buf_num;
    u16 x;
    u16 y;
    u16 width;
    u16 height;
//    u8 *ay;
//    u8 *au;
//    u8 *av;
    u8 *buf;
    u8 *mask_buf;   //ͼ��1��Ч
    struct dsc_ram *dsc_ram;
}IMGLAYER;

//MASKͼ�����
typedef struct
{
    u16 x;
    u16 y;
    u16 width;
    u16 height;
    u8  *buf;
    u8  enable;
}MASKLAYER;

//ת��ͼ�����
typedef struct
{
    u32 y_offs;
    u32 y_gain;
    u32 u_gain;
    u32 v_gain;
    u32 r_gain;
    u32 g_gain;
    u32 b_gain;
}CGAIN;


typedef struct
{
    u8 drive_mode;         //��������
    u8 data_width;         //���ݽӿ�λ��
    u8 out_format;         //������ݸ�ʽ
    u8 isr_prior;          //�ж����ȼ�
    u32 bkcolor;           //������ɫ
    L0_PARA_CFG *para;     //ʱ������
    CGAIN *gain;           //��ɫ����
    void (*hook) (u16 x,u16 y,u16 w,u16 h);   //���ÿ�������ֻ���MCU�ӿڵ���
    u8 interface_mode;
}DISP_DEV_CFG;

extern IMGLAYER image[2];
extern OSDLAYER osdlayer[2];
extern MASKLAYER mask;

extern u32 tempaddr;
extern u16 tempwidth,tempheight;

#define SET_A(addr,width,height)  tempaddr = addr; tempwidth = width; tempheight = height;
#define GET_AY()  (tempaddr)
#define GET_AU()  (tempaddr+tempwidth*tempheight)
#define GET_AV()  (tempaddr+tempwidth*tempheight*5/4)

//OSDͼ����ɫ����
#define COLOR_LUCENCY   0      //͸��
#define COLOR_YELLOW    5      //��ɫ
#define COLOR_RED       35     //��ɫ
#define COLOR_GREEN     225    //��ɫ
#define COLOR_BLUE		235    //��ɫ
#define COLOR_WHITE		245    //��ɫ
#define COLOR_GRAY      250    //��ɫ
//#define COLOR_BLACK		255    //��ɫ
#define COLOR_BLACK		224    //��ɫ

/*----------------------------------------------------------------------------*/
/*                            IMB Module Function List                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/**@brief   ��ʼ��IMBģ��
   @param   dev:�豸����
   @return  ��
   @note    void imb_init(DISP_DEV_CFG* dev);
*/
/*----------------------------------------------------------------------------*/
extern void imb_init(DISP_DEV_CFG* dev);
/*----------------------------------------------------------------------------*/
/**@brief   ��ȡͼƬ����
   @param   layer:ͼ�����
   @return  ͼƬ����
   @note    IMGLAYER *imb_get_image_hdl(u8 layer);
*/
/*----------------------------------------------------------------------------*/
extern IMGLAYER *imb_get_image_hdl(u8 layer);
/*----------------------------------------------------------------------------*/
/**@brief   ͼƬ������
   @param   img:ͼƬ����
   @return  ��
   @note    void imb_image_cfg(IMGLAYER *img);
*/
/*----------------------------------------------------------------------------*/
extern void imb_image_cfg(IMGLAYER *img);
/*----------------------------------------------------------------------------*/
/**@brief   ��ȡOSD����
   @param   layer:ͼ�����
   @return  OSD����
   @note    OSDWIN *imb_get_osd_hdl(u8 layer);
*/
/*----------------------------------------------------------------------------*/
extern OSDWIN *imb_get_osd_hdl(u8 layer);
/*----------------------------------------------------------------------------*/
/**@brief   OSD������
   @param   oshdl:OSD����
   @return  ��
   @note    void imb_osd_win_cfg(OSDWIN *osdhdl);
*/
/*----------------------------------------------------------------------------*/
extern void imb_osd_win_cfg(OSDWIN *osdhdl);
/*----------------------------------------------------------------------------*/
/**@brief   ����IMBģ��
   @param   ��
   @return  ��
   @note    void imb_start(void);
*/
/*----------------------------------------------------------------------------*/
extern void imb_start(void);
/*----------------------------------------------------------------------------*/
/**@brief   ��IMBģ��
   @param   ��
   @return  ��
   @note    void imb_open(void);
*/
/*----------------------------------------------------------------------------*/
extern void imb_open(void);
/*----------------------------------------------------------------------------*/
/**@brief   �ر�IMBģ��
   @param   ��
   @return  ��
   @note    void imb_close(void);
*/
/*----------------------------------------------------------------------------*/
extern void imb_close(void);


#endif
