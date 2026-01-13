#include "Waveform_Parser.h"
#include <iostream>

WAVEParser::WAVEParser()
	: m_header({})
	, m_descriptor({})
	, m_dataInfo({})
	, m_size(0)
{
	m_data = std::vector<char>();
}

void WAVEParser::ReadWaveFile(char const* _path)
{
	FILE* pFile;
	if (fopen_s(&pFile, _path, "rb")) 
	{ std::cout << "Failed to open file!" << '\n'; std::cout << "File path : " << _path << '\n'; return;}
	else 
	{ std::cout << "Success to open file!" << '\n';std::cout << "File path : " << _path << '\n';}


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


	WAVE_DATA_INFO testt;
	testt.DataBlocID = 0;
	testt.DataSize = 0;

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

