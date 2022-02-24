#pragma once

/**
 * @brief 散列函数
 *
 */

#ifndef _hylib_hash_func_h_
#define _hylib_hash_func_h_

#include <hylib/basic/basic.h>

#ifdef __cplusplus
namespace hylib
{
	namespace hash
	{

		/**
		 * @brief 'class Hash'中可能出现的最大的数（取不到）
		 *
		 */
		uint Hash_max = 97;

		/**
		 * @brief 散列函数类（取余法）
		 *
		 */
		class Hash
		{
		private:
			ulong a;
			uint max;

		public:
			ulong hash;

			inline Hash()
			{
				init();
			}

			inline void init()
			{
#ifdef Debug
				printf("\'class Hash\'初始化\n");
#endif
				hash = 0;
				a = 1;
				max = Hash_max;
			}
			inline void init(const int _max)
			{
#ifdef Debug
				printf("\'class Hash\'初始化\n");
#endif
				hash = 0;
				a = 1;
				max = _max;
			}
			inline uint getmax() const
			{
				return max;
			}

			template <typename T>
			inline void input(const T &data)
			{
#ifdef Debug
				printf("\'class Hash\'输入%d个字节\n", sizeof(T));
#endif
				for (int i = 0; i < sizeof(T); i++)
				{
					a %= Hash_max;
					hash += a * *(((byte *)&data) + i);
					hash %= Hash_max;
					a *= 0x0100 % Hash_max;
				}
			}
			inline void input(const void *data, const int len)
			{
#ifdef Debug
				printf("\'class Hash\'输入%d个字节\n", len);
#endif
				for (int i = 0; i < len; i++)
				{
					a %= Hash_max;
					hash += a * *(((byte *)data) + i);
					hash %= Hash_max;
					a *= 0x0100 % Hash_max;
				}
			}

			inline void print()
			{
				printf("%08x\n", hash);
			}
		};
		inline ulong gethash(const char *str)
		{
			if (Hash_max == 0)
				return 0;
			ulong h = 0; //返回的hash值
			ulong a = 1;
			int i;
			for (i = 0; *(str + i) != '\0'; i++)
				; //获取字符串长度
			for (i--; i >= 0; i--)
			{
				a %= Hash_max;
				h += (a * *(str + i)) % Hash_max;
				if (h >= Hash_max)
					h -= Hash_max;
				a *= 0x0100 % Hash_max;
			}
			return h;
		}
		inline ulong gethash(const byte *arr, const int len)
		{
			if (Hash_max == 0)
				return 0;
			ulong h = 0; //返回的hash值
			ulong a = 1;
			for (int i = 0; i < len; i++)
			{
				a %= Hash_max;
				h += (a * *(arr + i)) % Hash_max;
				if (h >= Hash_max)
					h -= Hash_max;
				a *= 0x0100 % Hash_max;
			}
			return h;
		}
		inline ulong gethash(const ulong v)
		{
			return v % Hash_max;
		}
		inline ulong gethash(const float v)
		{
			return *((uint32 *)&v) % Hash_max;
		}
		inline ulong gethash(const double v)
		{
			return *((uint64 *)&v) % Hash_max;
		}

