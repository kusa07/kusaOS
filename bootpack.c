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

void HariMain(void)
{
    int i;
    char *p;    /* BYTE[...]用の番地 ポインタ変数事態は4バイト */

    init_palette(); /* パレットを設定 */

    p = (char *) 0xa0000;   /* 値を文字型ポインタにキャスト変換して、ポインタ変数に代入 */

    /* 四角を三つ書く */
    /* 引数は、（ビデオアクセス用メモリ番地、画面のX軸の大きさ、色、X軸の開始位置、Y軸の開始位置、X軸の終了位置、Y軸の終了位置） */
    boxfill8(p, 320, COL8_FF0000, 20, 20, 120, 120);
    boxfill8(p, 320, COL8_00FF00, 70, 50, 170, 150);
    boxfill8(p, 320, COL8_0000FF, 120, 80, 220, 180);

    
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
/* 引数は、（ビデオアクセス用メモリ番地、画面のX軸の大きさ、色、X軸の開始位置、Y軸の開始位置、X軸の終了位置、Y軸の終了位置） */
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
