/* 他のファイルで作った関数がある事をCコンパイラに教える(関数宣言) */
/* 関数宣言しているので、naskfunc.nasの関数が実行できる */

/* naskfunc.cで使う関数宣言 */
void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

/* bootpack.cで使う関数の宣言 */
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen(char *vram, int x, int y);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);

/* パレットカラーの定数宣言 */
#define COL8_000000     0   /* 0:黒 */
#define COL8_FF0000     1   /* 1:明るい赤 */
#define COL8_00FF00     2   /* 2:明るい緑 */
#define COL8_FFFF00     3   /* 3:明るい黄色 */
#define COL8_0000FF     4   /* 4:明るい青 */
#define COL8_FF00FF     5   /* 5:明るい紫 */
#define COL8_00FFFF     6   /* 6:明るい水色 */
#define COL8_FFFFFF     7   /* 7:白 */
#define COL8_C6C6C6     8   /* 8:明るい灰色 */
#define COL8_840000     9   /* 9:暗い赤 */
#define COL8_008400     10  /* 10:暗い緑 */
#define COL8_848400     11  /* 11:暗い黄色 */
#define COL8_000084     12  /* 12:暗い青 */
#define COL8_840084     13  /* 13:暗い紫 */
#define COL8_008484     14  /* 14:暗い水色 */
#define COL8_848484     15  /* 15:暗い灰色 */

/* 構造体でBOOT情報をまとめる */
struct BOOTINFO {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
};


void HariMain(void)
{
    struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;     /* asmhead.nasと同じ先頭のメモリ番地を指定している。これは同時にcylsのメモリ番地を示している事にもなる。 */
    extern char hankaku[4096];

    init_palette(); /* パレットを設定 */

    /* asmhead.nasで宣言したメモリ番地（BOOTINFO）の先頭番地を読み込んで、
       後に続くメモリ番地の配置は構造体BOOTINFOと同じ間隔（構造体で宣言した型分のバイトが予約されるため）なので、
       そのまま指し示されて使える。*/

    /* 直接引数に構造体のメンバを示すための矢印記法を使っている */
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);    /* デスクトップの描画 */

    /* 文字表示　ABC 123 */
    //putfont8(binfo->vram, binfo->scrnx,  8, 8, COL8_FFFFFF, hankaku + 0x41 * 16);     /* C言語では'A'は0x41に自動的に変更されるので、逆に0x41を指定しても'A'フォントが表示される */
    putfont8(binfo->vram, binfo->scrnx,  8, 8, COL8_FFFFFF, hankaku + 'A' * 16);
    putfont8(binfo->vram, binfo->scrnx, 16, 8, COL8_FFFFFF, hankaku + 'B' * 16);
    putfont8(binfo->vram, binfo->scrnx, 24, 8, COL8_FFFFFF, hankaku + 'C' * 16);
    putfont8(binfo->vram, binfo->scrnx, 40, 8, COL8_FFFFFF, hankaku + '1' * 16);
    putfont8(binfo->vram, binfo->scrnx, 48, 8, COL8_FFFFFF, hankaku + '2' * 16);
    putfont8(binfo->vram, binfo->scrnx, 56, 8, COL8_FFFFFF, hankaku + '3' * 16);

    /* 処理が終わったら無限HLT */
    for (;;) {
        io_hlt();   
    }
}

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

    for(i = start; i <= end; i++){

        /* 0x03c9にRGBの順に書き込む */
        io_out8(0x03c9, rgb[0] / 4); 
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb +=3;
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
void init_screen(char *vram, int x, int y )
{
    /* boxfill8の引数は、（ビデオアクセス用メモリ番地、画面のX軸の大きさ、色、X軸の開始位置、Y軸の開始位置、X軸の終了位置、Y軸の終了位置） */
    boxfill8(vram, x, COL8_000084,      0,        0, x -  1, y - 29);    /* 背景 */

    boxfill8(vram, x, COL8_008484,      0,   y - 28, x -  1, y - 28);    /* タスクバー 上部横線 その１ */
    boxfill8(vram, x, COL8_848400,      0,   y - 27, x -  1, y - 27);    /* タスクバー 上部横線 その２ */
    boxfill8(vram, x, COL8_000000,      0,   y - 26, x -  1, y -  1);    /* タスクバー */

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
