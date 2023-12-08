#ifndef CHECKSUMENCODER_HPP
#define CHECKSUMENCODER_HPP

#include <cstring>

#include "Defs.hpp"
#include "Debugger.hpp"
#include "LogicLong.hpp"

class ChecksumEncoder
{
public:
  ChecksumEncoder();

  s32 getCheckSum();

  bool isByteStream();
  bool isCheckSumEnabled();
  bool isCheckSumOnlyMode();
  bool equals(ChecksumEncoder *checksum_instance);

  s32 hashCode();

  void enableCheckSum(bool state);
  void resetCheckSum();

  void writeInt8(s32 value);
  void writeInt16(s32 value);
  void writeInt24(s32 value);
  void writeBoolean(bool value);
  void writeByte(u8 value);
  void writeShort(u16 value);
  void writeInt(s32 value);
  void writeLong(LogicLong logiclong);
  void writeLongLong(LogicLong logiclong);
  void writeVInt(s32 value);
  void writeVLong(LogicLong logiclong);
  void writeBytes(s32 value, size_t len);
  void writeString(s8 *value);
  void writeStringReference(s8 *value);

  ~ChecksumEncoder();
private:
  static u32 checksum;
  static u32 checksum2;
  static bool isEnabled;
};

#endif // !CHECKSUMENCODER_HPP