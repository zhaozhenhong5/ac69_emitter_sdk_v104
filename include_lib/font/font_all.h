#ifndef __FONT_ALL_H__
#define __FONT_ALL_H__

#include "typedef.h"
#include "sydf/syd_file.h"
#include "font_GBK.h"
#include "font_BIG5.h"
#include "font_SJIS.h"
#include "font_KSC.h"
#include "font_OtherLanguage.h"
#include "font_utf16toansi.h"
//#include "font_out.h"

#define ntohl(x)	(u32)((x>>24)|((x>>8)&0xff00)|(x<<24)|((x&0xff00)<<8))
#define ntoh(x)		(u16)((x>>8&0x00ff)|x<<8&0xff00)

#define  Chinese_Simplified       1   //��������
#define  Chinese_Traditional      2   //��������
#define  Japanese                 3   //����
#define  Korean                   4   //����
#define  English                  5   //Ӣ��
#define  French                   6   //����
#define  German                   7   //����
#define  Italian                  8   //�������
#define  Dutch                    9   //������
#define  Portuguese               10  //��������
#define  Spanish                  11  //��������
#define  Swedish                  12  //�����
#define  Czech                    13  //�ݿ���
#define  Danish                   14  //������
#define  Polish                   15  //������
#define  Russian                  16  //����
#define  Turkey                   17  //��������
#define  Hebrew                   18  //ϣ������
#define  Thai                     19  //̩��
#define  Hungarian                20  //��������
#define  Romanian                 21  //����������
#define  Arabic                   22  //��������

#ifdef WIN32
#pragma pack(push)
#pragma pack(1)
#endif
typedef struct
{
	u32 addr;
	u8 flag;
	u8 font_size;
	u8 font_nbytes;
}FONT_MATRIX;
#ifdef WIN32
#pragma pack(pop)
#endif

extern FONT_MATRIX fontinfo[3];
extern FONT_MATRIX ascfontinfo[3];
extern FONT_MATRIX *font;
extern FONT_MATRIX *ascfont;
extern u8 g_amplify;
extern u32 g_ConvertTableFileAddr;
extern u8 g_language_mode;
extern bool IsGB2312;
extern u8  pixBuf[100];

//extern u32 g_AsciiPixDataFileAddr;
//extern u32 g_PixDataFileAddr;
extern u32 g_ConvertTableFileAddr;


#endif
