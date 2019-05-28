; naskfunc
; TAB=4

[FORMAT "WCOFF"]            ; オブジェクトファイルを作るモード(bootpack.objとくっつけるためにWCOFFフォーマットにした)
[INSTRSET "i486p"]          ; 486の命令まで使いたいという記述
[BITS 32]                   ; 32ビットモー用の機械語を作らせる
[FILE "naskfunc.nas"]       ; ソースファイル名情報。オブジェクトファイルのための情報

        GLOBAL  _io_hlt,_write_mem8      ; 関数名のグローバル宣言

; 以下、関数。C言語と連携するにはEAX、ECX、EDXしか使えない。

[SECTION .text]             ; オブジェクトファイルではこれを書いてからプログラムを書く

_io_hlt:                    ; void io_hlt(void);
        HLT
        RET

_write_mem8:				; void write_mem8(int addr, int data); MOV BYTE[addr],data みたいなことをさせたい。
		MOV		ECX,[ESP+4]	; 一つ目の引数（アドレス）を代入
		MOV		AL,[ESP+8]	; 二つ目の引数を代入
		MOV		[ECX],AL	; ECXの上位一つ目の引数が入っている状態。そこの下位8bitに二つ目の引数を詰める。
		RET
