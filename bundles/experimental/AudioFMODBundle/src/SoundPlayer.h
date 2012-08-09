#pragma once

#include "fmod.hpp"
#include "fmod_errors.h"
#include <string>
#include "cinder/Vector.h"
#include "BaseParams.h"

class SoundPlayer
{
	
public:
	
	SoundPlayer(FMOD::System* pSystem);
	virtual ~SoundPlayer();
	
	void loadSound(std::string fileName, bool bSound3d = false, bool bStream = false);
	void unloadSound();
	void play();
	void stop();
	void setParams(BaseParams params);

	void setVolume(float vol);
	void setPan(float vol);
	void setSpeed(float spd);
	void setPaused(bool bP);
	void setLoop(bool bLp);
	void setMultiPlay(bool bMp);
	void setPosition(float pct); // 0 = start, 1 = end;
	
	float getPosition();
	bool getIsPlaying();
	float getSpeed();
	float getPan();

	void		setMinMaxDistance3d(float min, float max);
	void		setPosition3d(ci::Vec3f pos);
	ci::Vec3f	getPosition3d();
	void		setVelocity3d(ci::Vec3f vel);
	ci::Vec3f	getVelocity3d();
	
private:
	bool			m_bIsStreaming;
	bool			m_bMultiPlay;
	bool			m_bLoop;
	bool			m_bLoadedOk;
	bool			m_bPaused;
	bool			m_bSound3d;
	float			m_fPan; // 0 - 1
	float			m_fVolume; // 0 - 1
	float			m_fInternalFreq; // 44100 ?
	float			m_fSpeed; // -n to n, 1 = normal, -1 backwards
	unsigned int	m_iLength; // in samples;
	ci::Vec3f		m_vPos;
	ci::Vec3f		m_vVel;

	FMOD_RESULT		m_Result;
	FMOD::Channel*	m_pChannel;
	FMOD::Sound*	m_pSound;
	FMOD::System*	m_pSystem;
};

