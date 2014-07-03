CC = gcc
objects = main.o info.o reverse.o sample_rate.o varispeed.o play.o 

utility: ${objects}
	$(CC) -o utility main.o info.o reverse.o sample_rate.o varispeed.o play.o -lsndfile -lsamplerate -lportaudio

main.o: main.c
	$(CC) -c $(@:.o=.c)

info.o: info.c info.h 
	$(CC) -c $(@:.o=.c)

reverse.o: reverse.c reverse.h
	$(CC) -c $(@:.o=.c)

sample_rate.o: sample_rate.c sample_rate.h
	$(CC) -c $(@:.o=.c)

varispeed.o: varispeed.c varispeed.h
	$(CC) -c $(@:.o=.c)

play.o: play.c play.h
	$(CC) -c $(@:.o=.c)

clean:
	rm -rf $(objects) utility 
