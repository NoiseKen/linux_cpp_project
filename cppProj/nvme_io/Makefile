# Copyright (c) 2017, Ken Wang
#
# Compiling pkg requires : None
# 

CFLAG = -shared -fPIC -Wall
LIB_NAME = nvme_io.so

all:make_lib

make_lib:nvme_io.c
	$(CC) -o $(LIB_NAME) $(CFLAG) nvme_io.c

clean:
	rm -f *.so
