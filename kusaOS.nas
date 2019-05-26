; kusaOS
; TAB=4

        ORG     0xc200          ; このプログラムが読み込まれるメモリ番地。ディスクイメージで0x4200以降にkusaOS.sysが入るため、0x8000 + 0x4200 = 0xc200から始まるようにした。

        MOV     AL,0x13         ; VGAグラフィックス、320x200x8bitカラー、パックドピクセル 
        MOV     AH,0x00         ; os-wikiによるとこの値でいいらしい
        INT     0x10            ; ビデオ関連のBIOS機能を呼び出し

fin:
        HLT
        JMP     fin