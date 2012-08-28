#pragma once

#include "_2RealDatatypes.h"

#include "opencv/cv.h"

#include "Poco/Timespan.h"
#include "Poco/Timestamp.h"

#include <set>
#include <list>
#include <vector>

namespace faceTracking
{
namespace impl
{

	typedef std::vector< _2Real::Space2D >	FeatureVector;
	typedef std::vector< CvRect >			CvRectVector;

	class TimeImpl
	{
	public:
		TimeImpl() :
			m_startTime()
		{}

		void restart()
		{
			m_startTime = Poco::Timestamp();
		}

		double milliSeconds() const
		{
			return ( m_startTime.elapsed() * 1e-6 );
		}

	private:
		Poco::Timestamp	m_startTime;
	};

	class Trajectory
	{
	public:
		struct Entry
		{
			_2Real::Vec2 pos;
			_2Real::Vec2 size;

			bool	isExtrapolated;
			double	coherence;

			Entry( const _2Real::Vec2 &pos, const _2Real::Vec2 &size, double coherence, bool isExtrapolated ) :
				pos( pos ), 
				size( size ), 
				isExtrapolated( isExtrapolated ), 
				coherence( coherence )
			{}
		};

		typedef std::list<Entry>	EntryList;

	private:
		double calcPosAffinity( const _2Real::Vec2 &p, double screenWidth, double time ) const
		{
			if( !this->canCalcPosAffinity() )
				throw std::exception( "cannot calc position affinity -- not enough values" );

			double ret = ( p - this->v1 ).norm();
			double dt = time - this->prevTime;

			return ret / ( screenWidth * dt * 30.0f );
		}

		double calcSizeAffinity( const _2Real::Vec2 &size, double time ) const
		{
			if( !this->canCalcSizeAffinity() )
				throw std::exception( "cannot calc size affinity -- not enough values" );

			double area0 = this->s1[0] * this->s1[1];	//calc area of previous size
			double area1 = size[0] * size[1];			//calc area of this size

			double dt = time - this->prevTime;

			double ratio = 0.0f;
			if( std::abs( area0 * area1 ) > std::numeric_limits<double>::epsilon() )	//if one of both is 0, ratio should be zero (actually the return value should be even higher than 1?)
			{
				ratio = sqrt( area0 / area1 );
				if( ratio > 1.0f )
					ratio = 1.0f / ratio;
			}

			return ( 1.0f - ratio ) / ( dt * 30.0f );
		}

		double calcDirCoherence( const _2Real::Vec2 &p, double time, double dirTolerance = 0.0f ) const
		{
			if( !this->canCalcDirCoherence() )
				throw std::exception( "cannot calc directional coherence -- not enough values" );

			_2Real::Vec2 d0( this->v1 - this->v0 );
			_2Real::Vec2 d1( p - this->v1 );

			double len0 = d0.norm();
			double len1 = d1.norm();

			double dt = time - this->prevTime;

			double compDir = 0.0f;
			if( len0 * len1 > std::numeric_limits<double>::epsilon() )
			{
				if( len0 > dirTolerance && len1 > dirTolerance )
					compDir = ( 1.0f - ( d0.dot( d1 ) / ( len0 * len1 ) ) ) * 0.5f;
				else
					compDir = 0.5f;		//cannot make a decision -> tie value.
			}

			return compDir / ( dt * 30.0f );
		}

		double calcVelCoherence( const _2Real::Vec2 &p, double time, double velTolerance = 0.0f ) const
		{
			if( !this->canCalcVelCoherence() )
				throw std::exception( "cannot calc velocity coherence -- not enough values" );

			_2Real::Vec2 d0( this->v1 - this->v0 );
			_2Real::Vec2 d1( p - this->v1 );

			double len0 = std::max<double>( d0.norm(), velTolerance );
			double len1 = std::max<double>( d1.norm(), velTolerance );

			double dt = time - this->prevTime;

			double compVel = 0.0f;
			compVel = 1.0f - 2.0f * std::sqrt( len0 * len1 ) / ( len0 + len1 );

			return compVel / ( dt * 30.0f );
		}

		double calcSizeCoherence( const _2Real::Vec2 &s, double time, double sizeTolerance = 0.0f ) const
		{
			if( !this->canCalcSizeCoherence() )
				throw std::exception( "cannot calc size coherence -- not enough values" );

			double a0 = sqrt( this->s0[0] * this->s0[1] );
			double a1 = sqrt( this->s1[0] * this->s1[1] );
			double a2 = sqrt( s[0] * s[1] );

			double d0 = std::max( std::abs( a1 - a0 ), sizeTolerance );
			double d1 = std::max( std::abs( a2 - a1 ), sizeTolerance );

			double dt = time - this->prevTime;

			double compSize = 0.0f;
			compSize = 1.0f - 2.0f * std::sqrt( d0 * d1 ) / ( d0 + d1 );

			return compSize / ( dt * 30.0f );
		}

		bool canCalcPosAffinity() const			{	return ( this->list.size() );		}
		bool canCalcSizeAffinity() const		{	return ( this->list.size() );		}

