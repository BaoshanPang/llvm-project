//===-- TriCoreMCExpr.cpp - TriCore specific MC expression classes --------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the implementation of the assembly expression modifiers
// accepted by the TriCore architecture (e.g. "%hi", "%lo", ...).
//
//===----------------------------------------------------------------------===//

#include "TriCoreMCExpr.h"
#include "TriCoreFixupKinds.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCObjectStreamer.h"
#include "llvm/MC/MCSymbolELF.h"

using namespace llvm;

#define DEBUG_TYPE "TriCoremcexpr"

const TriCoreMCExpr*
TriCoreMCExpr::create(VariantKind Kind, const MCExpr *Expr,
                      MCContext &Ctx) {
    return new (Ctx) TriCoreMCExpr(Kind, Expr);
}

void TriCoreMCExpr::printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const {

  bool closeParen = printVariantKind(OS, Kind);

  const MCExpr *Expr = getSubExpr();
  Expr->print(OS, MAI);

  if (closeParen)
    OS << ')';
}

bool TriCoreMCExpr::printVariantKind(raw_ostream &OS, VariantKind Kind)
{
  switch (Kind) {
  case VK_TriCore_None:     return false;
  case VK_TriCore_LO:       OS << "lo:";  return false;
  case VK_TriCore_HI:       OS << "hi:";  return false;
#if 0
  case VK_TriCore_H44:      OS << "%h44("; return true;
  case VK_TriCore_M44:      OS << "%m44("; return true;
  case VK_TriCore_L44:      OS << "%l44("; return true;
  case VK_TriCore_HH:       OS << "%hh(";  return true;
  case VK_TriCore_HM:       OS << "%hm(";  return true;
  case VK_TriCore_LM:       OS << "%lm(";  return true;
    // FIXME: use %pc22/%pc10, if system assembler supports them.
  case VK_TriCore_PC22:     OS << "%hi("; return true;
  case VK_TriCore_PC10:     OS << "%lo("; return true;
    // FIXME: use %got22/%got10, if system assembler supports them.
  case VK_TriCore_GOT22:    OS << "%hi("; return true;
  case VK_TriCore_GOT10:    OS << "%lo("; return true;
  case VK_TriCore_GOT13:    return false;
  case VK_TriCore_13:       return false;
  case VK_TriCore_WDISP30:  return false;
  case VK_TriCore_WPLT30:   return false;
  case VK_TriCore_R_DISP32: OS << "%r_disp32("; return true;
  case VK_TriCore_TLS_GD_HI22:   OS << "%tgd_hi22(";   return true;
  case VK_TriCore_TLS_GD_LO10:   OS << "%tgd_lo10(";   return true;
  case VK_TriCore_TLS_GD_ADD:    OS << "%tgd_add(";    return true;
  case VK_TriCore_TLS_GD_CALL:   OS << "%tgd_call(";   return true;
  case VK_TriCore_TLS_LDM_HI22:  OS << "%tldm_hi22(";  return true;
  case VK_TriCore_TLS_LDM_LO10:  OS << "%tldm_lo10(";  return true;
  case VK_TriCore_TLS_LDM_ADD:   OS << "%tldm_add(";   return true;
  case VK_TriCore_TLS_LDM_CALL:  OS << "%tldm_call(";  return true;
  case VK_TriCore_TLS_LDO_HIX22: OS << "%tldo_hix22("; return true;
  case VK_TriCore_TLS_LDO_LOX10: OS << "%tldo_lox10("; return true;
  case VK_TriCore_TLS_LDO_ADD:   OS << "%tldo_add(";   return true;
  case VK_TriCore_TLS_IE_HI22:   OS << "%tie_hi22(";   return true;
  case VK_TriCore_TLS_IE_LO10:   OS << "%tie_lo10(";   return true;
  case VK_TriCore_TLS_IE_LD:     OS << "%tie_ld(";     return true;
  case VK_TriCore_TLS_IE_LDX:    OS << "%tie_ldx(";    return true;
  case VK_TriCore_TLS_IE_ADD:    OS << "%tie_add(";    return true;
  case VK_TriCore_TLS_LE_HIX22:  OS << "%tle_hix22(";  return true;
  case VK_TriCore_TLS_LE_LOX10:  OS << "%tle_lox10(";  return true;
#endif
  }
  llvm_unreachable("Unhandled TriCoreMCExpr::VariantKind");
}

