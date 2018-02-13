#increase build version
if [ -n "$1" ]
	then 
	if [ $1 = "+" ]
		then echo "increase fw build"
		../../nvme/tool/fwbuilder ./ioctl_vendor.h
	fi
fi

make clean

rm -f moc_* *.exe Makefile

#find . -name \*.exe -type f -delete
#find . -name \*.o -type f -delete

#tar zcvf cppProj.tar.gz *
