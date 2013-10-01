#include "reverse.h"

void reverse(SNDFILE *infile, SNDFILE *outfile, const char *infilename, const char *outfilename, SF_INFO *sfinfo){

	int readcount, i = 0, frames = sfinfo->frames;
	double *data = malloc(sizeof(double)*(frames*sfinfo->channels));
	double *dataToWrite = malloc(sizeof(double)*(frames*sfinfo->channels));

	if (!infile)
	{
		printf("Not able to open input file %s.\n", infilename);
		puts(sf_strerror (NULL));
		return;
	}

	outfile = sf_open(outfilename, SFM_WRITE, sfinfo);

	if (!outfile)
	{
		printf("Not able to open output file %s.\n", outfilename);
		puts(sf_strerror(NULL));
		return;
	}

	readcount = sf_readf_double(infile, data, frames);
	
	for (i = 0; i < frames*sfinfo->channels; i++){
		dataToWrite[i] = data[(frames*sfinfo->channels)-i];
	}
	
	sf_writef_double(outfile, dataToWrite, frames);

	free(data);	
	sf_close(infile);
	sf_close(outfile);

	
	return;
}


