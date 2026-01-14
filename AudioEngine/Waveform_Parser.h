#pragma once
#include <cstdint>
#include <map>
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
	uint16_t NbrChannels;
	uint32_t SampleRate;
	uint32_t BytePerSec;
	uint16_t BytePerBloc;
	uint16_t BitsPerSample;
};

struct WAVE_DATA_INFO
{
	uint32_t DataBlocID;
	uint32_t DataSize;
};

struct CHANNELS
{
	uint32_t ChannelNumber;
	std::map<int, std::vector<char>> m_data;

	void AddDataTo(int _channel, char& _data, int _bitsPerSample)
	{
		for (int i = 0; i < _bitsPerSample; i++)
		{
			m_data[_channel].push_back(_data);
		}
	}
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

	CHANNELS m_channels;

	WAVEParser();
	WAVEParser(char const* _path); 
	void ReadWaveFile(char const* _path);
	void DivideChannels();

	void SetData(std::vector<float>& _data);
	void SetData(std::vector<int>& _data);
	void SetSampleRate(uint32_t _numb);
	void SetChannelNumber(uint32_t _numb);

	void ExtractFile(char const* _name);

	void Print();

private:
	void Init();
	void UpdateData();
};