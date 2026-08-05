; ChipSnake interrupt-driven Light Speed Player wrapper.
; Exec calls _lsp_vblank_interrupt from the system vertical-blank server
; chain. LSP prepares new channel data, then this wrapper waits twelve raster
; lines before enabling Paula DMA, as required by the standard player.

        code

        xdef    _lsp_init
        xdef    _lsp_tick
        xdef    _lsp_stop
        xdef    _lsp_enable
        xdef    _lsp_set_rate
        xdef    _lsp_vblank_interrupt

_lsp_init:
        movem.l d2-d7/a2-a6,-(sp)
        lea     lsp_dmacon+1(pc),a2
        bsr     LSP_MusicInit
        movem.l (sp)+,d2-d7/a2-a6
        rts

_lsp_tick:
        movem.l d0-d7/a0-a6,-(sp)
        bsr.s   lsp_play_frame
        movem.l (sp)+,d0-d7/a0-a6
        rts

; Exec interrupt-server entry. Return zero so the server chain continues.
_lsp_vblank_interrupt:
        movem.l d1-d7/a0-a6,-(sp)
        lea     lsp_enabled(pc),a0
        tst.b   (a0)
        beq.s   .done
        move.w  lsp_accumulator(pc),d1
        add.w   #50,d1
        cmp.w   lsp_frame_rate(pc),d1
        blo.s   .store
        sub.w   lsp_frame_rate(pc),d1
        move.w  d1,lsp_accumulator
        bsr.s   lsp_play_frame
        bra.s   .done
.store:
        move.w  d1,lsp_accumulator
.done:
        movem.l (sp)+,d1-d7/a0-a6
        moveq   #0,d0
        rts

lsp_play_frame:
        lea     $dff0a0,a6
        bsr     LSP_MusicPlayTick

        ; Measure elapsed raster lines rather than using a CPU-speed delay.
        move.l  $dff004,d2
        lsr.l   #8,d2
        and.w   #$01ff,d2
.waitdma:
        move.l  $dff004,d3
        lsr.l   #8,d3
        and.w   #$01ff,d3
        sub.w   d2,d3
        and.w   #$01ff,d3
        cmp.w   #12,d3
        blo.s   .waitdma

        move.w  lsp_dmacon(pc),$dff096
        rts

_lsp_enable:
        move.b  d0,lsp_enabled
        rts

_lsp_set_rate:
        tst.w   d0
        bne.s   .valid
        move.w  #50,d0
.valid:
        move.w  d0,lsp_frame_rate
        clr.w   lsp_accumulator
        rts

_lsp_stop:
        move.w  #$000f,$dff096
        clr.w   $dff0a8
        clr.w   $dff0b8
        clr.w   $dff0c8
        clr.w   $dff0d8
        move.w  #$8000,lsp_dmacon
        rts

lsp_dmacon:
        dc.w    $8000

lsp_frame_rate:
        dc.w    50
lsp_accumulator:
        dc.w    0
lsp_enabled:
        dc.b    0
        even

        include "third_party/LSPlayer/LightSpeedPlayer.asm"
