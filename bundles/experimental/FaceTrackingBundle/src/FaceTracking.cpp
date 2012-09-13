#include "FaceTracking.h"

#include <set>
#include <map>

double Trajectory::calcPosAffinity( const _2Real::Vec2 &p, double screenWidth, double time ) const
{
	if( !this->canCalcPosAffinity() )
		throw _2Real::Exception( "cannot calc position affinity -- not enough values" );

	double ret = ( p - m_v1 ).norm();
	double dt = time - m_prevTime;

	return ret / ( screenWidth * dt * 30.0 );
}

double Trajectory::calcSizeAffinity( const _2Real::Vec2 &size, double time ) const
{
	if( !this->canCalcSizeAffinity() )
		throw _2Real::Exception( "cannot calc size affinity -- not enough values" );

	double area0 = m_s1[0] * m_s1[1];	//calc area of previous size
	double area1 = size[0] * size[1];			//calc area of this size

	double dt = time - m_prevTime;

	double ratio = 0.0;
	if( std::abs( area0 * area1 ) > std::numeric_limits<double>::epsilon() )	//if one of both is 0, ratio should be zero (actually the return value should be even higher than 1?)
	{
		ratio = sqrt( area0 / area1 );
		if( ratio > 1.0 )
			ratio = 1.0 / ratio;
	}

	return ( 1.0 - ratio ) / ( dt * 30.0 );
}

double Trajectory::calcDirCoherence( const _2Real::Vec2 &p, double time, double dirTolerance ) const
{
	if( !this->canCalcDirCoherence() )
		throw _2Real::Exception( "cannot calc directional coherence -- not enough values" );

	_2Real::Vec2 d0( m_v1 - m_v0 );
	_2Real::Vec2 d1( p - m_v1 );

	double len0 = d0.norm();
	double len1 = d1.norm();

	double dt = time - m_prevTime;

	double compDir = 0.0;
	if( len0 * len1 > std::numeric_limits<double>::epsilon() )
	{
		if( len0 > dirTolerance && len1 > dirTolerance )
			compDir = ( 1.0 - ( d0.dot( d1 ) / ( len0 * len1 ) ) ) * 0.5f;
		else
			compDir = 0.5f;		//cannot make a decision -> tie value.
	}

	return compDir / ( dt * 30.0 );
}

double Trajectory::calcVelCoherence( const _2Real::Vec2 &p, double time, double velTolerance ) const
{
	if( !this->canCalcVelCoherence() )
		throw _2Real::Exception( "cannot calc velocity coherence -- not enough values" );

	_2Real::Vec2 d0( m_v1 - m_v0 );
	_2Real::Vec2 d1( p - m_v1 );

	double len0 = std::max<double>( d0.norm(), velTolerance );
	double len1 = std::max<double>( d1.norm(), velTolerance );

	double dt = time - m_prevTime;

	double compVel = 0.0;
	compVel = 1.0 - 2.0 * std::sqrt( len0 * len1 ) / ( len0 + len1 );

	return compVel / ( dt * 30.0 );
}

double Trajectory::calcSizeCoherence( const _2Real::Vec2 &s, double time, double sizeTolerance ) const
{
	if( !this->canCalcSizeCoherence() )
		throw _2Real::Exception( "cannot calc size coherence -- not enough values" );

	double a0 = sqrt( m_s0[0] * m_s0[1] );
	double a1 = sqrt( m_s1[0] * m_s1[1] );
	double a2 = sqrt( s[0] * s[1] );

	double d0 = std::max( std::abs( a1 - a0 ), sizeTolerance );
	double d1 = std::max( std::abs( a2 - a1 ), sizeTolerance );

	double dt = time - m_prevTime;

	double compSize = 0.0;
	compSize = 1.0 - 2.0 * std::sqrt( d0 * d1 ) / ( d0 + d1 );

	return compSize / ( dt * 30.0 );
}
	
