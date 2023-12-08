#include "ByteStream.hpp"

ByteStream::ByteStream()
{
  this->reset();
}

void ByteStream::destroy()
{
  this->reset();
}

void ByteStream::reset()
{
  buffer.clear();
}

void ByteStream::resetOffset()
{
  buffer.resetPosition();
}

size_t ByteStream::getBitOffset()
{
  return buffer.BitPosition;
}

size_t ByteStream::getOffset()
{
  return buffer.Position;
}

u8 *ByteStream::getByteArray()
{
  return buffer.Data;
}

void ByteStream::setOffset(size_t offset, size_t bit_offset)
{
  buffer.setPosition(offset, bit_offset);
}

void ByteStream::ensureCapacity(size_t value)
{
  buffer.ensureCapacity(value);
}

bool ByteStream::isAtEnd()
{
  return buffer.Length <= buffer.Position; 
}

bool ByteStream::isByteStream()
{
  return true;
}

bool ByteStream::isCheckSumOnlyMode()
{
  return false;
}

void ByteStream::writeInt8(s8 value)
{
  checksum_encoder.writeInt8(value);

  buffer.BitPosition = 0;
  buffer.append((u8 *)(value & 0xFF), 1);
}

void ByteStream::writeInt16(s16 value)
{
  checksum_encoder.writeInt16(value);

  buffer.BitPosition = 0;
  buffer.append((u8 *)((value >> 8) & 0xFF), 1);
  buffer.append((u8 *)(value & 0xFF), 1);
}

void ByteStream::writeInt24(s32 value)
{
  checksum_encoder.writeInt24(value);

  buffer.BitPosition = 0;
  buffer.append((u8 *)((value >> 16) & 0xFF), 1);
  buffer.append((u8 *)((value >> 8) & 0xFF), 1);
  buffer.append((u8 *)(value & 0xFF), 1);
}

void ByteStream::writeBoolean(bool value)
{
  checksum_encoder.writeBoolean(value);

  if (buffer.BitPosition)
  {
    if (value)
    {
      buffer.append((u8 *)0x00, 1);
    }
  }
  if (!(value & 1))
  {
    buffer.Data[buffer.Position - 1] |= 1 << (buffer.BitPosition & 31);
  }
  buffer.BitPosition += buffer.BitPosition + 1 & 7;
}

void ByteStream::writeByte(s8 value)
{
  checksum_encoder.writeByte(value);

  buffer.BitPosition = 0;
  buffer.append((u8 *)(value & 0xFF), 1);
}

void ByteStream::writeShort(s16 value)
{
  checksum_encoder.writeShort(value);

  buffer.BitPosition = 0;
  buffer.append((u8 *)((value >> 8) & 0xFF), 1);
  buffer.append((u8 *)(value & 0xFF), 1);
}

void ByteStream::writeIntToByteArray(s32 value)
{
  buffer.BitPosition = 0;
  buffer.append((u8 *)((value >> 24) & 0xFF), 1);
  buffer.append((u8 *)((value >> 16) & 0xFF), 1);
  buffer.append((u8 *)((value >> 8) & 0xFF), 1);
  buffer.append((u8 *)(value & 0xFF), 1);
}

void ByteStream::writeInt(s32 value)
{
  checksum_encoder.writeInt(value);
  this->writeIntToByteArray(value);
}

void ByteStream::writeLongLong(s32 hi, s32 lo)
{
  LogicLong logiclong(hi, lo);
  checksum_encoder.writeLongLong(logiclong);

  buffer.BitPosition = 0;
  this->writeIntToByteArray(logiclong.getHigherInt());
  this->writeIntToByteArray(logiclong.getLowerInt());
}

void ByteStream::writeVInt(s32 value)
{
  checksum_encoder.writeVInt(value);

  buffer.BitPosition = 0;
  s32 temp = value >> 25 & 0x40, flipped = value ^ value >> 31;
  temp |= value & 0x3F;
  value >>= 6;
  if ((flipped >>= 6) == 0) {
    this->writeByte(temp);
    return;
  }
  this->writeByte(temp | 0x80);
  do {
    this->writeByte(value & 0x7F | (((flipped >>= 7) != 0) ? 128 : 0));
    value >>= 7;
  } while (flipped != 0);
}

