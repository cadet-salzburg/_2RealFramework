#include "SoundManager.h"
#include "cinder/Utilities.h"

bool SoundManager::init()
{
	if(!m_bFmodInitialized)
	{
		FMOD::System_Create(&m_pSystem);
		#ifdef TARGET_LINUX
				sys->setOutput(FMOD_OUTPUTTYPE_ALSA);
		#endif
		m_pSystem->init(32, FMOD_INIT_NORMAL, NULL);  //do we want just 32 channels?
		m_pSystem->getMasterChannelGroup(&m_pChannelGroup);
		m_pSystem->set3DSettings(10.0f, 10.0f, 10.0f); 
		m_bFmodInitialized = true;
		return true;
	}
	return false;
}

FMOD::System* SoundManager::getFmodSystemPtr()
{
	return m_pSystem;
}

void SoundManager::addSound(std::string strId, std::string strFileName,  bool bSound3d, bool bStream)
{
	m_Sounds[strId] = std::shared_ptr<SoundPlayer>(new SoundPlayer(m_pSystem));
	m_Sounds[strId]->loadSound(strFileName, bSound3d, bStream);
}	

std::shared_ptr<SoundPlayer> SoundManager::getSound(std::string strId)
{
	return m_Sounds[strId];
}	

void SoundManager::update()
{
	if(m_bFmodInitialized)
		m_pSystem->update();
}

void SoundManager::shutdown()
{
	m_Sounds.clear();
	if(m_bFmodInitialized)
	{
		m_pSystem->close();
		m_bFmodInitialized = false;
	}
}

void SoundManager::stopAll()
{
	m_pChannelGroup->stop();
}

void SoundManager::setVolume(float vol)
{
	m_pChannelGroup->setVolume(vol);
}

void SoundManager::setListener3d(ci::Vec3f pos, ci::Vec3f vel, ci::Vec3f forward, ci::Vec3f up)
{
	FMOD_VECTOR vPosition = { pos.x, pos.y, pos.z }; 
	FMOD_VECTOR vVelocity = { vel.x, vel.y, vel.z }; 
	FMOD_VECTOR vForward = { forward.x, forward.y, forward.z }; 
	FMOD_VECTOR vUp = { up.x, up.y, up.z }; 

	m_pSystem->set3DListenerAttributes( 0, &vPosition, &vVelocity, &vForward, &vUp);
}

float* SoundManager::getSpectrum(int nBands)
{
	// 	set to 0
	for (int i = 0; i < 8192; i++){
		fftInterpValues[i] = 0;
	}
	
	// 	check what the user wants vs. what we can do:
	if (nBands > 8192){
		std::cout << "error in FmodexGetSpectrum, the maximum number of bands is 8192 - you asked for" + ci::toString(nBands) + "we will return 8192" << std::endl;
		nBands = 8192;
	} else if (nBands <= 0){
		std::cout << "error in FmodexSpectrum, you've asked for" + ci::toString(nBands) + "bands, minimum is 1" << std::endl;
		return fftInterpValues;
	}
	
	// 	FMOD needs pow2
	int nBandsToGet = nextPow2(nBands);
	if (nBandsToGet < 64) nBandsToGet = 64;  // can't seem to get fft of 32, etc from Fmodex
	
	// 	get the fft
	m_pSystem->getSpectrum(fftSpectrum, nBandsToGet, 0, FMOD_DSP_FFT_WINDOW_HANNING);
	
	// 	convert to db scale
	for(int i = 0; i < nBandsToGet; i++){
        fftValues[i] = 10.0f * (float)log10(1 + fftSpectrum[i]) * 2.0f;
	}
	
	// 	try to put all of the values (nBandsToGet) into (nBands)
	//  in a way which is accurate and preserves the data:
	//
	
	if (nBandsToGet == nBands){
		
		for(int i = 0; i < nBandsToGet; i++){
			fftInterpValues[i] = fftValues[i];
		}
		
	} else {
		
		float step 		= (float)nBandsToGet / (float)nBands;
		//float pos 		= 0;
		// so for example, if nBands = 33, nBandsToGet = 64, step = 1.93f;
		int currentBand = 0;
		
		for(int i = 0; i < nBandsToGet; i++){
			
			// if I am current band = 0, I care about (0+1) * step, my end pos
			// if i > endPos, then split i with me and my neighbor
			
			if (i >= ((currentBand+1)*step)){
				
				// do some fractional thing here...
				float fraction = ((currentBand+1)*step) - (i-1);
				float one_m_fraction = 1 - fraction;
				fftInterpValues[currentBand] += fraction * fftValues[i];
				currentBand++;
				// safety check:
				if (currentBand >= nBands){
					std::cout << "FmodexGetSpectrum - currentBand >= nBands" << std::endl;
				}
				
				fftInterpValues[currentBand] += one_m_fraction * fftValues[i];
				
			} else {
				// do normal things
				fftInterpValues[currentBand] += fftValues[i];
			}
		}
		
		// because we added "step" amount per band, divide to get the mean:
		for (int i = 0; i < nBands; i++){
			fftInterpValues[i] /= step;
			if (fftInterpValues[i] > 1)fftInterpValues[i] = 1; 	// this seems "wrong"
		}
		
	}
	
	return fftInterpValues;
}


int SoundManager::nextPow2(int a)
{
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}
