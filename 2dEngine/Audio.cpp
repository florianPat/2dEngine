#include "Audio.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPGUID, LPDIRECTSOUND*, LPUNKNOWN)
typedef DIRECT_SOUND_CREATE(direct_sound_create);

eg::Audio::Audio(HWND windowHandle, int samlesPerSecound, int bufferSize) : samplesPerSecound(samlesPerSecound), wavePeriod(samlesPerSecound/hz),
																			bufferSize(bufferSize)
{
	HMODULE dSoundLibary = LoadLibraryA("dsound.dll");

	if (dSoundLibary)
	{
		direct_sound_create* directSoundCreate = (direct_sound_create*) GetProcAddress(dSoundLibary, "DirectSoundCreate");

		if (directSoundCreate)
		{
			LPDIRECTSOUND directSound;
			if (SUCCEEDED(directSoundCreate(0, &directSound, 0)))
			{
				WAVEFORMATEX waveFormat = { 0 };
				waveFormat.wFormatTag = WAVE_FORMAT_PCM;
				waveFormat.nChannels = 2;
				waveFormat.nSamplesPerSec = samlesPerSecound;
				waveFormat.wBitsPerSample = 16;
				waveFormat.nBlockAlign = waveFormat.nChannels * waveFormat.wBitsPerSample / 8;
				waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;

				if (SUCCEEDED(directSound->SetCooperativeLevel(windowHandle, DSSCL_PRIORITY)))
				{
					DSBUFFERDESC bufferDescription = { 0 };
					bufferDescription.dwSize = sizeof(bufferDescription);
					bufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;

					LPDIRECTSOUNDBUFFER primaryBuffer;
					if (SUCCEEDED(directSound->CreateSoundBuffer(&bufferDescription, &primaryBuffer, 0)))
					{
						if (!SUCCEEDED(primaryBuffer->SetFormat(&waveFormat)))
							utilsLogBreak("unsucceeded call to setFormat");
					}
					else
						utilsLogBreak("unsucceeded call to createSoundBuffer for primaryBuffer!");
				}
				else
					utilsLogBreak("unsucceeded call to setCooperativeLevel!");

				DSBUFFERDESC bufferDescription = { 0 };
				bufferDescription.dwSize = sizeof(bufferDescription);
				bufferDescription.dwBufferBytes = bufferSize;
				bufferDescription.lpwfxFormat = &waveFormat;

				if (SUCCEEDED(directSound->CreateSoundBuffer(&bufferDescription, &secondaryBuffer, 0)))
				{
					secondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
				}
				else
					utilsLogBreak("unsucceeded call to createSoundBuffer for secondaryBuffer!");
			}
			else
				utilsLogBreak("unsucceeded call to DirectSoundCreate!");
		}
		else
			utilsLogBreak("DirectSoundCreate could not be called!");
	}
	else
		utilsLogBreak("dsound.dll not found!");
}

void eg::Audio::outputTest()
{
	DWORD writeCursor, playCursor;

	if (SUCCEEDED(secondaryBuffer->GetCurrentPosition(&playCursor, &writeCursor)))
	{
		VOID* region0;
		DWORD region0Size;
		VOID* region1;
		DWORD region1Size;

		DWORD byteToLock = sampleIndex * bytesPerSample % bufferSize;
		DWORD bytesToWrite = 0;
		if (byteToLock > playCursor)
		{
			bytesToWrite = bufferSize - byteToLock + playCursor;
		}
		else
		{
			bytesToWrite = playCursor - byteToLock;
		}

		if (SUCCEEDED(secondaryBuffer->Lock(byteToLock, bytesToWrite, &region0, &region0Size, &region1, &region1Size, 0)))
		{
			short* sampleOutput = (short*)region0;
			int region0SampleCount = region0Size / bytesPerSample;
			for (int i = 0; i < region0SampleCount; ++i, ++sampleIndex)
			{
				float t = 2.0f * (float)M_PI * (float)sampleIndex / (float)wavePeriod;
				short sampleValue = (short)(sinf(t) * volume);

				*sampleOutput++ = sampleValue;
				*sampleOutput++ = sampleValue;
			}

			sampleOutput = (short*)region1;
			int region1SampleCount = region1Size / bytesPerSample;
			for (int i = 0; i < region1SampleCount; ++i, ++sampleIndex)
			{
				float t = 2.0f * (float)M_PI * (float)sampleIndex / (float)wavePeriod;
				short sampleValue = (short)(sinf(t) * volume);

				*sampleOutput++ = sampleValue;
				*sampleOutput++ = sampleValue;
			}

			if (!SUCCEEDED(secondaryBuffer->Unlock(region0, region0Size, region1, region1Size)))
				utilsLogBreak("unlock failed!");
		}
		else
		{
			//NOTE / TODO: lock sometimes failes!!
			utilsLog("lock failed!");
		}
	}
	else
		utilsLogBreak("getCurrentPos failed!");
}
