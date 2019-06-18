/* グラフィック処理関係 */

#include "bootpack.h"


void init_palette(void)
{
    static unsigned char table_rgb[16 * 3] = {  /*static charはデータにしか使えないがDB命令相当 */
    /*   
         0xff = 明るい　0x84 = 暗い
         赤     緑    青    　　 
    */
        0x00, 0x00, 0x00,   /* 0:黒 */
        0xff, 0x00, 0x00,   /* 1:明るい赤 */
        0x00, 0xff, 0x00,   /* 2:明るい緑 */
        0xff, 0xff, 0x00,   /* 3:明るい黄色 */
        0x00, 0x00, 0xff,   /* 4:明るい青 */
        0xff, 0x00, 0xff,   /* 5:明るい紫 */
        0x00, 0xff, 0xff,   /* 6:明るい水色 */
        0xff, 0xff, 0xff,   /* 7:白 */
        0xc6, 0xc6, 0xc6,   /* 8:明るい灰色 */
        0x84, 0x00, 0x00,   /* 9:暗い赤 */
        0x00, 0x84, 0x00,   /* 10:暗い緑 */
        0x84, 0x84, 0x00,   /* 11:暗い黄色 */
        0x00, 0x00, 0x84,   /* 12:暗い青 */
        0x84, 0x00, 0x84,   /* 13:暗い紫 */
        0x00, 0x84, 0x84,   /* 14:暗い水色 */
        0x84, 0x84, 0x84,   /* 15:暗い灰色 */
    };

    set_palette(0, 15, table_rgb);      /* 上で設定した配列を、パレットカラーとして設定していく */
    return;
}

/* table_rgbを、パレットカラーとして設定していく */
void set_palette(int start, int end, unsigned char *rgb)
{
    int i, eflags;

    eflags = io_load_eflags();      /* 現在のeflagsの状態を取っておく */
    io_cli();                       /* 割り込み禁止にする */

    /* 0x03c8に設定したいパレット番号を書き込む。
       続けて設定する場合にはここは省略できる。 */
    io_out8(0x03c8, start); 

    for (i = start; i <= end; i++) {

        /* 0x03c9にRGBの順に書き込む */
        io_out8(0x03c9, rgb[0] / 4); 
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    io_store_eflags(eflags);        /* パレットを設定する前のeflagsの状態に戻す。ここで割り込み禁止も解かれる */
    return;
}

/* 四角を描くための関数 */
/* 引数は、
    vram = ビデオアクセス用メモリ番地
    xsize = 画面のX軸の大きさ
    c = 色
    x0 = X軸の開始位置
    y0 = Y軸の開始位置
    x1 = X軸の終了位置
    y1 = Y軸の終了位置
*/
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
    int x, y;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++) {
            /* vram変数にビデオアクセス用のメモリ番地が入っており、
            結局「0xa0000 + y * 320 + x」をやっているのと変わらない。
            変数cで色のパレット番号をメモリに入れている。*/
            vram[y * xsize + x] = c;    
        }
    }
    return;
}

/* デスクトップを描画 */
void init_screen8(char *vram, int x, int y, char bc )
{
    /* boxfill8の引数は、（ビデオアクセス用メモリ番地、画面のX軸の大きさ、色、X軸の開始位置、Y軸の開始位置、X軸の終了位置、Y軸の終了位置） */
    boxfill8(vram, x,          bc,      0,        0, x -  1, y - 29);    /* 背景 (背景の後に色々描画しないと、背景にすべて消される */

    /* 補助線 */
    boxfill8(vram, x, COL8_848484,      0,   (y - 28) / 2,  x - 1, (y - 28) / 2);   /* 横線 */
    boxfill8(vram, x, COL8_FFFFFF,      x / 2,   0,     x / 2, y -  1);   /* 縦線 */

    boxfill8(vram, x, COL8_008484,      0,   y - 28, x -  1, y - 28);    /* タスクバー 上部横線 その１ */
    boxfill8(vram, x, COL8_848400,      0,   y - 27, x -  1, y - 27);    /* タスクバー 上部横線 その２ */
    boxfill8(vram, x, COL8_FFFFFF,      0,   y - 26, x -  1, y -  1);    /* タスクバー */

    boxfill8(vram, x, COL8_C6C6C6,      3,   y - 24,     59, y - 24);   /* OSボタン 上部横線　その１ */
    boxfill8(vram, x, COL8_C6C6C6,      2,   y - 24,      2, y -  4);   /* OSボタン 左縦線 */
    boxfill8(vram, x, COL8_848484,      3,   y -  4,     59, y -  4);   /* OSボタン 下部横線 */
    boxfill8(vram, x, COL8_848484,     59,   y - 23,     59, y -  5);   /* OSボタン 右縦線 */

    boxfill8(vram, x, COL8_000000,      2,   y -  3,     59, y -  3);   /* ? */
    boxfill8(vram, x, COL8_000000,     60,   y - 24,     60, y -  3);   /* ? */

    boxfill8(vram, x, COL8_848484, x - 47,   y - 24, x -  4, y - 24);   /* タスクバー右のくぼみ 上部横線 */
    boxfill8(vram, x, COL8_848484, x - 47,   y - 23, x - 47, y -  4);   /* タスクバーの右のくぼみ 左縦線 */
    boxfill8(vram, x, COL8_FFFFFF, x - 47,   y -  3, x -  4, y -  3);   /* タスクバーの右のくぼみ 下横線 */
    boxfill8(vram, x, COL8_848484, x -  3,   y - 24, x -  3, y -  3);   /* タスクバーの右のくぼみ 右縦線 */

    return;
}

