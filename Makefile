# Just a scratch makefile to automate the testing

CC=gcc

all:
	$(CC) decoder.c -o decoder
	$(CC) encoder.c -o encoder

test:
	./encoder ./InputFiles/sudoku1.txt
	minisat output.cnf encoded.txt || /bin/true
	./decoder encoded.txt >> output.txt
	./encoder ./InputFiles/sudoku2.txt
	minisat output.cnf encoded.txt || /bin/true
	./decoder encoded.txt >> output.txt
	./encoder ./InputFiles/sudoku3.txt
	minisat output.cnf encoded.txt || /bin/true
	./decoder encoded.txt >> output.txt
	./encoder ./InputFiles/sudoku4.txt
	minisat output.cnf encoded.txt || /bin/true
	./decoder encoded.txt >> output.txt
	./encoder ./InputFiles/sudoku5.txt
	minisat output.cnf encoded.txt || /bin/true
	./decoder encoded.txt >> output.txt

clean:
	rm -f encoder
	rm -f decoder
	rm -f output.cnf
	rm -f encoded.txt
