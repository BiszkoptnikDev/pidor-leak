#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <cstdint>
#include <cstring>

#include "Defs.hpp"

class Buffer
{
public:
  Buffer(size_t len = 0);

  u8 *Data;
  u8 *data();

  size_t Length, Position, BitPosition;

  size_t length();
  size_t position();
  size_t bit_pos();

  void resetPosition();
  void clear();

  void setPosition(size_t pos, size_t bit_pos);
  void setBuf(u8 *buf, size_t buf_len);

  void ensureCapacity(size_t new_len);
  void append(u8 *value, size_t value_len);

  ~Buffer();
};

#endif // !BUFFER_HPP