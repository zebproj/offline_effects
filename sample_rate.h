#include "sndfile.h"
#include "stdlib.h"
#include "samplerate.h"

void sample_rate(SNDFILE *infile, SNDFILE *outfile, const char *infilename, const char *outfilename, SF_INFO *sfinfo, double new_sample_rate);
