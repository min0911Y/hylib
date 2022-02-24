#pragma once

// endecrypt.h
#ifndef _hylib_endecrypt_h_
#define _hylib_endecrypt_h_

#include <hylib/hash func.h>

#ifdef __cplusplus
namespace hylib
{
	namespace endecrypt
	{

		/**
		 * @brief 加密、解密二进制数据
		 *
		 * @param data 要加密的二进制数据
		 * @param len 数据的长度
		 * @param passwd 密码
		 * @param passwdlen 密码长度
		 */
		inline void endecrypt(void *data, const int len, const void *passwd, const int passwdlen)
		{
			for (int i = 0; i < len; i++)
				*((byte *)data + i) ^= *((byte *)passwd + i % passwdlen);
		}
		/**
		 * @brief 加密、解密二进制数据
		 *
		 * @param data1 要加密的二进制数据
		 * @param data2 要加密的二进制数据
		 * @param len 数据的长度
		 * @param passwd 密码
		 * @param passwdlen 密码长度
		 */
		inline void endecrypt(void *data1, const void *data2, const int len, const void *passwd, const int passwdlen)
		{
			for (int i = 0; i < len; i++)
				*((byte *)data1 + i) = *((byte *)data1 + i) ^ *((byte *)passwd + i % passwdlen);
		}
		/**
		 * @brief 加密、解密二进制数据
		 *
		 * @param data 要加密的二进制数据
		 * @param len 数据的长度
		 * @param passwd 密码
		 */
		inline void endecrypt_sha256(void *data, const int len, const byte passwd[32])
		{
			endecrypt(data, len, passwd, 32);
		}

		/**
		 * @brief 使用sha256值加密二进制数据
		 *
		 * @param data 要加密的二进制数据
		 * @param len 数据的长度
		 * @param sha256 密码的sha256值
		 */
		inline void encrypt(void *data, const int len, byte sha256[32])
		{
			for (int i = 0; i < len; i++)
				*((byte *)data + i) = *((byte *)data + i) ^ sha256[i % 32];
		}
		/**
		 * @brief 使用密码的sha256值加密二进制数据
		 *
		 * @param str 要加密的二进制数据
		 * @param len 数据的长度
		 * @param passwd 密码（字符串）
		 */
		inline void encrypt(void *data, const int len, const char *passwd)
		{
			hash::SHA256 sha256;
			sha256.getfromstr(passwd);
			encrypt(data, len, sha256.hash);
		}
		/**
		 * @brief 使用密码的sha256值加密二进制数据
		 *
		 * @param str 要加密的二进制数据
		 * @param len 数据的长度
		 * @param passwd 密码（二进制数据）
		 * @param passwdlen 密码的长度
		 */
		inline void encrypt(void *data, const int len, const void *passwd, const int passwdlen)
		{
			hash::SHA256 sha256;
			sha256.getfromptr(passwd, passwdlen);
			encrypt(data, len, sha256.hash);
		}
		/**
		 * @brief 使用sha256值加密字符串
		 *
		 * @param str 要加密的字符串
		 * @param sha256 密码的sha256值
		 */
		inline void encrypt(char *str, byte sha256[32])
		{
			for (int i = 0; *(str + i) != '\0'; i++)
				*(str + i) = *(str + i) ^ sha256[i % 32];
		}
		/**
		 * @brief 使用密码的sha256值加密字符串
		 *
		 * @param str 要加密的字符串
		 * @param passwd 密码（字符串）
		 */
		inline void encrypt(char *str, const char *passwd)
		{
			hash::SHA256 sha256;
			sha256.getfromstr(passwd);
			encrypt(str, sha256.hash);
		}
		/**
		 * @brief 使用密码的sha256值加密字符串
		 *
		 * @param str 要加密的字符串
		 * @param passwd 密码（二进制数据）
		 * @param passwdlen 密码的长度
		 */
		inline void encrypt(char *str, const void *passwd, const int passwdlen)
		{
			hash::SHA256 sha256;
			sha256.getfromptr(passwd, passwdlen);
			encrypt(str, sha256.hash);
		}

		inline bool encrypt(const char *file1, const char *file2, const char *passwd)
		{
			int ifile = open(file1, O_RDONLY);
			int ofile = open(file2, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if (ifile < 0 || ofile < 0)
				return false;
			char c = 0;
			int passwdptr = 0;
			Hash hash;
			for (int i = 0; i < 12; i++)
				write(ofile, &c, 1);
			while (read(ifile, &c, 1) == 1)
			{
				c ^= passwd[passwdptr];
				write(ofile, &c, 1);
				passwdptr++;
				if (passwd[passwdptr] == '\0')
					passwdptr = 0;
				hash.input(c);
			}
			write(ofile, &hash.data, 256);

			lseek(ofile, 0, SEEK_SET);
			write(ofile, "cryp", 4);
			Long size = hash.inputlen + 256;
			write(ofile, &size, 8);

			close(ifile);
			close(ofile);
			return true;
		}
		bool decrypt(const char *file1, const char *file2, const char *passwd)
		{
			int ifile = open(file1, O_RDONLY);
			int ofile = open(file2, O_WRONLY | O_CREAT, 644);
			if (ifile < 0 || ofile < 0)
				return false;
			char c = 0;
			Long size;
			int passwdptr = 0;
			Hash hash;
			read(ifile, &size, 4);
			if ((int32)size != 'cryp')
			{
				close(ifile);
				close(ofile);
				remove(file2);
				return false;
			}
			read(ifile, &size, 8);
			size -= 256;
			for (Long i = 0; i < size; i++)
			{
				if (read(ifile, &c, 1) != 1)
				{
					close(ifile);
					close(ofile);
					remove(file2);
					return false;
				}
				hash.input(c);
				c ^= passwd[passwdptr];
				write(ofile, &c, 1);
				passwdptr++;
				if (passwd[passwdptr] == '\0')
					passwdptr = 0;
			}
			for (int i = 0; i < 256; i++)
			{
				if (read(ifile, &c, 1) != 1 || hash.data[i] != c)
				{
					close(ifile);
					close(ofile);
					remove(file2);
					return false;
				}
			}
			close(ifile);
			close(ofile);
			return true;
		}

	}
}

#endif

#endif