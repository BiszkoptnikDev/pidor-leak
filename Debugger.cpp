#include "Debugger.hpp"

s32 Debugger::warningCount = 0, Debugger::errorCount = 0;

void Debugger::print(const s8 *format, ...)
{
  va_list va;
  va_start(va, format);

  vprintf(format, va);
  printf("\n");

  va_end(va);
}

void Debugger::warning(const s8 *format, ...)
{
  va_list va;
  va_start(va, format);

  ++warningCount;
  printf("[warning] ");
  vprintf(format, va);
  printf("\n");

  va_end(va);
}

void Debugger::error(const s8 *format, ...)
{
  va_list va;
  va_start(va, format);

  ++errorCount;
  printf("[ERROR] ");
  vprintf(format, va);
  printf("\n");

  va_end(va);
}