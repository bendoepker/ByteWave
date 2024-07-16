#include <QtWidgets/QApplication.h>

#include "MainWindow.h"

#include "Audio/portaudio.h"
#include "InternalAudio/audioIO.h"

#include <Windows.h>
#include <iostream>

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



	//Begin Qt Event Loop
	returnVal = app.exec();

	//Application Exit
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