
#pragma once

#ifndef _hylib_basic_include_h_
#define _hylib_basic_include_h_

/* C头 */
#ifndef __cplusplus
#include <stdio.h>  //标准输入输出
#include <stdlib.h> //标准库
#include <string.h> //字符串
#endif
/* C++头 */
#ifdef __cplusplus
#include <cstdio>   //标准输入输出
#include <cstdlib>  //标准库
#include <iostream> //输入输出流
#include <cstring>  //字符串
#endif
/* Windows头 */
#ifdef Windows_OS
#include <Windows.h>
#endif

#include <fcntl.h>    //POSIX系统调用
#include <sys/file.h> //文件
#include <unistd.h>   //UNIX标准定义、库
#include <sys/time.h> //时间

#endif
