#! /bin/bash

if [ $# -ne "4" ]; then
	echo "Scriptul are nevoie de 4 parmetri"
	echo "./test_scalability.sh input_file output_file num_filters num_processors" && exit 1
fi

INPUT_FILE=$1
OUTPUT_FILE=$2
NUM_FILTERS=$3
NUM_PROC=$4

make clean
echo ""
make tema3

if [ $? -ne "0" ]; then
	echo "Tema nu compileaza" && exit 2
fi

# Seteaza numarul de procese pentru fiecare rulare
for i in $(seq "$NUM_PROC"); do
	NPs[i]=$i
done

# Adauga 200 de filtre de tip `smooth`
for i in $(seq "$NUM_FILTERS"); do
	filters+="smooth "
done

echo ""

for numProc in "${NPs[@]}"; do
	echo "Testare cu NP = ${numProc}"
	make run_tema3_quiet InputFile=$INPUT_FILE OutputFile="${numProc}_processes_${OUTPUT_FILE}" NP=$numProc FilterTypes="${filters}"
	echo ""
done