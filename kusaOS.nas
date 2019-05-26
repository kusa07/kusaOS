; kusaOS
; TAB=4

; BOOT_INFO関係の定数宣言 BOOT_INFOは起動時の情報という意味で名付けたらしい。
CYLS    EQU     0x0ff0          ; シリンダの値を記録するメモリ番地の定数
LEDS    EQU     0x0ff1          ; キーボードのLED状態の値を記録するメモリ番地の定数
VMODE   EQU     0x0ff2          ; 画面モードの値を記録するメモリ番地の定数
SCRNX   EQU     0x0ff4          ; スクリーンのX軸の値を記録するメモリ番地の定数
SCRNY   EQU     0x0ff6          ; スクリーンのY軸の値を記録するメモリ番地の定数
VRAM    EQU     0x0ff8          ; VRAMのアドレスを記録するメモリ番地の定数

        ORG     0xc200          ; このプログラムが読み込まれるメモリ番地。ディスクイメージで0x4200以降にkusaOS.sysが入るため、0x8000 + 0x4200 = 0xc200から始まるようにした。

        MOV     AL,0x13         ; VGAグラフィックス、320x200x8bitカラー、パックドピクセル 
        MOV     AH,0x00         ; os-wikiによるとこの値でいいらしい
        INT     0x10            ; ビデオ関連のBIOS機能を呼び出し

; 画面モードの情報をメモリにメモする
        MOV     BYTE [VMODE],8  
        MOV     WORD [SCRNX],320
        MOV     WORD [SCRNY],200
        MOV     DWORD [VRAM],0x000a0000 ; 320 × 200の画面モードのVRAMのメモリ番地

; キーボードのLED状態をBIOSに教えてもらう

        MOV     AH,0x02         ; キーボード関係のBIOS機能呼び出しの「キーロック＆シフト状態取得」で動作させるために、0x02をAHにセット。
        INT     0x16            ; キーボード関係のBIOS機能呼び出し。
        MOV     [LEDS],AL       ; BIOS機能を呼び出した結果の状態コードをLEDSのアドレスに記録（BOOT_INFO)

fin:
        HLT
        JMP     fin