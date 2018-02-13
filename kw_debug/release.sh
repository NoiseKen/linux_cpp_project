CURRENT_PWD=$PWD
TARGET_FOLDER=$CURRENT_PWD/kw_debug
TAG_TO=kwdebug.tar.gz
REF_FILE= 
DATETIME=$(date "+%Y-%m-%d@%H:%M:%S")
OS_TYPE=$(uname -m)
ABS_TARGET_FOLDER=$CURRENT_PWD/$TARGET_FOLDER
ABS_TARGET_FILE=$ABS_TARGET_FOLDER/$TARGET_FILE
ABS_TMP_FOLDER=$ABS_TARGET_FOLDER"_tmp"

ME="$(basename "$(test -L "$0" && readlink "$0" || echo "$0")")"

hintColor=$'\033[38;5;51m'
passColor=$'\033[38;5;46m'
failColor=$'\033[38;5;196m'
standardColor=$'\033[39m'

print_hint()
{
    printf "%s$1\n%s" $hintColor $standardColor
}

print_pass()
{
    printf "%s$1\n%s" $passColor $standardColor
}

print_fail()
{
    printf "%s$1\n%s" $failColor $standardColor
}


if [ -d $TARGET_FOLDER ]; then
    rm -rf "$TARGET_FOLDER"
fi

if [ -f $TAG_TO ]; then
    rm -f "$TAG_TO"
fi

print_hint "re build kernel moduler"
cd ../../../../
make clean M=drivers/nvme
make M=drivers/nvme
cd $CURRENT_PWD

print_hint "copy source file to target folder"
mkdir $TARGET_FOLDER
mkdir $TARGET_FOLDER/ref
cp *.c $TARGET_FOLDER/
cp *.h $TARGET_FOLDER/
cp ../pci.c $TARGET_FOLDER/ref/pci.c
cp ../core.c $TARGET_FOLDER/ref/core.c
cp ../Makefile $TARGET_FOLDER/ref/Makefile
cp $ME  $TARGET_FOLDER/$ME

print_hint "copy ko file to target folder"
cp ../nvme.ko $TARGET_FOLDER/nvme.ko
cp ../nvme-core.ko $TARGET_FOLDER/nvme-core.ko

print_hint "tag target folder"
tar zcvf $TAG_TO kw_debug

print_hint "remove temp resource"
rm -rf $TARGET_FOLDER
