; kusaOS
; TAB=4

        ORG     0xc200          ; このプログラムが読み込まれるメモリ番地。ディスクイメージで0x4200以降にkusaOS.sysが入るため、0x8000 + 0x4200 = 0xc200から始まるようにした。

fin:
        HLT
        JMP     fin