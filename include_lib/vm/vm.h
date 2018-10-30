#ifndef _VM_H_
#define _VM_H_


#include "common/list.h"
#include "typedef.h"
#include "rtos/os_api.h"
#include "peripherals/winbond_flash.h"


#define AT_VM AT(.vm)


/*-------------------vm_new_version------------------------*/
#define VM_INDEX_DES_MAX    128

typedef struct _VM_DESC
{
    u16 v_addr;
    u16 v_len;
}VM_DESC;

typedef struct _AREA_RANGE
{
	u32 start_addr;
	u32 len;			//byte
	u16 erase_bit_map;	//surport 16 * 4 sector
}AREA_INFO;

typedef struct _VM_STRUCT
{
	AREA_INFO area[2];
	VM_DESC idx_desc[VM_INDEX_DES_MAX];
	struct list_head write_list;
	OS_MUTEX mutex;
	u32 last_write_addr;
	u8 using_area;		//0:main_area	1:temp_area
	u8 warning_line;	//percentage
	u8 init_flag;		//vm_init_statu
	// u8 defrag_flag;		//vm_defrag
}VM_STRUCT;
/*-------------------vm_new_version------------------------*/


enum
{
    VM_ERR_NONE = 0,
    VM_INDEX_ERR = -0x100,
    VM_INDEX_EXIST,
	VM_DATA_LEN_ERR,
    VM_READ_NO_INDEX,
    VM_READ_DATA_ERR,
    VM_WRITE_OVERFLOW,
	VM_NOT_INIT,
	VM_INIT_ALREADY,
	VM_DEFRAG_ERR,
    VM_ERR_INIT,
    VM_ERR_PROTECT
};

typedef s32	vm_err;
typedef u8	vm_hdl;

/*��ʽ��VM_����*/
extern vm_err vm_eraser(void);

/*
ÿ���ϵ��ʼ��VMϵͳ,
need_defrag_when_write_full: ��vm����д��ʱ�Ƿ���Ҫ������Ƭ
*/
// extern void vm_run_mode_and_keyt_init(u8 mode);
extern vm_err vm_init(u32 vm_addr, u32 vm_len, u32 spi_base, u8 mode, bool need_defrag_when_write_full);

/*
�� vm_write,vm_read ǰ�����ȴ� ָ��index ��þ��,
�����δ�: vm_read/vm_write(vm_open(index,data_len),data_buf)
*/
extern s32 vm_open(u8 index, u16 data_len);

/*�������õ���vm_write,vm_read ʱ���Թرվ���ͷ��ڴ�*/
extern vm_err vm_close(vm_hdl hdl);

/*��д����������������ʾʵ�ʶ�д���ȣ����ظ�����ʾ����*/
extern s32 vm_write(vm_hdl hdl, const void *data_buf);
extern s32 vm_read(vm_hdl hdl, void * data_buf);

/*�ֶ�������Ƭ*/
extern vm_err vm_defrag(void);

/*��� vm ���� ʹ�����, ����ʹ�õİٷֱ�*/
extern vm_err vm_status(VM_STRUCT * vm_str_p);

extern const struct DEV_IO * dev_reg_vm(void *parm);

/*----------------------------------------------------------------------------*/
/**@brief  VM��Ƭ��⺯��
   @param  level��0��������ٶ�����VM���򣬵���һ��ȫ��������1������ȫVM����
   @return NON
   @note   �ⲿ���붨ʱͨ���˽ӿڲ�ѯVM�Ƿ���Ҫ����
*/
/*----------------------------------------------------------------------------*/
void vm_check_all(u8 level);

/*----------------------------------------------------------------------------*/
/**@brief  �ڲ�flash
   @param  cmd������ʽ
   @param  addr������ַ��flash��Ե�ַ
   @return 0��������1�����ɹ�
   @note   u32 sfc_read(u8 *buf, u32 addr, u32 len)
*/
/*----------------------------------------------------------------------------*/
bool sfc_erase(FLASH_ERASER cmd, u32 addr);

/*----------------------------------------------------------------------------*/
/**@brief  �ڲ�flash
   @param  buf��д����
   @param  addr��д��ַ��flash��Ե�ַ
   @param  len��д���ȣ���λbyte
   @return 0��д����1��д�ɹ�len
   @note   u32 sfc_read(u8 *buf, u32 addr, u32 len)
*/
/*----------------------------------------------------------------------------*/
u32 sfc_write(const u8 *buf, u32 adr, u32 len);

/*----------------------------------------------------------------------------*/
/**@brief  �ڲ�flash
   @param  buf��������
   @param  addr������ַ��flash��Ե�ַ
   @param  len�������ȣ���λbyte
   @return 0��������1�����ɹ�
   @note   u32 sfc_read(u8 *buf, u32 addr, u32 len)
*/
/*----------------------------------------------------------------------------*/
u32 sfc_read(u8 *buf, u32 adr, u32 len);

#endif  //_VM_H_



