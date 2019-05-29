/* 他のファイルで作った関数がある事をCコンパイラに教える(関数宣言) */
/* 関数宣言しているので、naskfunc.nasの関数が実行できる */

void io_hlt(void);

void HariMain(void)
{
    int i;
    char *p;    /* BYTE[...]用の番地 ポインタ変数事態は4バイト */

    /* pに値として0x0000が入った。 */
    p = (char *) 0xa0000;   /* 値を文字型ポインタにキャスト変換して、ポインタ変数に代入 */

    for (i = 0; i <= 0xffff; i++) {
        *(p + i) = i & 0x0f;
    }
    
    /* 処理が終わったら無限HLT */
    for (;;) {
        io_hlt();   
    }
}
