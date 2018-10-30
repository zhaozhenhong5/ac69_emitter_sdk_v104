#ifndef __UI_CON_H__
#define __UI_CON_H__

#include "typedef.h"
#include "string.h"
// #include "font_out.h"
#include "imb_module.h"
#include "sydf/syd_file.h"




#define USE_ORIG_COLOR      0x00
#define USE_NEW_COLOR       0x01
#define ERASE_BACKGROUND    0x02

//�ı�����
#define ANSI			0x01
#define UNICODE_BE		0x02
#define UNICODE_LE		0x04
#define	SCROLL_RESET	0x08
#define	TEXT_CENTRED    0x10

typedef struct
{
	u8 *buf;
	u16 len;
	u8 flags;		//�������� UNICODE_BE��UNICODE_LE��ANSI
}DTEXT;

typedef struct
{
	u16 year;		//��
	u8 month;		//��
	u8 day;			//��
	u8 hour;		//ʱ
	u8 minute;		//��
	u8 second;		//��
}TIME;

#define MENU_NODE  u8

enum
{
	MENU_INIT,
	MENU_ACTIVE,
	MENU_SELECT,
};


#define OTHER_SCREEN		0xfe//��������
#define NO_MENU				0xff

typedef u8 (*MenuFun)(u16 id, u8 mode);//�˵�����ָ��
typedef struct
{
    u16 id;					//�˵���������
    MENU_NODE child_node;	//��һ���˵� NO_MENU:���¼��˵� ����:�¼��˵�����
} S_MENU;

typedef struct
{
	u8 layer;
	u8 color;
}BORDER;//ͼ���ɰ�

//�˵���ʽ
typedef enum
{
    STYLE_NORMAL,       //����
    STYLE_ONE_CHOICE,   //��ѡ
    STYLE_MULTI_CHOICE, //��ѡ
    STYLE_USER_DEFINED, //�Զ���
	STYLE_CONFIRM,		//ȷ��
} MENU_STYLE;

//�˵���Ϣ
enum
{
	UI_MSG_PREV,
	UI_MSG_NEXT,
	UI_MSG_CONFIRM,
	UI_MSG_100MS,
	UI_MSG_200MS,
	UI_MSG_500MS,
	UI_MSG_RETURN,
	UI_MSG_EXIT,
	UI_MSG_INVALID,
};

typedef struct
{
	u8 layer;
	u16 x;
	u16 y;
	u16 width;
	u16 height;
	u16 currpage;
	u16 totalpage;
}MENU_INFO;

typedef struct
{
    MENU_NODE father_node;  //��һ���˵�
    MENU_NODE node;			//��ǰ�˵�����
    u16 title;				//�˵�����,0�ޱ���,��0�б���
    MENU_STYLE style;		//�˵���ʽ 0:���� 1:��ѡ 2:��ѡ 3.�Զ���
    const S_MENU  *menu;	//�˵��� ָ��S_MENU��ָ��
    const u16  *pic;		//ͼ�� ָ��˵���ͼ���ָ��,ֻ��ѡ���Զ�����ʽ��Ч,	û������ΪNULL
    MenuFun fun;			//�˵��ص�����
    u8 menuCnt;				//�ü��˵�����
}S_MENULIST;

typedef void (*MenuHook)(MENU_INFO *info);
typedef u16 (*MsgHook)(void);
typedef void (*KeyHook)(void);
//typedef void (*LcdHook)(u8 start_page, u8 page_len,u8 * pbuf);
typedef struct
{
	MenuHook title;
	MenuHook bottom;
	MsgHook  msg_hook;
	KeyHook  key_in_hook;
	KeyHook  key_out_hook;
//	LcdHook  lcd_draw;
	S_MENULIST *list;
	u16 timeout;		//��ʱ�˳�ʱ������(��λ����),Ϊ0���ֹ��ʱ�˳�
	OSDCOLOR act_color;		//��˵���������ɫ
	OSDCOLOR unact_color;	//�ǻ�˵������ɫ
	BORDER border;			//ͼ���ɰ�߿�
	u16 selpic2;			//����ѡ��ͼƬ2(�Ӳ˵�)
	s16 icon_offset;        //�˵����ͼ��ƫ��
	s16 child_area_x;       //�Ӳ˵�����xƫ��
	s16 child_area_y;       //�Ӳ˵�����yƫ��
	s16 child_area_width;   //�Ӳ˵�����widthƫ��
	s16 child_area_height;  //�Ӳ˵�����heightƫ��
}MPARAM;


typedef struct
{
	u32 min_val;
	u32 curr_val;
	u32 max_val;
	u16 total_len;
	u16 curr_pos;
}DSLIDER;


#define UI_SLIDER_CURR   0
#define UI_SLIDER_PREV   1
#define UI_SLIDER_NEXT   2
typedef u16 (*SLIDER_HOOK)(u8 mode,DSLIDER *s_param);
//mode����ʾ��ȡ��ǰֵ��ǰһλ��ֵ����һλ��ֵ


