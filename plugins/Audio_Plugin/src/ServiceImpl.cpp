#include "ServiceImpl.h"
#include "_2RealException.h"
#include "_2RealEnum.h"
#include "Poco/Mutex.h"

#include "portaudio.h"

#define NUM_SECONDS         (5)
#define SAMPLE_RATE         (44100)
#define FRAMES_PER_BUFFER   (1024)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE   (200)

using namespace _2Real;
using namespace std;

void AudioOutputService::setup(_2Real::ServiceContext &context)
{
	try
	{
	  
		m_Test = context.getOutputHandle("test");

		PaStreamParameters outputParameters;
		PaStream *stream;
		PaError err;

		float leftBuffer[FRAMES_PER_BUFFER];
		float rightBuffer[FRAMES_PER_BUFFER];
		void *buffers[2]; /* points to both non-interleaved buffers. */

		float sine[TABLE_SIZE]; /* sine wavetable */
		int left_phase = 0;
		int right_phase = 0;
		int left_inc = 1;
		int right_inc = 3; /* higher pitch so we can distinguish left and right. */
		int i, j, k;
		int bufferCount;


		printf("PortAudio Test: output sine wave NON-INTERLEAVED. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);

		/* initialise sinusoidal wavetable */
		for( i=0; i<TABLE_SIZE; i++ )
		{
			sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
		}


		err = Pa_Initialize();
		if( err != paNoError ) goto error;

		outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
		if (outputParameters.device == paNoDevice) {
			fprintf(stderr,"Error: No default output device.\n");
			goto error;
		}
		outputParameters.channelCount = 2;       /* stereo output */
		outputParameters.sampleFormat = paFloat32 | paNonInterleaved; /* 32 bit floating point output NON-INTERLEAVED */
		outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
		outputParameters.hostApiSpecificStreamInfo = NULL;

		err = Pa_OpenStream(
			&stream,
			NULL, /* no input */
			&outputParameters,
			SAMPLE_RATE,
			FRAMES_PER_BUFFER,
			paClipOff,      /* we won't output out of range samples so don't bother clipping them */
			NULL, /* no callback, use blocking API */
			NULL ); /* no callback, so no callback userData */
		if( err != paNoError ) goto error;


		printf( "Play 3 times, higher each time.\n" );

		/* Set up array of buffer pointers for Pa_WriteStream */
		buffers[0] = leftBuffer;
		buffers[1] = rightBuffer;
	 
		for( k=0; k < 3; ++k )
		{
			err = Pa_StartStream( stream );
			if( err != paNoError ) goto error;

			printf("Play for %d seconds.\n", NUM_SECONDS );

			bufferCount = ((NUM_SECONDS * SAMPLE_RATE) / FRAMES_PER_BUFFER);

			for( i=0; i < bufferCount; i++ )
			{
				for( j=0; j < FRAMES_PER_BUFFER; j++ )
				{
					leftBuffer[j] = sine[left_phase];  /* left */
					rightBuffer[j] = sine[right_phase];  /* right */
					left_phase += left_inc;
					if( left_phase >= TABLE_SIZE ) left_phase -= TABLE_SIZE;
					right_phase += right_inc;
					if( right_phase >= TABLE_SIZE ) right_phase -= TABLE_SIZE;
				}

				err = Pa_WriteStream( stream, buffers, FRAMES_PER_BUFFER );
				if( err != paNoError ) goto error;
			}   

			err = Pa_StopStream( stream );
			if( err != paNoError ) goto error;

			++left_inc;
			++right_inc;

			Pa_Sleep( 1000 );
		}

		err = Pa_CloseStream( stream );
		if( err != paNoError ) goto error;

		Pa_Terminate();
		printf("Test finished.\n");


error:
		Pa_Terminate();
		fprintf( stderr, "An error occured while using the portaudio stream\n" );
		fprintf( stderr, "Error number: %d\n", err );
		fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
		 

	}
	catch (_2Real::Exception &e)
	{
		e.rethrow();
	}
	catch (std::exception &e)
	{
		std::ostringstream msg;
		msg << "exception during service setup: " << e.what();
		throw _2Real::ServiceException(msg.str());
	}
};

void AudioOutputService::update()
{
	try
	{
 

	}
	catch (_2Real::Exception &e)
	{
		e.rethrow();
	}
	catch (std::exception &e)
	{
		std::ostringstream msg;
		msg << "exception during service update: " << e.what();
		throw _2Real::ServiceException(msg.str());
	}
};

void AudioOutputService::shutdown()
{
	 
}
 