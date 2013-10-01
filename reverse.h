#include <stdlib.h>
#include "sndfile.h"

void reverse(SNDFILE *infile, SNDFILE *outfile, const char *infilename, const char *outfilename, SF_INFO *sfinfo);
