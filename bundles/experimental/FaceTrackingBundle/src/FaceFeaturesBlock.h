#pragma once

#include "_2RealBlock.h"
//#include "_2RealDatatypes.h"

class CVImpl;

class FaceFeaturesBlock : public _2Real::bundle::Block
{

public:

	FaceFeaturesBlock();
	~FaceFeaturesBlock();
	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	unsigned int				m_numChannels;
	_2Real::ImageType			m_imageType;
	_2Real::ImageChannelOrder	m_channelOrder;

	_2Real::bundle::BlockHandle		m_Block;

	_2Real::bundle::InletHandle		m_imageIn;

	_2Real::bundle::InletHandle		m_useFaceIn;
	_2Real::bundle::InletHandle		m_useEyesIn;
	_2Real::bundle::InletHandle		m_useNoseIn;
	_2Real::bundle::InletHandle		m_useMouthIn;

	_2Real::bundle::InletHandle		m_cascFileFaceIn;
	_2Real::bundle::InletHandle		m_cascFileEyesIn;
	_2Real::bundle::InletHandle		m_cascFileNoseIn;
	_2Real::bundle::InletHandle		m_cascFileMouthIn;

	_2Real::bundle::OutletHandle	m_facesOut;

	CVImpl	*m_cvImpl;
};