		bool canCalcDirCoherence() const		{	return ( this->list.size() > 1 );	}
		bool canCalcVelCoherence() const		{	return ( this->list.size() > 1 );	}
		bool canCalcSizeCoherence() const		{	return ( this->list.size() > 1 );	}

		size_t	maxSize;
		double coherenceSum;

		_2Real::Vec2 v0;
		_2Real::Vec2 v1;

		_2Real::Vec2 s0;
		_2Real::Vec2 s1;

		double prevTime;

		EntryList	list;
	
	public:
		explicit Trajectory( size_t maxSize = 16 ) : 
			maxSize( maxSize ),
			coherenceSum( 0.0f ),
			v0( 0.0f, 0.0f ), v1( 0.0f, 0.0f ),
			s0( 0.0f, 0.0f ), s1( 0.0f, 0.0f ),
			prevTime( 0.0f )
		{}

		//TODO: the calculated measures are not quite correct... have to take image aspect reatio into account as well (since
		// passed coordinates/regions are normalized to image bounds and measures/tolerances are non-uniform as a result)
		double calcAffinity( const _2Real::Space2D &region, double posWeight, double sizeWeight, double screenWidth, double time ) const
		{
			if( !this->canCalcAffinity() )
				throw std::exception( "cannot calc affinity -- not enough values" );

			_2Real::Vec2 center( ( region.getP1() + region.getP0() ) * 0.5f );
			_2Real::Vec2 size( region.getP1() - region.getP0() );

			return 
				( this->calcPosAffinity( center, screenWidth, time ) * posWeight ) + 
				( this->calcSizeAffinity( size, time ) * sizeWeight );
		}

		//TODO: the calculated measures are not quite correct... have to take image aspect reatio into account as well (since
		// passed coordinates/regions are normalized to image bounds and measures/tolerances are non-uniform as a result)
		double calcCoherence( const _2Real::Space2D &region, double dirWeight, double velWeight, double sizeWeight, double time, double dirTolerance, double velTolerance, double sizeTolerance ) const
		{
			if( !this->canCalcCoherence() )
				throw std::exception( "cannot calc coherence -- not enough values" );

			_2Real::Vec2 center( ( region.getP1() + region.getP0() ) * 0.5f );
			_2Real::Vec2 size( region.getP1() - region.getP0() );

			return 
				( this->calcDirCoherence( center, time, dirTolerance ) * dirWeight ) + 
				( this->calcVelCoherence( center, time, velTolerance ) * velWeight ) + 
				( this->calcSizeCoherence( size, time, sizeTolerance ) * sizeWeight );
		}
	
		bool canCalcAffinity() const			{	return ( this->canCalcPosAffinity() && this->canCalcSizeAffinity() );									}
		bool canCalcCoherence() const			{	return ( this->canCalcDirCoherence() && this->canCalcVelCoherence() && this->canCalcSizeCoherence() );	}

		void add( const _2Real::Space2D &region, double coherence, double time, bool isExtrapolated = false )
		{
			this->v0 = this->v1;
			this->v1 = ( region.getP1() + region.getP0() ) * 0.5f;

			this->s0 = this->s1;
			this->s1 = region.getP1() - region.getP0();

			this->prevTime = time;

			while( this->list.size() + 1 >= this->maxSize )
				this->prune();

			this->list.push_back( Entry( this->v1, this->s1, coherence, isExtrapolated ) );
			this->coherenceSum += coherence;
		}

		bool extrapolate( const _2Real::Vec2 &minSize, double time, double damping, double coherenceRise )
		{
			size_t s = this->list.size();
			if( s < 2 )
				return false;

			_2Real::Vec2 pos( this->v1 + ( this->v1 - this->v0 ) * damping );
			_2Real::Vec2 size( this->s1 + ( this->s1 - this->s0 ) * damping );

			double dt = time - this->prevTime;

			size[0] = std::max( minSize[0], size[0] );
			size[1] = std::max( minSize[1], size[1] );

			double coherence = this->list.back().coherence * ( 1.0f + coherenceRise / ( dt * 30.0f ) );

			this->add( _2Real::Space2D( pos - size * 0.5f, pos + size * 0.5f ), coherence, time, true );

			return true;
		}

		void prune()
		{
			if( this->list.size() )
			{
				this->coherenceSum -= this->list.front().coherence;
				this->list.pop_front();
			}
		}

		unsigned int getEntryCount() const					{	return this->list.size();	}
		const EntryList &getEntries() const					{	return this->list;			}

		double getAvrgCoherence() const
		{
			return ( this->list.size() ? this->coherenceSum / this->list.size() : 0.0f );
		}

		_2Real::Space2D getLastRegion() const
		{
			if( !this->list.size() )
				throw std::runtime_error( "cannot get last region -- trajectory is empty!" );

			const Entry &e = this->list.back();
			return _2Real::Space2D( e.pos - e.size * 0.5f, e.pos + e.size * 0.5f );
		}
	};

	class CVImpl
	{
	public:

