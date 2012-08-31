#include "FaceDetection.h"

#include "opencv/cv.h"

namespace facetracking
{
	typedef std::vector< CvRect >			CvRectVector;

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
			m_subRegionEyes( _2Real::Vec2( 0.0, 0.0 ), _2Real::Vec2( 1.0, 0.5 ) ),
			m_subRegionNose( _2Real::Vec2( 0.25, 0.0 ), _2Real::Vec2( 0.75, 1.0 ) ),
			m_subRegionMouth( _2Real::Vec2( 0.0, 0.5 ), _2Real::Vec2( 1.0, 1.0 ) ),
			m_doCannyPruning( false ),
			m_haarScaleFactor( 1.1 ),
			m_cascadeFace( NULL ),
			m_cascadeEyes( NULL ),
			m_cascadeNose( NULL ),
			m_cascadeMouth( NULL ),
			m_cvStorage( cvCreateMemStorage( 0 ) ),
			m_greyImgOnlyHeader( false ),
			m_greyImgSmlScale( 1.0 ),
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

		void minSizeFace( const _2Real::Vec2 &size )		{	m_minSizeFace = cvSize( (int)size[0], (int)size[1] );		}
		void minSizeEyes( const _2Real::Vec2 &size )		{	m_minSizeEyes = cvSize( (int)size[0], (int)size[1] );		}
		void minSizeNose( const _2Real::Vec2 &size )		{	m_minSizeNose = cvSize( (int)size[0], (int)size[1] );		}
		void minSizeMouth( const _2Real::Vec2 &size )		{	m_minSizeMouth = cvSize( (int)size[0], (int)size[1] );	}

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

			if( img.getBitsPerChannel() != 8 )
			{
				std::stringstream sstr;
				sstr << "Image data type conversion not supported, image has to have 8 bits per channel";
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
			
			m_greyImgOnlyHeader = ( img.getNumberOfChannels() == 1 && img.getImageType() == _2Real::ImageType::UNSIGNED_BYTE );
			m_greyImg = ( m_greyImgOnlyHeader ? 
				cvCreateImageHeader( cvSize( img.getWidth(), img.getHeight() ), IPL_DEPTH_8U, 1 ) : 
				cvCreateImage( cvSize( img.getWidth(), img.getHeight() ), IPL_DEPTH_8U, 1 ) );
			
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

			this->detect( foundFaces, m_cascadeFace, m_minNeighboursFace, m_minSizeFace );

			convertCvRectToSpace2D( foundFaces, featureVec, _2Real::Space2D( _2Real::Vec2( 0.0, 0.0 ), _2Real::Vec2( 1.0, 1.0 ) ) );
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

			return true;
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
}


FaceDetection::FaceDetection() :
	m_pImpl( new facetracking::CVImpl() )
{}

FaceDetection::~FaceDetection()
{
	if( m_pImpl )
	{
		delete m_pImpl;
		m_pImpl = NULL;
	}
}

void FaceDetection::loadFaceCascade( const std::string &file )
{
	m_pImpl->loadFaceCascade( file );
}

void FaceDetection::loadEyesCascade( const std::string &file )
{
	m_pImpl->loadEyesCascade( file );
}

void FaceDetection::loadNoseCascade( const std::string &file )
{
	m_pImpl->loadNoseCascade( file );
}

void FaceDetection::loadMouthCascade( const std::string &file )
{
	m_pImpl->loadMouthCascade( file );
}

bool FaceDetection::equalizeHist() const
{
	return m_pImpl->equalizeHist();
}

void FaceDetection::equalizeHist( bool equalize )
{
	m_pImpl->equalizeHist( equalize );
}

void FaceDetection::minNeighboursFace( unsigned int count )
{
	m_pImpl->minNeighboursFace( count );
}

void FaceDetection::minNeighboursEyes( unsigned int count )
{
	m_pImpl->minNeighboursEyes( count );
}

void FaceDetection::minNeighboursNose( unsigned int count )
{
	m_pImpl->minNeighboursNose( count );
}

void FaceDetection::minNeighboursMouth( unsigned int count )
{
	m_pImpl->minNeighboursMouth( count );
}

void FaceDetection::minSizeFace( const _2Real::Vec2 &size )
{
	m_pImpl->minSizeFace( size );
}

void FaceDetection::minSizeEyes( const _2Real::Vec2 &size )
{
	m_pImpl->minSizeEyes( size );
}

void FaceDetection::minSizeNose( const _2Real::Vec2 &size )
{
	m_pImpl->minSizeNose( size );
}

void FaceDetection::minSizeMouth( const _2Real::Vec2 &size )
{
	m_pImpl->minSizeMouth( size );
}

void FaceDetection::doCannyPruning( bool prune )
{
	m_pImpl->doCannyPruning( prune );
}

void FaceDetection::haarScaleFactor( double factor )
{
	m_pImpl->haarScaleFactor( factor );
}

void FaceDetection::greyImgSmlScale( double scale )
{
	m_pImpl->greyImgSmlScale( scale );
}

bool FaceDetection::createImages( const _2Real::Image &img )
{
	return m_pImpl->createImages( img );
}

void FaceDetection::releaseImages()
{
	m_pImpl->releaseImages();
}

void FaceDetection::detectFaces( const _2Real::Image &img, FeatureVector &featureVec )
{
	m_pImpl->detectFaces( img, featureVec );
}

void FaceDetection::detectFeatures( const _2Real::Image &img, const _2Real::Space2D &faceRegion, FeatureVector &eyesCandidates, FeatureVector &noseCandidates, FeatureVector &mouthCandidates, bool useEyes, bool useNose, bool useMouth )
{
	m_pImpl->detectFeatures( img, faceRegion, eyesCandidates, noseCandidates, mouthCandidates, useEyes, useNose, useMouth );
}