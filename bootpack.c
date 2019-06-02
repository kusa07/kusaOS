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
void set_paletter(int start, int end, unsigned char *rgb);


void HariMain(void)
{
    int i;
    char *p;    /* BYTE[...]用の番地 ポインタ変数事態は4バイト */

    init_palette(); /* パレットを設定 */

    p = (char *) 0xa0000;   /* 値を文字型ポインタにキャスト変換して、ポインタ変数に代入 */

    for (i = 0; i <= 0xffff; i++) {
        p[i] = i & 0x0f;        /* ポインタは配列と同じで表現方法の違い */
    }
    
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

    set_palette(0, 15, table_rgb);
    return;
}

void set_palette(int start, int end, unsigned char *rgb)
{
    int i, eflags;

    eflags = io_load_eflags();
    io_cli();

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
    io_store_eflags(eflags);
    return;
}