		CVImpl() :
			m_equalizeHist( false ),
			m_minNeighboursFace( 3 ),
			m_minNeighboursEyes( 3 ),
			m_minNeighboursNose( 3 ),
			m_minNeighboursMouth( 3 ),
			m_minSizeFace( cvSize( 0, 0 ) ),
			m_minSizeEyes( cvSize( 0, 0 ) ),
			m_minSizeNose( cvSize( 0, 0 ) ),
			m_minSizeMouth( cvSize( 0, 0 ) ),
			m_subRegionEyes( _2Real::Vec2( 0.0f, 0.0f ), _2Real::Vec2( 1.0f, 0.5f ) ),
			m_subRegionNose( _2Real::Vec2( 0.25f, 0.0f ), _2Real::Vec2( 0.75f, 1.0f ) ),
			m_subRegionMouth( _2Real::Vec2( 0.0f, 0.5f ), _2Real::Vec2( 1.0f, 1.0f ) ),
			m_doCannyPruning( false ),
			m_haarScaleFactor( 1.1 ),
			m_cascadeFace( NULL ),
			m_cascadeEyes( NULL ),
			m_cascadeNose( NULL ),
			m_cascadeMouth( NULL ),
			m_cvStorage( cvCreateMemStorage( 0 ) ),
			m_greyImgOnlyHeader( false ),
			m_greyImgSmlScale( 1.0f ),
			m_conversionFlag( CV_RGB2GRAY ),
			m_sourceImg( NULL ),
			m_greyImg( NULL ),
			m_greyImgSml( NULL ),
			m_sourceImageWidth( 0 ),
			m_sourceImageHeight( 0 ),
			m_sourceImageBPC( 0 ),
			m_sourceImageCO( _2Real::ImageChannelOrder::RGB ),
			m_sourceImageNOC( 0 ),
			m_souceImageType( _2Real::ImageType::UNSIGNED_BYTE )
		{}

		~CVImpl()
		{
			releaseImages();

			releaseFaceCascade();
			releaseEyesCascade();
			releaseNoseCascade();
			releaseMouthCascade();

			if( m_cvStorage )
			{
				cvReleaseMemStorage( &m_cvStorage );
				m_cvStorage = NULL;
			}
		}

		void loadFaceCascade( const std::string &file )
		{
			releaseFaceCascade();

			std::cout << "loading face cascade file" << std::endl;

			//TODO: replace this when FilePath datatype has arrived
			std::string absPath( "E:\\work\\source\\_2RealFramework\\bundles\\bin\\" );
			absPath.append( file );
			m_cascadeFace = (CvHaarClassifierCascade*)cvLoad( absPath.c_str() );
			/*
			FilePath relPath( file );
			m_cascadeFace = (CvHaarClassifierCascade*)cvLoad( relPath );
			*/
		
			if( !m_cascadeFace )
			{
				std::stringstream sstr;
				sstr << "failed to load cascade file \"" << absPath << "\"" << std::endl;
				//sstr << "failed to load cascade file \"" << relPath << "\"" << std::endl;
				throw std::runtime_error( sstr.str() );
			}
		}

		void loadEyesCascade( const std::string &file )
		{
			releaseEyesCascade();

			std::cout << "loading eyes cascade file" << std::endl;

			//TODO: replace this when FilePath datatype has arrived
			std::string absPath( "E:\\work\\source\\_2RealFramework\\bundles\\bin\\" );
			absPath.append( file );
			m_cascadeEyes = (CvHaarClassifierCascade*)cvLoad( absPath.c_str() );
			/*
			FilePath relPath( file );
			m_cascadeFace = (CvHaarClassifierCascade*)cvLoad( relPath );
			*/

			if( !m_cascadeEyes )
			{
				std::stringstream sstr;
				sstr << "failed to load cascade file \"" << absPath << "\"" << std::endl;
				//sstr << "failed to load cascade file \"" << relPath << "\"" << std::endl;
				throw std::runtime_error( sstr.str() );
			}
		}

		void loadNoseCascade( const std::string &file )
		{
			releaseNoseCascade();

			std::cout << "loading nose cascade file" << std::endl;

			//TODO: replace this when FilePath datatype has arrived
			std::string absPath( "E:\\work\\source\\_2RealFramework\\bundles\\bin\\" );
			absPath.append( file );
			m_cascadeNose = (CvHaarClassifierCascade*)cvLoad( absPath.c_str() );
			/*
			FilePath relPath( file );
			m_cascadeFace = (CvHaarClassifierCascade*)cvLoad( relPath );
			*/

			if( !m_cascadeNose )
			{
				std::stringstream sstr;
				sstr << "failed to load cascade file \"" << absPath << "\"" << std::endl;
				//sstr << "failed to load cascade file \"" << relPath << "\"" << std::endl;
				throw std::runtime_error( sstr.str() );
			}
		}

