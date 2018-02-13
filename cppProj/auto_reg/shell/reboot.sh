#common head
. $PWD/shell/common

if [ $# -eq 0 ]
then LOOP_CNT=5
else LOOP_CNT=$1
fi


while [ $LOOP_CNT -ne 0 ]; do
	echo "reboot count down $LOOP_CNT"
	sleep 1
	LOOP_CNT=$((LOOP_CNT-1))
done  

shutdown -r now