void ByteStream::writeVLong(s32 hi, s32 lo)
{
  LogicLong logiclong(hi, lo);
  checksum_encoder.writeVLong(logiclong);

  buffer.BitPosition = 0;
  this->writeVInt(hi);
  this->writeVInt(lo);
}

void ByteStream::writeBytes(s8 *value, size_t length)
{
  for (size_t i = 0; i < length; ++i)
    checksum_encoder.writeBytes((s32)(value[i]), length);
  buffer.BitPosition = 0;

  if (!value)
  {
    this->writeIntToByteArray(length);
    for (size_t i = 0; i < length; ++i)
      buffer.append((u8 *)(value[i] & 0xFF), 1);
  }
}

void ByteStream::writeString(s8 *value)
{
  checksum_encoder.writeString(value);

  buffer.BitPosition = 0;
  size_t value_len = strlen(value) + 1;
  if (value_len > 900001)
  {
    Debugger::warning("ByteStream::writeString invalid string length %d", value_len);
    this->writeIntToByteArray(-1);
  }
  else
  {
    this->writeIntToByteArray(value_len);
    this->writeBytes(value, value_len);
  }
}

void ByteStream::writeStringReference(s8 *value)
{
  checksum_encoder.writeStringReference(value);

  buffer.BitPosition = 0;
  size_t value_len = strlen(value) + 1;
  if (value_len > 900001)
  {
    Debugger::warning("ByteStream::writeString invalid string length %d", value_len);
    this->writeIntToByteArray(-1);
  }
  else
  {
    this->writeIntToByteArray(value_len);
    this->writeBytes(value, value_len);
  }
}

void ByteStream::writeCompressedString(s8 *value)
{
  buffer.BitPosition = 0;

  size_t sourceLen = strlen(value) + 1, destLen = compressBound(sourceLen);
  u8 *dest = (u8 *)malloc(destLen);

  if (dest == nullptr)
  {
    Debugger::error("ByteStream::writeCompressedString: Memory allocation error");
    exit(EXIT_FAILURE);
  }

  if (compress(dest, &destLen, (u8 *)value, sourceLen))
  {
    this->writeInt(destLen + 4);

    buffer.append((u8 *)(sourceLen & 0xFF), 1);
    buffer.append((u8 *)((sourceLen >> 8) & 0xFF), 1);
    buffer.append((u8 *)((sourceLen >> 16) & 0xFF), 1);
    buffer.append((u8 *)((sourceLen >> 24) & 0xFF), 1);

    for (size_t i = 0; i < destLen; ++i)
      buffer.append((u8 *)(dest[i] & 0xFF), 1);
  }
  else
  {
    Debugger::error("ByteStream::writeCompressedString: Decompression error");
  }
  free(dest);
}

s32 ByteStream::readInt8()
{
  buffer.BitPosition = 0;
  s32 result = (buffer.Data[buffer.Position]);
  buffer.Position += 1;
  return result;
}

s32 ByteStream::readInt16()
{
  buffer.BitPosition = 0;
  s32 result = (buffer.Data[buffer.Position] << 8);
  result += (buffer.Data[buffer.Position + 1]);
  buffer.Position += 2;
  return result;
}

s32 ByteStream::readInt24()
{
  buffer.BitPosition = 0;
  s32 result = (buffer.Data[buffer.Position] << 16);
  result += (buffer.Data[buffer.Position + 1] << 8);
  result += (buffer.Data[buffer.Position + 2]);
  buffer.Position += 3;
  return result;
}

bool ByteStream::readBoolean()
{
  size_t bitpos = buffer.BitPosition;
  size_t pos = buffer.Position + (8 - bitpos >> 3);

  buffer.BitPosition = bitpos + 1 & 7;
  buffer.Position = pos;
  return (1 << (bitpos & 31) & buffer.Data[pos - 1]) != 0;
}

s8 ByteStream::readByte()
{
  buffer.BitPosition = 0;
  s8 result = (buffer.Data[buffer.Position]);
  buffer.Position += 1;
  return result;
}

