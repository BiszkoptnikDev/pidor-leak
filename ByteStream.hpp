#ifndef BYTESTREAM_HPP
#define BYTESTREAM_HPP

#include <zlib.h>

#include "Buffer.hpp"
#include "ChecksumEncoder.hpp"

class ByteStream {
public:
  ByteStream();

  void destroy();

  void reset();
  void resetOffset();
  size_t getOffset();
  void setOffset(size_t offset, size_t bit_offset);
  size_t getBitOffset();

  u8 *getByteArray();

  void ensureCapacity(size_t value);

  bool isAtEnd();
  bool isByteStream();
  bool isCheckSumOnlyMode();

  void writeInt8(s8 value);
  void writeInt16(s16 value);
  void writeInt24(s32 value);

  void writeBoolean(bool value);
  void writeByte(s8 value);
  void writeShort(s16 value);
  void writeInt(s32 value);
  void writeIntToByteArray(s32 value);
  void writeLongLong(s32 hi, s32 lo);
  void writeVInt(s32 value);
  void writeVLong(s32 hi, s32 lo);
  void writeBytes(s8 *value, size_t length);
  void writeString(s8 *value);
  void writeStringReference(s8 *value);
  void writeCompressedString(s8 *value);

  s32 readInt8();
  s32 readInt16();
  s32 readInt24();

  bool readBoolean();
  s8 readByte();
  s16 readShort();
  s32 readInt();
  s32 *readLongLong();
  s32 readBytesLength();
  s32 readVInt();
  s32 *readVLong();

  s8 *readBytes(size_t length, size_t max = 0x3E8);
  s8 *readString(size_t max = 0xDBBA0);
  s8 *readStringReference(size_t max = 0xDBBA0);
  u8 *readCompressedString();

  ~ByteStream();
private:
  Buffer buffer;
  ChecksumEncoder checksum_encoder;
};

#endif // !BYTESTREAM_HPP