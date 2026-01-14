#include "Waveform_Parser.h"
#include <cstdio>
#include <iostream>
#include <vector>

namespace MB
{
#define RIFF_ID 0x46464952
#define WAVE_ID 0x45564157
#define FMT_ID  0x20746D66
#define DATA_ID 0x61746164

	WAVEParser::WAVEParser()
		: m_header({})
		, m_descriptor({})
		, m_dataInfo({})
		, m_size(0)
	{
		m_data = std::vector<char>();
		Init();
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
		m_channels.ChannelNumber = m_descriptor.NbrChannels;

		int i = 1;
		for (char* ptr = m_data.data(); ptr < m_data.data() + m_data.size(); ptr += m_descriptor.BitsPerSample)
		{
			m_channels.AddDataTo(i, *ptr, m_descriptor.BitsPerSample);
			i = ((++i - 1) % m_channels.ChannelNumber) + 1;
		}
		return;
	}

	void WAVEParser::SetData(std::vector<float>& _data)
	{
		std::vector<char> newData;
		//memccpy(&newData, &_data, 1, sizeof(std::vector<char>));

		newData.resize(_data.size() * sizeof(float));
		memcpy(newData.data(), _data.data(), newData.size());

		m_data = newData;
		m_dataInfo.DataSize = newData.size();
		m_descriptor.AudioFormat = 3;	/// 3: IEEE 754 float
		m_descriptor.BitsPerSample = sizeof(float) * 8;
		UpdateData();
	}

	void WAVEParser::SetData(std::vector<int>& _data)
	{
		std::vector<char> newData;
		//memccpy(&newData, &_data, 1, sizeof(std::vector<char>));
		newData.resize(_data.size() * sizeof(int));
		memcpy(newData.data(), _data.data(), newData.size());

		m_data = newData;
		m_dataInfo.DataSize = newData.size();
		m_descriptor.AudioFormat = 1;	/// 1: PCM integer
		m_descriptor.BitsPerSample = sizeof(int) * 8;
		UpdateData();
	}

	void WAVEParser::SetSampleRate(uint32_t _numb)
	{
		m_descriptor.SampleRate = _numb;
		UpdateData();
	}

	void WAVEParser::SetChannelNumber(uint32_t _numb)
	{
		m_descriptor.NbrChannels = _numb;
		UpdateData();
	}

	void WAVEParser::ExtractFile(char const* _name)
	{
		m_header.FileSize = sizeof(WAVE_HEADER) + sizeof(WAVE_DESCRIPTOR) + sizeof(WAVE_DATA_INFO) + m_data.size() - 8;


		FILE* pFile;
		if (fopen_s(&pFile, _name, "wb"))
		{
			std::cout << "Failed to open file!" << '\n'; std::cout << "File path : " << _name << "\n\n"; return;
		}
		else
		{
			std::cout << "Success to open file!" << '\n'; std::cout << "File path : " << _name << "\n\n";
		}

		if (fwrite(&m_header, sizeof(WAVE_HEADER), 1, pFile) != 1) { std::cout << "Write Header : Not Good !" << "\n"; };
		if (fwrite(&m_descriptor, sizeof(WAVE_DESCRIPTOR), 1, pFile) != 1) { std::cout << "Write Descriptor : Not Good !" << "\n"; };
		if (fwrite(&m_dataInfo, sizeof(WAVE_DATA_INFO), 1, pFile) != 1) { std::cout << "Write Data Info : Not Good !" << "\n"; };
		if (fwrite(m_data.data(), m_data.size(), 1, pFile) != 1) { std::cout << "Write Data : Not Good !" << "\n"; };

		m_size = ftell(pFile);
		fclose(pFile);

		Print();
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
		std::cout << "Channels: " << m_descriptor.NbrChannels << '\n';
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

	void WAVEParser::Init()
	{
		m_header.FileTypeBlocID = RIFF_ID;	/// 0x52, 0x49, 0x46, 0x46 
		m_header.FileSize = 36;	/// Overall file size minus 8 bytes
		m_header.FileFormatID = WAVE_ID;	/// 0x57, 0x41, 0x56, 0x45

		m_descriptor.FormatBlocID = FMT_ID;	/// 0x66, 0x6D, 0x74, 0x20
		m_descriptor.BlocSize = 16;	/// 0x10 Constant for PCM
		m_descriptor.AudioFormat = 1;	/// 1: PCM integer, 3: IEEE 754 float
		m_descriptor.NbrChannels = 1;	/// 
		m_descriptor.SampleRate = 44100;	/// 
		m_descriptor.BitsPerSample = 32;	/// 
		m_descriptor.BytePerBloc = m_descriptor.NbrChannels * (m_descriptor.BitsPerSample / 8);	/// NbrChannels * BitsPerSample/8
		m_descriptor.BytePerSec = m_descriptor.SampleRate * m_descriptor.BytePerBloc;	/// Frequency * BytePerBloc

		m_dataInfo.DataBlocID = DATA_ID;	/// 0x64, 0x61, 0x74, 0x61
		m_dataInfo.DataSize = 0;	/// 
	}

	void WAVEParser::UpdateData()
	{
		uint16_t bytePerBloc = m_descriptor.NbrChannels * m_descriptor.BitsPerSample;
		uint16_t bytePerSec = m_descriptor.SampleRate * m_descriptor.BytePerBloc;
		m_descriptor.BytePerBloc = bytePerBloc;	/// NbrChannels * BitsPerSample/8
		m_descriptor.BytePerSec = bytePerSec;	/// Frequency * BytePerBloc
	}

}