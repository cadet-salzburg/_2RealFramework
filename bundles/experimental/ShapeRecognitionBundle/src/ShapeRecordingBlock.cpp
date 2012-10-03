#include "_2RealDatatypes.h"
#include "ShapeRecordingBlock.h"

#include "highgui/highgui.hpp"
#include "BlobResult.h"
#include "dirent.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;

class ShapeRecordingBlockImpl
{
public:
	ShapeRecordingBlockImpl() : 
		m_isShutDown( false ),
		m_minBlobSize( 10000)
	{}

	~ShapeRecordingBlockImpl()
	{
		if( !m_isShutDown )
			this->shutdown();
	}

	void shutdown()
	{
		m_isShutDown = true;
	}

	bool findBiggestBlobImage(IplImage* img, int color, IplImage* &output)
	{
		CBlobResult blobs;
		CBlob *currentBlob;

		blobs = CBlobResult( img, NULL, 0 );
		blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, m_minBlobSize );

		double biggestArea = m_minBlobSize;
		int biggestBlob = -1;

		for (int i = 0; i < blobs.GetNumBlobs(); i++ )
		{
			currentBlob = blobs.GetBlob(i);
			double blobArea = currentBlob->Area();
			if(blobArea > biggestArea) 
			{
				biggestBlob = i;
				biggestArea = blobArea;
			}
		}

		if(biggestBlob >= 0)
		{
			int x = (int) blobs.GetBlob(biggestBlob)->MinX();
			int y = (int) blobs.GetBlob(biggestBlob)->MinY();
			int width= (int) blobs.GetBlob(biggestBlob)->MaxX()-x;
			int height= (int) blobs.GetBlob(biggestBlob)->MaxY()-y;

			IplImage* temp = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U, 1);
			IplImage* temp2 = cvCreateImage(cvSize(width, height),IPL_DEPTH_8U, 1);
			IplImage* result = cvCreateImage(cvSize(width, height),IPL_DEPTH_8U, 1);

			if(biggestBlob>=0) blobs.GetBlob(biggestBlob)->FillBlob(temp,cvScalar(255),x,y);

			cvSetImageROI(temp, cvRect(x, y, width, height));

			cvCopy(temp,temp2);

			uchar* tempData;

			uchar* resultData;

			tempData = (uchar *)(temp2->imageData);
			resultData = (uchar *) (result->imageData);

			for (int j = 0; j < width*height; j++)
			{
				if (tempData[j]==255) resultData[j] = color;
				else	resultData[j] = 0;
			}

			cvResize(result, output);

			cvReleaseImage(&temp);
			cvReleaseImage(&temp2);
			cvReleaseImage(&result);

