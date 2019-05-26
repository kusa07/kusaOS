; kusa-os
; TAB=4

CYLS	EQU		10				; �萔�錾�BCYLS=10�Ƃ����Ӗ��BCylinders�̗���CYLS�Ƃ����͗l

		ORG		0x7c00			; �v���O�����̊J�n�_�̃������Ԓn���w�肷��B�^�����߂�CPU�ł̏����͉������Ȃ��B

; �ȉ��͕W���I��FAT12�t�H�[�}�b�g�t���b�s�[�f�B�X�N�̂��߂̋L�q
		JMP		entry			; ��������entry���x���փW�����v
		DB		0x90			; NOP���߁iNO Operation)�������Ȃ�
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
		MOV		SS,AX			; AX��������Ƃ������Ƃ́A�C�R�[��0�������鎖�Ɠ����B
		MOV		SP,0x7c00		; �X�^�b�N�|�C���^��ORG���߂Ŏw�肵��IPL�̊J�n�ʒu����
		MOV		DS,AX
		;MOV		ES,AX		; �ˑR�T�C�����g�폜����Ă���

; �f�B�X�N��ǂݍ���
		MOV		AX,0x0820		; 0x8000~0x81ff�܂ł�512�o�C�g�͌�Ńu�[�g�Z�N�^�̓��e�������̂ŁA0x8200���g���B
		MOV		ES,AX			; ES�ɂ͒��ڒl�������Ȃ��̂�AX����ē���Ă���BES�Z�O�����g�Ńf�B�X�N�̓ǂݍ��ރZ�O�����g�A�h���X���w��ł���B
		MOV		CH,0			; �V�����_0
		MOV		DH,0			; �w�b�h0
		MOV		CL,2			; �Z�N�^2 ����Ńu�[�g�Z�N�^�̎����ǂݍ��߂�

; 1�V�����_�̕\�Ɨ������ꂼ��18�Z�N�^�ǂ�ŁA�����CYLS���J��Ԃ����߂̃��[�v
readloop:
		MOV		SI,0			; ���s�񐔂𐔂��郌�W�X�^��������

; �f�B�X�N��BIOS�@�\�œǂݍ��ނ��߂̒l�̃Z�b�g�Ƌ@�\�Ăяo��
retry:
		MOV		AH,0x02			; �f�B�X�N�ǂݍ���: AH=0x02
		MOV		AL,1			; ��������Z�N�^: 1�Z�N�^
		MOV		BX,0			; �o�b�t�@�A�h���X�̃I�t�Z�b�g�A�h���X(�ׂ����������Ԓn)���w��
		MOV		DL,0x00			; �h���C�u�ԍ�: A�h���C�u
		INT		0x13			; �f�B�X�N�֘A��BIOS�@�\�Ăяo��
		JNC		next			; JNC���߁uJamp if No Carry�v��INT0x13�ŃG���[���Ȃ���΃L�����[�t���O��0�Ȃ̂ŁA0��������next�փW�����v����

;�G���[���������̃��g���C�����B5��G���[��������G���[���b�Z�[�W�ɔ�ԁB
		ADD		SI,1			; �G���[�񐔂��{�P����
		CMP		SI,5			; CMP���߁uCoMPare�v��r���߁BSI - 5 ���s���āA���ʂ��X�e�[�^�X�t���O�ɃZ�b�g����B
		JAE		error			; JAE���߁uJamp if Above or Equal�v�Œ��O��CMP���߂ŁASI��5�ȏ�Ȃ�W�����v�i>=�j
		MOV		AH,0x00			; �V�X�e�����Z�b�g���s���ɂ́AAH��0x00���Z�b�g����iOS-wiki���j
		MOV		DL,0x00			; �h���C�u�ԍ�: A�h���C�u�B
		INT		0x13			; �V�X�e�����Z�b�g�̂��߂�BIOS�@�\�Ăяo��
		JMP		retry			; �������W�����v

; 18�Z�N�^�ǂݍ���
next:
		MOV		AX,ES			; ES�Z�O�����g�ɓǂ݂ɍs���Z�O�����g(0x0820)�������Ă�����̂ŁA�����ɒl�𑫂����߈�UAX�Ɉڂ�
        ADD		AX,0x0020		; 0x0020�𑫂��B���ۂ� �~ 16�����0x0200(512�o�C�g)��0x8200�ɑ������̂ŁA0x8400�ɂȂ�FDD��1�Z�N�^���������Ԓn����������ꂽ�B
		MOV		ES,AX			; ���������ʂ�ES�Z�O�����g�ɖ߂����ŁA�Z�O�����g��i�߂���B
		ADD		CL,1			; ���݂̃Z�N�^��+�P����
		CMP		CL,18			; �Z�N�^��18�܂œ��B���邩�� CL - 18 ���s���Ċm�F����B
		JBE		readloop		; JBE���߁uJamp if Below Equal�v�Œ��O��CMP���߂�CL��18�ȉ��Ȃ�W�����v(<=)

; �w�b�h�̗���(�w�b�h1)���g���ăZ�N�^1~18�܂œǂ݂ɂ����B�w�b�h0�̃Z�N�^18�܂œǂݏI���Ǝ��͓����V�����_�̗���ǂ݂ɂ����B 
		MOV		CL,1			; �Z�N�^1 �Z�N�^1�ɖ߂��B
		ADD		DH,1			; �w�b�h1 �w�b�h0��1������
		CMP		DH,2			; CMP���߁uCoMPare�v��r���߁BDH - 2 ���s���āA���ʂ��X�e�[�^�X�t���O�ɃZ�b�g����B 
		JB		readloop		; JB���߁uJamp if Below�v�Œ��O��CMP���߂�DH��2��菬������΃W�����v�i<)

; CYLS�������ǂ݂ɍs���B
		MOV		DH,0			; �w�b�h��0�ɖ߂��ĕ\�̃w�b�h�ɐ؂�ւ���B
		ADD		CH,1			; ���݂̃V�����_�ԍ���1������B
		CMP		CH,CYLS			; �V�����_���V�����_�萔�ɒB���Ă��邩��r
		JB		readloop		; CH��CYLS�����Ȃ�readloop�փW�����v

; �ǂݍ��݂��I�����kusaOS.sys�փW�����v�I
		MOV		[0x0ff0],CH		; �Z�N�^�̒l��0x0ff0�փ������āA�ǂ��܂œǂݍ��񂾂��L�^����
		JMP		0xc200			; kusaOS.sys�̂��郁�����Ԓn�B�f�B�X�N�C���[�W��0x4200�ȍ~��kusaOS.sys�����邽�߁A0x8000 + 0x4200 = 0xc200����n�܂�悤�ɂ����B

; �ȉ��A�G���[���b�Z�[�W�̃Z�N�V����
error:
		MOV		SI,msg			; msg���x���̃������Ԓn���\�[�X�C���f�b�N�X�ɃZ�b�g

; ���b�Z�[�W���ꕶ���Â\�����A���������������[�v����
putloop:
		MOV		AL,[SI]			; msg�̒��g�̐擪��AL�ɑ��
		ADD		SI,1			; �\�[�X�C���f�b�N�X��1�𑫂���msg�̎��̕����̃������Ԓn�������悤�ɂ���
		CMP		AL,0			; msg���Ō�܂ōs�������ɂȂ�̂ŁA0���ǂ�����r���Ă���
		JE		fin				; ��r�̌���0�Ȃ��fin�F���x���փW�����v
		MOV		AH,0x0e			; ��r��0�łȂ���Α����B�ꕶ���\���������̂Ńr�f�I���荞�݂̈ꕶ���\���̂��߂Ƀ��W�X�^���Z�b�g
		MOV		BX,15			; BH��0�ł悭�ABL�̓J���[�R�[�h���w��BFFFF�͔��Ȃ̂�15�������0x0000FFFF�ƂȂ�B
		INT		0x10			; �r�f�I�֘A�̃\�t�g�E�F�A���荞�݂��Ăяo��
		JMP		putloop			; �ꕶ���Â\���Ȃ̂ŁAputloop�ɖ߂��ČJ��Ԃ��B�i0�Ȃ�fin�ɔ�����j

; �G���[���b�Z�[�W��\�����I�������I���
fin:
		HLT						; ��������܂�CPU��~
		JMP		fin				; fin���x���֔�Ԃ̂Ŏ����������[�v

; ���b�Z�[�W����
msg:
		DB		0x0a, 0x0a		; ���s��2��
		DB		"load error"
		DB		0x0a			; ���s
		DB		0

		RESB	0x7dfe-$		; 0x07dfe�܂ł�0x00�Ŗ��߂閽��

		DB		0x55, 0xaa
