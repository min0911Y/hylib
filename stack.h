
/**
 * @brief hylib/basictypes/stack.h
 * 栈
 *
 */

#pragma once

#ifndef _hylib_stack_h_
#define _hylib_stack_h_

// C头
#ifndef __cplusplus
//标准库
#ifndef _hylib_includes_stdlib_
#define _hylib_includes_stdlib_
#include <stdlib.h>
#endif
#endif
// C++头
#ifdef __cplusplus
//标准库
#ifndef _hylib_includes_cstdlib_
#define _hylib_includes_cstdlib_
#include <cstdlib>
#endif
#endif

#ifdef __cplusplus
namespace hylib
{

	namespace basictypes
	{
		template <typename T>
		class SizeableStack
		{
		public:
			T *data;			   //数据
			int top;			   //栈顶部的位置
			int size;			   //栈申请的内存大小
			int malloc_increments; //每次增加或减少的内存量

		public:
			bool isready; //栈是否已就绪
			/**
			 * @brief 构造可变大小的栈
			 */
			inline SizeableStack()
			{
				init();
			}
			/**
			 * @brief 构造可变大小的栈
			 * @param _malloc_increments 每次增加或减少的内存量
			 */
			inline SizeableStack(const int _malloc_increments)
			{
				init(_malloc_increments);
			}
			/**
			 * @brief 构造可变大小的栈
			 * @param _stack 从指定栈复制
			 */
			inline SizeableStack(const class SizeableStack<T> &_stack)
			{
				init(_stack);
			}
			/**
			 * @brief 析构
			 */
			inline ~SizeableStack()
			{
				destroy();
			}

			/**
			 * @brief 初始化
			 */
			inline void init()
			{
				isready = false;
				data = NULL;
				top = -1;
				size = 0;
				malloc_increments = 32;
			}
			/**
			 * @brief 初始化
			 * @param _malloc_increments 每次增加或减少的内存量
			 * @return 是否初始化成功
			 */
			inline bool init(const int _malloc_increments)
			{
				top = -1;
				malloc_increments = _malloc_increments;
				data = (T *)malloc(malloc_increments * sizeof(T));
				if (data == NULL)
				{
					isready = false;
					size = 0;
					return false;
				}
				else
				{
					isready = true;
					size = malloc_increments;
					return true;
				}
			}
			/**
			 * @brief 初始化
			 * @param _stack 从指定栈复制
			 * @return 是否初始化成功
			 */
			inline bool init(const class SizeableStack<T> &_stack)
			{
				init();
				return copyfrom(_stack);
			}
			/**
			 * @brief 释放内存
			 */
			inline void destroy()
			{
				if (isready)
					free(data); // data:我免费了！
				init();
			}
			/**
			 * @brief 重新初始化
			 * @return 是否初始化成功
			 */
			inline bool reinit()
			{
				int _malloc_increments = malloc_increments;
				destroy();
				return init(_malloc_increments);
			}
			/**
			 * @brief 重新初始化
			 * @param _malloc_increments 每次增加或减少的内存量
			 * @return 是否初始化成功
			 */
			inline bool reinit(const int _malloc_increments)
			{
				destroy();
				return init(_malloc_increments);
			}
			/**
			 * @brief 获取大小
			 * @return 栈的大小
			 */
			inline int getsize() const
			{
				return size;
			}
			/**
			 * @brief 设置大小
			 * @param _size 栈的大小
			 * @return 是否设置成功
			 */
			inline bool setsize(const int _size)
			{
				if (isready)
				{
					if (_size <= 0)
						return false;
					T *_data = (T *)realloc(data, _size * sizeof(T));
					if (_data == NULL)
						return false;
					else
					{
						data = _data;
						size = _size;
						if (top >= size)
							top = size - 1;
						return true;
					}
				}
				else
				{
					init();
					data = (T *)malloc(_size * sizeof(T));
					if (data == NULL)
						return false;
					else
					{
						size = _size;
						return true;
					}
				}
			}

			/**
			 * @brief 获取该栈是否为空
			 * @return 是否为空
			 */
			inline bool isempty() const
			{
				return top == -1;
			}
			/**
			 * @brief 获取该栈储存的元素数
			 * @return 储存的元素数
			 */
			inline int getitemnum() const
			{
				return top + 1;
			}

