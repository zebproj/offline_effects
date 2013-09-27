// Andrew Ikenberry
// andrewikenberry@gmail.com
// 2013
// Audio File Utility Program

#include <stdio.h>
#include <sndfile.h>
#include <stdlib.h>
#include <string.h>
#include <samplerate.h>


#define INFO 0
#define REVERSE 1
#define SAMPLE_RATE 2

void info(const char *infilename, SF_INFO *sfinfo);
void reverse(SNDFILE *infile, SNDFILE *outfile, const char *infilename, char *outfilename, SF_INFO *sfinfo);
void sample_rate(SNDFILE *infile, SNDFILE *outfile, const char *infilename, char *outfilename, SF_INFO *sfinfo, double new_sample_rate);

int main(int argc, const char **argv){
	
	if (argc < 3){
		printf("USAGE: inputFileName effect parameters(if any)\n");
		return 1;
	}

	SNDFILE *infile, *outfile;
	SF_INFO sfinfo;
	sfinfo.format = 0;
	
	const char *infilename = argv[1];
	const char *effect_arg = argv[2];
	int effect = 0;
	double new_sample_rate;
	char outfilename[300];

	if (argc > 3)
		new_sample_rate = atoi(argv[3]);

	infile = sf_open(infilename, SFM_READ, &sfinfo);
	int len  = (int)strlen(infilename);
	len -=4;
	len = strxfrm(outfilename, infilename, len);

	// Reverse
	if (!(strcmp(effect_arg, "reverse"))){

		effect = REVERSE;
		if (sfinfo.format == 131074)
			strcat(outfilename, "_Reverse.aif");
		else if (sfinfo.format == 65538 || sfinfo.format == 65539)
			strcat(outfilename, "_Reverse.wav");
		else {
			printf("Only .wav or .aif files are allowed\n");
			return 1;
		}

		reverse(infile, outfile, infilename, outfilename, &sfinfo);

	// Info
	} else if (!(strcmp(effect_arg, "info"))){

		effect = INFO;
		info(infilename, &sfinfo);

	// Sample Rate
	} else if (!(strcmp(effect_arg, "sr"))){
		
		effect = SAMPLE_RATE;
		if (sfinfo.format == 131074)
			strcat(outfilename, "_SR.aif");
		else if (sfinfo.format == 65538 || sfinfo.format == 65539)
			strcat(outfilename, "_SR.wav");
		else {
			printf("Only .wav or .aif files are allowed\n");
			return 1;
		}
		
		sample_rate(infile, outfile, infilename, outfilename, &sfinfo, new_sample_rate);

	}

	return 0;

}

void reverse(SNDFILE *infile, SNDFILE *outfile, const char *infilename, char *outfilename, SF_INFO *sfinfo){

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


void info(const char *infilename, SF_INFO *sfinfo){

	int length = ((sfinfo->frames*sfinfo->channels)/sfinfo->samplerate);
	system("clear");	
	printf("%s\n\n", infilename);
	printf("Channels: %d\n", sfinfo->channels);
	printf("Sample Rate: %d\n", sfinfo->samplerate);	
	printf("Length: %d seconds\n\n\n", length);
	return;
}

void sample_rate(SNDFILE *infile, SNDFILE *outfile, const char *infilename, char *outfilename, SF_INFO *sfinfo, double new_sample_rate){

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
	src_data.output_frames = total_frames-2;
	src_data.src_ratio = new_sample_rate/sfinfo->samplerate;

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

