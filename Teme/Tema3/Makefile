CC=mpicc
CFLAGS=-Wall -Wextra -Wpedantic
MPI=mpirun
MPIFLAGS=-lm

NP=12  # sarakilor
FilterTypes=blur smooth sharpen emboss mean blur smooth sharpen emboss mean
InputFile=input_files/PNM/baby-yoda.pnm
OutputFile=baby-yoda_smooth.pnm

.PHONY: build clean

build: tema3

tema3: tema3.o pnm_image_utils.o filters.o image_processing.o
	$(CC) $^ -o $@ $(MPIFLAGS)

tema3.o: tema3.c
	$(CC) $(CFLAGS) $^ -c

pnm_image_utils.o: pnm_image_utils.c
	$(CC) $(CFLAGS) $^ -c

filters.o: filters.c
	$(CC) $(CFLAGS) $^ -c

image_processing.o: image_processing.c
	$(CC) $(CFLAGS) $^ -c

run_tema3: tema3
	time $(MPI) -np $(NP) $< $(InputFile) $(OutputFile) $(FilterTypes)

run_tema3_quiet: tema3
	@time $(MPI) -np $(NP) $< $(InputFile) $(OutputFile) $(FilterTypes)

clean:
	rm -f tema3 *.o .fuse_hidden* *.pgm *.pnm