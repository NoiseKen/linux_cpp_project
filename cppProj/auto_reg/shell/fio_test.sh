#common head
. $PWD/shell/common
#shell head
RUN_TIME=$1
TEST_SIZE=$2

#shell start
shell_running

#if [ -n $1 ]
#then echo "no argv 1"; exit 0
#fi

rmmod dnvme
rmmod nvme
sleep 3
modprobe nvme
sleep 3

#rm old log
rm -f fio*.log

#SEQ write
fio --filename=/dev/nvme0n1 --direct=1 --rw=write --refill_buffers --norandommap --randrepeat=0 --ioengine=libaio --bs=64K --iodepth=32 --numjobs=4 --runtime=$RUN_TIME --size=$TEST_SIZE --group_reporting --name=AutoREG --output=fio_seq_write.log
shell_exit_if_error $? "run fio SEQ Write error"

#RAND write
fio --filename=/dev/nvme0n1 --direct=1 --rw=randwrite --refill_buffers --norandommap --randrepeat=0 --ioengine=libaio --bs=4K --iodepth=32 --numjobs=4 --runtime=$RUN_TIME --size=$TEST_SIZE --group_reporting --name=AutoREG --output=fio_rand_write.log
shell_exit_if_error $? "run fio RAND Write error"

#SEQ read
fio --filename=/dev/nvme0n1 --direct=1 --rw=read --refill_buffers --norandommap --randrepeat=0 --ioengine=libaio --bs=64K --iodepth=32 --numjobs=4 --runtime=$RUN_TIME --size=$TEST_SIZE --group_reporting --name=AutoREG --output=fio_seq_read.log
shell_exit_if_error $? "run fio SEQ Read error"

#RAND read
fio --filename=/dev/nvme0n1 --direct=1 --rw=randread --refill_buffers --norandommap --randrepeat=0 --ioengine=libaio --bs=4K --iodepth=32 --numjobs=4 --runtime=$RUN_TIME --size=$TEST_SIZE --group_reporting --name=AutoREG --output=fio_rand_read.log
shell_exit_if_error $? "run fio RAND Read error"


shell_exit
#exit $rst
