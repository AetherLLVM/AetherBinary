// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-or-later */
// See LICENSE file in the root directory for full license text.

/*
Usage: /path/to/icpp aether-asm.cc arch asm-code
  e.g.: /path/to/icpp aether-asm.cc arm64 "svc #80"
*/

#include "aether-comm.h"

int main(int argc, const char *argv[]) {
  if (argc < 3) {
    std::print("Usage: {} arm64|arm|thumb|x86|x86_64 asm-code\n", argv[0]);
    return 1;
  }
  // load LLVM and AetherBinary libraries, so we can use their APIs directly
  // within ICPP runtime environment.
  load_libraries(argv[0]);

  aether::Disassembler diser(argv[1]);
  if (!diser.ready()) {
    std::print("Failed to initialize the disassembler for {}", argv[1]);
    return -1;
  }
  unsigned char opcode[20];
  auto error = diser.assemble(argv[2], opcode);
  if (error.size()) {
    std::print("Failed to assemble {}: {}", argv[2], error);
    return -1;
  }
  return 0;
}
