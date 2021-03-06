/* asmhead.nasのブート情報を入れる構造体 */
struct BOOTINFO {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
};

#define ADR_BOOTINFO    0x00000ff0      /* BOOTINFOの開始メモリ番地 */

/* naskfunc.nas */
void io_hlt(void);
void io_cli(void);
void io_sti(void);
int io_in8(int port);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_stihlt(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
void asm_inthandler21(void);
void asm_inthandler27(void);
void asm_inthandler2c(void);


/* graphic.c */
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


/* dsctbl.c */
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

#define ADR_IDT         0x0026f800      /* IDTの開始メモリ番地 */
#define LIMIT_IDT       0x000007ff      /* IDTのリミット値 */
#define ADR_GDT         0x00270000      /* GDTの開始メモリ番地 */
#define LIMIT_GDT       0x0000ffff      /* GDTのリミット値 */
#define ADR_BOTPAK      0x00280000      /* bootpack.cの開始アドレス */
#define LIMIT_BOTPAK    0x0007ffff
#define AR_DATA32_RW    0x4092
#define AR_CODE32_ER    0x409a
#define AR_INTGATE32    0x008e

/* int.c */
struct KEYBUF {
    unsigned char data[32];
    int next;
};

void init_pic(void);
void inthandler21(int *esp);
void inthandler27(int *esp);
void inthandler2c(int *esp);

#define PIC0_ICW1        0x0020
#define PIC0_OCW2        0x0020
#define PIC0_IMR         0x0021
#define PIC0_ICW2        0x0021
#define PIC0_ICW3        0x0021
#define PIC0_ICW4        0x0021
#define PIC1_ICW1        0x00a0
#define PIC1_OCW2        0x00a0
#define PIC1_IMR         0x00a1
#define PIC1_ICW2        0x00a1
#define PIC1_ICW3        0x00a1
#define PIC1_ICW4        0x00a1

/* bootpack.c */
extern char bc;     /* 背景色 */
