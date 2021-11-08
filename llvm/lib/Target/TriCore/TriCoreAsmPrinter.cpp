//===-- TriCoreAsmPrinter.cpp - TriCore LLVM assembly writer --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to the XAS-format TriCore assembly language.
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/StringRef.h"
#define DEBUG_TYPE "asm-printer"
#include "TriCore.h"
#include "MCTargetDesc/TriCoreInstPrinter.h"
#include "TriCoreInstrInfo.h"
#include "TriCoreMCInstLower.h"
#include "TriCoreSubtarget.h"
#include "TriCoreTargetMachine.h"
#include "TargetInfo/TriCoreTargetInfo.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineJumpTableInfo.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Mangler.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include <algorithm>
#include <cctype>
using namespace llvm;

namespace {
class TriCoreAsmPrinter : public AsmPrinter {
  TriCoreMCInstLower MCInstLowering;

public:
  explicit TriCoreAsmPrinter(TargetMachine &TM,
                         std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)), MCInstLowering(*this) {}

  StringRef getPassName() const override { return "TriCore Assembly Printer"; }

  void EmitFunctionEntryLabel();
  void EmitInstruction(const MachineInstr *MI);
  void EmitFunctionBodyStart();
};
} // end of anonymous namespace

void TriCoreAsmPrinter::EmitFunctionBodyStart() {
  assert(0);
//  MCInstLowering.Initialize(Mang, &MF->getContext());
}

void TriCoreAsmPrinter::EmitFunctionEntryLabel() {
  assert(0);
//  OutStreamer->EmitLabel(CurrentFnSym);
}

void TriCoreAsmPrinter::EmitInstruction(const MachineInstr *MI) {
  MCInst TmpInst;
  MCInstLowering.Lower(MI, TmpInst);

  EmitToStreamer(*OutStreamer, TmpInst);
}

// Force static initialization.
extern "C" void LLVMInitializeTriCoreAsmPrinter() {
  RegisterAsmPrinter<TriCoreAsmPrinter> X(getTheTriCoreTarget());
}
