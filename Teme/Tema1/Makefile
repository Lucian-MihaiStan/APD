CC=gcc
CFLAGS=-lm -lpthread
WARNS=-Wall -Wextra -Wpedantic
OPT=-O0

inputFile=ft4.data.in
trueOutput=ft3.data_out
outputFile=ft4.data.out
numThreads=12
numThreadsFFT=4
seed=42
newInputFile=ft4.data.in
N=1000
NPow2=67108864

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
	time ./$< $(inputFile) $(outputFile) $(numThreadsFFT)

run_diff: compareOutputs
	./$< $(trueOutput) $(outputFile)

run_inputGen: inputGenerator
	./$< $(NPow2) $(newInputFile) $(seed)

clean:
	rm -f homeworkFFT homeworkFT inputGenerator compareOutputs *.out