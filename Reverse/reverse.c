// Andrew Ikenberry
// aikenberry@berklee.edu
// June 18, 2013
// Audio File Reverse

#include <stdio.h>
#include <sndfile.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char **argv){
	
	if (argc < 2){
		printf("USAGE: reverse inputFileName\n");
		return 1;
	}

	SNDFILE *infile, *outfile;
	SF_INFO sfinfo;
	sfinfo.format = 0;
	
	const char *infilename = argv[1];
	char outfilename[300];

	infile = sf_open(infilename, SFM_READ, &sfinfo);
	int len  = (int)strlen(infilename);
	len -=3;
	len = strxfrm(outfilename, infilename, len);

	if (sfinfo.format == 131074)
		strcat(outfilename, "_Reverse.aif");
	else if (sfinfo.format == 65538 || sfinfo.format == 65539)
		strcat(outfilename, "_Reverse.wav");
	else {
		printf("Only .wav or .aif files are allowed\n");
		return 1;
	}

	int readcount, i = 0, frames = sfinfo.frames;
	double *data = malloc(sizeof(double)*(frames*sfinfo.channels));
	double *dataToWrite = malloc(sizeof(double)*(frames*sfinfo.channels));

	if (!infile)
	{
		printf("Not able to open input file %s.\n", infilename);
		puts(sf_strerror (NULL));
		return 1;
	}

	outfile = sf_open(outfilename, SFM_WRITE, &sfinfo);

	if (!outfile)
	{
		printf("Not able to open output file %s.\n", outfilename);
		puts(sf_strerror(NULL));
		return 1;
	}

	readcount = sf_readf_double(infile, data, frames);
	
	for (i = 0; i < frames*sfinfo.channels; i++){
		dataToWrite[i] = data[(frames*sfinfo.channels)-i];
	}
	
	sf_writef_double(outfile, dataToWrite, frames);

	free(data);	
	sf_close(infile);
	sf_close(outfile);

	return 0;
}

