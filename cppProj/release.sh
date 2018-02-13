CURRENT_PWD=$PWD
TARGET_FOLDER=auto_reg
TARGET_FILE=main.cpp
DATETIME=$(date "+%Y-%m-%d@%H:%M:%S")
OS_TYPE=$(uname -m)
ABS_TARGET_FOLDER=$CURRENT_PWD/$TARGET_FOLDER
ABS_TARGET_FILE=$ABS_TARGET_FOLDER/$TARGET_FILE
ABS_TMP_FOLDER=$ABS_TARGET_FOLDER"_tmp"

#argv 1 for path
cd_pwd()
{
	cd $1
}

#increase build version
echo "increase project ver builder"
./tools/fw_builder/fwbuilder $ABS_TARGET_FILE "#define VER_BUILD "

#compile new exe
cd_pwd $TARGET_FOLDER
make clean all
cd_pwd $CURRENT_PWD

mv $ABS_TARGET_FOLDER $ABS_TMP_FOLDER
mkdir $ABS_TARGET_FOLDER
#keep nessary file only 
cp $ABS_TMP_FOLDER/*.exe $ABS_TARGET_FOLDER/.
cp $ABS_TMP_FOLDER/*.sh $ABS_TARGET_FOLDER/.
cp -r $ABS_TMP_FOLDER/cfg $ABS_TARGET_FOLDER/cfg
#change name auto_reg.cfg to auto_reg.cfg.bak
mv $ABS_TARGET_FOLDER/cfg/auto_reg.cfg $ABS_TARGET_FOLDER/cfg/auto_reg.cfg.bak
cp -r $ABS_TMP_FOLDER/shell $ABS_TARGET_FOLDER/shell
#tar project folder
tar zcvf $TARGET_FOLDER.tar.gz $TARGET_FOLDER
#recover target folder
rm -rf $ABS_TARGET_FOLDER
mv $ABS_TMP_FOLDER $ABS_TARGET_FOLDER


#rm -f cppProj.tar.gz

#cd ./nvme/myIOCTL
#make clean

#cd ../myNVMe
#make clean

#cd ../../qt/nxgn_nvme
#make clean

#cd ../../
#rm -rf ./cppProj/auto_reg/cmd
#rm -rf ./cppProj/auto_reg/status
#rm -rf ./cppProj/auto_reg/log_tnvme
#rm -f ./cppProj/auto_reg/regression*


#find . -name \*.exe -type f -delete
#find . -name \*.o -type f -delete

#tar zcvf cppProj.tar.gz cppProj backup.sh



