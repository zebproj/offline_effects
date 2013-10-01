#include <stdlib.h>
#include "sndfile.h"
#include "samplerate.h"

void vari_speed(SNDFILE *infile, SNDFILE *outfile, const char *infilename, const char *outfilename, SF_INFO *sfinfo, double pitch);
