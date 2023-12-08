#include "Buffer.hpp"

Buffer::Buffer(size_t len)
{
  this->Length = (len) ? len : 1;
  this->Data = (u8 *)malloc(this->Length);
  this->Position = 0;
  this->BitPosition = 0;
}

void Buffer::resetPosition()
{
  this->Position = 0;
  this->BitPosition = 0;
}

void Buffer::clear()
{
  free(this->Data);
  this->Data = nullptr;
  this->Length = 0;
  this->Position = 0;
  this->BitPosition = 0;
}

void Buffer::setPosition(size_t pos, size_t bit_pos)
{
  this->Position = pos;
  this->BitPosition = bit_pos;
}

void Buffer::setBuf(u8 *buf, size_t buf_len)
{
  free(this->Data);
  this->Data = buf;
  this->Length = buf_len;
  this->Position = 0;
  this->BitPosition = 0;
}

void Buffer::ensureCapacity(size_t new_len)
{
  u8 *newData = (u8 *)realloc(this->Data, this->Length + new_len);
  this->Data = (newData != nullptr) ? newData : this->Data;
  this->Length += new_len;
}

void Buffer::append(u8 *value, size_t value_len)
{
  this->ensureCapacity(value_len);
  memcpy(this->Data, value, value_len);
  this->Position += value_len;
}

u8 *Buffer::data()
{
  return this->Data;
}

size_t Buffer::length()
{
  return this->Length;
}

size_t Buffer::position()
{
  return this->Position;
}

size_t Buffer::bit_pos()
{
  return this->BitPosition;
}

Buffer::~Buffer()
{
  if (this->Data != nullptr)
  {
    free(this->Data);
    this->Data = nullptr;
  }
  this->Length = 0;
  this->Position = 0;
  this->BitPosition = 0;
}