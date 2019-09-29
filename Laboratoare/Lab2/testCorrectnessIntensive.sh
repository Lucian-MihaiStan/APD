#!/bin/bash

prog=$1
N=$2
if [ -n "$3" ]; then
	runs=$3
else
	runs="1 2 4 8 10 12"  # sarakilor
fi

if [ ! -d "out" ]; then
	mkdir out
fi

./$prog $N 1 > true_out
#cat out
# we add 1 as a parameter just so we can use a par_program from parameters instead of a missing sequential one
echo Running intensive correctness test with $P threads =================
for i in `seq 1 1000`; do
	for P in $runs; do
		./$prog $N $P > out/out.$i.$P 
	done
	#if [ i -eq 1 ]; then
	#	cat out.$i.$P
	#fi
done

diff -q --from-file true_out out/out.*
if [ $? -eq 0 ]; then
	echo Output correct on intensive test
fi

rm -rf true_out out