#ifndef DEFS_HPP
#define DEFS_HPP

#include <string>
#include <cstddef>
#include <cstdint>
#include <cstdbool>

using namespace std;

using f64     =  double;
using f32     =  float;

using u64     =  unsigned long long;
using u32     =  unsigned int;
using u16     =  unsigned short;
using u8      =  unsigned char;

using s64     =  long int;
using s32     =  int;
using s16     =  short;
using s8      =  char;

using boolean =  bool;

using str     =  string;
using uptr    =  uintptr_t;

using F64     =  f64;
using F32     =  f32;

using U64     =  u64;
using U32     =  u32;
using U16     =  u16;
using  U8     =   u8;

using S64     =  s64;
using S32     =  s32;
using S16     =  s16;
using  S8     =   s8;

using BOOL = boolean;

using  STR    =  str;
using UPTR    = uptr;

using _QWORD  =  U64;
using _DWORD  =  U32;
using _WORD   =  U16;
using _BYTE   =   U8;
using _BOOL4  = BOOL;

#define LAST_IND(x,part_type)    (sizeof(x)/sizeof(part_type) - 1)
#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN
#  define LOW_IND(x,part_type)   LAST_IND(x,part_type)
#  define HIGH_IND(x,part_type)  0
#else
#  define HIGH_IND(x,part_type)  LAST_IND(x,part_type)
#  define LOW_IND(x,part_type)   0
#endif

#define BYTEn(x, n)   (*((_BYTE*)&(x)+n))
#define WORDn(x, n)   (*((_WORD*)&(x)+n))
#define DWORDn(x, n)  (*((_DWORD*)&(x)+n))

#define LOBYTE(x)  BYTEn(x,LOW_IND(x,_BYTE))
#define LOWORD(x)  WORDn(x,LOW_IND(x,_WORD))
#define LODWORD(x) DWORDn(x,LOW_IND(x,_DWORD))
#define HIBYTE(x)  BYTEn(x,HIGH_IND(x,_BYTE))
#define HIWORD(x)  WORDn(x,HIGH_IND(x,_WORD))
#define HIDWORD(x) DWORDn(x,HIGH_IND(x,_DWORD))
#define BYTE1(x)   BYTEn(x,  1)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
#define BYTE4(x)   BYTEn(x,  4)
#define BYTE5(x)   BYTEn(x,  5)
#define BYTE6(x)   BYTEn(x,  6)
#define BYTE7(x)   BYTEn(x,  7)
#define BYTE8(x)   BYTEn(x,  8)
#define BYTE9(x)   BYTEn(x,  9)
#define BYTE10(x)  BYTEn(x, 10)
#define BYTE11(x)  BYTEn(x, 11)
#define BYTE12(x)  BYTEn(x, 12)
#define BYTE13(x)  BYTEn(x, 13)
#define BYTE14(x)  BYTEn(x, 14)
#define BYTE15(x)  BYTEn(x, 15)
#define WORD1(x)   WORDn(x,  1)
#define WORD2(x)   WORDn(x,  2)
#define WORD3(x)   WORDn(x,  3)
#define WORD4(x)   WORDn(x,  4)
#define WORD5(x)   WORDn(x,  5)
#define WORD6(x)   WORDn(x,  6)
#define WORD7(x)   WORDn(x,  7)

#define SBYTEn(x, n)   (*((s8*)&(x)+n))
#define SWORDn(x, n)   (*((s16*)&(x)+n))
#define SDWORDn(x, n)  (*((s32*)&(x)+n))

#define SLOBYTE(x)  SBYTEn(x,LOW_IND(x,s8))
#define SLOWORD(x)  SWORDn(x,LOW_IND(x,s16))
#define SLODWORD(x) SDWORDn(x,LOW_IND(x,s32))
#define SHIBYTE(x)  SBYTEn(x,HIGH_IND(x,s8))
#define SHIWORD(x)  SWORDn(x,HIGH_IND(x,s16))
#define SHIDWORD(x) SDWORDn(x,HIGH_IND(x,s32))
#define SBYTE1(x)   SBYTEn(x,  1)
#define SBYTE2(x)   SBYTEn(x,  2)
#define SBYTE3(x)   SBYTEn(x,  3)
#define SBYTE4(x)   SBYTEn(x,  4)
#define SBYTE5(x)   SBYTEn(x,  5)
#define SBYTE6(x)   SBYTEn(x,  6)
#define SBYTE7(x)   SBYTEn(x,  7)
#define SBYTE8(x)   SBYTEn(x,  8)
#define SBYTE9(x)   SBYTEn(x,  9)
#define SBYTE10(x)  SBYTEn(x, 10)
#define SBYTE11(x)  SBYTEn(x, 11)
#define SBYTE12(x)  SBYTEn(x, 12)
#define SBYTE13(x)  SBYTEn(x, 13)
#define SBYTE14(x)  SBYTEn(x, 14)
#define SBYTE15(x)  SBYTEn(x, 15)
#define SWORD1(x)   SWORDn(x,  1)
#define SWORD2(x)   SWORDn(x,  2)
#define SWORD3(x)   SWORDn(x,  3)
#define SWORD4(x)   SWORDn(x,  4)
#define SWORD5(x)   SWORDn(x,  5)
#define SWORD6(x)   SWORDn(x,  6)
#define SWORD7(x)   SWORDn(x,  7)

template<class T> s16 __PAIR__(s8  high, T low) { return (((s16)high) << sizeof(high)*8) | uint8(low); }
template<class T> s32 __PAIR__(s16 high, T low) { return (((s32)high) << sizeof(high)*8) | uint16(low); }
template<class T> s64 __PAIR__(s32 high, T low) { return (((s64)high) << sizeof(high)*8) | uint32(low); }
template<class T> u16 __PAIR__( u8 high, T low) { return (((u16)high) << sizeof(high)*8) | uint8(low); }
template<class T> u32 __PAIR__(u16 high, T low) { return (((u32)high) << sizeof(high)*8) | uint16(low); }
template<class T> u64 __PAIR__(u32 high, T low) { return (((u64)high) << sizeof(high)*8) | uint32(low); }

template<class T> T __ROL__(T value, s32 count)
{
  const u32 nbits = sizeof(T) * 8;

  if (count > 0)
  {
    count %= nbits;
    T high = value >> (nbits - count);
    if (T(-1) < 0)
      high &= ~((T(-1) << count));
    value <<= count;
    value |= high;
  }
  else
  {
    count = -count % nbits;
    T low = value << (nbits - count);
    value >>= count;
    value |= low;
  }
  return value;
}

inline  u8 __ROL1__( u8 value, s32 count) { return __ROL__(( u8)value,  count); }
inline u16 __ROL2__(u16 value, s32 count) { return __ROL__((u16)value,  count); }
inline u32 __ROL4__(u32 value, s32 count) { return __ROL__((u32)value,  count); }
inline u64 __ROL8__(u64 value, s32 count) { return __ROL__((u64)value,  count); }
inline  u8 __ROR1__( u8 value, s32 count) { return __ROL__(( u8)value, -count); }
inline u16 __ROR2__(u16 value, s32 count) { return __ROL__((u16)value, -count); }
inline u32 __ROR4__(u32 value, s32 count) { return __ROL__((u32)value, -count); }
inline u64 __ROR8__(u64 value, s32 count) { return __ROL__((u64)value, -count); }

#endif // !DEFS_HPP