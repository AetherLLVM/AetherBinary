// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-or-later */
// See LICENSE file in the root directory for full license text.

#include "AetherBinary.h"

#include <format>

namespace aether {

const char *Insinfo::typeString() const {
  switch (info.type) {
  case NORMAL:
    return "INSN";
  case JUMP:
    return "JUMP";
  case JCOND:
    return "JCON";
  case CALL:
    return "CALL";
  case RET:
    return "RETN";
  default:
    return "DATA";
  }
}

void Section::dump() const {
  Binary::analyze_log("%s\n", std::format(R"(Section {} {{
  .type = {}
  .addr = 0x{:x}
  .foff = 0x{:x}
  .size = 0x{:x} / {}
}})",
                                          name, type == TEXT ? "Code" : "Data",
                                          addr, foff, size, size)
                                  .c_str());
}

void Import::dump() const {
  Binary::analyze_log("%s\n", std::format(R"(Import {} {{
  .foff = 0x{:x}
  .ilib = {}
}})",
                                          name, foff, lib)
                                  .c_str());
}

void Function::dump() const {
  Binary::analyze_log("%s\n", std::format(R"(Function {} {{
  .begin = 0x{:x}
  .end   = 0x{:x}
  .size  = {}
}})",
                                          name, start, end, end - start)
                                  .c_str());
}

} // namespace aether
