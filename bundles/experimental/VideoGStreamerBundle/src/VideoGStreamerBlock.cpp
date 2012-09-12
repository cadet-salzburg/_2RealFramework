#include "VideoGStreamerBlock.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;
using namespace _2RealGStreamerWrapper;

VideoGStreamerBlock::VideoGStreamerBlock() :
	Block()
{
	
}

VideoGStreamerBlock::~VideoGStreamerBlock()
{

}

void VideoGStreamerBlock::update()
{
	try
	{
		// Open a new file if the file path inlet has a new value
		// After opening a new file update the outlets with appropriate values according to the new file
		if ( m_FilePath.hasChanged() )
		{
			m_2RealGStreamerPlayer->open( m_FilePath.getReadableRef<string>(), true, m_ManualAudioBuffer.getReadableRef<bool>() );
			updateOutlets();
		}

		// Change the play state if the inlet receives a new value
		if ( m_PlayState.hasChanged() )
			setPlayState( m_PlayState.getReadableRef<int>() );

		// Seek to a new position in the media file if the inlet receives a new value
		if ( m_Position.hasChanged() )
			m_2RealGStreamerPlayer->setPosition( m_Position.getReadableRef<float>() );

		// Changes the playback direction if the inlet receives a new value
		if ( m_Direction.hasChanged() )
			setDirection( m_Direction.getReadableRef<int>() );

		// Changes the play speed if the inlet receives a new value
		if ( m_Speed.hasChanged() )
			m_2RealGStreamerPlayer->setSpeed( m_Speed.getReadableRef<float>() );

		// Changes the volume if the inlet receives a new value
		if ( m_Volume.hasChanged() )
			m_2RealGStreamerPlayer->setVolume( m_Volume.getReadableRef<float>() );

		// Changes the loop mode if the inlet receives a new value
		if ( m_LoopMode.hasChanged() )
			setLoopMode( m_LoopMode.getReadableRef<int>() );


		// Update wrapper
		m_2RealGStreamerPlayer->update();

		// Update Video
		unsigned char* cVideoPixels = m_2RealGStreamerPlayer->getVideo();
		if ( cVideoPixels != NULL ) // Doesn't work with "isNewVideoFrame()" --> why ???
		{
			m_Video.getWriteableRef<Image>() = Image( cVideoPixels, 
													  false,
													  m_2RealGStreamerPlayer->getWidth(),
													  m_2RealGStreamerPlayer->getHeight(),
													  ImageChannelOrder::RGB );
		}

		// Update Audio
		if ( m_ManualAudioBuffer.getReadableRef<bool>() )
		{
			unsigned char* cAudioBuffer = m_2RealGStreamerPlayer->getAudio();
			if ( cAudioBuffer != NULL )
			{
				m_Audio.getWriteableRef<AudioBuffer>() = AudioBuffer( cAudioBuffer,
																	  false,
																	  m_2RealGStreamerPlayer->getAudioBufferSize(),
																	  m_2RealGStreamerPlayer->getAudioSampleRate(),
																	  0, // sample count ??
																	  m_2RealGStreamerPlayer->getNumOfAudioChannels(),
																	  0, // bitResolution ??
																	  0 ); // pts ??
			}
		}

	}
	catch ( Exception& e )
	{
		discardAllOutlets();
		cout << e.message() << endl;
		e.rethrow();
	}
}

void VideoGStreamerBlock::setup( BlockHandle &context )
{
	try
	{
		// Get all Inlet handles
		m_FilePath = context.getInletHandle( "FilePath" );
		m_PlayState = context.getInletHandle( "PlayState" );
		m_Position = context.getInletHandle( "Position" );
		m_Direction = context.getInletHandle( "Direction" );
		m_Speed = context.getInletHandle( "Speed" );
		m_Volume = context.getInletHandle( "Volume" );
		m_LoopMode = context.getInletHandle( "LoopMode" );
		m_ManualAudioBuffer = context.getInletHandle( "ManualAudioBuffer" );

		// Get all outlet handles
		m_Video = context.getOutletHandle( "Video" );
		m_Width = context.getOutletHandle( "Width" );
		m_Height = context.getOutletHandle( "Height" );
		m_NumberOfFrames = context.getOutletHandle( "NumberOfFrames" );
		m_FPS = context.getOutletHandle( "FPS" );
		m_Audio = context.getOutletHandle( "Audio" );

		// D:/TestProjects/GStreamerTest/bin/Debug/acl.avi

		// Init GStreamerWrapper
		m_2RealGStreamerPlayer = new GStreamerWrapper();
	}
	catch ( Exception& e )
	{
		discardAllOutlets();
		cout << e.message() << endl;
		e.rethrow();
	}
}

void VideoGStreamerBlock::shutdown()
{
	discardAllOutlets();
	m_2RealGStreamerPlayer->stop();
	delete m_2RealGStreamerPlayer;
}

void VideoGStreamerBlock::setPlayState( int iPlayState )
{
	switch ( iPlayState )
	{
		case 0:
			m_2RealGStreamerPlayer->play();
			break;
		case 1:
			m_2RealGStreamerPlayer->pause();
			break;
		case 2:
			m_2RealGStreamerPlayer->stop();
			break;
		default:
			break;
	}
}

void VideoGStreamerBlock::setDirection( int iDirection )
{
	switch ( iDirection )
	{
		case 0:
			m_2RealGStreamerPlayer->setDirection( FORWARD );
			break;
		case 1:
			m_2RealGStreamerPlayer->setDirection( BACKWARD );
			break;
		default:
			break;
	}
}

void VideoGStreamerBlock::setLoopMode( int iLoopMode )
{
	switch ( iLoopMode )
	{
	case 0:
		m_2RealGStreamerPlayer->setLoopMode( NO_LOOP );
		break;
	case 1:
		m_2RealGStreamerPlayer->setLoopMode( LOOP );
		break;
	case 2:
		m_2RealGStreamerPlayer->setLoopMode( BIDIRECTIONAL_LOOP );
		break;
	default:
		break;
	}
}

void VideoGStreamerBlock::updateOutlets()
{
	if ( m_2RealGStreamerPlayer->hasVideo() )
	{
		m_Width.getWriteableRef<int>() = m_2RealGStreamerPlayer->getWidth();
		m_Height.getWriteableRef<int>() = m_2RealGStreamerPlayer->getHeight();
		m_NumberOfFrames.getWriteableRef<unsigned long>() = m_2RealGStreamerPlayer->getNumberOfFrames();
		m_FPS.getWriteableRef<float>() = m_2RealGStreamerPlayer->getFps();
	}
}

void VideoGStreamerBlock::discardAllOutlets()
{
	m_Video.discard();
	m_Width.discard();
	m_Height.discard();
	m_NumberOfFrames.discard();
	m_FPS.discard();
	m_Audio.discard();
}
