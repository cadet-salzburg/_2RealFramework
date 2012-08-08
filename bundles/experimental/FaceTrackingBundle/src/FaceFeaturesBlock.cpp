#include "_2RealDatatypes.h"
#include "FaceFeaturesBlock.h"

#include "_2RealApplication.h"

#include "opencv/cv.h"

#include <iostream>

//TODO
#include "bench.h"
//-----

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;

typedef std::vector< Space2D >	FeatureVector;
typedef std::vector< CvRect >	CvRectVector;

class CVImpl
{
public:
	CVImpl() :
		m_useFace( false ),
		m_useEyes( false ),
		m_useNose( false ),
		m_useMouth( false ),
		m_minNeighboursFace( 3 ),
		m_minNeighboursEyes( 3 ),
		m_minNeighboursNose( 3 ),
		m_minNeighboursMouth( 3 ),
		m_minSizeFace( cvSize( 0, 0 ) ),
		m_minSizeEyes( cvSize( 0, 0 ) ),
		m_minSizeNose( cvSize( 0, 0 ) ),
		m_minSizeMouth( cvSize( 0, 0 ) ),
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
		m_sourceImageCO( ImageChannelOrder::RGB ),
		m_sourceImageNOC( 0 ),
		m_souceImageType( ImageType::UNSIGNED_BYTE )
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
		std::string absPath( "E:\\work\\source\\cadetFH\\_2RealFramework\\bundles\\bin\\" );
		absPath.append( file );
		
		m_cascadeFace = (CvHaarClassifierCascade*)cvLoad( absPath.c_str() );

