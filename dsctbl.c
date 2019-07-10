/* GDTやIDTなどの、descriptor table 関係 */

#include "bootpack.h"

void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

void init_gdtidt(void)
{
    struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;     /* GDTの開始位置メモリ番地を指定 */
    struct GATE_DESCRIPTOR    *idt = (struct GATE_DESCRIPTOR    *) ADR_IDT;     /* IDTの開始位置メモリ番地を指定 */
    int i;

    /* GDTの初期化 */
    for (i = 0; i <= LIMIT_GDT / 8; i++) {
        set_segmdesc(gdt + i, 0, 0, 0);
    }
    set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, AR_DATA32_RW);
    set_segmdesc(gdt + 2, LIMIT_BOTPAK, ADR_BOTPAK, AR_CODE32_ER);
    load_gdtr(LIMIT_GDT, ADR_GDT);

    /* IDTの初期化 */
    for (i = 0; i <= LIMIT_IDT / 8; i++) {
        set_gatedesc(idt + i, 0, 0, 0);
    }
    load_idtr(LIMIT_IDT, ADR_IDT);

    /* IDTの設定 */
    set_gatedesc(idt + 0x21, (int) asm_inthandler21, 2 * 8, AR_INTGATE32);
    set_gatedesc(idt + 0x27, (int) asm_inthandler27, 2 * 8, AR_INTGATE32);
    set_gatedesc(idt + 0x2c, (int) asm_inthandler2c, 2 * 8, AR_INTGATE32);

    return;
}

/*Segment Descriptorをセットするための関数 */
/* 引数は、
    *sd   = セグメントディスクリプタのメモリ番地
    limit = セグメントディスクリプタのリミット値
    base  = セグメントディスクリプタのベースアドレス値
    ar    = アクセス権のフラグコントロールのための値
*/
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar)
{
    if (limit > 0xfffff) {  /* limitが20bitより大きいかを確認（大きければG-bitを立てる) */
        ar |= 0x8000;       /* G-bitのフラグを立てる(ページ単位が4KBになる) */
        limit /= 0x1000;    /* G-bitが立つと1bitが4KBで解釈されるため、4KB(4096 → 0x1000)で割る */
    }
    sd->limit_low       = limit & 0xffff;       /* 16bitを＆演算子で取り出している */
    sd->base_low        = base & 0xffff;        /* 16bitを＆演算子で取り出している */
    sd->base_mid        = (base >> 16) & 0xff;  /* base_lowの隣の8bitを入れるため、16bitシフトして&演算子で8bit取り出している */
    //sd->base_mid        = base & 0xff0000;    /* この形でも同じこと */
    sd->access_right    = ar & 0xff;            /* 16bitを＆演算子で取り出している */
    sd->limit_high      = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);  /* limit_highは下位4bitがlimit値の上位4bitを入れ、上位4bitには拡張アクセス権を入れている。 */
    sd ->base_high      = (base >> 24) & 0xff;  /* base_low(16bit)とbase_mid(8bit)をすでに代入済みなので、その分の24bitを右シフトして、残りの8bitを入れる。 */
    //sd ->base_high      = base & 0xff000000;  /* この形でも同じこと */

    return;
}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar)
{
    gd->offset_low      = offset & 0xffff;
    gd->selector        = selector;
    gd->dw_count        = (ar >> 8) & 0xff;
    gd->access_right    = ar & 0xff;
    gd->offset_high     = (offset >> 16) & 0xffff;

    return;
}