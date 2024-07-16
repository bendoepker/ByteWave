#include <Audio/portaudio.h>
#include "audioIO.h"

int BWPACallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags statusFlags, void* userData)
{
	BWStereoAudioBuffer* data = (BWStereoAudioBuffer*)userData;
	//TODO: Process buffers
	return 0;
}