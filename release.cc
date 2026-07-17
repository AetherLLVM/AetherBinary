// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-or-later */
// See LICENSE file in the root directory for full license text.

#include "build-Release/generated/Version.h"

#include <icpp.hpp>

#if __APPLE__
const std::string_view platform = "apple";
const std::string_view osname = "macos";
#elif __linux__
const std::string_view platform = "linux";
const std::string_view osname = "linux";
#else
const std::string_view platform = "win";
const std::string_view osname = "windows";
#endif

#if __aarch64__ || __arm64__
#if __linux__
const std::string_view arch = "aarch64";
#else
const std::string_view arch = "arm64";
#endif
#else
const std::string_view arch = "x86_64";
#endif

static auto pack_dir(const fs::path &srcdir, const fs::path &dstroot,
                     std::string_view dstname = "") {
  auto dstdir = dstroot / (dstname.size() ? dstname : srcdir.filename());
  std::error_code err;
  auto source = srcdir;
  if (fs::is_symlink(srcdir))
    source = fs::canonical(srcdir, err);
  fs::copy(source, dstdir,
           fs::copy_options::overwrite_existing | fs::copy_options::recursive |
               fs::copy_options::copy_symlinks,
           err);
  if (err)
    std::println("Failed to copy directory: {} ==> {}, {}.", source.string(),
                 dstdir.string(), err.message());
  else
    std::println("Packed directory {} from {}.", dstdir.string(),
                 source.string());
}

int main(int argc, const char *argv[]) {
  if (argc != 2) {
    std::println("Usage: {} /path/to/install", argv[0]);
    return -1;
  }

  auto dstroot = fs::path(argv[1]);
  auto script_dir = fs::absolute(argv[0]).parent_path();
  auto pkgname = std::format("AetherBinary-v{}-{}-{}", PROJECT_VERSION_STRING,
                             osname, arch);
  pack_dir(script_dir / "build-Release/install", dstroot, pkgname);

  auto targz = std::format("{}.tar.gz", pkgname);
  std::println("Packing AetherLLVM release package {}...", targz);
#if __APPLE__
  std::system(
      std::format("find {} -name .DS_Store -delete", dstroot.string()).data());
#endif
  std::system(
      std::format("cd {} && tar czf {} {}", dstroot.string(), targz, pkgname)
          .data());
  std::println("Created AetherLLVM package {}.", targz);
  return 0;
}
