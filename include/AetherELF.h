// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: Apache License, Version 2.0
// See LICENSE file in the root directory for full license text.

#pragma once

#include "AetherBinary.h"
#include "AetherCommop.h"

namespace aether {

class __AETHER_API__ ELFBinary : public Binary {
public:
  ELFBinary();
  virtual ~ELFBinary();

  virtual bool analyze(const void *llvmbin);
  ImportInfos parseImports();
  ExportInfos parseExports();

protected:
  virtual void initBaseAddr(const void *llvmbin);
  virtual void initImports(const void *llvmbin);

  template <typename ELFOBJ, typename ELFFILE>
  void analyzePrograms(const ELFOBJ *elfobj, const ELFFILE *elffile);
  Function *getOrInsertFunction(addr_t addr, const char *name, bool *exist,
                                const Section **sect);
};

class __AETHER_API__ ELFX32Binary : public ELFBinary, public MachineX86 {
public:
  friend class BinaryCommop<ELFX32Binary>;

public:
  ELFX32Binary();
  virtual ~ELFX32Binary();

  virtual bool analyze(const void *llvmbin);
};

class __AETHER_API__ ELFX64Binary : public ELFX32Binary {
public:
  friend class BinaryCommop<ELFX64Binary>;

public:
  ELFX64Binary();
  virtual ~ELFX64Binary();

  virtual bool analyze(const void *llvmbin);
};

class __AETHER_API__ ELFARMBinary : public ELFBinary, public MachineARM {
public:
  friend class BinaryCommop<ELFARMBinary>;

public:
  ELFARMBinary();
  virtual ~ELFARMBinary();

  virtual bool analyze(const void *llvmbin);
};

class __AETHER_API__ ELFARM64Binary : public ELFBinary, public MachineARM64 {
public:
  friend class BinaryCommop<ELFARM64Binary>;

public:
  ELFARM64Binary();
  virtual ~ELFARM64Binary();

  virtual bool analyze(const void *llvmbin);
};

} // namespace aether