//TODO: the calculated measures are not quite correct... have to take image aspect reatio into account as well (since
// passed coordinates/regions are normalized to image bounds and measures/tolerances are non-uniform as a result)
double Trajectory::calcAffinity( const _2Real::Space2D &region, double posWeight, double sizeWeight, double screenWidth, double time ) const
{
	if( !this->canCalcAffinity() )
		throw _2Real::Exception( "cannot calc affinity -- not enough values" );

	_2Real::Vec2 center( ( region.getP1() + region.getP0() ) * 0.5f );
	_2Real::Vec2 size( region.getP1() - region.getP0() );

	return 
		( this->calcPosAffinity( center, screenWidth, time ) * posWeight ) + 
		( this->calcSizeAffinity( size, time ) * sizeWeight );
}

//TODO: the calculated measures are not quite correct... have to take image aspect reatio into account as well (since
// passed coordinates/regions are normalized to image bounds and measures/tolerances are non-uniform as a result)
double Trajectory::calcCoherence( const _2Real::Space2D &region, double dirWeight, double velWeight, double sizeWeight, double time, double dirTolerance, double velTolerance, double sizeTolerance ) const
{
	if( !this->canCalcCoherence() )
		throw _2Real::Exception( "cannot calc coherence -- not enough values" );

	_2Real::Vec2 center( ( region.getP1() + region.getP0() ) * 0.5f );
	_2Real::Vec2 size( region.getP1() - region.getP0() );

	return 
		( this->calcDirCoherence( center, time, dirTolerance ) * dirWeight ) + 
		( this->calcVelCoherence( center, time, velTolerance ) * velWeight ) + 
		( this->calcSizeCoherence( size, time, sizeTolerance ) * sizeWeight );
}
	
void Trajectory::add( const _2Real::Space2D &region, double coherence, double time, bool isExtrapolated )
{
	m_v0 = m_v1;
	m_v1 = ( region.getP1() + region.getP0() ) * 0.5f;

	m_s0 = m_s1;
	m_s1 = region.getP1() - region.getP0();

	m_prevTime = time;

	while( m_list.size() + 1 >= m_maxSize )
		this->prune();

	m_list.push_back( Entry( m_v1, m_s1, coherence, isExtrapolated ) );
	m_coherenceSum += coherence;
}

bool Trajectory::extrapolate( const _2Real::Vec2 &minSize, double time, double damping, double coherenceRise )
{
	size_t s = m_list.size();
	if( s < 2 )
		return false;

	_2Real::Vec2 pos( m_v1 + ( m_v1 - m_v0 ) * (_2Real::Vec2::Scalar)damping );
	_2Real::Vec2 size( m_s1 + ( m_s1 - m_s0 ) * (_2Real::Vec2::Scalar)damping );

	double dt = time - m_prevTime;

	size[0] = std::max( minSize[0], size[0] );
	size[1] = std::max( minSize[1], size[1] );

	double coherence = m_list.back().m_coherence * ( 1.0 + coherenceRise / ( dt * 30.0 ) );

	this->add( _2Real::Space2D( pos - size * 0.5, pos + size * 0.5 ), coherence, time, true );

	return true;
}

void Trajectory::prune()
{
	if( m_list.size() )
	{
		m_coherenceSum -= m_list.front().m_coherence;
		m_list.pop_front();
	}
}

_2Real::Space2D Trajectory::getLastRegion() const
{
	if( !m_list.size() )
		throw std::runtime_error( "cannot get last region -- trajectory is empty!" );

	const Entry &e = m_list.back();
	return _2Real::Space2D( e.m_pos - e.m_size * 0.5f, e.m_pos + e.m_size * 0.5f );
}



