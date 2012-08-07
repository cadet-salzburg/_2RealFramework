#include "_2RealDatatypes.h"
#include "FaceFeaturesBlock.h"

#include "opencv/cv.h"

#include <iostream>

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;

class CVImpl
{
public:
	CVImpl() :
		m_useFace( false ),
		m_useEyes( false ),
		m_useNose( false ),
		m_useMouth( false ),
		m_cascadeFace( NULL ),
		m_cascadeEyes( NULL ),
		m_cascadeNose( NULL ),
		m_cascadeMouth( NULL ),
		m_cvStorage( NULL ),
		m_greyImgOnlyHeader( false ),
		m_greyImgSmlScale( 1.0f ),
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
		m_cascadeFace = (CvHaarClassifierCascade*)cvLoad( file.c_str() );	
	}

	void loadEyesCascade( const std::string &file )
	{
		releaseEyesCascade();
		m_cascadeEyes = (CvHaarClassifierCascade*)cvLoad( file.c_str() );
	}

	void loadNoseCascade( const std::string &file )
	{
		releaseNoseCascade();
		m_cascadeNose = (CvHaarClassifierCascade*)cvLoad( file.c_str() );
	}

	void loadMouthCascade( const std::string &file )
	{
		releaseMouthCascade();
		m_cascadeMouth = (CvHaarClassifierCascade*)cvLoad( file.c_str() );
	}

	void useFace( bool use )		{	m_useFace = use;	}
	void useEyes( bool use )		{	m_useEyes = use;	}
	void useNose( bool use )		{	m_useNose = use;	}
	void useMouth( bool use )		{	m_useMouth = use;	}

	void createImages( const _2Real::Image &img )
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

		m_sourceImageWidth = img.getWidth();
		m_sourceImageHeight = img.getHeight();
		m_sourceImageBPC = img.getBitsPerChannel();
		m_sourceImageCO = img.getChannelOrder();
		m_sourceImageNOC = img.getNumberOfChannels();
		m_souceImageType = img.getImageType();

		m_sourceImg = cvCreateImageHeader( cvSize( img.getWidth(), img.getHeight() ), img.getBitsPerChannel(), img.getNumberOfChannels() );

		m_greyImgOnlyHeader = ( img.getNumberOfChannels() == 1 && img.getImageType() == ImageType::UNSIGNED_BYTE );
		m_greyImg = ( m_greyImgOnlyHeader ? 
			cvCreateImageHeader( cvSize( img.getWidth(), img.getHeight() ), IPL_DEPTH_8U, 1 ) : 
			cvCreateImage( cvSize( img.getWidth(), img.getHeight() ), IPL_DEPTH_8U, 1 ) );

		//TODO: reallocate if according inlet changes
		m_greyImgSml = cvCreateImage( cvSize( cvRound( img.getWidth() * m_greyImgSmlScale ), cvRound( img.getHeight() * m_greyImgSmlScale ) ), 8, 1 );
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
		if( m_greyImg )
		{
			cvReleaseImage( &m_greyImg );
			m_greyImg = NULL;
		}
	}

	void detect( const _2Real::Image &img )
	{
		makeGreyImages( img );
		/*
		cvResize( this->greyImage, this->greyImageSml, CV_INTER_LINEAR );
		cvEqualizeHist( this->greyImageSml, this->greyImageSml );

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

private:
	bool	m_useFace;
	bool	m_useEyes;
	bool	m_useNose;
	bool	m_useMouth;

	CvHaarClassifierCascade		*m_cascadeFace;
	CvHaarClassifierCascade		*m_cascadeEyes;
	CvHaarClassifierCascade		*m_cascadeNose;
	CvHaarClassifierCascade		*m_cascadeMouth;

	CvMemStorage	*m_cvStorage;

	bool	m_greyImgOnlyHeader;
	float	m_greyImgSmlScale;

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

	void makeGreyImages( const _2Real::Image &img )
	{
		if( img.getWidth() != m_sourceImageWidth ||
			img.getHeight() != m_sourceImageHeight ||
			img.getBitsPerChannel() != m_sourceImageBPC ||
			img.getChannelOrder() != m_sourceImageCO ||
			img.getNumberOfChannels() != m_sourceImageNOC ||
			img.getImageType() != m_souceImageType )
		{
			createImages( img );
		}

		//if greyimage only header -> simply copy source ptr
		//otherwise convert to greyscale unsigned char
		//downscale greyscale image		
	}
};


FaceFeaturesBlock::FaceFeaturesBlock() : 
	Block(), 
	m_numChannels( 0 ),
	m_imageType( ImageType::UNSIGNED_BYTE ),
	m_channelOrder( ImageChannelOrder::RGB ),
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
		m_useMouthIn = m_Block.getInletHandle( "use_mout" );

		m_cascFileFaceIn = m_Block.getInletHandle( "cascadefile_face" );
		m_cascFileEyesIn = m_Block.getInletHandle( "cascadefile_eyes" );
		m_cascFileNoseIn = m_Block.getInletHandle( "cascadefile_nose" );
		m_cascFileMouthIn = m_Block.getInletHandle( "cascadefile_mouth" );

		m_facesOut = m_Block.getOutletHandle( "faces_out" );
		
		const Image &image = m_imageIn.getReadableRef< Image >();

		m_numChannels = image.getNumberOfChannels();
		m_imageType = image.getImageType();
		m_channelOrder = image.getChannelOrder();

		std::cout << "number of channels: " << m_numChannels << std::endl;
		std::cout << "imageType : " << m_imageType << std::endl;
		std::cout << "channel order: " << m_channelOrder << std::endl;

		m_cvImpl->loadFaceCascade( m_cascFileFaceIn.getReadableRef< std::string >() );
		m_cvImpl->loadEyesCascade( m_cascFileEyesIn.getReadableRef< std::string >() );
		m_cvImpl->loadNoseCascade( m_cascFileNoseIn.getReadableRef< std::string >() );
		m_cvImpl->loadMouthCascade( m_cascFileMouthIn.getReadableRef< std::string >() );

		m_cvImpl->useFace( m_useFaceIn.getReadableRef< bool >() );
		m_cvImpl->useEyes( m_useEyesIn.getReadableRef< bool >() );
		m_cvImpl->useNose( m_useNoseIn.getReadableRef< bool >() );
		m_cvImpl->useMouth( m_useMouthIn.getReadableRef< bool >() );

		m_cvImpl->createImages( image );
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
		const Image &image = m_imageIn.getReadableRef< Image >();

		if( image.getNumberOfChannels() != m_numChannels )
		{
			std::cout << "number of channels changed from " << m_numChannels << " to " << image.getNumberOfChannels() << std::endl;
			m_numChannels = image.getNumberOfChannels();
		}

		if( image.getImageType() != m_imageType )
		{
			std::cout << "imageType changed from " << m_imageType << " to " << image.getImageType() << std::endl;
			m_imageType = image.getImageType();
		}

		if( image.getChannelOrder() != m_channelOrder )
		{
			std::cout << "channel order changed from " << m_channelOrder << " to " << image.getChannelOrder() << std::endl;
			m_channelOrder = image.getChannelOrder();
		}

		//TODO: call if inlest changed
		/*
		m_cvImpl->loadFaceCascade( m_cascFileFaceIn.getReadableRef< std::string >() );
		m_cvImpl->loadEyesCascade( m_cascFileEyesIn.getReadableRef< std::string >() );
		m_cvImpl->loadNoseCascade( m_cascFileNoseIn.getReadableRef< std::string >() );
		m_cvImpl->loadMouthCascade( m_cascFileMouthIn.getReadableRef< std::string >() );

		m_cvImpl->useFace( m_useFaceIn.getReadableRef< bool >() );
		m_cvImpl->useEyes( m_useEyesIn.getReadableRef< bool >() );
		m_cvImpl->useNose( m_useNoseIn.getReadableRef< bool >() );
		m_cvImpl->useMouth( m_useMouthIn.getReadableRef< bool >() );
		*/

		std::vector< Space2D > &faces = m_facesOut.getWriteableRef< std::vector< Space2D > >();
		faces.clear();

		//TODO: track faces
		faces.push_back( Space2D( Vec2( 0.2f, 0.2f ), Vec2( 0.45f, 0.75f ) ) );
		faces.push_back( Space2D( Vec2( 0.7f, 0.2f ), Vec2( 0.95f, 0.65f ) ) );

		if( !faces.size() )
			m_facesOut.discard();
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