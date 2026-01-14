#include <filesystem>
#include "Waveform_Parser.h"

using namespace std;
#define BASE_PATH std::filesystem::current_path().parent_path().parent_path().parent_path().parent_path()

int main()
{
















	return;

	MB::WAVEParser waveFile;
	const char* filename = "Haydn_94_Andante.wav"; // Symphony_No.6, file_example_WAV_1MG, Haydn_94_Andante
	std::filesystem::path path = BASE_PATH / "AudioEngine\\src\\" / filename;
	waveFile.ReadWaveFile(path.string().c_str());

	waveFile.Print();

	//waveFile.DivideChannels();
	


	// Oscillateur sinusoidal sur 1 canal 
	const int SetSampleRate = 44100;
	std::vector<float> audio_buffer(SetSampleRate * 5); // Fréquence d'échantillonnage * 5 (= 5 secondes de son) 
	const float freq = 440.0f; // 440 hz -> tonalité du téléphone 
	for (size_t i = 0; i < audio_buffer.size(); ++i) {
		audio_buffer[i] = 0.3 * sinf(2.0f * 3.14159265f * freq * (float)i / (float)SetSampleRate); // 0.3 c'est l'atténuation d'amplitude 
	}
	MB::WAVEParser Myfile;
	Myfile.SetData(audio_buffer);
	Myfile.SetSampleRate(SetSampleRate);
	Myfile.SetChannelNumber(1);
	Myfile.ExtractFile("Myfile.wav");

	return 0;
}