		void loadMouthCascade( const std::string &file )
		{
			releaseMouthCascade();

			std::cout << "loading mouth cascade file" << std::endl;

			//TODO: replace this when FilePath datatype has arrived
			std::string absPath( "E:\\work\\source\\_2RealFramework\\bundles\\bin\\" );
			absPath.append( file );
			m_cascadeMouth = (CvHaarClassifierCascade*)cvLoad( absPath.c_str() );
			/*
			FilePath relPath( file );
			m_cascadeFace = (CvHaarClassifierCascade*)cvLoad( relPath );
			*/

			if( !m_cascadeMouth )
			{
				std::stringstream sstr;
				sstr << "failed to load cascade file \"" << absPath << "\"" << std::endl;
				//sstr << "failed to load cascade file \"" << relPath << "\"" << std::endl;
				throw std::runtime_error( sstr.str() );
			}
		}

		bool equalizeHist() const			{	return m_equalizeHist;	}
		void equalizeHist( bool equalize )	{	m_equalizeHist = equalize;	}

		void minNeighboursFace( unsigned int count )	{	m_minNeighboursFace = count;	}
		void minNeighboursEyes( unsigned int count )	{	m_minNeighboursEyes = count;	}
		void minNeighboursNose( unsigned int count )	{	m_minNeighboursNose = count;	}
		void minNeighboursMouth( unsigned int count )	{	m_minNeighboursMouth = count;	}

		void minSizeFace( const _2Real::Vec2 &size )		{	m_minSizeFace = cvSize( size[0], size[1] );		}
		void minSizeEyes( const _2Real::Vec2 &size )		{	m_minSizeEyes = cvSize( size[0], size[1] );		}
		void minSizeNose( const _2Real::Vec2 &size )		{	m_minSizeNose = cvSize( size[0], size[1] );		}
		void minSizeMouth( const _2Real::Vec2 &size )		{	m_minSizeMouth = cvSize( size[0], size[1] );	}

		void doCannyPruning( bool prune )			{	m_doCannyPruning = prune;		}
		void haarScaleFactor( double factor )		{	m_haarScaleFactor = factor;		}

		void greyImgSmlScale( double scale )
		{
			//TODO: is it possible to specify boundaries for inlets?
			m_greyImgSmlScale = std::max( 0.1, std::min( 1.0, scale ) );

			createSmallImage();
		}

		bool createImages( const _2Real::Image &img )
		{
			releaseImages();

			if( img.getBitsPerChannel() != 1 &&
				img.getBitsPerChannel() != 8 &&
				img.getBitsPerChannel() != 16 &&
				img.getBitsPerChannel() != 32 )
			{
				std::stringstream sstr;
				sstr << "OpenCV cannot deal with " << img.getBitsPerChannel() << " bits per channel";
				throw std::runtime_error( sstr.str() );
			}

			//TODO:
			if( img.getBitsPerChannel() != 8 )
			{
				std::stringstream sstr;
				sstr << "Image data type conversion not implemented, image has to have 8 bits per channel";
				throw std::runtime_error( sstr.str() );
			}

			m_sourceImageWidth = img.getWidth();
			m_sourceImageHeight = img.getHeight();
			m_sourceImageBPC = img.getBitsPerChannel();
			m_sourceImageCO = img.getChannelOrder();
			m_sourceImageNOC = img.getNumberOfChannels();
			m_souceImageType = img.getImageType();

			switch( m_sourceImageCO.getCode() )
			{
			case _2Real::ImageChannelOrder::RGB:
				m_conversionFlag = CV_RGB2GRAY;
				break;
			case _2Real::ImageChannelOrder::BGR:
				m_conversionFlag = CV_BGR2GRAY;
				break;
			case _2Real::ImageChannelOrder::RGBA:
			case _2Real::ImageChannelOrder::RGBX:
				m_conversionFlag = CV_RGBA2GRAY;
				break;
			case _2Real::ImageChannelOrder::BGRA:
			case _2Real::ImageChannelOrder::BGRX:
				m_conversionFlag = CV_BGRA2GRAY;
				break;
			case _2Real::ImageChannelOrder::R:
			case _2Real::ImageChannelOrder::G:
			case _2Real::ImageChannelOrder::B:
			case _2Real::ImageChannelOrder::A:
				//no conversion, only header will be created, source pointer will be copied
				break;
			case _2Real::ImageChannelOrder::ARGB:
			case _2Real::ImageChannelOrder::XRGB:
			case _2Real::ImageChannelOrder::ABGR:
			case _2Real::ImageChannelOrder::XBGR:
				{
					std::stringstream sstr;
					sstr << "image channel order " << m_sourceImageCO << ": conversion to greyscale not implemented" << std::endl;
					throw std::runtime_error( sstr.str() );
				}
			default:
				{
					std::stringstream sstr;
					sstr << "unknown image channel order " << m_sourceImageCO << ": conversion to greyscale not implemented" << std::endl;
					throw std::runtime_error( sstr.str() );
				}
			}

			if( m_sourceImageWidth * m_sourceImageHeight * m_sourceImageBPC * m_sourceImageNOC == 0 )
				return false;

			m_sourceImg = cvCreateImageHeader( cvSize( img.getWidth(), img.getHeight() ), img.getBitsPerChannel(), img.getNumberOfChannels() );
			std::cout << "created source image header with " << m_sourceImg->width << " x " << m_sourceImg->height << " x " << m_sourceImg->depth << " x " << m_sourceImg->nChannels << std::endl;

			m_greyImgOnlyHeader = ( img.getNumberOfChannels() == 1 && img.getImageType() == _2Real::ImageType::UNSIGNED_BYTE );
			m_greyImg = ( m_greyImgOnlyHeader ? 
				cvCreateImageHeader( cvSize( img.getWidth(), img.getHeight() ), IPL_DEPTH_8U, 1 ) : 
				cvCreateImage( cvSize( img.getWidth(), img.getHeight() ), IPL_DEPTH_8U, 1 ) );
			std::cout << "created grey image " << ( m_greyImgOnlyHeader ? "HEADER" : "" ) << " with " << m_greyImg->width << " x " << m_greyImg->height << " x " << m_greyImg->depth << " x " << m_greyImg->nChannels << std::endl;

			if( !createSmallImage() )
				return false;

			return true;
		}

