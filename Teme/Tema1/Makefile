CC=gcc
CFLAGS=-lm -lpthread
WARNS=-Wall -Wextra -Wpedantic
OPT=-O0

inputFile=ft1.data.in
trueOutput=ft1.data_out
outputFile=ft1.data.out
numThreads=12
seed=42
newInputFile=ft3.data.in
N=1000000
NPow2=1048576

all: homeworkFT homeworkFFT inputGenerator compareOutputs

compareOutputs: compareOutputs.c
	$(CC) -o $@ $^ $(CFLAGS) $(WARNS) $(OPT)

inputGenerator: inputGenerator.c
	$(CC) -o $@ $^ $(CFLAGS) $(WARNS) $(OPT)

homeworkFT: homeworkFT.c
	$(CC) -o $@ $^ $(CFLAGS) $(WARNS) $(OPT)

homeworkFFT: homeworkFFT.c
	$(CC) -o $@ $^ $(CFLAGS) $(WARNS) $(OPT)

run_FT: homeworkFT
	time ./$< $(inputFile) $(outputFile) $(numThreads)

run_FFT: homeworkFFT
	time ./$< $(inputFile) $(outputFile) $(numThreads)

run_diff: compareOutputs
	./$< $(trueOutput) $(outputFile)

run_inputGen: inputGenerator
	./$< $(N) $(newInputFile) $(seed)

clean:
	rm -f homeworkFFT homeworkFT inputGenerator compareOutputs *.out