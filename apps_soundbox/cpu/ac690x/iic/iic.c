/*--------------------------------------------------------------------------*/
/**@file     iic_io.c
   @brief    IOģ���IIC������
   @details
   @author  zhiying
   @date   2013-3-26
   @note
*/
/*----------------------------------------------------------------------------*/
#include "iic.h"

/*----------------------------------------------------------------------------*/
/** @brief: IIC ģ���ʼ������
    @param: void
    @return:void
    @author:Juntham
    @note:  void iic_init_io(void)
*/
/*----------------------------------------------------------------------------*/
void iic_init_io(void)
{
    iic_data_h();
    iic_data_out();         //SDA���ó����
    iic_clk_h();
    iic_clk_out();         	//SCL���ó����
}
/*----------------------------------------------------------------------------*/
/**@brief
   @param   ��
   @return
   @note
*/
/*----------------------------------------------------------------------------*/
void iic_start(void)
{
    iic_init_io();
    iic_delay();
    iic_data_l();
    iic_delay();
    iic_clk_l();
    iic_delay();
    //iic_data_h();
}
/*----------------------------------------------------------------------------*/
/**@brief   STOP IIC
   @param   ��
   @return  ��
   @note    void iic_stop(void)
*/
/*----------------------------------------------------------------------------*/
void iic_stop(void)
{
    iic_data_out();
    iic_data_l();
    iic_delay();
    iic_clk_h();
    iic_delay();
    iic_data_h();
    iic_delay();
}
/*----------------------------------------------------------------------------*/
/**@brief   ��ȡACK
   @param   ��
   @return  �����ACK/NACK�ĵ�ƽ
   @note    tbool r_ack(void)
*/
/*----------------------------------------------------------------------------*/
tbool r_ack(void)
{
    bool tnack;
    iic_data_in();
    iic_delay();
    iic_clk_h();
    iic_delay();
    iic_delay();
    iic_delay();
    iic_delay();
    iic_delay();
    tnack = iic_data_r();
    iic_clk_l();
    iic_delay();
    return tnack;
}
/*----------------------------------------------------------------------------*/
/**@brief   ����һ��ACK�źŵ�����,
   @param   flag �����͵�ACK/nack������
   @return  ��
   @note    void s_ack(u8 flag)
*/
/*----------------------------------------------------------------------------*/
void s_ack(u8 flag)
{
    iic_data_out();
    iic_clk_l();
    if (flag)
    {
        iic_data_h();
    }
    else
    {
        iic_data_l();
    }
    iic_delay();
    iic_clk_h();
    iic_delay();
    iic_clk_l();
}
/*----------------------------------------------------------------------------*/
/**@brief   ��IIC���߽���һ��BYTE������,
   @param   ��
   @return  ��ȡ�ص�����
   @note    u8 iic_revbyte_io( void )
*/
/*----------------------------------------------------------------------------*/
tu8 iic_revbyte_io( void )
{
    u8 tbyteI2C = 0X00;
    u8 i;
    iic_data_in();
    iic_delay();
    for (i = 0; i < 8; i++)
    {
        iic_clk_h();
        tbyteI2C <<= 1;
        iic_delay();
        if (iic_data_r())
        {
            tbyteI2C++;
        }
        iic_clk_l();
        iic_delay();
    }
    return tbyteI2C;
}
/*----------------------------------------------------------------------------*/
/**@brief   ��IIC���߽���һ��BYTE������,������һ��ָ����ACK
   @param   para ������ACK ���� NACK
   @return  ��ȡ�ص�����
   @note    u8 iic_revbyte( u8 para )
*/
/*----------------------------------------------------------------------------*/
tu8 iic_revbyte( u8 para )
{
    u8 tbyte;
    tbyte = iic_revbyte_io();
    s_ack(para);
	delay(30000);
    return tbyte;
}
/*----------------------------------------------------------------------------*/
/**@brief   ��IIC���߷���һ��BYTE������
   @param   byte ��Ҫд��EEROM�ĵ�ַ
   @return  ��
   @note    void iic_sendbyte_io(u8 byte)
*/
/*----------------------------------------------------------------------------*/
void iic_sendbyte_io(u8 byte)
{
    u8 i;
    iic_data_out();
    iic_delay();
    //printf("byte: %02x\n", byte);
    for (i = 0; i < 8; i++)
    {
        if (byte & BIT(7))
        {
            iic_data_h();  //���λ�Ƿ�Ϊ1,Ϊ1��SDA= 1,���� SDA=0
        }
        else
        {
            iic_data_l();
        }
        iic_delay();
        iic_clk_h();
        iic_delay();
        byte <<= 1;                   //��������һλ,������һ������
        iic_clk_l();
    }
}
/*----------------------------------------------------------------------------*/
/**@brief   ��IIC���߷���һ��BYTE������,����ȡACK
   @param   byte ��Ҫд��EEROM�ĵ�ַ
   @return  ��
   @note    void iic_sendbyte(u8 byte)
*/
/*----------------------------------------------------------------------------*/
void iic_sendbyte(u8 byte)
{
    iic_sendbyte_io(byte);
    r_ack();
	delay(30000);
}





