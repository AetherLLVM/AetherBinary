// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-or-later */
// See LICENSE file in the root directory for full license text.

#include "AetherCommop.h"
#include "AetherArch.h"

namespace aether {

void insertDatas(Binary *bin, Functions &funcs, bool isobj) {
  std::vector<std::pair<addr_t, addr_t>> datfns;
  for (auto it = funcs.begin(), itend = funcs.end(); it != itend;) {
    auto curit = it++;
    if (it == itend) {
      break;
    }
    addr_t start = it->second.start;
    switch (bin->archType()) {
    case ARMV5TE:
    case ARM:
      start &= ~1;
      break;
    default:
      break;
    }
    if (curit->second.end < start) {
      if (bin->isCode(curit->second.end) && bin->isCode(start)) {
        datfns.push_back(std::make_pair(curit->second.end, start));
      }
    }
  }
  MachineARM64 ma64;
  for (auto &data : datfns) {
    auto &newfunc =
        funcs.insert(std::make_pair(data.first, Function())).first->second;
    newfunc.start = data.first;
    newfunc.end = data.second;
    strfmt(newfunc.name, "data_" ADDRFMT "", newfunc.start);
    if (bin->archType() == ARM64) {
      const char *opbuff = bin->addrBuff(newfunc.start);
      const char *opbuffend = bin->addrBuff(newfunc.end);
      ma64.analyzeFunc(bin->diser(), isobj, opbuff, opbuffend, newfunc, nullptr,
                       bin);
      continue;
    }
    for (addr_t a = newfunc.start; a < newfunc.end;) {
      Insinfo ii;
      ii.fnoff = (int)(a - newfunc.start);
      switch (bin->archType()) {
      case ARM64:
        ii.info.type = NORMAL;
        break;
      default:
        ii.info.type = IDATA;
        break;
      }
      addr_t left = newfunc.end - a;
      if (left < 4) {
        ii.info.oplen = left;
        newfunc.insns.push_back(ii);
        break;
      } else {
        ii.info.oplen = 4;
        a += 4;
        newfunc.insns.push_back(ii);
      }
    }
  }
}

} // namespace aether
