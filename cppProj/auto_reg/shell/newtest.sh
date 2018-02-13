#common head
. $PWD/shell/common
#shell head
TNVME_SUITE=$1
TEST=$2
LOG_PATH=$3
LOG_NAME=$4

DNVME_KO=$1/dnvme/dnvme.ko
TARGET_DEV=/dev/nvme0

#start here
shell_running

rmmod nvme
rmmod dnvme
sleep 1
insmod $DNVME_KO
sleep 1
retry=0
while [ ! -e $TARGET_DEV ]; do 	
	echo "Can not detected device $TARGET_DEV!!"
	sleep 1
	retry=$((retry+1))
	if [ $retry -ge 10 ]		
		then shell_exit; exit -1
	fi
done

cd $TNVME_SUITE/manage
./runtnvme.sh -t$TEST -v 1.2 -b

rst=$?

PASS_LINE=$(cat ./Logs/log0 | grep "passed       : ")
FAIL_LINE=$(cat ./Logs/log0 | grep "failed       : ")
SKIP_LINE=$(cat ./Logs/log0 | grep "skipped      : ")
TOTAL_LINE=$(cat ./Logs/log0 | grep "total tests  : ")
PASS_CNT="${PASS_LINE#*passed       : }"
FAIL_CNT="${FAIL_LINE#*failed       : }"
SKIP_CNT="${SKIP_LINE#*skipped      : }"
TOTAL_CNT="${TOTAL_LINE#*total tests  : }"

#echo result
echo "\n----- run tnvme $TEST -----" >> $LOG_PATH/$LOG_NAME
#add VCT result -------------------------->
TEST_GRP="${TEST%:*}"
NEXT_GRP=$((TEST_GRP+1))
TEST_XYZ="${TEST#*:}"
echo "TEST GRP=$TEST_GRP"
echo "NEXT GRP=$NEXT_GRP"
echo "TEST_XYZ=$TEST_XYZ"
TNVME_DESC=$(../tnvme/tnvme -summary -v1.2 | sed -n /^$TEST_GRP:/,/^$NEXT_GRP:/p | grep $TEST_XYZ: | head -n 1)
echo "$TNVME_DESC,  $PASS_CNT, $FAIL_CNT, $SKIP_CNT" >> $LOG_PATH/$LOG_NAME.vct
#<-------------------------- add VCT result
echo "pass  item=$PASS_CNT" >> $LOG_PATH/$LOG_NAME
echo "fail  item=$FAIL_CNT" >> $LOG_PATH/$LOG_NAME
echo "skip  item=$SKIP_CNT" >> $LOG_PATH/$LOG_NAME
echo "total item=$TOTAL_CNT" >> $LOG_PATH/$LOG_NAME

#move log to test side
NEW_LOG=$(echo $LOG_PATH/$TEST | tr : _)
rm -rf $NEW_LOG
mv Logs $NEW_LOG

#finish
shell_exit
exit $rst
