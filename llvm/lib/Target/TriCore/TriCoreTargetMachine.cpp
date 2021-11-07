//===-- TriCoreTargetMachine.cpp - Define TargetMachine for TriCore -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#include "TriCoreTargetMachine.h"
#include "TriCore.h"
#include "TriCoreTargetObjectFile.h"
#include "TargetInfo/TriCoreTargetInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
using namespace llvm;

/**
@brief Creates a TriCore machine architecture.
*
* The data layout is described as below:
*
*  Meaning of symbols:
*  Symbol    | Definition                                       |
*  :-------- | :--:                                             |
*   e        | little endian                                    |
*   p:32:32  | pointer size: pointer ABI alignment              |
*   i1:8:32  | integer data type : size of type : ABI alignment |
*   f:64:64  | float data type : size of type : ABI alignment   |
*   n32      | all 32 bit registers are available               |
*
*
*/
TriCoreTargetMachine::TriCoreTargetMachine(const Target &T, const Triple &TT,
                                   StringRef CPU, StringRef FS,
                                   const TargetOptions &Options,
                                   Optional<Reloc::Model> RM,Optional<CodeModel::Model> CM,
                                   CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T,
      "e-m:e-p:32:32-i64:32-a:0:32-n32",
      TT, CPU, FS,
      Options, RM.getValueOr(Reloc::Static), CM.getValueOr(CodeModel::Small), OL),
      Subtarget(TT, CPU, FS, *this),
      TLOF(std::make_unique<TriCoreTargetObjectFile>()) {
    initAsmInfo();
}


 extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeTriCoreTarget() {
   // Register the target.
   RegisterTargetMachine<TriCoreTargetMachine> X(getTheTriCoreTarget());
 }


// static std::string computeDataLayout(const Triple &T, bool is64Bit) {
//   // TriCore is typically big endian, but some are little.
//   std::string Ret = T.getArch() == Triple::sparcel ? "e" : "E";
//   Ret += "-m:e";

//   // Some ABIs have 32bit pointers.
//   if (!is64Bit)
//     Ret += "-p:32:32";

//   // Alignments for 64 bit integers.
//   Ret += "-i64:64";

//   // On TriCoreV9 128 floats are aligned to 128 bits, on others only to 64.
//   // On TriCoreV9 registers can hold 64 or 32 bits, on others only 32.
//   if (is64Bit)
//     Ret += "-n32:64";
//   else
//     Ret += "-f128:64-n32";

//   if (is64Bit)
//     Ret += "-S128";
//   else
//     Ret += "-S64";

//   return Ret;
// }

// static Reloc::Model getEffectiveRelocModel(Optional<Reloc::Model> RM) {
//   return RM.getValueOr(Reloc::Static);
// }

// // Code models. Some only make sense for 64-bit code.
// //
// // SunCC  Reloc   CodeModel  Constraints
// // abs32  Static  Small      text+data+bss linked below 2^32 bytes
// // abs44  Static  Medium     text+data+bss linked below 2^44 bytes
// // abs64  Static  Large      text smaller than 2^31 bytes
// // pic13  PIC_    Small      GOT < 2^13 bytes
// // pic32  PIC_    Medium     GOT < 2^32 bytes
// //
// // All code models require that the text segment is smaller than 2GB.
// static CodeModel::Model
// getEffectiveTriCoreCodeModel(Optional<CodeModel::Model> CM, Reloc::Model RM,
//                            bool Is64Bit, bool JIT) {
//   if (CM) {
//     if (*CM == CodeModel::Tiny)
//       report_fatal_error("Target does not support the tiny CodeModel", false);
//     if (*CM == CodeModel::Kernel)
//       report_fatal_error("Target does not support the kernel CodeModel", false);
//     return *CM;
//   }
//   if (Is64Bit) {
//     if (JIT)
//       return CodeModel::Large;
//     return RM == Reloc::PIC_ ? CodeModel::Small : CodeModel::Medium;
//   }
//   return CodeModel::Small;
// }

// /// Create an ILP32 architecture model
// TriCoreTargetMachine::TriCoreTargetMachine(
//     const Target &T, const Triple &TT, StringRef CPU, StringRef FS,
//     const TargetOptions &Options, Optional<Reloc::Model> RM,
//     Optional<CodeModel::Model> CM, CodeGenOpt::Level OL, bool JIT, bool is64bit)
//     : LLVMTargetMachine(T, computeDataLayout(TT, is64bit), TT, CPU, FS, Options,
//                         getEffectiveRelocModel(RM),
//                         getEffectiveTriCoreCodeModel(
//                             CM, getEffectiveRelocModel(RM), is64bit, JIT),
//                         OL),
//       TLOF(std::make_unique<TriCoreELFTargetObjectFile>()),
//       Subtarget(TT, std::string(CPU), std::string(FS), *this, is64bit),
//       is64Bit(is64bit) {
//   initAsmInfo();
// }

