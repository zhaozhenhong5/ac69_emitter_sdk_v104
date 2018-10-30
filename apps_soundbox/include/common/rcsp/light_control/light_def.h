#ifndef _LIGHT_DEF_H_
#define _LIGHT_DEF_H_

#include "../light_head.h"

#define FICKER_FREQUENCY     750L

//Port b
#define LED_W_IO (7)
#define LED_R_IO (3)
#define LED_G_IO (8)
#define LED_B_IO (9)


//�׹��ʼ������timer3ͨ��OUT_CH1�����
#define WHITE_LED_OPEN()     do{JL_PORTB->DIR &= ~BIT(LED_W_IO);JL_PORTB->PU |= BIT(LED_W_IO);JL_PORTB->PD |= BIT(LED_W_IO);\
                                JL_PORTB->DIE |= BIT(LED_W_IO);}while(0);

#define WHITE_LED_CLOSE()    do{JL_PORTB->DIR &= ~BIT(LED_W_IO);JL_PORTB->PU &= ~BIT(LED_W_IO);\
                                JL_PORTB->PD  &= ~BIT(LED_W_IO);JL_PORTB->OUT &= ~BIT(LED_W_IO);\
                                JL_PORTB->DIE &= ~BIT(LED_W_IO);}while(0);


//����ʼ�� ����timer2ͨ��PB3�����
#define R_LED_OPEN()         do{JL_PORTB->DIR &= ~BIT(LED_R_IO);JL_PORTB->PU &= ~BIT(LED_R_IO);JL_PORTB->PD &= ~BIT(LED_R_IO);\
                                JL_PORTB->DIE |= BIT(LED_R_IO);}while(0);

#define R_LED_CLOSE()        do{JL_PORTB->DIR &= ~BIT(LED_R_IO);JL_PORTB->PU &= ~BIT(LED_R_IO);\
                                JL_PORTB->PD  &= ~BIT(LED_R_IO);JL_PORTB->OUT &= ~BIT(LED_R_IO);JL_TIMER2->CON = 0x0000;\
                                JL_PORTB->DIE &= ~BIT(LED_R_IO);}while(0);


//�̹��ʼ�� ����timer3ͨ��OUT_CH1�����
#define G_LED_OPEN()         do{JL_PORTB->DIR &= ~BIT(LED_G_IO);JL_PORTB->PU |= BIT(LED_G_IO);JL_PORTB->PD |= BIT(LED_G_IO);\
                                JL_PORTB->DIE |= BIT(LED_G_IO);}while(0);

#define G_LED_CLOSE()        do{JL_PORTB->DIR &= ~BIT(LED_G_IO);JL_PORTB->PU &= ~BIT(LED_G_IO);\
                                JL_PORTB->PD  &= ~BIT(LED_G_IO);JL_PORTB->OUT &= ~BIT(LED_G_IO);\
                                JL_PORTB->DIE &= ~BIT(LED_G_IO);}while(0);


//�����ʼ������timer1ͨ��OUT_CH0�����
#define B_LED_OPEN()         do{JL_PORTB->DIR &= ~BIT(LED_B_IO);JL_PORTB->PU |= BIT(LED_B_IO);JL_PORTB->PD |= BIT(LED_B_IO);\
                                JL_PORTB->DIE &= ~BIT(LED_B_IO);}while(0);

#define B_LED_CLOSE()        do{JL_PORTB->DIR &= ~BIT(LED_B_IO);JL_PORTB->PU &= ~BIT(LED_B_IO);\
                                JL_PORTB->PD  &= ~BIT(LED_B_IO);JL_PORTB->OUT &= ~BIT(LED_B_IO);\
                                JL_PORTB->DIE &= ~BIT(LED_B_IO);}while(0);

//ö�ٽṹ��
enum
{
    LED_FLASH_100MS,
    LED_FLASH_300MS,
    LED_FLASH_500MS,
};

