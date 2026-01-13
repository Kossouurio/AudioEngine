#include "Waveform_Parser.h"
#include <cstdio>
#include <iostream>
#include <vector>

WAVEParser::WAVEParser()
	: m_header({})
	, m_descriptor({})
	, m_dataInfo({})
	, m_size(0)
{
	m_data = std::vector<char>();
}

WAVEParser::WAVEParser(char const* _path)
	: m_header({})
	, m_descriptor({})
	, m_dataInfo({})
	, m_size(0)
{
	m_data = std::vector<char>();

	ReadWaveFile(_path);
}

void WAVEParser::ReadWaveFile(char const* _path)
{
	FILE* pFile;
	if (fopen_s(&pFile, _path, "rb")) 
	{ std::cout << "Failed to open file!" << '\n'; std::cout << "File path : " << _path << "\n\n"; return;}
	else 
	{ std::cout << "Success to open file!" << '\n';std::cout << "File path : " << _path << "\n\n";}


	fseek(pFile, 0, SEEK_END);
	m_size = ftell(pFile);
	rewind(pFile);

	int offset = 0;
	fread(&m_header, sizeof(WAVE_HEADER), 1, pFile);
	offset += sizeof(WAVE_HEADER);

	fseek(pFile, offset, SEEK_SET);
	fread(&m_descriptor, sizeof(WAVE_DESCRIPTOR), 1, pFile);
	offset += sizeof(WAVE_DESCRIPTOR);

	m_descriptor.BitsPerSample /= 8;


	WAVE_DATA_INFO testt{0, 0};

	for (;;)
	{
		fseek(pFile, offset, SEEK_SET);
		fread(&testt, sizeof(WAVE_DATA_INFO), 1, pFile);
		offset += sizeof(WAVE_DATA_INFO);

		if (testt.DataBlocID == 0x61746164)
		{
			m_dataInfo = testt;
			fseek(pFile, offset, SEEK_SET);

			m_data.resize(m_dataInfo.DataSize);
			fread(&m_data[0], sizeof(m_data[0]), m_data.size(), pFile);

			break;
		}

		offset += testt.DataSize;
	}

	fclose(pFile);
}

void WAVEParser::DivideChannels()
{
	m_channels.ChannelNumber = m_descriptor.NbrCanaux;

	int i = 1;
	for (char* ptr = m_data.data(); ptr < m_data.data() + m_data.size(); ptr += m_descriptor.BitsPerSample)
	{
		m_channels.AddDataTo(i, *ptr, m_descriptor.BitsPerSample);
		i = ((++i - 1) % m_channels.ChannelNumber) + 1;
	}
	return;
}

void WAVEParser::Print()
{
	/// HEADER
	std::cout << "=== WAVE File Information ===" << '\n';
	std::cout << "File Type ID: " << (char*)&m_header.FileTypeBlocID << '\n';
	std::cout << "File Size: " << m_header.FileSize << " bytes" << '\n';
	std::cout << "File Format ID: " << (char*)&m_header.FileFormatID << '\n';
	std::cout << '\n';
	/// DESCRIPTOR FormatBlocID
	std::cout << "Format bloc ID: " << (char*)&m_descriptor.FormatBlocID << '\n';
	std::cout << "Audio Format: " << m_descriptor.AudioFormat << '\n';
	std::cout << "Channels: " << m_descriptor.NbrCanaux << '\n';
	std::cout << "Sample Rate: " << m_descriptor.SampleRate << " Hz" << '\n';
	std::cout << "Byte Rate: " << m_descriptor.BytePerSec << " bytes/sec" << '\n';
	std::cout << "Bytes Per Sample: " << m_descriptor.BitsPerSample << '\n';
	std::cout << '\n';
	/// DATA INFO
	std::cout << "Data ID: " << (char*)&m_dataInfo.DataBlocID << '\n';
	std::cout << "Data Size: " << m_dataInfo.DataSize << " bytes" << '\n';
	std::cout << "Total File Size: " << m_size << " bytes" << '\n';
	std::cout << "=============================" << '\n';
}

