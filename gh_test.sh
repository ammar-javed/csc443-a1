#! /bin/bash

# small_test.txt = 20 bytes
# big _test.txt = 30 MB 
FILE_TO_READ="big_test.txt"

#4KB 8KB 16KB 32KB 64KB 256KB 512KB 1MB 2MB 3MB
#block_sizes=(4096 8192 16384 32768 65536 262144 524288 1048576 2097152 3145728)

block_names=("1KB" "2KB" "4KB" "6KB" "8KB" "16KB" "24KB" "32KB" "64KB" "256KB" "512KB" "1MB" "1.5MB" "2MB" "2.5MB" "3MB")
block_sizes=(1024 2048 4096 6144 8192 16384 24576 32768 65536 262144 524288 1048576 1572864 2097152 2621440 3145728)

b=0
for i in ${block_sizes[@]}
do
   
   echo "Runnning Block Size = ${block_names[$b]} ( ${i} bytes) test..."
   bin/get_histogram $FILE_TO_READ ${i}
   echo "-----"

   b=`expr ${b} + 1`

done
