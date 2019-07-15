/* bootpackのメイン */

#include "bootpack.h"
#include <stdio.h>

/* 背景色(壁紙) */
char bc = COL8_000084;   /* back-color 暗い青 */


void HariMain(void)
{
    struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;     /* asmhead.nasと同じ先頭のメモリ番地を指定している。これは同時にcylsのメモリ番地を示している事にもなる。 */
    char s[40], mcursor[256];
    int mx, my;

    /* GDT,IDTの初期化 */
    init_gdtidt();

    /* PICの初期化 */
    init_pic();
    io_sti();

    /* パレットを設定 */
    init_palette();


    /* デスクトップの描画 */
    init_screen8(binfo->vram, binfo->scrnx, binfo->scrny, bc);   /* 直接引数に構造体のメンバを示すための矢印記法を使っている */

    /* マウスカーソルの色や形の設定 */
    init_mouse_cursor8(mcursor, bc);

    mx = (binfo->scrnx - 16) / 2;       /* マウスのX軸の開始位置 */
    my = (binfo->scrny - 28 - 16) / 2;  /* マウスのY軸の開始位置 */

    /* マウスカーソルの描画 */
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

    /* マウスの位置を画面に表示するためにメモリに書き出す */
    sprintf(s, "(%d, %d)", mx, my);

    /* マウスの位置を画面に描画 */
    /* putfonts8_ascの引数、ビデオアクセス用メモリ番地、画面のX軸、文字列のX軸の開始位置、文字列のY軸の開始位置、色、文字列 */
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

    /* 割り込みマスクをキーボード(IRQ1)とマウス(IRQ12)のところだけ解除 */
    io_out8(PIC0_IMR, 0xf9);    /* 0xf9 = 11111001でIRQ1とPIC1がカスケード接続されているIRQ2のマスクを解除 */
    io_out8(PIC1_IMR, 0xef);    /* 0xef = 11101111でIRQ8から割り振られているため、5bit目がIRQ12で、そのマスクを解除 */

    /* 処理が終わったら無限HLT */
    for (;;) {
        io_hlt();   
    }
}
