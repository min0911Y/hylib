#pragma once

#ifndef _hylib_basic_defines_
#define _hylib_basic_defines_

//整数
#define _hylib_int8_ signed char
#define _hylib_uint8_ unsigned char
#define _hylib_int16_ signed short
#define _hylib_uint16_ unsigned short
#define _hylib_int32_ signed int
#define _hylib_uint32_ unsigned int
#define _hylib_int64_ signed long long
#define _hylib_uint64_ unsigned long long
#define _hylib_volatile_int8_ volatile _hylib_int8_
#define _hylib_volatile_uint8_ volatile _hylib_uint8_
#define _hylib_volatile_int16_ volatile _hylib_int16_
#define _hylib_volatile_uint16_ volatile _hylib_uint16_
#define _hylib_volatile_int32_ volatile _hylib_int32_
#define _hylib_volatile_uint32_ volatile _hylib_uint32_
#define _hylib_volatile_int64_ volatile _hylib_int64_
#define _hylib_volatile_uint64_ volatile _hylib_uint64_

#define _hylib_sbyte_ _hylib_int8_
#define _hylib_byte_ _hylib_uint8_
#define _hylib_short_ _hylib_int16_
#define _hylib_ushort_ _hylib_uint16_
#define _hylib_int_ _hylib_int32_
#define _hylib_uint_ _hylib_uint32_
#define _hylib_long_ _hylib_int64_
#define _hylib_ulong_ _hylib_uint64_
#define _hylib_volatile_sbyte_ volatile _hylib_sbyte_
#define _hylib_volatile_byte_ volatile _hylib_byte_
#define _hylib_volatile_short_ volatile _hylib_short_
#define _hylib_volatile_ushort_ volatile _hylib_ushort_
#define _hylib_volatile_int_ volatile _hylib_int_
#define _hylib_volatile_uint_ volatile _hylib_uint_
#define _hylib_volatile_long_ volatile _hylib_long_
#define _hylib_volatile_ulong_ volatile _hylib_ulong_
//浮点
#define _hylib_float32_ float
#define _hylib_float64_ double
#define _hylib_volatile_float32_ volatile _hylib_float32_
#define _hylib_volatile_float64_ volatile _hylib_float64_

#define _hylib_float_ _hylib_float32_
#define _hylib_double_ _hylib_float64_
#define _hylib_volatile_float_ volatile _hylib_float_
#define _hylib_volatile_double_ volatile _hylib_double_s
//其它
#ifdef __cplusplus
#define _hylib_bool_ bool
#endif
#ifndef __cplusplus
#define _hylib_bool_ _hylib_sbyte_
#endif
#define _hylib_void_ void
#define _hylib_ptr_ (_hylib_byte_ *)
#define _hylib_volatile_ptr_ (_hylib_volatile_byte_ *)
#define _hylib_fd_ _hylib_int32_

//

#ifdef __cplusplus
#define _hylib_true_ true
#define _hylib_false_ false
#endif
#ifndef __cplusplus
#define _hylib_true_ ((_hylib_bool_)1)
#define _hylib_false_ ((_hylib_bool_)0)
#endif

#ifdef __cplusplus
namespace hylib
{
#endif

#define int8 _hylib_int8_
#define uint8 _hylib_uint8_
#define int16 _hylib_int16_
#define uint16 _hylib_uint16_
#define int32 _hylib_int32_
#define uint32 _hylib_uint32_
#define int64 _hylib_int64_
#define uint64 _hylib_uint64_

#define sbyte _hylib_sbyte_
#define byte _hylib_byte_
#define ushort _hylib_ushort_
#define uint _hylib_uint_
#define Long _hylib_long_
#define ulong _hylib_ulong_

#define float32 _hylib_float32_
#define float64 _hylib_float64_

#define Ptr _hylib_ptr_
#define FD _hylib_fd_

#ifndef __cplusplus
#define bool _hylib_bool_
#define true _hylib_true_
#define false _hylib_false_
#endif

#ifdef __cplusplus
}
#endif

#endif