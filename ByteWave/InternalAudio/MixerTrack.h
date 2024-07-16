#include "Audio/portaudio.h"
#include "InternalAudio/audioIO.h"

enum BWMixerType
{
	MT_PLAYLIST = 0, //Mixer track that only takes an input from the playlist
	MT_EXTERNAL_PLAYLIST = 1, //Mizer track that takes an input from the playlist as well as an external source
	MT_GLOBAL = 2 //Mixer track that only takes input from other mixer tracks 
};

//General Mixer track that takes inputs from either an external input, a playlist input, or another mixer track
	class MixerTrack
	{
	public:
		BWMixerType getMixerType();
		void setMixerType(BWMixerType);
	private:
		BWStereoAudioBuffer BWAudioBuffer;
		unsigned char m_trackID;
		BWMixerType m_mixerType;
	};

//Global Mixer track that outputs to the audio device
	class GlobalMixerTrack : MixerTrack
	{
	public:
		BWMixerType getMixerType();
	private:
		const BWMixerType m_mixerType = MT_GLOBAL;
	};