        code
        xdef _level1_audio_irq
        xref _level1AudioIRQ
_level1_audio_irq:
        movem.l d0-d7/a0-a6,-(sp)
        jsr _level1AudioIRQ
        movem.l (sp)+,d0-d7/a0-a6
        moveq #0,d0
        rts
