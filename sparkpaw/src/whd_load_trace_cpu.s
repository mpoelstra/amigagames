; Diagnostic only: 68020+ read of CACR via Exec Supervisor.
; No write to CACR, no cache flush or interrupt-mask modification here.
        code
        xdef _whdLoadTraceReadCacr
_whdLoadTraceReadCacr:
        movem.l a5-a6,-(sp)
        move.l 4.w,a6
        lea .read(pc),a5
        jsr -30(a6)
        movem.l (sp)+,a5-a6
        rts
.read:
        movec cacr,d0
        rte
