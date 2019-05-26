#変数設定
TOOLPATH = ../z_tools/
MAKE	 = $(TOOLPATH)make.exe -r
NASK	 = $(TOOLPATH)nask.exe
EDIMG	 = $(TOOLPATH)edimg.exe
IMGTOL	 = $(TOOLPATH)imgtol.com
COPY	 = copy
DEL		 = del

#デフォルト動作

default :
				$(MAKE) img

#ファイル生成規則

ipl10.bin :		ipl10.nas Makefile
				$(NASK) ipl10.nas ipl10.bin ipl10.lst

kusaOS.sys :	kusaOS.nas Makefile
				$(NASK) kusaOS.nas kusaOS.sys kusaOS.lst

kusaOS.img :	ipl10.bin	kusaOS.sys Makefile
				$(EDIMG) imgin:../z_tools/fdimg0at.tek \
					wbinimg src:ipl10.bin len:512 from:0 to:0 \
					copy from:kusaOS.sys to:@: \
					imgout:kusaOS.img

#コマンド

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
				$(DEL) ipl10.bin
				$(DEL) ipl10.lst
				$(DEL) kusaOS.sys
				$(DEL) kusaOS.lst


src_only :
				$(MAKE) clean
				$(DEL) kusaOS.img