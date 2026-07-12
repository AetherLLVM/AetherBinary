// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-or-later */
// See LICENSE file in the root directory for full license text.

#pragma once

#include "AetherBinary.h"
#include "AetherCommop.h"

namespace aether {

class __AETHER_API__ PEBinary : public Binary {
public:
  PEBinary();
  virtual ~PEBinary();

  virtual bool analyze(const void *llvmbin);

protected:
  virtual void initBaseAddr(const void *llvmbin);
  virtual void initImports(const void *llvmbin);
};

class __AETHER_API__ PEX32Binary : public PEBinary, public MachineX86 {
public:
  friend class BinaryCommop<PEX32Binary>;

public:
  PEX32Binary();
  virtual ~PEX32Binary();

  virtual bool analyze(const void *llvmbin);
};

class __AETHER_API__ PEX64Binary : public PEX32Binary {
public:
  PEX64Binary();
  virtual ~PEX64Binary();

  virtual bool analyze(const void *llvmbin);
};

class __AETHER_API__ PEARM64Binary : public PEBinary, public MachineARM64 {
public:
  friend class BinaryCommop<PEARM64Binary>;

public:
  PEARM64Binary();
  virtual ~PEARM64Binary();

  virtual bool analyze(const void *llvmbin);
};

} // namespace aether
