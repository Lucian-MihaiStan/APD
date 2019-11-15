CC=mpicc
MPI=mpirun

NP_ex2_8=2
NP_ex4_5_7=4
NP_ex6=10
NP_ex9=12

.PHONY: build clean

build: ex2 ex4 ex5 ex6 ex7 ex8 ex9

ex2: ex2.c
	$(CC) $^ -o $@

ex4: ex4.c
	$(CC) $^ -o $@

ex5: ex5.c
	$(CC) $^ -o $@

ex6: ex6.c
	$(CC) $^ -o $@

ex7: ex7.c
	$(CC) $^ -o $@

ex8: ex8.c
	$(CC) $^ -o $@

ex9: ex9.c
	$(CC) $^ -o $@

run_ex2: ex2
	$(MPI) -np $(NP_ex2_8) $<

run_ex4: ex4
	$(MPI) -np $(NP_ex4_5_7) $<

run_ex5: ex5
	$(MPI) -np $(NP_ex4_5_7) $<

run_ex6: ex6
	$(MPI) -np $(NP_ex6) $<

run_ex7: ex7
	$(MPI) -np $(NP_ex4_5_7) $<

run_ex8: ex8
	$(MPI) -np $(NP_ex2_8) $<

run_ex9: ex9
	$(MPI) -np $(NP_ex9) $<

clean:
	rm -f .fuse_hidden* ex2 ex4 ex5 ex6 ex7 ex8 ex9 