		namespace
		{

#define SHA256_swap32(x) (((x) >> 24) | (((x)&0x00FF0000) >> 8) | (((x)&0x0000FF00) << 8) | ((x) << 24))
#define SHA256_swap64(x)                                                      \
	(((x) >> 56) | (((x) >> 40) & 0x000000000000FF00) |                       \
	 (((x) >> 24) & 0x0000000000FF0000) | (((x) >> 8) & 0x00000000FF000000) | \
	 (((x) << 8) & 0x000000FF00000000) | (((x) << 24) & 0x0000FF0000000000) | \
	 (((x) << 40) & 0x00FF000000000000) | ((x) << 56))

#define SHA256_R(v, n) ((v) >> (n))									   // 右移
#define SHA256_S(v, n) (((v) >> (n)) | ((v) << (32 - (n))))			   // 循环右移
#define SHA256_rshift(v, n) ((v) >> (n))							   // 右移
#define SHA256_rshift_cycle(v, n) (((v) >> (n)) | ((v) << (32 - (n)))) // 循环右移
#define SHA256_CH(e, f, g) (((e) & (f)) ^ ((~(e)) & (g)))
#define SHA256_MAJ(a, b, c) (((a) & (c)) ^ ((a) & (b)) ^ ((b) & (c)))
#define SHA256_SIGMA0(x) (SHA256_rshift_cycle(x, 2) ^ SHA256_rshift_cycle(x, 13) ^ SHA256_rshift_cycle(x, 22))
#define SHA256_SIGMA1(x) (SHA256_rshift_cycle(x, 6) ^ SHA256_rshift_cycle(x, 11) ^ SHA256_rshift_cycle(x, 25))
#define SHA256_GAMMA0(x) (SHA256_rshift_cycle(x, 7) ^ SHA256_rshift_cycle(x, 18) ^ SHA256_rshift(x, 3))
#define SHA256_GAMMA1(x) (SHA256_rshift_cycle(x, 17) ^ SHA256_rshift_cycle(x, 19) ^ SHA256_rshift(x, 10))

#define SHA256_h0 0x6a09e667
#define SHA256_h1 0xbb67ae85
#define SHA256_h2 0x3c6ef372
#define SHA256_h3 0xa54ff53a
#define SHA256_h4 0x510e527f
#define SHA256_h5 0x9b05688c
#define SHA256_h6 0x1f83d9ab
#define SHA256_h7 0x5be0cd19

			static const uint32 SHA256_K[] = {
				0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
				0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
				0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
				0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
				0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
				0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
				0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
				0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

		}

		class SHA256
		{
		private:
			bool isBigEndian;
			byte buf[64];
			byte bufp;
			ulong size;

		public:
			byte hash[32]; // sha256值

		private:
			/**
			 * @brief 计算一个（SHA256）块（缓冲区必须满）
			 *
			 */
			void iterate()
			{
#ifdef Debug
				printf("\'class SHA256\'缓冲已满，开始计算\n");
#endif
				uint32 *_h = (uint32 *)hash;
				uint32 *_buf = (uint32 *)buf;
				uint32 a, b, c, d, e, f, g, h; //临时变量
				uint32 temp1, temp2;		   //临时变量
				uint32 word[64];			   //（SHA256）字
				int i;

				//生成每一个（SHA256）字
				if (isBigEndian)
					for (i = 0; i < 16; i++)
						word[i] = _buf[i];
				else
					for (i = 0; i < 16; i++)
						word[i] = SHA256_swap32(_buf[i]);
				for (i = 16; i < 64; i++)
					word[i] = word[i - 16] + SHA256_GAMMA0(word[i - 15]) + word[i - 7] + SHA256_GAMMA1(word[i - 2]);

				//复制数据到临时变量
				a = _h[0];
				b = _h[1];
				c = _h[2];
				d = _h[3];
				e = _h[4];
				f = _h[5];
				g = _h[6];
				h = _h[7];
				for (i = 0; i < 64; i++) //计算每一个（SHA256）字
				{
					temp1 = h + SHA256_SIGMA1(e) + SHA256_CH(e, f, g) + SHA256_K[i] + word[i];
					temp2 = SHA256_SIGMA0(a) + SHA256_MAJ(a, b, c);
					h = g;
					g = f;
					f = e;
					e = d + temp1;
					d = c;
					c = b;
					b = a;
					a = temp1 + temp2;
				}
				//保存迭代结果
				_h[0] += a;
				_h[1] += b;
				_h[2] += c;
				_h[3] += d;
				_h[4] += e;
				_h[5] += f;
				_h[6] += g;
				_h[7] += h;
			}

		public:
			inline SHA256()
			{
				init();
			}

			inline void init()
			{
				uint32 *_h = (uint32 *)hash;
				_h[0] = SHA256_h0;
				_h[1] = SHA256_h1;
				_h[2] = SHA256_h2;
				_h[3] = SHA256_h3;
				_h[4] = SHA256_h4;
				_h[5] = SHA256_h5;
				_h[6] = SHA256_h6;
				_h[7] = SHA256_h7;
				size = 0;
				bufp = 0;
				isBigEndian = (endiancheck() == Big_Endian);
#ifdef Debug
				if (isBigEndian)
					printf("\'class SHA256\'初始化成功，你的电脑是大端的\n");
				else
					printf("\'class SHA256\'初始化成功，你的电脑是小端的\n");
#endif
			}

