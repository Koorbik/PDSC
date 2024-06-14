
bsearch: bsearch.o
	gcc -g $^ -o $@  -lm
test: test.o bsearch.o
	gcc -g $^ -o $@  -lm

.c.o: 
	gcc -g -Wall -pedantic -c  $<

bsearch.o: bsearch.c


clean:
	-rm bsearch.o 
