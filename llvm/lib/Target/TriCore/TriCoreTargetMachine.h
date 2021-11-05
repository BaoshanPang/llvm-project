//===-- TriCoreTargetMachine.h - Define TargetMachine for TriCore ---*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the TriCore specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TRICORE_TRICORETARGETMACHINE_H
#define LLVM_LIB_TARGET_TRICORE_TRICORETARGETMACHINE_H

#include "TriCoreInstrInfo.h"
#include "TriCoreSubtarget.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetLoweringObjectFile.h"

namespace llvm {

class TriCoreTargetMachine : public LLVMTargetMachine {
  TriCoreSubtarget Subtarget;
  std::unique_ptr<TargetLoweringObjectFile> TLOF;

public:
  TriCoreTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                       StringRef FS, const TargetOptions &Options,
                       Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                       CodeGenOpt::Level OL, bool JIT);

//FIXME
    const TriCoreSubtarget * getSubtargetImpl() const {
      return &Subtarget;
    }

    virtual const TargetSubtargetInfo *
    getSubtargetImpl(const Function &) const override {
      return &Subtarget;
    }

 // Pass Pipeline Configuration
    virtual TargetPassConfig *createPassConfig(legacy::PassManagerBase &PM) override;

    TargetLoweringObjectFile *getObjFileLowering() const override {
      return TLOF.get();
    }
};

} // end namespace llvm

#endif
