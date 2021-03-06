#!/bin/bash

seq_program=$1
par_program=$2
N=$3
NReps=$4
printLevel=$5

if [ -n "$6" ];
then
	runs=$6
else
	runs="1 2 4 8 12" # sarakilor
fi

if [ ! -d "out" ];
then
	mkdir out
fi


./$seq_program $N $NReps $printLevel 1 > true_out
#cat out
# we add 1 as a parameter just so we can use a par_program from parameters instead of a missing sequential one
echo Running intensive correctness test with $P threads =================
for i in `seq 1 10`;
do
	for P in $runs;
	do
		./$par_program $N $NReps $printLevel $P > out/out.$i.$P 
	done
	#if [ i -eq 1 ]; then
	#	cat out.$i.$P
	#fi
done

diff -q --from-file true_out out/out.*
if [ $? -eq 0 ];
then
	echo Output correct on intensive test
fi

rm -rf out true_out
