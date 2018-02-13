# Copyright (c) 2011, Intel Corporation.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#

# Compiling tnvme requires boost, XML, doxygen, etc libraries
# See: https://github.com/nvmecompliance/tnvme/wiki/Compiling

CC  = g++ -O -Wall -Wno-unused-result
EXE = AutoREG.exe
LIBRARY = ../library
LIB_INC = $(LIBRARY)/include
LIB_SRC = $(LIBRARY)/src
LOCAL_SRC = ./src
SHELL_SRC = $(LOCAL_SRC)/shell
SCRIPT_SRC = $(LOCAL_SRC)/script

DIST = $(shell uname -r)
INCLUDES = -I./ -I$(LIB_INC) -I$(LOCAL_SRC) -I$(SHELL_SRC) -I$(SCRIPT_SRC)
#-I../myNVMe/include
DFLAGS = -std=c++0x
SOURCES = \
	main.cpp	\
	$(LOCAL_SRC)/auto_reg_cfg.cpp	\
	$(LOCAL_SRC)/auto_reg_manager.cpp	\
	$(LOCAL_SRC)/auto_reg_server.cpp	\
	$(SCRIPT_SRC)/shell_script.cpp	\
	$(SCRIPT_SRC)/tnvme_script.cpp	\
	$(SCRIPT_SRC)/init_card_script.cpp	\
	$(SCRIPT_SRC)/auto_log_backup_script.cpp	\
	$(SCRIPT_SRC)/fio_test_script.cpp	\
	$(SCRIPT_SRC)/svn_check_script.cpp	\
	$(SHELL_SRC)/extern_shell.cpp	\
	$(SHELL_SRC)/tnvme_shell.cpp	\
	$(SHELL_SRC)/tnvme_log_backup_shell.cpp	\
	$(SHELL_SRC)/initialize_card_shell.cpp	\
	$(SHELL_SRC)/fio_test_shell.cpp	\
	$(SHELL_SRC)/svn_check_shell.cpp	\
	$(LIB_SRC)/file_ctrl.cpp	\
	$(LIB_SRC)/directory_ctrl.cpp	\
	$(LIB_SRC)/log_file.cpp	\
	$(LIB_SRC)/legacy_string.cpp	\
	$(LIB_SRC)/timer_ctrl.cpp	\
	$(LIB_SRC)/extension_string.cpp	\
	$(LIB_SRC)/string_list.cpp	\
	$(LIB_SRC)/value_list.cpp	\
	$(LIB_SRC)/network_app.cpp	\
	$(LIB_SRC)/utils.c
	
all:
	$(CC) $(INCLUDES) $(DFLAGS) $(SOURCES) -o $(EXE)

clean:
	rm -f *.bin *.exe *.log *.so *.stackdump *.d $(EXE) *~
	rm -rf tmp
