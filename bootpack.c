/* ���̃t�@�C���ō�����֐������鎖��C�R���p�C���ɋ�����(�֐��錾) */
/* �֐��錾���Ă���̂ŁAnaskfunc.nas�̊֐������s�ł��� */

void io_hlt(void);
void write_mem8(int addr, int data);


void HariMain(void)
{

    int i;

    for (i = 0xa0000; i <= 0xaffff; i++) {
        write_mem8(i, 15); /* �������ɒ��ڃf�[�^���������� */
    }
    
    /* �������I������疳��HLT */
    for (;;) {
        io_hlt();   
    }
}
