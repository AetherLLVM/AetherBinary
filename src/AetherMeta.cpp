// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-or-later */
// See LICENSE file in the root directory for full license text.

#include "AetherMeta.h"

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

} // namespace aether
