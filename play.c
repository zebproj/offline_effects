#include "play.h"

#define FRAMES_PER_BUFFER (256)

typedef struct
{
	float *file; 
	int file_ptr;
	int frames;
}
paTestData;

int patestCallback (const void *inputBuffer, void *outputBuffer,
				unsigned long framesPerBuffer,
				const PaStreamCallbackTimeInfo* timeInfo,
				PaStreamCallbackFlags statusFlags,
				void *userData) 
{
	paTestData *data = (paTestData*)userData;
	float *out = (float*)outputBuffer;
	unsigned long i;
	
	(void) timeInfo;
	(void) statusFlags;
	(void) inputBuffer;

	for (i=0; i < framesPerBuffer; i++) {

		*out++ = data->file[data->file_ptr];
		data->file_ptr++;
		
		if (data->file_ptr >= data->frames)
			data->file_ptr -= data->frames;
		
		if (data->file_ptr <= 1)
			data->file_ptr += data->frames;

	}
	
	return paContinue;
}

void StreamFinished(){

	printf("Stream completed\n");
}

void play(const char *infilename) {
	
	SNDFILE *infile;
	SF_INFO sfinfo;
	sfinfo.format = 0;

	infile = sf_open(infilename, SFM_READ, &sfinfo);

	if (sfinfo.format != 131074 && sfinfo.format != 65538 && sfinfo.format != 65539 && sfinfo.format != 1245187) {
		
		printf("Only .wav or .aif files are allowed\n");
		return;
	}

	paTestData data;
	PaStreamParameters outputParameters;
	PaStream *stream;
	PaError err;
	err = Pa_Initialize();
	const char *errorString;

	if (err != paNoError)
		printf("There was an error opening port audio\n");

	outputParameters.device =  Pa_GetDefaultOutputDevice();
	outputParameters.channelCount = sfinfo.channels;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	//outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.suggestedLatency = .1;
	
	err = Pa_OpenStream(&stream,
				NULL,
				&outputParameters,
				sfinfo.samplerate,
				FRAMES_PER_BUFFER,
				paClipOff,
				patestCallback,
				&data);
	if (err != paNoError){

		printf(" error %d opening stream\n", err);
		errorString = Pa_GetErrorText(err);
		printf("%s\n", errorString);
	}

	int readcount, i = 0, frames = sfinfo.frames;
	float *samples = malloc(sizeof(float)*(frames*sfinfo.channels));

	if (!infile)
	{
		printf("Not able to open input file");
		puts(sf_strerror (NULL));
		return;
	}

	readcount = sf_readf_float(infile, samples, frames);

	data.file = samples;
	data.frames = frames;
	data.file_ptr = 0;

	err = Pa_SetStreamFinishedCallback(stream, &StreamFinished);
	
	if (err != paNoError)
		printf("There was an error with Pa_SetStreamFinishedCallback\n");

	err = Pa_StartStream(stream);

	Pa_Sleep((frames/sfinfo.samplerate)*1000);	

	err = Pa_StopStream(stream);

	err = Pa_CloseStream(stream);

	Pa_Terminate();

	return;
}

