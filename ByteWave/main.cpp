#include <QtWidgets/QApplication.h>

#include "MainWindow.h"

#include "Audio/portaudio.h"
#include "audioProcessing.h"

//#include <Windows.h>
//#include <iostream>


int main(int argc, char* argv[])
{
	int returnVal;
	PaError err;

	QApplication app(argc, argv);
	MainWindow mw;
	mw.showMaximized();

	//Initialize Port Audio Library and Handle Errors
	err = Pa_Initialize();
	if (err != paNoError) goto Error;

	//Begin Qt Event Loop
	returnVal = app.exec();

	err = Pa_Terminate();
	if (err != paNoError) goto Error;

	//Application Exit
	return returnVal;

Error:
	//Uncommenting this code will show a console with the error code (must Un-comment the includes for Windows.h)
	//TODO: Add error handling pop-ups using Qt
		//AllocConsole();
		//freopen("CONOUT$", "w", stdout);
		//std::cout << "Port Audio Error:" << Pa_GetErrorText(err) << std::endl;
		//std::system("cmd");
	return -1;
}