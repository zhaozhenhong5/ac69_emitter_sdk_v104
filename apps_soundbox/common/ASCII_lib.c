#include "common/ASCII_lib.h"


/*----------------------------------------------------------------------------*/
/** @brief:
    @param:
    @return:
    @author:Juntham
    @note:
    @date: 2012-06-08,10:16
*/
/*----------------------------------------------------------------------------*/

//void ASCII_ToUpper(void *buf, u32 len)
//{
//    while (len--)
//    {
//        if ((((u8 *)buf)[len] >= 'a') && (((u8 *)buf)[len] <= 'z'))
//        {
//            ((u8 *)buf)[len] -= 'a' - 'A';
//        }
//    }
//}

/*----------------------------------------------------------------------------*/
/** @brief:�ַ���ƥ�䣬�ļ�������ʹ��?��ƥ��
    @param:
    @return:
    @author:Juntham
    @note:
    @date: 2012-06-08,14:52
*/
/*----------------------------------------------------------------------------*/

u32 ASCII_StrCmp(const u8 *src, const u8 *dst, u32 len)
{
    u32 m = len; 			//���ڴ����ļ�����'*'
    while (len--)
    {
        if ((*src == '\0') && (*dst== '\0') )				//ͬʱ����
        {
            return 0;
        }
        if ((*src == '\0') || (*dst== '\0'))	//��һ����ǰ����
        {
            return m-len;
        }

        if (*src != *dst)							//�Ƚϲ����
        {
            if ((*dst != '?') && (*src != '?'))
            {
                return m-len;
            }
        }

        src++;
        dst++;
    }
    return 0;

}


void ASCII_IntToStr(void *pStr , u32 intNum , u32 strLen)
{
    while(strLen--)
    {
        ((u8 *)pStr)[strLen] = '0' + intNum%10;
        intNum/=10;
    }
}


u32 ASCII_StrToInt(const void *pStr , u32 *pRint, u32 strLen)
{
    u32 m;

    for(m=1,*pRint=0; strLen--; m*=10)
    {
        if(((u8 *)pStr)[strLen]<'0' || ((u8 *)pStr)[strLen]>'9')
        {
            return strLen;
        }
        *pRint += (((u8 *)pStr)[strLen]-'0') * m;
    }

    return 0;
}


//u32 ASCII_StrLen(void *str , u32 len)
//{
//    u32 i;
//
//    for(i=0 ; i< len ; i++)
//    {
//        if(((u8   *)str)[i] ==0)
//        {
//            break;
//        }
//    }
//
//    return i;
//}


//u32 ASCII_WStrLen(void *str , u32 len)
//{
//    u32 i;
//
//    for(i=0 ; i< len ; i+=2)
//    {
//        if(((u8 *)str)[i] ==0 && ((u8 *)str)[i+1] ==0)
//        {
//            break;
//        }
//    }
//
//    return i;
//}
