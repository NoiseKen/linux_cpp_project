# Copyright (c) 2017, Ken Wang
#
# Compiling pkg requires : None
# 

CC  = g++ -O -Wall -Wno-unused-result
EXE = NVMeCtrlRW.exe
LIBRARY	= ../library
PY_LIB	= $(LIBRARY)/pylib
LIB_INC	= $(LIBRARY)/include
LIB_SRC = $(LIBRARY)/src
NVM_LIB = $(LIBRARY)/NVMe
IOEX_LIB = ./ioex_cmd
FOP_LIB  = ./fop_cmd

DIST = $(shell uname -r)
INCLUDES = -I./ -I$(LIB_INC) -I$(NVM_LIB) -I$(NVM_LIB)/legacy -I$(NVM_LIB)/sequence -I $(IOEX_LIB) -I $(FOP_LIB)

DFLAGS = -std=c++0x
LFLAGS = -lrt

SOURCES = \
	main.cpp	\
	console_function.cpp	\
	fun_action.cpp	\
	fun_ioex.cpp	\
	action_base.cpp	\
	action_read.cpp	\
	action_write.cpp	\
	action_reset.cpp	\
	action_dump.cpp	\
	file_operation.cpp	\
	$(LIB_SRC)/io_ctrl.cpp	\
	$(LIB_SRC)/disk_io.cpp	\
	$(LIB_SRC)/disk_ioex.cpp	\
	$(NVM_LIB)/nvme_ioex.cpp	\
	$(NVM_LIB)/nvme_ioex_seq.cpp	\
	$(NVM_LIB)/legacy/nvme_task_fill_lba.cpp	\
	$(NVM_LIB)/legacy/nvme_task_fill_const.cpp	\
	$(NVM_LIB)/legacy/nvme_task_fill_increase.cpp	\
	$(NVM_LIB)/legacy/nvme_task_fill_decrease.cpp	\
	$(NVM_LIB)/legacy/nvme_task_dummy_read.cpp	\
	$(NVM_LIB)/legacy/nvme_task_rc.cpp	\
	$(NVM_LIB)/legacy/nvme_task_rc_const.cpp	\
	$(NVM_LIB)/legacy/nvme_task_rc_lba.cpp	\
	$(NVM_LIB)/legacy/nvme_task_rc_increase.cpp	\
	$(NVM_LIB)/legacy/nvme_task_rc_decrease.cpp	\
	$(NVM_LIB)/sequence/nvme_seq_fill_lba.cpp	\
	$(NVM_LIB)/sequence/nvme_seq_fill_const.cpp	\
	$(NVM_LIB)/sequence/nvme_seq_fill_increase.cpp	\
	$(NVM_LIB)/sequence/nvme_seq_fill_decrease.cpp	\
	$(NVM_LIB)/sequence/nvme_seq_dummy_read.cpp	\
	$(NVM_LIB)/sequence/nvme_seq_rc.cpp	\
	$(NVM_LIB)/sequence/nvme_seq_rc_const.cpp	\
	$(NVM_LIB)/sequence/nvme_seq_rc_lba.cpp	\
	$(NVM_LIB)/sequence/nvme_seq_rc_increase.cpp	\
	$(NVM_LIB)/sequence/nvme_seq_rc_decrease.cpp	\
	$(IOEX_LIB)/ioex_cmd.cpp	\
	$(IOEX_LIB)/ioex_dummy_read.cpp	\
	$(IOEX_LIB)/ioex_fill.cpp	\
	$(IOEX_LIB)/ioex_compare.cpp	\
	$(FOP_LIB)/fop_cmd.cpp	\
	$(FOP_LIB)/fop_read.cpp	\
	$(FOP_LIB)/fop_write.cpp	\
	$(LIB_SRC)/thread_locker.cpp	\
	$(LIB_SRC)/async_cmd.cpp	\
	$(LIB_SRC)/async_rw_handler.cpp	\
	$(LIB_SRC)/extension_string.cpp	\
	$(LIB_SRC)/legacy_string.cpp	\
	$(LIB_SRC)/global_variable.cpp	\
	$(LIB_SRC)/console_arguments.cpp	\
	$(LIB_SRC)/sys_ctrl.cpp	\
	$(LIB_SRC)/file_ctrl.cpp	\
	$(LIB_SRC)/utils.c

all:
	$(PY_LIB)/build_version.py app_ver.dat
	$(CC) $(INCLUDES) $(DFLAGS) $(SOURCES) -o $(EXE) $(LFLAGS)

clean:
	rm -f *.bin *.exe *.log *.so *.stackdump *.d $(EXE) *~
	rm -rf tmp
# DO NOT DELETE

