#include <stdlib.h>
#include "sndfile.h"
#include "portaudio.h"

void play(const char *infilename);
static void StreamFinished();

