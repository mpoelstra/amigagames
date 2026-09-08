        code
        xdef _proof_audio_irq
        xdef _proof_vblank_irq
        xref _proofAudioIRQ
        xref _proofVBlank
_proof_audio_irq:
        movem.l d0-d7/a0-a6,-(sp)
        jsr _proofAudioIRQ
        movem.l (sp)+,d0-d7/a0-a6
        moveq #0,d0
        rts
_proof_vblank_irq:
        movem.l d0-d7/a0-a6,-(sp)
        jsr _proofVBlank
        movem.l (sp)+,d0-d7/a0-a6
        moveq #0,d0
        rts
