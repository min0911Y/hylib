#pragma once

// terminal.h
#ifndef _hylib_terminal_h_
#define _hylib_terminal_h_

#include <hylib/basic/basic.h>

#ifdef Linux_OS
#include <sys/ioctl.h>
#endif

#ifdef __cplusplus
namespace hylib
{
	namespace terminal
	{

		/**
		 * @brief 清屏
		 *
		 */
		inline void cls()
		{
#ifdef Linux_OS
			printf("\033c");
#endif
#ifdef Windows_OS
			system("cls");
#endif
		}

#ifdef Linux_OS
		inline struct winsize getsize()
		{
			struct winsize size;
			ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
			return size;
		}
#endif
#ifdef Windows_OS
		inline void getsize() //未完成
		{
		}
#endif

		/**
		 * @brief 设置终端字体颜色
		 *
		 * @param r 红色
		 * @param g 绿色
		 * @param b 蓝色
		 */
		inline void setcolor(const byte r, const byte g, const byte b)
		{
#ifdef Linux_OS
			printf("\033[38;2;%d;%d;%dm", r, g, b);
#endif
#ifdef Windows_OS
#endif
		}
		/**
		 * @brief 设置终端背景颜色
		 *
		 * @param r 红色
		 * @param g 绿色
		 * @param b 蓝色
		 */
		inline void setbgcolor(const byte r, const byte g, const byte b)
		{
#ifdef Linux_OS
			printf("\033[48;2;%d;%d;%dm", r, g, b);
#endif
#ifdef Windows_OS
#endif
		}
		/**
		 * @brief 设置终端字体与背景颜色
		 *
		 * @param r 字体红
		 * @param g 字体绿
		 * @param b 字体蓝
		 * @param bgr 背景红
		 * @param bgg 背景绿
		 * @param bgb 背景蓝
		 */
		inline void setcolor(const byte r, const byte g, const byte b, const byte bgr, const byte bgg, const byte bgb)
		{
#ifdef Linux_OS
			printf("\033[38;2;%d;%d;%d;48;2;%d;%d;%dm", r, g, b, bgr, bgg, bgb);
#endif
#ifdef Windows_OS
#endif
		}

		/**
		 * @brief
		 *
		 * @param str
		 * @param r
		 * @param g
		 * @param b
		 */
		inline void printwithcolor(const char *str, const byte r, const byte g, const byte b)
		{
#ifdef Linux_OS
			printf("\033[38;2;%d;%d;%dm%s\033[38;2;0;0;0m", r, g, b, str);
#endif
#ifdef Windows_OS
			printf(str);
#endif
		}
		inline void printwithcolor(const char *str, const byte r, const byte g, const byte b, const byte bgr, const byte bgg, const byte bgb)
		{
#ifdef Linux_OS
			printf("\033[38;2;%d;%d;%d;48;2;%d;%d;%dm%s\033[38;2;0;0;0;48;2;255;255;255m", r, g, b, bgr, bgg, bgb, str);
#endif
#ifdef Windows_OS
			printf(str);
#endif
		}

#ifdef __cplusplus /* C++部分 */
		template <typename T>
		inline void printwithcolor(const T &arg, const byte r, const byte g, const byte b)
		{
#ifdef Linux_OS
			printf("\033[38;2;%d;%d;%dm", r, g, b);
			cout << arg;
			printf("\033[38;2;0;0;0m");
#endif
#ifdef Windows_OS
			cout << arg;
#endif
		}
		template <typename T>
		inline void printwithcolor(const T &arg, const byte r, const byte g, const byte b, const byte bgr, const byte bgg, const byte bgb)
		{
#ifdef Linux_OS
			printf("\033[38;2;%d;%d;%d;48;2;%d;%d;%dm", r, g, b, bgr, bgg, bgb);
			cout << arg;
			printf("\033[38;2;0;0;0;48;2;255;255;255m");
#endif
#ifdef Windows_OS
			cout << arg;
#endif
		}
#endif /* C++部分结束 */

		inline void printprogressbar(double v)
		{
#ifdef Linux_OS
			winsize size;
			ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
			size.ws_col -= 5;
			int len = v * size.ws_col + 0.5;
			cout << "\r\033[?25l";
			cout << '[';
			for (int i = 0; i < len; i++)
				cout << '-';
			for (int i = len; i < size.ws_col; i++)
				cout << ' ';
			cout << ']';
			printf("%2d", (int)(v * 100 + 0.5));
			if ((int)(v * 100 + 0.5) < 100)
				cout << '%';
#endif
#ifdef Windows_OS
#endif
		}
		inline void clearprogressbar()
		{
#ifdef Linux_OS
			printf("\r\033[K\033[?25h");
#endif
#ifdef Windows_OS
#endif
		}
	}
}

#endif

#endif