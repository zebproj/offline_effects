#include <stdio.h>
#include <sndfile.h>

#define MAX_CHANNELS 2

static void process_data (double *data, int count, int channels);

int main(int argc, const char **argv){
	
	if (argc < 2){
		printf("USAGE: soundFile inputFileName\n");
		return 1;
	}

	SNDFILE *infile, *outfile;
	SF_INFO sfinfo;
	int readcount,totalReadCount=0,  i = 0, d = 0, chan, channels, frames, samples, bufferLength = 1, flag = 1;
	const char *infilename = argv[1];
	const char *outfilename = "output.wav";

	infile = sf_open(infilename, SFM_READ, &sfinfo);
	frames = sfinfo.frames;
	printf("Frames is %d\n", sfinfo.frames);
	double *data = malloc(sizeof(double)*frames);
	double *dataTemp = malloc(sizeof(double)*frames);
	double *dataToWrite = malloc(sizeof(double)*frames);

	if (data)
		printf("Data was allocated\n");


	while (bufferLength < frames)
		 bufferLength <<= 1;
	bufferLength >>=1;

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

	readcount = sf_readf_double(infile, data, frames);
	printf("Readcount is %d\n", readcount);
	while (readcount)
	{
		printf("In while loop\n");
		printf("TotalReadCount is %d\n", totalReadCount);
		printf("ReadCount is %d\n", readcount);
		for (i = totalReadCount; i < totalReadCount+readcount; i++){
			dataTemp[i] = data[i];
		}
		readcount = sf_read_double(infile, data, bufferLength);	
		totalReadCount+=readcount;
					
	}
	printf("Out of while loop\n");		
	for (i = 0; i < (totalReadCount-1); i++){
		dataToWrite[i] = dataTemp[(totalReadCount-1)-i];	
	}
	printf("Seg fault on writing\n");
	sf_write_double(outfile, dataToWrite, frames);
	/*	
		for (chan = 0; chan < channels; chan++){
			for (i = chan; i < readcount;i+=channels){
					dataToWrite[i] = data[(readcount-1)-i];

			}
		}

			readcount = sf_read_double(infile, data, bufferLength);
	*/		

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
