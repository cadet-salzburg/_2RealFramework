#include "FMODSoundrenderBlock.h"

FMODSoundrenderBlock* FMODSoundrenderBlock::srb = nullptr;

FMODSoundrenderBlock::FMODSoundrenderBlock( _2Real::bundle::ContextBlock& context )
	: Block(),
	  m_FmodManager( static_cast<FMODContextManager*>( &context ) ), 	/*!< Creating FMOD-Context object from 2Real */
	  m_Sound( nullptr ),
	  m_Channel( nullptr )
{
}

FMODSoundrenderBlock::~FMODSoundrenderBlock( void )
{

}

void FMODSoundrenderBlock::setup( _2Real::bundle::BlockHandle &context )
{
	m_IsPlay = context.getInletHandle( "Play" );
	m_FilePath = context.getInletHandle( "FilePath" );
	m_HasLoadedAudio = context.getOutletHandle( "HasLoadedAudio" );
	m_HasLoadedAudio.getWriteableRef<bool>() = false;

	// Create sound ex info for userdefined buffer filling
	memset( &m_SoundExInfo, 0, sizeof( FMOD_CREATESOUNDEXINFO ) );

	m_SoundExInfo.cbsize            = sizeof( FMOD_CREATESOUNDEXINFO );				/* required. */
	m_SoundExInfo.decodebuffersize  = 10240;										/* 10kb Chunk size of stream update in samples.This will be the amount of data passed to the user callback. */
	m_SoundExInfo.numchannels       = 2;											/* Number of channels in the sound. */
	m_SoundExInfo.length            = -1;											/* Length of PCM data in bytes of whole song.  -1 = infinite. */
	m_SoundExInfo.defaultfrequency  = (int)44100;									/* Default playback rate of sound. */
	m_SoundExInfo.format            = FMOD_SOUND_FORMAT_PCM16;						/* Data format of sound. */
	m_SoundExInfo.pcmreadcallback   = pcmReadCallback;								/* User callback for reading. */

	// load file to memory and use it as buffer resource as there is no gstreamer to use
	s = m_FmodManager->CreateSound( "88.mp3", FMOD_OPENONLY, nullptr );
	srb = this;
}

void FMODSoundrenderBlock::update()
{
	//if( m_FilePath.hasChanged() && m_FilePath.getReadableRef<std::string>().size() > 0 )
	//{
	//	
	//	// creating Sound file which buffer will be filled by pcmReadCallback from "remote buffer"
	//	m_Sound = m_FmodManager->CreateSound( m_FilePath.getReadableRef<std::string>().c_str(),
	//										  FMOD_SOFTWARE | FMOD_2D | FMOD_OPENUSER | FMOD_LOOP_OFF,
	//										  &m_SoundExInfo );
	//	if( m_Sound )
	//		m_HasLoadedAudio.getWriteableRef<bool>() = true;
	//	else
	//		m_HasLoadedAudio.getWriteableRef<bool>() = false;
	//}
	// Play logic only on checkfield change
	if( !m_IsPlay.hasChanged() )
		return;

	// set to play sound
	if( m_IsPlay.getReadableRef<bool>() )
	{
		if( !m_Sound )
			m_Sound = m_FmodManager->CreateSound(	"",
													FMOD_2D | FMOD_OPENUSER | FMOD_LOOP_OFF | FMOD_CREATESTREAM,
													&m_SoundExInfo );
		m_Channel = m_FmodManager->PlaySound( m_Sound );
	}
	// toggle stop
	else if( !m_IsPlay.getReadableRef<bool>() && m_Channel )
	{
		bool isPlay = false;
		m_Channel->isPlaying( &isPlay );
		if( isPlay )
		{
			m_Channel->stop();
			m_Channel = nullptr;
		}
	}
}

void FMODSoundrenderBlock::shutdown()
{
	// freeing resources
	bool isPlay = false;
	m_Channel->isPlaying( &isPlay );
	if( isPlay )
		m_Channel->stop();
	m_Channel = nullptr;
	if( m_Sound )
		m_Sound->release();
	m_Sound = nullptr;
}

void FMODSoundrenderBlock::discardOutlets()
{

}

uint8_t* mybuffer = new uint8_t[1048576]; //1mb maximum buffer size
FMOD_RESULT F_CALLBACK pcmReadCallback( FMOD_SOUND *sound, void *data, unsigned int datalen )
{
	// access buffer -> will be a gstreamer call in future
	FMOD::Sound* s = FMODSoundrenderBlock::srb->s;
	FMOD::Channel* c = FMODSoundrenderBlock::srb->c;
	uint32_t readBytes = 0;
	//read data to temporary buffer, datalen is desired buffer-fill-rate in bytes
	s->readData( mybuffer, datalen, &readBytes );
	memcpy( data, mybuffer, readBytes );
	std::cout << datalen << std::endl;
	return FMOD_OK;
}