typedef enum _LIGHT_TYPE_
{
    WHITE_LED,
    RED_LED,
    GREEN_LED,
    BLUE_LED,
    MAX_LED,
}LIGHT_TYPE;

typedef enum _LIGHT_MODE_
{
    OP_WHITE_LED,   //�׳��
    MUSIC_FLICKER,  //������˸
    LED_FLICKER,    //ģʽ��˸
    LED_COLOUR,     //�ʵ�
    LED_SCENE_CTL, //�龰����
    LED_CLOSE_STATE, //CLOSE
}LIGHT_MODE_E;

typedef enum
{
    LIGHT_CMD_ON = 0,
    LIGHT_CMD_OFF,
    LIGHT_CMD_BRIGHT,
    LIGHT_CMD_CHOSE_MODE,
    LIGHT_CMD_CHOSE_SCENE,
}LIGHT_CMD_E;

typedef enum _MUSIC_FLICKER_MODE_
{
    COLORFUL_MUSIC_FLICKER, //�߲�������˸
    RED_MUSIC_FLICKER,      //��ɫ������˸
    ORANGE_MUSIC_FLICKER,   //��ɫ������˸
    YELLOW_MUSIC_FLICKER,   //��ɫ������˸
    GREEN_MUSIC_FLICKER,    //��ɫ������˸
    CYAN_MUSIC_FLICKER,     //��ɫ������˸
    BLUE_MUSIC_FLICKER,     //��ɫ������˸
    PURPLE_MUSIC_FLICKER,   //��ɫ������˸
    MUSIC_FLICKER_MAX,
}MUSIC_FLICKER_MODE;

typedef enum _LED_FLICKER_MODE_
{
    COLORFUL_SLOW_FLICKER,
    COLORFUL_GRADUAL_FLICKER,
    COLORFUL_FAST_FLICKER,
    RED_SLOW_FLICKER,
    RED_GRADUAL_FLICKER,
    RED_FAST_FLICKER,
    ORANGE_SLOW_FLICKER,
    ORANGE_GRADUAL_FLICKER,
    ORANGE_FAST_FLICKER,
    YELLOW_SLOW_FLICKER,
    YELLOW_GRADUAL_FLICKER,
    YELLOW_FAST_FLICKER,
    GREEN_SLOW_FLICKER,
    GREEN_GRADUAL_FLICKER,
    GREEN_FAST_FLICKER,
    CYAN_SLOW_FLICKER,
    CYAN_GRADUAL_FLICKER,
    CYAN_FAST_FLICKER,
    BLUE_SLOW_FLICKER,
    BLUE_GRADUAL_FLICKER,
    BLUE_FAST_FLICKER,
    PURPLE_SLOW_FLICKER,
    PURPLE_GRADUAL_FLICKER,
    PURPLE_FAST_FLICKER,
    LED_FLICKER_MAX,
}LED_FLICKER_MODE;

//��������
#pragma pack(1)
typedef struct _LIGHT_EFFECT_
{
    u8 enable;            //ʹ��
    u8 mode;              //ģʽ
}LIGHT_EFFECT;

typedef struct _LIGHT_ATTRIBUTE_
{
    u8 light_mode;            //����ʾģʽ
    u8 brightness;            //����,0-100��
    u8 duty[MAX_LED];         //ռ�ձ�
    LIGHT_EFFECT effect;      //Ч��
}LIGHT_ATTRIBUTE;
#pragma pack()

extern LIGHT_ATTRIBUTE light_attr;
extern u8 light_on_off;

//�����ӿ�
void bt_led_init();
void set_brightness(u8 value, tbool fade);
void white_led_control(u8 status,u8 duty, tbool fade);
void rgb_led_control(u8 status,u8 r_duty,u8 g_duty,u8 b_duty, tbool fade);
void bt_control_data_deal(u16 devnum, u16 cmd,u8 *buf,u32 len);

#endif
