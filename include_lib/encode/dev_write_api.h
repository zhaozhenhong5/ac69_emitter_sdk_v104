#ifndef __DEV_WRITE_API_H__
#define __DEV_WRITE_API_H__

#include "file_operate/file_op.h"
#include "encode.h"
#include "fat/tff.h"

//typedef enum
//{
//    FR_OK = 0,
//    FR_FIND_DIR = 0x80,
//    FR_FIND_FILE,
//    FR_DIR_END,         //ǰ�漸��λ�ò��ܱ�
//    FR_NO_FILE,
//    FR_NO_PATH,
//    FR_EXIST,
//    FR_INVALID_NAME,
//    FR_INVALID_DRIVE,
//    FR_DENIED,
//    FR_RW_ERROR,
//    FR_WRITE_PROTECTED,
//    FR_NO_FILESYSTEM,
//    FR_DEEP_LIMIT,
//    FR_END_PATH,
//    FR_FILE_LIMIT,
//    FR_END_FILE,
//    FR_LFN_ERR,
//    FR_MKFS_ABORTED,
//    FR_DIR_DELETE,
//    FR_DISK_ERROR,
//    FR_FILE_END,
//	FR_FILE_ERR,
//    FR_NO_WINBUF,
//	FR_INT_ERR,				/* (2) Assertion failed */
//	FR_NO_SEL_DRIVE,
//} FRESULT;

#define FA_CREATE_HIDDEN    0x02
#define	FA_WRITE			0x04            //�Ƿ�����д�ļ�
#define	FA_CREATE_NEW		0x08            //�ļ�������ʱ����
#define	FA_CREATE_ALWAYS	0x10            //�����ļ��з���ڣ�������


s16 encode_fs_open(RECORD_OP_API *e_api,ENUM_DEV_SEL_MODE dev_sel,u32 dev_let,char *folderpath,u32 mode);
s16 encode_file_open(RECORD_OP_API *e_api,char *filepath,u32 mode);
void encode_fs_close(RECORD_OP_API *e_api);
s16 encode_get_fileinfo(RECORD_OP_API *e_api,char *path, char *ext,u32 *first_fn,u32 *total);

#endif