			template <typename T>
			inline void input(const T &data)
			{
#ifdef Debug
				printf("\'class SHA256\'输入%d个字节\n", sizeof(T));
#endif
				for (int i = 0; i < sizeof(T); i++)
				{
					buf[bufp] = ((byte *)&data)[i];
					bufp++;
					if (bufp == 64)
					{
						iterate();
						bufp = 0;
					}
				}
				size += sizeof(T);
			}
			inline void inputptr(const void *data, const int len)
			{
#ifdef Debug
				printf("\'class SHA256\'输入%d个字节\n", len);
#endif
				const byte *_data = (byte *)data;
				for (int i = 0; i < len; i++)
				{
					buf[bufp] = _data[i];
					bufp++;
					if (bufp == 64)
					{
						iterate();
						bufp = 0;
					}
				}
				size += len;
			}
			inline void input(const void *data, const int len)
			{
				inputptr(data, len);
			}
			inline void inputstr(const char *data)
			{
				const byte *_data = (byte *)data;
				int i;
				for (i = 0; data[i] != '\0'; i++)
				{
					buf[bufp] = _data[i];
					bufp++;
					if (bufp == 64)
					{
						iterate();
						bufp = 0;
					}
				}
				size += i;
#ifdef Debug
				printf("\'class SHA256\'输入%d个字节\n", i);
#endif
			}
			inline void input(const char *data)
			{
				inputstr(data);
			}

			/**
			 * @brief 报文结束时调用
			 *
			 */
			void end()
			{
				buf[bufp] = 0x80;
				bufp++;
				int i = 1;
				for (; bufp != 56; bufp++)
				{
					if (bufp == 64)
					{
						iterate();
						bufp = 0;
					}
					buf[bufp] = 0;
					i++;
				}
#ifdef Debug
				printf("\'class SHA256\'已填充%llu,共%llu\n", i, size + i);
#endif

				*((uint64 *)(buf + 56)) = SHA256_swap64(size * 8);
#ifdef Debug
				printf("\'class SHA256\'已填充文件大小区域,共%llu\n", size + i + 8);
#endif
				iterate();
			}

			inline void print()
			{
#ifdef Debug
				printf("\'class SHA256\'将输出SHA256值，请确保已调用SHA256::end函数\n	");
#endif
				uint32 *_h = (uint32 *)hash;
				printf("%08llx", _h[0]);
				printf("%08llx", _h[1]);
				printf("%08llx", _h[2]);
				printf("%08llx", _h[3]);
				printf("%08llx", _h[4]);
				printf("%08llx", _h[5]);
				printf("%08llx", _h[6]);
				printf("%08llx", _h[7]);
				printf("\n");
			}

			inline void getfromfile(const FD file)
			{
				init();
				while (1)
				{
					bufp = read(file, buf, 64);
					size += bufp;
#ifdef Debug
					printf("\'class SHA256\'成功读取%d,已读取%llu\n", bufp, size);
#endif
					if (bufp == 64)
						iterate();
					else
						break;
				}
				end();
			}
			inline bool getfromfile(const char *path)
			{
				FD file = open(path, O_RDONLY);
				if (file < 0)
					return false;
				init();
				while (1)
				{
					bufp = read(file, buf, 64);
					size += bufp;
#ifdef Debug
					printf("\'class SHA256\'成功读取%d,已读取%llu\n", bufp, size);
#endif
					if (bufp == 64)
						iterate();
					else
						break;
				}
				end();
				return true;
			}
			inline void getfromstr(const char *str)
			{
				init();
				const byte *_str = (byte *)str;
				int i;
				for (i = 0; str[i] != '\0'; i++)
				{
					buf[bufp] = _str[i];
					bufp++;
					if (bufp == 64)
					{
						iterate();
						bufp = 0;
					}
				}
				size = i;
				end();
			}
			inline void getfromptr(const void *data, const int len)
			{
				init();
				inputptr(data, len);
				end();
			}
			inline void get(const FD file)
			{
				getfromfile(file);
			}
			inline void get(const char *str)
			{
				getfromstr(str);
			}
			inline void get(const void *data, const int len)
			{
				getfromptr(data, len);
			}
		};