void TrackingInfo::addFromFeatureCandidates( const FeatureVector &eyesCandidates, const FeatureVector &noseCandidates, const FeatureVector &mouthCandidates, double time, double posAffWeight, double sizeAffWeight )
{
	typedef std::multimap<double, const _2Real::Space2D*> PriorityMap;

	if( eyesCandidates.size() )
	{
		PriorityMap mapLeft;
		PriorityMap mapRight;

		for( FeatureVector::const_iterator it = eyesCandidates.begin(); it != eyesCandidates.end(); ++it )
		{
			_2Real::Vec2 center( ( it->getP0() + it->getP1() ) * 0.5f );
			if( center[0] > 0.5f )
			{
				if( m_eyeLeftTrajectory.canCalcAffinity() )
					mapLeft.insert( std::make_pair( m_eyeLeftTrajectory.calcAffinity( *it, posAffWeight, sizeAffWeight, 1.0, time ), &( *it ) ) );
				else
					mapLeft.insert( std::make_pair( 0.0, &( *it ) ) );
			}
			else
			{
				if( m_eyeRightTrajectory.canCalcAffinity() )
					mapRight.insert( std::make_pair( m_eyeRightTrajectory.calcAffinity( *it, posAffWeight, sizeAffWeight, 1.0, time ), &( *it ) ) );
				else
					mapRight.insert( std::make_pair( 0.0, &( *it ) ) );
			}
		}

		if( mapLeft.size() )
			m_eyeLeftTrajectory.add( *( mapLeft.begin()->second ), 0.0, time );
		else
		{
			//NOTE: there is no measure (yet?) to decide whether or not it would make sense to still extrapolate face feature, so this is not done at all,
			// instead trajectories are pruned so entries don't last forever. (actually *duplicating* last entry would make more sense in context of
			// face feature regions)
			m_eyeLeftTrajectory.prune();
		}

		if( mapRight.size() )
			m_eyeRightTrajectory.add( *( mapRight.begin()->second ), 0.0, time );
		else
		{
			//NOTE: there is no measure (yet?) to decide whether or not it would make sense to still extrapolate face feature, so this is not done at all,
			// instead trajectories are pruned so entries don't last forever. (actually *duplicating* last entry would make more sense in context of
			// face feature regions)
			m_eyeRightTrajectory.prune();
		}
	}
	else
	{
		//NOTE: there is no measure (yet?) to decide whether or not it would make sense to still extrapolate face feature, so this is not done at all,
		// instead trajectories are pruned so entries don't last forever. (actually *duplicating* last entry would make more sense in context of
		// face feature regions)
		m_eyeLeftTrajectory.prune();
		m_eyeRightTrajectory.prune();
	}

	if( noseCandidates.size() )
	{
		double minAffinity = 0.0;
		const _2Real::Space2D *bestCandidate = NULL;

		if( !m_noseTrajectory.canCalcAffinity() )
			bestCandidate = &( noseCandidates.front() ); //not enough regions in trajectory history in order to judge, just take first in candidate list
		else
		{
			for( FeatureVector::const_iterator it = noseCandidates.begin(); it != noseCandidates.end(); ++it )
			{
				if( !bestCandidate )
				{
					bestCandidate = &( *it );
					minAffinity = m_noseTrajectory.calcAffinity( *it, posAffWeight, sizeAffWeight, 1.0, time );
				}
				else
				{
					double affinity = m_noseTrajectory.calcAffinity( *it, posAffWeight, sizeAffWeight, 1.0, time );
					if( affinity < minAffinity )
					{
						bestCandidate = &( *it );
						minAffinity = affinity;
					}
				}
			}
		}

		m_noseTrajectory.add( *bestCandidate, 0.0, time );
	}
	else
	{
		//NOTE: there is no measure (yet?) to decide whether or not it would make sense to still extrapolate face feature, so this is not done at all,
		// instead trajectories are pruned so entries don't last forever. (actually *duplicating* last entry would make more sense in context of
		// face feature regions)
		m_noseTrajectory.prune();
	}

	if( mouthCandidates.size() )
	{
		double minAffinity = 0.0;
		const _2Real::Space2D *bestCandidate = NULL;

		if( !m_mouthTrajectory.canCalcAffinity() )
			bestCandidate = &( mouthCandidates.front() ); //not enough regions in trajectory history in order to judge, just take first in candidate list
		else
		{
			for( FeatureVector::const_iterator it = mouthCandidates.begin(); it != mouthCandidates.end(); ++it )
			{
				if( !bestCandidate )
				{
					bestCandidate = &( *it );
					minAffinity = m_mouthTrajectory.calcAffinity( *it, posAffWeight, sizeAffWeight, 1.0, time );
				}
				else
				{
					double affinity = m_mouthTrajectory.calcAffinity( *it, posAffWeight, sizeAffWeight, 1.0, time );
					if( affinity < minAffinity )
					{
						bestCandidate = &( *it );
						minAffinity = affinity;
					}
				}
			}
		}

		m_mouthTrajectory.add( *bestCandidate, 0.0, time );
	}
	else
	{
		//NOTE: there is no measure (yet?) to decide whether or not it would make sense to still extrapolate face feature, so this is not done at all,
		// instead trajectories are pruned so entries don't last forever. (actually *duplicating* last entry would make more sense in context of
		// face feature regions)
		m_mouthTrajectory.prune();
	}
}

