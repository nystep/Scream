#include "oggPlayer.h"
#include <malloc.h>
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#define STB_VORBIS_NO_STDIO
#define STB_VORBIS_NO_PUSHDATA_API
#include <stb_vorbis.h>


namespace SCore {

	static HWAVEOUT hWaveOut;

	static DWORD decodeOggThread( LPVOID data )
	{
		stb_vorbis *stream = (stb_vorbis*) data;
		unsigned int sampleRate = 44100;
		unsigned int bufferSz = 65536;

		unsigned int samplesDecoded = 0;
		unsigned int streamTotalSamples = stb_vorbis_stream_length_in_samples( stream );

		WAVEFORMATEX *WaveFMT = (WAVEFORMATEX*) malloc( sizeof(WAVEFORMATEX) );
		memset( WaveFMT, 0, sizeof(WAVEFORMATEX) );
		WaveFMT->nChannels = 2;
		WaveFMT->nSamplesPerSec = sampleRate;
		WaveFMT->nAvgBytesPerSec = sampleRate*sizeof(float)*WaveFMT->nChannels;
		WaveFMT->wBitsPerSample = sizeof(float)*8;
		WaveFMT->nBlockAlign = sizeof(float)*WaveFMT->nChannels;
		WaveFMT->wFormatTag = WAVE_FORMAT_IEEE_FLOAT;

		float *soundBuffer = (float*) _mm_malloc( bufferSz*sizeof(float)*2, WaveFMT->nBlockAlign );

		WAVEHDR *WaveHDR = (WAVEHDR*) malloc( sizeof(WAVEHDR) );
		memset( WaveHDR, 0, sizeof(WAVEHDR) );
		WaveHDR->lpData = (LPSTR) soundBuffer;
		WaveHDR->dwBufferLength = sizeof(float)*bufferSz*2;
		WaveHDR->dwLoops = (streamTotalSamples / bufferSz) + 1;
		WaveHDR->dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;

		samplesDecoded += stb_vorbis_get_samples_float_interleaved( stream, 2, (float*) WaveHDR->lpData, bufferSz*2 );

		MMRESULT init = waveOutOpen( &hWaveOut, WAVE_MAPPER, WaveFMT, NULL, 0, CALLBACK_NULL );
		MMRESULT res = waveOutPrepareHeader( hWaveOut, WaveHDR, sizeof(WAVEHDR) );
		MMRESULT res2 = waveOutWrite( hWaveOut, WaveHDR, sizeof(WAVEHDR) );

		MMTIME time;
		memset( &time, 0, sizeof(MMTIME) );

		while (samplesDecoded < streamTotalSamples)
		{
			waveOutGetPosition( hWaveOut, &time, sizeof(MMTIME) );
			unsigned int bufferPos = time.u.sample / (sizeof(float)*2);

			if ((samplesDecoded-bufferPos) < (bufferSz/2))
			{
				unsigned int samplesDecodedModSz = samplesDecoded & (bufferSz-1);
				unsigned int bufferPosModSz = bufferPos & (bufferSz-1);

				if (samplesDecodedModSz>bufferPosModSz)
				{
					samplesDecoded += stb_vorbis_get_samples_float_interleaved( 
						stream, 2, ((float*) WaveHDR->lpData)+samplesDecodedModSz*2, 
						(bufferSz-samplesDecodedModSz+1)*2 );
					samplesDecoded += stb_vorbis_get_samples_float_interleaved( 
						stream, 2, (float*) WaveHDR->lpData, 
						(bufferPosModSz-1)*2 );
				}
				else
				{
					samplesDecoded += stb_vorbis_get_samples_float_interleaved( 
						stream, 2, ((float*) WaveHDR->lpData)+samplesDecodedModSz*2, 
						(bufferPosModSz-samplesDecodedModSz+1)*2 );
				}
			}

			Sleep( 20 );
		}

		waveOutUnprepareHeader( hWaveOut, WaveHDR, sizeof(WAVEHDR) );
		free( WaveHDR );
		_mm_free( soundBuffer );
		waveOutClose( hWaveOut );

		return 0;
	}

	bool oggPlayer::decodeDiskStream( const char *filename )
	{
		FILE *f = fopen( filename, "rb" );
		if (!f) return false;
		fseek( f, 0, SEEK_END );
		size_t sz = ftell( f );
		fseek( f, 0, SEEK_SET );
		unsigned char *memblock = (unsigned char*) malloc( sz );
		fread( memblock, 1, sz, f );
		fclose( f );
		return decodeMemoryStream( memblock, sz );
	}

	bool oggPlayer::decodeMemoryStream( void* data, unsigned int sz )
	{
		int err;
		stream = stb_vorbis_open_memory( (unsigned char*) data, sz, &err, 0 );
		return stream != 0;
	}

	void oggPlayer::startPlaying()
	{
		if (stream)
		{
			stb_vorbis_seek_start( (stb_vorbis*) stream );
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE) decodeOggThread, stream, 0, 0);
		}
	}

	float oggPlayer::getPlayedTimeSeconds()
	{
		float timePlayed = 0.0f;

		if (hWaveOut)
		{
			MMTIME time;
			memset( &time, 0, sizeof(MMTIME) );
			waveOutGetPosition( hWaveOut, &time, sizeof(MMTIME) );
			unsigned int bufferPos = time.u.sample / (sizeof(float)*2);
			timePlayed = float(bufferPos) / 44100;
		}

		return timePlayed;
	}

	oggPlayer::~oggPlayer()
	{
		if (stream) free( stream );
		stream = 0;
	}

}
