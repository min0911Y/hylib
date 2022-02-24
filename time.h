#pragma once

#include <pthread.h>

#include <hylib/basic/basic.h>

namespace hylib
{
	namespace time
	{
		/**
		 * @brief 时间
		 *
		 */
		struct Time
		{
			/**
			 * @brief 天数
			 *
			 */
			uint day;
			/**
			 * @brief 小时数
			 *
			 */
			byte hour;
			/**
			 * @brief 分钟数
			 *
			 */
			byte min;
			/**
			 * @brief 秒数
			 *
			 */
			byte sec;
			/**
			 * @brief 毫秒数
			 *
			 */
			ushort msec;
			/**
			 * @brief 微秒数
			 *
			 */
			ushort usec;
			/**
			 * @brief 总微秒数
			 *
			 */
			ulong totalus;
#ifdef Linux_OS
			/**
			 * @brief 纳秒数
			 *
			 */
			ushort nsec;
			/**
			 * @brief 总纳秒数
			 *
			 */
			ulong totalns;
#endif
			/**
			 * @brief
			 *
			 */
			inline Time()
			{
#ifdef Linux_OS
				totalns = 0;
				nsec = 0;
#endif
				totalus = 0;
				usec = 0;
				msec = 0;
				sec = 0;
				min = 0;
				hour = 0;
				day = 0;
			}
			/**
			 * @brief 从秒数转换
			 *
			 * @param time 时间
			 */
			inline void init(ulong time)
			{
#ifdef Linux_OS
				totalns = time * 1000000000UL;
				nsec = 0;
#endif
				totalus = time * 1000000UL;
				usec = 0;
				msec = 0;
				sec = time % 60;
				time /= 60;
				min = time % 60;
				time /= 60;
				hour = time % 24;
				time /= 24;
				day = time;
			}
			/**
			 * @brief 从毫秒数转换
			 *
			 * @param time 时间
			 */
			inline void initms(ulong time)
			{
#ifdef Linux_OS
				totalns = time * 1000000UL;
				nsec = 0;
#endif
				totalus = time * 1000UL;
				usec = 0;
				msec = time % 1000;
				time /= 1000;
				sec = time % 60;
				time /= 60;
				min = time % 60;
				time /= 60;
				hour = time % 24;
				time /= 24;
				day = time;
			}
			/**
			 * @brief 从微秒数转换
			 *
			 * @param time 时间
			 */
			inline void initus(ulong time)
			{
#ifdef Linux_OS
				totalns = time * 1000UL;
				nsec = 0;
#endif
				totalus = time;
				usec = time % 1000;
				time /= 1000;
				msec = time % 1000;
				time /= 1000;
				sec = time % 60;
				time /= 60;
				min = time % 60;
				time /= 60;
				hour = time % 24;
				time /= 24;
				day = time;
			}
#ifdef Linux_OS
			/**
			 * @brief 从纳秒数转换
			 *
			 * @param time 时间
			 */
			inline void initns(ulong time)
			{
				totalns = time;
				totalus = time / 1000;
				nsec = time % 1000;
				time /= 1000;
				usec = time % 1000;
				time /= 1000;
				msec = time % 1000;
				time /= 1000;
				sec = time % 60;
				time /= 60;
				min = time % 60;
				time /= 60;
				hour = time % 24;
				time /= 24;
				day = time;
			}
#endif
			/**
			 * @brief 获取时间(精确到秒)
			 *
			 */
			inline void gettime()
			{
				init(time(NULL));
			}
			/**
			 * @brief 获取时间(精确到毫秒)
			 *
			 */
			inline void gettimems()
			{
				struct timeval t;
				gettimeofday(&t, NULL);
				initms(t.tv_sec * 1000UL + (t.tv_usec + 500) / 1000);
			}
			/**
			 * @brief 获取时间(精确到微秒)
			 *
			 */
			inline void gettimeus()
			{
				struct timeval t;
				gettimeofday(&t, NULL);
				initus(t.tv_sec * 1000000UL + t.tv_usec);
			}
#ifdef Linux_OS
			/**
			 * @brief 获取时间(精确到纳秒)
			 *
			 */
			inline void gettimens()
			{
				struct timespec t;
				clock_gettime(CLOCK_REALTIME, &t);
				initns(t.tv_sec * 1000000000UL + t.tv_nsec);
			}
#endif
		};

