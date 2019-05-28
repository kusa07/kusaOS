; naskfunc
; TAB=4

[FORMAT "WCOFF"]            ; オブジェクトファイルを作るモード(bootpack.objとくっつけるためにWCOFFフォーマットにした)
[BITS 32]                   ; 32ビットモー用の機械語を作らせる

; オブジェクトファイルのための情報

[FILE "naskfunc.nas"]       ; ソースファイル名情報

        GLOBAL  _io_hlt      ; 関数名のグローバル宣言


; 関数

[SECTION .text]             ; オブジェクトファイルではこれを書いてからプログラムを書く

_io_hlt:                    ; void io_hlt(void);
        HLT
        RET