// TriCoreTargetMachine::~TriCoreTargetMachine() {}

// const TriCoreSubtarget *
// TriCoreTargetMachine::getSubtargetImpl(const Function &F) const {
//   Attribute CPUAttr = F.getFnAttribute("target-cpu");
//   Attribute FSAttr = F.getFnAttribute("target-features");

//   std::string CPU =
//       CPUAttr.isValid() ? CPUAttr.getValueAsString().str() : TargetCPU;
//   std::string FS =
//       FSAttr.isValid() ? FSAttr.getValueAsString().str() : TargetFS;

//   // FIXME: This is related to the code below to reset the target options,
//   // we need to know whether or not the soft float flag is set on the
//   // function, so we can enable it as a subtarget feature.
//   bool softFloat = F.getFnAttribute("use-soft-float").getValueAsBool();

//   if (softFloat)
//     FS += FS.empty() ? "+soft-float" : ",+soft-float";

//   auto &I = SubtargetMap[CPU + FS];
//   if (!I) {
//     // This needs to be done before we create a new subtarget since any
//     // creation will depend on the TM and the code generation flags on the
//     // function that reside in TargetOptions.
//     resetTargetOptions(F);
//     I = std::make_unique<TriCoreSubtarget>(TargetTriple, CPU, FS, *this,
//                                           this->is64Bit);
//   }
//   return I.get();
// }

 // namespace {
 // /// TriCore Code Generator Pass Configuration Options.
 // class TriCorePassConfig : public TargetPassConfig {
 // public:
 //   TriCorePassConfig(TriCoreTargetMachine &TM, PassManagerBase &PM)
 //     : TargetPassConfig(TM, PM) {}

 //   TriCoreTargetMachine &getTriCoreTargetMachine() const {
 //     return getTM<TriCoreTargetMachine>();
 //   }

 //   void addIRPasses() override;
 //   bool addInstSelector() override;
 //   void addPreEmitPass() override;
 // };
 // } // namespace

TargetPassConfig *TriCoreTargetMachine::createPassConfig(PassManagerBase &PM) {
//   return new TriCorePassConfig(*this, PM);
    assert(0);
    return NULL;
}

// void TriCorePassConfig::addIRPasses() {
//   addPass(createAtomicExpandPass());

//   TargetPassConfig::addIRPasses();
// }

// bool TriCorePassConfig::addInstSelector() {
//   addPass(createTriCoreISelDag(getTriCoreTargetMachine()));
//   return false;
// }

// void TriCorePassConfig::addPreEmitPass(){
//   addPass(createTriCoreDelaySlotFillerPass());

//   if (this->getTriCoreTargetMachine().getSubtargetImpl()->insertNOPLoad())
//   {
//     addPass(new InsertNOPLoad());
//   }
//   if (this->getTriCoreTargetMachine().getSubtargetImpl()->detectRoundChange()) {
//     addPass(new DetectRoundChange());
//   }
//   if (this->getTriCoreTargetMachine().getSubtargetImpl()->fixAllFDIVSQRT())
//   {
//     addPass(new FixAllFDIVSQRT());
//   }
// }

// void TriCoreV8TargetMachine::anchor() { }

// TriCoreV8TargetMachine::TriCoreV8TargetMachine(const Target &T, const Triple &TT,
//                                            StringRef CPU, StringRef FS,
//                                            const TargetOptions &Options,
//                                            Optional<Reloc::Model> RM,
//                                            Optional<CodeModel::Model> CM,
//                                            CodeGenOpt::Level OL, bool JIT)
//     : TriCoreTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, JIT, false) {}

// void TriCoreV9TargetMachine::anchor() { }

// TriCoreV9TargetMachine::TriCoreV9TargetMachine(const Target &T, const Triple &TT,
//                                            StringRef CPU, StringRef FS,
//                                            const TargetOptions &Options,
//                                            Optional<Reloc::Model> RM,
//                                            Optional<CodeModel::Model> CM,
//                                            CodeGenOpt::Level OL, bool JIT)
//     : TriCoreTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, JIT, true) {}

// void TriCoreelTargetMachine::anchor() {}

// TriCoreelTargetMachine::TriCoreelTargetMachine(const Target &T, const Triple &TT,
//                                            StringRef CPU, StringRef FS,
//                                            const TargetOptions &Options,
//                                            Optional<Reloc::Model> RM,
//                                            Optional<CodeModel::Model> CM,
//                                            CodeGenOpt::Level OL, bool JIT)
//     : TriCoreTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, JIT, false) {}