			return true;
		}
		else
			return false;
	}
	
	int getNumberOfFiles (const char *path, const char *type)
	{
		int counter = 0;
		DIR *pdir = NULL; 
		pdir = opendir (path);
		struct dirent *pent = NULL;
		if (pdir == NULL)
		{
	        printf ("\nERROR! Directory could not be initialised correctly");
			return -1;
		}

		while (pent = readdir (pdir))
		{
			if (pent == NULL)
			{
	            printf ("\nERROR! File could not be initialised correctly");
		        return -1;
			}
			string filename = pent->d_name;
			size_t pos = filename.find(".");
			string extension = filename.substr(pos+1);
			if (extension == type) counter++;
		}
		closedir (pdir);
		return counter;
	}
	
	int getPixelLine(IplImage img, CvPoint pt)
	{
		CvLineIterator iterator;
		int sum = 0;
		int li = cvInitLineIterator(&img, cvPoint(100,100), pt, &iterator, 8 );
		for( int i = 0; i < li; i++ )
		{
			if(iterator.ptr[0]>127)
			sum++;
			CV_NEXT_LINE_POINT(iterator);
		}
		return sum;
	}

	int getCurrentLines(int* &line, IplImage* blub)
	{
		int sum = 0;

		for(int i = 0; i < 9; i++)
		{			
			line[i+0] = getPixelLine(*blub,cvPoint(i*25, 0));
			sum+= line[i+0];
			line[i+9] = getPixelLine(*blub,cvPoint(i*25, 200));
			sum+= line[i+9];
			line[i+18] = getPixelLine(*blub,cvPoint(0, i*25));
			sum+= line[i+18];
			line[i+27] = getPixelLine(*blub,cvPoint(200, i*25));
			sum+= line[i+27];
		}
		return sum;
	}

	void saveImage(Image img, string path, int maxDistance)
	{
		IplImage* one = cvCreateImage(cvSize(img.getWidth(), img.getHeight()), img.getBitsPerChannel(), img.getNumberOfChannels());
		IplImage* two = cvCreateImage(cvSize(img.getWidth(), img.getHeight()), img.getBitsPerChannel(), img.getNumberOfChannels());
		IplImage* image = cvCreateImage(cvSize(img.getWidth(), img.getHeight()), IPL_DEPTH_8U, 1);

		one->imageData = (char*)img.getData();

		uchar* src = (uchar*) one->imageData;
		uchar* dst = (uchar*) two->imageData;

		float thresh = maxDistance/65535.f*255.f;

		for (int j = 0; j < one->imageSize; j++)
		{
			float diff = src[j];		
			
			if (diff > thresh) diff = 0.f;

            dst[j] = diff;		
		}

		cvConvertScale(two, image);

		IplImage* blub = cvCreateImage(cvSize(200,200),IPL_DEPTH_8U, 1);

		if(findBiggestBlobImage(image, 255, blub))
		{
			int number = getNumberOfFiles(path.c_str(), "png")+1;

			int *lines = NULL;
			lines = new int[36];

			getCurrentLines(lines,blub);

			char buffer [33];
			itoa(number, buffer, 10);

			string filename = path;
			filename.append("/pose");
			filename.append(buffer);
			filename.append(".png");

			cvSaveImage(filename.c_str(), blub);

			filename = path;
			filename.append("/data");
			filename.append(buffer);
			filename.append(".txt");
		
			ofstream myfile;
			myfile.open(filename);
			if (myfile.is_open())
			{
				for(int i=0; i < 36; i++)
				{
					myfile << "l " << lines[i] << endl;
				}
				myfile.close();
			}
			else cout << "Unable to open file";

		printf("Image saved.\n");

		cvReleaseImage(&one);
		cvReleaseImage(&two);
		cvReleaseImage(&image);
		cvReleaseImage(&blub);
		}
		else printf("Unable to aquire the required data, please try again.\n");

	}

private:
	bool	m_isShutDown;
	int		m_minBlobSize;
};

ShapeRecordingBlock::ShapeRecordingBlock() : 
Block(),
	m_blockImpl( new ShapeRecordingBlockImpl() ),
	saving ( false ),
	saveCounter ( 0 )
{
	std::cout << "ShapeRecordingblock .ctor" << std::endl;
}

ShapeRecordingBlock::~ShapeRecordingBlock()
{
	std::cout << "ShapeRecordingblock .dtor" << std::endl;

	if( m_blockImpl )
	{
		delete m_blockImpl;
		m_blockImpl = NULL;
	}
}

void ShapeRecordingBlock::setup( BlockHandle &block )
{
	try
	{
		m_Block = block;

		// inlet handles
		m_depthImageIn = m_Block.getInletHandle("depth_image");
		m_outputPathIn = m_Block.getInletHandle("output_path");
		m_countTimeIn = m_Block.getInletHandle("count_time");
		m_maxDistanceIn = m_Block.getInletHandle("max_distance");
		m_saveIn =		 m_Block.getInletHandle("save");

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

void ShapeRecordingBlock::update()
{
	try
	{
		if( m_depthImageIn.hasUpdated() && m_saveIn.hasChanged() && m_saveIn.getReadableRef< bool >())
		{
			saving = true;
		}

		int timer = m_countTimeIn.getReadableRef<int>();

		if(saving)
		{
			saveCounter++;
			if(saveCounter%30 == 0)
			{
				cout << saveCounter/30 << endl;
			}
		}

		if(saveCounter >= timer*30)
		{
			const Image &image = m_depthImageIn.getReadableRef< Image >();

			int maxDist = m_maxDistanceIn.getReadableRef< int >();

			std::string path = m_outputPathIn.getReadableRef< string >();

			if( m_blockImpl )
			{			
				m_blockImpl->saveImage(image, path, maxDist);
			}
			saveCounter = 0;
			saving = false;
		}		
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
}

void ShapeRecordingBlock::shutdown() {

}