#include "SoundPlayer.h"
#include "SoundManager.h"

using namespace std;
using namespace cinder;


SoundPlayer::SoundPlayer(FMOD::System* pSystem)
{
	m_bLoop 			= false;
	m_bLoadedOk 		= false;
	m_fPan 				= 0.5f;
	m_fVolume 			= 1.0f;
	m_fInternalFreq 	= 44100;
	m_fSpeed 			= 1.0;
	m_bPaused			= false;
	m_bIsStreaming		= false;
	m_bSound3d			= false;
	m_pSystem			= pSystem;
}

SoundPlayer::~SoundPlayer()
{
	unloadSound();
}

void SoundPlayer::setParams(BaseParams params)
{
	m_bLoop 			= params.m_iLoopMode;
	m_fVolume 			= params.m_fVolume;
	m_fSpeed 			= params.m_fSpeed;
	m_bIsStreaming		= params.m_bStream;
	m_bSound3d			= params.m_bSound3d;
}

void SoundPlayer::loadSound(std::string fileName, bool bSound3d, bool stream)
{
	m_bMultiPlay = false;
	unloadSound();
	
	int fmodFlags;
	if(bSound3d)
		fmodFlags =  FMOD_3D;
	else
		fmodFlags =  FMOD_SOFTWARE;

	if(stream)
		fmodFlags =  fmodFlags | FMOD_CREATESTREAM;
	
	m_Result = m_pSystem->createSound(fileName.c_str(),  fmodFlags, NULL, &m_pSound);
	
	if (m_Result != FMOD_OK){
		m_bLoadedOk = false;
		std::cout << "SoundPlayer: Could not load sound file " + fileName << std::endl;
	} else {
		m_bLoadedOk = true;
		m_pSound->getLength(&m_iLength, FMOD_TIMEUNIT_PCM);
		m_bIsStreaming = stream;
	}
	
}

//------------------------------------------------------------
void SoundPlayer::unloadSound()
{
	if (m_bLoadedOk)
	{
		stop();						// try to stop the sound
		if(!m_bIsStreaming) 
			m_pSound->release();
	}
}

//------------------------------------------------------------
bool SoundPlayer::getIsPlaying()
{
	
	if (!m_bLoadedOk) return false;
	
	bool playing;
	m_pChannel->isPlaying(&playing);
	return playing;
}

//------------------------------------------------------------
float SoundPlayer::getSpeed()
{
	return m_fSpeed;
}

//------------------------------------------------------------
float SoundPlayer::getPan(){
	return m_fPan;
}

//------------------------------------------------------------
void SoundPlayer::setVolume(float vol)
{
	if (getIsPlaying() == true)
	{
		m_pChannel->setVolume(vol);
	}
	m_fVolume = vol;
}

//------------------------------------------------------------
void SoundPlayer::setPosition(float pct)
{
	if (getIsPlaying() == true)
	{
		int sampleToBeAt = (int)(m_iLength * pct);
		m_pChannel->setPosition(sampleToBeAt, FMOD_TIMEUNIT_PCM);
	}
}

//------------------------------------------------------------
float SoundPlayer::getPosition(){
	if (getIsPlaying() == true){
		unsigned int sampleImAt;
		
		m_pChannel->getPosition(&sampleImAt, FMOD_TIMEUNIT_PCM);
		
		float pct = 0.0f;
		if (m_iLength > 0){
			pct = sampleImAt / (float)m_iLength;
		}
		return pct;
	} else {
		return 0;
	}
}

//------------------------------------------------------------
void SoundPlayer::setPan(float p)
{
	if (getIsPlaying() == true)
	{
		m_pChannel->setPan(p);
	}
	m_fPan = p;
}


//------------------------------------------------------------
void SoundPlayer::setPaused(bool bP){
	if (getIsPlaying() == true)
	{
		m_pChannel->setPaused(bP);
		m_bPaused = bP;
	}
}


//------------------------------------------------------------
void SoundPlayer::setSpeed(float spd)
{
	if (getIsPlaying() == true)
	{
		m_pChannel->setFrequency( m_fInternalFreq * spd);
	}
	m_fSpeed = spd;
}


//------------------------------------------------------------
void SoundPlayer::setLoop(bool bLp)
{
	if (getIsPlaying() == true)
	{
		m_pChannel->setMode( (bLp == true) ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	}
	m_bLoop = bLp;
}

// ----------------------------------------------------------------------------
void SoundPlayer::setMultiPlay(bool bMp)
{
	m_bMultiPlay = bMp;		// be careful with this...
}

// ----------------------------------------------------------------------------
void SoundPlayer::play()
{
	
	// if it's a looping sound, we should try to kill it, no?
	// or else people will have orphan channels that are looping
	if (m_bLoop == true)
	{
		m_pChannel->stop();
	}
	
	// if the sound is not set to multiplay, then stop the current,
	// before we start another
	if (!m_bMultiPlay){
		m_pChannel->stop();
	}
	
	m_pSystem->playSound(FMOD_CHANNEL_FREE, m_pSound, m_bPaused, &m_pChannel);
	
	m_pChannel->getFrequency(&m_fInternalFreq);
	m_pChannel->setVolume(m_fVolume);
	m_pChannel->setPan(m_fPan);
	m_pChannel->setFrequency(m_fInternalFreq * m_fSpeed);
	m_pChannel->setMode((m_bLoop == true) ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	m_pSystem->update();
}

// ----------------------------------------------------------------------------
void SoundPlayer::stop()
{
	m_pChannel->stop();
}

void SoundPlayer::setMinMaxDistance3d(float min, float max)
{
	m_pChannel->set3DMinMaxDistance(min, max);
}

void SoundPlayer::setPosition3d(ci::Vec3f pos)
{
	static ci::Vec3f oldPos;
	m_vPos = pos;
	m_vVel = oldPos - pos;
	m_vVel*=-10.0;
	FMOD_VECTOR position = { m_vPos.x, m_vPos.y, m_vPos.z }; 
	FMOD_VECTOR velocity = { m_vVel.x, m_vVel.y, m_vVel.z }; 
	m_pChannel->set3DAttributes(&position, &velocity);  
	oldPos = pos;
}

ci::Vec3f SoundPlayer::getPosition3d()
{
	return m_vPos;
}

void SoundPlayer::setVelocity3d(ci::Vec3f vel)
{
	m_vVel = vel;
	FMOD_VECTOR position = { m_vPos.x, m_vPos.y, m_vPos.z }; 
	FMOD_VECTOR velolicty = { m_vVel.x, m_vVel.y, m_vVel.z }; 
	m_pChannel->set3DAttributes(&position, &velolicty); 
}

ci::Vec3f SoundPlayer::getVelocity3d()
{
	return m_vVel;
}
