// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-or-later */
// See LICENSE file in the root directory for full license text.

/*
Usage: /path/to/icpp aether-binary.cc -- arm64-ios

Note: Don't miss the double dashes --, otherwise icpp will treat the input
binary file as a C++ source file, which will fail the whole command running.
*/

#include "aether-comm.h"

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    std::print("Usage: {} /path/to/binary\n", argv[0]);
    return 1;
  }
  // load LLVM and AetherBinary libraries, so we can use their APIs directly
  // within ICPP runtime environment.
  load_libraries(argv[0]);

  // load and dump the input binary file
  auto bin = aether::New(argv[1]);
  if (!bin) {
    std::print("Failed to parse {}", argv[1]);
    return -1;
  }
  bin->dump();
  aether::Delete(bin);
  return 0;
}
