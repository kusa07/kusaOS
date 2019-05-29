/* 他のファイルで作った関数がある事をCコンパイラに教える(関数宣言) */
/* 関数宣言しているので、naskfunc.nasの関数が実行できる */

void io_hlt(void);


void HariMain(void)
{

    int i;
    char *p;    /* BYTE[...]用の番地 */
    //char * i;

    for (i = 0xa0000; i <= 0xaffff; i++) {
        p = (char *) i;  /* 番地を代入 */
        *p = i & 0x0f;  /* write_mem8(i, i & 0x0f);の代わりになる */
        //*((char * ) i) = i & 0x0f;
        //*i = (int) i & 0x0f;    /* 本とは違う実装方法を考えた */
    }
    
    /* 処理が終わったら無限HLT */
    for (;;) {
        io_hlt();   
    }
}
