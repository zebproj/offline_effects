// Andrew Ikenberry
// andrewikenberry@gmail.com
// 2013
// Audio File Utility Program

#include <stdio.h>
#include <string.h>
#include "sndfile.h"

int main(int argc, const char **argv){
	
	if (argc < 3){
		printf("USAGE: utility effect inputFileName outputFileName parameters(if any)\n");
		return 1;
	}

	SNDFILE *infile, *outfile;
	SF_INFO sfinfo;
	sfinfo.format = 0;
	
	const char *effect_arg = argv[1];
	const char *infilename, *outfilename;
	double new_sample_rate, pitch;

	
	// Reverse
	if (!(strcmp(effect_arg, "reverse"))){

		if (argc < 4){
			printf("USAGE: utility reverse inputFileName outputFileName\n");	
			return 1;
		}

		infilename = argv[2];
		outfilename = argv[3];
		infile = sf_open(infilename, SFM_READ, &sfinfo);

		if (sfinfo.format != 131074 && sfinfo.format != 65538 && sfinfo.format != 65539 && sfinfo.format != 1245187) {

			printf("Only .wav or .aif files are allowed\n");
			return 1;
		}

		reverse(infile, outfile, infilename, outfilename, &sfinfo);

	// Info
	} else if (!(strcmp(effect_arg, "info"))){

		if (argc < 3) {
			
			printf("USAGE: utility info inputFileName\n");
			return 1;
		}
		infilename = argv[2];
		infile = sf_open(infilename, SFM_READ, &sfinfo);

		info(infilename, &sfinfo);

	// Sample Rate
	} else if (!(strcmp(effect_arg, "sr"))){
	
		if (argc < 5) {
			printf("USAGE: utility sr inputFileName outputFileName new_sr\n");	
			return 1;
		}	

		infilename = argv[2];
		outfilename = argv[3];
		new_sample_rate = atof(argv[4]);
		infile = sf_open(infilename, SFM_READ, &sfinfo);

		if (sfinfo.format != 131074 && sfinfo.format != 65538 && sfinfo.format != 65539 && sfinfo.format != 1245187) {

			printf("Only .wav or .aif files are allowed\n");
			return 1;
		}
		
		sample_rate(infile, outfile, infilename, outfilename, &sfinfo, new_sample_rate);

	// Vari Speed
	} else if (!(strcmp(effect_arg, "vari"))){
		
		if (argc < 5) {
			printf("USAGE: utility vari inputFileName outputFileName pitch\n");
			return 1;
		}
		
		infilename = argv[2];
		outfilename = argv[3];
		pitch = atof(argv[4]);
		infile = sf_open(infilename, SFM_READ, &sfinfo);

		if (sfinfo.format != 131074 && sfinfo.format != 65538 && sfinfo.format != 65539 && sfinfo.format != 1245187) {
			
			printf("Only .wav or .aif files are allowed\n");
			return 1;
		}

		vari_speed(infile, outfile, infilename, outfilename, &sfinfo, pitch);

	// Play
	} else if (!(strcmp(effect_arg, "play"))){
	
		if (argc < 3) {
			printf("USAGE: utility play inputFileName\n");
			return 1;
		}	

		infilename = argv[2];
		infile = sf_open(infilename, SFM_READ, &sfinfo);

		if (sfinfo.format != 131074 && sfinfo.format != 65538 && sfinfo.format != 65539 && sfinfo.format != 1245187) {
		
			printf("Only .wav or .aif files are allowed\n");
			return 1;
		}

		play(infile, &sfinfo);

	}

	return 0;

}
