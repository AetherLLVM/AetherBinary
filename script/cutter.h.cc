// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-or-later */
// See LICENSE file in the root directory for full license text.

#include "comdef.h"

// local storage, keep BINENV valid all the time
static std::string bin_env;

int main(int argc, const char *argv[]) {
  if (argc != 3) {
    std::println("Usage: {} /path/to/file log-function-pointer", argv[0]);
    return -1;
  }
  // set Cutter++'s log function
  auto log_print = (aether::analyze_log_t)std::strtoull(argv[2], nullptr, 16);
  log_print("AetherBinary's analyzing %s...", argv[1]);

  // load LLVM and AetherBinary dynamically, after this icpp can resolve symbols
  // for us
  if (!load_libraries(argv[0], log_print)) {
    return -1;
  }

  auto inst = aether::New(argv[1]);
  if (!inst) {
    log_print("Failed to open %s", argv[1]);
    return -1;
  }
  log_print("AetherBinary %s. Copyright (c) 2026 aethervm.com.\nPlaying "
            "with a binary file using C++.",
            aether::getVersion());
  // return BIN within BINENV
  bin_env = std::format("{}={:p}", BINENV, (void *)inst);
  putenv((char *)bin_env.c_str());
  return 0;
}
