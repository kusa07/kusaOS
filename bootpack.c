/* ���̃t�@�C���ō�����֐������鎖��C�R���p�C���ɋ�����(�֐��錾) */
/* �֐��錾���Ă���̂ŁAnaskfunc.nas�̊֐������s�ł��� */

void io_hlt(void);


void HariMain(void)
{

    int i;
    char *p;    /* BYTE[...]�p�̔Ԓn */
    //char * i;

    for (i = 0xa0000; i <= 0xaffff; i++) {
        p = (char *) i;  /* �Ԓn���� */
        *p = i & 0x0f;  /* write_mem8(i, i & 0x0f);�̑���ɂȂ� */
        //*((char * ) i) = i & 0x0f;
        //*i = (int) i & 0x0f;    /* �{�Ƃ͈Ⴄ�������@���l���� */
    }
    
    /* �������I������疳��HLT */
    for (;;) {
        io_hlt();   
    }
}