///------------------------------------------------------------------------------
///------------------------------------IIC_API-----------------------------------
///------------------------------------------------------------------------------


bool g_iic_busy = 0; ///<iic��æ���
/*----------------------------------------------------------------------------*/
/** @brief: IIC ģ���ʼ������
    @param: void
    @return:void
    @author:Juntham
    @note:  void iic_init(void)
*/
/*----------------------------------------------------------------------------*/
void iic_init(void)
{
   // iic_data_out();
    iic_data_h();
    iic_data_out();
    //iic_clk_out();
    iic_clk_h();    ///<���ýӿں���IO
    iic_clk_out();
}

/*----------------------------------------------------------------------------*/
/** @brief: eeprom ��ʵ
    @param: void
    @return:void
    @author:Juntham
    @note:  void eeprom_verify(void)
*/
/*----------------------------------------------------------------------------*/
void eeprom_verify(void)
{
    puts("eeprom_verify 1\n");

    if ((read_eerom(0) != 0x55)
            ||(read_eerom(1) != 0xAA))
    {
        write_eerom(0, 0x55);
        write_eerom(1, 0xAA);
    }

    puts("eeprom_verify 2\n");
    if ((read_eerom(0) != 0x55)
            ||(read_eerom(1) != 0xAA))
    {
        puts("iic-no\n");//���eeprom��Ч
    }
    else
    {
        puts("iic-yes\n"); //�����eeprom
    }
}

void iic_write_one_byte(u8 iic_dat)
{
    g_iic_busy  = 1;
    iic_start();                //I2C����
    iic_sendbyte(iic_dat);      //д����
    iic_stop();                 //I2Cֹͣʱ��
}
/*----------------------------------------------------------------------------*/
/**@brief   IICд����
   @param   chip_id ��Ŀ��IC��ID��
   @param   iic_addr: Ŀ��IC��Ŀ��Ĵ����ĵ�ַ
   @param   *iic_dat: д��Ŀ��IC�����ݵ�ָ��
   @param   n:��Ҫд�����ݵ���Ŀ
   @return  ��
   @note    void  iic_write(u8 chip_id,u8 iic_addr,u8 *iic_dat,u8 n)
*/
/*----------------------------------------------------------------------------*/
void iic_write(u8 chip_id,u8 iic_addr,u8 *iic_dat,u8 n)
{
    g_iic_busy  = 1;
    iic_start();                //I2C����
    iic_sendbyte(chip_id);         //д����

    if (0xff != iic_addr)
    {
        iic_sendbyte(iic_addr);   //д��ַ
    }
    for (; n>0; n--)
    {
        iic_sendbyte(*iic_dat++);      //д����
    }
    iic_stop();                 //I2Cֹͣʱ��
    g_iic_busy = 0;
}
/*----------------------------------------------------------------------------*/
/**@brief   IIC������һ��Ŀ��ID��ȡ��������
   @param   address : Ŀ��ID
   @param   *p     :  �浵��ȡ�������ݵ�bufferָ��
   @param   number :  ��Ҫ��ȡ�����ݵĸ���
   @return  ��
   @note    void i2c_read_nbyte(u8 address,u8 *p,u8 number)
*/
/*----------------------------------------------------------------------------*/
void iic_readn(u8 chip_id,u8 iic_addr,u8 *iic_dat,u8 n)
{
    g_iic_busy = 1;
    iic_start();                //I2C����
    iic_sendbyte(chip_id);         //д����
    if (0xff != iic_addr)
    {
        iic_sendbyte(iic_addr);   //д��ַ
    }
    for (; n>1; n--)
    {
        *iic_dat++ = iic_revbyte(0);      //������
    }
    *iic_dat++ = iic_revbyte(1);
    iic_stop();                 //I2Cֹͣʱ��
    g_iic_busy = 0;
}