TriCoreMCExpr::VariantKind TriCoreMCExpr::parseVariantKind(StringRef name)
{
  return StringSwitch<TriCoreMCExpr::VariantKind>(name)
    .Case("lo",  VK_TriCore_LO)
    .Case("hi",  VK_TriCore_HI)
#if 0
    .Case("h44", VK_TriCore_H44)
    .Case("m44", VK_TriCore_M44)
    .Case("l44", VK_TriCore_L44)
    .Case("hh",  VK_TriCore_HH)
    .Case("hm",  VK_TriCore_HM)
    .Case("lm",  VK_TriCore_LM)
    .Case("pc22",  VK_TriCore_PC22)
    .Case("pc10",  VK_TriCore_PC10)
    .Case("got22", VK_TriCore_GOT22)
    .Case("got10", VK_TriCore_GOT10)
    .Case("got13", VK_TriCore_GOT13)
    .Case("r_disp32",   VK_TriCore_R_DISP32)
    .Case("tgd_hi22",   VK_TriCore_TLS_GD_HI22)
    .Case("tgd_lo10",   VK_TriCore_TLS_GD_LO10)
    .Case("tgd_add",    VK_TriCore_TLS_GD_ADD)
    .Case("tgd_call",   VK_TriCore_TLS_GD_CALL)
    .Case("tldm_hi22",  VK_TriCore_TLS_LDM_HI22)
    .Case("tldm_lo10",  VK_TriCore_TLS_LDM_LO10)
    .Case("tldm_add",   VK_TriCore_TLS_LDM_ADD)
    .Case("tldm_call",  VK_TriCore_TLS_LDM_CALL)
    .Case("tldo_hix22", VK_TriCore_TLS_LDO_HIX22)
    .Case("tldo_lox10", VK_TriCore_TLS_LDO_LOX10)
    .Case("tldo_add",   VK_TriCore_TLS_LDO_ADD)
    .Case("tie_hi22",   VK_TriCore_TLS_IE_HI22)
    .Case("tie_lo10",   VK_TriCore_TLS_IE_LO10)
    .Case("tie_ld",     VK_TriCore_TLS_IE_LD)
    .Case("tie_ldx",    VK_TriCore_TLS_IE_LDX)
    .Case("tie_add",    VK_TriCore_TLS_IE_ADD)
    .Case("tle_hix22",  VK_TriCore_TLS_LE_HIX22)
    .Case("tle_lox10",  VK_TriCore_TLS_LE_LOX10)
#endif
    .Default(VK_TriCore_None);
}

TriCore::Fixups TriCoreMCExpr::getFixupKind(TriCoreMCExpr::VariantKind Kind) {
#if 1
  assert(0);
  return TriCore::fixup_tricore_mov_lo16_pcrel;
#else
  switch (Kind) {
  default: llvm_unreachable("Unhandled TriCoreMCExpr::VariantKind");
  case VK_TriCore_LO:       return TriCore::fixup_TriCore_lo10;
  case VK_TriCore_HI:       return TriCore::fixup_TriCore_hi22;
  case VK_TriCore_H44:      return TriCore::fixup_TriCore_h44;
  case VK_TriCore_M44:      return TriCore::fixup_TriCore_m44;
  case VK_TriCore_L44:      return TriCore::fixup_TriCore_l44;
  case VK_TriCore_HH:       return TriCore::fixup_TriCore_hh;
  case VK_TriCore_HM:       return TriCore::fixup_TriCore_hm;
  case VK_TriCore_LM:       return TriCore::fixup_TriCore_lm;
  case VK_TriCore_PC22:     return TriCore::fixup_TriCore_pc22;
  case VK_TriCore_PC10:     return TriCore::fixup_TriCore_pc10;
  case VK_TriCore_GOT22:    return TriCore::fixup_TriCore_got22;
  case VK_TriCore_GOT10:    return TriCore::fixup_TriCore_got10;
  case VK_TriCore_GOT13:    return TriCore::fixup_TriCore_got13;
  case VK_TriCore_13:       return TriCore::fixup_TriCore_13;
  case VK_TriCore_WPLT30:   return TriCore::fixup_TriCore_wplt30;
  case VK_TriCore_WDISP30:  return TriCore::fixup_TriCore_call30;
  case VK_TriCore_TLS_GD_HI22:   return TriCore::fixup_TriCore_tls_gd_hi22;
  case VK_TriCore_TLS_GD_LO10:   return TriCore::fixup_TriCore_tls_gd_lo10;
  case VK_TriCore_TLS_GD_ADD:    return TriCore::fixup_TriCore_tls_gd_add;
  case VK_TriCore_TLS_GD_CALL:   return TriCore::fixup_TriCore_tls_gd_call;
  case VK_TriCore_TLS_LDM_HI22:  return TriCore::fixup_TriCore_tls_ldm_hi22;
  case VK_TriCore_TLS_LDM_LO10:  return TriCore::fixup_TriCore_tls_ldm_lo10;
  case VK_TriCore_TLS_LDM_ADD:   return TriCore::fixup_TriCore_tls_ldm_add;
  case VK_TriCore_TLS_LDM_CALL:  return TriCore::fixup_TriCore_tls_ldm_call;
  case VK_TriCore_TLS_LDO_HIX22: return TriCore::fixup_TriCore_tls_ldo_hix22;
  case VK_TriCore_TLS_LDO_LOX10: return TriCore::fixup_TriCore_tls_ldo_lox10;
  case VK_TriCore_TLS_LDO_ADD:   return TriCore::fixup_TriCore_tls_ldo_add;
  case VK_TriCore_TLS_IE_HI22:   return TriCore::fixup_TriCore_tls_ie_hi22;
  case VK_TriCore_TLS_IE_LO10:   return TriCore::fixup_TriCore_tls_ie_lo10;
  case VK_TriCore_TLS_IE_LD:     return TriCore::fixup_TriCore_tls_ie_ld;
  case VK_TriCore_TLS_IE_LDX:    return TriCore::fixup_TriCore_tls_ie_ldx;
  case VK_TriCore_TLS_IE_ADD:    return TriCore::fixup_TriCore_tls_ie_add;
  case VK_TriCore_TLS_LE_HIX22:  return TriCore::fixup_TriCore_tls_le_hix22;
  case VK_TriCore_TLS_LE_LOX10:  return TriCore::fixup_TriCore_tls_le_lox10;
  }
#endif
}

