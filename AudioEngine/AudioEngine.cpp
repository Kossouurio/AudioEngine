#include "AudioPlayer.h"
#include "Waveform_Parser.h"
#include <cmath>
#include <filesystem>
#include <iostream>
#include <vector>


using namespace std;
#define BASE_PATH std::filesystem::current_path().parent_path().parent_path().parent_path().parent_path()


// Cette fonction est appelée à chaque fois que le driver a besoin de n échantillons en sortie
//void audio_callback(float** in, float** out, size_t n_samples, uint8_t in_channels, uint8_t out_channels)
//{
//    // Débrouille toi avec les échantillons entrant
//    // Ecris ce que tu veux en sortie
//}
// Interface 
//class my_voice_callback : public IXAudio2VoiceCallback
//{
//    void OnBufferStart(void* pBufferContext) override; // Appelé quand une voix s'apprête à lire un buffer 
//    void OnBufferEnd(void* pBufferContext) override; // Appelé quand une voix finit la lecture d'un buffer
//
//    void OnStreamEnd() override; // Quand une voix finit un buffer XAUDIO2_BUFFER dont le XAUDIO2_END_OF_STREAM a été set 
//    void OnLoopEnd(void* pBufferContext) override; // Quand une voix atteint le point de fin de boucle 
//
//    void OnVoiceError(void* pBufferContext, HRESULT error) override; // Une erreur ? 
//
//    void OnProcessingPassStart(UINT32 BytesRequired) override; // Juste avant que XAudio lise les data du buffer d'une voix  
//    void OnProcessingPassEnd() override; // Après un "processing pass" (?)
//};

int main()
{
	const char* xfilename = "un_gars_une_fille.wav"; // Symphony_No6, file_example_WAV_1MG, Haydn_94_Andante
	std::filesystem::path xpath = BASE_PATH / "AudioEngine\\src\\" / xfilename;
	AudioPlayer myAudioPlayer;
	myAudioPlayer.LoadSound(xpath.string().c_str());

	myAudioPlayer.Start();











	return 0;

	WAVEParser waveFile;
	const char* filename = "Symphony_No6.wav"; // Symphony_No6, file_example_WAV_1MG, Haydn_94_Andante
	std::filesystem::path path = BASE_PATH / "AudioEngine\\src\\" / filename;
	waveFile.ReadWaveFile(path.string().c_str());

	waveFile.Print();

	//waveFile.DivideChannels();
	
    return 0;

	// Oscillateur sinusoidal sur 1 canal 
	const int SetSampleRate = 44100;
	std::vector<float> audio_buffer(SetSampleRate * 5); // Fréquence d'échantillonnage * 5 (= 5 secondes de son) 
	const float freq = 440.0f; // 440 hz -> tonalité du téléphone 
	for (size_t i = 0; i < audio_buffer.size(); ++i) {
		audio_buffer[i] = 0.3 * sinf(2.0f * 3.14159265f * freq * (float)i / (float)SetSampleRate); // 0.3 c'est l'atténuation d'amplitude 
	}
	WAVEParser Myfile;
	Myfile.SetData(audio_buffer);
	Myfile.SetSampleRate(SetSampleRate);
	Myfile.SetChannelNumber(1);
	Myfile.ExtractFile("Myfile.wav");

	return 0;
}
