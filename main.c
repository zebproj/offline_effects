// Andrew Ikenberry
// andrewikenberry@gmail.com
// 2013
// Audio File Utility Program

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, const char **argv){
	
	if (argc < 3){
		printf("USAGE: utility effect inputFileName outputFileName parameters(if any)\n");
		return 1;
	}

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

		reverse(infilename, outfilename);

	// Info
	} else if (!(strcmp(effect_arg, "info"))){

		if (argc < 3) {
			
			printf("USAGE: utility info inputFileName\n");
			return 1;
		}

		infilename = argv[2];

		info(infilename);

	// Sample Rate
	} else if (!(strcmp(effect_arg, "sr"))){
	
		if (argc < 5) {
			printf("USAGE: utility sr inputFileName outputFileName new_sr\n");	
			return 1;
		}	

		infilename = argv[2];
		outfilename = argv[3];
		new_sample_rate = atof(argv[4]);
		
		sample_rate(infilename, outfilename, new_sample_rate);

	// Vari Speed
	} else if (!(strcmp(effect_arg, "vari"))){
		
		if (argc < 5) {
			printf("USAGE: utility vari inputFileName outputFileName pitch\n");
			return 1;
		}
		
		infilename = argv[2];
		outfilename = argv[3];
		pitch = atof(argv[4]);

		vari_speed(infilename, outfilename, pitch);

	// Play
	} else if (!(strcmp(effect_arg, "play"))){
	
		if (argc < 3) {
			printf("USAGE: utility play inputFileName\n");
			return 1;
		}	

		infilename = argv[2];

		play(infilename); 

	}

	return 0;

}