		namespace
		{

#define MD5_swap32(x) (((x) >> 24) | (((x)&0x00FF0000) >> 8) | (((x)&0x0000FF00) << 8) | ((x) << 24))
#define MD5_swap64(x)                                                         \
	(((x) >> 56) | (((x) >> 40) & 0x000000000000FF00) |                       \
	 (((x) >> 24) & 0x0000000000FF0000) | (((x) >> 8) & 0x00000000FF000000) | \
	 (((x) << 8) & 0x000000FF00000000) | (((x) << 24) & 0x0000FF0000000000) | \
	 (((x) << 40) & 0x00FF000000000000) | ((x) << 56))

#define MD5_A 0x67452301
#define MD5_B 0xefcdab89
#define MD5_C 0x98badcfe
#define MD5_D 0x10325476

#define MD5_L(v, n) (((v) << (n)) | ((v) >> (32 - (n)))) // 循环左移

#define MD5_F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define MD5_G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define MD5_H(x, y, z) ((x) ^ (y) ^ (z))
#define MD5_I(x, y, z) ((y) ^ ((x) | (~z)))

			//循环左移的位数
			static const byte MD5_S_v[4][4] = {
				{7, 12, 17, 22},
				{5, 9, 14, 20},
				{4, 11, 16, 23},
				{6, 10, 15, 21}};

			//每轮循环中子组处理顺序
			static const byte MD5_oi[4][16] = {
				{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
				{1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12},
				{5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2},
				{0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9}};

			static const uint32 MD5_ac[4][16] = {
				{0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
				 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821},
				{0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
				 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a},
				{0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
				 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665},
				{0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
				 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391}};

		}

		class MD5
		{

		private:
			bool isBigEndian;
			byte buf[64];
			byte bufp;
			ulong size;

		public:
			byte hash[16]; // MD5值

		private:
			/**
			 * @brief 计算一个（MD5）块（缓冲区必须满）
			 *
			 */
			void iterate()
			{
#ifdef Debug
				printf("\'class MD5\'缓冲已满，开始计算\n");
#endif

				uint32 *_h = (uint32 *)hash;
				uint32 *_buf = (uint32 *)buf;
				uint32 a, b, c, d; //临时变量
				uint32 temp1;	   //临时变量
				int i;

				// if (!isBigEndian)
				// for (i = 0; i < 16; i++)
				//_buf[i] = MD5_swap32(_buf[i]);

				//复制数据到临时变量
				a = _h[0];
				b = _h[1];
				c = _h[2];
				d = _h[3];
				//第1轮变换
				for (i = 0; i < 16; i++)
				{
					temp1 = a + MD5_F(b, c, d) + _buf[i] + MD5_ac[0][i];
					temp1 = MD5_L(temp1, MD5_S_v[0][i % 4]);
					a = d;
					d = c;
					c = b;
					b += temp1;
				}
				//第2轮变换
				for (i = 0; i < 16; i++)
				{
					temp1 = a + MD5_G(b, c, d) + _buf[(5 * i + 1) % 16] + MD5_ac[1][i];
					temp1 = MD5_L(temp1, MD5_S_v[1][i % 4]);
					a = d;
					d = c;
					c = b;
					b += temp1;
				}
				//第3轮变换
				for (i = 0; i < 16; i++)
				{
					temp1 = a + MD5_H(b, c, d) + _buf[(3 * i + 5) % 16] + MD5_ac[2][i];
					temp1 = MD5_L(temp1, MD5_S_v[2][i % 4]);
					a = d;
					d = c;
					c = b;
					b += temp1;
				}
				//第4轮变换
				for (i = 0; i < 16; i++)
				{
					temp1 = a + MD5_I(b, c, d) + _buf[(7 * i) % 16] + MD5_ac[3][i];
					temp1 = MD5_L(temp1, MD5_S_v[3][i % 4]);
					a = d;
					d = c;
					c = b;
					b += temp1;
				}
				//保存迭代结果
				_h[0] += a;
				_h[1] += b;
				_h[2] += c;
				_h[3] += d;
			}

		public:
			inline MD5()
			{
				init();
			}

