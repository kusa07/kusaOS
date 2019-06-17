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

# nask → バイナリファイル(.bin)
ipl10.bin :		ipl10.nas Makefile
				$(NASK) ipl10.nas ipl10.bin ipl10.lst

# nask → バイナリファイル(.bin)
asmhead.bin :	asmhead.nas Makefile
				$(NASK) asmhead.nas asmhead.bin asmhead.lst

# メインのファイル(C言語)
# C言語(.c) → GNUアセンブラ(.gas)
bootpack.gas :	bootpack.c Makefile
				$(CC1) -o bootpack.gas bootpack.c

# GNUアセンブラ(.gas) → nask(.nas)
bootpack.nas :	bootpack.gas Makefile
				$(GAS2NASK) bootpack.gas bootpack.nas

# nask(.nas) → オブジェクトファイル(.obj)
bootpack.obj :	bootpack.nas Makefile
				$(NASK) bootpack.nas bootpack.obj bootpack.lst

# naskの関数
# nask(.nas) → オブジェクトファイル(.obj)
naskfunc.obj :	naskfunc.nas Makefile
				$(NASK) naskfunc.nas naskfunc.obj naskfunc.lst

# フォントファイル
# フォント（.txt) → バイナリファイル(.bin)
hankaku.bin :	hankaku.txt Makefile
				$(MAKEFONT) hankaku.txt hankaku.bin

# バイナリファイル(.bin) →　オブジェクトファイル(.obj)
hankaku.obj :	hankaku.bin Makefile
				$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

# 画面描画関係(C言語)
# GNUアセンブラ(.gas) → nask(.nas)
graphic.gas :	graphic.c Makefile
				$(CC1) -o graphic.gas graphic.c

# GNUアセンブラ(.gas) → nask(.nas)
graphic.nas :	graphic.gas Makefile
				$(GAS2NASK) graphic.gas graphic.nas

# nask(.nas) → オブジェクトファイル(.obj)
graphic.obj :	graphic.nas Makefile
				$(NASK) graphic.nas graphic.obj graphic.lst

# GDTやIDT関係(C言語)
# GNUアセンブラ(.gas) → nask(.nas)
dsctbl.gas :	dsctbl.c Makefile
				$(CC1) -o dsctbl.gas dsctbl.c

# GNUアセンブラ(.gas) → nask(.nas)
dsctbl.nas :	dsctbl.gas Makefile
				$(GAS2NASK) dsctbl.gas dsctbl.nas

# nask(.nas) → オブジェクトファイル(.obj)
dsctbl.obj :	dsctbl.nas Makefile
				$(NASK) dsctbl.nas dsctbl.obj dsctbl.lst

# 必要なオブジェクトファイルを全てくっつけてbimファイル(binary image 著者オリジナル2進数イメージファイル)　3MB(3MB*1024=3072K)+64KB=3136KB
bootpack.bim :	bootpack.obj naskfunc.obj  hankaku.obj graphic.obj dsctbl.obj Makefile
				$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
					bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj

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
				$(DEL)  bootpack.nas
				$(DEL)  graphic.nas
				$(DEL)  dsctbl.nas
				$(DEL)  bootpack.map
				$(DEL)  bootpack.bim
				$(DEL)  bootpack.hrb
				$(DEL) 	$(OSNAME).sys

src_only :
				$(MAKE) clean
				$(DEL)  $(OSNAME).img
