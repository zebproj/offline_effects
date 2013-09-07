CC = gcc
reverse: reverse.o
	$(CC) -o reverse reverse.c -lsndfile 

reverse.o: reverse.c
	$(CC) -c reverse.c

clean:
	rm -rf *o reverse && rm *Reverse.*
