#include "ChecksumEncoder.hpp"

ChecksumEncoder::ChecksumEncoder()
{
  this->checksum = 0;
  this->checksum2 = 0;
  this->isEnabled = true;
}

s32 ChecksumEncoder::getCheckSum()
{
  this->checksum = (!this->isEnabled) ? this->checksum2 : this->checksum;
  return this->checksum;
}

bool ChecksumEncoder::isByteStream()
{
  return false;
}

bool ChecksumEncoder::isCheckSumEnabled()
{
  return this->isEnabled;
}

bool ChecksumEncoder::isCheckSumOnlyMode()
{
  return true;
}

bool ChecksumEncoder::equals(ChecksumEncoder *checksum_instance)
{
  u32 checksum_instance_compare = checksum_instance->checksum, checksum_compare = this->checksum;
  if (!checksum_instance)
  {
    return false;
  }
  if (!checksum_instance->isEnabled)
  {
    checksum_instance_compare = checksum_instance->checksum2;
  }
  if (!this->isEnabled)
  {
    checksum_compare = this->checksum2;
  }
  return checksum_compare == checksum_instance_compare;
}

s32 ChecksumEncoder::hashCode()
{
  Debugger::error("ChecksumEncoder hashCode not designed");
  return 42;
}

void ChecksumEncoder::enableCheckSum(bool state)
{
  if (!this->isEnabled || state)
  {
    if (!this->isEnabled && state)
    {
      this->checksum = this->checksum2;
    }
    this->isEnabled = state;
  }
  else
  {
    this->checksum2 = this->checksum;
    this->isEnabled = false;
  }
}

void ChecksumEncoder::resetCheckSum()
{
  this->checksum = 0;
}

void ChecksumEncoder::writeInt8(s32 value)
{
  if ((u32)(value + 0x80) >= 0x100)
    Debugger::error("ChecksumEncoder::writeInt8: value too big.");
  this->checksum = __ROR4__(this->checksum, 31) + (u8)(value) + 11;
}

void ChecksumEncoder::writeInt16(s32 value)
{
  if ((u32)(value + 0x8000) >= 0x10000)
    Debugger::error("ChecksumEncoder::writeInt16: value too big.");
  this->checksum = __ROR4__(this->checksum, 31) + (u16)(value) + 19;
}

void ChecksumEncoder::writeInt24(s32 value)
{
  if ((u32)(value + 0x800000) >= 0x1000000)
    Debugger::error("ChecksumEncoder::writeInt24: value too big.");
  this->checksum = __ROR4__(this->checksum, 31) + (u32)(value & 0xFFFFFF) + 21;
}

void ChecksumEncoder::writeBoolean(bool value)
{
  this->checksum = __ROR4__(this->checksum, 31) + (!value) ? 7 : 13;
}

void ChecksumEncoder::writeByte(u8 value)
{
  this->checksum = __ROR4__(this->checksum, 31) + value + 11;
}

void ChecksumEncoder::writeShort(u16 value)
{
  this->checksum = __ROR4__(this->checksum, 31) + value + 19;
}

void ChecksumEncoder::writeInt(s32 value)
{
  this->checksum = __ROR4__(this->checksum, 31) + value + 9;
}

void ChecksumEncoder::writeLong(LogicLong logiclong)
{
  logiclong.encode();
}

void ChecksumEncoder::writeLongLong(LogicLong logiclong)
{
  this->checksum = __ROR4__(logiclong.getHigherInt() + __ROR4__(this->checksum, 31) + 67, 31) + logiclong.getLowerInt() + 91;
}

void ChecksumEncoder::writeVInt(s32 value)
{
  this->checksum = __ROR4__(this->checksum, 31) + value + 33;
}

void ChecksumEncoder::writeVLong(LogicLong logiclong)
{
  this->checksum = __ROR4__(logiclong.getHigherInt() + __ROR4__(this->checksum, 31) + 65, 31) + logiclong.getLowerInt() + 88;
}

void ChecksumEncoder::writeBytes(s32 value, size_t len)
{
  this->checksum = __ROR4__(this->checksum, 31) + (!value) ? 37 : len + 38;
}

void ChecksumEncoder::writeString(s8 *value)
{
  this->checksum =  __ROR4__(this->checksum, 31) + (!value) ? 27 : strlen(value) + 28;
}

void ChecksumEncoder::writeStringReference(s8 *value)
{
  this->checksum = __ROR4__(this->checksum, 31) + strlen(value) + 28;
}

ChecksumEncoder::~ChecksumEncoder()
{
  this->checksum = 0;
  this->checksum2 = 0;
  this->isEnabled = true;
}