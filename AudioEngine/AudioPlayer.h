#pragma once
#include "Waveform_Parser.h"
#include <map>
#include <Windows.h>
#include <xaudio2.h>

class WAVEParser;
class AudioPlayer
{
public:
	AudioPlayer();

	int LoadSound(char const* _path);
	void Start();
	void Stop();

	void Destroy();

	WAVEParser m_file;
	WAVEFORMATEX m_wfx;
	IXAudio2SourceVoice* m_sourceVoice;
	IXAudio2* m_xaudio2;
	IXAudio2MasteringVoice* m_master_voice;
	XAUDIO2_BUFFER m_audioBuffer;
};
