#pragma once

#include "_2RealBlock.h"
//#include "_2RealDatatypes.h"

namespace faceTracking
{
namespace impl
{
	class CVImpl;
	class TimeImpl;
	class FaceTrackingImpl;
}
}

class FaceFeaturesBlock : public _2Real::bundle::Block
{

public:

	FaceFeaturesBlock();
	~FaceFeaturesBlock();
	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	unsigned int	m_frames;
	double			m_timeAccu;
	double			m_timeOverall;

	_2Real::Vec2	m_extrapolationMinSizeFace;
	_2Real::Vec2	m_extrapolationMinSizeEye;
	_2Real::Vec2	m_extrapolationMinSizeNose;
	_2Real::Vec2	m_extrapolationMinSizeMouth;

	_2Real::bundle::BlockHandle		m_Block;

	_2Real::bundle::InletHandle		m_imageIn;

	_2Real::bundle::InletHandle		m_haarDetectionDownscaleIn;

	_2Real::bundle::InletHandle		m_useEyesIn;
	_2Real::bundle::InletHandle		m_useNoseIn;
	_2Real::bundle::InletHandle		m_useMouthIn;

	_2Real::bundle::InletHandle		m_haarMinNeighboursFaceIn;
	_2Real::bundle::InletHandle		m_haarMinNeighboursEyesIn;
	_2Real::bundle::InletHandle		m_haarMinNeighboursNoseIn;
	_2Real::bundle::InletHandle		m_haarMinNeighboursMouthIn;

	_2Real::bundle::InletHandle		m_haarMinSizeFaceIn;
	_2Real::bundle::InletHandle		m_haarMinSizeEyesIn;
	_2Real::bundle::InletHandle		m_haarMinSizeNoseIn;
	_2Real::bundle::InletHandle		m_haarMinSizeMouthIn;

	_2Real::bundle::InletHandle		m_haarDoCannyPruningIn;
	_2Real::bundle::InletHandle		m_haarScaleFactorIn;

	_2Real::bundle::InletHandle		m_equalizeHistogramIn;

	_2Real::bundle::InletHandle		m_cascFileFaceIn;
	_2Real::bundle::InletHandle		m_cascFileEyesIn;
	_2Real::bundle::InletHandle		m_cascFileNoseIn;
	_2Real::bundle::InletHandle		m_cascFileMouthIn;

	_2Real::bundle::InletHandle		m_extrapolationDampingIn;
	_2Real::bundle::InletHandle		m_extrapolationCoherenceRiseIn;

	_2Real::bundle::InletHandle		m_affinityWeightPosIn;
	_2Real::bundle::InletHandle		m_affinityWeightSizeIn;
	_2Real::bundle::InletHandle		m_coherenceWeightDirIn;
	_2Real::bundle::InletHandle		m_coherenceWeightVelIn;
	_2Real::bundle::InletHandle		m_coherenceWeightSizeIn;
	_2Real::bundle::InletHandle		m_coherenceToleranceDirIn;
	_2Real::bundle::InletHandle		m_coherenceToleranceVelIn;
	_2Real::bundle::InletHandle		m_coherenceToleranceSizeIn;

	_2Real::bundle::InletHandle		m_affinityThresholdIn;
	_2Real::bundle::InletHandle		m_coherenceThresholdIn;
	_2Real::bundle::InletHandle		m_discardThresholdIn;

	_2Real::bundle::OutletHandle	m_faceOut;

	faceTracking::impl::CVImpl				*m_cvImpl;
	faceTracking::impl::FaceTrackingImpl	*m_faceTrackingImpl;

	faceTracking::impl::TimeImpl			*m_timeImpl;
};