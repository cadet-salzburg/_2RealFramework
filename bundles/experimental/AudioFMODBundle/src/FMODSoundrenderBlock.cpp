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
	m_SoundExInfo.decodebuffersize  = 1024;											/* Chunk size of stream update in samples.  This will be the amount of data passed to the user callback. */
	m_SoundExInfo.numchannels       = 2;											/* Number of channels in the sound. */
	m_SoundExInfo.length            = -1;											/* Length of PCM data in bytes of whole song.  -1 = infinite. */
	m_SoundExInfo.defaultfrequency  = (int)44100;									/* Default playback rate of sound. */
	m_SoundExInfo.format            = FMOD_SOUND_FORMAT_PCM16;						/* Data format of sound. */
	m_SoundExInfo.pcmreadcallback   = pcmReadCallback;								/* User callback for reading. */

	m_FmodManager->m_System->createSound( "88.mp3", FMOD_SOFTWARE, nullptr, &s );
	srb = this;
}

void FMODSoundrenderBlock::update()
{
	if( m_FilePath.hasChanged() && m_FilePath.getReadableRef<std::string>().size() > 0 )
	{
		if( m_Sound )
			m_Sound->release();
		// creating Sound file which buffer will be filled by pcmReadCallback from "remote buffer"
		m_Sound = m_FmodManager->CreateSound( m_FilePath.getReadableRef<std::string>().c_str(),
											  FMOD_SOFTWARE | FMOD_2D | FMOD_OPENUSER | FMOD_LOOP_OFF,
											  &m_SoundExInfo );
		if( m_Sound )
			m_HasLoadedAudio.getWriteableRef<bool>() = true;
		else
			m_HasLoadedAudio.getWriteableRef<bool>() = false;
	}

	if( !m_IsPlay.hasChanged() )
		return;

	if( m_IsPlay.getReadableRef<bool>() && !m_Sound && !m_Channel )
	{
		m_Sound = m_FmodManager->CreateSound( m_FilePath.getReadableRef<std::string>().c_str(),
											  FMOD_SOFTWARE | FMOD_2D | FMOD_OPENUSER | FMOD_LOOP_OFF,
											  &m_SoundExInfo );
		m_Channel = m_FmodManager->PlaySound( m_Sound );
	}
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

FMOD_RESULT F_CALLBACK pcmReadCallback( FMOD_SOUND *sound, void *data, unsigned int datalen )
{
//static long oldDts=-1;
//
//int len = 2048*sizeof(short);
//if(audioData.m_pData!=nullptr)
//	memcpy(data, audioData.m_pData, len);
//
//oldDts = audioData.m_lDts;

	uint16_t buffer[1024];
	uint32_t readBytes = 0;
	FMODSoundrenderBlock::srb->s->readData( buffer, 30, &readBytes );
	memcpy( data, buffer, readBytes );

	return FMOD_OK;
}
