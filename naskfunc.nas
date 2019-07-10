; naskfunc
; TAB=4

[FORMAT "WCOFF"]            ; オブジェクトファイルを作るモード(bootpack.objとくっつけるためにWCOFFフォーマットにした)
[INSTRSET "i486p"]          ; 486の命令まで使いたいという記述
[BITS 32]                   ; 32ビットモー用の機械語を作らせる
[FILE "naskfunc.nas"]       ; ソースファイル名情報。オブジェクトファイルのための情報

        GLOBAL  _io_hlt, _io_cli, _io_sti, _io_stihlt      ; 関数名のグローバル宣言
        GLOBAL  _io_in8, _io_in16, _io_in32 
        GLOBAL  _io_out8, _io_out16, _io_out32 
        GLOBAL	_io_load_eflags, _io_store_eflags
        GLOBAL  _load_gdtr, _load_idtr
        GLOBAL  _asm_inthandler21, _asm_inthandler27, _asm_inthandler2c
        EXTERN  _inthandler21, _inthandler27, _inthandler2c

; 以下、関数。C言語と連携するにはEAX、ECX、EDXしか使えない。

[SECTION .text]             ; オブジェクトファイルではこれを書いてからプログラムを書く

_io_hlt:        ; void io_hlt(void);
        HLT
        RET

_io_cli:        ; void io_cli(void);
        CLI
        RET

_io_sti:        ; void io_sti(void);
        STI
        RET

_io_stihlt:     ; void io_stihlt(void);
        STI
        HLT
        RET

; _io_inや_io_outラベルは、メモリ番地やI/Oポートに直接値を入れる関数群。
;   x86ではEDXでポート番号を指定し、EAXでデータ転送する様に決まっている。
;   IA-32 デベロッパーズマニュアル 上巻 「13.4 I/O命令」より

; 指定したメモリ番地のデータを読み取る関数群
_io_in8:        ; int io_in8(int port);
        MOV     EDX,[ESP+4]     ; [ESP+4]に第一引数がある。
        MOV     EAX,0           ; EAXの初期化
        IN      AL,DX           ; DXで指定したI/Oポートから読み取った値を、ALに入れる。
        RET


_io_in16:       ; int io_in16(int port);
        MOV     EDX,[ESP+4]
        MOV     EAX,0
        IN      AX,DX
        RET

_io_in32:       ; int io_in32(int port);
        MOV     EDX,[ESP+4]
        IN      EAX,DX
        RET

; 指定したメモリ番地にデータを書き込む関数群
_io_out8:       ; void io_out8(int port, int data);
        MOV     EDX,[ESP+4]
        MOV     AL,[ESP+8]
        OUT     DX,AL           ; デバイスへ電気信号を送る
        RET

_io_out16:      ; void io_out16(int port, int data);
        MOV     EDX,[ESP+4]
        MOV     EAX,[ESP+8]
        OUT     DX,AX
        RET

_io_out32:      ; void io_out32(int port, int data);
        MOV     EDX,[ESP+4]
        MOV     EAX,[ESP+8]
        OUT     DX,EAX
        RET

_io_load_eflags:    ; int io_load_eflags(vodid);
        PUSHFD
        POP     EAX
        RET

_io_store_eflags:   ; void io_store_eflags(int eflags);
        MOV     EAX,[ESP+4]
        PUSH    EAX
        POPFD
        RET

_load_gdtr:     ; void load_gdtr(int limit, int adder);
        MOV     AX,[ESP+4] 
        MOV     [ESP+6],AX
        LGDT    [ESP+6]
        RET

_load_idtr:     ; void load_idtr(int limit, int addr);
        MOV     AX,[ESP+4]
        MOV     [ESP+6],AX
        LIDT    [ESP+6]
        RET

_asm_inthandler21:
        PUSH    ES
        PUSH    DS
        PUSHAD
        MOV     EAX,ESP
        PUSH    EAX
        MOV     AX,SS
        MOV     DS,AX
        MOV     ES,AX
        CALL    _inthandler21
        POP     EAX
        POPAD
        POP     DS
        POP     ES
        IRETD

_asm_inthandler27:
        PUSH    ES
        PUSH    DS
        PUSHAD
        MOV     EAX,ESP
        PUSH    EAX
        MOV     AX,SS
        MOV     DS,AX
        MOV     ES,AX
        CALL    _inthandler27
        POP     EAX
        POPAD
        POP     DS
        POP     ES
        IRETD

_asm_inthandler2c:
        PUSH    ES
        PUSH    DS
        PUSHAD
        MOV     EAX,ESP
        PUSH    EAX
        MOV     AX,SS
        MOV     DS,AX
        MOV     ES,AX
        CALL    _inthandler2c
        POP     EAX
        POPAD
        POP     DS
        POP     ES
        IRETD
