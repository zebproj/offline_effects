#include <stdlib.h>
#include "sndfile.h"
#include "portaudio.h"

void play(SNDFILE *infile, SF_INFO *sfinfo);
static void StreamFinished();

