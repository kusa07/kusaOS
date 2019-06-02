#include <stdio.h>

int main(void)
{
    int i,j;
    int x;      /* x軸 */
    int y;      /* y軸 */
    int mem;

    x = 32;
    y = 20;
    mem = 655360;      /* 0xa0000 */

    printf("x = %d y = %d\n",x, y);
    printf("※大きすぎるため1/10している\n");
    printf("\n");

    /* ドットで示す、縦横の長さイメージ */
    printf("■ドットで示す、縦横の長さイメージ\n");
    for (i = 0; i < y; i++) {
        for (j = 0; j < x; j++) {
            printf("・");
        }
        printf("\n");
    }

    printf("\n");

    /* 座標イメージ */
    printf("■座行イメージ\n");
    for (i = 0; i < y; i++) {
        for (j = 0; j < x; j++) {
            printf("|%2d,%2d", j, i);
        }
        printf("|\n");
    }

    printf("\n");

    /* メモリレイアウト */
    printf("■メモリレイアウト\n");
    printf("x = %d y = %d 開始アドレス = 0x%08x\n", x, y, mem);
    for (i = 0; i < y * 10; i++) {
        for (j = 0; j < x * 10; j++) {
            printf("(%3d,%3d) = 0x%08x\n", j, i, mem + j + i *320);
        }
    }

}