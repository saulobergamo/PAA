all: novotsp

novotsp: novotsp.o
	gcc -o novotsp novotsp.o -lm

novotsp.o: novotsp.c
	gcc -c -Wall novotsp.c

clean:
	rm novotsp novotsp.o