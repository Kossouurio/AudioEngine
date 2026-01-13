#include <filesystem>
#include "Waveform_Parser.h"

using namespace std;
#define BASE_PATH std::filesystem::current_path().parent_path().parent_path().parent_path().parent_path()

int main()
{
	MB::WAVEParser waveFile;
	const char* filename = "file_example_WAV_10MG.wav"; // Symphony_No.6, file_example_WAV_1MG, Haydn_94_Andante
	std::filesystem::path path = BASE_PATH / "AudioEngine\\src\\" / filename;
	waveFile.ReadWaveFile(path.string().c_str());

	waveFile.Print();

	//waveFile.DivideChannels();
	
	// Oscillateur sinusoidal sur 1 canal 
	std::vector<float> audio_buffer(waveFile.m_descriptor.BytePerSec * 5); // Fréquence d'échantillonnage * 5 (= 5 secondes de son) 
	const float SampleRate = 440.0f; // 440 hz -> tonalité du téléphone 
	for (size_t i = 0; i < audio_buffer.size(); ++i) {
		audio_buffer[i] = 0.3 * sinf(2.0f * 3.14159265f * SampleRate * (float)i / (float)waveFile.m_descriptor.BytePerSec); // 0.3 c'est l'atténuation d'amplitude 
	}

	waveFile.SetData(audio_buffer);

	return 0;
}
