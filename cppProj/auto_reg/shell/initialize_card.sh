#common head
. $PWD/shell/common
#shell head
MP_TOOLS_ROOT=$1
SVN_FW_ROOT=$2
#shell start
shell_running

echo "rmmod dnvme"; rmmod dnvme
echo "rmmod nvme"; rmmod nvme
sleep 3
echo "modprobe nvme"; modprobe nvme
sleep 3
cd_pwd $MP_TOOLS_ROOT

if [ OS_TYPE=="X86_64" ]
then MP_TOOLS="./NVMe_Tool_64"; MERGE_TOOLS="./VCT_Tool_64"
else MP_TOOLS="./NVMe_Tool_32"; MERGE_TOOLS="./VCT_Tool_32"
fi


if [ $TRUE ]
then 
	#FW bin file move
	cp $SVN_FW_ROOT/2260_MPISP_Package.bin .
	#SVN1017 can not complete card init ,skip it
	cp $SVN_FW_ROOT/2260_ISP_Package.bin ./REGBIN/2260AA/.
	$MERGE_TOOLS MPISP 2260_MPISP_Package.bin > $SH_RST  2>&1
	rst=$?
	echo "$rst" >> $SH_RST
	if [ $rst -ne 0 ]
	then shell_exit_if_error $rst "Merge MPISP fail"
	fi
fi

#$MP_TOOLS /dev/nvme0n1 initial
$MP_TOOLS /dev/nvme0n1 initial > $SH_RST  2>&1
#$MP_TOOLS /dev/nvme0n1 initial | tee $SH_RST
#stdbuf -i0 -o0 -e0 $MP_TOOLS /dev/nvme0n1 initial | tee $SH_RST
rst="$?"

#sleep 1
#CHECK_RST=$(cat $SH_RST | grep ErrorCode:)

#echo $CHECK_RST
#if [ -n "$CHECK_RST" ]
#then rst="${CHECK_RST#ErrorCode:}"; rst="${rst#"${rst%%[![:space:]]*}"}"
#else rst="0"
#fi
#echo "result=$rst"
cd_pwd $CURRENT_PWD

echo $DATETIME >> $SH_RST
echo $rst >> $SH_RST

shell_exit
#exit $rst
