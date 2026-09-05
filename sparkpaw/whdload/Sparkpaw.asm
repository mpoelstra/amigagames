; Sparkpaw WHDLoad slave
; MrDig Productions, 2026
;
; Sparkpaw is an AmigaOS executable which takes over the AGA custom chips.
; Run it inside a small Kickstart 3.1 environment so WHDLoad owns startup and
; the F10 emergency exit without changing the accepted game executable.

	INCDIR	Include:
	INCLUDE	whdload.i
	INCLUDE	whdmacros.i

CHIPMEMSIZE	= $1ff000
; kick31.s adds its $80000 ROM area to this value. Together they consume the
; complete 8 MB expansion allocation while leaving 7.5 MB as emulated Fast RAM.
FASTMEMSIZE	= $780000
NUMDRIVES	= 1
WPDRIVES	= %0001

BOOTDOS
CACHECHIP
HDINIT
INIT_AUDIO
IOCACHE		= 4096
SEGTRACKER
SETKEYBOARD
WHDCTRL

slv_Version	= 19
slv_Flags	= WHDLF_NoError|WHDLF_Examine
slv_keyexit	= $59			; F10

	INCLUDE	whdload/kick31.s

slv_CurrentDir	dc.b	"data",0
slv_name	dc.b	"Sparkpaw: The Stormstone Quest",0
slv_copy	dc.b	"2026 MrDig Productions",0
slv_info	dc.b	"Installed and adapted for WHDLoad",10
		dc.b	"by MrDig Productions / 100% AI",10
		dc.b	"Version 0.7.0-alpha.2 (05-Sep-2026)",0
slv_config	= slv_base
	EVEN

_bootdos
	move.l	(_resload,pc),a2

	moveq	#OLTAG_DOS,d0
	move.l	(4),a6
	jsr	(_LVOTaggedOpenLibrary,a6)
	lea	(_dosbase,pc),a0
	move.l	d0,(a0)
	move.l	d0,a6
	beq	.program_err

	lea	(_program,pc),a0
	move.l	a0,d1
	jsr	(_LVOLoadSeg,a6)
	move.l	d0,d7
	beq	.program_err

	move.l	d7,a1
	add.l	a1,a1
	add.l	a1,a1
	moveq	#_args_end-_args,d0
	lea	(_args,pc),a0
	jsr	(4,a1)

	move.l	d7,d1
	move.l	(_dosbase,pc),a6
	jsr	(_LVOUnLoadSeg,a6)

	pea	TDREASON_OK
	move.l	(_resload,pc),a2
	jmp	(resload_Abort,a2)

.program_err
	move.l	(_dosbase,pc),a6
	beq	.no_dos_error
	jsr	(_LVOIoErr,a6)
	bra	.abort
.no_dos_error
	moveq	#0,d0
.abort
	pea	(_program,pc)
	move.l	d0,-(a7)
	pea	TDREASON_DOSREAD
	move.l	(_resload,pc),a2
	jmp	(resload_Abort,a2)

_program	dc.b	"Sparkpaw",0
_args		dc.b	10
_args_end
	EVEN

_dosbase	dc.l	0
