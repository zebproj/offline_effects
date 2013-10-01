#include "varispeed.h"

void vari_speed(SNDFILE *infile, SNDFILE *outfile, const char *infilename, const char *outfilename, SF_INFO *sfinfo, double pitch){

	int readcount, i = 0, frames = sfinfo->frames, error;
	long total_frames = sfinfo->frames*sfinfo->channels;
	SRC_DATA src_data;  
	float *data = malloc(sizeof(float)*(frames*sfinfo->channels));
	float *dataToWrite = malloc(sizeof(float)*(frames*sfinfo->channels));
	const char* error_string;

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

	readcount = sf_readf_float(infile, data, frames);

	src_data.data_in = data;
	src_data.input_frames = total_frames;
	src_data.data_out = dataToWrite;
	src_data.output_frames = total_frames-1;
	src_data.src_ratio = 1/pitch;

	error = src_simple(&src_data, 0, sfinfo->channels);		

	if (error){
		error_string =  src_strerror(error);
		printf("%s\n", error_string);
	}

	sf_writef_float(outfile, dataToWrite, frames);

	free(data);	
	sf_close(infile);
	sf_close(outfile);

	return;	


}
