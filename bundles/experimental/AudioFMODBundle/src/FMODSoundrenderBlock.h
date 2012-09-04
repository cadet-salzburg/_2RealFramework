#pragma once
#include "FMODContextManager.h"


class FMODSoundrenderBlock : public _2Real::bundle::Block
{
	typedef _2Real::bundle::InletHandle Inlet;
	typedef _2Real::bundle::OutletHandle Outlet;

	public:
		FMODSoundrenderBlock( _2Real::bundle::ContextBlock& context );
		~FMODSoundrenderBlock(void);

		void					shutdown();
		void					update();
		void					setup( _2Real::bundle::BlockHandle &context );

		

	private:
		void					discardOutlets();

		FMODContextManager*						m_FmodManager;
		Inlet									m_IsPlay;
		Inlet									m_FilePath;
		Outlet									m_HasLoadedAudio;

		FMOD::Sound*							m_Sound;
		FMOD::Channel*							m_Channel;
		FMOD_CREATESOUNDEXINFO					m_SoundExInfo;
public:
FMOD::Sound*							s;
FMOD::Channel							*c;
 static FMODSoundrenderBlock*	srb;

};

/*! \brief     If created sound with exinfo and this callback, sound buffer has to be handled/filled by
 *! this callack function
	\param     sound - Pointer to the sound - Cast FMOD_SOUND* to FMOD::Sound* inside the callback and use as normal
	\param     data - Pointer to raw PCM data that the user can either read or write to. 
	\param     datalen - Length of data in bytes
	\return    FMOD_RESULT F_CALLBACK - Returning FMOD_OK or any value defined in FMOD_RESULT
*/
FMOD_RESULT F_CALLBACK	pcmReadCallback( FMOD_SOUND *sound, void *data, unsigned int datalen );