main.o: /usr/include/errno.h /usr/include/features.h
main.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
main.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
main.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
main.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
main.o: /usr/include/asm-generic/errno-base.h /usr/include/stdio.h
main.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
main.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
main.o: /usr/include/bits/wchar.h /usr/include/xlocale.h
main.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
main.o: /usr/include/malloc.h console_function.h
main.o: ../library/include/console_arguments.h
main.o: ../library/include/global_variable.h ../library/include/property.h
main.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
main.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
main.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
main.o: /usr/include/bits/byteswap-16.h /usr/include/sys/types.h
main.o: /usr/include/time.h /usr/include/sys/select.h
main.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
main.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
main.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
main.o: /usr/include/assert.h ../library/include/extension_string.h
main.o: ../library/include/sys_ctrl.h /usr/include/linux/limits.h
main.o: ../library/include/file_ctrl.h fun_action.h fun_ioex.h
main.o: file_operation.h ../library/include/disk_ioex.h
main.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
main.o: /usr/include/asm/ioctls.h /usr/include/asm-generic/ioctls.h
main.o: /usr/include/linux/ioctl.h /usr/include/asm/ioctl.h
main.o: /usr/include/asm-generic/ioctl.h /usr/include/bits/ioctl-types.h
main.o: /usr/include/sys/ttydefaults.h ../library/include/disk_io.h
main.o: ../library/include/io_ctrl.h /usr/include/fcntl.h
main.o: /usr/include/bits/fcntl.h /usr/include/bits/fcntl-linux.h
main.o: /usr/include/bits/stat.h ../library/include/async_rw_handler.h
main.o: ../library/include/async_cmd.h /usr/include/stdint.h
main.o: /usr/include/aio.h /usr/include/bits/siginfo.h
main.o: ../library/include/report_channel.h
main.o: ../library/include/thread_locker.h kw_ioctl.h ./linux/lightnvm.h
main.o: /usr/include/linux/types.h /usr/include/asm/types.h
main.o: /usr/include/asm-generic/types.h /usr/include/asm-generic/int-ll64.h
main.o: /usr/include/asm/bitsperlong.h /usr/include/asm-generic/bitsperlong.h
main.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
main.o: /usr/include/asm/posix_types.h /usr/include/asm/posix_types_64.h
main.o: /usr/include/asm-generic/posix_types.h ./linux/nvme_ioctl.h
main.o: app_ver.dat
console_function.o: /usr/include/stdio.h /usr/include/features.h
console_function.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
console_function.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
console_function.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
console_function.o: /usr/include/libio.h /usr/include/_G_config.h
console_function.o: /usr/include/wchar.h /usr/include/bits/wchar.h
console_function.o: /usr/include/xlocale.h /usr/include/bits/stdio_lim.h
console_function.o: /usr/include/bits/sys_errlist.h console_function.h
console_function.o: ../library/include/console_arguments.h
console_function.o: ../library/include/global_variable.h
console_function.o: ../library/include/property.h /usr/include/stdlib.h
console_function.o: /usr/include/bits/waitflags.h
console_function.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
console_function.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
console_function.o: /usr/include/bits/byteswap-16.h /usr/include/sys/types.h
console_function.o: /usr/include/time.h /usr/include/sys/select.h
console_function.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
console_function.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
console_function.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
console_function.o: /usr/include/assert.h ../library/include/sys_ctrl.h
console_function.o: /usr/include/linux/limits.h
console_function.o: ../library/include/file_ctrl.h
fun_action.o: fun_action.h ../library/include/console_arguments.h
fun_action.o: ../library/include/global_variable.h
fun_action.o: ../library/include/property.h /usr/include/stdlib.h
fun_action.o: /usr/include/features.h /usr/include/stdc-predef.h
fun_action.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
fun_action.o: /usr/include/gnu/stubs.h /usr/include/bits/waitflags.h
fun_action.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
fun_action.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
fun_action.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
fun_action.o: /usr/include/bits/byteswap-16.h /usr/include/sys/types.h
fun_action.o: /usr/include/time.h /usr/include/sys/select.h
fun_action.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
fun_action.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
fun_action.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
fun_action.o: /usr/include/assert.h action_read.h action_base.h
fun_action.o: /usr/include/sys/mman.h /usr/include/bits/mman.h
fun_action.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
fun_action.o: /usr/include/bits/fcntl-linux.h /usr/include/bits/stat.h
fun_action.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
fun_action.o: /usr/include/bits/environments.h /usr/include/bits/confname.h
fun_action.o: /usr/include/getopt.h action_write.h action_reset.h
fun_action.o: action_dump.h
fun_ioex.o: fun_ioex.h ../library/include/console_arguments.h
fun_ioex.o: ../library/include/global_variable.h
fun_ioex.o: ../library/include/property.h /usr/include/stdlib.h
fun_ioex.o: /usr/include/features.h /usr/include/stdc-predef.h
fun_ioex.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
fun_ioex.o: /usr/include/gnu/stubs.h /usr/include/bits/waitflags.h
fun_ioex.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
fun_ioex.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
fun_ioex.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
fun_ioex.o: /usr/include/bits/byteswap-16.h /usr/include/sys/types.h
fun_ioex.o: /usr/include/time.h /usr/include/sys/select.h
fun_ioex.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
fun_ioex.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
fun_ioex.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
fun_ioex.o: /usr/include/assert.h ./ioex_cmd/ioex_dummy_read.h
fun_ioex.o: ./ioex_cmd/ioex_cmd.h ../library/NVMe/nvme_ioex.h
fun_ioex.o: ../library/include/disk_ioex.h /usr/include/sys/ioctl.h
fun_ioex.o: /usr/include/bits/ioctls.h /usr/include/asm/ioctls.h
fun_ioex.o: /usr/include/asm-generic/ioctls.h /usr/include/linux/ioctl.h
fun_ioex.o: /usr/include/asm/ioctl.h /usr/include/asm-generic/ioctl.h
fun_ioex.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
fun_ioex.o: ../library/include/disk_io.h ../library/include/io_ctrl.h
fun_ioex.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
fun_ioex.o: /usr/include/bits/fcntl-linux.h /usr/include/bits/stat.h
fun_ioex.o: ../library/include/async_rw_handler.h
fun_ioex.o: ../library/include/async_cmd.h /usr/include/stdint.h
fun_ioex.o: /usr/include/bits/wchar.h /usr/include/aio.h
fun_ioex.o: /usr/include/bits/siginfo.h ../library/include/report_channel.h
fun_ioex.o: ../library/include/thread_locker.h nvme.h
fun_ioex.o: /usr/include/linux/types.h /usr/include/asm/types.h
fun_ioex.o: /usr/include/asm-generic/types.h
fun_ioex.o: /usr/include/asm-generic/int-ll64.h
fun_ioex.o: /usr/include/asm/bitsperlong.h
fun_ioex.o: /usr/include/asm-generic/bitsperlong.h
fun_ioex.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
fun_ioex.o: /usr/include/asm/posix_types.h /usr/include/asm/posix_types_64.h
fun_ioex.o: /usr/include/asm-generic/posix_types.h ./ioex_cmd/ioex_fill.h
fun_ioex.o: ./ioex_cmd/ioex_compare.h ../library/include/memory.h
fun_ioex.o: ../library/include/legacy_string.h
action_base.o: action_base.h /usr/include/sys/mman.h /usr/include/features.h
action_base.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
action_base.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
action_base.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
action_base.o: /usr/include/bits/mman.h /usr/include/fcntl.h
action_base.o: /usr/include/bits/fcntl.h /usr/include/bits/fcntl-linux.h
action_base.o: /usr/include/time.h /usr/include/bits/stat.h
action_base.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
action_base.o: /usr/include/bits/environments.h /usr/include/bits/confname.h
action_base.o: /usr/include/getopt.h ../library/include/console_arguments.h
action_base.o: ../library/include/global_variable.h
action_base.o: ../library/include/property.h /usr/include/stdlib.h
action_base.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
action_base.o: /usr/include/endian.h /usr/include/bits/endian.h
action_base.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
action_base.o: /usr/include/sys/types.h /usr/include/sys/select.h
action_base.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
action_base.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
action_base.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
action_base.o: /usr/include/assert.h
action_read.o: action_read.h action_base.h /usr/include/sys/mman.h
action_read.o: /usr/include/features.h /usr/include/stdc-predef.h
action_read.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
action_read.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
action_read.o: /usr/include/bits/typesizes.h /usr/include/bits/mman.h
action_read.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
action_read.o: /usr/include/bits/fcntl-linux.h /usr/include/time.h
action_read.o: /usr/include/bits/stat.h /usr/include/unistd.h
action_read.o: /usr/include/bits/posix_opt.h /usr/include/bits/environments.h
action_read.o: /usr/include/bits/confname.h /usr/include/getopt.h
action_read.o: ../library/include/console_arguments.h
action_read.o: ../library/include/global_variable.h
action_read.o: ../library/include/property.h /usr/include/stdlib.h
action_read.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
action_read.o: /usr/include/endian.h /usr/include/bits/endian.h
action_read.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
action_read.o: /usr/include/sys/types.h /usr/include/sys/select.h
action_read.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
action_read.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
action_read.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
action_read.o: /usr/include/assert.h fun_action.h ../library/include/utils.h
action_read.o: /usr/include/linux/types.h /usr/include/asm/types.h
action_read.o: /usr/include/asm-generic/types.h
action_read.o: /usr/include/asm-generic/int-ll64.h
action_read.o: /usr/include/asm/bitsperlong.h
action_read.o: /usr/include/asm-generic/bitsperlong.h
action_read.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
action_read.o: /usr/include/asm/posix_types.h
action_read.o: /usr/include/asm/posix_types_64.h
action_read.o: /usr/include/asm-generic/posix_types.h /usr/include/sys/time.h
action_write.o: action_write.h action_base.h /usr/include/sys/mman.h
action_write.o: /usr/include/features.h /usr/include/stdc-predef.h
action_write.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
action_write.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
action_write.o: /usr/include/bits/typesizes.h /usr/include/bits/mman.h
action_write.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
action_write.o: /usr/include/bits/fcntl-linux.h /usr/include/time.h
action_write.o: /usr/include/bits/stat.h /usr/include/unistd.h
action_write.o: /usr/include/bits/posix_opt.h
action_write.o: /usr/include/bits/environments.h /usr/include/bits/confname.h
action_write.o: /usr/include/getopt.h ../library/include/console_arguments.h
action_write.o: ../library/include/global_variable.h
action_write.o: ../library/include/property.h /usr/include/stdlib.h
action_write.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
action_write.o: /usr/include/endian.h /usr/include/bits/endian.h
action_write.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
action_write.o: /usr/include/sys/types.h /usr/include/sys/select.h
action_write.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
action_write.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
action_write.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
action_write.o: /usr/include/assert.h fun_action.h ../library/include/utils.h
action_write.o: /usr/include/linux/types.h /usr/include/asm/types.h
action_write.o: /usr/include/asm-generic/types.h
action_write.o: /usr/include/asm-generic/int-ll64.h
action_write.o: /usr/include/asm/bitsperlong.h
action_write.o: /usr/include/asm-generic/bitsperlong.h
action_write.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
action_write.o: /usr/include/asm/posix_types.h
action_write.o: /usr/include/asm/posix_types_64.h
action_write.o: /usr/include/asm-generic/posix_types.h
action_write.o: /usr/include/sys/time.h
action_reset.o: action_reset.h action_base.h /usr/include/sys/mman.h
action_reset.o: /usr/include/features.h /usr/include/stdc-predef.h
action_reset.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
action_reset.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
action_reset.o: /usr/include/bits/typesizes.h /usr/include/bits/mman.h
action_reset.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
action_reset.o: /usr/include/bits/fcntl-linux.h /usr/include/time.h
action_reset.o: /usr/include/bits/stat.h /usr/include/unistd.h
action_reset.o: /usr/include/bits/posix_opt.h
action_reset.o: /usr/include/bits/environments.h /usr/include/bits/confname.h
action_reset.o: /usr/include/getopt.h ../library/include/console_arguments.h
action_reset.o: ../library/include/global_variable.h
action_reset.o: ../library/include/property.h /usr/include/stdlib.h
action_reset.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
action_reset.o: /usr/include/endian.h /usr/include/bits/endian.h
action_reset.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
action_reset.o: /usr/include/sys/types.h /usr/include/sys/select.h
action_reset.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
action_reset.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
action_reset.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
action_reset.o: /usr/include/assert.h fun_action.h ../library/include/utils.h
action_reset.o: /usr/include/linux/types.h /usr/include/asm/types.h
action_reset.o: /usr/include/asm-generic/types.h
action_reset.o: /usr/include/asm-generic/int-ll64.h
action_reset.o: /usr/include/asm/bitsperlong.h
action_reset.o: /usr/include/asm-generic/bitsperlong.h
action_reset.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
action_reset.o: /usr/include/asm/posix_types.h
action_reset.o: /usr/include/asm/posix_types_64.h
action_reset.o: /usr/include/asm-generic/posix_types.h
action_reset.o: /usr/include/sys/time.h
action_dump.o: action_dump.h action_base.h /usr/include/sys/mman.h
action_dump.o: /usr/include/features.h /usr/include/stdc-predef.h
action_dump.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
action_dump.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
action_dump.o: /usr/include/bits/typesizes.h /usr/include/bits/mman.h
action_dump.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
action_dump.o: /usr/include/bits/fcntl-linux.h /usr/include/time.h
action_dump.o: /usr/include/bits/stat.h /usr/include/unistd.h
action_dump.o: /usr/include/bits/posix_opt.h /usr/include/bits/environments.h
action_dump.o: /usr/include/bits/confname.h /usr/include/getopt.h
action_dump.o: ../library/include/console_arguments.h
action_dump.o: ../library/include/global_variable.h
action_dump.o: ../library/include/property.h /usr/include/stdlib.h
action_dump.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
action_dump.o: /usr/include/endian.h /usr/include/bits/endian.h
action_dump.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
action_dump.o: /usr/include/sys/types.h /usr/include/sys/select.h
action_dump.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
action_dump.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
action_dump.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
action_dump.o: /usr/include/assert.h fun_action.h
action_dump.o: ../library/include/disk_ioex.h /usr/include/sys/ioctl.h
action_dump.o: /usr/include/bits/ioctls.h /usr/include/asm/ioctls.h
action_dump.o: /usr/include/asm-generic/ioctls.h /usr/include/linux/ioctl.h
action_dump.o: /usr/include/asm/ioctl.h /usr/include/asm-generic/ioctl.h
action_dump.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
action_dump.o: ../library/include/disk_io.h ../library/include/io_ctrl.h
action_dump.o: ../library/include/async_rw_handler.h
action_dump.o: ../library/include/async_cmd.h /usr/include/stdint.h
action_dump.o: /usr/include/bits/wchar.h /usr/include/aio.h
action_dump.o: /usr/include/bits/siginfo.h
action_dump.o: ../library/include/report_channel.h
action_dump.o: ../library/include/thread_locker.h ../library/include/utils.h
action_dump.o: /usr/include/linux/types.h /usr/include/asm/types.h
action_dump.o: /usr/include/asm-generic/types.h
action_dump.o: /usr/include/asm-generic/int-ll64.h
action_dump.o: /usr/include/asm/bitsperlong.h
action_dump.o: /usr/include/asm-generic/bitsperlong.h
action_dump.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
action_dump.o: /usr/include/asm/posix_types.h
action_dump.o: /usr/include/asm/posix_types_64.h
action_dump.o: /usr/include/asm-generic/posix_types.h /usr/include/sys/time.h
action_dump.o: ../library/include/memory.h ../library/include/legacy_string.h
file_operation.o: file_operation.h ../library/include/console_arguments.h
file_operation.o: ../library/include/global_variable.h
file_operation.o: ../library/include/property.h /usr/include/stdlib.h
file_operation.o: /usr/include/features.h /usr/include/stdc-predef.h
file_operation.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
file_operation.o: /usr/include/gnu/stubs.h /usr/include/bits/waitflags.h
file_operation.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
file_operation.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
file_operation.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
file_operation.o: /usr/include/bits/byteswap-16.h /usr/include/sys/types.h
file_operation.o: /usr/include/time.h /usr/include/sys/select.h
file_operation.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
file_operation.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
file_operation.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
file_operation.o: /usr/include/assert.h ../library/include/disk_ioex.h
file_operation.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
file_operation.o: /usr/include/asm/ioctls.h /usr/include/asm-generic/ioctls.h
file_operation.o: /usr/include/linux/ioctl.h /usr/include/asm/ioctl.h
file_operation.o: /usr/include/asm-generic/ioctl.h
file_operation.o: /usr/include/bits/ioctl-types.h
file_operation.o: /usr/include/sys/ttydefaults.h ../library/include/disk_io.h
file_operation.o: ../library/include/io_ctrl.h /usr/include/fcntl.h
file_operation.o: /usr/include/bits/fcntl.h /usr/include/bits/fcntl-linux.h
file_operation.o: /usr/include/bits/stat.h
file_operation.o: ../library/include/async_rw_handler.h
file_operation.o: ../library/include/async_cmd.h /usr/include/stdint.h
file_operation.o: /usr/include/bits/wchar.h /usr/include/aio.h
file_operation.o: /usr/include/bits/siginfo.h
file_operation.o: ../library/include/report_channel.h
file_operation.o: ../library/include/thread_locker.h nvme.h
file_operation.o: /usr/include/linux/types.h /usr/include/asm/types.h
file_operation.o: /usr/include/asm-generic/types.h
file_operation.o: /usr/include/asm-generic/int-ll64.h
file_operation.o: /usr/include/asm/bitsperlong.h
file_operation.o: /usr/include/asm-generic/bitsperlong.h
file_operation.o: /usr/include/linux/posix_types.h
file_operation.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
file_operation.o: /usr/include/asm/posix_types_64.h
file_operation.o: /usr/include/asm-generic/posix_types.h
file_operation.o: ../library/NVMe/nvme_ioex.h ./fop_cmd/fop_read.h
file_operation.o: ./fop_cmd/fop_cmd.h /usr/include/malloc.h
file_operation.o: /usr/include/stdio.h /usr/include/libio.h
file_operation.o: /usr/include/_G_config.h /usr/include/wchar.h
file_operation.o: /usr/include/xlocale.h /usr/include/bits/stdio_lim.h
file_operation.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
file_operation.o: ./fop_cmd/fop_write.h
../library/src/io_ctrl.o: /usr/include/unistd.h /usr/include/features.h
../library/src/io_ctrl.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
../library/src/io_ctrl.o: /usr/include/bits/wordsize.h
../library/src/io_ctrl.o: /usr/include/gnu/stubs.h
../library/src/io_ctrl.o: /usr/include/bits/posix_opt.h
../library/src/io_ctrl.o: /usr/include/bits/environments.h
../library/src/io_ctrl.o: /usr/include/bits/types.h
../library/src/io_ctrl.o: /usr/include/bits/typesizes.h
../library/src/io_ctrl.o: /usr/include/bits/confname.h /usr/include/getopt.h
../library/src/io_ctrl.o: ../library/include/io_ctrl.h /usr/include/fcntl.h
../library/src/io_ctrl.o: /usr/include/bits/fcntl.h
../library/src/io_ctrl.o: /usr/include/bits/fcntl-linux.h /usr/include/time.h
../library/src/io_ctrl.o: /usr/include/bits/stat.h /usr/include/sys/ioctl.h
../library/src/io_ctrl.o: /usr/include/bits/ioctls.h
../library/src/io_ctrl.o: /usr/include/asm/ioctls.h
../library/src/io_ctrl.o: /usr/include/asm-generic/ioctls.h
../library/src/io_ctrl.o: /usr/include/linux/ioctl.h /usr/include/asm/ioctl.h
../library/src/io_ctrl.o: /usr/include/asm-generic/ioctl.h
../library/src/io_ctrl.o: /usr/include/bits/ioctl-types.h
../library/src/io_ctrl.o: /usr/include/sys/ttydefaults.h
../library/src/io_ctrl.o: ../library/include/property.h /usr/include/stdlib.h
../library/src/io_ctrl.o: /usr/include/bits/waitflags.h
../library/src/io_ctrl.o: /usr/include/bits/waitstatus.h
../library/src/io_ctrl.o: /usr/include/endian.h /usr/include/bits/endian.h
../library/src/io_ctrl.o: /usr/include/bits/byteswap.h
../library/src/io_ctrl.o: /usr/include/bits/byteswap-16.h
../library/src/io_ctrl.o: /usr/include/sys/types.h /usr/include/sys/select.h
../library/src/io_ctrl.o: /usr/include/bits/select.h
../library/src/io_ctrl.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
../library/src/io_ctrl.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
../library/src/io_ctrl.o: /usr/include/bits/stdlib-float.h
../library/src/io_ctrl.o: /usr/include/assert.h
../library/src/disk_io.o: /usr/include/fcntl.h /usr/include/features.h
../library/src/disk_io.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
../library/src/disk_io.o: /usr/include/bits/wordsize.h
../library/src/disk_io.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
../library/src/disk_io.o: /usr/include/bits/typesizes.h
../library/src/disk_io.o: /usr/include/bits/fcntl.h
../library/src/disk_io.o: /usr/include/bits/fcntl-linux.h /usr/include/time.h
../library/src/disk_io.o: /usr/include/bits/stat.h /usr/include/unistd.h
../library/src/disk_io.o: /usr/include/bits/posix_opt.h
../library/src/disk_io.o: /usr/include/bits/environments.h
../library/src/disk_io.o: /usr/include/bits/confname.h /usr/include/getopt.h
../library/src/disk_io.o: /usr/include/string.h /usr/include/xlocale.h
../library/src/disk_io.o: ../library/include/disk_io.h
../library/src/disk_io.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
../library/src/disk_io.o: /usr/include/asm/ioctls.h
../library/src/disk_io.o: /usr/include/asm-generic/ioctls.h
../library/src/disk_io.o: /usr/include/linux/ioctl.h /usr/include/asm/ioctl.h
../library/src/disk_io.o: /usr/include/asm-generic/ioctl.h
../library/src/disk_io.o: /usr/include/bits/ioctl-types.h
../library/src/disk_io.o: /usr/include/sys/ttydefaults.h
../library/src/disk_io.o: ../library/include/property.h /usr/include/stdlib.h
../library/src/disk_io.o: /usr/include/bits/waitflags.h
../library/src/disk_io.o: /usr/include/bits/waitstatus.h
../library/src/disk_io.o: /usr/include/endian.h /usr/include/bits/endian.h
../library/src/disk_io.o: /usr/include/bits/byteswap.h
../library/src/disk_io.o: /usr/include/bits/byteswap-16.h
../library/src/disk_io.o: /usr/include/sys/types.h /usr/include/sys/select.h
../library/src/disk_io.o: /usr/include/bits/select.h
../library/src/disk_io.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
../library/src/disk_io.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
../library/src/disk_io.o: /usr/include/bits/stdlib-float.h
../library/src/disk_io.o: /usr/include/assert.h ../library/include/io_ctrl.h
../library/src/disk_io.o: ../library/include/async_rw_handler.h
../library/src/disk_io.o: ../library/include/async_cmd.h
../library/src/disk_io.o: /usr/include/stdint.h /usr/include/bits/wchar.h
../library/src/disk_io.o: /usr/include/aio.h /usr/include/bits/siginfo.h
../library/src/disk_io.o: ../library/include/legacy_string.h
../library/src/disk_ioex.o: /usr/include/unistd.h /usr/include/features.h
../library/src/disk_ioex.o: /usr/include/stdc-predef.h
../library/src/disk_ioex.o: /usr/include/sys/cdefs.h
../library/src/disk_ioex.o: /usr/include/bits/wordsize.h
../library/src/disk_ioex.o: /usr/include/gnu/stubs.h
../library/src/disk_ioex.o: /usr/include/bits/posix_opt.h
../library/src/disk_ioex.o: /usr/include/bits/environments.h
../library/src/disk_ioex.o: /usr/include/bits/types.h
../library/src/disk_ioex.o: /usr/include/bits/typesizes.h
../library/src/disk_ioex.o: /usr/include/bits/confname.h
../library/src/disk_ioex.o: /usr/include/getopt.h /usr/include/string.h
../library/src/disk_ioex.o: /usr/include/xlocale.h /usr/include/malloc.h
../library/src/disk_ioex.o: /usr/include/stdio.h /usr/include/libio.h
../library/src/disk_ioex.o: /usr/include/_G_config.h /usr/include/wchar.h
../library/src/disk_ioex.o: /usr/include/bits/wchar.h
../library/src/disk_ioex.o: /usr/include/bits/stdio_lim.h
../library/src/disk_ioex.o: /usr/include/bits/sys_errlist.h
../library/src/disk_ioex.o: /usr/include/linux/fs.h
../library/src/disk_ioex.o: /usr/include/linux/limits.h
../library/src/disk_ioex.o: /usr/include/linux/ioctl.h
../library/src/disk_ioex.o: /usr/include/asm/ioctl.h
../library/src/disk_ioex.o: /usr/include/asm-generic/ioctl.h
../library/src/disk_ioex.o: /usr/include/linux/types.h
../library/src/disk_ioex.o: /usr/include/asm/types.h
../library/src/disk_ioex.o: /usr/include/asm-generic/types.h
../library/src/disk_ioex.o: /usr/include/asm-generic/int-ll64.h
../library/src/disk_ioex.o: /usr/include/asm/bitsperlong.h
../library/src/disk_ioex.o: /usr/include/asm-generic/bitsperlong.h
../library/src/disk_ioex.o: /usr/include/linux/posix_types.h
../library/src/disk_ioex.o: /usr/include/linux/stddef.h
../library/src/disk_ioex.o: /usr/include/asm/posix_types.h
../library/src/disk_ioex.o: /usr/include/asm/posix_types_64.h
../library/src/disk_ioex.o: /usr/include/asm-generic/posix_types.h
../library/src/disk_ioex.o: ../library/include/disk_ioex.h
../library/src/disk_ioex.o: /usr/include/sys/ioctl.h
../library/src/disk_ioex.o: /usr/include/bits/ioctls.h
../library/src/disk_ioex.o: /usr/include/asm/ioctls.h
../library/src/disk_ioex.o: /usr/include/asm-generic/ioctls.h
../library/src/disk_ioex.o: /usr/include/bits/ioctl-types.h
../library/src/disk_ioex.o: /usr/include/sys/ttydefaults.h
../library/src/disk_ioex.o: ../library/include/disk_io.h
../library/src/disk_ioex.o: ../library/include/property.h
../library/src/disk_ioex.o: /usr/include/stdlib.h
../library/src/disk_ioex.o: /usr/include/bits/waitflags.h
../library/src/disk_ioex.o: /usr/include/bits/waitstatus.h
../library/src/disk_ioex.o: /usr/include/endian.h /usr/include/bits/endian.h
../library/src/disk_ioex.o: /usr/include/bits/byteswap.h
../library/src/disk_ioex.o: /usr/include/bits/byteswap-16.h
../library/src/disk_ioex.o: /usr/include/sys/types.h /usr/include/time.h
../library/src/disk_ioex.o: /usr/include/sys/select.h
../library/src/disk_ioex.o: /usr/include/bits/select.h
../library/src/disk_ioex.o: /usr/include/bits/sigset.h
../library/src/disk_ioex.o: /usr/include/bits/time.h
../library/src/disk_ioex.o: /usr/include/sys/sysmacros.h
../library/src/disk_ioex.o: /usr/include/alloca.h
../library/src/disk_ioex.o: /usr/include/bits/stdlib-float.h
../library/src/disk_ioex.o: /usr/include/assert.h
../library/src/disk_ioex.o: ../library/include/io_ctrl.h /usr/include/fcntl.h
../library/src/disk_ioex.o: /usr/include/bits/fcntl.h
../library/src/disk_ioex.o: /usr/include/bits/fcntl-linux.h
../library/src/disk_ioex.o: /usr/include/bits/stat.h
../library/src/disk_ioex.o: ../library/include/async_rw_handler.h
../library/src/disk_ioex.o: ../library/include/async_cmd.h
../library/src/disk_ioex.o: /usr/include/stdint.h /usr/include/aio.h
../library/src/disk_ioex.o: /usr/include/bits/siginfo.h
../library/src/disk_ioex.o: ../library/include/report_channel.h
../library/src/disk_ioex.o: ../library/include/thread_locker.h
../library/src/disk_ioex.o: ../library/include/legacy_string.h
../library/src/disk_ioex.o: ../library/include/utils.h
../library/src/disk_ioex.o: /usr/include/sys/time.h
../library/NVMe/nvme_ioex.o: /usr/include/fcntl.h /usr/include/features.h
../library/NVMe/nvme_ioex.o: /usr/include/stdc-predef.h
../library/NVMe/nvme_ioex.o: /usr/include/sys/cdefs.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/wordsize.h
../library/NVMe/nvme_ioex.o: /usr/include/gnu/stubs.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/types.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/typesizes.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/fcntl.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/nvme_ioex.o: /usr/include/time.h /usr/include/bits/stat.h
../library/NVMe/nvme_ioex.o: /usr/include/unistd.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/posix_opt.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/environments.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/confname.h
../library/NVMe/nvme_ioex.o: /usr/include/getopt.h /usr/include/string.h
../library/NVMe/nvme_ioex.o: /usr/include/xlocale.h
../library/NVMe/nvme_ioex.o: ../library/include/memory.h
../library/NVMe/nvme_ioex.o: /usr/include/stdint.h /usr/include/bits/wchar.h
../library/NVMe/nvme_ioex.o: ../library/include/property.h
../library/NVMe/nvme_ioex.o: /usr/include/stdlib.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/waitflags.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/waitstatus.h
../library/NVMe/nvme_ioex.o: /usr/include/endian.h /usr/include/bits/endian.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/byteswap.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/byteswap-16.h
../library/NVMe/nvme_ioex.o: /usr/include/sys/types.h
../library/NVMe/nvme_ioex.o: /usr/include/sys/select.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/select.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/sigset.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/time.h
../library/NVMe/nvme_ioex.o: /usr/include/sys/sysmacros.h
../library/NVMe/nvme_ioex.o: /usr/include/alloca.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/stdlib-float.h
../library/NVMe/nvme_ioex.o: /usr/include/assert.h
../library/NVMe/nvme_ioex.o: ../library/include/legacy_string.h
../library/NVMe/nvme_ioex.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/nvme_ioex.o: ../library/include/disk_ioex.h
../library/NVMe/nvme_ioex.o: /usr/include/sys/ioctl.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/ioctls.h
../library/NVMe/nvme_ioex.o: /usr/include/asm/ioctls.h
../library/NVMe/nvme_ioex.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/nvme_ioex.o: /usr/include/linux/ioctl.h
../library/NVMe/nvme_ioex.o: /usr/include/asm/ioctl.h
../library/NVMe/nvme_ioex.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/nvme_ioex.o: /usr/include/bits/ioctl-types.h
../library/NVMe/nvme_ioex.o: /usr/include/sys/ttydefaults.h
../library/NVMe/nvme_ioex.o: ../library/include/disk_io.h
../library/NVMe/nvme_ioex.o: ../library/include/io_ctrl.h
../library/NVMe/nvme_ioex.o: ../library/include/async_rw_handler.h
../library/NVMe/nvme_ioex.o: ../library/include/async_cmd.h
../library/NVMe/nvme_ioex.o: /usr/include/aio.h /usr/include/bits/siginfo.h
../library/NVMe/nvme_ioex.o: ../library/include/report_channel.h
../library/NVMe/nvme_ioex.o: ../library/include/thread_locker.h
../library/NVMe/nvme_ioex.o: ../library/NVMe/nvme_status.h
../library/NVMe/nvme_ioex_seq.o: ../library/NVMe/nvme_ioex_seq.h
../library/NVMe/nvme_ioex_seq.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/nvme_ioex_seq.o: ../library/include/property.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/stdlib.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/features.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/stdc-predef.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/sys/cdefs.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/wordsize.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/gnu/stubs.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/waitflags.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/waitstatus.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/endian.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/endian.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/byteswap.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/types.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/typesizes.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/byteswap-16.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/sys/types.h /usr/include/time.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/sys/select.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/select.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/sigset.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/time.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/sys/sysmacros.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/alloca.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/stdlib-float.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/assert.h
../library/NVMe/nvme_ioex_seq.o: ../library/include/disk_ioex.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/sys/ioctl.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/ioctls.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/asm/ioctls.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/linux/ioctl.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/asm/ioctl.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/ioctl-types.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/sys/ttydefaults.h
../library/NVMe/nvme_ioex_seq.o: ../library/include/disk_io.h
../library/NVMe/nvme_ioex_seq.o: ../library/include/io_ctrl.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/fcntl.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/fcntl.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/stat.h
../library/NVMe/nvme_ioex_seq.o: ../library/include/async_rw_handler.h
../library/NVMe/nvme_ioex_seq.o: ../library/include/async_cmd.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/stdint.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/wchar.h /usr/include/aio.h
../library/NVMe/nvme_ioex_seq.o: /usr/include/bits/siginfo.h
../library/NVMe/nvme_ioex_seq.o: ../library/include/report_channel.h
../library/NVMe/nvme_ioex_seq.o: ../library/include/thread_locker.h
../library/NVMe/legacy/nvme_task_fill_lba.o: ../library/NVMe/legacy/nvme_task_fill_lba.h
../library/NVMe/legacy/nvme_task_fill_lba.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/legacy/nvme_task_fill_lba.o: ../library/include/property.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/stdlib.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/features.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/stdc-predef.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/sys/cdefs.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/wordsize.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/gnu/stubs.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/waitflags.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/waitstatus.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/endian.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/endian.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/byteswap.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/types.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/typesizes.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/byteswap-16.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/sys/types.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/time.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/sys/select.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/select.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/sigset.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/time.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/sys/sysmacros.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/alloca.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/stdlib-float.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/assert.h
../library/NVMe/legacy/nvme_task_fill_lba.o: ../library/include/disk_ioex.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/sys/ioctl.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/ioctls.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/asm/ioctls.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/linux/ioctl.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/asm/ioctl.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/ioctl-types.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/sys/ttydefaults.h
../library/NVMe/legacy/nvme_task_fill_lba.o: ../library/include/disk_io.h
../library/NVMe/legacy/nvme_task_fill_lba.o: ../library/include/io_ctrl.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/fcntl.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/fcntl.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/stat.h
../library/NVMe/legacy/nvme_task_fill_lba.o: ../library/include/async_rw_handler.h
../library/NVMe/legacy/nvme_task_fill_lba.o: ../library/include/async_cmd.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/stdint.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/wchar.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/aio.h
../library/NVMe/legacy/nvme_task_fill_lba.o: /usr/include/bits/siginfo.h
../library/NVMe/legacy/nvme_task_fill_lba.o: ../library/include/report_channel.h
../library/NVMe/legacy/nvme_task_fill_lba.o: ../library/include/thread_locker.h
../library/NVMe/legacy/nvme_task_fill_lba.o: ../library/include/legacy_string.h
../library/NVMe/legacy/nvme_task_fill_const.o: ../library/NVMe/legacy/nvme_task_fill_const.h
../library/NVMe/legacy/nvme_task_fill_const.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/legacy/nvme_task_fill_const.o: ../library/include/property.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/stdlib.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/features.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/stdc-predef.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/sys/cdefs.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/wordsize.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/gnu/stubs.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/waitflags.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/waitstatus.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/endian.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/endian.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/byteswap.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/types.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/typesizes.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/byteswap-16.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/sys/types.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/time.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/sys/select.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/select.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/sigset.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/time.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/sys/sysmacros.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/alloca.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/stdlib-float.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/assert.h
../library/NVMe/legacy/nvme_task_fill_const.o: ../library/include/disk_ioex.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/sys/ioctl.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/ioctls.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/asm/ioctls.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/linux/ioctl.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/asm/ioctl.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/ioctl-types.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/sys/ttydefaults.h
../library/NVMe/legacy/nvme_task_fill_const.o: ../library/include/disk_io.h
../library/NVMe/legacy/nvme_task_fill_const.o: ../library/include/io_ctrl.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/fcntl.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/fcntl.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/stat.h
../library/NVMe/legacy/nvme_task_fill_const.o: ../library/include/async_rw_handler.h
../library/NVMe/legacy/nvme_task_fill_const.o: ../library/include/async_cmd.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/stdint.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/wchar.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/aio.h
../library/NVMe/legacy/nvme_task_fill_const.o: /usr/include/bits/siginfo.h
../library/NVMe/legacy/nvme_task_fill_const.o: ../library/include/report_channel.h
../library/NVMe/legacy/nvme_task_fill_const.o: ../library/include/thread_locker.h
../library/NVMe/legacy/nvme_task_fill_const.o: ../library/include/legacy_string.h
../library/NVMe/legacy/nvme_task_fill_increase.o: ../library/NVMe/legacy/nvme_task_fill_increase.h
../library/NVMe/legacy/nvme_task_fill_increase.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/legacy/nvme_task_fill_increase.o: ../library/include/property.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/stdlib.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/features.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/stdc-predef.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/sys/cdefs.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/wordsize.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/gnu/stubs.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/waitflags.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/waitstatus.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/endian.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/endian.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/byteswap.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/types.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/typesizes.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/byteswap-16.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/sys/types.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/time.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/sys/select.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/select.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/sigset.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/time.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/sys/sysmacros.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/alloca.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/stdlib-float.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/assert.h
../library/NVMe/legacy/nvme_task_fill_increase.o: ../library/include/disk_ioex.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/sys/ioctl.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/ioctls.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/asm/ioctls.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/linux/ioctl.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/asm/ioctl.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/ioctl-types.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/sys/ttydefaults.h
../library/NVMe/legacy/nvme_task_fill_increase.o: ../library/include/disk_io.h
../library/NVMe/legacy/nvme_task_fill_increase.o: ../library/include/io_ctrl.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/fcntl.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/fcntl.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/stat.h
../library/NVMe/legacy/nvme_task_fill_increase.o: ../library/include/async_rw_handler.h
../library/NVMe/legacy/nvme_task_fill_increase.o: ../library/include/async_cmd.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/stdint.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/wchar.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/aio.h
../library/NVMe/legacy/nvme_task_fill_increase.o: /usr/include/bits/siginfo.h
../library/NVMe/legacy/nvme_task_fill_increase.o: ../library/include/report_channel.h
../library/NVMe/legacy/nvme_task_fill_increase.o: ../library/include/thread_locker.h
../library/NVMe/legacy/nvme_task_fill_increase.o: ../library/include/legacy_string.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: ../library/NVMe/legacy/nvme_task_fill_decrease.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: ../library/include/property.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/stdlib.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/features.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/stdc-predef.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/sys/cdefs.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/wordsize.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/gnu/stubs.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/waitflags.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/waitstatus.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/endian.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/endian.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/byteswap.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/types.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/typesizes.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/byteswap-16.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/sys/types.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/time.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/sys/select.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/select.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/sigset.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/time.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/sys/sysmacros.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/alloca.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/stdlib-float.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/assert.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: ../library/include/disk_ioex.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/sys/ioctl.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/ioctls.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/asm/ioctls.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/linux/ioctl.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/asm/ioctl.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/ioctl-types.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/sys/ttydefaults.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: ../library/include/disk_io.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: ../library/include/io_ctrl.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/fcntl.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/fcntl.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/stat.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: ../library/include/async_rw_handler.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: ../library/include/async_cmd.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/stdint.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/wchar.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/aio.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: /usr/include/bits/siginfo.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: ../library/include/report_channel.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: ../library/include/thread_locker.h
../library/NVMe/legacy/nvme_task_fill_decrease.o: ../library/include/legacy_string.h
../library/NVMe/legacy/nvme_task_dummy_read.o: ../library/NVMe/legacy/nvme_task_dummy_read.h
../library/NVMe/legacy/nvme_task_dummy_read.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/legacy/nvme_task_dummy_read.o: ../library/include/property.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/stdlib.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/features.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/stdc-predef.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/sys/cdefs.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/wordsize.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/gnu/stubs.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/waitflags.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/waitstatus.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/endian.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/endian.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/byteswap.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/types.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/typesizes.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/byteswap-16.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/sys/types.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/time.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/sys/select.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/select.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/sigset.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/time.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/sys/sysmacros.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/alloca.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/stdlib-float.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/assert.h
../library/NVMe/legacy/nvme_task_dummy_read.o: ../library/include/disk_ioex.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/sys/ioctl.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/ioctls.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/asm/ioctls.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/linux/ioctl.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/asm/ioctl.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/ioctl-types.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/sys/ttydefaults.h
../library/NVMe/legacy/nvme_task_dummy_read.o: ../library/include/disk_io.h
../library/NVMe/legacy/nvme_task_dummy_read.o: ../library/include/io_ctrl.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/fcntl.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/fcntl.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/stat.h
../library/NVMe/legacy/nvme_task_dummy_read.o: ../library/include/async_rw_handler.h
../library/NVMe/legacy/nvme_task_dummy_read.o: ../library/include/async_cmd.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/stdint.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/wchar.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/aio.h
../library/NVMe/legacy/nvme_task_dummy_read.o: /usr/include/bits/siginfo.h
../library/NVMe/legacy/nvme_task_dummy_read.o: ../library/include/report_channel.h
../library/NVMe/legacy/nvme_task_dummy_read.o: ../library/include/thread_locker.h
../library/NVMe/legacy/nvme_task_dummy_read.o: ../library/include/legacy_string.h
../library/NVMe/legacy/nvme_task_rc.o: ../library/NVMe/legacy/nvme_task_rc.h
../library/NVMe/legacy/nvme_task_rc.o: nvme.h /usr/include/linux/types.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/asm/types.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/asm-generic/types.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/asm-generic/int-ll64.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/asm/bitsperlong.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/asm-generic/bitsperlong.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/linux/posix_types.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/linux/stddef.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/asm/posix_types.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/asm/posix_types_64.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/asm-generic/posix_types.h
../library/NVMe/legacy/nvme_task_rc.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/legacy/nvme_task_rc.o: ../library/include/property.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/stdlib.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/features.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/stdc-predef.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/sys/cdefs.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/wordsize.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/gnu/stubs.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/waitflags.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/waitstatus.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/endian.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/endian.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/byteswap.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/types.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/typesizes.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/byteswap-16.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/sys/types.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/time.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/sys/select.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/select.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/sigset.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/time.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/sys/sysmacros.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/alloca.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/stdlib-float.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/assert.h
../library/NVMe/legacy/nvme_task_rc.o: ../library/include/disk_ioex.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/sys/ioctl.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/ioctls.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/asm/ioctls.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/linux/ioctl.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/asm/ioctl.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/ioctl-types.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/sys/ttydefaults.h
../library/NVMe/legacy/nvme_task_rc.o: ../library/include/disk_io.h
../library/NVMe/legacy/nvme_task_rc.o: ../library/include/io_ctrl.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/fcntl.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/fcntl.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/stat.h
../library/NVMe/legacy/nvme_task_rc.o: ../library/include/async_rw_handler.h
../library/NVMe/legacy/nvme_task_rc.o: ../library/include/async_cmd.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/stdint.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/wchar.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/aio.h
../library/NVMe/legacy/nvme_task_rc.o: /usr/include/bits/siginfo.h
../library/NVMe/legacy/nvme_task_rc.o: ../library/include/report_channel.h
../library/NVMe/legacy/nvme_task_rc.o: ../library/include/thread_locker.h
../library/NVMe/legacy/nvme_task_rc.o: ../library/include/memory.h
../library/NVMe/legacy/nvme_task_rc.o: ../library/include/legacy_string.h
../library/NVMe/legacy/nvme_task_rc_const.o: ../library/NVMe/legacy/nvme_task_rc_const.h
../library/NVMe/legacy/nvme_task_rc_const.o: ../library/NVMe/legacy/nvme_task_rc.h
../library/NVMe/legacy/nvme_task_rc_const.o: nvme.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/linux/types.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/asm/types.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/asm-generic/types.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/asm-generic/int-ll64.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/asm/bitsperlong.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/asm-generic/bitsperlong.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/linux/posix_types.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/linux/stddef.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/asm/posix_types.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/asm/posix_types_64.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/asm-generic/posix_types.h
../library/NVMe/legacy/nvme_task_rc_const.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/legacy/nvme_task_rc_const.o: ../library/include/property.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/stdlib.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/features.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/stdc-predef.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/sys/cdefs.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/wordsize.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/gnu/stubs.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/waitflags.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/waitstatus.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/endian.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/endian.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/byteswap.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/types.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/typesizes.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/byteswap-16.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/sys/types.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/time.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/sys/select.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/select.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/sigset.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/time.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/sys/sysmacros.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/alloca.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/stdlib-float.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/assert.h
../library/NVMe/legacy/nvme_task_rc_const.o: ../library/include/disk_ioex.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/sys/ioctl.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/ioctls.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/asm/ioctls.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/linux/ioctl.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/asm/ioctl.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/ioctl-types.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/sys/ttydefaults.h
../library/NVMe/legacy/nvme_task_rc_const.o: ../library/include/disk_io.h
../library/NVMe/legacy/nvme_task_rc_const.o: ../library/include/io_ctrl.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/fcntl.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/fcntl.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/stat.h
../library/NVMe/legacy/nvme_task_rc_const.o: ../library/include/async_rw_handler.h
../library/NVMe/legacy/nvme_task_rc_const.o: ../library/include/async_cmd.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/stdint.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/wchar.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/aio.h
../library/NVMe/legacy/nvme_task_rc_const.o: /usr/include/bits/siginfo.h
../library/NVMe/legacy/nvme_task_rc_const.o: ../library/include/report_channel.h
../library/NVMe/legacy/nvme_task_rc_const.o: ../library/include/thread_locker.h
../library/NVMe/legacy/nvme_task_rc_const.o: ../library/include/memory.h
../library/NVMe/legacy/nvme_task_rc_const.o: ../library/include/legacy_string.h
../library/NVMe/legacy/nvme_task_rc_lba.o: ../library/NVMe/legacy/nvme_task_rc_lba.h
../library/NVMe/legacy/nvme_task_rc_lba.o: ../library/NVMe/legacy/nvme_task_rc.h
../library/NVMe/legacy/nvme_task_rc_lba.o: nvme.h /usr/include/linux/types.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/asm/types.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/asm-generic/types.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/asm-generic/int-ll64.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/asm/bitsperlong.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/asm-generic/bitsperlong.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/linux/posix_types.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/linux/stddef.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/asm/posix_types.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/asm/posix_types_64.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/asm-generic/posix_types.h
../library/NVMe/legacy/nvme_task_rc_lba.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/legacy/nvme_task_rc_lba.o: ../library/include/property.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/stdlib.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/features.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/stdc-predef.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/sys/cdefs.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/wordsize.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/gnu/stubs.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/waitflags.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/waitstatus.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/endian.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/endian.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/byteswap.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/types.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/typesizes.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/byteswap-16.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/sys/types.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/time.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/sys/select.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/select.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/sigset.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/time.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/sys/sysmacros.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/alloca.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/stdlib-float.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/assert.h
../library/NVMe/legacy/nvme_task_rc_lba.o: ../library/include/disk_ioex.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/sys/ioctl.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/ioctls.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/asm/ioctls.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/linux/ioctl.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/asm/ioctl.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/ioctl-types.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/sys/ttydefaults.h
../library/NVMe/legacy/nvme_task_rc_lba.o: ../library/include/disk_io.h
../library/NVMe/legacy/nvme_task_rc_lba.o: ../library/include/io_ctrl.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/fcntl.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/fcntl.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/stat.h
../library/NVMe/legacy/nvme_task_rc_lba.o: ../library/include/async_rw_handler.h
../library/NVMe/legacy/nvme_task_rc_lba.o: ../library/include/async_cmd.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/stdint.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/wchar.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/aio.h
../library/NVMe/legacy/nvme_task_rc_lba.o: /usr/include/bits/siginfo.h
../library/NVMe/legacy/nvme_task_rc_lba.o: ../library/include/report_channel.h
../library/NVMe/legacy/nvme_task_rc_lba.o: ../library/include/thread_locker.h
../library/NVMe/legacy/nvme_task_rc_lba.o: ../library/include/memory.h
../library/NVMe/legacy/nvme_task_rc_lba.o: ../library/include/legacy_string.h
../library/NVMe/legacy/nvme_task_rc_increase.o: ../library/NVMe/legacy/nvme_task_rc_increase.h
../library/NVMe/legacy/nvme_task_rc_increase.o: ../library/NVMe/legacy/nvme_task_rc.h
../library/NVMe/legacy/nvme_task_rc_increase.o: nvme.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/linux/types.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/asm/types.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/asm-generic/types.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/asm-generic/int-ll64.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/asm/bitsperlong.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/asm-generic/bitsperlong.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/linux/posix_types.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/linux/stddef.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/asm/posix_types.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/asm/posix_types_64.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/asm-generic/posix_types.h
../library/NVMe/legacy/nvme_task_rc_increase.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/legacy/nvme_task_rc_increase.o: ../library/include/property.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/stdlib.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/features.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/stdc-predef.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/sys/cdefs.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/wordsize.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/gnu/stubs.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/waitflags.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/waitstatus.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/endian.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/endian.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/byteswap.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/types.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/typesizes.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/byteswap-16.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/sys/types.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/time.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/sys/select.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/select.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/sigset.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/time.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/sys/sysmacros.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/alloca.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/stdlib-float.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/assert.h
../library/NVMe/legacy/nvme_task_rc_increase.o: ../library/include/disk_ioex.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/sys/ioctl.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/ioctls.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/asm/ioctls.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/linux/ioctl.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/asm/ioctl.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/ioctl-types.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/sys/ttydefaults.h
../library/NVMe/legacy/nvme_task_rc_increase.o: ../library/include/disk_io.h
../library/NVMe/legacy/nvme_task_rc_increase.o: ../library/include/io_ctrl.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/fcntl.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/fcntl.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/stat.h
../library/NVMe/legacy/nvme_task_rc_increase.o: ../library/include/async_rw_handler.h
../library/NVMe/legacy/nvme_task_rc_increase.o: ../library/include/async_cmd.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/stdint.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/wchar.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/aio.h
../library/NVMe/legacy/nvme_task_rc_increase.o: /usr/include/bits/siginfo.h
../library/NVMe/legacy/nvme_task_rc_increase.o: ../library/include/report_channel.h
../library/NVMe/legacy/nvme_task_rc_increase.o: ../library/include/thread_locker.h
../library/NVMe/legacy/nvme_task_rc_increase.o: ../library/include/memory.h
../library/NVMe/legacy/nvme_task_rc_increase.o: ../library/include/legacy_string.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: ../library/NVMe/legacy/nvme_task_rc_decrease.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: ../library/NVMe/legacy/nvme_task_rc.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: nvme.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/linux/types.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/asm/types.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/asm-generic/types.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/asm-generic/int-ll64.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/asm/bitsperlong.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/asm-generic/bitsperlong.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/linux/posix_types.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/linux/stddef.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/asm/posix_types.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/asm/posix_types_64.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/asm-generic/posix_types.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: ../library/include/property.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/stdlib.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/features.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/stdc-predef.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/sys/cdefs.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/wordsize.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/gnu/stubs.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/waitflags.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/waitstatus.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/endian.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/endian.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/byteswap.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/types.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/typesizes.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/byteswap-16.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/sys/types.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/time.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/sys/select.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/select.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/sigset.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/time.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/sys/sysmacros.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/alloca.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/stdlib-float.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/assert.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: ../library/include/disk_ioex.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/sys/ioctl.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/ioctls.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/asm/ioctls.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/linux/ioctl.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/asm/ioctl.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/ioctl-types.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/sys/ttydefaults.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: ../library/include/disk_io.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: ../library/include/io_ctrl.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/fcntl.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/fcntl.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/stat.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: ../library/include/async_rw_handler.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: ../library/include/async_cmd.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/stdint.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/wchar.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/aio.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: /usr/include/bits/siginfo.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: ../library/include/report_channel.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: ../library/include/thread_locker.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: ../library/include/memory.h
../library/NVMe/legacy/nvme_task_rc_decrease.o: ../library/include/legacy_string.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: ../library/NVMe/sequence/nvme_seq_fill_lba.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: ../library/NVMe/nvme_ioex_seq.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: ../library/include/property.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/stdlib.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/features.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/stdc-predef.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/sys/cdefs.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/wordsize.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/gnu/stubs.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/waitflags.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/waitstatus.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/endian.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/endian.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/byteswap.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/types.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/typesizes.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/byteswap-16.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/sys/types.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/time.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/sys/select.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/select.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/sigset.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/time.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/sys/sysmacros.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/alloca.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/stdlib-float.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/assert.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: ../library/include/disk_ioex.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/sys/ioctl.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/ioctls.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/asm/ioctls.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/linux/ioctl.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/asm/ioctl.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/ioctl-types.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/sys/ttydefaults.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: ../library/include/disk_io.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: ../library/include/io_ctrl.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/fcntl.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/fcntl.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/stat.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: ../library/include/async_rw_handler.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: ../library/include/async_cmd.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/stdint.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/wchar.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/aio.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: /usr/include/bits/siginfo.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: ../library/include/report_channel.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: ../library/include/thread_locker.h
../library/NVMe/sequence/nvme_seq_fill_lba.o: ../library/include/legacy_string.h
../library/NVMe/sequence/nvme_seq_fill_const.o: ../library/NVMe/sequence/nvme_seq_fill_const.h
../library/NVMe/sequence/nvme_seq_fill_const.o: ../library/NVMe/nvme_ioex_seq.h
../library/NVMe/sequence/nvme_seq_fill_const.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/sequence/nvme_seq_fill_const.o: ../library/include/property.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/stdlib.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/features.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/stdc-predef.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/sys/cdefs.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/wordsize.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/gnu/stubs.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/waitflags.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/waitstatus.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/endian.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/endian.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/byteswap.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/types.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/typesizes.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/byteswap-16.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/sys/types.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/time.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/sys/select.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/select.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/sigset.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/time.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/sys/sysmacros.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/alloca.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/stdlib-float.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/assert.h
../library/NVMe/sequence/nvme_seq_fill_const.o: ../library/include/disk_ioex.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/sys/ioctl.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/ioctls.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/asm/ioctls.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/linux/ioctl.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/asm/ioctl.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/ioctl-types.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/sys/ttydefaults.h
../library/NVMe/sequence/nvme_seq_fill_const.o: ../library/include/disk_io.h
../library/NVMe/sequence/nvme_seq_fill_const.o: ../library/include/io_ctrl.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/fcntl.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/fcntl.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/stat.h
../library/NVMe/sequence/nvme_seq_fill_const.o: ../library/include/async_rw_handler.h
../library/NVMe/sequence/nvme_seq_fill_const.o: ../library/include/async_cmd.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/stdint.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/wchar.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/aio.h
../library/NVMe/sequence/nvme_seq_fill_const.o: /usr/include/bits/siginfo.h
../library/NVMe/sequence/nvme_seq_fill_const.o: ../library/include/report_channel.h
../library/NVMe/sequence/nvme_seq_fill_const.o: ../library/include/thread_locker.h
../library/NVMe/sequence/nvme_seq_fill_const.o: ../library/include/legacy_string.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: ../library/NVMe/sequence/nvme_seq_fill_increase.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: ../library/NVMe/nvme_ioex_seq.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: ../library/include/property.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/stdlib.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/features.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/stdc-predef.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/sys/cdefs.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/wordsize.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/gnu/stubs.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/waitflags.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/waitstatus.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/endian.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/endian.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/byteswap.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/types.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/typesizes.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/byteswap-16.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/sys/types.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/time.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/sys/select.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/select.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/sigset.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/time.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/sys/sysmacros.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/alloca.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/stdlib-float.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/assert.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: ../library/include/disk_ioex.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/sys/ioctl.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/ioctls.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/asm/ioctls.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/linux/ioctl.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/asm/ioctl.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/ioctl-types.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/sys/ttydefaults.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: ../library/include/disk_io.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: ../library/include/io_ctrl.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/fcntl.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/fcntl.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/stat.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: ../library/include/async_rw_handler.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: ../library/include/async_cmd.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/stdint.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/wchar.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/aio.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: /usr/include/bits/siginfo.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: ../library/include/report_channel.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: ../library/include/thread_locker.h
../library/NVMe/sequence/nvme_seq_fill_increase.o: ../library/include/legacy_string.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: ../library/NVMe/sequence/nvme_seq_fill_decrease.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: ../library/NVMe/nvme_ioex_seq.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: ../library/include/property.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/stdlib.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/features.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/stdc-predef.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/sys/cdefs.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/wordsize.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/gnu/stubs.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/waitflags.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/waitstatus.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/endian.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/endian.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/byteswap.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/types.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/typesizes.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/byteswap-16.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/sys/types.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/time.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/sys/select.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/select.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/sigset.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/time.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/sys/sysmacros.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/alloca.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/stdlib-float.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/assert.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: ../library/include/disk_ioex.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/sys/ioctl.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/ioctls.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/asm/ioctls.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/linux/ioctl.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/asm/ioctl.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/ioctl-types.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/sys/ttydefaults.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: ../library/include/disk_io.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: ../library/include/io_ctrl.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/fcntl.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/fcntl.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/stat.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: ../library/include/async_rw_handler.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: ../library/include/async_cmd.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/stdint.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/wchar.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/aio.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: /usr/include/bits/siginfo.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: ../library/include/report_channel.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: ../library/include/thread_locker.h
../library/NVMe/sequence/nvme_seq_fill_decrease.o: ../library/include/legacy_string.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: ../library/NVMe/sequence/nvme_seq_dummy_read.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: ../library/NVMe/nvme_ioex_seq.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: ../library/include/property.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/stdlib.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/features.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/stdc-predef.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/sys/cdefs.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/wordsize.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/gnu/stubs.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/waitflags.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/waitstatus.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/endian.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/endian.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/byteswap.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/types.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/typesizes.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/byteswap-16.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/sys/types.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/time.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/sys/select.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/select.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/sigset.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/time.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/sys/sysmacros.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/alloca.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/stdlib-float.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/assert.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: ../library/include/disk_ioex.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/sys/ioctl.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/ioctls.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/asm/ioctls.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/linux/ioctl.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/asm/ioctl.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/ioctl-types.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/sys/ttydefaults.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: ../library/include/disk_io.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: ../library/include/io_ctrl.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/fcntl.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/fcntl.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/stat.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: ../library/include/async_rw_handler.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: ../library/include/async_cmd.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/stdint.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/wchar.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/aio.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: /usr/include/bits/siginfo.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: ../library/include/report_channel.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: ../library/include/thread_locker.h
../library/NVMe/sequence/nvme_seq_dummy_read.o: ../library/include/legacy_string.h
../library/NVMe/sequence/nvme_seq_rc.o: ../library/NVMe/sequence/nvme_seq_rc.h
../library/NVMe/sequence/nvme_seq_rc.o: nvme.h /usr/include/linux/types.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/asm/types.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/asm-generic/types.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/asm-generic/int-ll64.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/asm/bitsperlong.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/asm-generic/bitsperlong.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/linux/posix_types.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/linux/stddef.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/asm/posix_types.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/asm/posix_types_64.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/asm-generic/posix_types.h
../library/NVMe/sequence/nvme_seq_rc.o: ../library/NVMe/nvme_ioex_seq.h
../library/NVMe/sequence/nvme_seq_rc.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/sequence/nvme_seq_rc.o: ../library/include/property.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/stdlib.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/features.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/stdc-predef.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/sys/cdefs.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/wordsize.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/gnu/stubs.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/waitflags.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/waitstatus.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/endian.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/endian.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/byteswap.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/types.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/typesizes.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/byteswap-16.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/sys/types.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/time.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/sys/select.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/select.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/sigset.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/time.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/sys/sysmacros.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/alloca.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/stdlib-float.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/assert.h
../library/NVMe/sequence/nvme_seq_rc.o: ../library/include/disk_ioex.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/sys/ioctl.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/ioctls.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/asm/ioctls.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/linux/ioctl.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/asm/ioctl.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/ioctl-types.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/sys/ttydefaults.h
../library/NVMe/sequence/nvme_seq_rc.o: ../library/include/disk_io.h
../library/NVMe/sequence/nvme_seq_rc.o: ../library/include/io_ctrl.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/fcntl.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/fcntl.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/stat.h
../library/NVMe/sequence/nvme_seq_rc.o: ../library/include/async_rw_handler.h
../library/NVMe/sequence/nvme_seq_rc.o: ../library/include/async_cmd.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/stdint.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/wchar.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/aio.h
../library/NVMe/sequence/nvme_seq_rc.o: /usr/include/bits/siginfo.h
../library/NVMe/sequence/nvme_seq_rc.o: ../library/include/report_channel.h
../library/NVMe/sequence/nvme_seq_rc.o: ../library/include/thread_locker.h
../library/NVMe/sequence/nvme_seq_rc.o: ../library/include/memory.h
../library/NVMe/sequence/nvme_seq_rc.o: ../library/include/legacy_string.h
../library/NVMe/sequence/nvme_seq_rc_const.o: ../library/NVMe/sequence/nvme_seq_rc_const.h
../library/NVMe/sequence/nvme_seq_rc_const.o: ../library/NVMe/sequence/nvme_seq_rc.h
../library/NVMe/sequence/nvme_seq_rc_const.o: nvme.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/linux/types.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/asm/types.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/asm-generic/types.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/asm-generic/int-ll64.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/asm/bitsperlong.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/asm-generic/bitsperlong.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/linux/posix_types.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/linux/stddef.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/asm/posix_types.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/asm/posix_types_64.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/asm-generic/posix_types.h
../library/NVMe/sequence/nvme_seq_rc_const.o: ../library/NVMe/nvme_ioex_seq.h
../library/NVMe/sequence/nvme_seq_rc_const.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/sequence/nvme_seq_rc_const.o: ../library/include/property.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/stdlib.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/features.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/stdc-predef.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/sys/cdefs.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/wordsize.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/gnu/stubs.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/waitflags.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/waitstatus.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/endian.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/endian.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/byteswap.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/types.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/typesizes.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/byteswap-16.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/sys/types.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/time.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/sys/select.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/select.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/sigset.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/time.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/sys/sysmacros.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/alloca.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/stdlib-float.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/assert.h
../library/NVMe/sequence/nvme_seq_rc_const.o: ../library/include/disk_ioex.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/sys/ioctl.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/ioctls.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/asm/ioctls.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/linux/ioctl.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/asm/ioctl.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/ioctl-types.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/sys/ttydefaults.h
../library/NVMe/sequence/nvme_seq_rc_const.o: ../library/include/disk_io.h
../library/NVMe/sequence/nvme_seq_rc_const.o: ../library/include/io_ctrl.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/fcntl.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/fcntl.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/stat.h
../library/NVMe/sequence/nvme_seq_rc_const.o: ../library/include/async_rw_handler.h
../library/NVMe/sequence/nvme_seq_rc_const.o: ../library/include/async_cmd.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/stdint.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/wchar.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/aio.h
../library/NVMe/sequence/nvme_seq_rc_const.o: /usr/include/bits/siginfo.h
../library/NVMe/sequence/nvme_seq_rc_const.o: ../library/include/report_channel.h
../library/NVMe/sequence/nvme_seq_rc_const.o: ../library/include/thread_locker.h
../library/NVMe/sequence/nvme_seq_rc_const.o: ../library/include/memory.h
../library/NVMe/sequence/nvme_seq_rc_const.o: ../library/include/legacy_string.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: ../library/NVMe/sequence/nvme_seq_rc_lba.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: ../library/NVMe/sequence/nvme_seq_rc.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: nvme.h /usr/include/linux/types.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/asm/types.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/asm-generic/types.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/asm-generic/int-ll64.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/asm/bitsperlong.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/asm-generic/bitsperlong.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/linux/posix_types.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/linux/stddef.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/asm/posix_types.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/asm/posix_types_64.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/asm-generic/posix_types.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: ../library/NVMe/nvme_ioex_seq.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: ../library/include/property.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/stdlib.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/features.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/stdc-predef.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/sys/cdefs.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/wordsize.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/gnu/stubs.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/waitflags.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/waitstatus.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/endian.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/endian.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/byteswap.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/types.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/typesizes.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/byteswap-16.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/sys/types.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/time.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/sys/select.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/select.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/sigset.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/time.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/sys/sysmacros.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/alloca.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/stdlib-float.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/assert.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: ../library/include/disk_ioex.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/sys/ioctl.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/ioctls.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/asm/ioctls.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/linux/ioctl.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/asm/ioctl.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/ioctl-types.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/sys/ttydefaults.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: ../library/include/disk_io.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: ../library/include/io_ctrl.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/fcntl.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/fcntl.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/stat.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: ../library/include/async_rw_handler.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: ../library/include/async_cmd.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/stdint.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/wchar.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/aio.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: /usr/include/bits/siginfo.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: ../library/include/report_channel.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: ../library/include/thread_locker.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: ../library/include/memory.h
../library/NVMe/sequence/nvme_seq_rc_lba.o: ../library/include/legacy_string.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: ../library/NVMe/sequence/nvme_seq_rc_increase.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: ../library/NVMe/sequence/nvme_seq_rc.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: nvme.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/linux/types.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/asm/types.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/asm-generic/types.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/asm-generic/int-ll64.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/asm/bitsperlong.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/asm-generic/bitsperlong.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/linux/posix_types.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/linux/stddef.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/asm/posix_types.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/asm/posix_types_64.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/asm-generic/posix_types.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: ../library/NVMe/nvme_ioex_seq.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: ../library/include/property.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/stdlib.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/features.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/stdc-predef.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/sys/cdefs.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/wordsize.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/gnu/stubs.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/waitflags.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/waitstatus.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/endian.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/endian.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/byteswap.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/types.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/typesizes.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/byteswap-16.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/sys/types.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/time.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/sys/select.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/select.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/sigset.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/time.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/sys/sysmacros.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/alloca.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/stdlib-float.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/assert.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: ../library/include/disk_ioex.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/sys/ioctl.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/ioctls.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/asm/ioctls.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/linux/ioctl.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/asm/ioctl.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/ioctl-types.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/sys/ttydefaults.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: ../library/include/disk_io.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: ../library/include/io_ctrl.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/fcntl.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/fcntl.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/stat.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: ../library/include/async_rw_handler.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: ../library/include/async_cmd.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/stdint.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/wchar.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/aio.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: /usr/include/bits/siginfo.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: ../library/include/report_channel.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: ../library/include/thread_locker.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: ../library/include/memory.h
../library/NVMe/sequence/nvme_seq_rc_increase.o: ../library/include/legacy_string.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: ../library/NVMe/sequence/nvme_seq_rc_decrease.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: ../library/NVMe/sequence/nvme_seq_rc.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: nvme.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/linux/types.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/asm/types.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/asm-generic/types.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/asm-generic/int-ll64.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/asm/bitsperlong.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/asm-generic/bitsperlong.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/linux/posix_types.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/linux/stddef.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/asm/posix_types.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/asm/posix_types_64.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/asm-generic/posix_types.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: ../library/NVMe/nvme_ioex_seq.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: ../library/NVMe/nvme_ioex.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: ../library/include/property.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/stdlib.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/features.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/stdc-predef.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/sys/cdefs.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/wordsize.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/gnu/stubs.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/waitflags.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/waitstatus.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/endian.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/endian.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/byteswap.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/types.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/typesizes.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/byteswap-16.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/sys/types.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/time.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/sys/select.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/select.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/sigset.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/time.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/sys/sysmacros.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/alloca.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/stdlib-float.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/assert.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: ../library/include/disk_ioex.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/sys/ioctl.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/ioctls.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/asm/ioctls.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/asm-generic/ioctls.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/linux/ioctl.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/asm/ioctl.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/asm-generic/ioctl.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/ioctl-types.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/sys/ttydefaults.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: ../library/include/disk_io.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: ../library/include/io_ctrl.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/fcntl.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/fcntl.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/fcntl-linux.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/stat.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: ../library/include/async_rw_handler.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: ../library/include/async_cmd.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/stdint.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/wchar.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/aio.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: /usr/include/bits/siginfo.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: ../library/include/report_channel.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: ../library/include/thread_locker.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: ../library/include/memory.h
../library/NVMe/sequence/nvme_seq_rc_decrease.o: ../library/include/legacy_string.h
./ioex_cmd/ioex_cmd.o: ./ioex_cmd/ioex_cmd.h
./ioex_cmd/ioex_cmd.o: ../library/include/console_arguments.h
./ioex_cmd/ioex_cmd.o: ../library/include/global_variable.h
./ioex_cmd/ioex_cmd.o: ../library/include/property.h /usr/include/stdlib.h
./ioex_cmd/ioex_cmd.o: /usr/include/features.h /usr/include/stdc-predef.h
./ioex_cmd/ioex_cmd.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
./ioex_cmd/ioex_cmd.o: /usr/include/gnu/stubs.h /usr/include/bits/waitflags.h
./ioex_cmd/ioex_cmd.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
./ioex_cmd/ioex_cmd.o: /usr/include/bits/endian.h
./ioex_cmd/ioex_cmd.o: /usr/include/bits/byteswap.h /usr/include/bits/types.h
./ioex_cmd/ioex_cmd.o: /usr/include/bits/typesizes.h
./ioex_cmd/ioex_cmd.o: /usr/include/bits/byteswap-16.h
./ioex_cmd/ioex_cmd.o: /usr/include/sys/types.h /usr/include/time.h
./ioex_cmd/ioex_cmd.o: /usr/include/sys/select.h /usr/include/bits/select.h
./ioex_cmd/ioex_cmd.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
./ioex_cmd/ioex_cmd.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
./ioex_cmd/ioex_cmd.o: /usr/include/bits/stdlib-float.h /usr/include/assert.h
./ioex_cmd/ioex_cmd.o: ../library/NVMe/nvme_ioex.h
./ioex_cmd/ioex_cmd.o: ../library/include/disk_ioex.h
./ioex_cmd/ioex_cmd.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
./ioex_cmd/ioex_cmd.o: /usr/include/asm/ioctls.h
./ioex_cmd/ioex_cmd.o: /usr/include/asm-generic/ioctls.h
./ioex_cmd/ioex_cmd.o: /usr/include/linux/ioctl.h /usr/include/asm/ioctl.h
./ioex_cmd/ioex_cmd.o: /usr/include/asm-generic/ioctl.h
./ioex_cmd/ioex_cmd.o: /usr/include/bits/ioctl-types.h
./ioex_cmd/ioex_cmd.o: /usr/include/sys/ttydefaults.h
./ioex_cmd/ioex_cmd.o: ../library/include/disk_io.h
./ioex_cmd/ioex_cmd.o: ../library/include/io_ctrl.h /usr/include/fcntl.h
./ioex_cmd/ioex_cmd.o: /usr/include/bits/fcntl.h
./ioex_cmd/ioex_cmd.o: /usr/include/bits/fcntl-linux.h
./ioex_cmd/ioex_cmd.o: /usr/include/bits/stat.h
./ioex_cmd/ioex_cmd.o: ../library/include/async_rw_handler.h
./ioex_cmd/ioex_cmd.o: ../library/include/async_cmd.h /usr/include/stdint.h
./ioex_cmd/ioex_cmd.o: /usr/include/bits/wchar.h /usr/include/aio.h
./ioex_cmd/ioex_cmd.o: /usr/include/bits/siginfo.h
./ioex_cmd/ioex_cmd.o: ../library/include/report_channel.h
./ioex_cmd/ioex_cmd.o: ../library/include/thread_locker.h nvme.h
./ioex_cmd/ioex_cmd.o: /usr/include/linux/types.h /usr/include/asm/types.h
./ioex_cmd/ioex_cmd.o: /usr/include/asm-generic/types.h
./ioex_cmd/ioex_cmd.o: /usr/include/asm-generic/int-ll64.h
./ioex_cmd/ioex_cmd.o: /usr/include/asm/bitsperlong.h
./ioex_cmd/ioex_cmd.o: /usr/include/asm-generic/bitsperlong.h
./ioex_cmd/ioex_cmd.o: /usr/include/linux/posix_types.h
./ioex_cmd/ioex_cmd.o: /usr/include/linux/stddef.h
./ioex_cmd/ioex_cmd.o: /usr/include/asm/posix_types.h
./ioex_cmd/ioex_cmd.o: /usr/include/asm/posix_types_64.h
./ioex_cmd/ioex_cmd.o: /usr/include/asm-generic/posix_types.h
./ioex_cmd/ioex_dummy_read.o: ./ioex_cmd/ioex_dummy_read.h
./ioex_cmd/ioex_dummy_read.o: ../library/include/global_variable.h
./ioex_cmd/ioex_dummy_read.o: ../library/include/property.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/stdlib.h /usr/include/features.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/stdc-predef.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/sys/cdefs.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/bits/wordsize.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/gnu/stubs.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/bits/waitflags.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/bits/waitstatus.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/endian.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/bits/endian.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/bits/byteswap.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/bits/types.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/bits/typesizes.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/bits/byteswap-16.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/sys/types.h /usr/include/time.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/sys/select.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/bits/select.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/bits/sigset.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/bits/time.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/sys/sysmacros.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/alloca.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/bits/stdlib-float.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/assert.h ./ioex_cmd/ioex_cmd.h
./ioex_cmd/ioex_dummy_read.o: ../library/include/console_arguments.h
./ioex_cmd/ioex_dummy_read.o: ../library/NVMe/nvme_ioex.h
./ioex_cmd/ioex_dummy_read.o: ../library/include/disk_ioex.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/sys/ioctl.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/bits/ioctls.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/asm/ioctls.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/asm-generic/ioctls.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/linux/ioctl.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/asm/ioctl.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/asm-generic/ioctl.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/bits/ioctl-types.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/sys/ttydefaults.h
./ioex_cmd/ioex_dummy_read.o: ../library/include/disk_io.h
./ioex_cmd/ioex_dummy_read.o: ../library/include/io_ctrl.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/bits/fcntl-linux.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/bits/stat.h
./ioex_cmd/ioex_dummy_read.o: ../library/include/async_rw_handler.h
./ioex_cmd/ioex_dummy_read.o: ../library/include/async_cmd.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/stdint.h /usr/include/bits/wchar.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/aio.h /usr/include/bits/siginfo.h
./ioex_cmd/ioex_dummy_read.o: ../library/include/report_channel.h
./ioex_cmd/ioex_dummy_read.o: ../library/include/thread_locker.h nvme.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/linux/types.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/asm/types.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/asm-generic/types.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/asm-generic/int-ll64.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/asm/bitsperlong.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/asm-generic/bitsperlong.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/linux/posix_types.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/linux/stddef.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/asm/posix_types.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/asm/posix_types_64.h
./ioex_cmd/ioex_dummy_read.o: /usr/include/asm-generic/posix_types.h
./ioex_cmd/ioex_dummy_read.o: fun_ioex.h
./ioex_cmd/ioex_dummy_read.o: ../library/NVMe/legacy/nvme_task_dummy_read.h
./ioex_cmd/ioex_dummy_read.o: ../library/NVMe/sequence/nvme_seq_dummy_read.h
./ioex_cmd/ioex_dummy_read.o: ../library/NVMe/nvme_ioex_seq.h
./ioex_cmd/ioex_fill.o: ./ioex_cmd/ioex_fill.h
./ioex_cmd/ioex_fill.o: ../library/include/global_variable.h
./ioex_cmd/ioex_fill.o: ../library/include/property.h /usr/include/stdlib.h
./ioex_cmd/ioex_fill.o: /usr/include/features.h /usr/include/stdc-predef.h
./ioex_cmd/ioex_fill.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
./ioex_cmd/ioex_fill.o: /usr/include/gnu/stubs.h
./ioex_cmd/ioex_fill.o: /usr/include/bits/waitflags.h
./ioex_cmd/ioex_fill.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
./ioex_cmd/ioex_fill.o: /usr/include/bits/endian.h
./ioex_cmd/ioex_fill.o: /usr/include/bits/byteswap.h
./ioex_cmd/ioex_fill.o: /usr/include/bits/types.h
./ioex_cmd/ioex_fill.o: /usr/include/bits/typesizes.h
./ioex_cmd/ioex_fill.o: /usr/include/bits/byteswap-16.h
./ioex_cmd/ioex_fill.o: /usr/include/sys/types.h /usr/include/time.h
./ioex_cmd/ioex_fill.o: /usr/include/sys/select.h /usr/include/bits/select.h
./ioex_cmd/ioex_fill.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
./ioex_cmd/ioex_fill.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
./ioex_cmd/ioex_fill.o: /usr/include/bits/stdlib-float.h
./ioex_cmd/ioex_fill.o: /usr/include/assert.h ./ioex_cmd/ioex_cmd.h
./ioex_cmd/ioex_fill.o: ../library/include/console_arguments.h
./ioex_cmd/ioex_fill.o: ../library/NVMe/nvme_ioex.h
./ioex_cmd/ioex_fill.o: ../library/include/disk_ioex.h
./ioex_cmd/ioex_fill.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
./ioex_cmd/ioex_fill.o: /usr/include/asm/ioctls.h
./ioex_cmd/ioex_fill.o: /usr/include/asm-generic/ioctls.h
./ioex_cmd/ioex_fill.o: /usr/include/linux/ioctl.h /usr/include/asm/ioctl.h
./ioex_cmd/ioex_fill.o: /usr/include/asm-generic/ioctl.h
./ioex_cmd/ioex_fill.o: /usr/include/bits/ioctl-types.h
./ioex_cmd/ioex_fill.o: /usr/include/sys/ttydefaults.h
./ioex_cmd/ioex_fill.o: ../library/include/disk_io.h
./ioex_cmd/ioex_fill.o: ../library/include/io_ctrl.h /usr/include/fcntl.h
./ioex_cmd/ioex_fill.o: /usr/include/bits/fcntl.h
./ioex_cmd/ioex_fill.o: /usr/include/bits/fcntl-linux.h
./ioex_cmd/ioex_fill.o: /usr/include/bits/stat.h
./ioex_cmd/ioex_fill.o: ../library/include/async_rw_handler.h
./ioex_cmd/ioex_fill.o: ../library/include/async_cmd.h /usr/include/stdint.h
./ioex_cmd/ioex_fill.o: /usr/include/bits/wchar.h /usr/include/aio.h
./ioex_cmd/ioex_fill.o: /usr/include/bits/siginfo.h
./ioex_cmd/ioex_fill.o: ../library/include/report_channel.h
./ioex_cmd/ioex_fill.o: ../library/include/thread_locker.h nvme.h
./ioex_cmd/ioex_fill.o: /usr/include/linux/types.h /usr/include/asm/types.h
./ioex_cmd/ioex_fill.o: /usr/include/asm-generic/types.h
./ioex_cmd/ioex_fill.o: /usr/include/asm-generic/int-ll64.h
./ioex_cmd/ioex_fill.o: /usr/include/asm/bitsperlong.h
./ioex_cmd/ioex_fill.o: /usr/include/asm-generic/bitsperlong.h
./ioex_cmd/ioex_fill.o: /usr/include/linux/posix_types.h
./ioex_cmd/ioex_fill.o: /usr/include/linux/stddef.h
./ioex_cmd/ioex_fill.o: /usr/include/asm/posix_types.h
./ioex_cmd/ioex_fill.o: /usr/include/asm/posix_types_64.h
./ioex_cmd/ioex_fill.o: /usr/include/asm-generic/posix_types.h fun_ioex.h
./ioex_cmd/ioex_fill.o: ../library/NVMe/legacy/nvme_task_fill_const.h
./ioex_cmd/ioex_fill.o: ../library/NVMe/legacy/nvme_task_fill_decrease.h
./ioex_cmd/ioex_fill.o: ../library/NVMe/legacy/nvme_task_fill_increase.h
./ioex_cmd/ioex_fill.o: ../library/NVMe/legacy/nvme_task_fill_lba.h
./ioex_cmd/ioex_fill.o: ../library/NVMe/sequence/nvme_seq_fill_const.h
./ioex_cmd/ioex_fill.o: ../library/NVMe/nvme_ioex_seq.h
./ioex_cmd/ioex_fill.o: ../library/NVMe/sequence/nvme_seq_fill_decrease.h
./ioex_cmd/ioex_fill.o: ../library/NVMe/sequence/nvme_seq_fill_increase.h
./ioex_cmd/ioex_fill.o: ../library/NVMe/sequence/nvme_seq_fill_lba.h
./ioex_cmd/ioex_compare.o: ./ioex_cmd/ioex_compare.h
./ioex_cmd/ioex_compare.o: ../library/include/global_variable.h
./ioex_cmd/ioex_compare.o: ../library/include/property.h
./ioex_cmd/ioex_compare.o: /usr/include/stdlib.h /usr/include/features.h
./ioex_cmd/ioex_compare.o: /usr/include/stdc-predef.h
./ioex_cmd/ioex_compare.o: /usr/include/sys/cdefs.h
./ioex_cmd/ioex_compare.o: /usr/include/bits/wordsize.h
./ioex_cmd/ioex_compare.o: /usr/include/gnu/stubs.h
./ioex_cmd/ioex_compare.o: /usr/include/bits/waitflags.h
./ioex_cmd/ioex_compare.o: /usr/include/bits/waitstatus.h
./ioex_cmd/ioex_compare.o: /usr/include/endian.h /usr/include/bits/endian.h
./ioex_cmd/ioex_compare.o: /usr/include/bits/byteswap.h
./ioex_cmd/ioex_compare.o: /usr/include/bits/types.h
./ioex_cmd/ioex_compare.o: /usr/include/bits/typesizes.h
./ioex_cmd/ioex_compare.o: /usr/include/bits/byteswap-16.h
./ioex_cmd/ioex_compare.o: /usr/include/sys/types.h /usr/include/time.h
./ioex_cmd/ioex_compare.o: /usr/include/sys/select.h
./ioex_cmd/ioex_compare.o: /usr/include/bits/select.h
./ioex_cmd/ioex_compare.o: /usr/include/bits/sigset.h
./ioex_cmd/ioex_compare.o: /usr/include/bits/time.h
./ioex_cmd/ioex_compare.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
./ioex_cmd/ioex_compare.o: /usr/include/bits/stdlib-float.h
./ioex_cmd/ioex_compare.o: /usr/include/assert.h ./ioex_cmd/ioex_cmd.h
./ioex_cmd/ioex_compare.o: ../library/include/console_arguments.h
./ioex_cmd/ioex_compare.o: ../library/NVMe/nvme_ioex.h
./ioex_cmd/ioex_compare.o: ../library/include/disk_ioex.h
./ioex_cmd/ioex_compare.o: /usr/include/sys/ioctl.h
./ioex_cmd/ioex_compare.o: /usr/include/bits/ioctls.h
./ioex_cmd/ioex_compare.o: /usr/include/asm/ioctls.h
./ioex_cmd/ioex_compare.o: /usr/include/asm-generic/ioctls.h
./ioex_cmd/ioex_compare.o: /usr/include/linux/ioctl.h
./ioex_cmd/ioex_compare.o: /usr/include/asm/ioctl.h
./ioex_cmd/ioex_compare.o: /usr/include/asm-generic/ioctl.h
./ioex_cmd/ioex_compare.o: /usr/include/bits/ioctl-types.h
./ioex_cmd/ioex_compare.o: /usr/include/sys/ttydefaults.h
./ioex_cmd/ioex_compare.o: ../library/include/disk_io.h
./ioex_cmd/ioex_compare.o: ../library/include/io_ctrl.h /usr/include/fcntl.h
./ioex_cmd/ioex_compare.o: /usr/include/bits/fcntl.h
./ioex_cmd/ioex_compare.o: /usr/include/bits/fcntl-linux.h
./ioex_cmd/ioex_compare.o: /usr/include/bits/stat.h
./ioex_cmd/ioex_compare.o: ../library/include/async_rw_handler.h
./ioex_cmd/ioex_compare.o: ../library/include/async_cmd.h
./ioex_cmd/ioex_compare.o: /usr/include/stdint.h /usr/include/bits/wchar.h
./ioex_cmd/ioex_compare.o: /usr/include/aio.h /usr/include/bits/siginfo.h
./ioex_cmd/ioex_compare.o: ../library/include/report_channel.h
./ioex_cmd/ioex_compare.o: ../library/include/thread_locker.h nvme.h
./ioex_cmd/ioex_compare.o: /usr/include/linux/types.h
./ioex_cmd/ioex_compare.o: /usr/include/asm/types.h
./ioex_cmd/ioex_compare.o: /usr/include/asm-generic/types.h
./ioex_cmd/ioex_compare.o: /usr/include/asm-generic/int-ll64.h
./ioex_cmd/ioex_compare.o: /usr/include/asm/bitsperlong.h
./ioex_cmd/ioex_compare.o: /usr/include/asm-generic/bitsperlong.h
./ioex_cmd/ioex_compare.o: /usr/include/linux/posix_types.h
./ioex_cmd/ioex_compare.o: /usr/include/linux/stddef.h
./ioex_cmd/ioex_compare.o: /usr/include/asm/posix_types.h
./ioex_cmd/ioex_compare.o: /usr/include/asm/posix_types_64.h
./ioex_cmd/ioex_compare.o: /usr/include/asm-generic/posix_types.h fun_ioex.h
./ioex_cmd/ioex_compare.o: ../library/NVMe/legacy/nvme_task_rc_const.h
./ioex_cmd/ioex_compare.o: ../library/NVMe/legacy/nvme_task_rc.h
./ioex_cmd/ioex_compare.o: ../library/include/memory.h
./ioex_cmd/ioex_compare.o: ../library/include/legacy_string.h
./ioex_cmd/ioex_compare.o: ../library/NVMe/legacy/nvme_task_rc_decrease.h
./ioex_cmd/ioex_compare.o: ../library/NVMe/legacy/nvme_task_rc_increase.h
./ioex_cmd/ioex_compare.o: ../library/NVMe/legacy/nvme_task_rc_lba.h
./ioex_cmd/ioex_compare.o: ../library/NVMe/sequence/nvme_seq_rc_const.h
./ioex_cmd/ioex_compare.o: ../library/NVMe/sequence/nvme_seq_rc.h
./ioex_cmd/ioex_compare.o: ../library/NVMe/nvme_ioex_seq.h
./ioex_cmd/ioex_compare.o: ../library/NVMe/sequence/nvme_seq_rc_decrease.h
./ioex_cmd/ioex_compare.o: ../library/NVMe/sequence/nvme_seq_rc_increase.h
./ioex_cmd/ioex_compare.o: ../library/NVMe/sequence/nvme_seq_rc_lba.h
./fop_cmd/fop_cmd.o: ./fop_cmd/fop_cmd.h /usr/include/malloc.h
./fop_cmd/fop_cmd.o: /usr/include/features.h /usr/include/stdc-predef.h
./fop_cmd/fop_cmd.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
./fop_cmd/fop_cmd.o: /usr/include/gnu/stubs.h /usr/include/stdio.h
./fop_cmd/fop_cmd.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
./fop_cmd/fop_cmd.o: /usr/include/libio.h /usr/include/_G_config.h
./fop_cmd/fop_cmd.o: /usr/include/wchar.h /usr/include/bits/wchar.h
./fop_cmd/fop_cmd.o: /usr/include/xlocale.h /usr/include/bits/stdio_lim.h
./fop_cmd/fop_cmd.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
./fop_cmd/fop_cmd.o: ../library/include/console_arguments.h
./fop_cmd/fop_cmd.o: ../library/include/global_variable.h
./fop_cmd/fop_cmd.o: ../library/include/property.h /usr/include/stdlib.h
./fop_cmd/fop_cmd.o: /usr/include/bits/waitflags.h
./fop_cmd/fop_cmd.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
./fop_cmd/fop_cmd.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
./fop_cmd/fop_cmd.o: /usr/include/bits/byteswap-16.h /usr/include/sys/types.h
./fop_cmd/fop_cmd.o: /usr/include/time.h /usr/include/sys/select.h
./fop_cmd/fop_cmd.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
./fop_cmd/fop_cmd.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
./fop_cmd/fop_cmd.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
./fop_cmd/fop_cmd.o: /usr/include/assert.h ../library/include/disk_ioex.h
./fop_cmd/fop_cmd.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
./fop_cmd/fop_cmd.o: /usr/include/asm/ioctls.h
./fop_cmd/fop_cmd.o: /usr/include/asm-generic/ioctls.h
./fop_cmd/fop_cmd.o: /usr/include/linux/ioctl.h /usr/include/asm/ioctl.h
./fop_cmd/fop_cmd.o: /usr/include/asm-generic/ioctl.h
./fop_cmd/fop_cmd.o: /usr/include/bits/ioctl-types.h
./fop_cmd/fop_cmd.o: /usr/include/sys/ttydefaults.h
./fop_cmd/fop_cmd.o: ../library/include/disk_io.h
./fop_cmd/fop_cmd.o: ../library/include/io_ctrl.h /usr/include/fcntl.h
./fop_cmd/fop_cmd.o: /usr/include/bits/fcntl.h
./fop_cmd/fop_cmd.o: /usr/include/bits/fcntl-linux.h /usr/include/bits/stat.h
./fop_cmd/fop_cmd.o: ../library/include/async_rw_handler.h
./fop_cmd/fop_cmd.o: ../library/include/async_cmd.h /usr/include/stdint.h
./fop_cmd/fop_cmd.o: /usr/include/aio.h /usr/include/bits/siginfo.h
./fop_cmd/fop_cmd.o: ../library/include/report_channel.h
./fop_cmd/fop_cmd.o: ../library/include/thread_locker.h
./fop_cmd/fop_cmd.o: ../library/include/memory.h
./fop_cmd/fop_cmd.o: ../library/include/legacy_string.h
./fop_cmd/fop_read.o: file_operation.h ../library/include/console_arguments.h
./fop_cmd/fop_read.o: ../library/include/global_variable.h
./fop_cmd/fop_read.o: ../library/include/property.h /usr/include/stdlib.h
./fop_cmd/fop_read.o: /usr/include/features.h /usr/include/stdc-predef.h
./fop_cmd/fop_read.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
./fop_cmd/fop_read.o: /usr/include/gnu/stubs.h /usr/include/bits/waitflags.h
./fop_cmd/fop_read.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
./fop_cmd/fop_read.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
./fop_cmd/fop_read.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
./fop_cmd/fop_read.o: /usr/include/bits/byteswap-16.h
./fop_cmd/fop_read.o: /usr/include/sys/types.h /usr/include/time.h
./fop_cmd/fop_read.o: /usr/include/sys/select.h /usr/include/bits/select.h
./fop_cmd/fop_read.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
./fop_cmd/fop_read.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
./fop_cmd/fop_read.o: /usr/include/bits/stdlib-float.h /usr/include/assert.h
./fop_cmd/fop_read.o: ./fop_cmd/fop_read.h ./fop_cmd/fop_cmd.h
./fop_cmd/fop_read.o: /usr/include/malloc.h /usr/include/stdio.h
./fop_cmd/fop_read.o: /usr/include/libio.h /usr/include/_G_config.h
./fop_cmd/fop_read.o: /usr/include/wchar.h /usr/include/bits/wchar.h
./fop_cmd/fop_read.o: /usr/include/xlocale.h /usr/include/bits/stdio_lim.h
./fop_cmd/fop_read.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
./fop_cmd/fop_read.o: ../library/include/disk_ioex.h /usr/include/sys/ioctl.h
./fop_cmd/fop_read.o: /usr/include/bits/ioctls.h /usr/include/asm/ioctls.h
./fop_cmd/fop_read.o: /usr/include/asm-generic/ioctls.h
./fop_cmd/fop_read.o: /usr/include/linux/ioctl.h /usr/include/asm/ioctl.h
./fop_cmd/fop_read.o: /usr/include/asm-generic/ioctl.h
./fop_cmd/fop_read.o: /usr/include/bits/ioctl-types.h
./fop_cmd/fop_read.o: /usr/include/sys/ttydefaults.h
./fop_cmd/fop_read.o: ../library/include/disk_io.h
./fop_cmd/fop_read.o: ../library/include/io_ctrl.h /usr/include/fcntl.h
./fop_cmd/fop_read.o: /usr/include/bits/fcntl.h
./fop_cmd/fop_read.o: /usr/include/bits/fcntl-linux.h
./fop_cmd/fop_read.o: /usr/include/bits/stat.h
./fop_cmd/fop_read.o: ../library/include/async_rw_handler.h
./fop_cmd/fop_read.o: ../library/include/async_cmd.h /usr/include/stdint.h
./fop_cmd/fop_read.o: /usr/include/aio.h /usr/include/bits/siginfo.h
./fop_cmd/fop_read.o: ../library/include/report_channel.h
./fop_cmd/fop_read.o: ../library/include/thread_locker.h
./fop_cmd/fop_read.o: ../library/include/legacy_string.h
./fop_cmd/fop_write.o: file_operation.h
./fop_cmd/fop_write.o: ../library/include/console_arguments.h
./fop_cmd/fop_write.o: ../library/include/global_variable.h
./fop_cmd/fop_write.o: ../library/include/property.h /usr/include/stdlib.h
./fop_cmd/fop_write.o: /usr/include/features.h /usr/include/stdc-predef.h
./fop_cmd/fop_write.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
./fop_cmd/fop_write.o: /usr/include/gnu/stubs.h /usr/include/bits/waitflags.h
./fop_cmd/fop_write.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
./fop_cmd/fop_write.o: /usr/include/bits/endian.h
./fop_cmd/fop_write.o: /usr/include/bits/byteswap.h /usr/include/bits/types.h
./fop_cmd/fop_write.o: /usr/include/bits/typesizes.h
./fop_cmd/fop_write.o: /usr/include/bits/byteswap-16.h
./fop_cmd/fop_write.o: /usr/include/sys/types.h /usr/include/time.h
./fop_cmd/fop_write.o: /usr/include/sys/select.h /usr/include/bits/select.h
./fop_cmd/fop_write.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
./fop_cmd/fop_write.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
./fop_cmd/fop_write.o: /usr/include/bits/stdlib-float.h /usr/include/assert.h
./fop_cmd/fop_write.o: ./fop_cmd/fop_write.h ./fop_cmd/fop_cmd.h
./fop_cmd/fop_write.o: /usr/include/malloc.h /usr/include/stdio.h
./fop_cmd/fop_write.o: /usr/include/libio.h /usr/include/_G_config.h
./fop_cmd/fop_write.o: /usr/include/wchar.h /usr/include/bits/wchar.h
./fop_cmd/fop_write.o: /usr/include/xlocale.h /usr/include/bits/stdio_lim.h
./fop_cmd/fop_write.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
./fop_cmd/fop_write.o: ../library/include/disk_ioex.h
./fop_cmd/fop_write.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
./fop_cmd/fop_write.o: /usr/include/asm/ioctls.h
./fop_cmd/fop_write.o: /usr/include/asm-generic/ioctls.h
./fop_cmd/fop_write.o: /usr/include/linux/ioctl.h /usr/include/asm/ioctl.h
./fop_cmd/fop_write.o: /usr/include/asm-generic/ioctl.h
./fop_cmd/fop_write.o: /usr/include/bits/ioctl-types.h
./fop_cmd/fop_write.o: /usr/include/sys/ttydefaults.h
./fop_cmd/fop_write.o: ../library/include/disk_io.h
./fop_cmd/fop_write.o: ../library/include/io_ctrl.h /usr/include/fcntl.h
./fop_cmd/fop_write.o: /usr/include/bits/fcntl.h
./fop_cmd/fop_write.o: /usr/include/bits/fcntl-linux.h
./fop_cmd/fop_write.o: /usr/include/bits/stat.h
./fop_cmd/fop_write.o: ../library/include/async_rw_handler.h
./fop_cmd/fop_write.o: ../library/include/async_cmd.h /usr/include/stdint.h
./fop_cmd/fop_write.o: /usr/include/aio.h /usr/include/bits/siginfo.h
./fop_cmd/fop_write.o: ../library/include/report_channel.h
./fop_cmd/fop_write.o: ../library/include/thread_locker.h
./fop_cmd/fop_write.o: ../library/include/legacy_string.h
./fop_cmd/fop_write.o: ../library/include/memory.h
../library/src/thread_locker.o: ../library/include/thread_locker.h
../library/src/thread_locker.o: /usr/include/unistd.h /usr/include/features.h
../library/src/thread_locker.o: /usr/include/stdc-predef.h
../library/src/thread_locker.o: /usr/include/sys/cdefs.h
../library/src/thread_locker.o: /usr/include/bits/wordsize.h
../library/src/thread_locker.o: /usr/include/gnu/stubs.h
../library/src/thread_locker.o: /usr/include/bits/posix_opt.h
../library/src/thread_locker.o: /usr/include/bits/environments.h
../library/src/thread_locker.o: /usr/include/bits/types.h
../library/src/thread_locker.o: /usr/include/bits/typesizes.h
../library/src/thread_locker.o: /usr/include/bits/confname.h
../library/src/thread_locker.o: /usr/include/getopt.h
../library/src/async_cmd.o: /usr/include/stdlib.h /usr/include/features.h
../library/src/async_cmd.o: /usr/include/stdc-predef.h
../library/src/async_cmd.o: /usr/include/sys/cdefs.h
../library/src/async_cmd.o: /usr/include/bits/wordsize.h
../library/src/async_cmd.o: /usr/include/gnu/stubs.h
../library/src/async_cmd.o: /usr/include/bits/waitflags.h
../library/src/async_cmd.o: /usr/include/bits/waitstatus.h
../library/src/async_cmd.o: /usr/include/endian.h /usr/include/bits/endian.h
../library/src/async_cmd.o: /usr/include/bits/byteswap.h
../library/src/async_cmd.o: /usr/include/bits/types.h
../library/src/async_cmd.o: /usr/include/bits/typesizes.h
../library/src/async_cmd.o: /usr/include/bits/byteswap-16.h
../library/src/async_cmd.o: /usr/include/sys/types.h /usr/include/time.h
../library/src/async_cmd.o: /usr/include/sys/select.h
../library/src/async_cmd.o: /usr/include/bits/select.h
../library/src/async_cmd.o: /usr/include/bits/sigset.h
../library/src/async_cmd.o: /usr/include/bits/time.h
../library/src/async_cmd.o: /usr/include/sys/sysmacros.h
../library/src/async_cmd.o: /usr/include/alloca.h
../library/src/async_cmd.o: /usr/include/bits/stdlib-float.h
../library/src/async_cmd.o: /usr/include/errno.h /usr/include/bits/errno.h
../library/src/async_cmd.o: /usr/include/linux/errno.h
../library/src/async_cmd.o: /usr/include/asm/errno.h
../library/src/async_cmd.o: /usr/include/asm-generic/errno.h
../library/src/async_cmd.o: /usr/include/asm-generic/errno-base.h
../library/src/async_cmd.o: ../library/include/async_cmd.h
../library/src/async_cmd.o: /usr/include/stdint.h /usr/include/bits/wchar.h
../library/src/async_cmd.o: /usr/include/aio.h /usr/include/bits/siginfo.h
../library/src/async_cmd.o: ../library/include/property.h
../library/src/async_cmd.o: /usr/include/assert.h
../library/src/async_rw_handler.o: ../library/include/memory.h
../library/src/async_rw_handler.o: /usr/include/stdint.h
../library/src/async_rw_handler.o: /usr/include/features.h
../library/src/async_rw_handler.o: /usr/include/stdc-predef.h
../library/src/async_rw_handler.o: /usr/include/sys/cdefs.h
../library/src/async_rw_handler.o: /usr/include/bits/wordsize.h
../library/src/async_rw_handler.o: /usr/include/gnu/stubs.h
../library/src/async_rw_handler.o: /usr/include/bits/wchar.h
../library/src/async_rw_handler.o: ../library/include/property.h
../library/src/async_rw_handler.o: /usr/include/stdlib.h
../library/src/async_rw_handler.o: /usr/include/bits/waitflags.h
../library/src/async_rw_handler.o: /usr/include/bits/waitstatus.h
../library/src/async_rw_handler.o: /usr/include/endian.h
../library/src/async_rw_handler.o: /usr/include/bits/endian.h
../library/src/async_rw_handler.o: /usr/include/bits/byteswap.h
../library/src/async_rw_handler.o: /usr/include/bits/types.h
../library/src/async_rw_handler.o: /usr/include/bits/typesizes.h
../library/src/async_rw_handler.o: /usr/include/bits/byteswap-16.h
../library/src/async_rw_handler.o: /usr/include/sys/types.h
../library/src/async_rw_handler.o: /usr/include/time.h
../library/src/async_rw_handler.o: /usr/include/sys/select.h
../library/src/async_rw_handler.o: /usr/include/bits/select.h
../library/src/async_rw_handler.o: /usr/include/bits/sigset.h
../library/src/async_rw_handler.o: /usr/include/bits/time.h
../library/src/async_rw_handler.o: /usr/include/sys/sysmacros.h
../library/src/async_rw_handler.o: /usr/include/alloca.h
../library/src/async_rw_handler.o: /usr/include/bits/stdlib-float.h
../library/src/async_rw_handler.o: /usr/include/assert.h
../library/src/async_rw_handler.o: ../library/include/legacy_string.h
../library/src/async_rw_handler.o: ../library/include/async_rw_handler.h
../library/src/async_rw_handler.o: ../library/include/async_cmd.h
../library/src/async_rw_handler.o: /usr/include/aio.h
../library/src/async_rw_handler.o: /usr/include/bits/siginfo.h
../library/src/extension_string.o: ../library/include/extension_string.h
../library/src/extension_string.o: ../library/include/property.h
../library/src/extension_string.o: /usr/include/stdlib.h
../library/src/extension_string.o: /usr/include/features.h
../library/src/extension_string.o: /usr/include/stdc-predef.h
../library/src/extension_string.o: /usr/include/sys/cdefs.h
../library/src/extension_string.o: /usr/include/bits/wordsize.h
../library/src/extension_string.o: /usr/include/gnu/stubs.h
../library/src/extension_string.o: /usr/include/bits/waitflags.h
../library/src/extension_string.o: /usr/include/bits/waitstatus.h
../library/src/extension_string.o: /usr/include/endian.h
../library/src/extension_string.o: /usr/include/bits/endian.h
../library/src/extension_string.o: /usr/include/bits/byteswap.h
../library/src/extension_string.o: /usr/include/bits/types.h
../library/src/extension_string.o: /usr/include/bits/typesizes.h
../library/src/extension_string.o: /usr/include/bits/byteswap-16.h
../library/src/extension_string.o: /usr/include/sys/types.h
../library/src/extension_string.o: /usr/include/time.h
../library/src/extension_string.o: /usr/include/sys/select.h
../library/src/extension_string.o: /usr/include/bits/select.h
../library/src/extension_string.o: /usr/include/bits/sigset.h
../library/src/extension_string.o: /usr/include/bits/time.h
../library/src/extension_string.o: /usr/include/sys/sysmacros.h
../library/src/extension_string.o: /usr/include/alloca.h
../library/src/extension_string.o: /usr/include/bits/stdlib-float.h
../library/src/extension_string.o: /usr/include/assert.h
../library/src/legacy_string.o: /usr/include/stdio.h /usr/include/features.h
../library/src/legacy_string.o: /usr/include/stdc-predef.h
../library/src/legacy_string.o: /usr/include/sys/cdefs.h
../library/src/legacy_string.o: /usr/include/bits/wordsize.h
../library/src/legacy_string.o: /usr/include/gnu/stubs.h
../library/src/legacy_string.o: /usr/include/bits/types.h
../library/src/legacy_string.o: /usr/include/bits/typesizes.h
../library/src/legacy_string.o: /usr/include/libio.h /usr/include/_G_config.h
../library/src/legacy_string.o: /usr/include/wchar.h
../library/src/legacy_string.o: /usr/include/bits/wchar.h
../library/src/legacy_string.o: /usr/include/xlocale.h
../library/src/legacy_string.o: /usr/include/bits/stdio_lim.h
../library/src/legacy_string.o: /usr/include/bits/sys_errlist.h
../library/src/legacy_string.o: ../library/include/legacy_string.h
../library/src/legacy_string.o: ../library/include/property.h
../library/src/legacy_string.o: /usr/include/stdlib.h
../library/src/legacy_string.o: /usr/include/bits/waitflags.h
../library/src/legacy_string.o: /usr/include/bits/waitstatus.h
../library/src/legacy_string.o: /usr/include/endian.h
../library/src/legacy_string.o: /usr/include/bits/endian.h
../library/src/legacy_string.o: /usr/include/bits/byteswap.h
../library/src/legacy_string.o: /usr/include/bits/byteswap-16.h
../library/src/legacy_string.o: /usr/include/sys/types.h /usr/include/time.h
../library/src/legacy_string.o: /usr/include/sys/select.h
../library/src/legacy_string.o: /usr/include/bits/select.h
../library/src/legacy_string.o: /usr/include/bits/sigset.h
../library/src/legacy_string.o: /usr/include/bits/time.h
../library/src/legacy_string.o: /usr/include/sys/sysmacros.h
../library/src/legacy_string.o: /usr/include/alloca.h
../library/src/legacy_string.o: /usr/include/bits/stdlib-float.h
../library/src/legacy_string.o: /usr/include/assert.h
../library/src/global_variable.o: ../library/include/global_variable.h
../library/src/global_variable.o: ../library/include/property.h
../library/src/global_variable.o: /usr/include/stdlib.h
../library/src/global_variable.o: /usr/include/features.h
../library/src/global_variable.o: /usr/include/stdc-predef.h
../library/src/global_variable.o: /usr/include/sys/cdefs.h
../library/src/global_variable.o: /usr/include/bits/wordsize.h
../library/src/global_variable.o: /usr/include/gnu/stubs.h
../library/src/global_variable.o: /usr/include/bits/waitflags.h
../library/src/global_variable.o: /usr/include/bits/waitstatus.h
../library/src/global_variable.o: /usr/include/endian.h
../library/src/global_variable.o: /usr/include/bits/endian.h
../library/src/global_variable.o: /usr/include/bits/byteswap.h
../library/src/global_variable.o: /usr/include/bits/types.h
../library/src/global_variable.o: /usr/include/bits/typesizes.h
../library/src/global_variable.o: /usr/include/bits/byteswap-16.h
../library/src/global_variable.o: /usr/include/sys/types.h
../library/src/global_variable.o: /usr/include/time.h
../library/src/global_variable.o: /usr/include/sys/select.h
../library/src/global_variable.o: /usr/include/bits/select.h
../library/src/global_variable.o: /usr/include/bits/sigset.h
../library/src/global_variable.o: /usr/include/bits/time.h
../library/src/global_variable.o: /usr/include/sys/sysmacros.h
../library/src/global_variable.o: /usr/include/alloca.h
../library/src/global_variable.o: /usr/include/bits/stdlib-float.h
../library/src/global_variable.o: /usr/include/assert.h
../library/src/console_arguments.o: ../library/include/extension_string.h
../library/src/console_arguments.o: ../library/include/property.h
../library/src/console_arguments.o: /usr/include/stdlib.h
../library/src/console_arguments.o: /usr/include/features.h
../library/src/console_arguments.o: /usr/include/stdc-predef.h
../library/src/console_arguments.o: /usr/include/sys/cdefs.h
../library/src/console_arguments.o: /usr/include/bits/wordsize.h
../library/src/console_arguments.o: /usr/include/gnu/stubs.h
../library/src/console_arguments.o: /usr/include/bits/waitflags.h
../library/src/console_arguments.o: /usr/include/bits/waitstatus.h
../library/src/console_arguments.o: /usr/include/endian.h
../library/src/console_arguments.o: /usr/include/bits/endian.h
../library/src/console_arguments.o: /usr/include/bits/byteswap.h
../library/src/console_arguments.o: /usr/include/bits/types.h
../library/src/console_arguments.o: /usr/include/bits/typesizes.h
../library/src/console_arguments.o: /usr/include/bits/byteswap-16.h
../library/src/console_arguments.o: /usr/include/sys/types.h
../library/src/console_arguments.o: /usr/include/time.h
../library/src/console_arguments.o: /usr/include/sys/select.h
../library/src/console_arguments.o: /usr/include/bits/select.h
../library/src/console_arguments.o: /usr/include/bits/sigset.h
../library/src/console_arguments.o: /usr/include/bits/time.h
../library/src/console_arguments.o: /usr/include/sys/sysmacros.h
../library/src/console_arguments.o: /usr/include/alloca.h
../library/src/console_arguments.o: /usr/include/bits/stdlib-float.h
../library/src/console_arguments.o: /usr/include/assert.h
../library/src/console_arguments.o: ../library/include/console_arguments.h
../library/src/console_arguments.o: ../library/include/global_variable.h
../library/src/console_arguments.o: ../library/include/sys_ctrl.h
../library/src/console_arguments.o: /usr/include/linux/limits.h
../library/src/console_arguments.o: ../library/include/file_ctrl.h
../library/src/sys_ctrl.o: /usr/include/unistd.h /usr/include/features.h
../library/src/sys_ctrl.o: /usr/include/stdc-predef.h
../library/src/sys_ctrl.o: /usr/include/sys/cdefs.h
../library/src/sys_ctrl.o: /usr/include/bits/wordsize.h
../library/src/sys_ctrl.o: /usr/include/gnu/stubs.h
../library/src/sys_ctrl.o: /usr/include/bits/posix_opt.h
../library/src/sys_ctrl.o: /usr/include/bits/environments.h
../library/src/sys_ctrl.o: /usr/include/bits/types.h
../library/src/sys_ctrl.o: /usr/include/bits/typesizes.h
../library/src/sys_ctrl.o: /usr/include/bits/confname.h /usr/include/getopt.h
../library/src/sys_ctrl.o: ../library/include/sys_ctrl.h
../library/src/sys_ctrl.o: /usr/include/linux/limits.h
../library/src/file_ctrl.o: /usr/include/unistd.h /usr/include/features.h
../library/src/file_ctrl.o: /usr/include/stdc-predef.h
../library/src/file_ctrl.o: /usr/include/sys/cdefs.h
../library/src/file_ctrl.o: /usr/include/bits/wordsize.h
../library/src/file_ctrl.o: /usr/include/gnu/stubs.h
../library/src/file_ctrl.o: /usr/include/bits/posix_opt.h
../library/src/file_ctrl.o: /usr/include/bits/environments.h
../library/src/file_ctrl.o: /usr/include/bits/types.h
../library/src/file_ctrl.o: /usr/include/bits/typesizes.h
../library/src/file_ctrl.o: /usr/include/bits/confname.h
../library/src/file_ctrl.o: /usr/include/getopt.h /usr/include/time.h
../library/src/file_ctrl.o: ../library/include/file_ctrl.h
../library/src/file_ctrl.o: /usr/include/linux/limits.h