typedef struct
{
	u16 id;				//id��
	u8 type;			//�ؼ�����
	void *p;			//����
	u8 state;			//״̬ ��ʾ/����/�ػ�
	GUI_RECT rect;		//����
	void *pNextObj;		//ָ����һ���ؼ�
}WIN;

#ifndef _STRUCT_LAYER
#define _STRUCT_LAYER
typedef struct
{
	u8 /*LAYER_TYPE*/type;	//ͼ�����
	u8 alpha;			//ͼ��͸���ȣ�64 level(0~63)
	u16 x;				//x����
	u16 y;				//y����
	u16 width;			//ͼ����
	u16 height;			//ͼ��߶�
}__attribute__((packed)) LAYER;//10 bytes
#endif

#ifdef WIN32
#pragma pack(pop)
#endif

extern sdfile_t resfile;
extern sdfile_t styfile;

enum
{
	SHOW_CURR_VALUE,
	SHOW_NEXT_VALUE,
	SHOW_PREV_VALUE,
};


//�ؼ�����
enum
{
	TYPE_PIC,
	TYPE_TEXT,
	TYPE_TIME,
	TYPE_MENU,
	TYPE_NUMBER,
	TYPE_SLIDER,
	TYPE_PROGRESS,
	TYPE_RADIO,
	TYPE_CHECKBOX,
	TYPE_BUTTON,
	TYPE_LAYER,
	TYPE_CLRSCR,
	TYPE_DRAWSTR,
	TYPE_DRAWPIC,
};

typedef struct
{
	u32 flag;
	u16 fileversion;
	u16 filecount;
}STYFILEHEADER;

typedef struct
{
	u32 num;			//����ؼ�����
	u32 offset;			//����ؼ�ƫ�Ƶ�ַ
} __attribute__((packed)) CONTROL_HEADER;		//8 bytes

typedef struct
{
	u16 x;
	u16 y;
	u16 id;
}PIC;

//1.ͼƬ�ؼ�
typedef struct
{
	u8 layer;
	u16 x;		//x����
	u16 y;		//y����
	u16 width;	//ͼƬ���
	u16 height;	//ͼƬ�߶�
	u16 id;		//ͼƬid��
} __attribute__((packed)) CPIC; //11 Bytes

//2.�ı��ؼ�
typedef enum
{
	UI_STATIC_TEXT,			//��̬��ʾ
	UI_SCROLL_TEXT,			//������ʾ
	UI_KLOK_TEXT,			//����OK������ʾ
}DISP_MODE;

typedef struct
{
	u8 layer;				//ͼ��
	u16 x;					//��ʼx����
	u16 y;					//��ʼy����
	u16 width;				//��ʾ����Ŀ�
	u16 height;				//��ʾ����ĸ�
	u16 backpicture;		//����ͼƬ 0:�ޱ���ͼƬ,������ɫ��Ч ����:����ͼƬID��,������ɫ��Ч
	u8 backcolor;			//������ɫ
	u8 forecolor;			//ǰ����ɫ
	u8 /*FONT_SIZE*/ font_size;	//�ֺ�,֧��С(16x16),��(20x20),��(24x24)�����ֺ�
	u8 /*DISP_MODE*/ dispmode;		//��ʾ��ʽ
	//u8 ttt;
} __attribute__((packed)) CTEXT; //16 Bytes
//˵��:������ɫ��ǰ����ɫΪ8λɫ

//ʱ��ؼ�����
//0:	hh:mm					ʱ�ӣ�����
//1:	hh:mm:ss				ʱ�ӣ����ӣ�����
//2:	yyyy-mm-dd hh:mm:ss		��-��-�� ʱ�ӣ����ӣ�����

//3.ʱ��ؼ�
typedef struct
{
	u8 layer;
	u16 x;				//��ʼx����
	u16 y;				//��ʼy����
	u16 width;			//�ؼ����
	u16 height;			//�ؼ��߶�
	u8  type;			//ʱ����ʾ����
	u16 num_id[12];		//0~9Ϊ���ֵ�ID�ţ�10~11Ϊ�ָ���ID��,ǰ�������ͼ�¼��num_id[10],���һ�ּ�¼��num_id[11]
}__attribute__((packed)) CTIME;// 26 Bytes

//4.�˵��ؼ�
typedef struct
{
	u8 layer;
	u16 x;					//��ʼx����
	u16 y;					//��ʼy����
	u16 width;				//�˵����
	u16 height;				//�˵��߶�
	u16 mItemWidth;			//�˵������ͼ����
	u16 mItemHeight;		//�˵���߶�
	u16 backpic;			//����ͼƬ				ͼƬ�ؼ�id��
	u16 titlepic;			//������ͼƬ
	u16 bottompic;			//״̬��ͼƬ
	u16 activepic;			//���ͼ��
	u16 unactivepic;		//�ǻ��ͼ��
	u16 OneChoiceSel;		//��ѡ��ѡ��ͼ��
	u16 OneChoiceNoSel;		//��ѡδѡ��ͼ��
	u16 MultiChoiceSel;		//��ѡ��ѡ��ͼ��
	u16 MultiChoiceNoSel;	//��ѡδѡ��ͼ��
	u16 selpic;				//����ѡ��ͼƬ1
	u16 scrollbar;			//������
	u16 scrollbar_p;		//������ͼƬID��
	u8 menuNumber;          //�˵�����
}__attribute__((packed)) CMENU; //40 Bytes

