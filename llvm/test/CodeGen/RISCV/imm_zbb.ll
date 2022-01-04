; RUN: llc -mtriple=riscv64 -mattr=+experimental-zbb \
; RUN:   -verify-machineinstrs < %s | FileCheck %s -check-prefix=RV64IZBB


; Materializing constants with zbb

; Function Attrs: mustprogress nofree norecurse nosync nounwind readnone willreturn
define dso_local i64 @li_rori_1() local_unnamed_addr #0 {
; RV64IZBB-LABEL: li_rori_1:
; RV64IZBB:       # %bb.0:
; RV64IZBB-NEXT:    li a0, -18
; RV64IZBB-NEXT:    rori    a0, a0, 21
; RV64IZBB-NEXT:    ret
  ret i64 -149533581377537
}
; Function Attrs: mustprogress nofree norecurse nosync nounwind readnone willreturn
define dso_local i64 @li_rori_2() local_unnamed_addr #0 {
; RV64IZBB-LABEL: li_rori_2:
; RV64IZBB:       # %bb.0:
; RV64IZBB-NEXT:    li a0, -86
; RV64IZBB-NEXT:    rori    a0, a0, 4
; RV64IZBB-NEXT:    ret
  ret i64 -5764607523034234886
}
