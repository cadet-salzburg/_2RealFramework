#pragma once

#include "_2RealDatatypes.h"

#include <list>
#include <vector>

class Trajectory
{
public:
	struct Entry
	{
		_2Real::Vec2 m_pos;
		_2Real::Vec2 m_size;

		bool	m_isExtrapolated;
		double	m_coherence;

		Entry( const _2Real::Vec2 &pos, const _2Real::Vec2 &size, double coherence, bool isExtrapolated ) :
			m_pos( pos ), 
			m_size( size ), 
			m_isExtrapolated( isExtrapolated ), 
			m_coherence( coherence )
		{}
	};

	typedef std::list<Entry>	EntryList;

private:
	double calcPosAffinity( const _2Real::Vec2 &p, double screenWidth, double time ) const;
	double calcSizeAffinity( const _2Real::Vec2 &size, double time ) const;

	double calcDirCoherence( const _2Real::Vec2 &p, double time, double dirTolerance = 0.0 ) const;
	double calcVelCoherence( const _2Real::Vec2 &p, double time, double velTolerance = 0.0 ) const;
	double calcSizeCoherence( const _2Real::Vec2 &s, double time, double sizeTolerance = 0.0 ) const;

	bool canCalcPosAffinity() const			{	return ( m_list.size() ? true : false );		}
	bool canCalcSizeAffinity() const		{	return ( m_list.size() ? true : false );		}

	bool canCalcDirCoherence() const		{	return ( m_list.size() > 1 );	}
	bool canCalcVelCoherence() const		{	return ( m_list.size() > 1 );	}
	bool canCalcSizeCoherence() const		{	return ( m_list.size() > 1 );	}

	size_t		m_maxSize;
	double		m_coherenceSum;

	_2Real::Vec2 m_v0;
	_2Real::Vec2 m_v1;

	_2Real::Vec2 m_s0;
	_2Real::Vec2 m_s1;

	double		m_prevTime;

	EntryList	m_list;
	
public:
	explicit Trajectory( size_t maxSize = 16 ) : 
		m_maxSize( maxSize ),
		m_coherenceSum( 0.0 ),
		m_v0( 0.0, 0.0 ), m_v1( 0.0, 0.0 ),
		m_s0( 0.0, 0.0 ), m_s1( 0.0, 0.0 ),
		m_prevTime( 0.0 )
	{}

	double calcAffinity( const _2Real::Space2D &region, double posWeight, double sizeWeight, double screenWidth, double time ) const;
	double calcCoherence( const _2Real::Space2D &region, double dirWeight, double velWeight, double sizeWeight, double time, double dirTolerance, double velTolerance, double sizeTolerance ) const;
	
	bool canCalcAffinity() const			{	return ( this->canCalcPosAffinity() && this->canCalcSizeAffinity() );									}
	bool canCalcCoherence() const			{	return ( this->canCalcDirCoherence() && this->canCalcVelCoherence() && this->canCalcSizeCoherence() );	}

	void add( const _2Real::Space2D &region, double coherence, double time, bool isExtrapolated = false );
	bool extrapolate( const _2Real::Vec2 &minSize, double time, double damping, double coherenceRise );

	void prune();

	unsigned int getEntryCount() const					{	return m_list.size();	}
	const EntryList &getEntries() const					{	return m_list;			}

	double getAvrgCoherence() const
	{
		return ( m_list.size() ? m_coherenceSum / m_list.size() : 0.0 );
	}

	_2Real::Space2D getLastRegion() const;
};

typedef std::vector< _2Real::Space2D >	FeatureVector;

class TrackingInfo
{
private:
	unsigned int	m_userID;

	Trajectory		m_faceTrajectory;

	Trajectory		m_eyeLeftTrajectory;
	Trajectory		m_eyeRightTrajectory;
	Trajectory		m_noseTrajectory;
	Trajectory		m_mouthTrajectory;

	_2Real::Vec2	m_minFaceSize;

public:
	explicit TrackingInfo( unsigned int userID, const _2Real::Vec2 &minFaceSize ) :
		m_userID( userID ),
		m_minFaceSize( minFaceSize )
	{}

	void add( const _2Real::Space2D &faceRegion, double coherence, double time )
	{
		m_faceTrajectory.add( faceRegion, coherence, time );
	}

	void addFromFeatureCandidates( const FeatureVector &eyesCandidates, const FeatureVector &noseCandidates, const FeatureVector &mouthCandidates, double time, double posAffWeight, double sizeAffWeight );
	void prune();
	bool extrapolate( double time, double damping, double coherenceRise );

	const Trajectory	&getFaceTrajectory() const		{	return m_faceTrajectory;		}

	const Trajectory	&getEyeLeftTrajectory() const	{	return m_eyeLeftTrajectory;		}
	const Trajectory	&getEyeRightTrajectory() const	{	return m_eyeRightTrajectory;	}
	const Trajectory	&getNoseTrajectory() const		{	return m_noseTrajectory;		}
	const Trajectory	&getMouthTrajectory() const		{	return m_mouthTrajectory;		}

	unsigned int getUserID() const			{	return m_userID;							}
	unsigned int getEntryCount() const		{	return m_faceTrajectory.getEntryCount();	}
};

typedef std::list<TrackingInfo>			TrackingInfoList;

class FaceTracking
{
public:
	explicit FaceTracking( unsigned int maxUsers )
	{
		for( int i = maxUsers - 1; i >= 0; i-- )
			m_availableUserIDs.push_back( i );
	}

	TrackingInfoList &trackingInfoList()					{	return m_trackingInfoList;		}
	const TrackingInfoList &trackingInfoList() const		{	return m_trackingInfoList;		}

	void track( FeatureVector &faces, double affWeightPos, double affWeightSize, double affThresh, 
				double cohWeightDir, double cohWeightVel, double cohWeightSize, double cohThresh, double tolDir, double tolVel, double tolSize, 
				double discardThresh, double damping, double cohRise, 
				const _2Real::Vec2 &minSizeFace,
				double time );

private:

	TrackingInfoList			m_trackingInfoList;

	std::vector<unsigned int>	m_availableUserIDs;
};