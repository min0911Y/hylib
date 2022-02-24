
#pragma once

/**
 * 程序开头格式：
 * 1  #pragma once
 * 2
 * 3 <注释,可跨多行>
 * 4 #ifndef _hylib_xxx_h_
 * 5 #define _hylib_xxx_h_
 * 6
 * 7 #includes <xxx.h>
 * 8
 * 9 xxxxxxxxxxxxx
 * 10
 * 11#endif
 */

/**
 * 注释:关于宏定义
 *
 * _hylib_ 加在所有namespace hylib外的宏定义中
 *
 */

#ifndef _hylib_basic_h_
#define _hylib_basic_h_

#define Linux_OS
//#define Windows_OS

#include <hylib/basic/include.h>
#include <hylib/basic/define.h>

#ifdef __cplusplus
using namespace std;

namespace hylib
{
#endif

#define Little_Endian false
#define Big_Endian true

	/**
	 * @brief 获取系统是大端序还是小端序
	 * 
	 * @return true 
	 * @return false 
	 */
	inline bool endiancheck()
	{
		int num = 1;
		if (*(char *)&num == 1)
			return Little_Endian;
		else
			return Big_Endian;
	}

#ifdef __cplusplus
	template <typename T>
	inline void memclear(T *ptr1)
	{
		for (int i = 0; i < sizeof(T); i++)
			*((byte *)ptr1 + i) = 0;
	}
	template <typename T>
	inline void memcpy(T *ptr1, const T *ptr2)
	{
		for (int i = 0; i < sizeof(T); i++)
			*((byte *)ptr1 + i) = *((byte *)ptr2 + i);
	}
#endif
	inline void memclear(void *ptr1, const int size)
	{
		for (int i = 0; i < size; i++)
			*((byte *)ptr1 + i) = 0;
	}

	inline void memcpy(void *ptr1, const void *ptr2, const int size)
	{
		for (int i = 0; i < size; i++)
			*((byte *)ptr1 + i) = *((byte *)ptr2 + i);
	}

#define TS_BIN 0				  // 2进制
#define TS_OCT 1				  // 8进制
#define TS_DEC 2				  // 10进制
#define TS_HEX 3				  // 16进制
#define TS_letters_l 4			  // 26个小写英文字母
#define TS_letters_u 5			  // 26个大写英文字母
#define TS_letters_all 6		  // 26个英文字母的大写与小写
#define TS_nums_and_letters_l 7	  // 数字与26个小写英文字母
#define TS_nums_and_letters_u 8	  // 数字与26个大写英文字母
#define TS_nums_and_letters_all 9 // 数字与26个英文字母大写与小写
#define TS_ascii_printable 10	  // ascii字符集中所有可打印字符

	inline int tostr_BIN(const void *arr, const int arrlen, char *str, const int strsize)
	{
		int strp = 0;
		int i, i2;
		for (i = 0; i < arrlen; i++)
		{
			for (i2 = 7; i2 >= 0; i2--)
			{
				*(str + strp) = '0' + (*((byte *)arr + i) >> i2) & 1;
				strp++;
				if (strp >= strsize - 1)
				{
					*(str + strp) = '\0';
					return strp;
				}
			}
		}
	}
	inline int tostr_OCT(const void *arr, const int arrlen, char *str, const int strsize)
	{
		int strp = 0;
		int i, i2;
		for (i = 0; i < arrlen; i++)
		{
			for (i2 = 0; i2 < 8; i2++)
			{
				*(str + strp) = '0' + (*((byte *)arr + i) >> i2) & 7;
				strp++;
				if (strp >= strsize - 1)
				{
					*(str + strp) = '\0';
					return strp;
				}
			}
		}
	}

	inline void arrtostr()
	{
	}
	inline void numtostr(const int num, const int mode, char *str, const int maxlen)
	{
		switch (mode)
		{
		}
	}

#ifdef __cplusplus
}

using namespace hylib;
#endif

#endif
