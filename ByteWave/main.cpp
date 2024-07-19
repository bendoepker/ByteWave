#include <QtWidgets/QApplication.h>

#include "MainWindow.h"

#include "Audio/portaudio.h"
#include "InternalAudio/audioIO.h"

#include <Windows.h>
#include <iostream>

#include <cstring>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512

void HandlePaError(PaError);
void QueryDevices(const PaDeviceInfo*);

int main(int argc, char* argv[])
{
	int returnVal;
	PaError err;
	//Initialize Port Audio Library and Handle Errors
	err = Pa_Initialize();
	HandlePaError(err);
	//Application Initialization
	QApplication app(argc, argv);
	MainWindow mw;
	mw.showMaximized();


	PaStreamParameters inputParameters;
	PaStreamParameters outputParameters;

	memset(&inputParameters, 0, sizeof(inputParameters));
	inputParameters.channelCount = 1;
	inputParameters.device = 0;
	inputParameters.hostApiSpecificStreamInfo = NULL;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(0)->defaultLowInputLatency;


	memset(&outputParameters, 0, sizeof(outputParameters));
	outputParameters.channelCount = 1;
	outputParameters.device = 0;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(0)->defaultLowOutputLatency;

	PaStream *stream;

	err = Pa_OpenStream(&stream, &inputParameters, &outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paNoFlag, BWPACallback, NULL);
	HandlePaError(err);

	PaStreamInfo *streamInfo;




	//Begin Qt Event Loop
	returnVal = app.exec();

	//Application Exit
	err = Pa_CloseStream(stream);
	HandlePaError(err);
	err = Pa_Terminate();
	HandlePaError(err);
	return returnVal;
}

/*

	This function uses an unsafe function "freopen", TODO: change to a qt popup window

*/
void HandlePaError(PaError error)
{
	if (error != paNoError)
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		std::cout << "Port Audio Error: " << Pa_GetErrorText(error) << std::endl;
		std::cout << "Error Number: " << error << std::endl;
		std::system("cmd");
		exit(-1);
	}
	return;
}