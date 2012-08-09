#pragma once

#include "Singleton.h"
#include "SoundPlayer.h"
#include <string>
#include <map>
#include "fmod.hpp"
#include "fmod_errors.h"

#define SoundMgr SoundManager::getInstance()


class SoundManager : public Singleton<SoundManager>
{
public:
	bool init();
	void update();
	void addSound(std::string strId, std::string strFileName,  bool bSound3d = false, bool bStream = false);
	std::shared_ptr<SoundPlayer> getSound(std::string strId);
	void shutdown();
	float* getSpectrum(int nBands);
	void stopAll();
	void setVolume(float vol);
	void setListener3d(ci::Vec3f pos, ci::Vec3f vel, ci::Vec3f dir, ci::Vec3f up);
	FMOD::System* getFmodSystemPtr();

private:
	int nextPow2(int a);

	FMOD::ChannelGroup*		m_pChannelGroup;
	FMOD::System*			m_pSystem;
	
	std::map<std::string, std::shared_ptr<SoundPlayer> > m_Sounds;
	float fftValues[8192];			
	float fftInterpValues[8192];			
	float fftSpectrum[8192];		// maximum # is 8192, in Fmodex....

	bool					m_bFmodInitialized;
};
