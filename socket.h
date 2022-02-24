#pragma once

/**
 * @brief socket.h
 *
 */

#ifndef _hylib_socket_h_
#define _hylib_socket_h_

#ifdef Linux_OS
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#ifdef Windows_OS
#include <WinSock2.h>
#endif

#include <pthread.h>

#include <hylib/basic/basic.h>
#include <hylib/hash/hashfunc.h>

#ifdef __cplusplus
namespace hylib
{
	namespace network
	{
		namespace defines
		{
#define IPv4 AF_INET
			//#define IPv6 AF_INET6

#define STREAM SOCK_STREAM

#define TCP IPPROTO_TCP
#define UDP IPPROTO_UDP
			//#define STCP IPPROTO_SCTP
		}

		class Connection
		{
		public:
			bool isready;
			int domain;
			int type;
			int protocol;
#ifdef Linux_OS
			FD sock;
#endif
#ifdef Windows_OS
			SOCKET sock;
#endif
			union
			{
				struct sockaddr_in addr;
				// struct sockaddr_in6 addr6;
			};
			void (*connect_callback)(class Connection &arg);

			inline Connection()
			{
				init();
			}
			inline ~Connection()
			{
				destroy();
			}

			inline void init()
			{
				isready = false;
				domain = IPv4;
				type = STREAM;
				protocol = TCP;
				sock = -1;
				connect_callback = NULL;
			}
			/**
			 * @brief 关闭套接字
			 *
			 */
			inline void destroy()
			{
				if (sock >= 0)
				{
#ifdef Linux_OS
					close(sock);
#endif
#ifdef Windows_OS
					closesocket(sock);
#endif
				}
			}
			inline void reinit()
			{
				destroy();
				init();
			}

			inline bool connectto(const char *ip, const ushort port)
			{
				if (isready)
					destroy();
				sock = socket(domain, type, protocol);
				if (sock < 0)
				{
					isready = false;
					return false;
				}
				/* 连接服务器 */
				if (domain == IPv4)
				{
					for (int i = 0; i < sizeof(struct sockaddr_in); i++)
						*(((byte *)&addr) + i) = 0;
					addr.sin_family = IPv4;
					addr.sin_addr.s_addr = inet_addr(ip);
					addr.sin_port = htons(port);
				}
				if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
				{
					isready = false;
					destroy();
					return false;
				}
				isready = true;
				if (connect_callback != NULL)
					connect_callback(*this);
				destroy();
				isready = false;
				return true;
			}

			inline bool senddata(const void *data, const int len) const
			{
				int sendsize = 0;
				int buf;
				while (sendsize < len)
				{
					buf = send(sock, (void *)((byte *)data + sendsize), len - sendsize, 0);
					if (buf <= 0)
						return false;
					sendsize += buf;
				}
				return true;
			}
			inline bool sendstr(const char *data) const
			{
				int i;
				for (i = 0; *(data + i) != '\0'; i++)
					if (send(sock, data + i, 1, 0) <= 0)
						return false;
				if (send(sock, data + i, 1, 0) <= 0)
					return false;
				return true;
			}
			template <typename T>
			inline bool senddata(const T &data) const
			{
				return senddata(&data, sizeof(T));
			}

			inline bool recvdata(void *data, const int len) const
			{
				int recvsize = 0;
				int buf;
				while (recvsize < len)
				{
					buf = recv(sock, (void *)((byte *)data + recvsize), len - recvsize, 0);
					if (buf <= 0)
						return false;
					recvsize += buf;
				}
				return true;
			}
			inline bool recvstr(char *data, const int maxsize) const
			{
				int recvsize = 0;
				char buf;
				int i;
				for (i = 0; i < maxsize - 1; i++)
				{
					if (recv(sock, &buf, 1, 0) <= 0)
					{
						*(data + i) = '\0';
						return false;
					}
					*(data + i) = buf;
					if (buf == '\0')
						return true;
				}
				*(data + i) = '\0';
				return true;
			}
			template <typename T>
			inline bool recvdata(T &data) const
			{
				return recvdata(&data, sizeof(T));
			}

			inline bool sendsha256(const void *data, const int len) const
			{
				hash::SHA256 sha;
				sha.input(data, len);
				int sendsize = 0;
				int buf;
				while (sendsize < 32)
				{
					buf = send(sock, (void *)(sha.hash + sendsize), 32 - sendsize, 0);
					if (buf < 0)
						return false;
					sendsize += buf;
				}
				return true;
			}
			inline bool recvsha256(byte data[32]) const
			{
				int sendsize = 0;
				int buf;
				while (sendsize < 32)
				{
					buf = recv(sock, (void *)(data + sendsize), 32 - sendsize, 0);
					if (buf < 0)
						return false;
					sendsize += buf;
				}
				return true;
			}

