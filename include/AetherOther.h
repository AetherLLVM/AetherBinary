// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: Apache License, Version 2.0
// See LICENSE file in the root directory for full license text.

#pragma once

#include "AetherBinary.h"

namespace aether {

class __AETHER_API__ AebiBinary : public Binary {
public:
  AebiBinary() {}
  virtual ~AebiBinary() {}
  bool initLLVM(const char *buff, int size);

  virtual bool analyze(const void *llvmbin);

protected:
  virtual void initBaseAddr(const void *llvmbin) {}
  virtual void initImports(const void *llvmbin) {}
};

class __AETHER_API__ AnyBinary : public Binary {
public:
  AnyBinary() {}
  virtual ~AnyBinary() {}

  virtual bool analyze(const void *llvmbin);

protected:
  virtual void initBaseAddr(const void *llvmbin) {}
  virtual void initImports(const void *llvmbin) {}
};

} // namespace aether
