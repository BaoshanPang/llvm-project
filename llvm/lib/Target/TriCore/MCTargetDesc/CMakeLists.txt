add_llvm_component_library(LLVMTriCoreDesc
  TriCoreMCTargetDesc.cpp

  LINK_COMPONENTS
  MC
  TriCoreInfo
  Support

  ADD_TO_COMPONENT
  TriCore
  )