//5.�������ؼ�
typedef struct
{
	u8 layer;
	u16 x;				//��ʼx����
	u16 y;				//��ʼy����
	u16 width;			//�ؼ����
	u16 height;			//�ؼ��߶�
	PIC bar;			//������ͼƬid��
	PIC slider;			//������ͼƬid��
	u16 cnum[3];		//���ֿؼ���id��,�ֱ��ʾ��Сֵ/��ǰֵ/���ֵ		���ֿؼ�id
	u16 backpic;		//����ͼƬ�ؼ�ID��								ͼƬ�ؼ�id
}__attribute__((packed)) CSLIDER;

//6.�������ؼ�
typedef struct
{
	u8 layer;
	u16 x;				//��ʼx����
	u16 y;				//��ʼy����
	u16 width;			//�ؼ����
	u16 height;			//�ؼ��߶�
	PIC bar;			//����С����
	PIC progress;		//������
	PIC percent;        //�ٷֺ�
	u16 number;			//���ֿؼ�id��
	u16 backpicture;	//����ͼƬ					ͼƬ�ؼ�id
}__attribute__((packed)) CPROGRESS;//31byte

//7.���ֿؼ�
typedef struct
{
	u8 layer;
    u16 x;          //��ʼx����
	u16 y;          //��ʼy����
	u16 width;      //�ؼ����
	u16 height;	   //�ؼ��߶�
    u16 num_id[10];  //����0-9��ͼƬID��
    u16 num_bits;   //��ֵ��Чλ��
}__attribute__((packed)) CNUMBER;

//8.��ѡ�ؼ�
typedef struct
{
	u8 layer;
	u16 x;		//x����
	u16 y;		//y����
	u16 width;	//�ؼ����
	u16 height;	//�ؼ��߶�
	PIC sel;	//ѡ��ͼ��
	PIC unsel;	//δѡ��ͼ��
	PIC strid;  //�ַ���id��
}__attribute__((packed)) CRADIO;//27

//9.��ѡ�ؼ�
typedef struct
{
	u8 layer;
	u16 x;			//x����
	u16 y;			//y����
	u16 width;		//�ؼ����
	u16 height;		//�ؼ��߶�
	PIC sel;		//ѡ��ͼ��
	PIC unsel;		//δѡ��ͼ��
	PIC strid;		//�ַ���id��
	u8 status;      //Ĭ����ѡ��״̬����δѡ��״̬
}__attribute__((packed)) CHECKBOX;//28

//10.��ť�ؼ�
typedef struct
{
	u8 layer;
	u16 x;			//x����
	u16 y;			//y����
	u16 width;		//�ؼ����
	u16 height;		//�ؼ��߶�
	PIC picid;		//��ťͼƬ
	PIC strid;		//��ť�ı�
}__attribute__((packed)) CBUTTON;

//11.ͼ��
#ifndef _STRUCT_LAYER
#define _STRUCT_LAYER
typedef struct
{
	u8 /*LAYER_TYPE*/type;	//ͼ�����
	u8 alpha;			//ͼ��͸���ȣ�64 level(0~63)
	u16 x;				//x����
	u16 y;				//y����
	u16 width;			//ͼ����
	u16 height;			//ͼ��߶�
}__attribute__((packed)) LAYER;//10 bytes
#endif

#ifdef WIN32
#pragma pack(pop)
#endif

bool ui_init(u16 width,u16 height);
bool ui_pic(u16 id,GUI_POINT *pos);
bool ui_menu(u16 id,MPARAM *pMenu);
bool ui_lcd_menu(u16 id,MPARAM *pMenu);
///flags �Ƿ�ȥ������ǰ��0��0����ȥ������0��ȥ��
bool ui_number(u16 id,u32 num,u16 x,u16 y,u8 flags);
bool ui_time(u16 id,TIME *time,u16 x,u16 y,u8 flags);
u16 ui_text(u16 id,DTEXT *text);
bool ui_progress(u16 id,u8 percent);
bool ui_slider(u16 id,SLIDER_HOOK hook,u8 cmd);
void ui_clear_screen(u8 layer,u16 x,u16 y,u16 width,u16 height,u32 color);
bool ui_layer(u16 id,LAYER *layer);
bool ui_radio(u16 id,u8 status);
bool ui_check(u16 id,u8 status);
bool ui_button(u16 id);

#endif
