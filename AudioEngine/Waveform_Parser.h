#pragma once
#include <cstdint>
#include <vector>

#pragma pack(push,1)
struct WAVE_HEADER
{
	uint32_t FileTypeBlocID;
	uint32_t FileSize;
	uint32_t FileFormatID;
};

struct WAVE_DESCRIPTOR
{
	uint32_t FormatBlocID;
	uint32_t BlocSize;
	uint16_t AudioFormat;
	uint16_t NbrCanaux;
	uint32_t Frequence;
	uint32_t BytePerSec;
	uint16_t BytePerBloc;
	uint16_t BitsPerSample;
};

struct WAVE_DATA_INFO
{
	uint32_t DataBlocID;
	uint32_t DataSize;
};
#pragma pack(pop)

class WAVEParser
{
public:
	WAVE_HEADER m_header;
	WAVE_DESCRIPTOR m_descriptor;
	WAVE_DATA_INFO m_dataInfo;
	std::vector<char> m_data;

	long m_size;

	WAVEParser();
	void ReadWaveFile(char const* _path);
};