void TrackingInfo::prune()
{
	m_faceTrajectory.prune();

	m_eyeLeftTrajectory.prune();
	m_eyeRightTrajectory.prune();
	m_noseTrajectory.prune();
	m_mouthTrajectory.prune();
}

bool TrackingInfo::extrapolate( double time, double damping, double coherenceRise )
{
	//NOTE: there is no measure (yet?) to decide whether or not it would make sense to still extrapolate face feature, so this is not done at all,
	// instead trajectories are pruned so entries don't last forever. (actually *duplicating* last entry would make more sense in context of
	// face feature regions)
	m_eyeLeftTrajectory.prune();
	m_eyeRightTrajectory.prune();
	m_noseTrajectory.prune();
	m_mouthTrajectory.prune();

	return m_faceTrajectory.extrapolate( m_minFaceSize, time, damping, coherenceRise );
}


void FaceTracking::track( FeatureVector &faces, double affWeightPos, double affWeightSize, double affThresh, 
			double cohWeightDir, double cohWeightVel, double cohWeightSize, double cohThresh, double tolDir, double tolVel, double tolSize, 
			double discardThresh, double damping, double cohRise, 
			const _2Real::Vec2 &minSizeFace,
			double time )
{
	std::set<_2Real::Space2D*> remainingDetections;
	for( FeatureVector::iterator itF = faces.begin(); itF != faces.end(); ++itF )
		remainingDetections.insert( &( *itF ) );

	if( m_trackingInfoList.size() )
	{
		typedef std::multimap<double, std::pair<TrackingInfo*, _2Real::Space2D*> > PriorityMap;	//ordered list of tracking results candidates, ordered by their coherence value

		PriorityMap priorityMapAffinity;
		PriorityMap priorityMapCoherence;

		//STEP 1: create map of trajectory/detectiondata pairs
		for( FeatureVector::iterator itF = faces.begin(); itF != faces.end(); ++itF )
		{
			for( TrackingInfoList::iterator itT = m_trackingInfoList.begin(); itT != m_trackingInfoList.end(); ++itT )
			{
				if( itT->getFaceTrajectory().canCalcAffinity() )
					priorityMapAffinity.insert( std::make_pair( itT->getFaceTrajectory().calcAffinity( *itF, affWeightPos, affWeightSize, 1.0f, time ),
							std::make_pair( &( *itT ), &( *itF ) ) ) );

				if( itT->getFaceTrajectory().canCalcCoherence() )
					priorityMapCoherence.insert( std::make_pair( itT->getFaceTrajectory().calcCoherence( *itF, cohWeightDir, cohWeightVel, cohWeightSize, time, tolDir, tolVel, tolSize ),
							std::make_pair( &( *itT ), &( *itF ) ) ) );
			}
		}

		std::set<TrackingInfo*> remainingTrajectories;
		for( TrackingInfoList::iterator itT = m_trackingInfoList.begin(); itT != m_trackingInfoList.end(); ++itT )
			remainingTrajectories.insert( &( *itT ) );

		//STEP 2: get best face for each trajectory by coherence value (thresholded)
		for( PriorityMap::const_iterator it = priorityMapCoherence.begin(); it != priorityMapCoherence.end(); ++it )
		{
			if( it->first > cohThresh )
				break;		//skip correlations with too high coherence value

			std::set<TrackingInfo*>::const_iterator itT = remainingTrajectories.find( it->second.first );
			if( itT == remainingTrajectories.end() )
				continue;	//trajectory already found it's best fitting face

			std::set<_2Real::Space2D*>::const_iterator itF = remainingDetections.find( it->second.second );
			if( itF == remainingDetections.end() )
				continue;	//face already used for another trajecotry

			( *itT )->add( **itF, it->first, time );

			remainingTrajectories.erase( itT );
			remainingDetections.erase( itF );
		}

		//STEP 3: remaining short trajectories (size < 2) are filled with best faces by affinity value (thresholded)
		for( PriorityMap::const_iterator it = priorityMapAffinity.begin(); it != priorityMapAffinity.end(); ++it )
		{
			if( it->first > affThresh )
				break;		//skip correllations with too high affinity value

			if( it->second.first->getFaceTrajectory().canCalcCoherence() )
				continue;

			std::set<TrackingInfo*>::const_iterator itT = remainingTrajectories.find( it->second.first );
			if( itT == remainingTrajectories.end() )
				continue;	//trajectory already found it's best fitting face

			std::set<_2Real::Space2D*>::const_iterator itF = remainingDetections.find( it->second.second );
			if( itF == remainingDetections.end() )
				continue;	//face already used for another trajecotry

			( *itT )->add( **itF, 0.0, time );

			remainingTrajectories.erase( itT );
			remainingDetections.erase( itF );
		}

		//STEP 4: remaining trajectories are extrapolated (implement kalman, finally?) (if size is large enough, otherwise it does not make sense
		for( std::set<TrackingInfo*>::const_iterator it = remainingTrajectories.begin(); it != remainingTrajectories.end(); ++it )
			( *it )->extrapolate( time, damping, cohRise );

		//STEP 5: discard all trajectories with too high coherence average
		TrackingInfoList::iterator itT = m_trackingInfoList.begin();
		while( itT != m_trackingInfoList.end() )
		{
			if( itT->getFaceTrajectory().getAvrgCoherence() > discardThresh )
			{
				remainingTrajectories.erase( &( *itT ) );
				m_availableUserIDs.push_back( itT->getUserID() );

				itT = m_trackingInfoList.erase( itT );
			}
			else
				itT++;
		}

		for( std::set<TrackingInfo*>::iterator it = remainingTrajectories.begin(); it != remainingTrajectories.end(); ++it )
			( *it )->prune();

		//STEP 6: discard all trajectories which ran out of entries
		itT = m_trackingInfoList.begin();
		while( itT != m_trackingInfoList.end() )
		{
			if( !itT->getEntryCount() )
			{
				m_availableUserIDs.push_back( itT->getUserID() );

				itT = m_trackingInfoList.erase( itT );
			}
			else
				itT++;
		}
	}

	//STEP 7: build new trajectories out of remaining faces (extrapolation not possible due to low number of entries)
	if( m_availableUserIDs.size() )
	{
		for( std::set<_2Real::Space2D*>::const_iterator it = remainingDetections.begin(); it != remainingDetections.end(); ++it )
		{
			m_trackingInfoList.push_back( TrackingInfo( m_availableUserIDs.back(), minSizeFace ) );
			m_availableUserIDs.pop_back();

			m_trackingInfoList.back().add( **it, 0.0, time );
		}
	}
}