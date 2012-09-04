#include "FMODContextManager.h"
#include <iostream>
#include <fmod_errors.h>
#include <fmod_output.h>
#include <sstream>

using namespace _2Real::bundle;
using _2Real::bundle::Block;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

using namespace FMOD;

FMODContextManager::FMODContextManager()
	: ContextBlock(),
	  m_System( nullptr ),
	  m_FmodVersion( 0 ),
	  m_NumDriver( -1 )
{

}

FMODContextManager::~FMODContextManager()
{

}

void FMODContextManager::setup( _2Real::bundle::BlockHandle &context )
{
	try
	{
		// Base fmod init in a special order recommended by fmod-forum http://www.fmod.org/forum/viewtopic.php?f=18&t=14604
		checkFMODError( "Setup()", FMOD::System_Create( &m_System ) );
		checkFMODError( "Setup()", m_System->getVersion( &m_FmodVersion ) );

		if( m_FmodVersion < FMOD_VERSION )
		{
			char msg[100];
			sprintf( msg, "Setup(): Error, you are using an old version of FMOD %08x. This program requires %08x\n", m_FmodVersion, FMOD_VERSION );
			throw FMODContextException( msg );
		}

		// <3 Ömer
		checkFMODError( "Setup()", m_System->getNumDrivers( &m_NumDriver ) );
		// Skip EAX probe which can crash/corrupt some bad drivers.
		checkFMODError( "Setup()", m_System->setHardwareChannels( 0 ) );  

		if ( m_NumDriver == 0 )
			checkFMODError( "Setup()", m_System->setOutput( FMOD_OUTPUTTYPE_NOSOUND ) );
		else
		{
			checkFMODError( "Setup()", m_System->getDriverCaps( 0, &m_FmodCaps, 0, &m_Speakermode ) );
			// Set the user selected speaker mode.
			checkFMODError( "Setup()", m_System->setSpeakerMode( m_Speakermode ) );       

			// The user has the 'Acceleration' slider set to off!  This is really bad for latency!.You might want to warn the user about this
			if ( m_FmodCaps & FMOD_CAPS_HARDWARE_EMULATED ) 
				checkFMODError( "Setup()", m_System->setDSPBufferSize( 1024, 10 ) );
			
			// Fetch name of device 
			char name[256];
			checkFMODError( "Setup()", m_System->getDriverInfo( 0, name, 256, 0 ) );
			m_Name = std::string( name );

			// Sigmatel sound devices crackle for some reason if the format is PCM 16bit. PCM floating point output seems to solve it.
			if( strstr( name, "SigmaTel" ) )   
				checkFMODError( "Setup()", m_System->setSoftwareFormat( 48000,
																		FMOD_SOUND_FORMAT_PCMFLOAT,
																		0,
																		0,
																		FMOD_DSP_RESAMPLER_LINEAR ) );
		}

		// Init System
		// Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo...
		if ( m_System->init( 100, FMOD_INIT_NORMAL, 0 ) == FMOD_ERR_OUTPUT_CREATEBUFFER )         
		{
			checkFMODError( "Setup()", m_System->setSpeakerMode( FMOD_SPEAKERMODE_STEREO ) );	
			checkFMODError( "Setup()", m_System->init( 100, FMOD_INIT_NORMAL, 0 ) );/* ... and re-init. */
		}
	}
	catch ( FMODContextException& e )
	{
		std::cout << "Error in FMODContextManager::setup: " << e.what() << std::endl;
	}
}

void FMODContextManager::update()
{
	m_System->update();
}

void FMODContextManager::shutdown()
{
	if( m_System )
	{
		m_System->close();
		m_System->release();
	}
	m_System = nullptr;
}

bool FMODContextManager::bindDevice( const unsigned int deviceIdx, int w, int h, int fps/*=30*/ )
{
	return true;
}

void FMODContextManager::unbindDevice( const unsigned int deviceIdx )
{

}

void FMODContextManager::checkFMODError( const char* method, const FMOD_RESULT& result ) const
{
	if( result != FMOD_OK )
	{
		std::stringstream string;
		string << method << ": " << FMOD_ErrorString( result ) << std::endl;
		throw FMODContextException( string.str().c_str() );
	}
}

FMOD::Sound* FMODContextManager::CreateSound( std::string file, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO* exInfo )
{
	try
	{
		FMOD::Sound* sound = nullptr;
		checkFMODError( "CreateSound()", m_System->createStream( 0, mode, exInfo, &sound ) );
		return sound;
	}
	catch( FMODContextException& e )
	{
		std::cout << "Error in FMODContextManager::CreateSound: " << e.what() << std::endl;
		return nullptr;
	}
}

FMOD::Channel* FMODContextManager::PlaySound( FMOD::Sound* sound )
{
	try
	{
		FMOD::Channel* channel;
		checkFMODError( "PlaySound()", m_System->playSound( FMOD_CHANNEL_FREE, sound, false, &channel ) );
		return channel;
	}
	catch (FMODContextException& e)
	{
		std::cout << "Error in FMODContextManager::PlaySound: " << e.what() << std::endl;
		return nullptr;
	}
}

void FMODContextManager::CreateSoundExInfo( FMOD_CREATESOUNDEXINFO* outInfo, const uint32_t sizeDecodeBuffer,
	const uint8_t numChannels, const int lengthPCM, const uint16_t defaultFrequenzy, const FMOD_SOUND_FORMAT format,
	FMOD_SOUND_PCMREADCALLBACK callback )
{
}
