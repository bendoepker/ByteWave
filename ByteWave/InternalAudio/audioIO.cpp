#include <Audio/portaudio.h>
#include "audioIO.h"

int BWPACallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
	//Retrieve input from device (input buffer)
	//Send that input data to mixers for processing

	//Retrieve output data from global mixer
	//Send that data to the device (output buffer)
	return 0;
}