bool
TriCoreMCExpr::evaluateAsRelocatableImpl(MCValue &Res,
                                       const MCAsmLayout *Layout,
                                       const MCFixup *Fixup) const {
  return getSubExpr()->evaluateAsRelocatable(Res, Layout, Fixup);
}

static void fixELFSymbolsInTLSFixupsImpl(const MCExpr *Expr, MCAssembler &Asm) {
  switch (Expr->getKind()) {
  case MCExpr::Target:
    llvm_unreachable("Can't handle nested target expr!");
    break;

  case MCExpr::Constant:
    break;

  case MCExpr::Binary: {
    const MCBinaryExpr *BE = cast<MCBinaryExpr>(Expr);
    fixELFSymbolsInTLSFixupsImpl(BE->getLHS(), Asm);
    fixELFSymbolsInTLSFixupsImpl(BE->getRHS(), Asm);
    break;
  }

  case MCExpr::SymbolRef: {
    const MCSymbolRefExpr &SymRef = *cast<MCSymbolRefExpr>(Expr);
    cast<MCSymbolELF>(SymRef.getSymbol()).setType(ELF::STT_TLS);
    break;
  }

  case MCExpr::Unary:
    fixELFSymbolsInTLSFixupsImpl(cast<MCUnaryExpr>(Expr)->getSubExpr(), Asm);
    break;
  }

}

void TriCoreMCExpr::fixELFSymbolsInTLSFixups(MCAssembler &Asm) const {
#if 1
  assert(0);
#else
  switch(getKind()) {
  default: return;
  case VK_TriCore_TLS_GD_CALL:
  case VK_TriCore_TLS_LDM_CALL: {
    // The corresponding relocations reference __tls_get_addr, as they call it,
    // but this is only implicit; we must explicitly add it to our symbol table
    // to bind it for these uses.
    MCSymbol *Symbol = Asm.getContext().getOrCreateSymbol("__tls_get_addr");
    Asm.registerSymbol(*Symbol);
    auto ELFSymbol = cast<MCSymbolELF>(Symbol);
    if (!ELFSymbol->isBindingSet())
      ELFSymbol->setBinding(ELF::STB_GLOBAL);
    LLVM_FALLTHROUGH;
  }
  case VK_TriCore_TLS_GD_HI22:
  case VK_TriCore_TLS_GD_LO10:
  case VK_TriCore_TLS_GD_ADD:
  case VK_TriCore_TLS_LDM_HI22:
  case VK_TriCore_TLS_LDM_LO10:
  case VK_TriCore_TLS_LDM_ADD:
  case VK_TriCore_TLS_LDO_HIX22:
  case VK_TriCore_TLS_LDO_LOX10:
  case VK_TriCore_TLS_LDO_ADD:
  case VK_TriCore_TLS_IE_HI22:
  case VK_TriCore_TLS_IE_LO10:
  case VK_TriCore_TLS_IE_LD:
  case VK_TriCore_TLS_IE_LDX:
  case VK_TriCore_TLS_IE_ADD:
  case VK_TriCore_TLS_LE_HIX22:
  case VK_TriCore_TLS_LE_LOX10: break;
  }
  fixELFSymbolsInTLSFixupsImpl(getSubExpr(), Asm);
#endif
}

void TriCoreMCExpr::visitUsedExpr(MCStreamer &Streamer) const {
  Streamer.visitUsedExpr(*getSubExpr());
}
