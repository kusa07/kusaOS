/* 他のファイルで作った関数がある事をCコンパイラに教える(関数宣言) */
/* 関数宣言しているので、naskfunc.nasの関数が実行できる */

void io_hlt(void);
void write_mem8(int addr, int data);


void HariMain(void)
{

    int i;

    for (i = 0xa0000; i <= 0xaffff; i++) {
        write_mem8(i, 15); /* メモリに直接データを書き込む */
    }
    
    /* 処理が終わったら無限HLT */
    for (;;) {
        io_hlt();   
    }
}
