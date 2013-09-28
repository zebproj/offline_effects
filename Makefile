CC = gcc
utility: audio_utility.o
	$(CC) -o utility audio_utility.c -lsndfile -lsamplerate -lportaudio

audio_utility.o: audio_utility.c
	$(CC) -c audio_utility.c

clean:
	rm -rf *o utility 