			inline void init()
			{
				uint32 *_h = (uint32 *)hash;
				_h[0] = MD5_A;
				_h[1] = MD5_B;
				_h[2] = MD5_C;
				_h[3] = MD5_D;
				size = 0;
				bufp = 0;
				isBigEndian = (endiancheck() == Big_Endian);
#ifdef Debug
				if (isBigEndian)
					printf("\'class MD5\'初始化成功，你的电脑是大端的\n");
				else
					printf("\'class MD5\'初始化成功，你的电脑是小端的\n");
#endif
			}

			template <typename T>
			inline void input(const T &data)
			{
#ifdef Debug
				printf("\'class MD5\'输入%d个字节\n", sizeof(T));
#endif
				for (int i = 0; i < sizeof(T); i++)
				{
					buf[bufp] = ((byte *)&data)[i];
					bufp++;
					if (bufp == 64)
					{
						iterate();
						bufp = 0;
					}
				}
				size += sizeof(T);
			}
			inline void inputptr(const void *data, const int len)
			{
#ifdef Debug
				printf("\'class MD5\'输入%d个字节\n", len);
#endif
				const byte *_data = (byte *)data;
				for (int i = 0; i < len; i++)
				{
					buf[bufp] = _data[i];
					bufp++;
					if (bufp == 64)
					{
						iterate();
						bufp = 0;
					}
				}
				size += len;
			}
			inline void input(const void *data, const int len)
			{
				inputptr(data, len);
			}
			inline void inputstr(const char *data)
			{
				const byte *_data = (byte *)data;
				int i;
				for (i = 0; data[i] != '\0'; i++)
				{
					buf[bufp] = _data[i];
					bufp++;
					if (bufp == 64)
					{
						iterate();
						bufp = 0;
					}
				}
				size += i;
#ifdef Debug
				printf("\'class SHA256\'输入%d个字节\n", i);
#endif
			}
			inline void input(const char *data)
			{
				inputstr(data);
			}

			/**
			 * @brief 报文结束时调用
			 *
			 */
			void end()
			{
				buf[bufp] = 0x80;
				bufp++;
				int i = 1;
				for (; bufp != 56; bufp++)
				{
					if (bufp == 64)
					{
						iterate();
						bufp = 0;
					}
					buf[bufp] = 0;
					i++;
				}
#ifdef Debug
				printf("\'class MD5\'已填充%llu,共%llu\n", i, size + i);
#endif

				//*((uint64 *)(buf + 56)) = MD5_swap64(size * 8);
				*((uint64 *)(buf + 56)) = size * 8;
#ifdef Debug
				printf("\'class MD5\'已填充文件大小区域,共%llu\n", size + i + 8);
#endif
				iterate();
			}

			inline void print()
			{
#ifdef Debug
				printf("\'class MD5\'将输出MD5值，请确保已调用MD5::end函数\n	");
#endif
				for (int i = 0; i < 16; i++)
					printf("%02x", hash[i]);
				printf("\n");
			}

			inline void getfromfile(const FD file)
			{
				init();
				while (1)
				{
					bufp = read(file, buf, 64);
					size += bufp;
#ifdef Debug
					printf("\'class MD5\'成功读取%d,已读取%llu\n", bufp, size);
#endif
					if (bufp == 64)
						iterate();
					else
						break;
				}
				end();
			}
			inline bool getfromfile(const char *path)
			{
				FD file = open(path, O_RDONLY);
				if (file < 0)
					return false;
				init();
				while (1)
				{
					bufp = read(file, buf, 64);
					size += bufp;
#ifdef Debug
					printf("\'class MD5\'成功读取%d,已读取%llu\n", bufp, size);
#endif
					if (bufp == 64)
						iterate();
					else
						break;
				}
				end();
				return true;
			}
			inline void getfromstr(const char *str)
			{
				init();
				const byte *_str = (byte *)str;
				int i;
				for (i = 0; str[i] != '\0'; i++)
				{
					buf[bufp] = _str[i];
					bufp++;
					if (bufp == 64)
					{
						iterate();
						bufp = 0;
					}
				}
				size = i;
				end();
			}
			inline void getfromptr(const void *data, const int len)
			{
				init();
				inputptr(data, len);
				end();
			}
			inline void get(const FD file)
			{
				getfromfile(file);
			}
			inline void get(const char *str)
			{
				getfromstr(str);
			}
			inline void get(const void *data, const int len)
			{
				getfromptr(data, len);
			}
		};
	}
}

#endif

#endif