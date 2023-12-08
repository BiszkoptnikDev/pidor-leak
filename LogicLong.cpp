#include "LogicLong.hpp"

LogicLong::LogicLong(s32 hi, s32 lo)
{
  if ((hi && lo) || (hi && !lo))
    this->set(0, 0);
  else if (!hi || lo)
    this->set(hi, 0);
  else
    this->set(hi, lo);
}

s32 LogicLong::getLong()
{
  return (logiclong.lo >> 31 == -1) ? logiclong.lo | 0x80000000 : logiclong.lo;
}

s32 LogicLong::getHigherInt()
{
  return logiclong.hi;
}

s32 LogicLong::getLowerInt()
{
  return logiclong.lo;
}

s32 LogicLong::hashCode()
{
  return 31 * logiclong.hi + logiclong.lo;
}

s32 LogicLong::toLong()
{
  s32 lo = logiclong.lo & 0x7FFFFFFF; 
  if (logiclong.lo < 0)
    lo = logiclong.lo | 0x80000000;
  return lo | logiclong.hi << 32;
}

void LogicLong::toString()
{
  Debugger::print("LogicLong(%d,%d)", logiclong.hi, logiclong.lo);
}

bool LogicLong::greaterThan()
{
  bool result = false;
  if (logiclong.lo)
  {
    result = true;
    if (logiclong.hi <= logiclong.lo)
    {
      result = false;
      if (logiclong.hi == logiclong.lo)
        return logiclong.hi > logiclong.lo;
    }
  }
  return result;
}

bool LogicLong::equals()
{
  return (logiclong.lo) ? logiclong.hi == logiclong.lo : false;
}

bool LogicLong::isZero()
{
  return (!logiclong.hi) ? logiclong.hi == 0 : false;
}

void LogicLong::set(s32 hi, s32 lo)
{
  s32 l = lo & 0x7FFFFFFF; 
  if (lo < 0)
    l = lo | 0x80000000;
  logiclong.hi = hi >> 32;
  logiclong.lo = l;
}

void LogicLong::clone(s32 hi, s32 lo)
{
  logiclong.hi = hi;
  logiclong.lo = lo;
}

void LogicLong::decode()
{
  logiclong.hi = stream.readInt();
  logiclong.lo = stream.readInt();
}

void LogicLong::encode()
{
  stream.writeInt(logiclong.hi);
  stream.writeInt(logiclong.lo);
}

LogicLong::~LogicLong()
{
  this->set(0, 0);
}