		inline ulong gettimeh()
		{
			return time(NULL) / 3600;
		}
		inline ulong gettimem()
		{
			return time(NULL) / 60;
		}
		inline ulong gettimes()
		{
			return time(NULL);
		}
		inline ulong gettimems()
		{
			struct timeval t;
			gettimeofday(&t, NULL);
			return t.tv_sec * 1000UL + (t.tv_usec + 500) / 1000;
		}
		inline ulong gettimeus()
		{
			struct timeval t;
			gettimeofday(&t, NULL);
			return t.tv_sec * 1000000UL + t.tv_usec;
		}
#ifdef Linux_OS
		/**
		 * @brief 获取时间(精确到纳秒)
		 *
		 * @return 时间(精确到纳秒)
		 */
		inline ulong gettimens()
		{
			struct timespec t;
			clock_gettime(CLOCK_REALTIME, &t);
			return t.tv_sec * 1000000000UL + t.tv_nsec;
		}
#endif

		class Timer
		{
			ulong starttime;
			ulong stoptime;

			ulong alarmtime;
			void *alarmuserdata;
			bool (*alarmcallback)(ulong timeus, void *userdata);
			pthread_t alarmthread;

			inline Timer()
			{
				starttime = 0;
				stoptime = 0;
				alarmtime = -1;
				alarmuserdata = NULL;
				alarmcallback = NULL;
				alarmthread = NULL;
			}
			inline ulong geth()
			{
				if (stoptime == 0)
					return gettimeh() - starttime / 3600000000;
				else
					return (stoptime - starttime) / 3600000000;
			}
			inline ulong getm()
			{
				if (stoptime == 0)
					return gettimem() - starttime / 60000000;
				else
					return (stoptime - starttime) / 60000000;
			}
			inline ulong gets()
			{
				if (stoptime == 0)
					return gettimes() - starttime / 1000000;
				else
					return (stoptime - starttime) / 1000000;
			}
			inline ulong getms()
			{
				if (stoptime == 0)
					return gettimems() - starttime / 1000;
				else
					return (stoptime - starttime) / 1000;
			}
			inline ulong getus()
			{
				if (stoptime == 0)
					return gettimeus() - starttime;
				else
					return stoptime - starttime;
			}
			inline Time get()
			{
				Time t;
				t.initus(getus());
				return t;
			}

			inline void setalarm(ulong time)
			{
				alarmtime = time;
			}

			inline void start()
			{
				starttime = gettimeus();
				stoptime = 0;
				if (alarmtime >= 0 && alarmcallback != NULL && alarmthread == 0)
					pthread_create(&alarmthread, NULL, alarm_func, this);
				return starttime;
			}
			inline ulong stop()
			{
				stoptime = gettimeus();
				if (alarmthread != 0)
				{
					pthread_cancel(t);
					pthread_join(t, NULL);
				}
				return stoptime - starttime;
			}
			inline void restart()
			{
				ulong time = stop();
				start();
				return time;
			}

			static void *alarm_func(void *arg)
			{
				ulong sleeptime;
				int i = 1;
				sleeptime = (Timer *)arg->alarmtime * i - (Timer *)arg->getus();
				if (sleeptime < 0)
					sleeptime = 0;
				while (1)
				{
					usleep(sleeptime);
					if (alarmcallback((Timer *)arg->alarmtime, (Timer *)arg->alarmuserdata) == false)
						break;
					do
					{
						i++;
					} while (sleeptime = (Timer *)arg->alarmtime * (i - 1) - (Timer *)arg->getus() > 0);
				}
				return NULL;
			}
		};
	}
}
