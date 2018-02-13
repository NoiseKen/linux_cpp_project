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
EXE = fio_iops_postprocess.exe
LIBRARY = ../../library
LIB_INC = $(LIBRARY)/include
LIB_SRC = $(LIBRARY)/src
DIST = $(shell uname -r)
INCLUDES = -I./ -I$(LIB_INC)
#-I../myNVMe/include
DFLAGS = -std=c++0x
SOURCES = \
	main.cpp	\
	$(LIB_SRC)/extension_string.cpp	\
	$(LIB_SRC)/string_list.cpp	\
	$(LIB_SRC)/utils.c
	
all:
	$(CC) $(INCLUDES) $(DFLAGS) $(SOURCES) -o $(EXE)

clean:
	rm -f *.bin *.exe *.log *.so *.stackdump *.d $(EXE) *~
	rm -rf tmp
