#pragma once
#include "_2RealBlock.h"
#include <fmod.hpp>
#include <string>

/*!
* _2Real Context - Bundle managing all In/Output to FMOD-Sound system
*/
class FMODContextManager : public _2Real::bundle::ContextBlock
{
	typedef std::exception			FMODContextException;

	public:
		FMODContextManager();
		~FMODContextManager();

		void							setup( _2Real::bundle::BlockHandle &context );
		void							update();
		void							shutdown();

		bool							bindDevice(const unsigned int deviceIdx, int w, int h, int fps=30);
		void							unbindDevice(const unsigned int deviceIdx);

		/*! \brief     Creates a sound with FMOD
			\param     file - path to file want to load (absolute path C:\... D:\ possible on windows systems 
			\param     mode - Use FMOD_SOFTWARE for standard
			\param     exInfo - default is nullptr, filled when creating file from ie. a buffer
			\return    FMOD::Sound* - Reference to the created sound
		*/
		FMOD::Sound*					CreateSound( std::string file, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO* exInfo );

		FMOD::Channel*					PlaySound( FMOD::Sound* sound );

		void							CreateSoundExInfo( FMOD_CREATESOUNDEXINFO* outInfo, const uint32_t sizeDecodeBuffer, const uint8_t numChannels, const int lengthPCM, const uint16_t defaultFrequenzy, const FMOD_SOUND_FORMAT format, FMOD_SOUND_PCMREADCALLBACK callback );
	//private:
	
					/*! \brief     Standard Fmod error checking, will throw a std::expection with fmod error-message
		\param     method - name of the function
		\param     result - result returned by fmod api-functions
	*/
		void							checkFMODError( const char* method, const FMOD_RESULT& result ) const;

		FMOD::System*					m_System;
		uint32_t						m_FmodVersion;
		int								m_NumDriver;
		FMOD_SPEAKERMODE				m_Speakermode;
		FMOD_CAPS						m_FmodCaps;
		std::string						m_Name;
};