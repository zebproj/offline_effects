#include "info.h"

void info(const char *infilename){
	
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

	int length = ((sfinfo.frames*sfinfo.channels)/sfinfo.samplerate);

	system("clear");	
	printf("%s\n\n", infilename);
	printf("Channels: %d\n", sfinfo.channels);
	printf("Sample Rate: %d\n", sfinfo.samplerate);	
	printf("Length: %d seconds\n\n\n", length);
	return;
}
