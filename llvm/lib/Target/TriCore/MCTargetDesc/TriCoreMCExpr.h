//====- TriCoreMCExpr.h - TriCore specific MC expression classes --*- C++ -*-=====//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes TriCore-specific MCExprs, used for modifiers like
// "%hi" or "%lo" etc.,
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TriCore_MCTARGETDESC_SPARCMCEXPR_H
#define LLVM_LIB_TARGET_TriCore_MCTARGETDESC_SPARCMCEXPR_H

#include "TriCoreFixupKinds.h"
#include "llvm/MC/MCExpr.h"

namespace llvm {

class StringRef;
class TriCoreMCExpr : public MCTargetExpr {
public:
  enum VariantKind {
    VK_TriCore_None,
    VK_TriCore_LO,
    VK_TriCore_HI,
#if 0
    VK_TriCore_H44,
    VK_TriCore_M44,
    VK_TriCore_L44,
    VK_TriCore_HH,
    VK_TriCore_HM,
    VK_TriCore_LM,
    VK_TriCore_PC22,
    VK_TriCore_PC10,
    VK_TriCore_GOT22,
    VK_TriCore_GOT10,
    VK_TriCore_GOT13,
    VK_TriCore_13,
    VK_TriCore_WPLT30,
    VK_TriCore_WDISP30,
    VK_TriCore_R_DISP32,
    VK_TriCore_TLS_GD_HI22,
    VK_TriCore_TLS_GD_LO10,
    VK_TriCore_TLS_GD_ADD,
    VK_TriCore_TLS_GD_CALL,
    VK_TriCore_TLS_LDM_HI22,
    VK_TriCore_TLS_LDM_LO10,
    VK_TriCore_TLS_LDM_ADD,
    VK_TriCore_TLS_LDM_CALL,
    VK_TriCore_TLS_LDO_HIX22,
    VK_TriCore_TLS_LDO_LOX10,
    VK_TriCore_TLS_LDO_ADD,
    VK_TriCore_TLS_IE_HI22,
    VK_TriCore_TLS_IE_LO10,
    VK_TriCore_TLS_IE_LD,
    VK_TriCore_TLS_IE_LDX,
    VK_TriCore_TLS_IE_ADD,
    VK_TriCore_TLS_LE_HIX22,
    VK_TriCore_TLS_LE_LOX10
#endif
  };

private:
  const VariantKind Kind;
  const MCExpr *Expr;

  explicit TriCoreMCExpr(VariantKind Kind, const MCExpr *Expr)
      : Kind(Kind), Expr(Expr) {}

public:
  /// @name Construction
  /// @{

  static const TriCoreMCExpr *create(VariantKind Kind, const MCExpr *Expr,
                                 MCContext &Ctx);
  /// @}
  /// @name Accessors
  /// @{

  /// getOpcode - Get the kind of this expression.
  VariantKind getKind() const { return Kind; }

  /// getSubExpr - Get the child of this expression.
  const MCExpr *getSubExpr() const { return Expr; }

  /// getFixupKind - Get the fixup kind of this expression.
  TriCore::Fixups getFixupKind() const { return getFixupKind(Kind); }

  /// @}
  void printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const override;
  bool evaluateAsRelocatableImpl(MCValue &Res,
                                 const MCAsmLayout *Layout,
                                 const MCFixup *Fixup) const override;
  void visitUsedExpr(MCStreamer &Streamer) const override;
  MCFragment *findAssociatedFragment() const override {
    return getSubExpr()->findAssociatedFragment();
  }

  void fixELFSymbolsInTLSFixups(MCAssembler &Asm) const override;

  static bool classof(const MCExpr *E) {
    return E->getKind() == MCExpr::Target;
  }

  static bool classof(const TriCoreMCExpr *) { return true; }

  static VariantKind parseVariantKind(StringRef name);
  static bool printVariantKind(raw_ostream &OS, VariantKind Kind);
  static TriCore::Fixups getFixupKind(VariantKind Kind);
};

} // end namespace llvm.

#endif