			/**
			 * @brief 加密发送数据（接收端需要调用recvsafe函数）
			 *
			 * @param type 用来标记当前发送的数据（用户自定义）
			 * @param data 指向数据的指针
			 * @param len 数据的长度
			 * @param passwd 密码的SHA256值
			 * @return 是否发送成功
			 */
			bool sendsafe(const int type, const void *data, const int len, const byte passwd[32]) const
			{
				/* 生成SHA256值 */
				hash::SHA256 sha256;
				sha256.input(type);
				sha256.input(data, len);
				sha256.end();
				/* 加密并发送数据 */
				byte buf[32];
				for (int i = 0; i < 4; i++)
					buf[i] = *(((byte *)&type) + i) ^ passwd[(len + i) % 32];
				if (!senddata(buf, 4)) //数据类型
					return false;
				if (!senddata(len)) //数据体长度
					return false;
				if (len != 0)
					for (int i = 0; i < len; i += 32) //数据体
					{
						for (int i2 = 0; (i2 < 32) && (i + i2 < len); i2++)
							buf[i2] = *(((byte *)data) + i + i2) ^ passwd[i2];
						if (i + 32 > len)
						{
							if (!senddata(buf, len - i))
								return false;
						}
						else
						{
							if (!senddata(buf, 32))
								return false;
						}
					}
				if (!senddata(sha256.hash, 32)) // HSA256
					return false;
				return true;
			}
			/**
			 * @brief 接收加密数据（发送端需要调用sendsafe函数）
			 *
			 * @param type 当前接收的数据的标记（用户自定义）
			 * @param data 指向数据的指针
			 * @param len 数据的长度
			 * @param passwd 密码的SHA256值
			 * @return 是否接收成功（若密码错误type将会被置-1）
			 */
			bool recvsafe(int &type, void *&data, int &len, const byte passwd[32]) const
			{
				{
					type = 0;
					if (data != NULL)
						free(data);
					data = NULL;
					len = 0;
				}
				hash::SHA256 sha256;
				/* 接收并解密数据 */
				byte buf[32];
				if (!recvdata(buf, 4)) //数据类型
					return false;
				if (!recvdata(len)) //数据体长度
					return false;
				for (int i = 0; i < 4; i++)
					*(((byte *)&type) + i) = buf[i] ^ passwd[(len + i) % 32];
				sha256.input(type);
				if (len != 0)
				{
					data = malloc(len);
					if (data == NULL)
					{
						type = -1;
						len = 0;
						return false;
					}
					for (int i = 0; i < len; i += 32) //数据体
					{
						if (i + 32 > len)
						{
							if (!recvdata(buf, len - i))
								return false;
						}
						else
						{
							if (!recvdata(buf, 32))
								return false;
						}
						for (int i2 = 0; (i2 < 32) && (i + i2 < len); i2++)
							*(((byte *)data) + i + i2) = buf[i2] ^ passwd[i2];
					}
					sha256.input(data, len);
				}
				/* 生成SHA256值 */
				sha256.end();
				if (!recvdata(buf, 32))
					return false;
				for (int i = 0; i < 32; i++)
					if (sha256.hash[i] != buf[i])
					{
						type = -1;
						if (data != NULL)
							free(data);
						data = NULL;
						len = 0;
						return false;
					}
				return true;
			}
		};

		class ServerSock
		{
		public:
			bool isready;
			int domain;
			int type;
			int protocol;
#ifdef Linux_OS
			FD sock;
#endif
#ifdef Windows_OS
			SOCKET sock;
#endif
			int queuemax;
			union
			{
				struct sockaddr_in addr;
				// struct sockaddr_in6 addr6;
			};
			void (*connect_callback)(class Connection &arg);

			inline ServerSock()
			{
				init();
			}
			inline ~ServerSock()
			{
				destroy();
			}

			inline void init()
			{
				isready = false;
				domain = IPv4;
				type = STREAM;
				protocol = TCP;
				sock = -1;
				queuemax = 20;
				connect_callback = NULL;
			}
			inline void destroy()
			{
				if (sock >= 0)
#ifdef Linux_OS
					close(sock);
#endif
#ifdef Windows_OS
				closesocket(sock);
#endif
			}
			inline void reinit()
			{
				destroy();
				init();
			}

			inline bool startserver(const char *ip, const ushort port)
			{
				if (isready)
					destroy();
				sock = socket(domain, type, protocol);
				if (sock < 0)
				{
					isready = false;
					return false;
				}
				/* 绑定端口 */
				if (domain == IPv4)
				{
					for (int i = 0; i < sizeof(struct sockaddr_in); i++)
						*(((byte *)&addr) + i) = 0;
					addr.sin_family = IPv4;
					addr.sin_addr.s_addr = inet_addr(ip);
					addr.sin_port = htons(port);
					if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
					{
						isready = false;
#ifdef Linux_OS
						close(sock);
#endif
#ifdef Windows_OS
						closesocket(sock);
#endif
						return false;
					}
				}
				/* 监听端口 */
				if (listen(sock, queuemax) < 0)
				{
					isready = false;
#ifdef Linux_OS
					close(sock);
#endif
#ifdef Windows_OS
					closesocket(sock);
#endif
					return false;
				}
				isready = true;
				/* 连接客户端 */
				while (1)
				{
					class Connection *connectarg = (class Connection *)malloc(sizeof(class Connection));
					if (connectarg == NULL)
						return true;
					connectarg->domain = domain;
					connectarg->type = type;
					connectarg->protocol = protocol;
					connectarg->connect_callback = connect_callback;
					uint size = sizeof(sockaddr);
					while ((connectarg->sock = accept(sock, (sockaddr *)&connectarg->addr, &size)) < 0)
						;
					pthread_t t;
					pthread_create(&t, NULL, connectclient, connectarg);
					pthread_detach(t);
				}
				return true;
			}
			static void *connectclient(void *arg)
			{
				class Connection *connectarg = (class Connection *)arg;
				if (connectarg->connect_callback != NULL)
					connectarg->connect_callback(*connectarg);
#ifdef Linux_OS
				close(connectarg->sock);
#endif
#ifdef Windows_OS
				closesocket(connectarg->sock);
#endif
				return NULL;
			}
		};

	}
}

#endif

#endif