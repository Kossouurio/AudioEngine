#include <filesystem>
#include "Waveform_Parser.h"

using namespace std;
#define BASE_PATH std::filesystem::current_path().parent_path().parent_path().parent_path().parent_path()

int main()
{
	WAVEParser waveFile;
	const char* filename = "file_example_WAV_1MG.wav"; // Symphony_No.6, file_example_WAV_1MG
	std::filesystem::path path = BASE_PATH / "AudioEngine\\src\\" / filename;
	waveFile.ReadWaveFile(path.string().c_str());


	return 0;
}
