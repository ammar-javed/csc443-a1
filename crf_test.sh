#! /bin/bash

# Currently 500 MB
#BYTES_TO_WRITE=524288000

# 10MB
#BYTES_TO_WRITE=10485760

# 50 MB
BYTES_TO_WRITE=52428800

#4KB 8KB 16KB 32KB 64KB 256KB 512KB 1MB 2MB 3MB
#block_sizes=(4096 8192 16384 32768 65536 262144 524288 1048576 2097152 3145728)

block_names=("4KB" "32KB" "64KB" "256KB" "512KB" "1MB" "1.5MB" "2MB" "2.5MB" "3MB")
block_sizes=(4096 32768 65536 262144 524288 1048576 1572864 2097152 2621440 3145728)

b=0
for i in ${block_sizes[@]}
do
   
   echo "Runnning Block Size = ${block_names[$b]} ( ${i} bytes) test..."
   bin/create_random_file test $BYTES_TO_WRITE ${i}
   echo "-----"

   b=`expr ${b} + 1`

done