/* フォントを描画するための関数　*/
/* 引数は、
   vram = vramのメモリ番地
   xsize = 画面のX軸のサイズ
   x = フォントのX軸の表示位置
   y = フォントのY軸の表示位置
   c = 色
   font = フォントの情報
*/
void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
{
    int i;
    char d;     /* data */
    char *p;

    for (i = 0; i < 16; i++) {
        p = vram + (y + i) * xsize + x;     /* ビデオアクセス用メモリ番地を指定する計算の共通部分をポインタ変数pに代入 */
        d = font[i];

        /* ポインタ変数p + 1~7 をしてx軸それぞれに代入できるようにする */
        if ((d & 0x80) != 0) { p[0] = c; }
        if ((d & 0x40) != 0) { p[1] = c; }
        if ((d & 0x20) != 0) { p[2] = c; }
        if ((d & 0x10) != 0) { p[3] = c; }
        if ((d & 0x08) != 0) { p[4] = c; }
        if ((d & 0x04) != 0) { p[5] = c; }
        if ((d & 0x02) != 0) { p[6] = c; }
        if ((d & 0x01) != 0) { p[7] = c; }
    }
    return;
}

/* 文字列を受け取って文字数分だけputfont8を使って描画する関数 */
/* 引数は、
   vram = vramのメモリ番地
   xsize = 画面のX軸のサイズ
   x = フォントのX軸の表示位置
   y = フォントのY軸の表示位置
   c = 色
   s = 描画したい文字列
*/
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s)
{
    extern char hankaku[4096];  /* hankaku.txtをコンパイルして外部の変数という扱いで読み込むためexternを使っている */
    for (; *s != 0x00; s++) {   /* 文字列の最後（0x00）でなければループを続ける */
        /* hankakuのメモリ番地 + 文字列の文字コード * 16
        （hankakuのメモリ番地のフォントのデータが0xNN0~0xNNfに入っているので、16倍している) 
        */
        putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
        x += 8;     /* x = x + 8 する事で次の文字の場所まで移動する */
    }
    return;
} 

/* マウスの形と色のデータをmcursor[]に代入していく */
/* 引数は、
   mouse = マウスのデータを格納する配列（mcursor[]）
   bc = 背景の色（back-color)
*/
void init_mouse_cursor8(char *mouse, char bc)
{
    /* マウスカーソルを準備（16×16) */
    static char cursor[16][16] = {
         
        /* 本オリジナルのマウスカーソル */

        /*
        "**************..",
        "*OOOOOOOOOOO*...",
        "*OOOOOOOOOO*....",
        "*OOOOOOOOO*.....",
        "*OOOOOOOO*......",
        "*OOOOOOO*.......",
        "*OOOOOOO*.......",
        "*OOOOOOOO*......",
        "*OOOO**OOO*.....",
        "*OOO*..*OOO*....",
        "*OO*....*OOO*...",
        "*O*......*OOO*..",
        "**........*OOO*.",
        "*..........*OOO*",
        "............*OO*",
        ".............***"
       */ 
       
        /* kusaOSオリジナルのマウスカーソル */

    /* 
        "*...............",
        "*O*.............",
        "*OO*............",
        "*OOO*...........",
        "*OOOO*..........",
        "*OOOOO*.........",
        "*OOOOOO*........",
        "*OOOOOOO*.......",
        "*OOOOOOOO*......",
        "***.OO*.*.......",
        "...*OO*.........",
        "....*OO*........",
        "....*OO*........",
        ".....*OO*.......",
        ".....*OO*.......",
        "......**........"
    */

        /* デバック用 */

        "O..............O",
        "*O*.............",
        "*OO*............",
        "*OOO*...........",
        "*OOOO*..........",
        "*OOOOO*.........",
        "*OOOOOO*........",
        "*OOOOOOO*.......",
        "*OOOOOOOO*......",
        "***.OO*.*.......",
        "...*OO*.........",
        "....*OO*........",
        "....*OO*........",
        ".....*OO*.......",
        ".....*OO*.......",
        "O.....**.......O"
    };

    int x, y;

    /* 各記号を色に置き換える */
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            /* y * 16 はy軸の一行にはx軸が16列あり、
            つまりy一個の行の中にx16個入っているために、
            yに16を掛けている。 */
            if (cursor[y][x] == '*') {
                mouse[y * 16 + x] = COL8_000000;
            }
            if (cursor[y][x] == 'O') {
                mouse[y * 16 + x] = COL8_FFFFFF;
            }
            if (cursor[y][x] == '.') {
                mouse[y * 16 + x] = bc;
            }
        }
    }
    return;
}

/* mcorsor[]のデータをvram[]に書き込む */
/*引数は、
   vram = vramのメモリ番地
   vxsize = 画面のX軸のサイズ
   pxsize = マウスポインタのX軸のサイズ 
   pysize = マウスポインタのY軸のサイズ
   px0 = マウスポインタのX軸の開始位置
   py0 = マウスポインタのY軸の開始位置
   buf = マウスポインタの色のデータ
   bxsize = マウスポインタのX軸のサイズ
*/
void putblock8_8(char *vram, int vxsize, int pxsize,
    int pysize, int px0, int py0, char *buf, int bxsize)
{
    int x, y;
    for (y = 0; y < pysize; y++) {
        for (x = 0; x < pxsize; x++) {
            /* init_mouse_cursor8で作ったmouse[]のそれぞれのデータを
            buf[]としてvram[]に入れていっている。 */
            vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
        }
    }
    return;
}
