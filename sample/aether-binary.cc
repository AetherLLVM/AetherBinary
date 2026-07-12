// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-or-later */
// See LICENSE file in the root directory for full license text.

/*
Usage: /path/to/icpp -I../include aether-binary.cc arm64-ios
*/

import std;

namespace fs = std::filesystem;

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    std::print("Usage: {} /path/to/binary\n", argv[0]);
    return 1;
  }

  return 0;
}
