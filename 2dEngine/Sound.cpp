#include "Sound.h"
#include <fstream>
#include <memory>

eg::Sound::Sound(const std::string & filename) : samples()
{
	std::ifstream file(filename, std::ifstream::binary);

	if (!file.good())
	{
		utilsLogBreak("Could not open file!");
	}

	file.seekg(0, file.end);
	int fileSize = (int) file.tellg();
	file.seekg(0, file.beg);

	std::unique_ptr<char[]> fileContents = std::make_unique<char[]>(fileSize);
	file.read(fileContents.get(), fileSize);

	FileHeader* fileHeader = (FileHeader*) fileContents.get();

	assert(fileHeader->riffId == (uint)ChunkId::RIFF);
	assert(fileHeader->waveId == (uint)ChunkId::WAVE);

	short* sampleData = nullptr;
	uint sampleDataSize = 0;

	for (RiffIt it = RiffIt(fileHeader + 1, (fileHeader + 1) + fileHeader->size - 4); it && (!sampleData || !nChannels); ++it)
	{
		switch (it.getType())
		{
			case (uint)ChunkId::FMT:
			{
				Fmt* fmt = (Fmt*)it.getChunkData();
				nChannels = fmt->nChannles;

				assert(fmt->wFormatTag == 1); //NOTE: Only PCM music!
				assert(fmt->nSamplesPerSecond == 48000);
				assert(fmt->wBitsPerSample == 16);
				assert(fmt->nBlockAlign == (sizeof(short)*fmt->nChannles));
				break;
			}
			case (uint)ChunkId::DATA:
			{
				sampleData = (short*)it.getChunkData();
				sampleDataSize = it.getChunkDataSize();
				break;
			}
		}
	}

	assert(nChannels && sampleData && sampleDataSize);

	nSamples = sampleDataSize / (nChannels * sizeof(short));

	nChannels = 1;

	//TODO: Think about how to do that faster!!
	if (nChannels == 1)
	{
		samples.push_back(std::vector<short>(nSamples));

		for (int i = 0; i < nSamples; ++i)
		{
			samples[0][i] = sampleData[i];
		}
	}
	else if (nChannels == 2)
	{
		samples.push_back(std::vector<short>(nSamples / 2));
		samples.push_back(std::vector<short>(nSamples / 2));

		for (int i = 0; i < nSamples;)
		{
			samples[0][i] = sampleData[i++];
			samples[1][i] = sampleData[i++];
		}
	}
	else
		utilsLogBreak("Invalid channel count!");
}

const std::vector<std::vector<short>>& eg::Sound::getSamples() const
{
	return samples;
}

const int eg::Sound::getNSamples() const
{
	return nSamples;
}

eg::Sound::RiffIt::RiffIt(void * at, void* stop) : at(reinterpret_cast<uchar*>(at)), stop(reinterpret_cast<uchar*>(stop))
{
}

eg::Sound::RiffIt::operator bool() const
{
	return this->at < this->stop;
}

eg::Sound::RiffIt& eg::Sound::RiffIt::operator++()
{
	Chunk* chunk = (Chunk*)at;
	uint size = chunk->size;
	if (size % 2 != 0)
		++size;
	at += sizeof(Chunk) + size;
	return *this;
}

uint eg::Sound::RiffIt::getChunkDataSize() const
{
	Chunk* chunk = (Chunk*)at;
	return chunk->size;
}

void * eg::Sound::RiffIt::getChunkData() const
{
	return at + sizeof(Chunk);
}

uint eg::Sound::RiffIt::getType() const
{
	Chunk* chunk = (Chunk*)at;
	return chunk->id;
}