			/**
			 * @brief 获取指定元素
			 * @param index 指定元素在栈中的位置
			 * @param item 储存指定元素
			 * @return 是否获取成功
			 */
			inline bool get(const int index, T &item) const
			{
				if (index >= 0 && index <= top)
				{
					item = data[index];
					return true;
				}
				else
				{
					errno = EFAULT;
					return false;
				}
			}
			/**
			 * @brief 获取指定元素
			 * @param index 指定元素在栈中的位置
			 * @param item 储存指定元素
			 * @return 是否获取成功
			 */
			inline bool getcpy(const int index, T &item) const
			{
				if (index >= 0 && index <= top)
				{
					memcpy(&item, data + index);
					return true;
				}
				else
				{
					errno = EFAULT;
					return false;
				}
			}
			/**
			 * @brief 将元素压入栈
			 * @return 是否压入成功
			 */
			inline bool push()
			{
				if (top >= size - 1)
					if (!setsize(size + malloc_increments))
					{
						errno = EFAULT;
						return false;
					}
				top++;
				memclear(data + top);
				return true;
			}
			/**
			 * @brief 将元素压入栈
			 * @param item 要压入的元素
			 * @return 是否压入成功
			 */
			inline bool push(const T &item)
			{
				if (top >= size - 1)
					if (!setsize(size + malloc_increments))
					{
						errno = EFAULT;
						return false;
					}
				top++;
				data[top] = item;
				return true;
			}
			/**
			 * @brief 将元素压入栈
			 * @param item 要压入的元素
			 * @return 是否压入成功
			 */
			inline bool pushcpy(const T &item)
			{
				if (top >= size - 1)
					if (!setsize(size + malloc_increments))
					{
						errno = EFAULT;
						return false;
					}
				top++;
				memcpy(data + top, &item);
				return true;
			}
			/**
			 * @brief 将元素弹出栈
			 * @return 是否弹出成功
			 */
			inline bool pop()
			{
				if (top >= 0)
				{
					top--;
					if (top < size - malloc_increments)
						setsize(size - malloc_increments);
				}
				else
				{
					errno = EFAULT;
					return false;
				}
			}
			/**
			 * @brief 将元素弹出栈
			 * @param item 储存弹出的元素
			 * @return 是否弹出成功
			 */
			inline bool pop(T &item)
			{
				if (top >= 0)
				{
					item = data[top];
					top--;
					if (top < size - malloc_increments)
						setsize(size - malloc_increments);
				}
				else
				{
					errno = EFAULT;
					return false;
				}
			}
			/**
			 * @brief 将元素弹出栈
			 * @param item 储存弹出的元素
			 * @return 是否弹出成功
			 */
			inline bool popcpy(T &item)
			{
				if (top >= 0)
				{
					memcpy(&item, data + top);
					top--;
					if (top < size - malloc_increments)
						setsize(size - malloc_increments);
				}
				else
				{
					errno = EFAULT;
					return false;
				}
			}

			/**
			 * @brief 将栈复制到
			 * @param stack 目标栈
			 * @return 是否复制成功
			 */
			inline bool copyto(class SizeableStack<T> &stack) const
			{
				if (isready)
					if (stack.setsize(size))
					{
						stack.malloc_increments = malloc_increments;
						memcpy(stack.data, data, size * sizeof(T));
						return true;
					}
				return false;
			}
			/**
			 * @brief 从指定栈复制
			 * @param stack 指定栈
			 * @return 是否复制成功
			 */
			inline bool copyfrom(class SizeableStack<T> &stack)
			{
				if (stack.isready)
					if (setsize(size))
					{
						malloc_increments = stack.malloc_increments;
						memcpy(data, stack.data, stack.size * sizeof(T));
						return true;
					}
				return false;
			}

			/**
			 * @brief 获取指定元素
			 * @param index 指定元素在栈中的位置
			 * @return 指定元素
			 */
			inline T &operator[](const int index) const
			{
				if (index >= 0 && index <= top)
					return data[index];
				else
				{
					errno = EFAULT;
					return data[size - 1];
				}
			}
			/**
			 * @brief 将元素压入栈
			 * @param item 要压入的元素
			 */
			inline class SizeableStack<T> &operator<<(const T &item)
			{
				pushcpy(item);
				return *this;
			}
			/**
			 * @brief 将元素弹出栈
			 * @param item 储存弹出的元素
			 */
			inline class SizeableStack<T> &
			operator>>(T &item)
			{
				popcpy(item);
				return *this;
			}
		};

		template <typename T>
		class FixedCapacityStack
		{
		private:
			bool isready; //
			T *data;	  //
			int top;	  //
			int size;	  //

		public:
			/**
			 * @brief 构造定容的单一类型的栈
			 */
			inline FixedCapacityStack()
			{
				init();
			}
			/**
			 * @brief 构造定容的单一类型的栈
			 * @param _size 栈的大小
			 */
			inline FixedCapacityStack(const int _size)
			{
				init(_size);
			}
			/**
			 * @brief 构造定容的单一类型的栈
			 * @param _stack 从指定栈复制
			 */
			inline FixedCapacityStack(const class FixedCapacityStack<T> &_stack)
			{
				init(_stack);
			}
			/**
			 * @brief 析构
			 */
			inline ~FixedCapacityStack()
			{
				destroy();
			}

