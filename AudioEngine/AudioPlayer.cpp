#include "AudioPlayer.h"
#include "Waveform_Parser.h"
#include <iostream>

AudioPlayer::AudioPlayer()
    : m_wfx({}),
    m_sourceVoice(nullptr),
    m_xaudio2(nullptr),
    m_master_voice(nullptr),
    m_audioBuffer({})
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr)) std::cout << "Initialisation COM :  FAILED" << '\n';

    hr = XAudio2Create(&m_xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr)) std::cout << "Create XAudio2 instance :  FAILED" << '\n';

    hr = m_xaudio2->CreateMasteringVoice(&m_master_voice);
    if (FAILED(hr)) std::cout << "Create mastering voice :  FAILED" << '\n';
}

int AudioPlayer::LoadSound(char const* _path)
{
    m_file.ReadWaveFile(_path);

    m_wfx.wFormatTag = m_file.m_descriptor.AudioFormat;
    m_wfx.nChannels = m_file.m_descriptor.NbrChannels;
    m_wfx.nSamplesPerSec = m_file.m_descriptor.SampleRate;
    m_wfx.wBitsPerSample = m_file.m_descriptor.BitsPerSample;
    m_wfx.nBlockAlign = m_file.m_descriptor.BytePerBloc;
    m_wfx.nAvgBytesPerSec = m_file.m_descriptor.BytePerSec;

    HRESULT hr = m_xaudio2->CreateSourceVoice(&m_sourceVoice, &m_wfx);
    if (FAILED(hr)) return hr;

    m_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(m_file.m_data.data());
    m_audioBuffer.AudioBytes = m_file.m_data.size() * sizeof(float);
    m_audioBuffer.Flags = XAUDIO2_END_OF_STREAM;

    m_sourceVoice->SubmitSourceBuffer(&m_audioBuffer);
}

void AudioPlayer::Start()
{
    m_sourceVoice->Start();

    while (true) {
        XAUDIO2_VOICE_STATE state;
        m_sourceVoice->GetState(&state);
        if (state.BuffersQueued == 0) break;
        Sleep(100);
    }
}

void AudioPlayer::Stop()
{
    m_sourceVoice->Stop();
}

void AudioPlayer::Destroy()
{
    m_sourceVoice->DestroyVoice();
    m_master_voice->DestroyVoice();
    m_xaudio2->Release();
    CoUninitialize();
}


