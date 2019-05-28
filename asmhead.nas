; kusaOS boot asm
; TAB=4

BOTPAK  EQU     0x00280000      ;
DSKCAC  EQU     0x00100000
DSKCAC0 EQU     0x00008000

; BOOT_INFO関係の定数宣言 BOOT_INFOは起動時の情報という意味で名付けたらしい。
CYLS    EQU     0x0ff0          ; シリンダの値を記録するメモリ番地の定数
LEDS    EQU     0x0ff1          ; キーボードのLED状態の値を記録するメモリ番地の定数
VMODE   EQU     0x0ff2          ; 画面モードの値を記録するメモリ番地の定数
SCRNX   EQU     0x0ff4          ; スクリーンのX軸の値を記録するメモリ番地の定数
SCRNY   EQU     0x0ff6          ; スクリーンのY軸の値を記録するメモリ番地の定数
VRAM    EQU     0x0ff8          ; VRAMのアドレスを記録するメモリ番地の定数

        ORG     0xc200          ; このプログラムが読み込まれるメモリ番地。ディスクイメージで0x4200以降にkusaOS.sysが入るため、0x8000 + 0x4200 = 0xc200から始まるようにした。

; 画面モード設定
        MOV     AL,0x13         ; VGAグラフィックス、320x200x8bitカラー、パックドピクセル 
        MOV     AH,0x00         ; os-wikiによるとこの値でいいらしい
        INT     0x10            ; ビデオ関連のBIOS機能を呼び出し

; 画面モードの情報をメモリにメモする(C言語が参照する)
        MOV     BYTE [VMODE],8  
        MOV     WORD [SCRNX],320
        MOV     WORD [SCRNY],200
        MOV     DWORD [VRAM],0x000a0000 ; 320 × 200の画面モードのVRAMのメモリ番地

; キーボードのLED状態をBIOSに教えてもらう

        MOV     AH,0x02         ; キーボード関係のBIOS機能呼び出しの「キーロック＆シフト状態取得」で動作させるために、0x02をAHにセット。
        INT     0x16            ; キーボード関係のBIOS機能呼び出し。
        MOV     [LEDS],AL       ; BIOS機能を呼び出した結果の状態コードをLEDSのアドレスに記録（BOOT_INFO)

;PICが割り込みを受け付けないようにする

        MOV     AL,0xff
        OUT     0x21,AL
        NOP
        OUT     0xa1,AL

        CLI

; CPUから1MB以上のメモリにアクセスできるように、A20GATEを設定

        CALL    waitkbdout
        MOV     AL,0xd1
        OUT     0x64,AL
        CALL    waitkbdout
        MOV     AL,0xdf
        OUT     0x60,AL
        CALL    waitkbdout

; プロテクトモード移行

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

; bootpackの転送

        MOV     ESI,bootpack
        MOV     EDI,BOTPAK
        MOV     ECX,512*1024/4
        CALL    memcpy

; ついでにディスクデータも本来の位置へ転送

;ブートセクタから

        MOV     ESI,0x7c00
        MOV     EDI,DSKCAC
        MOV     ECX,512/4
        CALL    memcpy

; 残り全部

        MOV     ESI,DSKCAC0+512
        MOV     EDI,DSKCAC+512
        MOV     ECX,0
        MOV     CL,BYTE [CYLS]
        IMUL    ECX,512*18*2/4  
        SUB     ECX,512/4
        CALL    memcpy

; asmheadでしなければいけない事はすべて終了、あとはbootpackに任せる

; bootpackの起動

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
