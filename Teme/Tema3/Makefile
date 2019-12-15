CC=mpicc
MPI=mpirun
MPIFLAGS=-lm

NP=12
FilterTypes=sharpen smooth emboss
InputFile=input_files/PNM/baby-yoda.pnm
OutputFile=filtered-baby-yoda.pnm

.PHONY: build clean

build: tema3

tema3: tema3.o pnm_image_utils.o filters.o
	$(CC) $^ -o $@ $(MPIFLAGS)

tema3.o: tema3.c
	$(CC) $^ -c

pnm_image_utils.o: pnm_image_utils.c
	$(CC) $^ -c

filters.o: filters.c
	$(CC) $^ -c

run_tema3: tema3
	$(MPI) -np $(NP) $< $(InputFile) $(OutputFile) $(FilterTypes)

clean:
	rm -f tema3 *.o .fuse_hidden* *.pgm *.pnm