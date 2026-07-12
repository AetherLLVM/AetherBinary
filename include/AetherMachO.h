// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-or-later */
// See LICENSE file in the root directory for full license text.

#pragma once

#include "AetherBinary.h"
#include "AetherCommop.h"

namespace aether {

class __AETHER_API__ MachOBinary : public Binary {
public:
  MachOBinary();
  virtual ~MachOBinary();

  void setFileOffset(int foff) { m_fileoff = foff; }

  virtual bool analyze(const void *llvmbin);
  virtual bool hasFuncStarts() const { return m_hasfnstarts; }
  bool isEncrypted();
  bool hasObjc();
  ImportInfos parseImports();
  int platformType();

  static int injectDylib(const char *macho, const char *dylib,
                         const char *rpath);

protected:
  virtual void initBaseAddr(const void *llvmbin);
  virtual void initImports(const void *llvmbin);

private:
  void parseSymptrs(const void *llvmbin, void *sect, int indsymidx,
                    std::map<unsigned, size_t> &symidxes);
  void parseDwarf(const std::string &path);
  void parseSwiftFunctions();
  void parseObjcStubs();

private:
  bool m_hasfnstarts;
};

class __AETHER_API__ MachOARM64Binary : public MachOBinary,
                                        public MachineARM64 {
public:
  friend class BinaryCommop<MachOARM64Binary>;

public:
  MachOARM64Binary();
  virtual ~MachOARM64Binary();

  virtual bool analyze(const void *llvmbin);
  virtual bool isArm64e() const;
};

class __AETHER_API__ MachOARMBinary : public MachOBinary, public MachineARM {
public:
  friend class BinaryCommop<MachOARMBinary>;

public:
  MachOARMBinary();
  virtual ~MachOARMBinary();

  virtual bool analyze(const void *llvmbin);
};

class __AETHER_API__ MachOX32Binary : public MachOBinary, public MachineX86 {
public:
  friend class BinaryCommop<MachOX32Binary>;

public:
  MachOX32Binary();
  virtual ~MachOX32Binary();

  virtual bool analyze(const void *llvmbin);
};

class __AETHER_API__ MachOX64Binary : public MachOX32Binary {
public:
  MachOX64Binary();
  virtual ~MachOX64Binary();

  virtual bool analyze(const void *llvmbin);
};

} // namespace aether