/*----------------------------------------------------------------------------*/
/**@brief   EEROM/RTC RAM������
   @param   addr ��Ҫд��EEROM/RTC RAM�ĵ�ַ
   @return  ����������
   @note    u8 read_info(u8 addr)
*/
/*----------------------------------------------------------------------------*/
tu8 read_info(u8 addr)
{
    return read_eerom(addr);
}

/*----------------------------------------------------------------------------*/
/**@brief   EEROM/RTC RAMд����
   @param   addr ��Ҫ����EEROM/RTC RAM�ĵ�ַ
   @param   dat    ����Ҫ��������
   @return  ��
   @note    void write_info(u8 addr,u8 dat)
*/
/*----------------------------------------------------------------------------*/
void write_info(u8 addr,u8 dat)
{
    write_eerom(addr,dat);
}

/*----------------------------------------------------------------------------*/
/**@brief   eeprom START
   @param   ��
   @return  ��
   @note    void  eeprom_page_write_start(void)
*/
/*----------------------------------------------------------------------------*/
void  eeprom_page_write_start(void)
{
    g_iic_busy  = 1;
    iic_start();                //I2C����
    iic_sendbyte(0xa0);         //д����
}
/*----------------------------------------------------------------------------*/
/**@brief   IIC STOP
   @param   ��
   @return  ��
   @note    void eeprom_page_write_stop(void)
*/
/*----------------------------------------------------------------------------*/
void eeprom_page_write_stop(void)
{
    iic_stop();                     //I2Cֹͣiic_data_out
    g_iic_busy = 0;
}
/*----------------------------------------------------------------------------*/
/**@brief   EEROM������
   @param   addr ��Ҫд��EEROM�ĵ�ַ
   @param   ��
   @return  ��
   @note    u8 iic_read(u8 iic_addr)
*/
/*----------------------------------------------------------------------------*/
tu8 read_eerom(u8 iic_addr)
{
    u8  tbyte;
    g_iic_busy = 1;
    iic_start();                    //I2C����
    iic_sendbyte(0xa0);             //д����
    iic_sendbyte(iic_addr);       //д��ַ
    iic_start();                    //дתΪ�������Ҫ�ٴ�����I2C
    iic_sendbyte(0xa1);             //������
    tbyte = iic_revbyte(1);
    iic_stop();                     //I2Cֹͣ
    g_iic_busy = 0;
    return  tbyte;
}

/*----------------------------------------------------------------------------*/
/**@brief   ���к�����ʱ��EEROMд����
   @param   addr ��Ҫд��EEROM�ĵ�ַ
   @param   dat    ����Ҫд������
   @return  ��
   @note    void write_info(u8 addr,u8 dat)
*/
/*----------------------------------------------------------------------------*/
void write_eerom(u8 addr,u8 dat)
{
    iic_write(0xa0,addr,&dat,1);
    iic_delay();
    iic_delay();
    iic_delay();
    iic_delay();
    iic_delay();
    iic_delay();
}

/*----------------------------------------------------------------------------*/
/** @brief: ������Ϣ���洢����EEPROM��
    @param: void
    @return:void
    @author:Juntham
    @note:  void set_memory(u8 addr, u8 dat)
*/
/*----------------------------------------------------------------------------*/
 void set_memory(u8 addr, u8 dat)
{
    write_eerom(addr, dat);
}
/*----------------------------------------------------------------------------*/
/** @brief: ��ȡ������Ϣ��EEPROM��
    @param: void
    @return:void
    @author:Juntham
    @note:  u8 get_memory(u8 addr)
*/
/*----------------------------------------------------------------------------*/
u8 get_memory(u8 addr)
{
    return read_eerom(addr);
}
