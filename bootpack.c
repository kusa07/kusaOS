/* 他のファイルで作った関数がある事をCコンパイラに教える */


void io_hlt(void);


void HariMain(void){

fin:

    io_hlt();   /* 関数宣言しているので、naskfunc.nasの関数が実行できる */

    goto fin;

}
