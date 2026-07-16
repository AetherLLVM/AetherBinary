// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-or-later */
// See LICENSE file in the root directory for full license text.

#include "include/AetherBinary.h"

#include <icpp.hpp>

#if __APPLE__
static const char *llvm_name = "libLLVM.dylib";
static const char *aether_name = "libAetherBinary.dylib";
static const char *lib_dir = "lib";
#elif __linux__
static const char *llvm_name = "libLLVM.so.22.1";
static const char *aether_name = "libAetherBinary.so";
static const char *lib_dir = "lib";
#else
static const char *llvm_name = "LLVM-22.dll";
static const char *aether_name = "AetherBinary.dll";
static const char *lib_dir = "bin";
#endif

bool load_libraries(std::string_view script_file) {
  auto script_path = fs::absolute(script_file);
  auto script_dir = script_path.parent_path();
  for (auto libname : {llvm_name, aether_name}) {
    auto lib = script_dir / lib_dir / libname;
    if (!icpp::load_library(lib.string())) {
      std::println("Failed to load {}", lib.string());
      return false;
    }
  }
  return true;
}

int main(int argc, const char *argv[]) {
  if (argc == 1) {
    std::println("Usage: {} /path/to/file", argv[0]);
    return -1;
  }
  // load LLVM and AetherBinary dynamically, after this icpp can resolve symbols
  // for us
  if (!load_libraries(argv[0])) {
    return -1;
  }

  // turn log on
  aether::setAnalyzeCallback(std::printf, nullptr);

  auto script_path = fs::absolute(argv[0]);
  auto script_dir = script_path.parent_path().string();
  std::replace(script_dir.begin(), script_dir.end(), '\\', '/');
  auto bin = aether::New(argv[1]);
  if (!bin) {
    std::println("Failed to open {}", argv[1]);
    return -1;
  }
  std::println("AetherBinary {}. Copyright (c) 2026 aethervm.com.\nPlaying "
               "with a binary file using C++.",
               aether::getVersion());
  // let the user play with this bin instance, have fun~
  while (!std::cin.eof()) {
    std::string snippet;
    std::cout << ">>> ";
    std::getline(std::cin, snippet);
    if (snippet.empty())
      continue;

    icpp::exec_string(std::format(R"(
      #include "{}/include/AetherBinary.h"
      #include <icpp.hpp>
      using namespace aether;
      int main(void) {{
        auto bin = (Binary *){:p};
        {};return 0;}})",
                                  script_dir, (void *)bin, snippet));
  }
  aether::Delete(bin);
  return 0;
}
