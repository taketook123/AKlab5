ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m := hello3.o
ccflags-y += -g 				# add debugging info
else
# normal makefile
KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD
	cp hello3.ko hello3.ko.unstripped
	$(CROSS_COMPILE)strip -g hello3.ko 	# strip only debugging info

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean

%.s %.i: %.c 					# just use make hello.s instead of objdump
	$(MAKE) -C $(KDIR) M=$$PWD $@

endif
