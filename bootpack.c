#include <stdio.h>

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
void init_screen8(char *vram, int x, int y, char bc );
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
    int pysize, int px0, int py0, char *buf, int bxsize);

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

/* 構造体でGDT(Global (segment) Descriptor Table)をまとめる */
struct SEGMENT_DESCRIPTOR {
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
};

/* 構造体でIDT(Intrrupt Descriptor Table)をまとめる */
struct GATE_DESCRIPTOR {
    short offset_low, selector;
    char dw_count, access_right;
    short offset_high;
};

/* GDTとIDTで使う関数の宣言 */
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);



void HariMain(void)
{
    struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;     /* asmhead.nasと同じ先頭のメモリ番地を指定している。これは同時にcylsのメモリ番地を示している事にもなる。 */
    char s[40], mcursor[256];
    int mx, my;
    char bc;                    /* back-color */

    init_gdtidt();

    init_palette(); /* パレットを設定 */

    /* 背景色(壁紙) */
    bc = COL8_000084;   /* 暗い青 */

    /* asmhead.nasで宣言したメモリ番地（BOOTINFO）の先頭番地を読み込んで、
       後に続くメモリ番地の配置は構造体BOOTINFOと同じ間隔（構造体で宣言した型分のバイトが予約されるため）なので、
       そのまま指し示されて使える。*/

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

    /* 処理が終わったら無限HLT */
    for (;;) {
        io_hlt();   
    }
}
