all: tsp

tsp: tsp.o
	gcc -o tsp tsp.o -lm

tsp.o: tsp.c
	gcc -c -Wall tsp.c

clean:
	rm tsp tsp.o tree.txt cycle.txt
