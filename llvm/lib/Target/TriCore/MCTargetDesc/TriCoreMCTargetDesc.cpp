//===-- TriCoreMCTargetDesc.cpp - TriCore Target Descriptions -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides TriCore specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;
 extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeTriCoreTargetMC() {
   // Register the target.
   //RegisterTargetMachine<TriCoreTargetMachine> Z(getTheTriCoreTarget());
 }