		void releaseImages()
		{
			if( m_sourceImg )
			{
				cvReleaseImageHeader( &m_sourceImg );
				m_sourceImg = NULL;
			}
			if( m_greyImg )
			{
				if( m_greyImgOnlyHeader )
					cvReleaseImageHeader( &m_greyImg );
				else
					cvReleaseImage( &m_greyImg );
				m_greyImg = NULL;
			}

			releaseSmallImage();
		}

		void detectFaces( const _2Real::Image &img, FeatureVector &featureVec )
		{
			featureVec.clear();

			if( !makeGreyImages( img ) )
				return;

			CvRectVector foundFaces;

			//TODO: before scanning whole image for faces, search in areas faces were previously at (with some tolerance, framerate-dependent)
			// only if unsuccessuful, or no previous face positions present, scan overall image
			this->detect( foundFaces, m_cascadeFace, m_minNeighboursFace, m_minSizeFace );
			std::cout << "found " << foundFaces.size() << " faces" << std::endl;

			convertCvRectToSpace2D( foundFaces, featureVec, _2Real::Space2D( _2Real::Vec2( 0.0f, 0.0f ), _2Real::Vec2( 1.0f, 1.0f ) ) );
		}

		void detectFeatures( const _2Real::Image &img, const _2Real::Space2D &faceRegion, FeatureVector &eyesCandidates, FeatureVector &noseCandidates, FeatureVector &mouthCandidates, bool useEyes, bool useNose, bool useMouth )
		{
			eyesCandidates.clear();
			noseCandidates.clear();
			mouthCandidates.clear();

			if( !useEyes && !useNose && !useMouth )
				return;

			//calc pixel-coordinates from faceRegion and img extends
			int x = cvRound( faceRegion.getP0()[0] * img.getWidth() );
			int y = cvRound( faceRegion.getP0()[1] * img.getHeight() );
			int width = cvRound( ( faceRegion.getP1()[0] - faceRegion.getP0()[0] ) * img.getWidth() );
			int height = cvRound( ( faceRegion.getP1()[1] - faceRegion.getP0()[1] ) * img.getHeight() );

			if( useEyes )
			{
				CvRectVector foundEyes;
				CvRect roi = cvRect( 
					cvRound( x + width * m_subRegionEyes.getP0()[0] ),
					cvRound( y + height * m_subRegionEyes.getP0()[1] ),
					cvRound( width * ( m_subRegionEyes.getP1()[0] - m_subRegionEyes.getP0()[0] ) ),
					cvRound( height * ( m_subRegionEyes.getP1()[1] - m_subRegionEyes.getP0()[1] ) ) );

				this->detect( foundEyes, m_cascadeEyes, m_minNeighboursEyes, m_minSizeEyes, &roi );

				convertCvRectToSpace2D( foundEyes, eyesCandidates, faceRegion );
			}
			if( useNose )
			{
				CvRectVector foundNoses;
				CvRect roi = cvRect( 
					cvRound( x + width * m_subRegionNose.getP0()[0] ),
					cvRound( y + height * m_subRegionNose.getP0()[1] ),
					cvRound( width * ( m_subRegionNose.getP1()[0] - m_subRegionNose.getP0()[0] ) ),
					cvRound( height * ( m_subRegionNose.getP1()[1] - m_subRegionNose.getP0()[1] ) ) );

				this->detect( foundNoses, m_cascadeNose, m_minNeighboursNose, m_minSizeNose, &roi );

				convertCvRectToSpace2D( foundNoses, noseCandidates, faceRegion );
			}
			if( useMouth )
			{
				CvRectVector foundMouths;
				CvRect roi = cvRect( 
					cvRound( x + width * m_subRegionMouth.getP0()[0] ),
					cvRound( y + height * m_subRegionMouth.getP0()[1] ),
					cvRound( width * ( m_subRegionMouth.getP1()[0] - m_subRegionMouth.getP0()[0] ) ),
					cvRound( height * ( m_subRegionMouth.getP1()[1] - m_subRegionMouth.getP0()[1] ) ) );

				this->detect( foundMouths, m_cascadeMouth, m_minNeighboursMouth, m_minSizeMouth, &roi );

				convertCvRectToSpace2D( foundMouths, mouthCandidates, faceRegion );
			}
		}

