// Andrew Ikenberry
// andrewikenberry@gmail.com
// 2013
// Audio File Utility Program

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "samplerate.h"
#include "portaudio.h"
#include "sndfile.h"

#define FRAMES_PER_BUFFER (256)

typedef struct
{
	float *file; 
	int file_ptr;
	int frames;
}
paTestData;

void info(const char *infilename, SF_INFO *sfinfo);
void reverse(SNDFILE *infile, SNDFILE *outfile, const char *infilename, const char *outfilename, SF_INFO *sfinfo);
void sample_rate(SNDFILE *infile, SNDFILE *outfile, const char *infilename, const char *outfilename, SF_INFO *sfinfo, double new_sample_rate);
void vari_speed(SNDFILE *infile, SNDFILE *outfile, const char *infilename, const char *outfilename, SF_INFO *sfinfo, double pitch);
void play(SNDFILE *infile, SF_INFO *sfinfo);
static void StreamFinished();

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

void reverse(SNDFILE *infile, SNDFILE *outfile, const char *infilename, const char *outfilename, SF_INFO *sfinfo){

	int readcount, i = 0, frames = sfinfo->frames;
	double *data = malloc(sizeof(double)*(frames*sfinfo->channels));
	double *dataToWrite = malloc(sizeof(double)*(frames*sfinfo->channels));

	if (!infile)
	{
		printf("Not able to open input file %s.\n", infilename);
		puts(sf_strerror (NULL));
		return;
	}

	outfile = sf_open(outfilename, SFM_WRITE, sfinfo);

	if (!outfile)
	{
		printf("Not able to open output file %s.\n", outfilename);
		puts(sf_strerror(NULL));
		return;
	}

	readcount = sf_readf_double(infile, data, frames);
	
	for (i = 0; i < frames*sfinfo->channels; i++){
		dataToWrite[i] = data[(frames*sfinfo->channels)-i];
	}
	
	sf_writef_double(outfile, dataToWrite, frames);

	free(data);	
	sf_close(infile);
	sf_close(outfile);

	
	return;
}


void info(const char *infilename, SF_INFO *sfinfo){

	int length = ((sfinfo->frames*sfinfo->channels)/sfinfo->samplerate);
	system("clear");	
	printf("%s\n\n", infilename);
	printf("Channels: %d\n", sfinfo->channels);
	printf("Sample Rate: %d\n", sfinfo->samplerate);	
	printf("Length: %d seconds\n\n\n", length);
	return;
}

void sample_rate(SNDFILE *infile, SNDFILE *outfile, const char *infilename, const char *outfilename, SF_INFO *sfinfo, double new_sample_rate){

	int readcount, i = 0, frames = sfinfo->frames, error;
	SF_INFO new_file_info;
	new_file_info.format = 0;
	long total_frames = sfinfo->frames*sfinfo->channels;
	SRC_DATA src_data;  
	float *data = malloc(sizeof(float)*(frames*sfinfo->channels));
	float *dataToWrite = malloc(sizeof(float)*(frames*sfinfo->channels));
	const char* error_string;

	new_file_info.samplerate = new_sample_rate;
	new_file_info.channels = sfinfo->channels;
	new_file_info.format = sfinfo->format;
	new_file_info.sections = sfinfo->sections;
	new_file_info.seekable = sfinfo->seekable;
	
	if (!infile)
	{
		printf("Not able to open input file %s.\n", infilename);
		puts(sf_strerror (NULL));
		return;
	}

	outfile = sf_open(outfilename, SFM_WRITE, &new_file_info);

	if (!outfile)
	{
		printf("Not able to open output file %s.\n", outfilename);
		puts(sf_strerror(NULL));
		return;
	}

	readcount = sf_readf_float(infile, data, frames);

	src_data.data_in = data;
	src_data.input_frames = total_frames;
	src_data.data_out = dataToWrite;
	src_data.output_frames = total_frames-2;
	src_data.src_ratio = new_sample_rate/sfinfo->samplerate;

	error = src_simple(&src_data, 0, sfinfo->channels);		

	if (error){
		error_string =  src_strerror(error);
		printf("%s\n", error_string);
	}

	sf_writef_float(outfile, dataToWrite, frames);

	free(data);	
	sf_close(infile);
	sf_close(outfile);

	return;	


}

void vari_speed(SNDFILE *infile, SNDFILE *outfile, const char *infilename, const char *outfilename, SF_INFO *sfinfo, double pitch){

	int readcount, i = 0, frames = sfinfo->frames, error;
	long total_frames = sfinfo->frames*sfinfo->channels;
	SRC_DATA src_data;  
	float *data = malloc(sizeof(float)*(frames*sfinfo->channels));
	float *dataToWrite = malloc(sizeof(float)*(frames*sfinfo->channels));
	const char* error_string;

	if (!infile)
	{
		printf("Not able to open input file %s.\n", infilename);
		puts(sf_strerror (NULL));
		return;
	}

	outfile = sf_open(outfilename, SFM_WRITE, sfinfo);

	if (!outfile)
	{
		printf("Not able to open output file %s.\n", outfilename);
		puts(sf_strerror(NULL));
		return;
	}

	readcount = sf_readf_float(infile, data, frames);

	src_data.data_in = data;
	src_data.input_frames = total_frames;
	src_data.data_out = dataToWrite;
	src_data.output_frames = total_frames-1;
	src_data.src_ratio = 1/pitch;

	error = src_simple(&src_data, 0, sfinfo->channels);		

	if (error){
		error_string =  src_strerror(error);
		printf("%s\n", error_string);
	}

	sf_writef_float(outfile, dataToWrite, frames);

	free(data);	
	sf_close(infile);
	sf_close(outfile);

	return;	


}

void StreamFinished(){


	printf("Stream completed\n");
}

void play(SNDFILE *infile, SF_INFO *sfinfo) {

	paTestData data;
	PaStreamParameters outputParameters;
	PaStream *stream;
	PaError err;
	err = Pa_Initialize();
	const char *errorString;

	if (err != paNoError)
		printf("There was an error opening port audio\n");
	outputParameters.device =  Pa_GetDefaultOutputDevice();
	outputParameters.channelCount = sfinfo->channels;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	//outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.suggestedLatency = .1;
	
	err = Pa_OpenStream(&stream,
				NULL,
				&outputParameters,
				sfinfo->samplerate,
				FRAMES_PER_BUFFER,
				paClipOff,
				patestCallback,
				&data);
	if (err != paNoError){

		printf(" error %d opening stream\n", err);
		errorString = Pa_GetErrorText(err);
		printf("%s\n", errorString);
	}

	int readcount, i = 0, frames = sfinfo->frames;
	float *samples = malloc(sizeof(float)*(frames*sfinfo->channels));

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

	Pa_Sleep((frames/sfinfo->samplerate)*1000);	

	err = Pa_StopStream(stream);

	err = Pa_CloseStream(stream);

	Pa_Terminate();


	return;
}

