#include "reverse.h"

void reverse(const char *infilename, const char *outfilename) {

	SNDFILE *infile, *outfile;
	SF_INFO sfinfo;
	sfinfo.format = 0;

	// open input file
	infile = sf_open(infilename, SFM_READ, &sfinfo);

	// check if input file was opened
	if (!infile)
	{
		printf("Not able to open input file %s\n", infilename);
		puts(sf_strerror (NULL));
		return;
	}

	int readcount, i = 0, frames = sfinfo.frames;

	// allocate memory
	double *data = malloc(sizeof(double)*(frames*sfinfo.channels));
	double *dataToWrite = malloc(sizeof(double)*(frames*sfinfo.channels));
	
	// check format of file
	if (sfinfo.format != 131074 && sfinfo.format != 65538 && sfinfo.format != 65539 && sfinfo.format != 1245187) {

		printf("Only .wav or .aif files are allowed\n");
		return;
	}
	
	// open outputfile
	outfile = sf_open(outfilename, SFM_WRITE, &sfinfo);

	// check if output file was opened
	if (!outfile)
	{
		printf("Not able to open output file %s.\n", outfilename);
		puts(sf_strerror(NULL));
		return;
	}

	readcount = sf_readf_double(infile, data, frames);

	// read data into output buffer from end of file to beginning 
	for (i = 0; i < frames*sfinfo.channels; i++){
		dataToWrite[i] = data[(frames*sfinfo.channels)-i];
	}

	// write output buffer to file
	sf_writef_double(outfile, dataToWrite, frames);

	// free data and close input/output files
	free(data);	
	sf_close(infile);
	sf_close(outfile);

	return;
}


