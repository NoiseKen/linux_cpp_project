./tool/fwbuilder ./myIOCTL/ioctl_vendor.h
cd ./myIOCTL
make
cd ../myNVMe
make

#cd ../
#rm -f nxgn_nvme_tool.tar.gz
#tar zcvf nxgn_nvme_tool.tar.gz myIOCTL myNVMe tool nvme-compatibility
