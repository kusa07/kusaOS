/* ���̃t�@�C���ō�����֐������鎖��C�R���p�C���ɋ�����(�֐��錾) */
/* �֐��錾���Ă���̂ŁAnaskfunc.nas�̊֐������s�ł��� */

void io_hlt(void);

void HariMain(void)
{
    int i;
    char *p;    /* BYTE[...]�p�̔Ԓn �|�C���^�ϐ����Ԃ�4�o�C�g */

    /* p�ɒl�Ƃ���0x0000���������B */
    p = (char *) 0xa0000;   /* �l�𕶎��^�|�C���^�ɃL���X�g�ϊ����āA�|�C���^�ϐ��ɑ�� */

    for (i = 0; i <= 0xffff; i++) {
        p[i] = i & 0x0f;        /* �|�C���^�͔z��Ɠ����ŕ\�����@�̈Ⴂ */
        // i[p] = i & 0x0f;    /* �z��\���͋t�ɂ��Ă��������ʂƂȂ�B�z��͕\���I */
    }
    
    /* �������I������疳��HLT */
    for (;;) {
        io_hlt();   
    }
}