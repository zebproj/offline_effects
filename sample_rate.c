#include "sample_rate.h"

void sample_rate(const char *infilename, const char *outfilename, double new_sample_rate){
	
	SNDFILE *infile, *outfile;
	SF_INFO sfinfo;
	sfinfo.format = 0;
	infile = sf_open(infilename, SFM_READ, &sfinfo);

	if (!infile)
	{
		printf("Not able to open input file %s\n", infilename);
		puts(sf_strerror (NULL));
		return;
	}

	if (sfinfo.format != 131074 && sfinfo.format != 65538 && sfinfo.format != 65539 && sfinfo.format != 1245187) {

		printf("Only .wav or .aif files are allowed\n");
		return;
	}

	int readcount, i = 0, frames = sfinfo.frames, error;
	SF_INFO new_file_info;
	new_file_info.format = 0;
	long total_frames = sfinfo.frames*sfinfo.channels;

	SRC_DATA src_data;  
	float *data = malloc(sizeof(float)*(frames*sfinfo.channels));
	float *dataToWrite = malloc(sizeof(float)*(frames*sfinfo.channels));
	const char* error_string;

	new_file_info.samplerate = new_sample_rate;
	new_file_info.channels = sfinfo.channels;
	new_file_info.format = sfinfo.format;
	new_file_info.sections = sfinfo.sections;
	new_file_info.seekable = sfinfo.seekable;
	
	outfile = sf_open(outfilename, SFM_WRITE, &new_file_info);

	if (!outfile)
	{
		printf("Not able to open output file %s\n", outfilename);
		puts(sf_strerror(NULL));
		return;
	}

	readcount = sf_readf_float(infile, data, frames);

	src_data.data_in = data;
	src_data.input_frames = total_frames;
	src_data.data_out = dataToWrite;
	src_data.output_frames = total_frames-2;
	src_data.src_ratio = new_sample_rate/sfinfo.samplerate;

	error = src_simple(&src_data, 0, sfinfo.channels);		

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
