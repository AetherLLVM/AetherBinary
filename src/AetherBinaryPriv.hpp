// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-or-later */
// See LICENSE file in the root directory for full license text.

#pragma once

#define GET_REGINFO_ENUM
#define GET_INSTRINFO_ENUM
#include "llvm/../../lib/Target/AArch64/AArch64GenInstrInfo.inc"
#include "llvm/../../lib/Target/AArch64/AArch64GenRegisterInfo.inc"

#define GET_REGINFO_ENUM
#define GET_INSTRINFO_ENUM
#include "llvm/../../lib/Target/ARM/ARMGenInstrInfo.inc"
#include "llvm/../../lib/Target/ARM/ARMGenRegisterInfo.inc"

#define GET_REGINFO_ENUM
#define GET_INSTRINFO_ENUM
#include "llvm/../../lib/Target/X86/X86GenInstrInfo.inc"
#include "llvm/../../lib/Target/X86/X86GenRegisterInfo.inc"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/Format.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_os_ostream.h"

#define strfmt(nm, fmt, ...)                                                   \
  {                                                                            \
    nm.clear();                                                                \
    llvm::raw_string_ostream __sout(nm);                                       \
    __sout << llvm::format(fmt, __VA_ARGS__);                                  \
  }

#define objbase ((object::ObjectFile *)llvmbin)
#define ins ((MCInst *)llvminst)
