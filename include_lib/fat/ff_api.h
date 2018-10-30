#ifndef _FF_API
#define _FF_API



#include "typedef.h"
#include "tff.h"




enum
{
    SEARCH_FILE_BY_NUMBER,  //���ļ���������ļ�
    SEARCH_SPEC_FILE,       //����ָ�����ļ�
    SEARCH_FILE_BY_PATH,   //��·�������ļ�
    SEARCH_FILE_IN_PATH,   //����·�������ļ�
};

enum
{
    SEARCH_DISK,                //��������Ч�ļ���
    SEARCH_FOLDER,           //������ǰĿ¼���µ���Ч�ļ���
};


/* function list */
_banked_func void fs_ext_setting(const char *str);
_banked_func void ui_ext_setting(const char *str);
_banked_func bool check_ext(char _xdata *fname,char const *ext);
_banked_func FRESULT compare_path(u8 * spath, u8 *dpath);
_banked_func bool fast_scan_files (FATFS _xdata *fs,DIR _xdata * dj,u32 spec_sclust, u32 *file_total,u8 search_mode);
_banked_func bool scan_files (FIL _xdata *fp,DIR _xdata *dj,FS_NAME _xdata *fs_n,u32 spec_sclust,u8 search_mode);
_banked_func bool fs_load_file(FIL *fp);
_banked_func bool fs_scan_disk(FATFS _xdata *fs, u32 sp_sclust);
_banked_func bool fs_getfile_bysclust(FATFS *fs, FIL *fp, u32 sclust,char *lfn);
_banked_func bool fs_getfile_bynumber(FATFS *fs, FIL *fp, u16 file_number, char *lfn);
_banked_func bool fs_getfile_bypath(FATFS *fs,FIL *fp, u8 *path,char *lfn);
_banked_func u16 fs_getfile_totalindir(FATFS *fs, DIR _xdata *f_dj);
_banked_func u32 fs_getrec_fileinfo(FATFS _xdata *fs, char *path, char *ext,u32 *first_fn);
_banked_func u32 fs_open_dir_info(FATFS _xdata *f_s, FIL *f_p, void* dir_dj);
_banked_func u32 fs_exit_dir_info(FATFS _xdata *f_s, FIL *f_p);
_banked_func u32 fs_get_dir_info(FATFS _xdata *f_s, FIL *f_p, u32 start_num, u32 total_num, void *buf_info);

_banked_func u32 fs_getfile_byname_indir(FATFS _xdata *f_s, FIL *tag_file, FIL *get_file, char * ext_name);

void fat_para_init(FATFS **fat_fs,FIL **fhandle);
void fat_para_free(FATFS **fat_fs,FIL **fhandle);
void fs_lfn_deal(FATFS _xdata *f_s, FIL *f_p);
bool create_rec_file(FATFS *fat_fs,FIL *fhandle);

void reg_ff_scan_ret_fun(u32 (*fun)(void));//ע��ص�������fast_scanʱ����ã�ע�ắ������1���˳�scan

#endif
