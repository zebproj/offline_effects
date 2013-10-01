CC = gcc

utility: main.o info.o reverse.o sample_rate.o varispeed.o play.o 
	$(CC) -o utility main.o info.o reverse.o sample_rate.o varispeed.o play.o -lsndfile -lsamplerate -lportaudio

main.o: main.c
	$(CC) -c main.c 

info.o: info.c info.h 
	$(CC) -c info.c

reverse.o: reverse.c reverse.h
	$(CC) -c reverse.c

sample_rate.o: sample_rate.c sample_rate.h
	$(CC) -c sample_rate.c

varispeed.o: varispeed.c varispeed.h
	$(CC) -c varispeed.c

play.o: play.c play.h
	$(CC) -c play.c


clean:
	rm -rf *o utility 