		if( !m_cascadeFace )
		{
			std::stringstream sstr;
			sstr << "failed to load cascade file \"" << absPath << "\"" << std::endl;
			throw std::runtime_error( sstr.str() );
		}
	}

	void loadEyesCascade( const std::string &file )
	{
		releaseEyesCascade();

		std::cout << "loading eyes cascade file" << std::endl;

		//TODO: replace this when FilePath datatype has arrived
		std::string absPath( "E:\\work\\source\\cadetFH\\_2RealFramework\\bundles\\bin\\" );
		absPath.append( file );
		
		m_cascadeEyes = (CvHaarClassifierCascade*)cvLoad( absPath.c_str() );

		if( !m_cascadeEyes )
		{
			std::stringstream sstr;
			sstr << "failed to load cascade file \"" << absPath << "\"" << std::endl;
			throw std::runtime_error( sstr.str() );
		}
	}

	void loadNoseCascade( const std::string &file )
	{
		releaseNoseCascade();

		std::cout << "loading nose cascade file" << std::endl;

		//TODO: replace this when FilePath datatype has arrived
		std::string absPath( "E:\\work\\source\\cadetFH\\_2RealFramework\\bundles\\bin\\" );
		absPath.append( file );
		
		m_cascadeNose = (CvHaarClassifierCascade*)cvLoad( absPath.c_str() );

		if( !m_cascadeNose )
		{
			std::stringstream sstr;
			sstr << "failed to load cascade file \"" << absPath << "\"" << std::endl;
			throw std::runtime_error( sstr.str() );
		}
	}

	void loadMouthCascade( const std::string &file )
	{
		releaseMouthCascade();

		std::cout << "loading mouth cascade file" << std::endl;

		//TODO: replace this when FilePath datatype has arrived
		std::string absPath( "E:\\work\\source\\cadetFH\\_2RealFramework\\bundles\\bin\\" );
		absPath.append( file );
		
		m_cascadeMouth = (CvHaarClassifierCascade*)cvLoad( absPath.c_str() );

		if( !m_cascadeMouth )
		{
			std::stringstream sstr;
			sstr << "failed to load cascade file \"" << absPath << "\"" << std::endl;
			throw std::runtime_error( sstr.str() );
		}
	}

	bool useFace() const			{	return m_useFace;	}
	bool useEyes() const			{	return m_useEyes;	}
	bool useNose() const			{	return m_useNose;	}
	bool useMouth() const			{	return m_useMouth;	}

	void useFace( bool use )		{	m_useFace = use;	}
	void useEyes( bool use )		{	m_useEyes = use;	}
	void useNose( bool use )		{	m_useNose = use;	}
	void useMouth( bool use )		{	m_useMouth = use;	}

	void minNeighboursFace( unsigned int count )	{	m_minNeighboursFace = count;	}
	void minNeighboursEyes( unsigned int count )	{	m_minNeighboursEyes = count;	}
	void minNeighboursNose( unsigned int count )	{	m_minNeighboursNose = count;	}
	void minNeighboursMouth( unsigned int count )	{	m_minNeighboursMouth = count;	}

	void minSizeFace( const Vec2 &size )		{	m_minSizeFace = cvSize( size[0], size[1] );		}
	void minSizeEyes( const Vec2 &size )		{	m_minSizeEyes = cvSize( size[0], size[1] );		}
	void minSizeNose( const Vec2 &size )		{	m_minSizeNose = cvSize( size[0], size[1] );		}
	void minSizeMouth( const Vec2 &size )		{	m_minSizeMouth = cvSize( size[0], size[1] );	}

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
			std::cerr << "OpenCV cannot deal with " << img.getBitsPerChannel() << " bits per channel" << std::endl;
			return false;
		}

		m_sourceImageWidth = img.getWidth();
		m_sourceImageHeight = img.getHeight();
		m_sourceImageBPC = img.getBitsPerChannel();
		m_sourceImageCO = img.getChannelOrder();
		m_sourceImageNOC = img.getNumberOfChannels();
		m_souceImageType = img.getImageType();

		switch( m_sourceImageCO.getCode() )
		{
		case ImageChannelOrder::RGB:
			m_conversionFlag = CV_RGB2GRAY;
			break;
		case ImageChannelOrder::BGR:
			m_conversionFlag = CV_BGR2GRAY;
			break;
		case ImageChannelOrder::RGBA:
		case ImageChannelOrder::RGBX:
			m_conversionFlag = CV_RGBA2GRAY;
			break;
		case ImageChannelOrder::BGRA:
		case ImageChannelOrder::BGRX:
			m_conversionFlag = CV_BGRA2GRAY;
			break;
		case ImageChannelOrder::R:
		case ImageChannelOrder::G:
		case ImageChannelOrder::B:
		case ImageChannelOrder::A:
			//no conversion, only header will be created, source pointer will be copied
			break;
		case ImageChannelOrder::ARGB:
		case ImageChannelOrder::XRGB:
		case ImageChannelOrder::ABGR:
		case ImageChannelOrder::XBGR:
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

		m_greyImgOnlyHeader = ( img.getNumberOfChannels() == 1 && img.getImageType() == ImageType::UNSIGNED_BYTE );
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

	void detect( const _2Real::Image &img, FeatureVector &faces, FeatureVector &eyes, FeatureVector &noses, FeatureVector &mouths )
	{
		faces.clear();
		eyes.clear();
		noses.clear();
		mouths.clear();

		if( !m_useFace && !m_useEyes && !m_useNose && !m_useMouth )
			return;

		if( !makeGreyImages( img ) )
			return;

		//faces are needed anyway. if useFace is false we still need them as ROIs for face features
		CvRectVector tempVecFaces;
		CvRectVector tempVecEyes;
		CvRectVector tempVecNoses;
		CvRectVector tempVecMouths;

		//TODO: before scanning whole image for faces, search in areas faces were previously at (with some tolerance, framerate-dependent)
		// only if unsuccessuful, or no previous face positions present, scan overall image
		this->detect( tempVecFaces, m_cascadeFace, m_minNeighboursFace, m_minSizeFace );
		std::cout << "found " << tempVecFaces.size() << " faces" << std::endl;

		//test
		unsigned int faceCntr = 0;
		//------

		//search for face features only within found face areas
		for( CvRectVector::const_iterator it = tempVecFaces.begin(); it != tempVecFaces.end(); ++it )
		{
			std::cout << "face #:" << ( faceCntr++ ) << std::endl;
			if( m_useEyes )
			{
				this->detect( tempVecEyes, m_cascadeEyes, m_minNeighboursEyes, m_minSizeEyes, &( *it ) );
				std::cout << "found " << tempVecEyes.size() << " eyes" << std::endl;
			}
			if( m_useNose )
			{
				this->detect( tempVecNoses, m_cascadeNose, m_minNeighboursNose, m_minSizeNose, &( *it ) );
				std::cout << "found " << tempVecNoses.size() << " noses" << std::endl;
			}
			if( m_useMouth )
			{
				this->detect( tempVecMouths, m_cascadeMouth, m_minNeighboursMouth, m_minSizeMouth, &( *it ) );
				std::cout << "found " << tempVecMouths.size() << " mouths" << std::endl;
			}
		}

		this->convertCvRectToSpace2D( tempVecFaces, faces );
		this->convertCvRectToSpace2D( tempVecEyes, eyes );
		this->convertCvRectToSpace2D( tempVecNoses, noses );
		this->convertCvRectToSpace2D( tempVecMouths, mouths );
	}

private:
	bool	m_useFace;
	bool	m_useEyes;
	bool	m_useNose;
	bool	m_useMouth;

	unsigned int	m_minNeighboursFace;
	unsigned int	m_minNeighboursEyes;
	unsigned int	m_minNeighboursNose;
	unsigned int	m_minNeighboursMouth;

	CvSize	m_minSizeFace;
	CvSize	m_minSizeEyes;
	CvSize	m_minSizeNose;
	CvSize	m_minSizeMouth;

	bool	m_doCannyPruning;
	double	m_haarScaleFactor;

	CvHaarClassifierCascade		*m_cascadeFace;
	CvHaarClassifierCascade		*m_cascadeEyes;
	CvHaarClassifierCascade		*m_cascadeNose;
	CvHaarClassifierCascade		*m_cascadeMouth;

	CvMemStorage	*m_cvStorage;

	bool			m_greyImgOnlyHeader;
	float			m_greyImgSmlScale;
	unsigned int	m_conversionFlag;

	IplImage	*m_sourceImg;
	IplImage	*m_greyImg;
	IplImage	*m_greyImgSml;

	unsigned int		m_sourceImageWidth;
	unsigned int		m_sourceImageHeight;
	size_t				m_sourceImageBPC;
	ImageChannelOrder	m_sourceImageCO;
	unsigned int		m_sourceImageNOC;
	ImageType			m_souceImageType;

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
		cvEqualizeHist( m_greyImgSml, m_greyImgSml );

		return true;
	}

	void detect( CvRectVector &vec, CvHaarClassifierCascade *cascade, unsigned int minNeighbours, const CvSize &minSize, const CvRect *roi = NULL )
	{
		if( roi && ( roi->x >= m_greyImgSml->width || roi->y >= m_greyImgSml->height || roi->x + roi->width <= 0 || roi->y + roi->height <= 0 ) )
		{
			std::cerr << "no overlap between ROI and image!" << std::endl;	//openCV haar detection algorithm would crash
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
/*
		//TODO: if trajectory is empty, find initial face by calculating confidence measure from different properties:
		// - how much is the face centered in the image
		// - how big is the face rect
		// accumulate confidence values until they reach a certain threshold, then stick to this face and hardly let go. 

		this->detectedFaces.clear();

		bool scanWholeImage = true;
		if( this->faceShapeList.size() )
		{
			for( Trajectory2DList::const_iterator itT = this->trajectories2D.begin(); itT != this->trajectories2D.end(); ++itT )
			{
				float confidence = ( *itT )->getConfidence();
				if( confidence < 0.5f )
					continue;

				float roiExtend = 1.0f - ( confidence - 0.5 ) * 2.0f;

				const Trajectory2D::EntryList &list = ( *itT )->getEntries();
			
				if( list.size() )
				{
					Vec2 size( list.back().width / this->faceScaleX, list.back().height / this->faceScaleY );
					size *= ( this->roiScale + roiExtend * 2 );

					Vec2 ul( list.back().v - size * 0.5f );

					this->roi = cvRect( ul[0], ul[1], size[0], size[1] );
					CvRect r = cvRect( ul[0] * this->scale, ul[1] * this->scale, size[0] * this->scale, size[1] * this->scale );

					this->collectFaces( &r );

					scanWholeImage = false;
				}
			}

			cvResetImageROI( this->greyImageSml );
		}

		if( scanWholeImage )
		{
			this->roi = cvRect( 0, 0, this->greyImage->width, this->greyImage->height );
			this->collectFaces();
		}

		LOG( LL_TRACE ) << "found " << this->detectedFaces.size() << " faces in total";

		//TODO: find solution for problem with tilted heads: only minor rotations are enough for 
		// the haar detection to fail
		//TODO: search for face profiles and push results to detecedFaces list
		//TODO: set ROI to detected face rect and search for mouth. store position and extends in 
		// faceshape class and send it to renderer
		*/
	}

	void convertCvRectToSpace2D( const CvRectVector &src, FeatureVector &dest )
	{
		dest.clear();

		for( CvRectVector::const_iterator it = src.begin(); it != src.end(); ++it )
		{
			//convert (downscaled) pixel coordinates to normalized values in range [0 1]
			float centerX = ( it->x + it->width * 0.5f ) / m_greyImgSml->width;
			float centerY = ( it->y + it->width * 0.5f ) / m_greyImgSml->height;
			float scaledWidth = (float)( it->width ) / m_greyImgSml->width;
			float scaledHeight = (float)( it->height ) / m_greyImgSml->height;

			dest.push_back( Space2D( 
					Vec2( centerX - scaledWidth * 0.5f, centerY - scaledHeight * 0.5f ), 
					Vec2( centerX + scaledWidth * 0.5f, centerY + scaledHeight * 0.5f ) ) );
		}
	}
};


