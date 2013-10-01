#include "info.h"

void info(const char *infilename, SF_INFO *sfinfo){

	int length = ((sfinfo->frames*sfinfo->channels)/sfinfo->samplerate);
	system("clear");	
	printf("%s\n\n", infilename);
	printf("Channels: %d\n", sfinfo->channels);
	printf("Sample Rate: %d\n", sfinfo->samplerate);	
	printf("Length: %d seconds\n\n\n", length);
	return;
}
