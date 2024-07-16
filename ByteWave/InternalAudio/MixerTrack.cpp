#include "MixerTrack.h"

BWMixerType MixerTrack::getMixerType()
{
	return m_mixerType;
}

void MixerTrack::setMixerType(BWMixerType mixerType)
{
	m_mixerType = mixerType;
}

