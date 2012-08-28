#pragma once

#include "_2RealBlock.h"

namespace faceTracking
{
namespace impl
{
	class CVImpl;
	class TimeImpl;
	class FaceTrackingImpl;
}
}

class FaceCastBlock : public _2Real::bundle::Block
{

public:

	FaceCastBlock();
	~FaceCastBlock();
	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	typedef	std::vector<_2Real::Vec3>	VertexList;
	typedef std::vector<_2Real::Vec3>	NormalList;
	typedef std::vector<int>			IndexList;
	typedef std::vector<unsigned short>	DepthList;

	unsigned int	m_frames;
	double			m_timeAccu;
	double			m_timeOverall;

	unsigned int	m_resX;
	unsigned int	m_resY;

	double			m_fov;

	unsigned int	m_avrgResX;
	unsigned int	m_avrgResY;

	//VertexList		m_vertices;
	//NormalList		m_normals;
	//IndexList		m_indices;
	DepthList		m_avrgArray;	// downsampled depth values

	_2Real::Vec2	m_extrapolationMinSizeFace;

	_2Real::bundle::BlockHandle		m_Block;

	_2Real::bundle::InletHandle		m_imageIn;
	_2Real::bundle::InletHandle		m_depthIn;

	_2Real::bundle::InletHandle		m_resXIn;
	_2Real::bundle::InletHandle		m_resYIn;

	_2Real::bundle::InletHandle		m_fovVerIn;

	_2Real::bundle::InletHandle		m_haarDetectionDownscaleIn;

	_2Real::bundle::InletHandle		m_haarMinNeighboursFaceIn;

	_2Real::bundle::InletHandle		m_haarMinSizeFaceIn;

	_2Real::bundle::InletHandle		m_haarDoCannyPruningIn;
	_2Real::bundle::InletHandle		m_haarScaleFactorIn;

	_2Real::bundle::InletHandle		m_equalizeHistogramIn;

	_2Real::bundle::InletHandle		m_cascFileFaceIn;

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

	bool makeDepthCast( const _2Real::Image &depthImg, const _2Real::Space2D &area, _2Real::FaceCast &cast );
	void resizeCast();
};