#ifndef LOGICLONG_HPP
#define LOGICLONG_HPP

#include "Defs.hpp"
#include "ByteStream.hpp"

class LogicLong
{
public:
  LogicLong(s32 hi = 0, s32 lo = 0);
  s32 getLong();
  s32 getHigherInt();
  s32 getLowerInt();

  s32 hashCode();

  s32 toLong();
  void toString();

  void set(s32 hi, s32 lo);
  void clone(s32 hi, s32 lo);

  bool greaterThan();
  bool equals();
  bool isZero();

  void decode();
  void encode();

  ~LogicLong();
private:
  typedef struct
  {
    s32 hi, lo;
  } ll;
  ll logiclong;
};

ByteStream stream;

#endif // !LOGICLONG_HPP