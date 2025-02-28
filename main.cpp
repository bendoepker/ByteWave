#include "include/portaudio/portaudio.h"
#include <iostream>
#include <string>

#define PA_USE_ASIO 1

void handleError(PaError error) {
	if (error == paNoError) return;
	else {
        std::cout << "PortAudio Error: " << error << "\n";
        exit(error);
    }
}

int main(void) {
	std::string filePath = "file_path";

    PaError err = Pa_Initialize();
    handleError(err);

    const PaVersionInfo *pa_ver = Pa_GetVersionInfo();

    std::cout << "PortAudio Version: " << pa_ver->versionMajor << "." << pa_ver->versionMinor << "." <<  pa_ver->versionSubMinor << "\n";
    std::cout << "PortAudio GetHostApiCount(): " << std::to_string(Pa_GetHostApiCount()) << "\n";
    std::cout << "PortAudio GetDeviceCount(): " << std::to_string(Pa_GetDeviceCount()) << "\n";

	std::cout << "Testing .wav file: " << filePath;

    Pa_Terminate();
}
