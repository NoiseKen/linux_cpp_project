#increase build version
#./nvme/tool/fwbuilder ./qt/nxgn_nvme/ioctl_vendor.h


rm -f cppProj.tar.gz

#cd ./nvme/myIOCTL
#make clean

#cd ../myNVMe
#make clean

#cd ../../qt/nxgn_nvme
#make clean

#cd ../../
rm -rf ./cppProj/auto_reg/cmd
rm -rf ./cppProj/auto_reg/status
rm -rf ./cppProj/auto_reg/fwbin
rm -rf ./cppProj/auto_reg/log_tnvme
rm -f ./cppProj/auto_reg/regression*


find . -name \*.exe -type f -delete
find . -name \*.o -type f -delete

this_shell=${0#*/}
tar zcvf cppProj.tar.gz cppProj $this_shell



