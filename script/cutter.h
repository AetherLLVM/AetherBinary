// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: Apache License, Version 2.0
// See LICENSE file in the root directory for full license text.

#pragma once

#include "comdef.h"

// export to Cutter++'s REPL
#define bin aether_binary()

static aether::Binary *aether_binary() {
  auto sptr = std::getenv(BINENV);
  if (!sptr) {
    auto location = std::source_location::current();
    auto src = std::string(location.file_name()) + ".cc";
    auto logfn = std::format("{:p}", (void *)cpp::print);
    std::array argv{cpp::current_file(), logfn.c_str()};
    // load and analyze the current file with the following script command:
    // cutter.h.cc /path/to/current_file cpp::print
    icpp::exec_source(src.c_str(), std::ssize(argv), &argv[0]);
    // it shoule be valid now
    sptr = std::getenv(BINENV);
  }
  if (!sptr) {
    // cutter.h.cc failed anyway?
    static aether::AnyBinary empty_bin;
    cpp::print(
        "Failed to initialize aether binary instance, return an empty one.");
    return &empty_bin;
  }
  return (aether::Binary *)std::strtoull(sptr, nullptr, 16);
}
