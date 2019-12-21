#! /bin/bash

INPUT_FILE=$1
OUTPUT_FILE=$2

make clean
echo ""
make tema3

if [ $? -ne "0" ]; then
	echo "Tema nu compileaza" && exit 1
fi

# Seteaza numarul de procese pentru fiecare rulare
for i in {0..11}; do
	NPs[i]=$((i + 1))
done

# Adauga 200 de filtre de tip `smooth`
for i in {0..199}; do
	filters+="smooth "
done

echo ""

for numProc in "${NPs[@]}"; do
	echo "Testare cu NP = ${numProc}"
	make run_tema3_quiet InputFile=$INPUT_FILE OutputFile="${numProc}_processes_${OUTPUT_FILE}" NP=$numProc FilterTypes="${filters}"
	echo ""
done