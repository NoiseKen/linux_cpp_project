#increase build version
#./nvme/tool/fwbuilder ./qt/nxgn_nvme/ioctl_vendor.h
if [ $# -lt 1 ]
then echo "please give backup folder name!!"
	exit -1
fi

TARGET_FOLDER=$1
echo "target folder = $TARGET_FOLDER"
if [ ! -d "./$TARGET_FOLDER" ]
then echo "Target Folder [$TARGET_FOLDER] not exist !!"
	exit -1
fi

rm -f ./$TARGET_FOLDER.tar.gz

cd ./TARGET_FOLDER
make clean

#find . -name \*.exe -type f -delete
#find . -name \*.o -type f -delete

this_shell=${0#*/}
tar zcvf $TARGET_FOLDER.tar.gz $TARGET_FOLDER $this_shell



