#include<stdio.h>

/*フロッピーのシリンダ、ヘッド、セクタがメモリ番地のどこに読みかまれていくかを計算して出力する*/

int main(void){
    int c,h,s,i;
    int c_end = 10;
    int h_end = 1;
    int s_end = 18;
    int start = 32768;
    int mem = 0;
    int byte = 512;
    int kb = 1024;
    i = 0;

    printf("C,H,S,mem,mem_byte,mem_KB\n");
    for(c = 0; c < c_end; c++){
        for(h = 0; h <= h_end; h++){
            for(s = 1; s <= s_end; s++){
                mem = start + byte * i;
                printf("C%2d,H%d,S%2d,0x%08x,%d,%d\n", c, h, s, mem, byte * i, byte * i / kb);
                i++;
            }
        }
    }
    c = 10;
    h = 0;
    s = 1;
    mem = start + byte * i;

    printf("C%2d,H%d,S%2d,0x%08x,%d,%d\n", c, h, s, mem, byte * i, byte * i / kb);

    return 0;
}



