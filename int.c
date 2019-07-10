/* 割り込み関係 */

#include "bootpack.h"

void init_pic(void)
/* PIC初期化 */
{
    /*
        PIC(Programmable Interrupt Controller)
        IMR(Interrupt Mask Register)
        ICW(Initial Control Word)
    */

    /* 初期化中に割り込みが入るのを防ぐため、全ポートをマスクする */
    io_out8( PIC0_IMR, 0xff    );
    io_out8( PIC1_IMR, 0xff    );

    io_out8( PIC0_ICW1, 0x11   );   /* ICW4を設定するフラグを立てている */
    io_out8( PIC0_ICW2, 0x20   );   /* 割り込みベクタ番号で32以降を使えるように設定。IRQ0-7をINT0x20-0x27で受ける */
    io_out8( PIC0_ICW3, 1 << 2 );   /* IRQ2に繋がっている事から、フラグを2bit分右シフトしている。 */
    io_out8( PIC0_ICW4, 0x01   );   /* ノンバッファモード */

    io_out8( PIC1_ICW1, 0x11   );   /* ICW4を設定するフラグを立てている */
    io_out8( PIC1_ICW2, 0x28   );   /* 割り込みベクタ番号で32以降を使えるように設定。IRQ8-15をINT0x28-0x2fで受ける */
    io_out8( PIC1_ICW3, 2      );   /* マスターPICのIRQ2に繋がっている事を表すため、IRQ2に対応した2bit目のフラグを立てている */
    io_out8( PIC1_ICW4, 0x01   );   /* ノンバッファモード */

    io_out8( PIC0_IMR, 0xfb    );   /* スレーブPICが繋がっているIRQ2のみマスクを解除 */
    io_out8( PIC1_IMR, 0xff    );   /* 全ポートをマスク */

    return;
}

void inthandler21(int *esp)
/* PS/2キーボードからの割り込み */
/* キーボード割り込みがあった時に、その旨のメッセージを表示する */
{
    struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
    boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 21 (IRQ-1) : PS/2 keyboard");
    for (;;) {
        io_hlt();
    }
}

void inthandler2c(int *esp)
/* PS/2マウスからの割り込み */
/* マウスからの割り込みがあった時に、その旨のメッセージを表示する */
{
    struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
    boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 2C (IRQ-12) : PS/2 mouse");
    for (;;) {
        io_hlt();
    }
}

void inthandler27(int *esp)
/* PIC0からの不完全割り込み対策 */
{
    io_out8(PIC0_OCW2, 0x67);
    return;
}
