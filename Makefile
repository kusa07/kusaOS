# 変数設定
OSNAME   = kusaOS

TOOLPATH = ../z_tools/
INCPATH  = $(TOOLPATH)/haribote/
RULEFILE = $(INCPATH)haribote.rul
MAKE     = $(TOOLPATH)make.exe -r
NASK	 = $(TOOLPATH)nask.exe
CC1		 = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
EDIMG	 = $(TOOLPATH)edimg.exe
IMGTOL	 = $(TOOLPATH)imgtol.com
COPY	 = copy
DEL		 = del

# デフォルト動作

default :
				$(MAKE) img

# ファイル生成規則

# nask → バイナリファイル(.bin)
ipl10.bin :		ipl10.nas Makefile
				$(NASK) ipl10.nas ipl10.bin ipl10.lst

# nask → バイナリファイル(.bin)
asmhead.bin :	asmhead.nas Makefile
				$(NASK) asmhead.nas asmhead.bin asmhead.lst

# C言語(.c) → GNUアセンブラ(.gas)
bootpack.gas :	bootpack.c Makefile
				$(CC1) -o bootpack.gas bootpack.c

# GNUアセンブラ(.gas) → nask(.nas)
bootpack.nas :	bootpack.gas Makefile
				$(GAS2NASK) bootpack.gas bootpack.nas

# nask(.nas) → オブジェクトファイル(.obj)
bootpack.obj :	bootpack.nas Makefile
				$(NASK) bootpack.nas bootpack.obj bootpack.lst

naskfunc.obj :	naskfunc.nas Makefile
				$(NASK) naskfunc.nas naskfunc.obj naskfunc.lst

# 必要なオブジェクトファイルを全てくっつけてbimファイル(binary image 著者オリジナル2進数イメージファイル)　3MB(3MB*1024=3072K)+64KB=3136KB
bootpack.bim :	bootpack.obj naskfunc.obj Makefile
				$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
					bootpack.obj naskfunc.obj

bootpack.hrb :	bootpack.bim Makefile
				$(BIM2HRB) bootpack.bim bootpack.hrb 0

kusaOS.sys :	asmhead.bin bootpack.hrb Makefile
				copy /B asmhead.bin+bootpack.hrb kusaOS.sys

kusaOS.img :	ipl10.bin	kusaOS.sys Makefile
				$(EDIMG) imgin:../z_tools/fdimg0at.tek \
					wbinimg src:ipl10.bin len:512 from:0 to:0 \
					copy from:kusaOS.sys to:@: \
					imgout:kusaOS.img

# コマンド

asm :
				$(MAKE) ipl10.bin

img :
				$(MAKE) kusaOS.img

run :		
				$(MAKE) img
				$(COPY) kusaOS.img ..\z_tools\qemu\fdimage0.bin
				$(MAKE) -C ../z_tools/qemu

install :
				$(MAKE) img
				$(IMGTOL) w a: kusaOS.img

clean :
				$(DEL)  *.bin
				$(DEL)  *.lst
				$(DEL)  *.gas
				$(DEL)  *.obj
				$(DEL)  bootpack.nas
				$(DEL)  bootpack.map
				$(DEL)  bootpack.bim
				$(DEL)  bootpack.hrb
				$(DEL) 	kusaOS.sys

src_only :
				$(MAKE) clean
				$(DEL)  kusaOS.img