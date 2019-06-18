# 変数設定
OSNAME   = kusaOS

OBJS_BOOTPACK = bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj

TOOLPATH = ../z_tools/
INCPATH  = $(TOOLPATH)/haribote/
RULEFILE = $(INCPATH)haribote.rul

MAKE     = $(TOOLPATH)make.exe -r
NASK	 = $(TOOLPATH)nask.exe
CC1		 = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
MAKEFONT = $(TOOLPATH)makefont.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
EDIMG	 = $(TOOLPATH)edimg.exe
IMGTOL	 = $(TOOLPATH)imgtol.com
COPY	 = copy
DEL		 = del

# デフォルト動作

default :
				$(MAKE) img

# ファイル生成規則
#(生成規則になければ、一般規則の順で確認される)
# 一般規則を適応したくない場合には、生成規則の方に書けば、こちらが優先される。

# nask → バイナリファイル(.bin)
ipl10.bin :		ipl10.nas Makefile
				$(NASK) ipl10.nas ipl10.bin ipl10.lst

# nask → バイナリファイル(.bin)
asmhead.bin :	asmhead.nas Makefile
				$(NASK) asmhead.nas asmhead.bin asmhead.lst

# フォントファイル
# フォント（.txt) → バイナリファイル(.bin)
hankaku.bin :	hankaku.txt Makefile
				$(MAKEFONT) hankaku.txt hankaku.bin

# バイナリファイル(.bin) →　オブジェクトファイル(.obj)
hankaku.obj :	hankaku.bin Makefile
				$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

# 必要なオブジェクトファイルを全てくっつけてbimファイル(binary image 著者オリジナル2進数イメージファイル)　3MB(3MB*1024=3072K)+64KB=3136KB
# 3MB(3072KB) + 64KB = 3136KB
bootpack.bim :	$(OBJS_BOOTPACK) Makefile
				$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
					$(OBJS_BOOTPACK)

# バイナリイメージ(bim) → hrbファイル
bootpack.hrb :	bootpack.bim Makefile
				$(BIM2HRB) bootpack.bim bootpack.hrb 0

# asmhead + bootpack + kusaOS
$(OSNAME).sys :	asmhead.bin bootpack.hrb Makefile
				copy /B asmhead.bin+bootpack.hrb $(OSNAME).sys

# OSイメージファイル
$(OSNAME).img :	ipl10.bin	$(OSNAME).sys Makefile
				$(EDIMG) imgin:../z_tools/fdimg0at.tek \
					wbinimg src:ipl10.bin len:512 from:0 to:0 \
					copy from:$(OSNAME).sys to:@: \
					imgout:$(OSNAME).img

# 一般規則
#(生成規則になければ、一般規則の順で確認される)

%.gas :			%.c Makefile
				$(CC1) -o $*.gas $*.c

%.nas :			%.gas Makefile
				$(GAS2NASK) $*.gas $*.nas

%.obj :			%.nas Makefile
				$(NASK) $*.nas $*.obj $*.lst

# コマンド

# 各ファイル生成コマンド
ipl10 :
				$(MAKE) ipl10.bin

asmhead :
				$(MAKE) asmhead.bin

bootpack :
				$(MAKE) bootpack.obj

naskfunc :
				$(MAKE) naskfunc.obj

hankaku :
				$(MAKE) hankaku.obj

graphic :		
				$(MAKE) graphic.obj

dsctbl :		
				$(MAKE) dsctbl.obj

img :
				$(MAKE) $(OSNAME).img

# イメージ起動コマンド
run :		
				$(MAKE) img
				$(COPY) $(OSNAME).img ..\z_tools\qemu\fdimage0.bin
				$(MAKE) -C ../z_tools/qemu

install :
				$(MAKE) img
				$(IMGTOL) w a: $(OSNAME).img

# 不要ファイル削除コマンド
clean :
				$(DEL)  *.bin
				$(DEL)  *.lst
				$(DEL)  *.gas
				$(DEL)  *.obj
				$(DEL)  bootpack.map
				$(DEL)  bootpack.bim
				$(DEL)  bootpack.hrb
				$(DEL) 	$(OSNAME).sys

src_only :
				$(MAKE) clean
				$(DEL)  $(OSNAME).img