			/**
			 * @brief 初始化
			 */
			inline void init()
			{
				isready = false;
				data = NULL;
				top = -1;
				size = 0;
			}
			/**
			 * @brief 初始化
			 * @param _size 栈的大小
			 * @return 是否初始化成功
			 */
			inline bool init(const int _size)
			{
				top = -1;
				data = (T *)malloc(_size * sizeof(T));
				if (data == NULL)
				{
					isready = false;
					size = 0;
					return false;
				}
				else
				{
					isready = true;
					size = _size;
					return true;
				}
			}
			/**
			 * @brief 初始化
			 * @param _stack 从指定栈复制
			 * @return 是否初始化成功
			 */
			inline bool init(const class FixedCapacityStack<T> &_stack)
			{
				init();
				return copyfrom(_stack);
			}
			/**
			 * @brief 释放内存
			 */
			inline void destroy()
			{
				if (isready)
					free(data); // data:我免费了！
				init();
			}
			/**
			 * @brief 重新初始化
			 * @return 是否初始化成功
			 */
			inline bool reinit()
			{
				int _size = size;
				destroy();
				return init(_size);
			}
			/**
			 * @brief 重新初始化
			 * @param _size 栈的大小
			 * @return 是否初始化成功
			 */
			inline bool reinit(const int _size)
			{
				destroy();
				return init(_size);
			}
			/**
			 * @brief 获取大小
			 * @return 栈的大小
			 */
			inline int getsize() const
			{
				return size;
			}
			/**
			 * @brief 设置大小
			 * @param _size 栈的大小
			 * @return 是否设置成功
			 */
			inline bool setsize(const int _size)
			{
				if (size > 0)
					if (isready)
					{
						T *_data = (T *)realloc(data, _size * sizeof(T));
						if (_data == NULL)
							return false;
						else
						{
							data = _data;
							size = _size;
							if (top >= size)
								top = size - 1;
							return true;
						}
					}
					else
						return init(_size);
				else if (size < 0)
					return false;
				else
				{
					destroy();
					return true;
				}
			}

			/**
			 * @brief 获取该栈是否为空
			 * @return 是否为空
			 */
			inline bool isempty() const
			{
				return top == -1;
			}
			/**
			 * @brief 获取该栈储存的元素数
			 * @return 储存的元素数
			 */
			inline int getitemnum() const
			{
				return top + 1;
			}

			/**
			 * @brief 获取指定元素
			 * @param index 指定元素在栈中的位置
			 * @param item 储存指定元素
			 * @return 是否获取成功
			 */
			inline bool get(const int index, T &item) const
			{
				if (index >= 0 && index <= top)
				{
					item = data[index];
					return true;
				}
				else
				{
					errno = EFAULT;
					return false;
				}
			}
			/**
			 * @brief 将元素压入栈
			 * @param item 要压入的元素
			 * @return 是否压入成功
			 */
			inline bool push(const T &item)
			{
				if (top < size - 1)
				{
					top++;
					data[top] = item;
					return true;
				}
				else
				{
					errno = EFAULT;
					return false;
				}
			}
			/**
			 * @brief 将元素弹出栈
			 * @param item 储存弹出的元素
			 * @return 是否弹出成功
			 */
			inline bool pop(T &item)
			{
				if (top >= 0)
				{
					item = data[top];
					top--;
					return true;
				}
				else
				{
					errno = EFAULT;
					return false;
				}
			}

			/**
			 * @brief 将栈复制到
			 * @param stack 目标栈
			 * @return 是否复制成功
			 */
			inline bool copyto(class FixedCapacityStack<T> &stack) const
			{
				if (isready)
					if (stack.setsize(size))
					{
						for (int i = 0; i <= top; i++)
							if (!stack.push(data[i]))
								return false;
						return true;
					}
				return false;
			}
			/**
			 * @brief 从指定栈复制
			 * @param stack 指定栈
			 * @return 是否复制成功
			 */
			inline bool copyfrom(class FixedCapacityStack<T> &stack)
			{
				if (stack.isready)
					if (setsize(size))
					{
						for (int i = 0; i <= stack.top; i++)
							if (!push(stack.data[i]))
								return false;
						return true;
					}
				return false;
			}

			/**
			 * @brief 获取指定元素
			 * @param index 指定元素在栈中的位置
			 * @return 指定元素
			 */
			inline T &operator[](const int index) const
			{
				if (index >= 0 && index <= top)
					return data[index];
				else
				{
					errno = EFAULT;
					return data[size - 1];
				}
			}
			/**
			 * @brief 将元素压入栈
			 * @param item 要压入的元素
			 */
			inline class FixedCapacityStack<T> &operator<<(const T &item)
			{
				push(item);
				return *this;
			}
			/**
			 * @brief 将元素弹出栈
			 * @param item 储存弹出的元素
			 */
			inline class FixedCapacityStack<T> &
			operator>>(T &item)
			{
				pop(item);
				return *this;
			}
		};
	}

}

#endif

#endif