	private:

		bool	m_equalizeHist;

		unsigned int	m_minNeighboursFace;
		unsigned int	m_minNeighboursEyes;
		unsigned int	m_minNeighboursNose;
		unsigned int	m_minNeighboursMouth;

		CvSize	m_minSizeFace;
		CvSize	m_minSizeEyes;
		CvSize	m_minSizeNose;
		CvSize	m_minSizeMouth;

		_2Real::Space2D	m_subRegionEyes;
		_2Real::Space2D	m_subRegionNose;
		_2Real::Space2D	m_subRegionMouth;

		bool	m_doCannyPruning;
		double	m_haarScaleFactor;

		CvHaarClassifierCascade		*m_cascadeFace;
		CvHaarClassifierCascade		*m_cascadeEyes;
		CvHaarClassifierCascade		*m_cascadeNose;
		CvHaarClassifierCascade		*m_cascadeMouth;

		CvMemStorage	*m_cvStorage;

		bool			m_greyImgOnlyHeader;
		double			m_greyImgSmlScale;
		unsigned int	m_conversionFlag;

		IplImage	*m_sourceImg;
		IplImage	*m_greyImg;
		IplImage	*m_greyImgSml;

		unsigned int				m_sourceImageWidth;
		unsigned int				m_sourceImageHeight;
		size_t						m_sourceImageBPC;
		_2Real::ImageChannelOrder	m_sourceImageCO;
		unsigned int				m_sourceImageNOC;
		_2Real::ImageType			m_souceImageType;

		void releaseFaceCascade()
		{
			if( m_cascadeFace )
			{
				cvReleaseHaarClassifierCascade( &m_cascadeFace );
				m_cascadeFace = NULL;
			}
		}

		void releaseEyesCascade()
		{
			if( m_cascadeEyes )
			{
				cvReleaseHaarClassifierCascade( &m_cascadeEyes );
				m_cascadeEyes = NULL;
			}
		}

		void releaseNoseCascade()
		{
			if( m_cascadeNose )
			{
				cvReleaseHaarClassifierCascade( &m_cascadeNose );
				m_cascadeNose = NULL;
			}
		}

		void releaseMouthCascade()
		{
			if( m_cascadeMouth )
			{
				cvReleaseHaarClassifierCascade( &m_cascadeMouth );
				m_cascadeMouth = NULL;
			}
		}

		bool createSmallImage()
		{
			releaseSmallImage();

			if( m_sourceImageWidth * m_sourceImageHeight * m_sourceImageBPC * m_sourceImageNOC == 0 )
				return false;

			m_greyImgSml = cvCreateImage( cvSize( cvRound( m_sourceImageWidth * m_greyImgSmlScale ), cvRound( m_sourceImageHeight * m_greyImgSmlScale ) ), 8, 1 );
			std::cout << "created grey image small with " << m_greyImgSml->width << " x " << m_greyImgSml->height << " x " << m_greyImgSml->depth << " x " << m_greyImgSml->nChannels << std::endl;
		}

		void releaseSmallImage()
		{
			if( m_greyImgSml )
			{
				cvReleaseImage( &m_greyImgSml );
				m_greyImgSml = NULL;
			}
		}

		bool makeGreyImages( const _2Real::Image &img )
		{
			if( img.getWidth() != m_sourceImageWidth ||
				img.getHeight() != m_sourceImageHeight ||
				img.getBitsPerChannel() != m_sourceImageBPC ||
				img.getChannelOrder() != m_sourceImageCO ||
				img.getNumberOfChannels() != m_sourceImageNOC ||
				img.getImageType() != m_souceImageType )
			{
				if( !createImages( img ) )
					return false;
			}

			if( m_sourceImageWidth * m_sourceImageHeight * m_sourceImageBPC * m_sourceImageNOC == 0 )
				return false;

			m_sourceImg->imageData = (char*)img.getData();	//IplImage does not have const ptrs to image data, so we have to cast.

			//if source image is in the correct format anyways, we don't convert or copy, but simply use the raw source data
			if( m_greyImgOnlyHeader )
				m_greyImg->imageData = m_sourceImg->imageData;
			else
				cvCvtColor( m_sourceImg, m_greyImg, m_conversionFlag );

			cvResize( m_greyImg, m_greyImgSml, CV_INTER_LINEAR );

			//TODO: figure out why this is recommended in the OpenCV book but actually feature detection (eyes, etc.) works better without
			if( m_equalizeHist )
				cvEqualizeHist( m_greyImgSml, m_greyImgSml );

			return true;
		}