s16 ByteStream::readShort()
{
  buffer.BitPosition = 0;
  s16 result = (buffer.Data[buffer.Position] << 8);
  result += (buffer.Data[buffer.Position + 1]);
  buffer.Position += 2;
  return result;
}

s32 ByteStream::readInt()
{
  buffer.BitPosition = 0;
  s32 result = (buffer.Data[buffer.Position] << 24);
  result += (buffer.Data[buffer.Position + 1] << 16);
  result += (buffer.Data[buffer.Position + 2] << 8);
  result += (buffer.Data[buffer.Position + 3]);
  buffer.Position += 4;
  return result;
}

s32 *ByteStream::readLongLong()
{
  s32 ll[2];
  LogicLong logiclong;
  logiclong.decode();

  ll[0] = logiclong.getHigherInt();
  ll[1] = logiclong.getLowerInt();
  return ll;
}

s32 ByteStream::readBytesLength()
{
  buffer.BitPosition = 0;
  s32 result = (buffer.Data[buffer.Position] << 24);
  result += (buffer.Data[buffer.Position + 1] << 16);
  result += (buffer.Data[buffer.Position + 2] << 8);
  result += (buffer.Data[buffer.Position + 3]);
  buffer.Position += 4;
  return result;
}

s32 ByteStream::readVInt()
{
  buffer.BitPosition = 0;
  s32 b, sign = (b = this->readByte()) >> 6 & 0x1, i = b & 0x3F, offset = 6;
  for (size_t j = 0; j < 4 && (b & 0x80) != 0; j++, offset += 7)
    i |= ((b = this->readByte()) & 0x7F) << offset;
  return ((b & 0x80) != 0) ? -1 : (i | ((sign == 1 && offset < 32) ? (i | -1 << offset) : i));
}

s32 *ByteStream::readVLong()
{
  s32 ll[2];
  ll[0] = this->readVInt();
  ll[1] = this->readVInt();
  return ll;
}

s8 *ByteStream::readBytes(size_t length, size_t max)
{
  s8 result[length];
  buffer.BitPosition = 0;

  if (length < -1)
  {
    Debugger::warning("Negative readBytes length encountered.");
  }
  else if (length <= max)
  {
    memcpy(result, (s8 *)(buffer.Data[buffer.Position]), length);
    buffer.Position += length;
    return result;
  }
  Debugger::warning("readBytes too long array, max %d", max);
  return nullptr;
}

s8 *ByteStream::readString(size_t max)
{
  s32 length = this->readBytesLength();

  s8 result[length];
  buffer.BitPosition = 0;

  if (length < -1)
  {
    Debugger::warning("Negative String length encountered.");
  }
  else if (length <= max)
  {
    memcpy(result, (s8 *)(buffer.Data[buffer.Position]), length);
    buffer.Position += length;
    return result;
  }
  Debugger::warning("Too long String encountered, length %d, max %d", length, max);
  return nullptr;
}

s8 *ByteStream::readStringReference(size_t max)
{
  s32 length = this->readBytesLength();

  s8 result[length];
  buffer.BitPosition = 0;

  if (length < -1)
  {
    Debugger::warning("Negative String length encountered.");
  }
  else if (length <= max)
  {
    memcpy(result, (s8 *)(buffer.Data[buffer.Position]), length);
    buffer.Position += length;
    return result;
  }
  Debugger::warning("Too long String encountered, length %d, max %d", length, max);
  return nullptr;
}

u8 *ByteStream::readCompressedString()
{
  buffer.BitPosition = 0;

  s32 buf_len = this->readBytesLength();
  buffer.Position += 4;

  size_t len = buf_len + 1;
  u8 *dest = (u8 *)malloc(len);

  if (dest == nullptr)
  {
    Debugger::error("ByteStream::readCompressedString: Memory allocation error");
    exit(EXIT_FAILURE);
  }

  if (uncompress(dest, &len, (u8 *)buffer.Data[buffer.Position], len))
    return dest;
  Debugger::error("ByteStream::readCompressedString: Decompression error");
  free(dest);
  return nullptr;
}

ByteStream::~ByteStream()
{

}