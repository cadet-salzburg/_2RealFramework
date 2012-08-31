#pragma once

#include "_2RealDatatypes.h"

#include "FaceTracking.h"


namespace facetracking
{
	class CVImpl;
}

class FaceDetection
{
public:
	FaceDetection();
	~FaceDetection();

	void loadFaceCascade( const std::string &file );
	void loadEyesCascade( const std::string &file );
	void loadNoseCascade( const std::string &file );
	void loadMouthCascade( const std::string &file );

	bool equalizeHist() const;
	void equalizeHist( bool equalize );

	void minNeighboursFace( unsigned int count );
	void minNeighboursEyes( unsigned int count );
	void minNeighboursNose( unsigned int count );
	void minNeighboursMouth( unsigned int count );

	void minSizeFace( const _2Real::Vec2 &size );
	void minSizeEyes( const _2Real::Vec2 &size );
	void minSizeNose( const _2Real::Vec2 &size );
	void minSizeMouth( const _2Real::Vec2 &size );

	void doCannyPruning( bool prune );
	void haarScaleFactor( double factor );

	void greyImgSmlScale( double scale );

	bool createImages( const _2Real::Image &img );
	void releaseImages();
	void detectFaces( const _2Real::Image &img, FeatureVector &featureVec );
	void detectFeatures( const _2Real::Image &img, const _2Real::Space2D &faceRegion, FeatureVector &eyesCandidates, FeatureVector &noseCandidates, FeatureVector &mouthCandidates, bool useEyes, bool useNose, bool useMouth );

private:
	facetracking::CVImpl	*m_pImpl;
};