		void detect( CvRectVector &vec, CvHaarClassifierCascade *cascade, unsigned int minNeighbours, const CvSize &minSize, const CvRect *roi = NULL )
		{
			if( roi && ( roi->x >= m_greyImgSml->width || roi->y >= m_greyImgSml->height || roi->x + roi->width <= 0 || roi->y + roi->height <= 0 || roi->width <= 0 || roi->height <= 0 ) )
			{
				//openCV haar detection algorithm would crash
				return;
			}

			cvClearMemStorage( m_cvStorage );

			if( roi )
				cvSetImageROI( m_greyImgSml, *roi );
			else
				cvResetImageROI( m_greyImgSml );

			CvSeq *objects = cvHaarDetectObjects(
				m_greyImgSml,
				cascade,
				m_cvStorage,
				m_haarScaleFactor,
				minNeighbours,
				( m_doCannyPruning ? CV_HAAR_DO_CANNY_PRUNING : 0 ),
				minSize );

			for( int i = 0; i < ( objects ? objects->total : 0 ); i++ )
			{
				CvRect *r = (CvRect*)cvGetSeqElem( objects, i );
				if( roi )
				{
					r->x += roi->x;
					r->y += roi->y;
				}

				vec.push_back( *r );
			}

			cvResetImageROI( m_greyImgSml );
		}

		void convertCvRectToSpace2D( const CvRect &src, _2Real::Space2D &dest, const _2Real::Space2D &refSpace )
		{
			//convert (downscaled) pixel coordinates to normalized values in range [0 1]
			double centerX = ( src.x + src.width * 0.5f ) / m_greyImgSml->width;
			double centerY = ( src.y + src.width * 0.5f ) / m_greyImgSml->height;
			double scaledWidth = (double)( src.width ) / m_greyImgSml->width;
			double scaledHeight = (double)( src.height ) / m_greyImgSml->height;

			_2Real::Vec2 refPos( refSpace.getP0() );
			_2Real::Vec2 refSize( refSpace.getP1() - refSpace.getP0() );

			centerX = ( centerX - refPos[0] ) / refSize[0];
			centerY = ( centerY - refPos[1] ) / refSize[1];
			scaledWidth /= refSize[0];
			scaledHeight /= refSize[1];

			dest = _2Real::Space2D( 
						_2Real::Vec2( centerX - scaledWidth * 0.5f, centerY - scaledHeight * 0.5f ), 
						_2Real::Vec2( centerX + scaledWidth * 0.5f, centerY + scaledHeight * 0.5f ) );
		}

		void convertCvRectToSpace2D( const CvRectVector &src, FeatureVector &dest, const _2Real::Space2D &refSpace )
		{
			dest.clear();

			_2Real::Space2D s;
			for( CvRectVector::const_iterator it = src.begin(); it != src.end(); ++it )
			{
				convertCvRectToSpace2D( *it, s, refSpace );
				dest.push_back( s );
			}
		}
	};

	class TrackingInfo
	{
	private:
		unsigned int	userID;

		Trajectory	faceTrajectory;

		Trajectory	eyeLeftTrajectory;
		Trajectory	eyeRightTrajectory;
		Trajectory	noseTrajectory;
		Trajectory	mouthTrajectory;

		_2Real::Vec2	minFaceSize;

	public:
		explicit TrackingInfo( unsigned int userID, const _2Real::Vec2 &minFaceSize ) :
			userID( userID ),
			minFaceSize( minFaceSize )
		{}

		void add( const _2Real::Space2D &faceRegion, double coherence, double time )
		{
			this->faceTrajectory.add( faceRegion, coherence, time );
		}

		void addFromFeatureCandidates( const FeatureVector &eyesCandidates, const FeatureVector &noseCandidates, const FeatureVector &mouthCandidates, double time, double posAffWeight, double sizeAffWeight )
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
						if( this->eyeLeftTrajectory.canCalcAffinity() )
							mapLeft.insert( std::make_pair( this->eyeLeftTrajectory.calcAffinity( *it, posAffWeight, sizeAffWeight, 1.0f, time ), &( *it ) ) );
						else
							mapLeft.insert( std::make_pair( 0.0f, &( *it ) ) );
					}
					else
					{
						if( this->eyeRightTrajectory.canCalcAffinity() )
							mapRight.insert( std::make_pair( this->eyeRightTrajectory.calcAffinity( *it, posAffWeight, sizeAffWeight, 1.0f, time ), &( *it ) ) );
						else
							mapRight.insert( std::make_pair( 0.0f, &( *it ) ) );
					}
				}

				if( mapLeft.size() )
					this->eyeLeftTrajectory.add( *( mapLeft.begin()->second ), 0.0f, time );
				else
					//TODO: there is no measure (yet) to decide whether or not it would make sense to still extrapolate face feature, so this is not done at all,
					// instead trajectories are pruned so entries don't last forever. (actually *duplicating* last entry would make more sense in context of
					// face feature regions)
					this->eyeLeftTrajectory.prune();

