// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-or-later */
// See LICENSE file in the root directory for full license text.

#pragma once
#pragma pack(push)
#pragma pack(1)

#define AETHER_FILE_ANA 'ibea'
#define AETHER_FILE_EXT "aebi"
#define AETHER_FILE_VERSION 1

typedef char sbit8;
typedef short sbit16;
typedef int sbit32;
typedef long long sbit64;
typedef unsigned char ubit8;
typedef unsigned short ubit16;
typedef unsigned int ubit32;
typedef unsigned long long ubit64;

#ifndef StructCheck
#define StructCheck(name, sz)                                                  \
  static const int __check##name[sizeof(name) == (sz) ? 1 : -1] = {0}
#endif

namespace aether {

struct AebiSect {
  struct {
    ubit8 type;
    ubit16 name;
    ubit8 reserved;
  } info;
  ubit32 fileoff;
  ubit32 size;
  ubit64 addr;
};
StructCheck(AebiSect, 5 * sizeof(ubit32));

struct AebiInsn {
  ubit32 type : 3;
  ubit32 oplen : 4;
  ubit32 ngout : 13;
  ubit32 ncomin : 12;
  ubit64 gooff : 34;
  ubit64 fnoff : 30;

  ubit32 opcodeSize() const { return oplen + 1; }
};
StructCheck(AebiInsn, 3 * sizeof(ubit32));

struct AebiLoop1 {
  ubit8 from;
  ubit8 to;
};
StructCheck(AebiLoop1, 2 * sizeof(ubit8));

struct AebiLoop2 {
  ubit16 from;
  ubit16 to;
};
StructCheck(AebiLoop2, 2 * sizeof(ubit16));

struct AebiLoop4 {
  ubit32 from;
  ubit32 to;
};
StructCheck(AebiLoop4, 2 * sizeof(ubit32));

enum AebiFuncFlag {
  MFF_EXPORT = 0x1,
  MFF_IMPORT = 0x2,
  MFF_CTOR = 0x4,
  MFF_DTOR = 0x8,
};

struct AebiFunc {
  ubit32 name;
  ubit32 rvastart;
  ubit32 rvaend;
  ubit32 ninsn;
  struct {
    ubit16 nloop;
    ubit8 thumb;
    ubit8 flags;
  } info;
  ubit32 insnoff;
  ubit32 loopoff;

  ubit32 sizeofIndex() const {
    if (ninsn <= 0xFF)
      return 1;
    if (ninsn <= 0xFFFF)
      return 2;
    if (ninsn <= 0xFFFFFF)
      return 4;
    return -1;
  }

  ubit32 cominOffset(const AebiInsn *insn) const {
    return insn->gooff + sizeofIndex() * insn->ngout;
  }
};
StructCheck(AebiFunc, 7 * sizeof(ubit32));

struct AebiFile {
  ubit32 magic;
  ubit32 version;
  ubit32 filehash;
  struct {
    ubit8 filetype;
    ubit8 arch;
    ubit8 nsect;
    ubit8 reserved;
  } info;
  ubit64 baseaddr;
  ubit32 nfunc;
  ubit32 stroff;
  ubit32 funcoff;

  const AebiSect *sect(int i) const {
    return (AebiSect *)((char *)this + sizeof(*this)) + i;
  }

  const AebiFunc *func(int i) const {
    return (AebiFunc *)((char *)this + funcoff) + i;
  }

  const AebiInsn *insn(const AebiFunc *fn, int i) const {
    return (AebiInsn *)((char *)this + fn->insnoff) + i;
  }

  const AebiLoop1 *loop1(const AebiFunc *fn, int i) const {
    return (AebiLoop1 *)((char *)this + fn->loopoff) + i;
  }

  const AebiLoop2 *loop2(const AebiFunc *fn, int i) const {
    return (AebiLoop2 *)((char *)this + fn->loopoff) + i;
  }

  const AebiLoop4 *loop4(const AebiFunc *fn, int i) const {
    return (AebiLoop4 *)((char *)this + fn->loopoff) + i;
  }

  const ubit8 *index1(ubit32 off) const {
    return (ubit8 *)((char *)this + off);
  }

  const ubit16 *index2(ubit32 off) const {
    return (ubit16 *)((char *)this + off);
  }

  const ubit32 *index4(ubit32 off) const {
    return (ubit32 *)((char *)this + off);
  }

  const char *string(int off) const { return (char *)this + stroff + off; }

  ubit64 address(ubit32 rva) const { return baseaddr + rva; }
};
StructCheck(AebiFile, 9 * sizeof(ubit32));

} // namespace aether

#pragma pack(pop)
