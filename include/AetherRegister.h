// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: Apache License, Version 2.0
// See LICENSE file in the root directory for full license text.

#pragma once

namespace aether {

// x86/x64 rflags bits
struct rfbits_t {
  uint64_t CF : 1, Rsv1 : 1, PF : 1, Rsv3 : 1, AF : 1, Rsv5 : 1, ZF : 1, SF : 1,
      TF : 1, IF : 1, DF : 1, OF : 1, IOPL : 2, NT : 1, RSV15 : 1, RF : 1,
      VM : 1, AC : 1, VIF : 1, VIP : 1, ID : 1, Rsv22 : 10, Rsv32 : 32;
};

struct x64regs_t {
  uint64_t cax;
  uint64_t ccx;
  uint64_t cdx;
  uint64_t cbx;
  uint64_t csp;
  uint64_t cbp;
  uint64_t csi;
  uint64_t cdi;
  uint64_t r8;
  uint64_t r9;
  uint64_t r10;
  uint64_t r11;
  uint64_t r12;
  uint64_t r13;
  uint64_t r14;
  uint64_t r15;
  uint64_t cip;
};

struct arm_subregs_t {
  uint32_t flags;
  uint32_t gprs[14];
};

struct arm64_subregs_t {
  uint32_t flags;
  uint64_t gprs[31];
};

} // namespace aether