FaceFeaturesBlock::FaceFeaturesBlock() : 
	Block(), 
	m_cvImpl( new CVImpl() )
{}

FaceFeaturesBlock::~FaceFeaturesBlock()
{
	if( m_cvImpl )
	{
		delete m_cvImpl;
		m_cvImpl = NULL;
	}
}

void FaceFeaturesBlock::setup( BlockHandle &block )
{
	try
	{
		std::cout << "SETUP facefeaturesblock" << std::endl;

		m_Block = block;

		m_imageIn = m_Block.getInletHandle( "image_in" );

		m_useFaceIn = m_Block.getInletHandle( "use_face" );
		m_useEyesIn = m_Block.getInletHandle( "use_eyes" );
		m_useNoseIn = m_Block.getInletHandle( "use_nose" );
		m_useMouthIn = m_Block.getInletHandle( "use_mouth" );

		m_haarMinNeighboursFaceIn = m_Block.getInletHandle( "haar_min_neighbours_face" );
		m_haarMinNeighboursEyesIn = m_Block.getInletHandle( "haar_min_neighbours_eyes" );
		m_haarMinNeighboursNoseIn = m_Block.getInletHandle( "haar_min_neighbours_nose" );
		m_haarMinNeighboursMouthIn = m_Block.getInletHandle( "haar_min_neighbours_mouth" );

		m_haarMinSizeFaceIn = m_Block.getInletHandle( "haar_minSizeFace" );
		m_haarMinSizeEyesIn = m_Block.getInletHandle( "haar_min_size_eyes" );
		m_haarMinSizeNoseIn = m_Block.getInletHandle( "haar_min_size_nose" );
		m_haarMinSizeMouthIn = m_Block.getInletHandle( "haar_min_size_mouth" );

		m_haarDoCannyPruningIn = m_Block.getInletHandle( "haar_do_canny_pruning" );
		m_haarScaleFactorIn = m_Block.getInletHandle( "haar_scalefactor" );

		m_cascFileFaceIn = m_Block.getInletHandle( "cascadefile_face" );
		m_cascFileEyesIn = m_Block.getInletHandle( "cascadefile_eyes" );
		m_cascFileNoseIn = m_Block.getInletHandle( "cascadefile_nose" );
		m_cascFileMouthIn = m_Block.getInletHandle( "cascadefile_mouth" );

		m_faceOut = m_Block.getOutletHandle( "face_out" );
		m_eyesOut = m_Block.getOutletHandle( "eyes_out" );
		m_noseOut = m_Block.getOutletHandle( "nose_out" );
		m_mouthOut = m_Block.getOutletHandle( "mouth_out" );

		const Image &image = m_imageIn.getReadableRef< Image >();

		m_cvImpl->loadFaceCascade( m_cascFileFaceIn.getReadableRef< std::string >() );
		m_cvImpl->loadEyesCascade( m_cascFileEyesIn.getReadableRef< std::string >() );
		m_cvImpl->loadNoseCascade( m_cascFileNoseIn.getReadableRef< std::string >() );
		m_cvImpl->loadMouthCascade( m_cascFileMouthIn.getReadableRef< std::string >() );

		m_cvImpl->useFace( m_useFaceIn.getReadableRef< bool >() );
		m_cvImpl->useEyes( m_useEyesIn.getReadableRef< bool >() );
		m_cvImpl->useNose( m_useNoseIn.getReadableRef< bool >() );
		m_cvImpl->useMouth( m_useMouthIn.getReadableRef< bool >() );

		m_cvImpl->minNeighboursFace( m_haarMinNeighboursFaceIn.getReadableRef< unsigned int >() );
		m_cvImpl->minNeighboursEyes( m_haarMinNeighboursEyesIn.getReadableRef< unsigned int >() );
		m_cvImpl->minNeighboursNose( m_haarMinNeighboursNoseIn.getReadableRef< unsigned int >() );
		m_cvImpl->minNeighboursMouth( m_haarMinNeighboursMouthIn.getReadableRef< unsigned int >() );

		m_cvImpl->minSizeFace( m_haarMinSizeFaceIn.getReadableRef< Vec2 >() );
		m_cvImpl->minSizeEyes( m_haarMinSizeEyesIn.getReadableRef< Vec2 >() );
		m_cvImpl->minSizeNose( m_haarMinSizeNoseIn.getReadableRef< Vec2 >() );
		m_cvImpl->minSizeMouth( m_haarMinSizeMouthIn.getReadableRef< Vec2 >() );

		m_cvImpl->doCannyPruning( m_haarDoCannyPruningIn.getReadableRef< bool >() );
		m_cvImpl->haarScaleFactor( m_haarScaleFactorIn.getReadableRef< double >() );

		if( !m_cvImpl->createImages( image ) )
			std::cerr << "warning: creating initial cv images failed" << std::endl;
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}

void FaceFeaturesBlock::update()
{
	try
	{
		Bench bench( "update facefeatures block" );

		const Image &image = m_imageIn.getReadableRef< Image >();

		bool doUpdate = m_imageIn.hasUpdated();

		if( m_cascFileFaceIn.hasChanged() )
		{
			m_cvImpl->loadFaceCascade( m_cascFileFaceIn.getReadableRef< std::string >() );
			doUpdate = true;
		}
		if( m_cascFileEyesIn.hasChanged() )
		{
			m_cvImpl->loadEyesCascade( m_cascFileEyesIn.getReadableRef< std::string >() );
			doUpdate = true;
		}
		if( m_cascFileNoseIn.hasChanged() )
		{
			m_cvImpl->loadNoseCascade( m_cascFileNoseIn.getReadableRef< std::string >() );
			doUpdate = true;
		}
		if( m_cascFileMouthIn.hasChanged() )
		{
			m_cvImpl->loadMouthCascade( m_cascFileMouthIn.getReadableRef< std::string >() );
			doUpdate = true;
		}

		if( m_haarMinNeighboursFaceIn.hasChanged() )
		{
			m_cvImpl->minNeighboursFace( m_haarMinNeighboursFaceIn.getReadableRef< unsigned int >() );
			doUpdate = true;
		}
		if( m_haarMinNeighboursEyesIn.hasChanged() )
		{
			m_cvImpl->minNeighboursEyes( m_haarMinNeighboursEyesIn.getReadableRef< unsigned int >() );
			doUpdate = true;
		}
		if( m_haarMinNeighboursNoseIn.hasChanged() )
		{
			m_cvImpl->minNeighboursNose( m_haarMinNeighboursNoseIn.getReadableRef< unsigned int >() );
			doUpdate = true;
		}
		if( m_haarMinNeighboursMouthIn.hasChanged() )
		{
			m_cvImpl->minNeighboursMouth( m_haarMinNeighboursMouthIn.getReadableRef< unsigned int >() );
			doUpdate = true;
		}

		if( m_haarMinSizeFaceIn.hasChanged() )
		{
			m_cvImpl->minSizeFace( m_haarMinSizeFaceIn.getReadableRef< Vec2 >() );
			doUpdate = true;
		}
		if( m_haarMinSizeEyesIn.hasChanged() )
		{
			m_cvImpl->minSizeEyes( m_haarMinSizeEyesIn.getReadableRef< Vec2 >() );
			doUpdate = true;
		}
		if( m_haarMinSizeNoseIn.hasChanged() )
		{
			m_cvImpl->minSizeNose( m_haarMinSizeNoseIn.getReadableRef< Vec2 >() );
			doUpdate = true;
		}
		if( m_haarMinSizeMouthIn.hasChanged() )
		{
			m_cvImpl->minSizeMouth( m_haarMinSizeMouthIn.getReadableRef< Vec2 >() );
			doUpdate = true;
		}

		if( m_haarScaleFactorIn.hasChanged() )
		{
			m_cvImpl->haarScaleFactor( m_haarScaleFactorIn.getReadableRef< double >() );
			doUpdate = true;
		}

		if( m_useFaceIn.hasChanged() )
		{
			m_cvImpl->useFace( m_useFaceIn.getReadableRef< bool >() );
			doUpdate = true;
		}
		if( m_useEyesIn.hasChanged() )
		{
			m_cvImpl->useEyes( m_useEyesIn.getReadableRef< bool >() );
			doUpdate = true;
		}
		if( m_useNoseIn.hasChanged() )
		{
			m_cvImpl->useNose( m_useNoseIn.getReadableRef< bool >() );
			doUpdate = true;
		}
		if( m_useMouthIn.hasChanged() )
		{
			m_cvImpl->useMouth( m_useMouthIn.getReadableRef< bool >() );
			doUpdate = true;
		}

		if( doUpdate )
		{
			FeatureVector &faces = m_faceOut.getWriteableRef< FeatureVector >();
			FeatureVector &eyes = m_eyesOut.getWriteableRef< FeatureVector >();
			FeatureVector &noses = m_noseOut.getWriteableRef< FeatureVector >();
			FeatureVector &mouths = m_mouthOut.getWriteableRef< FeatureVector >();

			faces.clear();
			eyes.clear();
			noses.clear();
			mouths.clear();

			/*
			//TODO: track faces
			faces.push_back( Space2D( Vec2( 0.2f, 0.2f ), Vec2( 0.45f, 0.75f ) ) );
			faces.push_back( Space2D( Vec2( 0.7f, 0.2f ), Vec2( 0.95f, 0.65f ) ) );
			*/

			m_cvImpl->detect( image, faces, eyes, noses, mouths );

			//TODO: run plausability check (discard eyes on bottom of face, mouth up at forehead...)
			//TODO: run coherency check by calculating distance measures based on change of angle,
			// speed and size (threshold framerate-dependent) and assign to userID accordingly. use
			// video file for proper testing

			if( !m_cvImpl->useFace() )
				m_faceOut.discard();
			if( !m_cvImpl->useEyes() )
				m_eyesOut.discard();
			if( !m_cvImpl->useNose() )
				m_noseOut.discard();
			if( !m_cvImpl->useMouth() )
				m_mouthOut.discard();
		}
		else
		{
			m_faceOut.discard();
			m_eyesOut.discard();
			m_noseOut.discard();
			m_mouthOut.discard();
		}
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
	}
}

void FaceFeaturesBlock::shutdown() {}