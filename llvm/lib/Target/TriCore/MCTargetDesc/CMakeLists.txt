add_llvm_component_library(LLVMTriCoreDesc
  TriCoreMCTargetDesc.cpp
  TriCoreInstPrinter.cpp
  TriCoreAsmBackend.cpp
  TriCoreELFObjectWriter.cpp
  TriCoreMCCodeEmitter.cpp
  TriCoreMCAsmInfo.cpp

  LINK_COMPONENTS
  MC
  TriCoreInfo
  Support

  ADD_TO_COMPONENT
  TriCore
  )
