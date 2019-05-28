; kusaOS boot asm
; TAB=4

BOTPAK  EQU     0x00280000      ;
DSKCAC  EQU     0x00100000
DSKCAC0 EQU     0x00008000

; BOOT_INFO�֌W�̒萔�錾 BOOT_INFO�͋N�����̏��Ƃ����Ӗ��Ŗ��t�����炵���B
CYLS    EQU     0x0ff0          ; �V�����_�̒l���L�^���郁�����Ԓn�̒萔
LEDS    EQU     0x0ff1          ; �L�[�{�[�h��LED��Ԃ̒l���L�^���郁�����Ԓn�̒萔
VMODE   EQU     0x0ff2          ; ��ʃ��[�h�̒l���L�^���郁�����Ԓn�̒萔
SCRNX   EQU     0x0ff4          ; �X�N���[����X���̒l���L�^���郁�����Ԓn�̒萔
SCRNY   EQU     0x0ff6          ; �X�N���[����Y���̒l���L�^���郁�����Ԓn�̒萔
VRAM    EQU     0x0ff8          ; VRAM�̃A�h���X���L�^���郁�����Ԓn�̒萔

        ORG     0xc200          ; ���̃v���O�������ǂݍ��܂�郁�����Ԓn�B�f�B�X�N�C���[�W��0x4200�ȍ~��kusaOS.sys�����邽�߁A0x8000 + 0x4200 = 0xc200����n�܂�悤�ɂ����B

; ��ʃ��[�h�ݒ�
        MOV     AL,0x13         ; VGA�O���t�B�b�N�X�A320x200x8bit�J���[�A�p�b�N�h�s�N�Z�� 
        MOV     AH,0x00         ; os-wiki�ɂ��Ƃ��̒l�ł����炵��
        INT     0x10            ; �r�f�I�֘A��BIOS�@�\���Ăяo��

; ��ʃ��[�h�̏����������Ƀ�������(C���ꂪ�Q�Ƃ���)
        MOV     BYTE [VMODE],8  
        MOV     WORD [SCRNX],320
        MOV     WORD [SCRNY],200
        MOV     DWORD [VRAM],0x000a0000 ; 320 �~ 200�̉�ʃ��[�h��VRAM�̃������Ԓn

; �L�[�{�[�h��LED��Ԃ�BIOS�ɋ����Ă��炤

        MOV     AH,0x02         ; �L�[�{�[�h�֌W��BIOS�@�\�Ăяo���́u�L�[���b�N���V�t�g��Ԏ擾�v�œ��삳���邽�߂ɁA0x02��AH�ɃZ�b�g�B
        INT     0x16            ; �L�[�{�[�h�֌W��BIOS�@�\�Ăяo���B
        MOV     [LEDS],AL       ; BIOS�@�\���Ăяo�������ʂ̏�ԃR�[�h��LEDS�̃A�h���X�ɋL�^�iBOOT_INFO)

;PIC�����荞�݂��󂯕t���Ȃ��悤�ɂ���

        MOV     AL,0xff
        OUT     0x21,AL
        NOP
        OUT     0xa1,AL

        CLI

; CPU����1MB�ȏ�̃������ɃA�N�Z�X�ł���悤�ɁAA20GATE��ݒ�

        CALL    waitkbdout
        MOV     AL,0xd1
        OUT     0x64,AL
        CALL    waitkbdout
        MOV     AL,0xdf
        OUT     0x60,AL
        CALL    waitkbdout

; �v���e�N�g���[�h�ڍs

[INSTRSET "i486p"]

        LGDT    [GDTR0]
        MOV     EAX,CR0
        AND     EAX,0x7fffffff
        OR      EAX,0x00000001
        MOV     CR0,EAX
        JMP     pipelineflush

pipelineflush:
        MOV     AX,1*8
        MOV     DS,AX
        MOV     ES,AX
        MOV     FS,AX
        MOV     GS,AX
        MOV     SS,AX

; bootpack�̓]��

        MOV     ESI,bootpack
        MOV     EDI,BOTPAK
        MOV     ECX,512*1024/4
        CALL    memcpy

; ���łɃf�B�X�N�f�[�^���{���̈ʒu�֓]��

;�u�[�g�Z�N�^����

        MOV     ESI,0x7c00
        MOV     EDI,DSKCAC
        MOV     ECX,512/4
        CALL    memcpy

; �c��S��

        MOV     ESI,DSKCAC0+512
        MOV     EDI,DSKCAC+512
        MOV     ECX,0
        MOV     CL,BYTE [CYLS]
        IMUL    ECX,512*18*2/4  
        SUB     ECX,512/4
        CALL    memcpy

; asmhead�ł��Ȃ���΂����Ȃ����͂��ׂďI���A���Ƃ�bootpack�ɔC����

; bootpack�̋N��

        MOV     EBX,BOTPAK
        MOV     ECX,[EBX+16]
        ADD     ECX,3
        SHR     ECX,2
        JZ      skip
        MOV     ESI,[EBX+20]
        ADD     ESI,EBX
        MOV     EDI,[EBX+12]
        CALL    memcpy

skip:
        MOV     ESP,[EBX+12]
        JMP     DWORD 2*8:0x0000001b

waitkbdout:
        IN      AL,0x64
        AND     AL,0x02
        JNZ     waitkbdout
        RET

memcpy:
        MOV     EAX,[ESI]
        ADD     ESI,4
        MOV     [EDI],EAX
        ADD     EDI,4
        SUB     ECX,1
        JNZ     memcpy
        RET

        ALIGNB  16
GDT0:
        RESB    8
        DW      0xffff,0x0000,0x9200,0x00cf
        DW      0xffff,0x0000,0x9a28,0x0047

        DW      0

GDTR0:
        DW      8*3-1
        DD      GDT0

        ALIGNB  16

bootpack:
