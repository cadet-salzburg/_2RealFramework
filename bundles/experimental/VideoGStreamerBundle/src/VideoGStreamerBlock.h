#pragma once

#include "_2realblock.h"

#include "_2RealGStreamerWrapper.h"

/*
class VideoGStreamerBlock

Defines the the VideoGStreamerBlock, which uses the _2RealGStreamerWrapper in order to open media
files and interact with them (play, pause, stop, play direction, playback speed). The Outlets return
each one buffer to the video and audio data respectively
*/
class VideoGStreamerBlock : public _2Real::bundle::Block
{
public:
	VideoGStreamerBlock();
	~VideoGStreamerBlock(void);

	// Virtual Methods
	virtual void					update();
	virtual void					setup( _2Real::bundle::BlockHandle &context );
	virtual void					shutdown();

private:
	/*
		Sets the playing state inside the wrapper
	*/
	void							setPlayState( int iPlayState );

	/*
		Sets the play direction
	*/
	void							setDirection( int iDirection );

	/*
		Sets the loop mode
	*/
	void							setLoopMode( int iLoopMode );

	/*
		Updates all outlets (width, height, number of frames and fps) each time a new file has been opened
	*/
	void							updateOutlets();

	/*
		In order to discard the outlet values
	*/
	void							discardAllOutlets();


	_2RealGStreamerWrapper::GStreamerWrapper*			m_2RealGStreamerPlayer; /* Instance to the _2RealGStreamerWrapper */

	////////////////////////////////////////////////////////////////////////// INLETS
	_2Real::bundle::InletHandle							m_FilePath; /* The path to the media file */
	_2Real::bundle::InletHandle							m_PlayState; /* The current play state */
	_2Real::bundle::InletHandle							m_Position; /* The position in the media file */
	_2Real::bundle::InletHandle							m_Direction; /* The current play direction */
	_2Real::bundle::InletHandle							m_Speed; /* The current playback speed */
	_2Real::bundle::InletHandle							m_Volume; /* The current volume */
	_2Real::bundle::InletHandle							m_LoopMode; /* The current loop mode */
	_2Real::bundle::InletHandle							m_ManualAudioBuffer; /* Decides whether the wrapper should generate an audio buffer or should use a native sound plugin to play the sound */

	////////////////////////////////////////////////////////////////////////// OUTLETS
	_2Real::bundle::OutletHandle						m_Video; /* The video frame data (if the media file contains video information) */
	_2Real::bundle::OutletHandle						m_Width; /* The video width */
	_2Real::bundle::OutletHandle						m_Height; /* The video height */
	_2Real::bundle::OutletHandle						m_NumberOfFrames; /* The number of frames in the video */
	_2Real::bundle::OutletHandle						m_FPS; /* The frames per second */
	_2Real::bundle::OutletHandle						m_Audio; /* The audio buffer data (if the media file contains audio information and the manual audio buffer generation is turned on) */
};

