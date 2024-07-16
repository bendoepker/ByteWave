#pragma once

typedef struct
{
	float left;
	float right;
} BWStereoAudioBuffer;

/*

	This is the callback function that is used by the PortAudio API

*/
int BWPACallback(const void* inputBuffer,
						void* outputBuffer,
						unsigned long framesPerBuffer,
						const PaStreamCallbackTimeInfo*,
						PaStreamCallbackFlags statusFlags,
						void* userData);