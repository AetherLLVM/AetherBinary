// AetherBinary - A library for MachO/ELF/PE analysis.
// Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com>
// SPDX-License-Identifier: AGPL-3.0-or-later */
// See LICENSE file in the root directory for full license text.

#pragma once

#include "AetherMeta.h"

#include <string.h>

namespace aether {

class Disassembler;
class Binary;

void insertDatas(Binary *bin, Functions &funcs, bool isobj);

#define AETHER_LIB_NAME "AetherBinary"

template <typename BIN> class BinaryCommop {
public:
  BinaryCommop(BIN *bin) { m_bin = bin; }

  ~BinaryCommop() {}

  bool analyze(const void *llvmbin, const char *buffid, bool isobj) {
    const char *fname = buffid;
    fname = strrchr(fname, '/');
    if (!fname) {
      fname = strrchr(buffid, '\\');
    }
    if (!fname) {
      fname = buffid;
    } else {
      fname += 1;
    }
    std::set<addr_t> newfunc, datas;
    for (auto &sect : m_bin->m_sects) {
      if (sect.second.type == TEXT && sect.second.addr != m_bin->imageBase()) {
        newfunc.insert(sect.second.addr);
      }
    }
    bool hasfnstarts = m_bin->hasFuncStarts();
    int prog = 1, progtmp;
    char progprefix[256];
    // 1 init parse
    snprintf(progprefix, sizeof(progprefix),
             AETHER_LIB_NAME " is parsing %s's functions", fname);
    for (auto it = m_bin->m_funcs.begin(), itend = m_bin->m_funcs.end();
         it != itend;) {
      BIN::analyze_progress(progprefix, prog++, (int)m_bin->m_funcs.size(),
                            progtmp);

      auto curit = it++;
      auto nextit = it;
      addr_t start = curit->second.start, end;
      auto sect = m_bin->addrSect(start);
      if (!sect) {
        // some encrypted elf will put fnaddr in non-section space
        continue;
      }
      if (nextit == itend) {
        end = sect->addr + sect->size;
      } else {
        end = std::min(nextit->first, sect->addr + sect->size);
      }
      const char *opbuff = m_bin->m_sectbuffs[sect->addr] + start - sect->addr;
      if ((start & 1) && m_bin->m_diserthumb) {
        m_bin->analyzeFunc(hasfnstarts, m_bin->m_diserthumb, opbuff, start, end,
                           newfunc, true);
      } else {
        m_bin->analyzeFunc(hasfnstarts, m_bin->m_diser, opbuff, start, end,
                           newfunc);
      }
    }
    m_bin->mergeFunc(newfunc);
    // 2 re-init parse
    newfunc.clear();
    prog = 1;
    snprintf(progprefix, sizeof(progprefix),
             AETHER_LIB_NAME " is reparsing %s's functions", fname);
    for (auto it = m_bin->m_funcs.begin(), itend = m_bin->m_funcs.end();
         it != itend;) {
      BIN::analyze_progress(progprefix, prog++, (int)m_bin->m_funcs.size(),
                            progtmp);

      auto curit = it++;
      auto nextit = it;
      addr_t start = curit->second.start, end;
      auto sect = m_bin->addrSect(start);
      if (!sect) {
        continue;
      }
      if (nextit == itend) {
        end = sect->addr + sect->size;
      } else {
        end = std::min(nextit->first, sect->addr + sect->size);
      }
      const char *opbuff = m_bin->m_sectbuffs[sect->addr] + start - sect->addr;
      if ((start & 1) && m_bin->m_diserthumb) {
        m_bin->analyzeFunc(hasfnstarts, m_bin->m_diserthumb, opbuff, start, end,
                           newfunc, true);
      } else {
        m_bin->analyzeFunc(hasfnstarts, m_bin->m_diser, opbuff, start, end,
                           newfunc, false, &datas);
      }
    }
    // remove function which has no section
    for (auto it = m_bin->m_funcs.begin(); it != m_bin->m_funcs.end();) {
      auto sect = m_bin->addrSect(it->second.start);
      if (sect) {
        it++;
      } else {
        it = m_bin->m_funcs.erase(it);
      }
    }
    m_bin->mergeFunc(newfunc);
    // 3 real analyzing
    prog = 1;
    snprintf(progprefix, sizeof(progprefix),
             AETHER_LIB_NAME " is analyzing %s's functions", fname);
    Function *lastfunc = nullptr;
    for (auto it = m_bin->m_funcs.begin(), itend = m_bin->m_funcs.end();
         it != itend;) {
      BIN::analyze_progress(progprefix, prog++, (int)m_bin->m_funcs.size(),
                            progtmp);

      addr_t start = it->second.start;
      auto sect = m_bin->addrSect(start);
      const char *opbuff = m_bin->m_sectbuffs[sect->addr] + start - sect->addr;
      const char *opbuffend = m_bin->m_sectbuffs[sect->addr] + sect->size;
      Function &func = it->second;
      if ((start & 1) && m_bin->m_diserthumb) {
        m_bin->analyzeFunc(m_bin->m_diserthumb, isobj, opbuff, opbuffend, func,
                           lastfunc, m_bin, true);
      } else {
        m_bin->analyzeFunc(m_bin->m_diser, isobj, opbuff, opbuffend, func,
                           lastfunc, m_bin, false, &datas);
      }
      lastfunc = &func;
      it++;
      if (isobj || hasfnstarts) {
        // no need reset function range for object file
        continue;
      }
      // remove the wrong sub function inside current function
      while (it != itend && it->first < func.end) {
        if (it->second.flags) {
          // fix func.end to the right range if middle has export/ctor flags
          func.end = it->first;
          break;
        }
        it = m_bin->m_funcs.erase(it);
      }
    }
    insertDatas(m_bin, m_bin->m_funcs, isobj);
    return true;
  }

private:
  BIN *m_bin;
};

} // namespace aether
