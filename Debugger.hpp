#ifndef DEBUGGER_HPP
#define DEBUGGER_HPP

#include <cstdio>
#include <cstdarg>

#include "Defs.hpp"

namespace Debugger
{
  static void print(const s8 *format, ...);
  static void warning(const s8 *format, ...);
  static void error(const s8 *format, ...);

  static s32 warningCount, errorCount;
}

#endif // !DEBUGGER_HPP