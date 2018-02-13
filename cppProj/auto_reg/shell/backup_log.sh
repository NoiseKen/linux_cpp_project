#!/bin/bash
# $1 is tar name
# other $ is the back up file/directory
TAR_NAME=$1
TAR_FOLDER=$2
SH_RST=$PWD/shrst
RUNNING_HINT=$PWD/shpid
echo $$ > $RUNNING_HINT
rm -f $TAR_NAME.tar.gz

#tar zcvf $TAR_NAME.tar.gz log_tnvme lspci.log
tar zcvf $TAR_NAME.tar.gz $TAR_FOLDER
rst=$?
rm -f $RUNNING_HINT
echo $rst > $SH_RST
