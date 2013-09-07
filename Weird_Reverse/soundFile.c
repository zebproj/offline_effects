#include <stdio.h>
#include <sndfile.h>

//#define BUFFER_LEN 16384
#define MAX_CHANNELS 2

static void process_data (double *data, int count, int channels);

int main(int argc, const char **argv){
	
	if (argc < 2){
		printf("USAGE: soundFile inputFileName\n");
		return 1;
	}
	int BUFFER_LEN = 32768;
//	static double data [BUFFER_LEN];	
//	static double dataToWrite [BUFFER_LEN];
	static double data [32768];
	static double dataToWrite[32768];

	SNDFILE *infile, *outfile;
	SF_INFO sfinfo;
	int readcount, i = 0, d = 0, chan, channels, frames, samples;
	const char *infilename = argv[1];
	const char *outfilename = "output.wav";

	infile = sf_open(infilename, SFM_READ, &sfinfo);
	if (!infile)
	{
		printf("Not able to open input file %s.\n", infilename);
		puts(sf_strerror (NULL));
		return 1;
	}

	if (sfinfo.channels > MAX_CHANNELS)
	{
		printf("Not able to process more than %d channels\n", MAX_CHANNELS);
		return 1;
	}

	outfile = sf_open(outfilename, SFM_WRITE, &sfinfo);
	printf("This file has %d channels\n", sfinfo.channels);
	channels = sfinfo.channels;

	if (!outfile)
	{
		printf("Not able to open output file %s.\n", outfilename);
		puts(sf_strerror(NULL));
		return 1;
	}

	readcount = sf_read_double(infile, data, BUFFER_LEN);
	printf("Readcount is %d\n", readcount);
	while (readcount)
	{
		//process_data(data, readcount, sfinfo.channels);
		for (chan = 0; chan < channels; chan++){
			for (i = chan; i < readcount;i+=channels){
				//dataToWrite[i] = data[(readcount-1)-i];
				if (BUFFER_LEN != 16384)
					dataToWrite[i] = data[(readcount-1)-i];
				else
					dataToWrite[i] = data[i];

			}
		}
			sf_write_double(outfile, dataToWrite, readcount);
			printf("Readcount is %d\n", readcount);
			readcount = sf_read_double(infile, data, BUFFER_LEN);
			d++;	
			if ((d % 211)== 0){ 
				BUFFER_LEN /=2;
			}
			else if ((d %67 ==0)){
				BUFFER_LEN = 16384;
			} else if ((d % 25 == 0)){
				BUFFER_LEN /= 4;
			}
			
	}

	sf_close(infile);
	sf_close(outfile);

	return 0;
}

static void process_data(double *data, int count, int channels)
{
	int k, chan;

	for (chan = 0; chan < channels; chan++)
		for (k = chan; k < count; k+=channels)
			data [k] *= 0;
	return;
}
