// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-or-later */
// See LICENSE file in the root directory for full license text.

/*
Usage:

 * option 1 - Run this command in the terminal to build the project:
  /path/to/icpp build.cc
  or
  /path/to/icpp build.cc Debug ; if you want to build in Debug mode.

 * option 2 - Run this script directly in VSCode with "ICPP - Run" command,
  the log of the building process can be found in OUTOUT panel of ICPP channel.
*/

import std;

namespace fs = std::filesystem;

#if 1 // change to 0 to only print the command without executing it.
#define command(fmt, ...) std::system(std::format(fmt, __VA_ARGS__).c_str())
#else
#define command(fmt, ...)                                                      \
  {                                                                            \
    std::print(fmt, __VA_ARGS__);                                              \
    std::print("\n");                                                          \
  }
#endif

// quote the path with double quotes, to avoid issues with spaces in the path.
std::string dqpath(const fs::path &p, const fs::path &child = "") {
  return "\"" + (p / child).string() + "\"";
}

int main(int argc, const char *argv[]) {
  auto script_path = fs::absolute(argv[0]);
  std::print("Running build script {}...\n", script_path.string());

  auto script_dir = script_path.parent_path();

  std::print("Phase 1: Build LLVM...\n");
  auto llvm_build_dir = script_dir / "build-llvm";
  auto llvm_cmake_dir = script_dir / "cmake" / "llvm";
  command("cmake -S {} -B {} -G Ninja", dqpath(llvm_cmake_dir),
          dqpath(llvm_build_dir));
  command("cmake --build {} --target install -- -j8", dqpath(llvm_build_dir));

  std::print("Phase 2: Build AetherBinary...\n");
  auto build_type = "Release";
  if (argc > 1) {
    build_type = argv[1];
  }
  auto aether_binary_build_dir =
      script_dir / (std::string("build") + "-" + build_type);
  command(
      "cmake -S {} -B {} -G Ninja -DCMAKE_BUILD_TYPE={} "
      "-DCMAKE_PREFIX_PATH={} -DCMAKE_INSTALL_PREFIX={} -DLLVM_BUILD_DIR={}",
      dqpath(script_dir), dqpath(aether_binary_build_dir), build_type,
      dqpath(llvm_build_dir, "install"),
      dqpath(aether_binary_build_dir, "install"),
      dqpath(llvm_build_dir, "llvm"));
  command("cmake --build {} --target install -- -j8",
          dqpath(aether_binary_build_dir));

  std::print("Build completed.\n");
  return 0;
}