				if( mapRight.size() )
					this->eyeRightTrajectory.add( *( mapRight.begin()->second ), 0.0f, time );
				else
					//TODO: there is no measure (yet) to decide whether or not it would make sense to still extrapolate face feature, so this is not done at all,
					// instead trajectories are pruned so entries don't last forever. (actually *duplicating* last entry would make more sense in context of
					// face feature regions)
					this->eyeRightTrajectory.prune();
			}
			else
			{
				//TODO: there is no measure (yet) to decide whether or not it would make sense to still extrapolate face feature, so this is not done at all,
				// instead trajectories are pruned so entries don't last forever. (actually *duplicating* last entry would make more sense in context of
				// face feature regions)
				this->eyeLeftTrajectory.prune();
				this->eyeRightTrajectory.prune();
			}

			if( noseCandidates.size() )
			{
				double minAffinity = 0.0f;
				const _2Real::Space2D *bestCandidate = NULL;

				if( !this->noseTrajectory.canCalcAffinity() )
					bestCandidate = &( noseCandidates.front() ); //not enough regions in trajectory history in order to judge, just take first in candidate list
				else
				{
					for( FeatureVector::const_iterator it = noseCandidates.begin(); it != noseCandidates.end(); ++it )
					{
						if( !bestCandidate )
						{
							bestCandidate = &( *it );
							minAffinity = this->noseTrajectory.calcAffinity( *it, posAffWeight, sizeAffWeight, 1.0f, time );
						}
						else
						{
							double affinity = this->noseTrajectory.calcAffinity( *it, posAffWeight, sizeAffWeight, 1.0f, time );
							if( affinity < minAffinity )
							{
								bestCandidate = &( *it );
								minAffinity = affinity;
							}
						}
					}
				}

				this->noseTrajectory.add( *bestCandidate, 0.0f, time );
			}
			else
				//TODO: there is no measure (yet) to decide whether or not it would make sense to still extrapolate face feature, so this is not done at all,
				// instead trajectories are pruned so entries don't last forever. (actually *duplicating* last entry would make more sense in context of
				// face feature regions)
				this->noseTrajectory.prune();

			if( mouthCandidates.size() )
			{
				double minAffinity = 0.0f;
				const _2Real::Space2D *bestCandidate = NULL;

				if( !this->mouthTrajectory.canCalcAffinity() )
					bestCandidate = &( mouthCandidates.front() ); //not enough regions in trajectory history in order to judge, just take first in candidate list
				else
				{
					for( FeatureVector::const_iterator it = mouthCandidates.begin(); it != mouthCandidates.end(); ++it )
					{
						if( !bestCandidate )
						{
							bestCandidate = &( *it );
							minAffinity = this->mouthTrajectory.calcAffinity( *it, posAffWeight, sizeAffWeight, 1.0f, time );
						}
						else
						{
							double affinity = this->mouthTrajectory.calcAffinity( *it, posAffWeight, sizeAffWeight, 1.0f, time );
							if( affinity < minAffinity )
							{
								bestCandidate = &( *it );
								minAffinity = affinity;
							}
						}
					}
				}

				this->mouthTrajectory.add( *bestCandidate, 0.0f, time );
			}
			else
				//TODO: there is no measure (yet) to decide whether or not it would make sense to still extrapolate face feature, so this is not done at all,
				// instead trajectories are pruned so entries don't last forever. (actually *duplicating* last entry would make more sense in context of
				// face feature regions)
				this->mouthTrajectory.prune();
		}

		void prune()
		{
			this->faceTrajectory.prune();

			this->eyeLeftTrajectory.prune();
			this->eyeRightTrajectory.prune();
			this->noseTrajectory.prune();
			this->mouthTrajectory.prune();
		}

		bool extrapolate( double time, double damping, double coherenceRise )
		{
			//TODO: there is no measure (yet) to decide whether or not it would make sense to still extrapolate face feature, so this is not done at all,
			// instead trajectories are pruned so entries don't last forever. (actually *duplicating* last entry would make more sense in context of
			// face feature regions)
			this->eyeLeftTrajectory.prune();
			this->eyeRightTrajectory.prune();
			this->noseTrajectory.prune();
			this->mouthTrajectory.prune();

			return this->faceTrajectory.extrapolate( this->minFaceSize, time, damping, coherenceRise );
		}

		const Trajectory	&getFaceTrajectory() const		{	return this->faceTrajectory;		}

		const Trajectory	&getEyeLeftTrajectory() const	{	return this->eyeLeftTrajectory;		}
		const Trajectory	&getEyeRightTrajectory() const	{	return this->eyeRightTrajectory;	}
		const Trajectory	&getNoseTrajectory() const		{	return this->noseTrajectory;		}
		const Trajectory	&getMouthTrajectory() const		{	return this->mouthTrajectory;		}

		unsigned int getUserID() const			{	return this->userID;							}
		unsigned int getEntryCount() const		{	return this->faceTrajectory.getEntryCount();	}
	};

	typedef std::list<TrackingInfo>	TrackingInfoList;

	class FaceTrackingImpl
	{
	public:
		explicit FaceTrackingImpl( unsigned int maxUsers )
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

					( *itT )->add( **itF, 0.0f, time );

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

					m_trackingInfoList.back().add( **it, 0.0f, time );
				}
			}
		}

	private:

		TrackingInfoList			m_trackingInfoList;

		std::vector<unsigned int>	m_availableUserIDs;

	};
}
}