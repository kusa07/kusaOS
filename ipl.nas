; kusa-os
; TAB=4

		ORG		0x7c00			;�v���O�����̊J�n�_�̃������Ԓn���w�肷��

; �ȉ��͕W���I��FAT12�t�H�[�}�b�g�t���b�s�[�f�B�X�N�̂��߂̋L�q

		JMP		entry			
		DB		0x90
		DB		"kusa-IPL"		; �u�[�g�Z�N�^�̖��O�����R�ɏ����Ă悢�i8�o�C�g�j
		DW		512				; 1�Z�N�^�̑傫���i512�ɂ��Ȃ���΂����Ȃ��j
		DB		1				; �N���X�^�̑傫���i1�Z�N�^�ɂ��Ȃ���΂����Ȃ��j
		DW		1				; FAT���ǂ�����n�܂邩�i���ʂ�1�Z�N�^�ڂ���ɂ���j
		DB		2				; FAT�̌��i2�ɂ��Ȃ���΂����Ȃ��j
		DW		224				; ���[�g�f�B���N�g���̈�̑傫���i���ʂ�224�G���g���ɂ���j
		DW		2880			; ���̃h���C�u�̑傫���i2880�Z�N�^�ɂ��Ȃ���΂����Ȃ��j
		DB		0xf0			; ���f�B�A�̃^�C�v�i0xf0�ɂ��Ȃ���΂����Ȃ��j
		DW		9				; FAT�̈�̒����i9�Z�N�^�ɂ��Ȃ���΂����Ȃ��j
		DW		18				; 1�g���b�N�ɂ����̃Z�N�^�����邩�i18�ɂ��Ȃ���΂����Ȃ��j
		DW		2				; �w�b�h�̐��i2�ɂ��Ȃ���΂����Ȃ��j
		DD		0				; �p�[�e�B�V�������g���ĂȂ��̂ł����͕K��0
		DD		2880			; ���̃h���C�u�傫����������x����
		DB		0,0,0x29		; �悭�킩��Ȃ����ǂ��̒l�ɂ��Ă����Ƃ����炵��
		DD		0xffffffff		; ���Ԃ�{�����[���V���A���ԍ�
		DB		"kusa-OS    "	; �f�B�X�N�̖��O�i11�o�C�g�j
		DB		"FAT12   "		; �t�H�[�}�b�g�̖��O�i8�o�C�g�j
		RESB	18				; �Ƃ肠����18�o�C�g�����Ă���

; �v���O�����{��

entry:
		MOV		AX,0			; ���W�X�^������
		MOV		SS,AX			;AX��������Ƃ������Ƃ́A�C�R�[��0�������鎖�Ɠ����B
		MOV		SP,0x7c00		;�X�^�b�N�|�C���^��ORG���߂Ŏw�肵��IPL�̊J�n�ʒu����
		MOV		DS,AX
		MOV		ES,AX

		MOV		SI,msg			;�\�[�X�C���f�b�N�X��msg:���x���̃������Ԓn������
putloop:
		MOV		AL,[SI]			;msg�̒��g�̐擪��AL�ɑ��
		ADD		SI,1			;�\�[�X�C���f�b�N�X��1�𑫂���msg�̎��̕����̃������Ԓn�������悤�ɂ���
		CMP		AL,0			;msg���Ō�܂ōs�������ɂȂ�̂ŁA0���ǂ�����r���Ă���
		JE		fin				;��r�̌���0�Ȃ��fin�F���x���փW�����v
		MOV		AH,0x0e			;��r��0�łȂ���Α����B�ꕶ���\���������̂Ńr�f�I���荞�݂̈ꕶ���\���̂��߂Ƀ��W�X�^���Z�b�g
		MOV		BX,15			;BH��0�ł悭�ABL�̓J���[�R�[�h���w��BFFFF�͔��Ȃ̂�15�������0x0000FFFF�ƂȂ�B
		INT		0x10			;�r�f�I�֘A�̃\�t�g�E�F�A���荞�݂��Ăяo��
		JMP		putloop			;�ꕶ���Â\���Ȃ̂ŁAputloop�ɖ߂��ČJ��Ԃ��B�i0�Ȃ�fin�ɔ�����j
fin:
		HLT						;CPU��ҋ@��Ԃɂ���
		JMP		fin

; ���b�Z�[�W����
msg:
		DB		0x0a, 0x0a		; ���s��2��
		DB		"hello, I'm kusaOS!"
		DB		0x0a			; ���s
		DB		0

		RESB	0x7dfe-$			; 0x07dfe�܂ł�0x00�Ŗ��߂閽��

		DB		0x55, 0xaa

; �ȉ��̓u�[�g�Z�N�^�ȊO�̕����̋L�q

		;DB		0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
		;RESB	4600
		;DB		0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
		;RESB	1469432